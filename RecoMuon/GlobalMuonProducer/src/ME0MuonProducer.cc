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
ME0MuonProducer::ME0MuonProducer(const ParameterSet& parameterSet) {
  LogDebug("Muon|RecoMuon|ME0MuonProducer") << "constructor called" << endl;

  me0TrackToken_ = consumes<reco::TrackCollection >(parameterSet.getParameter<InputTag>("me0TrackLabel"));
  muonToken_ = consumes<reco::MuonCollection >(parameterSet.getParameter<InputTag>("muonLabel"));
  trackAssoMapToken_=consumes<reco::TrackToTrackMap>(parameterSet.getParameter<InputTag>("me0TrackLabel").label());

  produces<reco::MuonCollection>();  
}
//
// destructor
//
ME0MuonProducer::~ME0MuonProducer() {}
//
// reconstruct muons
//
void ME0MuonProducer::produce(Event& event, const EventSetup& eventSetup) {

  Handle<reco::TrackCollection > me0TrackHandle;
  event.getByToken(me0TrackToken_, me0TrackHandle);
  //  const reco::TrackCollection *me0Tracks = me0TrackHandle.product();

  Handle<reco::MuonCollection > muonHandle;
  event.getByToken(muonToken_, muonHandle);  
  const reco::MuonCollection *muons = muonHandle.product();

  edm::Handle<reco::TrackToTrackMap> trackAssoMap;
  event.getByToken(trackAssoMapToken_, trackAssoMap);
    
  auto me0Muons = std::make_unique<reco::MuonCollection>();

  int ntrk=0;
  for (auto &mu : *muons ){
  
    reco::Muon newMu(mu);

    if (mu.isME0Muon() && !mu.isGlobalMuon()){
      const reco::TrackRef muTrk = mu.innerTrack();

      reco::TrackRef newMuTrk;
      reco::TrackToTrackMap::const_iterator iEnd;
      reco::TrackToTrackMap::const_iterator iii;
      iEnd = trackAssoMap->end();
      iii = trackAssoMap->find(muTrk);
      if (iii != iEnd ) newMuTrk = (*trackAssoMap)[muTrk];

      //const reco::Track* track = muTrk.get();
      
      if (newMuTrk.isNonnull()){
	const reco::Track* newTrack = newMuTrk.get();
	newMu.setCombined( newMuTrk );
	newMu.setGlobalTrack( newMuTrk );
	newMu.setBestTrack(reco::Muon::CombinedTrack);
	const double energy = hypot(newTrack->p(), 0.105658369);
	const math::XYZTLorentzVector p4(newTrack->px(), newTrack->py(), newTrack->pz(), energy);	
	newMu.setP4(p4);
	cout << "ME0MuonProducer mu  pt " << mu.pt() << " eta "<< mu.eta()<<endl;
	cout << "                new pt " << newMu.pt() << " eta "<< newMu.eta()
	     << " me0 hits "<< newTrack->hitPattern().numberOfValidMuonME0Hits()
	     <<endl;
      }
      
      // int noRecHitME0 = 0;
      // for(auto i=track->recHitsBegin(); i!=track->recHitsEnd(); i++) {
      // 	DetId hitId = (*i)->geographicalId();
      // 	if (!(*i)->isValid() ) continue;      
      // 	if (hitId.det()!=DetId::Muon) continue;
      // 	if (hitId.subdetId() == MuonSubdetId::ME0) ++noRecHitME0;
      // }
    }
    
    
    // cout << "ME0MuonProducer mu pt " << mu.pt() << " eta "<< mu.eta()
    // 	 << " recHitsSize "<< track.recHitsSize()
    // 	 << " numberOfValidHits "<< track.numberOfValidHits()
    // 	 << " noRecHitME0 "<< noRecHitME0
    // 	 << " me0 hits "<< track.hitPattern().numberOfValidMuonME0Hits()
    // 	 << endl;
    me0Muons->push_back(newMu);
    ntrk++;
  }
  event.put(std::move(me0Muons));

}
