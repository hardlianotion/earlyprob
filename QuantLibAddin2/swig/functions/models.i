
%pragma(reposit) group="models";

%pragma(reposit) obj_include=%{
#include <ql/models/shortrate/calibrationhelpers/swaptionhelper.hpp>
#include <ql/models/shortrate/onefactormodels/hullwhite.hpp>
%}

namespace QuantLib {

    class CalibrationHelper {
      public:
        void setPricingEngine(const boost::shared_ptr<QuantLib::PricingEngine>& engine);
    };

    class SwaptionHelper : public CalibrationHelper {
      public:
        SwaptionHelper(const QuantLib::Period& maturity,
                       const QuantLib::Period& length,
                       const QuantLib::Handle<QuantLib::Quote>& volatility,
                       const boost::shared_ptr<QuantLib::IborIndex>& index,
                       const QuantLib::Period& fixedLegTenor,
                       const QuantLib::DayCounter& fixedLegDayCounter,
                       const QuantLib::DayCounter& floatingLegDayCounter,
                       const QuantLib::Handle<QuantLib::YieldTermStructure>& termStructure/*,
                       QuantLib::CalibrationHelper::CalibrationErrorType errorType
                                      = QuantLib::CalibrationHelper::RelativePriceError,
                       const QuantLib::Real strike = QuantLib::Null<QuantLib::Real>(),
                       const QuantLib::Real nominal = 1.0,
                       const QuantLib::Real shift = 0.0*/);
        };
        
    class CalibratedModel {
      public:
        virtual void calibrate(
                const std::vector<boost::shared_ptr<QuantLib::CalibrationHelper> >& x,
                QuantLib::OptimizationMethod& method,
                const QuantLib::EndCriteria& endCriteria/*,
                const Constraint& constraint = Constraint(),
                const std::vector<Real>& weights = std::vector<Real>(),
                const std::vector<bool>& fixParameters = std::vector<bool>()*/);
    };
    
    class OneFactorAffineModel : public CalibratedModel {};
    
    class HullWhite : public /*Vasicek, public TermStructureConsistentModel*/OneFactorAffineModel {
      public:
        HullWhite(const QuantLib::Handle<QuantLib::YieldTermStructure>& termStructure/*,
                  Real a = 0.1, Real sigma = 0.01*/);
        };        
}
