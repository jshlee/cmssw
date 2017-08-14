
#ifndef MCTruth_DisplacedMuonProducer_h
#define MCTruth_DisplacedMuonProducer_h

#include <memory>
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonReco/interface/Muon.h"

class DisplacedMuonProducer : public edm::stream::EDProducer<> {
    public:
    explicit DisplacedMuonProducer(const edm::ParameterSet&);
    virtual ~DisplacedMuonProducer();
    
    private:
    virtual void produce(edm::Event&, const edm::EventSetup&);
    
    edm::EDGetTokenT<reco::TrackCollection> muonsToken;
    edm::Handle<reco::TrackCollection> muonCollectionH;
    
};

#endif
