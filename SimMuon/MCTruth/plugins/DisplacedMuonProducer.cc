
#include "SimMuon/MCTruth/plugins/DisplacedMuonProducer.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include "DataFormats/MuonDetId/interface/DTChamberId.h"
#include "DataFormats/MuonDetId/interface/MuonSubdetId.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include <sstream>

#include "FWCore/Framework/interface/ESHandle.h"
#include <DataFormats/MuonDetId/interface/RPCDetId.h>

DisplacedMuonProducer::DisplacedMuonProducer(const edm::ParameterSet& parset) :
muonsToken(consumes<reco::TrackCollection>(parset.getParameter< edm::InputTag >("muonsTag")))
{
    edm::LogVerbatim("DisplacedMuonProducer") << "constructing  DisplacedMuonProducer";
    produces<reco::TrackCollection>();
    produces<reco::TrackExtraCollection>();
    produces<TrackingRecHitCollection>();
}

DisplacedMuonProducer::~DisplacedMuonProducer() {
}

void DisplacedMuonProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    iEvent.getByToken(muonsToken,muonCollectionH);
    
    std::unique_ptr<reco::TrackCollection> selectedTracks(new reco::TrackCollection);
    std::unique_ptr<reco::TrackExtraCollection> selectedTrackExtras( new reco::TrackExtraCollection() );
    std::unique_ptr<TrackingRecHitCollection> selectedTrackHits( new TrackingRecHitCollection() );
    
    reco::TrackRefProd rTracks = iEvent.getRefBeforePut<reco::TrackCollection>();
    reco::TrackExtraRefProd rTrackExtras = iEvent.getRefBeforePut<reco::TrackExtraCollection>();
    TrackingRecHitRefProd rHits = iEvent.getRefBeforePut<TrackingRecHitCollection>();
    
    edm::Ref<reco::TrackExtraCollection>::key_type idx = 0;
    edm::Ref<reco::TrackExtraCollection>::key_type hidx = 0;
    
    edm::LogVerbatim("DisplacedMuonProducer") <<"\nThere are "<< muonCollectionH->size() <<" reco::DisplacedMuons.";
    unsigned int muon_index = 0;
    
    for(reco::TrackCollection::const_iterator muon = muonCollectionH->begin();
        muon != muonCollectionH->end(); ++muon, muon_index++) {
        
//        std::cout<<"MuonStationsWithValidHits: "<<muon->hitPattern().muonStationsWithValidHits()<<std::endl;
        
            if(!((muon->hitPattern().muonStationsWithValidHits()) > 1)) continue;
        
//            unsigned int nHitsDT = 0;
//            unsigned int nHitsCSC = 0;
//            unsigned int nHitsRPCb = 0;
//            unsigned int nHitsRPCf = 0;
//            unsigned int nHitsGEM = 0;
//            unsigned int nHitsME0 = 0;
//
//            for (trackingRecHit_iterator iHit = muon->recHitsBegin(); iHit != muon->recHitsEnd(); iHit++) {
//                
//                	if ((*iHit)->geographicalId().det() == DetId::Muon){
//                        
//                        if((*iHit)->geographicalId().subdetId() == MuonSubdetId::DT) ++nHitsDT;
//                        if((*iHit)->geographicalId().subdetId() == MuonSubdetId::CSC) ++nHitsCSC;
//                        if((*iHit)->geographicalId().subdetId() == MuonSubdetId::RPC){
//                            
//                            RPCDetId id((*iHit)->geographicalId());
//                            int endcap = id.region();
//                            
//                            if(endcap != 0) ++nHitsRPCb;
//                            else ++nHitsRPCf;
//                            
//                        }
//                        
//                        if((*iHit)->geographicalId().subdetId() == MuonSubdetId::GEM) ++nHitsGEM;
//                        if((*iHit)->geographicalId().subdetId() == MuonSubdetId::ME0) ++nHitsME0;
//                        
//                    }
//                
//            }
//        
//            if(!(nHitsCSC || nHitsRPCf || nHitsGEM || nHitsME0)) continue;
        
            const reco::Track* trk = &(*muon);
            // pointer to old track:
            reco::Track* newTrk = new reco::Track(*trk);
            
            newTrk->setExtra( reco::TrackExtraRef( rTrackExtras, idx++ ) );
            PropagationDirection seedDir = trk->seedDirection();
            // new copy of track Extras
            reco::TrackExtra * newExtra = new reco::TrackExtra(trk->outerPosition(), trk->outerMomentum(),
                                                               trk->outerOk(), trk->innerPosition(),
                                                               trk->innerMomentum(), trk->innerOk(),
                                                               trk->outerStateCovariance(), trk->outerDetId(),
                                                               trk->innerStateCovariance(), trk->innerDetId() , seedDir ) ;
            
            // new copy of the silicon hits; add hit refs to Extra and hits to hit collection
            
            //      edm::LogVerbatim("MuonTrackProducer")<<"\n printing initial hit_pattern";
            //      trk->hitPattern().print();
            unsigned int nHitsToAdd = 0;
            for (trackingRecHit_iterator iHit = trk->recHitsBegin(); iHit != trk->recHitsEnd(); iHit++) {
                TrackingRecHit* hit = (*iHit)->clone();
                selectedTrackHits->push_back( hit );
                ++nHitsToAdd;
            }
            newExtra->setHits( rHits, hidx, nHitsToAdd );
            hidx += nHitsToAdd;
        
            selectedTracks->push_back( *newTrk );
            selectedTrackExtras->push_back( *newExtra );
            
    }  // loop on reco::MuonCollection
    
    iEvent.put(std::move(selectedTracks));
    iEvent.put(std::move(selectedTrackExtras));
    iEvent.put(std::move(selectedTrackHits));
    
}
