/** \file
 *
 *  \author M. Maggi -- INFN Bari
*/

#include "GEMRecHitProducer.h"


#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "DataFormats/GEMDigi/interface/GEMDigiCollection.h"

#include "Geometry/GEMGeometry/interface/GEMEtaPartition.h"
#include "Geometry/GEMGeometry/interface/GEMGeometry.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "DataFormats/MuonDetId/interface/GEMDetId.h"
#include "DataFormats/GEMRecHit/interface/GEMRecHit.h"

#include "RecoLocalMuon/GEMRecHit/interface/GEMRecHitBaseAlgo.h"
#include "RecoLocalMuon/GEMRecHit/interface/GEMRecHitAlgoFactory.h"
#include "DataFormats/GEMRecHit/interface/GEMRecHitCollection.h"

#include "CondFormats/GEMObjects/interface/GEMMaskedStrips.h"
#include "CondFormats/GEMObjects/interface/GEMDeadStrips.h"
#include "CondFormats/DataRecord/interface/GEMMaskedStripsRcd.h"
#include "CondFormats/DataRecord/interface/GEMDeadStripsRcd.h"


#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "CLHEP/Random/RandFlat.h"

#include <string>
#include <fstream>


using namespace edm;
using namespace std;


GEMRecHitProducer::GEMRecHitProducer(const ParameterSet& config){

  // Set verbose output

  produces<GEMRecHitCollection>();

  theGEMDigiToken = consumes<GEMDigiCollection>(config.getParameter<edm::InputTag>("gemDigiLabel"));  

  badConnector_ = config.getParameter<bool>("badConnector");
  deadStripFraction_ = config.getParameter<double>("deadStripFraction");
  
  // Get the concrete reconstruction algo from the factory

  string theAlgoName = config.getParameter<string>("recAlgo");
  theAlgo = GEMRecHitAlgoFactory::get()->create(theAlgoName,
						config.getParameter<ParameterSet>("recAlgoConfig"));

  // Get masked- and dead-strip information
  ///
  GEMMaskedStripsObj = std::make_unique<GEMMaskedStrips>();
  GEMDeadStripsObj = std::make_unique<GEMDeadStrips>();

  const string maskSource = config.getParameter<std::string>("maskSource");
  if (maskSource == "File") {
    maskSource_ = MaskSource::File;
    edm::FileInPath fp = config.getParameter<edm::FileInPath>("maskvecfile");
    std::ifstream inputFile(fp.fullPath().c_str(), std::ios::in);
    if ( !inputFile ) {
      std::cerr << "Masked Strips File cannot not be opened" << std::endl;
      exit(1);
    }
    while ( inputFile.good() ) {
      GEMMaskedStrips::MaskItem Item;
      inputFile >> Item.rawId >> Item.strip;
      if ( inputFile.good() ) MaskVec.push_back(Item);
    }
    inputFile.close();
  }

  const string deadSource = config.getParameter<std::string>("deadSource");
  if (deadSource == "File") {
    deadSource_ = MaskSource::File;
    edm::FileInPath fp = config.getParameter<edm::FileInPath>("deadvecfile");
    std::ifstream inputFile(fp.fullPath().c_str(), std::ios::in);
    if ( !inputFile ) {
      std::cerr << "Dead Strips File cannot not be opened" << std::endl;
      exit(1);
    }
    while ( inputFile.good() ) {
      GEMDeadStrips::DeadItem Item;
      inputFile >> Item.rawId >> Item.strip;
      if ( inputFile.good() ) DeadVec.push_back(Item);
    }
    inputFile.close();
  }

  ///
}


GEMRecHitProducer::~GEMRecHitProducer(){

  delete theAlgo;

}



void GEMRecHitProducer::beginRun(const edm::Run& r, const edm::EventSetup& setup){

  /*if ( maskSource_ == MaskSource::EventSetup ) {
    edm::ESHandle<GEMMaskedStrips> readoutMaskedStrips;
    setup.get<GEMMaskedStripsRcd>().get(readoutMaskedStrips);
    const GEMMaskedStrips* tmp_obj = readoutMaskedStrips.product();
    GEMMaskedStripsObj->MaskVec = tmp_obj->MaskVec;
    delete tmp_obj;
  }*/
  //else if ( maskSource_ == MaskSource::File ) {
  if ( maskSource_ == MaskSource::File ) {
    std::vector<GEMMaskedStrips::MaskItem>::iterator posVec;
    for ( posVec = MaskVec.begin(); posVec != MaskVec.end(); ++posVec ) {
      GEMMaskedStrips::MaskItem Item; 
      Item.rawId = (*posVec).rawId;
      Item.strip = (*posVec).strip;
      GEMMaskedStripsObj->MaskVec.push_back(Item);
    }
  }
  /*
  if ( deadSource_ == MaskSource::EventSetup ) {
    edm::ESHandle<GEMDeadStrips> readoutDeadStrips;
    setup.get<GEMDeadStripsRcd>().get(readoutDeadStrips);
    const GEMDeadStrips* tmp_obj = readoutDeadStrips.product();
    GEMDeadStripsObj->DeadVec = tmp_obj->DeadVec;
    delete tmp_obj;
  }*/
  //else if ( deadSource_ == MaskSource::File ) {
  if ( deadSource_ == MaskSource::File ) {
    std::vector<GEMDeadStrips::DeadItem>::iterator posVec;
    for ( posVec = DeadVec.begin(); posVec != DeadVec.end(); ++posVec ) {
      GEMDeadStrips::DeadItem Item;
      Item.rawId = (*posVec).rawId;
      Item.strip = (*posVec).strip;
      GEMDeadStripsObj->DeadVec.push_back(Item);
    }
 }

}



void GEMRecHitProducer::produce(Event& event, const EventSetup& setup) {

  // Get the GEM Geometry

  ESHandle<GEMGeometry> gemGeom;
  setup.get<MuonGeometryRecord>().get(gemGeom);

  // Get the digis from the event

  Handle<GEMDigiCollection> digis; 
  event.getByToken(theGEMDigiToken,digis);


  edm::Service<edm::RandomNumberGenerator> rng;
  CLHEP::HepRandomEngine* engine = &rng->getEngine(event.streamID());
  
  // Pass the EventSetup to the algo

  theAlgo->setES(setup);

  // Create the pointer to the collection which will store the rechits

  auto recHitCollection = std::make_unique<GEMRecHitCollection>();

  // Iterate through all digi collections ordered by LayerId   

  GEMDigiCollection::DigiRangeIterator gemdgIt;
  for (gemdgIt = digis->begin(); gemdgIt != digis->end();
       ++gemdgIt){
       
    // The layerId
    const GEMDetId& gemId = (*gemdgIt).first;

    // Get the GeomDet from the setup
    const GEMEtaPartition* roll = gemGeom->etaPartition(gemId);

    // Get the iterators over the digis associated with this LayerId
    const GEMDigiCollection::Range& range = (*gemdgIt).second;

    // Getting the roll mask, that includes dead strips, for the given GEMDet
    EtaPartitionMask mask;
    int rawId = gemId.rawId();
    int Size = GEMMaskedStripsObj->MaskVec.size();
    for (int i = 0; i < Size; i++ ) {
      if ( GEMMaskedStripsObj->MaskVec[i].rawId == rawId ) {
	int bit = GEMMaskedStripsObj->MaskVec[i].strip;
	//std::cout << "GEMMaskedStripsObj "<< bit <<endl;
	mask.set(bit);
      }
    }

    Size = GEMDeadStripsObj->DeadVec.size();
    for (int i = 0; i < Size; i++ ) {
      if ( GEMDeadStripsObj->DeadVec[i].rawId == rawId ) {
	int bit = GEMDeadStripsObj->DeadVec[i].strip;
	//std::cout << "GEMDeadStripsObj "<< bit <<endl;
	mask.set(bit);
      }
    }
    /*
    // masking study for ge11
    if (gemId.station() == 1){
      if (badConnector_){ // masking 1 and 128 from each readout
	mask.set(0); mask.set(127);
	mask.set(128); mask.set(255);
	mask.set(256); mask.set(383);
      }
      if (deadStripFraction_ > 0.0){
	for (int i = 0; i < 383; i++ ){	
	  if (CLHEP::RandFlat::shoot(engine, 0., 100.) < deadStripFraction_)	
	    mask.set(i);
	}
      }
    }
   */
    // Call the reconstruction algorithm    
    //std::cout << mask << std::endl;
    OwnVector<GEMRecHit> recHits =
      theAlgo->reconstruct(*roll, gemId, range, mask);
    
    if(recHits.size() > 0) //FIXME: is it really needed?
      recHitCollection->put(gemId, recHits.begin(), recHits.end());
  }

  event.put(std::move(recHitCollection));

}

