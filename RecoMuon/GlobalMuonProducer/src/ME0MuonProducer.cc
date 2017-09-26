/**  \class ME0MuonProducer
 **/

// Framework
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Common/interface/Handle.h"
#include "Geometry/Records/interface/TrackerTopologyRcd.h"

// TrackFinder and specific GLB Trajectory Builder
#include "RecoMuon/GlobalMuonProducer/src/ME0MuonProducer.h"
#include "RecoMuon/GlobalTrackFinder/interface/GlobalMuonTrajectoryBuilder.h"
#include "RecoMuon/GlobalTrackingTools/interface/GlobalMuonRefitter.h"

#include "RecoMuon/TrackingTools/interface/MuonTrackFinder.h"
#include "RecoMuon/TrackingTools/interface/MuonTrackLoader.h"
#include "RecoMuon/TrackingTools/interface/MuonServiceProxy.h"
#include "RecoMuon/TransientTrackingRecHit/interface/MuonTransientTrackingRecHit.h"

#include "TrackingTools/Records/interface/TransientRecHitRecord.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"


using namespace edm;
using namespace std;
using namespace reco;

//
// constructor with config
//
ME0MuonProducer::ME0MuonProducer(const ParameterSet& parameterSet) {

  LogDebug("Muon|RecoMuon|ME0MuonProducer") << "constructor called" << endl;

  muonToken_ = consumes<reco::MuonCollection >(parameterSet.getParameter<InputTag>("muonLabel"));

  // service parameters
  ParameterSet serviceParameters = parameterSet.getParameter<ParameterSet>("ServiceParameters");

  // the services
  theService = new MuonServiceProxy(serviceParameters);
  edm::ConsumesCollector iC  = consumesCollector();  

  // TrackRefitter parameters
  ParameterSet refitterParameters = parameterSet.getParameter<ParameterSet>("RefitterParameters");
  theRefitter = new GlobalMuonRefitter(refitterParameters, theService, iC);

  theTrackerRecHitBuilderName = parameterSet.getParameter<string>("TrackerRecHitBuilder");
  theMuonRecHitBuilderName = parameterSet.getParameter<string>("MuonRecHitBuilder");

  // TrackLoader parameters
  ParameterSet trackLoaderParameters = parameterSet.getParameter<ParameterSet>("TrackLoaderParameters");
  theTrackLoader = new MuonTrackLoader(trackLoaderParameters,iC,theService);

  produces<reco::MuonCollection>();
}


//
// destructor
//
ME0MuonProducer::~ME0MuonProducer() {

  LogTrace("Muon|RecoMuon|ME0MuonProducer") << "destructor called" << endl;
  if (theService) delete theService;
  if (theRefitter) delete theRefitter;
  if (theTrackLoader) delete theTrackLoader;
}


//
// reconstruct muons
//
void ME0MuonProducer::produce(Event& event, const EventSetup& eventSetup) {

  const string metname = "Muon|RecoMuon|ME0MuonProducer";  
  LogTrace(metname)<< endl << endl;
  LogTrace(metname)<< "TeV Muon Reconstruction started" << endl;  

  // Update the services
  theService->update(eventSetup);

  theRefitter->setEvent(event);
  theRefitter->setServices(theService->eventSetup());

  eventSetup.get<TransientRecHitRecord>().get(theTrackerRecHitBuilderName,theTrackerRecHitBuilder);
  eventSetup.get<TransientRecHitRecord>().get(theMuonRecHitBuilderName,theMuonRecHitBuilder);
  
  //Retrieve tracker topology from geometry
  // edm::ESHandle<TrackerTopology> tTopoHand;
  // eventSetup.get<TrackerTopologyRcd>().get(tTopoHand);
  // const TrackerTopology *tTopo=tTopoHand.product();

  Handle<reco::MuonCollection > muonHandle;
  event.getByToken(muonToken_, muonHandle);
  
  auto me0Muons = std::make_unique<reco::MuonCollection>();

  const reco::MuonCollection *muons = muonHandle.product();
  
  for (auto &mu : *muons ){
    if (!mu.isME0Muon()) continue;
    if (mu.isGlobalMuon()) continue;
    
    cout << "ME0MuonProducer mu pt " << mu.pt() << " eta "<< mu.eta() << endl;
      
    const reco::Track *globalTrack = mu.innerTrack().get();
	
    TransientTrackingRecHit::ConstRecHitContainer allRecHitsTemp; // all muon rechits temp
      
    reco::TransientTrack track(*globalTrack,&*(theService->magneticField()),theService->trackingGeometry());
  
    auto tkbuilder = static_cast<TkTransientTrackingRecHitBuilder const *>(theTrackerRecHitBuilder.product());

    for (trackingRecHit_iterator hit = track.recHitsBegin(); hit != track.recHitsEnd(); ++hit){
      if ((*hit)->isValid()) {
	if ((*hit)->geographicalId().det() == DetId::Tracker)
	  allRecHitsTemp.push_back((**hit).cloneForFit(*tkbuilder->geometry()->idToDet( (**hit).geographicalId() ) ) );
      }
    }
      
    //float me0SegX = 100;
      
    for (auto chamber : mu.matches()){
      for (auto segment : chamber.me0Matches){
	if (chamber.detector() == 5){
	  auto me0Segment = (*segment.me0SegmentRef);
	  for ( const auto& rh : me0Segment.recHits()){
	    allRecHitsTemp.push_back(theMuonRecHitBuilder->build(&*rh));
	  }
	}
      }
    }
      
    vector<Trajectory> refitted = theRefitter->transform(*globalTrack,track,allRecHitsTemp);

    cout << " Initial trajectory state: " << refitted.front().lastMeasurement().updatedState().freeState()->parameters() << endl;
  
    
    //return refitted;
      
  }
}
