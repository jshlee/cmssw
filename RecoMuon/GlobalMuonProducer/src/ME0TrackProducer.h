#ifndef RecoMuon_GlobalMuonProducer_ME0TrackProducer_H
#define RecoMuon_GlobalMuonProducer_ME0TrackProducer_H

/**  \class ME0TrackProducer
 **/

#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/TrackToTrackMap.h"
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonReco/interface/MuonTrackLinks.h"

#include "Geometry/Records/interface/IdealGeometryRecord.h"

#include "TrackingTools/PatternTools/interface/TrajTrackAssociation.h"

#include "RecoMuon/GlobalTrackingTools/interface/GlobalMuonRefitter.h"
#include "RecoMuon/TrackingTools/interface/MuonTrackLoader.h"


typedef edm::ValueMap<reco::DYTInfo> DYTestimators;

namespace edm {class ParameterSet; class Event; class EventSetup;}

class MuonTrackFinder;
class MuonServiceProxy;

class ME0TrackProducer : public edm::stream::EDProducer<> {

 public:
  
  /// constructor with config
  ME0TrackProducer(const edm::ParameterSet&);
  
  /// destructor
  virtual ~ME0TrackProducer(); 
  
  /// reconstruct muons
  virtual void produce(edm::Event&, const edm::EventSetup&) override;
  
 private:
  
  edm::EDGetTokenT<reco::MuonCollection > muonToken_;
  edm::EDGetTokenT<reco::TrackCollection > trackToken_;
  
  /// the event setup proxy, it takes care the services update
  MuonServiceProxy* theService;
  
  GlobalMuonRefitter* theRefitter;

  MuonTrackLoader* theTrackLoader;

  std::string theTrackerRecHitBuilderName;
  edm::ESHandle<TransientTrackingRecHitBuilder> theTrackerRecHitBuilder;
  
  std::string theMuonRecHitBuilderName;
  edm::ESHandle<TransientTrackingRecHitBuilder> theMuonRecHitBuilder;
    
};
#endif
