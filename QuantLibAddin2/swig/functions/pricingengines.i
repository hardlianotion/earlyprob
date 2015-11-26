
%group(pricingengines);

%insert(pricingengines_library_hpp) %{
#include <ql/pricingengines/vanilla/analyticeuropeanengine.hpp>
#include <ql/pricingengines/swap/discountingswapengine.hpp>
#include <ql/pricingengines/swaption/jamshidianswaptionengine.hpp>
#include <ql/pricingengines/swaption/treeswaptionengine.hpp>
#include <ql/pricingengines/swaption/g2swaptionengine.hpp>
// FIXME this belongs in a separate group.
#include <ql/pricingengines/treecumulativeprobabilitycalculator1d.hpp> 
%}

%insert(pricingengines_addin_cpp) %{
#include <qlo/obj_processes.hpp>
#include <qlo/obj_termstructures.hpp>
#include <qlo/obj_models.hpp>
%}

namespace QuantLib {

    // FIXME this belongs in a separate group.
	/*struct*/class AdditionalResultCalculator {};

    // FIXME this belongs in a separate group.
	/*struct*/class TreeCumulativeProbabilityCalculator1D : public AdditionalResultCalculator {
      public:
        %generate(c#, TreeCumulativeProbabilityCalculator1D);
		TreeCumulativeProbabilityCalculator1D();
	};
	
    class PricingEngine {};

    class AnalyticEuropeanEngine : public PricingEngine {
      public:
        %generate(c++, AnalyticEuropeanEngine);
        %generate(c#, AnalyticEuropeanEngine);
        %generate(countify, AnalyticEuropeanEngine);
        AnalyticEuropeanEngine(
            const boost::shared_ptr<GeneralizedBlackScholesProcess>& process);
    };

    class DiscountingSwapEngine : public PricingEngine {
      public:
        %generate(c#, DiscountingSwapEngine);
        %generate(countify, DiscountingSwapEngine);
        DiscountingSwapEngine(
               const Handle<YieldTermStructure>& discountCurve,
               bool includeSettlementDateFlows,
               Date settlementDate,
               Date npvDate);
    };
    
    class JamshidianSwaptionEngine : public PricingEngine {
      public:
        %generate(c#, JamshidianSwaptionEngine);
        JamshidianSwaptionEngine(
                         const boost::shared_ptr<OneFactorAffineModel>& model/*,
                         const Handle<YieldTermStructure>& termStructure =
                                                 Handle<YieldTermStructure>()*/);
    };
    
    class TreeSwaptionEngine : public PricingEngine {
      public:
        %generate(c#, TreeSwaptionEngine);
        TreeSwaptionEngine(const boost::shared_ptr<ShortRateModel>& x,
                           Size timeSteps,
                           const Handle<YieldTermStructure>& termStructure /*=
                                                 Handle<YieldTermStructure>()*/,
						   const boost::shared_ptr<AdditionalResultCalculator>& additionalResultCalculator /*=
												 boost::shared_ptr<AdditionalResultCalculator>()*/);    
    };
    
    class G2SwaptionEngine : public PricingEngine {
      public:
        %generate(c#, G2SwaptionEngine);
        G2SwaptionEngine(const boost::shared_ptr<G2>& model,
                         Real range,
                         Size intervals);
    };
    
}

