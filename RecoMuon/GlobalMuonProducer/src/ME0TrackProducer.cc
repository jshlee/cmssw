/**  \class ME0TrackProducer
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
#include "RecoMuon/GlobalMuonProducer/src/ME0TrackProducer.h"
#include "RecoMuon/GlobalTrackFinder/interface/GlobalMuonTrajectoryBuilder.h"
#include "RecoMuon/GlobalTrackingTools/interface/GlobalMuonRefitter.h"

#include "RecoMuon/TrackingTools/interface/MuonTrackFinder.h"
#include "RecoMuon/TrackingTools/interface/MuonTrackLoader.h"
#include "RecoMuon/TrackingTools/interface/MuonServiceProxy.h"
#include "RecoMuon/TrackingTools/interface/MuonCandidate.h"
#include "RecoMuon/TransientTrackingRecHit/interface/MuonTransientTrackingRecHit.h"

#include "TrackingTools/Records/interface/TransientRecHitRecord.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"

using namespace edm;
using namespace std;
using namespace reco;

//
// constructor with config
//
ME0TrackProducer::ME0TrackProducer(const ParameterSet& parameterSet) {
  LogDebug("Muon|RecoMuon|ME0TrackProducer") << "constructor called" << endl;

  muonToken_ = consumes<reco::MuonCollection >(parameterSet.getParameter<InputTag>("muonLabel"));
  trackToken_ = consumes<reco::TrackCollection >(parameterSet.getParameter<InputTag>("trackLabel"));

  // service parameters
  ParameterSet serviceParameters = parameterSet.getParameter<ParameterSet>("ServiceParameters");

  // the services
  theService = new MuonServiceProxy(serviceParameters);
  edm::ConsumesCollector iC  = consumesCollector();  

  // TrackRefitter parameters
  ParameterSet refitterParameters = parameterSet.getParameter<ParameterSet>("RefitterParameters");
  theRefitter = new GlobalMuonRefitter(refitterParameters, theService, iC);

  theTrackerRecHitBuilderName = refitterParameters.getParameter<string>("TrackerRecHitBuilder");
  theMuonRecHitBuilderName = refitterParameters.getParameter<string>("MuonRecHitBuilder");

  // TrackLoader parameters
  ParameterSet trackLoaderParameters = parameterSet.getParameter<ParameterSet>("TrackLoaderParameters");
  theTrackLoader = new MuonTrackLoader(trackLoaderParameters,iC,theService);

  produces<reco::TrackCollection>();
  produces<TrackingRecHitCollection>();
  produces<reco::TrackExtraCollection>();
  produces<vector<Trajectory> >() ;
  produces<TrajTrackAssociationCollection>();
  produces<reco::TrackToTrackMap>();
  
}
//
// destructor
//
ME0TrackProducer::~ME0TrackProducer() {
  LogTrace("Muon|RecoMuon|ME0TrackProducer") << "destructor called" << endl;
  if (theService) delete theService;
  if (theRefitter) delete theRefitter;
  if (theTrackLoader) delete theTrackLoader;
}
//
// reconstruct muons
//
void ME0TrackProducer::produce(Event& event, const EventSetup& eventSetup) {
  const string metname = "Muon|RecoMuon|ME0TrackProducer";  
  LogTrace(metname)<< endl << endl;
  LogTrace(metname)<< "TeV Muon Reconstruction started" << endl;  

  // Update the services
  theService->update(eventSetup);

  theRefitter->setEvent(event);
  theRefitter->setServices(theService->eventSetup());

  eventSetup.get<TransientRecHitRecord>().get(theTrackerRecHitBuilderName,theTrackerRecHitBuilder);
  eventSetup.get<TransientRecHitRecord>().get(theMuonRecHitBuilderName,theMuonRecHitBuilder);
  
  //Retrieve tracker topology from geometry
  edm::ESHandle<TrackerTopology> tTopoHand;
  eventSetup.get<TrackerTopologyRcd>().get(tTopoHand);
  const TrackerTopology *tTopo=tTopoHand.product();

  Handle<reco::MuonCollection > muonHandle;
  event.getByToken(muonToken_, muonHandle);
  const reco::MuonCollection *muons = muonHandle.product();

  Handle<reco::TrackCollection > trackHandle;
  event.getByToken(trackToken_, trackHandle);
  
  //const TrajectoryContainer trajectories;
  vector<Trajectory*> trajectories;
  std::vector<std::pair<Trajectory*,reco::TrackRef> > miniMap;
  reco::TrackRef::key_type trackIndex = 0;

  for (auto &mu : *muons ){
    if (!mu.isME0Muon()) continue;
    if (mu.isGlobalMuon()) continue;
          
    const reco::TrackRef trkRef = mu.innerTrack();
    const reco::Track *muTrk = trkRef.get();
    // const reco::Track *muTrk = mu.bestTrack();
    // if (mu.isGlobalMuon()) muTrk = mu.globalTrack().get();
    // else if (mu.isStandAloneMuon()) muTrk = mu.outerTrack().get();
    // if (mu.isME0Muon() || mu.isTrackerMuon()) muTrk = mu.innerTrack().get();

    // cout << "ME0TrackProducer mu pt " << mu.pt() << " eta "<< mu.eta()
    // 	 << " recHitsSize "<< muTrk->recHitsSize()
    // 	 << " numberOfValidHits "<< muTrk->numberOfValidHits()
    // 	 << endl;
    // cout << "mu.isGlobalMuon() " << mu.isGlobalMuon()
    // 	 << " mu.isTrackerMuon() " << mu.isTrackerMuon()
    // 	 << " mu.isStandAloneMuon() " << mu.isStandAloneMuon()
    // 	 << " mu.isCaloMuon() " << mu.isCaloMuon()
    // 	 << " mu.isPFMuon() " << mu.isPFMuon()
    // 	 << " mu.isRPCMuon() " << mu.isRPCMuon()
    // 	 << " mu.isGEMMuon() " << mu.isGEMMuon()
    // 	 << " mu.isME0Muon() " << mu.isME0Muon()
    // 	 << endl;
    
    reco::TransientTrack track(*muTrk,&*(theService->magneticField()),theService->trackingGeometry());
  
    auto tkbuilder = static_cast<TkTransientTrackingRecHitBuilder const *>(theTrackerRecHitBuilder.product());

    TransientTrackingRecHit::ConstRecHitContainer allRecHitsTemp; // all muon rechits temp
    for (trackingRecHit_iterator hit = track.recHitsBegin(); hit != track.recHitsEnd(); ++hit){
      if ((*hit)->isValid()) {
	if ((*hit)->geographicalId().det() == DetId::Tracker)
	  allRecHitsTemp.push_back((**hit).cloneForFit(*tkbuilder->geometry()->idToDet( (**hit).geographicalId() ) ) );
	else if ((*hit)->geographicalId().det() == DetId::Muon) {
	  if ((*hit)->geographicalId().subdetId() == 3)
	    continue;
	  allRecHitsTemp.push_back(theMuonRecHitBuilder->build(&**hit));
	}
      }
    }
    
    //float me0SegX = 100;
    int nMe0Hits = 0;
    if (mu.isME0Muon() && !mu.isGlobalMuon()){    
      for (auto chamber : mu.matches()){
	for (auto segment : chamber.me0Matches){
	  if (chamber.detector() == 5){
	    auto me0Segment = (*segment.me0SegmentRef);
	    for ( const auto& rh : me0Segment.recHits()){
	      allRecHitsTemp.push_back(theMuonRecHitBuilder->build(&*rh));
	      ++nMe0Hits;
	    }
	  }
	}
      }
    }
    // cout << "ME0TrackProducer mu pt " << mu.pt() << " eta "<< mu.eta()
    // 	 << " recHitsSize "<< muTrk->recHitsSize()
    // 	 << " numberOfValidHits "<< muTrk->numberOfValidHits()
    //   	 << " me0 hits "<< nMe0Hits
    // 	 << endl;


    vector<Trajectory> refitted = theRefitter->transform(*muTrk,track,allRecHitsTemp);

    if (refitted.size()>0) {
      Trajectory *refit = new Trajectory(refitted.front());
      trajectories.push_back(refit);
      std::pair<Trajectory*,reco::TrackRef> thisPair(refit,trkRef);
      miniMap.push_back(thisPair);
    }    
    //    cout << " Initial trajectory state: " << refitted.front().lastMeasurement().updatedState().freeState()->parameters() << endl;      
  }
  
  edm::OrphanHandle<reco::TrackCollection> outPut = theTrackLoader->loadTracks(trajectories, event, miniMap, trackHandle, *tTopo, "", true);

}
