#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/RecoCandidate/interface/IsoDepositFwd.h"
#include "DataFormats/RecoCandidate/interface/IsoDeposit.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "PhysicsTools/IsolationAlgos/interface/EventDependentAbsVeto.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "PhysicsTools/IsolationAlgos/interface/CITKIsolationConeDefinitionBase.h"
#include "DataFormats/Common/interface/OwnVector.h"

#include "RecoMuon/MuonIsolation/interface/Range.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "CommonTools/Statistics/interface/ChiSquaredProbability.h"

#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include <string>
#include <unordered_map>

//module to compute isolation sum weighted with PUPPI weights
namespace citk {
  class PFIsolationSumProducerForPUPPI : public edm::stream::EDProducer<> {
    typedef reco::TrackBase::Point BeamPoint;
    typedef muonisolation::Range<float> Range;
    
  public:  
    PFIsolationSumProducerForPUPPI(const edm::ParameterSet&);
    
    ~PFIsolationSumProducerForPUPPI() override {}
    
    void beginLuminosityBlock(const edm::LuminosityBlock&,
			      const edm::EventSetup&) final;

    void produce(edm::Event&, const edm::EventSetup&) final;

    static void fillDescriptions(edm::ConfigurationDescriptions & descriptions);

    bool trackSelector(const reco::Track & tk, double vtx_z, reco::TrackBase::Point beamPoint) const;
    
  private:  
    // datamembers
    static constexpr unsigned kNPFTypes = 8;
    typedef std::unordered_map<std::string,int> TypeMap;
    typedef std::vector<std::unique_ptr<IsolationConeDefinitionBase> > IsoTypes;
    typedef edm::View<reco::Candidate> CandView;
    const TypeMap _typeMap;
    edm::EDGetTokenT<CandView> _to_isolate, _isolate_with;
    edm::EDGetTokenT<edm::ValueMap<float> > puppiValueMapToken_;//for puppiValueMap
    edm::Handle<edm::ValueMap<float>> puppiValueMap;//puppiValueMap
    // indexed by pf candidate type
    std::array<IsoTypes,kNPFTypes> _isolation_types; 
    std::array<std::vector<std::string>,kNPFTypes> _product_names;
    bool usePUPPI = true;
    bool useValueMapForPUPPI = true;
    bool usePUPPINoLepton = false;// in case puppi weights are taken from packedCandidate can take weights for puppiNoLeptons
    // track selection
    std::string theBeamlineOption;
    edm::EDGetTokenT<reco::BeamSpot> theBeamSpotToken;
  
    double theDiff_z;
    double theDiff_r;
    double        drMax;  //! cone size
    BeamPoint beamPoint;  //! beam spot position
    unsigned int       nHitsMin;  //! nValidHits >= nHitsMin
    double  chi2NdofMax;  //! max value of normalized chi2
    double  chi2ProbMin;  //! ChiSquaredProbability( chi2, ndf ) > chi2ProbMin
    double        ptMin;  //! tk.pt>ptMin

  };
}
typedef citk::PFIsolationSumProducerForPUPPI CITKPFIsolationSumProducerForPUPPI;

DEFINE_FWK_MODULE(CITKPFIsolationSumProducerForPUPPI);

namespace citk {
  PFIsolationSumProducerForPUPPI::PFIsolationSumProducerForPUPPI(const edm::ParameterSet& c) :
    _typeMap( { {"h+",1},
	  {"h0",5},
	    {"gamma",4},
	      {"electron",2},
		{"muon",3},
		  {"HFh",6},
		    {"HFgamma",7} } ){
    _to_isolate = 
      consumes<CandView>(c.getParameter<edm::InputTag>("srcToIsolate"));
    _isolate_with = 
      consumes<CandView>(c.getParameter<edm::InputTag>("srcForIsolationCone"));
    if (!c.getParameter<edm::InputTag>("puppiValueMap").label().empty()) {
      puppiValueMapToken_ = mayConsume<edm::ValueMap<float>>(c.getParameter<edm::InputTag>("puppiValueMap")); //getting token for puppiValueMap
      useValueMapForPUPPI = true;
    }
    else {
      useValueMapForPUPPI = false;
      usePUPPINoLepton = c.getParameter<bool>("usePUPPINoLepton");
    }
    usePUPPI = c.getParameter<bool>("usePUPPI");    
    
    const std::vector<edm::ParameterSet>& isoDefs = 
      c.getParameterSetVector("isolationConeDefinitions");
    for( const auto& isodef : isoDefs ) {
      const std::string& name = 
	isodef.getParameter<std::string>("isolationAlgo");
      const float coneSize = isodef.getParameter<double>("coneSize");
      char buf[50];
      std::sprintf(buf,"DR%.2f",coneSize);
      std::string coneName(buf);
      auto decimal = coneName.find('.');
      if( decimal != std::string::npos ) coneName.erase(decimal,1);
      const std::string& isotype = 
	isodef.getParameter<std::string>("isolateAgainst");
      IsolationConeDefinitionBase* theisolator =
	CITKIsolationConeDefinitionFactory::get()->create(name,isodef);
      theisolator->setConsumes(consumesCollector());
      const auto thetype = _typeMap.find(isotype);
      if( thetype == _typeMap.end() ) {
	throw cms::Exception("InvalidIsolationType")
	  << "Isolation type: " << isotype << " is not available in the "
	  << "list of allowed isolations!.";
      }
      _isolation_types[thetype->second].emplace_back(theisolator);
      const std::string dash("-");
      std::string pname = isotype+dash+coneName+dash+theisolator->additionalCode();
      _product_names[thetype->second].emplace_back(pname);
      produces<edm::ValueMap<float> >(pname);
    }
    const edm::ParameterSet par = c.getParameterSet("isolationTrackSelections");
    theDiff_r = par.getParameter<double>("Diff_r");
    theDiff_z =par.getParameter<double>("Diff_z");
    drMax = par.getParameter<double>("DR_Max");

    theBeamlineOption = par.getParameter<std::string>("BeamlineOption") ;
    theBeamSpotToken = consumes<reco::BeamSpot>(par.getParameter<edm::InputTag>("BeamSpotLabel"));

    nHitsMin = par.getParameter<unsigned int>("NHits_Min");
    chi2NdofMax = par.getParameter<double>("Chi2Ndof_Max");
    chi2ProbMin = par.getParameter<double>("Chi2Prob_Min");
    ptMin = par.getParameter<double>("Pt_Min");

  }

  void  PFIsolationSumProducerForPUPPI::
  beginLuminosityBlock(const edm::LuminosityBlock&,
		       const edm::EventSetup& es) {
    for( const auto& isolators_for_type : _isolation_types ) {
      for( const auto& isolator : isolators_for_type ) {
	isolator->getEventSetupInfo(es);
      }
    }
  }

  void  PFIsolationSumProducerForPUPPI::produce(edm::Event& ev, const edm::EventSetup& es) {
    typedef std::unique_ptr<edm::ValueMap<float> >  product_type;
    typedef std::vector<float> product_values;
    edm::Handle<CandView> to_isolate;
    edm::Handle<CandView> isolate_with;
    ev.getByToken(_to_isolate,to_isolate);
    ev.getByToken(_isolate_with,isolate_with);
    if(useValueMapForPUPPI)ev.getByToken(puppiValueMapToken_, puppiValueMap);

    reco::TrackBase::Point beamPoint(0,0, 0);
    if (theBeamlineOption == "BeamSpotFromEvent"){
      //pick beamSpot
      reco::BeamSpot beamSpot;
      edm::Handle<reco::BeamSpot> beamSpotH;

      ev.getByToken(theBeamSpotToken,beamSpotH);

      if (beamSpotH.isValid()){
	beamPoint = beamSpotH->position();
      }
    }
  
    // the list of value vectors indexed as "to_isolate"
    std::array<std::vector<product_values>,kNPFTypes> the_values;    
    // get extra event info and setup value cache
    unsigned i = 0;
    for( const auto& isolators_for_type : _isolation_types ) {
      the_values[i++].resize(isolators_for_type.size());
      for( const auto& isolator : isolators_for_type ) {
	isolator->getEventInfo(ev);
      }
    }
    reco::PFCandidate helper; // to translate pdg id to type    
    // loop over the candidates we are isolating and fill the values
    for( size_t c = 0; c < to_isolate->size(); ++c ) {
      auto cand_to_isolate = to_isolate->ptrAt(c);
      std::array<std::vector<float>,kNPFTypes> cand_values;      
      unsigned k = 0;
      for( const auto& isolators_for_type : _isolation_types ) {
	cand_values[k].resize(isolators_for_type.size());
	for( auto& value : cand_values[k] ) value = 0.0;
	++k;
      }
      for( size_t ic = 0; ic < isolate_with->size(); ++ic ) {
	auto isocand = isolate_with->ptrAt(ic);
	edm::Ptr<pat::PackedCandidate> aspackedCandidate(isocand);
	auto isotype = helper.translatePdgIdToType(isocand->pdgId());	
	const auto& isolations = _isolation_types[isotype];	
	for( unsigned i = 0; i < isolations.size(); ++ i  ) {
	  if( isolations[i]->isInIsolationCone(cand_to_isolate,isocand) ) {

	    double vtx_z = isocand->vz();

	    // check if it has track
	    const reco::Track * trk = isocand->bestTrack();
	    if (trk){
	      if (!trackSelector(*trk, vtx_z, beamPoint))
		continue;	      
	    }
	    
	    double puppiWeight = 0.;
	    if (!useValueMapForPUPPI && !usePUPPINoLepton) puppiWeight = aspackedCandidate -> puppiWeight(); // if miniAOD, take puppiWeight directly from the object
	    else if (!useValueMapForPUPPI && usePUPPINoLepton) puppiWeight = aspackedCandidate -> puppiWeightNoLep(); // if miniAOD, take puppiWeightNoLep directly from the object
	    else  puppiWeight = (*puppiValueMap)[isocand]; // if AOD, take puppiWeight from the valueMap
	    
	    if (usePUPPI){
	      if (puppiWeight > 0.)cand_values[isotype][i] += (isocand->pt())*puppiWeight; // this is basically the main change to Lindsey's code: scale pt with puppiWeight for candidates with puppiWeight > 0.
	    }
	    else {
	      cand_values[isotype][i] += (isocand->pt());
	    }
	  }
	}
      }
      // add this candidate to isolation value list
      for( unsigned i = 0; i < kNPFTypes; ++i ) {
	for( unsigned j = 0; j < cand_values[i].size(); ++j ) {
	  the_values[i][j].push_back(cand_values[i][j]);
	}
      }
    }
    // fill and put all products
    for( unsigned i = 0; i < kNPFTypes; ++ i ) {
      for( unsigned j = 0; j < the_values[i].size(); ++j ) {
	product_type the_product( new edm::ValueMap<float> );
	edm::ValueMap<float>::Filler fillerprod(*the_product);
	fillerprod.insert(to_isolate, 
			  the_values[i][j].begin(),
			  the_values[i][j].end());
	fillerprod.fill();
	ev.put(std::move(the_product),_product_names[i][j]);
      }
    }
  }

  bool PFIsolationSumProducerForPUPPI::trackSelector(const reco::Track & tk, double vtx_z, reco::TrackBase::Point beamPoint) const
  {    
    float tZ = tk.vz(); 
    float tPt = tk.pt();
    //float tD0 = fabs(tk.d0());  
    float tD0Cor = fabs(tk.dxy(beamPoint));
    //float tEta = tk.eta();
    //float tPhi = tk.phi();
    float tChi2Ndof = tk.normalizedChi2();

    Range zRange = Range(vtx_z-theDiff_z, vtx_z+theDiff_z);
    Range rRange = Range(0,theDiff_r);
  
    if ( !zRange.inside( tZ ) ) return false; 
    if ( tPt < ptMin ) return false;
    if ( !rRange.inside( tD0Cor) ) return false;
    //if ( dir.deltaR( reco::isodeposit::Direction(tEta, tPhi) ) > drMax ) return false;
    if ( tChi2Ndof > chi2NdofMax ) return false;

    //! skip if min Hits == 0; assumes any track has at least one valid hit
    if (nHitsMin > 0 ){
      unsigned int tHits = tk.numberOfValidHits();
      if ( tHits < nHitsMin ) return false;
    }

    //! similarly here
    if(chi2ProbMin > 0){
      float tChi2Prob = ChiSquaredProbability(tk.chi2(), tk.ndof());
      if ( tChi2Prob < chi2ProbMin ) return false;
    }
    
    return true;
  }

  // ParameterSet description for module
  void PFIsolationSumProducerForPUPPI::fillDescriptions(edm::ConfigurationDescriptions & descriptions)
  { 
    edm::ParameterSetDescription iDesc;
    iDesc.setComment("PUPPI isolation sum producer");

    iDesc.add<edm::InputTag>("srcToIsolate", edm::InputTag("no default"))->setComment("calculate isolation for this collection");
    iDesc.add<edm::InputTag>("srcForIsolationCone", edm::InputTag("no default"))->setComment("collection for the isolation calculation: like particleFlow ");
    iDesc.add<edm::InputTag>("puppiValueMap", edm::InputTag("puppi"))->setComment("source for puppi, if left empty weight from packedCandidate is taken");

    edm::ParameterSetDescription descIsoConeDefinitions;
    descIsoConeDefinitions.add<std::string>("isolationAlgo", "no default");
    descIsoConeDefinitions.add<double>("coneSize", 0.3);
    descIsoConeDefinitions.add<std::string>("isolateAgainst", "no default");
    descIsoConeDefinitions.add<std::vector<unsigned>>("miniAODVertexCodes", {2,3});
    descIsoConeDefinitions.addOptional<double>("VetoConeSizeBarrel", 0.0);
    descIsoConeDefinitions.addOptional<double>("VetoConeSizeEndcaps", 0.0);
    descIsoConeDefinitions.addOptional<double>("VetoThreshold", 0.0);
    descIsoConeDefinitions.addOptional<double>("VetoConeSize", 0.0);
    descIsoConeDefinitions.addOptional<int>("vertexIndex",0);
    descIsoConeDefinitions.addOptional<edm::InputTag>("particleBasedIsolation",edm::InputTag("no default"))->setComment("map for footprint removal that is used for photons");

    std::vector<edm::ParameterSet> isolationConeDefinitions;
    edm::ParameterSet chargedHadrons, neutralHadrons,photons;
    isolationConeDefinitions.push_back(chargedHadrons);
    isolationConeDefinitions.push_back(neutralHadrons);
    isolationConeDefinitions.push_back(photons);
    iDesc.addVPSet("isolationConeDefinitions", descIsoConeDefinitions, isolationConeDefinitions);

    edm::ParameterSetDescription descIsoTrkSelDefinitions;
    descIsoTrkSelDefinitions.add<double>("Diff_r",0.1);
    descIsoTrkSelDefinitions.add<double>("Diff_z",0.2);
    descIsoTrkSelDefinitions.add<double>("DR_Max",0.5);
    
    descIsoTrkSelDefinitions.add<std::string>("BeamlineOption", "BeamSpotFromEvent");
    descIsoTrkSelDefinitions.add<edm::InputTag>("BeamSpotLabel", edm::InputTag("offlineBeamSpot"));

    descIsoTrkSelDefinitions.add<unsigned int>("NHits_Min", 0);
    descIsoTrkSelDefinitions.add<double>("Chi2Ndof_Max", 1e+64);
    descIsoTrkSelDefinitions.add<double>("Chi2Prob_Min", -1.0);
    descIsoTrkSelDefinitions.add<double>("Pt_Min", -1.0);
        
    iDesc.add("isolationTrackSelections", descIsoTrkSelDefinitions);
    iDesc.add<bool>("usePUPPINoLepton",false);
    iDesc.add<bool>("usePUPPI",true);

    descriptions.add("CITKPFIsolationSumProducerForPUPPI", iDesc);
  }

}
