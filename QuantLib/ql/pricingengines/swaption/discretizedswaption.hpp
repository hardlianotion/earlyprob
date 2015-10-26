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

/*! \file discretizedswaption.hpp
    \brief Discretized swaption class
*/

#ifndef quantlib_discretized_swaption_hpp
#define quantlib_discretized_swaption_hpp

#include <ql/instruments/swaption.hpp>
#include <ql/discretizedasset.hpp>

namespace QuantLib {

    class DiscretizedSwaption : public DiscretizedOption {
      public:
        DiscretizedSwaption(const Swaption::arguments&,
                            const Date& referenceDate,
                            const DayCounter& dayCounter);
        void reset(Size size);
        const boost::shared_ptr<std::vector<std::pair<bool, size_t> > > exerciseIndex() const;
		const std::vector<Date>& exerciseDates() const;
	protected:
		void applyExerciseCondition();
      private:
        Swaption::arguments arguments_;
        Time lastPayment_;
		Array exerciseMargins_;
		boost::shared_ptr<std::vector<std::pair<bool, size_t> > > exerciseIndex_;
    };

}


#endif

