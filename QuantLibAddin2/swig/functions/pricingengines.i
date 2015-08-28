
%feature("rp:group", "pricingengines");
%feature("rp:obj_include") %{
#include <ql/pricingengines/vanilla/analyticeuropeanengine.hpp>
%}
%feature("rp:add_include") "#include \"qlo/obj_processes.hpp\""

%feature("rp:generate_countify") QuantLib::AnalyticEuropeanEngine::AnalyticEuropeanEngine;

namespace QuantLib {

    class PricingEngine {};
    
    class AnalyticEuropeanEngine : public PricingEngine {
      public:
        AnalyticEuropeanEngine(
            const boost::shared_ptr<QuantLib::GeneralizedBlackScholesProcess>& process);
    };
}

%feature("rp:obj_include", "");
%feature("rp:group", "");

