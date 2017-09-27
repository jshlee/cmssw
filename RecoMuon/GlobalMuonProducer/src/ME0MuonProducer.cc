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

  trackToken_ = consumes<reco::TrackCollection >(parameterSet.getParameter<InputTag>("trackLabel"));

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

  Handle<reco::TrackCollection > trackHandle;
  event.getByToken(trackToken_, trackHandle);
  const reco::TrackCollection *tracks = trackHandle.product();
  
  auto me0Muons = reco::MuonCollection();
  
  for (auto &track : *tracks ){
  
    const double energy = hypot(track.p(), 0.105658369);
    const math::XYZTLorentzVector p4(track.px(), track.py(), track.pz(), energy);
    reco::Muon mu( track.charge(), p4, track.vertex() );
    cout << "ME0MuonProducer mu pt " << mu.pt() << " eta "<< mu.eta() << endl;
    me0Muons.push_back(mu);
  }
  
}
