/*
Copyright (C) 2001, 2002, 2003 Sadruddin Rejeb
Copyright (C) 2005 StatPro Italia srl

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

#include <stack>
#include <boost/shared_ptr.hpp>
#include <ql/pricingengines/treecumulativeprobabilitycalculator1d.hpp> 
#include <ql/pricingengines/swaption/discretizedswaption.hpp>

namespace QuantLib{

	void TreeCumulativeProbabilityCalculator1D::setupDiscretizedAsset(const boost::shared_ptr<DiscretizedAsset>& asset) {
		const boost::shared_ptr<DiscretizedSwaption> swaption = boost::dynamic_pointer_cast<DiscretizedSwaption>(asset);
		if (swaption) {
			exerciseTimes_ = swaption->positiveExerciseTimes();
			setExerciseIndex(swaption->exerciseIndex());
			std::vector<Date> tempExercised = swaption->exerciseDates();
			int negativeDateCount = static_cast<int>(tempExercised.size()) - exerciseTimes_.size();

			QL_REQUIRE(negativeDateCount >= 0, "Exercise dates definition error.  The number of past exercise days should be positive by construction.");
			exerciseDates_.reserve(tempExercised.size());
			for (int i = tempExercised.size() - 1; i >= negativeDateCount; --i) {
				exerciseDates_[i] = tempExercised[i - negativeDateCount];
			}
		}
	}

	void TreeCumulativeProbabilityCalculator1D::setExerciseIndex(const std::vector<std::pair<bool, size_t> >& exerciseIndex) {
		exerciseIndex_ = exerciseIndex;
	}

	void TreeCumulativeProbabilityCalculator1D::calculateAdditionalResults() {
		std::map<Date, std::pair<double, double> > result;
		
		for (size_t i = 0; i < exerciseTimes_.size(); ++i) {
			result[exerciseDates_[i]] = exerciseProbability(exerciseTimes_[i]);
		}

		additionalResults_["ExerciseProbabilityAndSwapBoundary"] = result;
	}

	void TreeCumulativeProbabilityCalculator1D::setTree(const boost::shared_ptr<OneFactorModel::ShortRateTree>& tree) {
		tree_ = tree;
	}

	boost::shared_ptr<OneFactorModel::ShortRateTree> TreeCumulativeProbabilityCalculator1D::tree() const {
		return tree_;
	}
	//This tree constructs the total probability, for each exercise date, that the option remains unexercised.
	void TreeCumulativeProbabilityCalculator1D::computeCumulativeProbabilities() {
		const TimeGrid& times = tree_->timeGrid();
		std::stack<Size> exerciseTimeIndices;
		for (size_t exerciseId = exerciseTimes_.size() - 1; exerciseId >= 0; --exerciseId) {
			if (exerciseIndex_[exerciseId].first == true) {
				exerciseTimeIndices.push(times.index(exerciseTimes_[exerciseId]));
			}
		}
		
		Size exerciseTimeId = exerciseTimeIndices.top(); exerciseTimeIndices.pop();

		cumulativeProbs_.push_back(std::vector<std::pair<Real, Real> >(1, std::make_pair(1.0, 1.0)));
		std::vector<Size> exerciseLimits(times.size());
		
		for (Size timePt = 1; timePt < times.size(); ++timePt) {
			Size priceIdCount = tree_->size(timePt);
			if (timePt == exerciseTimeId) {
				priceIdCount = exerciseIndex_[exerciseTimeId].second;
				if (!exerciseTimeIndices.empty()) {
					exerciseTimeId = exerciseTimeIndices.top(); exerciseTimeIndices.pop();
				}
			}
			exerciseLimits[timePt] = priceIdCount;
			cumulativeProbs_.push_back(std::vector<std::pair<Real, Real> >(priceIdCount, std::make_pair(0.0, 0.0)));
		}
		for (Size timePt = 0; timePt < times.size(); ++timePt) {
			Size priceIdCount = exerciseLimits[timePt];
			
			for (Size priceId = 0; priceId < priceIdCount; ++priceId) {
				Real cumulant = 0.0;
				std::pair<Real, Real> parentProb = cumulativeProbs_[timePt][priceId];
				for (Size nodeId = 0; nodeId < tree_->size(1); ++nodeId) {
					double increment = tree_->probability(timePt, priceId, nodeId)*parentProb.second;
					cumulant += increment;
					cumulativeProbs_[timePt + 1][priceId] = std::make_pair(cumulant, increment);
				}
			}
		}
	}

	std::pair<Real, Real> TreeCumulativeProbabilityCalculator1D::exerciseProbability(Time exerciseDate) {
		QL_REQUIRE(!exerciseIndex_.empty(), "Index empty.  Must run pricing call before exerciseProbability().");
		Size exerciseDateId = tree_->timeGrid().index(exerciseDate);
		const std::pair<bool, Size>& exercise = exerciseIndex_[exerciseDateId];
		if (exercise.first == true) {
			
			return std::make_pair(1.0 - cumulativeProbs_[exerciseDateId][exercise.second].first, 0.0);
		}
		else {
			return std::make_pair(0.0, std::numeric_limits<double>::quiet_NaN());
		}
	}
}
