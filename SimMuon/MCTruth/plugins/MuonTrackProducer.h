//
// modified & integrated by Giovanni Abbiendi
// from code by Arun Luthra: UserCode/luthra/MuonTrackSelector/src/MuonTrackSelector.cc
//
#ifndef MCTruth_MuonTrackProducer_h
#define MCTruth_MuonTrackProducer_h

#include <memory>
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/CSCRecHit/interface/CSCSegmentCollection.h"
#include "DataFormats/DTRecHit/interface/DTRecSegment4DCollection.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"

#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/GEMGeometry/interface/ME0EtaPartitionSpecs.h"
#include "Geometry/GEMGeometry/interface/ME0Geometry.h"

class MuonTrackProducer : public edm::stream::EDProducer<> {
  public:
    explicit MuonTrackProducer(const edm::ParameterSet&);
    std::vector<double> findSimVtx(edm::Event&);
    bool isGlobalTightMuon(const reco::MuonCollection::const_iterator);
    bool isTrackerTightMuon(const reco::MuonCollection::const_iterator);
    bool isIsolatedMuon(const reco::MuonCollection::const_iterator);
    bool isME0MuonSel(reco::MuonCollection::const_iterator, double, double, double, double, double);
    bool isME0MuonSelNew(edm::Event&, const edm::EventSetup&, reco::MuonCollection::const_iterator, double, double, double);
    bool isLoose(edm::Event&, reco::MuonCollection::const_iterator);
    bool isTight(edm::Event&, reco::MuonCollection::const_iterator, bool, bool);
    bool isTightClassic(edm::Event&, reco::MuonCollection::const_iterator, bool, bool);
    bool isLooseMod(edm::Event&, reco::MuonCollection::const_iterator);
    bool isTightMod(edm::Event&, reco::MuonCollection::const_iterator, bool, bool);
    std::vector<bool> isLooseModExt(edm::Event&, const edm::EventSetup&, reco::MuonCollection::const_iterator);
    std::vector<bool> isTightModExt(edm::Event&, const edm::EventSetup&, reco::MuonCollection::const_iterator, bool, bool);
    bool isME0MuonLoose(edm::Event&, const edm::EventSetup&, reco::MuonCollection::const_iterator);
    bool isME0MuonTight(edm::Event&, const edm::EventSetup&, reco::MuonCollection::const_iterator, bool, bool);
    bool isTightModExtSim(edm::Event&, reco::MuonCollection::const_iterator);
    virtual ~MuonTrackProducer();

  private:
    virtual void produce(edm::Event&, const edm::EventSetup&);
  
    edm::Handle<reco::MuonCollection> muonCollectionH;
    edm::Handle<DTRecSegment4DCollection> dtSegmentCollectionH_;
    edm::Handle<CSCSegmentCollection> cscSegmentCollectionH_;

    edm::EDGetTokenT<reco::MuonCollection> muonsToken;
    edm::EDGetTokenT<reco::VertexCollection> vtx_Token;
    edm::EDGetTokenT<reco::GenParticleCollection> genP_Token;
    edm::EDGetTokenT<reco::BeamSpot> bs_Token;

    bool useIPxy, useIPz;
    edm::EDGetTokenT<DTRecSegment4DCollection> inputDTRecSegment4DToken_;
    edm::EDGetTokenT<CSCSegmentCollection> inputCSCSegmentToken_;

    std::vector<std::string> selectionTags;
    std::string trackType;
    const edm::ParameterSet parset_;
};

#endif
