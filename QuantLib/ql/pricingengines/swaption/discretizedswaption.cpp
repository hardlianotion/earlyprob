/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2001, 2002, 2003 Sadruddin Rejeb
 Copyright (C) 2004, 2007 StatPro Italia srl

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it
 under the terms of the QuantLib license.  You should have received a
 copy of the license along with this program; if not, please email
 <quantlib-dev@lists.sf.net>. The license is also available online at
 <http://quantlib.org/license.shtml>.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

#include <ql/pricingengines/swaption/discretizedswaption.hpp>
#include <ql/pricingengines/swap/discretizedswap.hpp>

namespace QuantLib {

    namespace {

        bool withinPreviousWeek(const Date& d1, const Date& d2) {
            return d2 >= d1-7 && d2 <= d1;
        }

        bool withinNextWeek(const Date& d1, const Date& d2) {
            return d2 >= d1 && d2 <= d1+7;
        }

    }

    DiscretizedSwaption::DiscretizedSwaption(const Swaption::arguments& args,
                                             const Date& referenceDate,
                                             const DayCounter& dayCounter)
    : DiscretizedOption(boost::shared_ptr<DiscretizedAsset>(),
                        args.exercise->type(),
                        std::vector<Time>()),
      arguments_(args),exerciseIndex_(new std::vector<std::pair<bool, std::pair<Size, Real> > >) {

        exerciseTimes_.resize(arguments_.exercise->dates().size());
        for (Size i=0; i<exerciseTimes_.size(); ++i)
            exerciseTimes_[i] =
                dayCounter.yearFraction(referenceDate,
                                        arguments_.exercise->date(i));

        // Date adjustments can get time vectors out of synch.
        // Here, we try and collapse similar dates which could cause
        // a mispricing.
        for (Size i=0; i<arguments_.exercise->dates().size(); i++) {
            Date exerciseDate = arguments_.exercise->date(i);
            for (Size j=0; j<arguments_.fixedPayDates.size(); j++) {
                if (withinNextWeek(exerciseDate,
                                   arguments_.fixedPayDates[j])
                    // coupons in the future are dealt with below
                    && arguments_.fixedResetDates[j] < referenceDate)
                    arguments_.fixedPayDates[j] = exerciseDate;
            }
            for (Size j=0; j<arguments_.fixedResetDates.size(); j++) {
                if (withinPreviousWeek(exerciseDate,
                                       arguments_.fixedResetDates[j]))
                    arguments_.fixedResetDates[j] = exerciseDate;
            }
            for (Size j=0; j<arguments_.floatingResetDates.size(); j++) {
                if (withinPreviousWeek(exerciseDate,
                                       arguments_.floatingResetDates[j]))
                    arguments_.floatingResetDates[j] = exerciseDate;
            }
        }

        Time lastFixedPayment =
            dayCounter.yearFraction(referenceDate,
                                    arguments_.fixedPayDates.back());
        Time lastFloatingPayment =
            dayCounter.yearFraction(referenceDate,
                                    arguments_.floatingPayDates.back());
        lastPayment_ = std::max(lastFixedPayment,lastFloatingPayment);

        underlying_ = boost::shared_ptr<DiscretizedAsset>(
                                            new DiscretizedSwap(arguments_,
                                                                referenceDate,
                                                                dayCounter));
        underlyingAsSwap_ = boost::dynamic_pointer_cast<DiscretizedSwap>(underlying_);
        QL_REQUIRE(underlyingAsSwap_ != nullptr, "Underlying must be a DiscreteSwap object.");
    }

	const boost::shared_ptr<std::vector<std::pair<bool, std::pair<Size, Real> > > > DiscretizedSwaption::exerciseIndex() const {
		return exerciseIndex_;
	}

	void DiscretizedSwaption::reset(Size size) {
		underlying_->initialize(method(), lastPayment_);
		DiscretizedOption::reset(size);
	}

	const std::vector<Date>& DiscretizedSwaption::exerciseDates() const {
		return arguments_.exercise->dates();
	}

    void DiscretizedSwaption::postAdjustValuesImpl(Time t) {
        /* In the real world, with time flowing forward, first
        any payment is settled and only after options can be
        exercised. Here, with time flowing backward, options
        must be exercised before performing the adjustment.
        */
        underlying_->partialRollback(time());
        underlying_->resetCachedValues(underlying_->values());
        Size i;
        switch (exerciseType_) {
        case Exercise::American:
            if (time_ >= exerciseTimes_[0] && time_ <= exerciseTimes_[1]) {
                underlying_->values() = cachedValues();
                underlying_->preAdjustValues(t);
                applyExerciseCondition(t);
            }
            break;
        case Exercise::Bermudan:
        case Exercise::European:
            for (i = 0; i<exerciseTimes_.size(); i++) {
                if (t >= 0.0 && isOnTime(t)) {
                    Time t = exerciseTimes_[i];
                    underlying_->values() = cachedValues();
                    underlying_->preAdjustValues(t);
                    applyExerciseCondition(t);
                }
            }
            break;
        default:
            QL_FAIL("invalid exercise type");
        }
        underlying_->postAdjustValues();
    }

	void DiscretizedSwaption::applyExerciseCondition(Time exerciseTime) {
		std::pair<bool, std::pair<size_t, double> > exercised = std::make_pair(false, std::make_pair(0, 0.0) );
        exerciseMargins_ = Array(values_.size());
		for (Size i = 0; i < values_.size(); i++) {
			exerciseMargins_[i] = underlyingAsSwap_->values()[i] - values_[i];
            values_[i] = std::max(underlyingAsSwap_->values()[i], values_[i]);
			if (!exercised.first && exerciseMargins_[i] > 0.0) {
				exercised.first = true;
				exercised.second = std::make_pair(i, underlyingAsSwap_->impliedSwapRate(exerciseTime, i));
				exerciseIndex_->push_back(exercised);
			}
		}
		if (!exercised.first) {
			exerciseIndex_->push_back(exercised);
		}
	}


}
