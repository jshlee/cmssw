#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "AnalysisDataFormats/TopObjects/interface/StGenEvent.h"

class StGenEventReco : public edm::stream::EDProducer<> {

 public:

  explicit StGenEventReco(const edm::ParameterSet&);
  ~StGenEventReco();
  virtual void produce(edm::Event&, const edm::EventSetup&) override;

 private:

  edm::EDGetTokenT<reco::GenParticleCollection> srcToken_;
  edm::EDGetTokenT<reco::GenParticleCollection> initToken_;
};
