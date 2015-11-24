/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
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

#include <ql/pricingengines/swap/discretizedcoterminalswapstrip.hpp>

namespace QuantLib {

    const DiscretizedSwap& DiscretizedCoterminalSwapStrip::swap(size_t i) const {
        return *(coterminalStrip_[i]);
    }

    DiscretizedCoterminalSwapStrip::DiscretizedCoterminalSwapStrip(const VanillaSwap::arguments& args,
        const Date& referenceDate,
        const DayCounter& dayCounter,
        const std::vector<Date>& entryDates)
        : arguments_(args), coterminalStrip_(entryDates.size()) {

        for (size_t i = 0; i < entryDates.size(); ++i) {
            coterminalStrip_[i] = boost::shared_ptr<DiscretizedSwap>(new DiscretizedSwap(args, referenceDate, dayCounter, entryDates[i]));
        }
    }

    void DiscretizedCoterminalSwapStrip::reset(Size size) {
        values_ = Array(size, 0.0);
        for (size_t i = 0; i < coterminalStrip_.size(); ++i) {
            coterminalStrip_[i]->initialize(method(), time());
        }
        adjustValues();
            
    }

    std::vector<Time> DiscretizedCoterminalSwapStrip::mandatoryTimes() const {
        return coterminalStrip_[0]->mandatoryTimes();
    }

    void DiscretizedCoterminalSwapStrip::preAdjustValuesImpl(Time entryTime) {
        for (size_t i = 0; i < coterminalStrip_.size(); ++i) {
            coterminalStrip_[i]->preAdjustValuesImpl(entryTime);
        }
    }

    void DiscretizedCoterminalSwapStrip::postAdjustValuesImpl(Time) {
        for (size_t i = 0; i < coterminalStrip_.size(); ++i) {
            coterminalStrip_[i]->postAdjustValuesImpl(Time());
        }
    }

}
