/** \unpacker for gem
 *  \author J. Lee - UoS
 */
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "DataFormats/FEDRawData/interface/FEDRawDataCollection.h"
#include "DataFormats/FEDRawData/interface/FEDNumbering.h"

#include "EventFilter/GEMRawToDigi/plugins/GEMRawToDigiModule.h"

using namespace gem;

GEMRawToDigiModule::GEMRawToDigiModule(const edm::ParameterSet & pset) :
  fed_token(consumes<FEDRawDataCollection>( pset.getParameter<edm::InputTag>("InputLabel") )),
  useDBEMap_(pset.getParameter<bool>("useDBEMap")),
  unPackStatusDigis_(pset.getParameter<bool>("unPackStatusDigis"))
{
  produces<GEMDigiCollection>(); 
  if (unPackStatusDigis_){
    produces<GEMVfatStatusDigiCollection>("vfatStatus");
    produces<GEMGEBStatusDigiCollection>("GEBStatus");
  }
}

void GEMRawToDigiModule::fillDescriptions(edm::ConfigurationDescriptions & descriptions)
{
  edm::ParameterSetDescription desc;
  desc.add<edm::InputTag>("InputLabel", edm::InputTag("rawDataCollector")); 
  desc.add<bool>("useDBEMap", false);
  desc.add<bool>("unPackStatusDigis", false);
  descriptions.add("muonGEMDigis", desc);  
}

std::shared_ptr<GEMROmap> GEMRawToDigiModule::globalBeginRun(edm::Run const&, edm::EventSetup const& iSetup) const
{
  if (useDBEMap_){
    edm::ESHandle<GEMEMap> gemEMapRcd;
    iSetup.get<GEMEMapRcd>().get(gemEMapRcd);
    auto gemEMap = std::make_unique<GEMEMap>(*(gemEMapRcd.product()));
    return std::make_shared<GEMROmap>(*(gemEMap->convert()));
  }
  else {
    // no eMap, using dummy
    auto gemEMap = std::make_unique<GEMEMap>();
    return std::make_shared<GEMROmap>(*(gemEMap->convertDummy()));
  }
}

void GEMRawToDigiModule::produce(edm::StreamID iID, edm::Event & iEvent, edm::EventSetup const&) const
{
  auto outGEMDigis = std::make_unique<GEMDigiCollection>();
  auto outVfatStatus = std::make_unique<GEMVfatStatusDigiCollection>();
  auto outGEBStatus = std::make_unique<GEMGEBStatusDigiCollection>();
  // Take raw from the event
  edm::Handle<FEDRawDataCollection> fed_buffers;
  iEvent.getByToken( fed_token, fed_buffers );
  
  auto gemROMap = runCache(iEvent.getRun().index());
  
  for (unsigned int id=FEDNumbering::MINGEMFEDID; id<=FEDNumbering::MINGEMFEDID; ++id){ 
    const FEDRawData& fedData = fed_buffers->FEDData(id);
    
    int nWords = fedData.size()/sizeof(uint64_t);
    LogDebug("GEMRawToDigiModule") <<" words " << nWords;
    if (nWords<5) continue;
    const unsigned char * data = fedData.data();
    
    auto amc13Event = std::make_unique<AMC13Event>();
    
    const uint64_t* word = reinterpret_cast<const uint64_t* >(data);
    
    amc13Event->setCDFHeader(*word);
    amc13Event->setAMC13header(*(++word));

    // Readout out AMC headers
    for (unsigned short i = 0; i < amc13Event->nAMC(); ++i)
      amc13Event->addAMCheader(*(++word));
    
    // Readout out AMC payloads
    for (unsigned short i = 0; i < amc13Event->nAMC(); ++i){
      auto amcData = std::make_unique<AMCdata>();
      amcData->setAMCheader1(*(++word));      
      amcData->setAMCheader2(*(++word));
      amcData->setGEMeventHeader(*(++word));
      uint16_t amcId = amcData->boardId();

      // Fill GEB
      for (unsigned short j = 0; j < amcData->gdCount(); ++j){
	auto gebData = std::make_unique<GEBdata>();
	gebData->setChamberHeader(*(++word));
	
	unsigned int m_nvb = gebData->vwh() / 3; // number of VFAT2 blocks. Eventually add here sanity check
	uint16_t gebId = gebData->inputID();
	GEMDetId gemId(-1,1,1,1,1,0); // temp ID
	for (unsigned short k = 0; k < m_nvb; k++){
	  auto vfatData = std::make_unique<VFATdata>();
	  vfatData->read_fw(*(++word));
	  vfatData->read_sw(*(++word));
	  vfatData->read_tw(*(++word));
	  gebData->addVFAT(*vfatData);
	  
	  uint16_t bc=vfatData->bc();
	  uint8_t b1010=vfatData->b1010();
	  uint8_t b1100=vfatData->b1100();
	  uint8_t b1110=vfatData->b1110();
	  uint16_t ChipID=vfatData->chipID();
	  uint16_t crc = vfatData->crc();
	  uint16_t crc_check = vfatData->checkCRC();
	  bool Quality = (b1010==10) && (b1100==12) && (b1110==14) && (crc==crc_check);

	  if (crc!=crc_check) edm::LogWarning("GEMRawToDigiModule") << "DIFFERENT CRC :"<<crc<<"   "<<crc_check;
	  if (!Quality) edm::LogWarning("GEMRawToDigiModule") << "Quality "<< Quality
							      << " b1010 "<< int(b1010)
							      << " b1100 "<< int(b1100)
							      << " b1110 "<< int(b1110);
	  
	  uint32_t vfatId = (amcId << (VFATdata::sizeChipID+GEBdata::sizeGebID)) | (gebId << VFATdata::sizeChipID) | ChipID;
	  //need to add gebId to DB
	  if (useDBEMap_) vfatId = ChipID;
	    
	  //check if ChipID exists.
	  GEMROmap::eCoord ec;
	  ec.vfatId = vfatId;
          ec.amcId = amcId;
          ec.gebId = gebId;
       
	  if (!gemROMap->isValidChipID(ec)){
	    edm::LogWarning("GEMRawToDigiModule") << "InValid ChipID :"<<ec.vfatId;
	    continue;
	  }
	  
	  for (int chan = 0; chan < VFATdata::nChannels; ++chan) {
	    uint8_t chan0xf = 0;
	    if (chan < 64) chan0xf = ((vfatData->lsData() >> chan) & 0x1);
	    else chan0xf = ((vfatData->msData() >> (chan-64)) & 0x1);

	    // no hits
	    if(chan0xf==0) continue;
            
	    GEMROmap::dCoord dc = gemROMap->hitPosition(ec);
            
	    int bx = bc-25;
	    gemId = dc.gemDetId;
         
            GEMROmap::channelNum chMap;
            chMap.vfatType = dc.vfatType;
            chMap.chNum = chan;
            GEMROmap::stripNum stMap = gemROMap->hitPosition(chMap);
            int maxVFat = maxVFatGE11_;
            if (gemId.station() == 2) maxVFat = maxVFatGE21_;
            int stripId = stMap.stNum + dc.iPhi%maxVFat*maxChan_;    

	    GEMDigi digi(stripId,bx);
	    LogDebug("GEMRawToDigiModule") <<" vfatId "<<ec.vfatId
					   <<" gemDetId "<< gemId
					   <<" chan "<< chMap.chNum
					   <<" strip "<< stripId
					   <<" bx "<< digi.bx();

	    outGEMDigis.get()->insertDigi(gemId,digi);	    
	  }

          if (unPackStatusDigis_){
	    GEMVfatStatusDigi vfatStatus(vfatData->lsData(), vfatData->msData(),
					 crc, vfatData->crc_calc(),
					 b1010, b1100, b1110, vfatData->flag(),
					 vfatData->isBlockGood());
            outVfatStatus.get()->insertDigi(gemId,vfatStatus);
	  }
	  
	}
	
	gebData->setChamberTrailer(*(++word));
        if (unPackStatusDigis_){
          GEMGEBStatusDigi gebStatus(gebData->zeroSup(),
                                     gebData->vwh(),
                                     gebData->errorC(),
                                     gebData->ohCRC(),
                                     gebData->vwt(),
                                     gebData->inFu(),
                                     gebData->inputID(),				     
                                     gebData->stuckd(),
                                     gebData->getGEBflag());
          outGEBStatus.get()->insertDigi(gemId.chamberId(),gebStatus); 
        }
		  	
	amcData->addGEB(*gebData);
      }
      
      amcData->setGEMeventTrailer(*(++word));
      amcData->setAMCTrailer(*(++word));
      amc13Event->addAMCpayload(*amcData);
    }
    
    amc13Event->setAMC13trailer(*(++word));
    amc13Event->setCDFTrailer(*(++word));
  }
  
  iEvent.put(std::move(outGEMDigis));
  if (unPackStatusDigis_){
    iEvent.put(std::move(outVfatStatus), "vfatStatus");
    iEvent.put(std::move(outGEBStatus), "GEBStatus");
  }
}
