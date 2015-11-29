/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
Copyright (C) 2004 Ferdinando Ametrano

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

/*! \file additionalresultcalculators.hpp
\brief A class to accumulate additional result calculations from a pricing engine.
*/

#ifndef quantlib_additionalresultcalculators_h
#define quantlib_additionalresultcalculators_h

#include <map>
#include <boost/any.hpp>

namespace QuantLib {
	class DiscretizedAsset;
	struct AdditionalResultCalculator {
		AdditionalResultCalculator();

		std::map<std::string, boost::any> additionalResults() const;

		virtual void calculateAdditionalResults() ;

		virtual void setupDiscretizedAsset(const boost::shared_ptr<DiscretizedAsset>& );

		virtual ~AdditionalResultCalculator();
	protected:
		std::map<std::string, boost::any> additionalResults_;
	};

	
}
#endif


