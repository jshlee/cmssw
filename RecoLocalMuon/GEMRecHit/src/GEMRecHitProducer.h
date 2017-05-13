#ifndef RecoLocalMuon_GEMRecHit_GEMRecHitProducer_h
#define RecoLocalMuon_GEMRecHit_GEMRecHitProducer_h

/** \class GEMRecHitProducer
 *  Module for GEMRecHit production. 
 *  
 *  \author M. Maggim -- INFN Bari
 */


#include <memory>
#include <fstream>
#include <iostream>
#include <stdint.h>
#include <cstdlib>
#include <bitset>
#include <map>

#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/MuonDetId/interface/GEMDetId.h"
#include "DataFormats/GEMDigi/interface/GEMDigiCollection.h"

#include "CondFormats/GEMObjects/interface/GEMMaskedStrips.h"
#include "CondFormats/GEMObjects/interface/GEMDeadStrips.h"

#include "GEMEtaPartitionMask.h"


namespace edm {
  class ParameterSet;
  class Event;
  class EventSetup;
}

class GEMRecHitBaseAlgo;

class GEMRecHitProducer : public edm::stream::EDProducer<> {

public:
  /// Constructor
  GEMRecHitProducer(const edm::ParameterSet& config);

  /// Destructor
  virtual ~GEMRecHitProducer();

  // Method that access the EventSetup for each run
  virtual void beginRun(const edm::Run&, const edm::EventSetup& ) override;

  /// The method which produces the rechits
  virtual void produce(edm::Event& event, const edm::EventSetup& setup) override;

private:

  // The token to be used to retrieve GEM digis from the event
  edm::EDGetTokenT<GEMDigiCollection> theGEMDigiToken;

  bool badConnector_;
  double deadStripFraction_;
  
  // The reconstruction algorithm
  GEMRecHitBaseAlgo *theAlgo;
  //   static std::string theAlgoName;

  std::unique_ptr<GEMMaskedStrips> GEMMaskedStripsObj;
  // Object with mask-strips-vector for all the GEM Detectors

  std::unique_ptr<GEMDeadStrips> GEMDeadStripsObj;
  // Object with dead-strips-vector for all the GEM Detectors

   enum class MaskSource { File, EventSetup } maskSource_, deadSource_;
   std::vector<GEMMaskedStrips::MaskItem> MaskVec;
   std::vector<GEMDeadStrips::DeadItem> DeadVec;

};

#endif

