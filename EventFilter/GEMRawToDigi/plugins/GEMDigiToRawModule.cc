/** \packer for gem
 *  \author J. Lee - UoS
 */
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/FEDRawData/interface/FEDRawDataCollection.h"
#include "DataFormats/FEDRawData/interface/FEDHeader.h"
#include "DataFormats/FEDRawData/interface/FEDTrailer.h"
#include "DataFormats/FEDRawData/interface/FEDRawData.h"
#include "DataFormats/FEDRawData/interface/FEDNumbering.h"

#include "EventFilter/GEMRawToDigi/plugins/GEMDigiToRawModule.h"

using namespace gem;

GEMDigiToRawModule::GEMDigiToRawModule(const edm::ParameterSet & pset):
  event_type_(pset.getParameter<int>("eventType")),
  digi_token(consumes<GEMDigiCollection>( pset.getParameter<edm::InputTag>("gemDigi") )),
  useDBEMap_(pset.getParameter<bool>("useDBEMap"))
{
  produces<FEDRawDataCollection>();
}

void GEMDigiToRawModule::fillDescriptions(edm::ConfigurationDescriptions & descriptions)
{
  edm::ParameterSetDescription desc;
  desc.add<edm::InputTag>("gemDigi", edm::InputTag("simMuonGEMDigis"));
  desc.add<int>("eventType", 0);
  desc.add<bool>("useDBEMap", false);
  descriptions.add("gemPackerDefault", desc);  
}

std::shared_ptr<GEMROmap> GEMDigiToRawModule::globalBeginRun(edm::Run const&, edm::EventSetup const& iSetup) const
{
  auto gemORmap = std::make_shared<GEMROmap>();
  if (useDBEMap_) {
    edm::ESHandle<GEMELMap> gemEMapRcd;
    iSetup.get<GEMELMapRcd>().get(gemEMapRcd);
    auto gemEMap = std::make_unique<GEMELMap>(*(gemEMapRcd.product()));
    gemEMap->convert(*gemORmap);
    gemEMap.reset();    
  }
  else {
    // no EMap in DB, using dummy
    auto gemEMap = std::make_unique<GEMELMap>();
    gemEMap->convertDummy(*gemORmap);
    gemEMap.reset();    
  }
  return gemORmap;
}

void GEMDigiToRawModule::produce(edm::StreamID iID, edm::Event & iEvent, edm::EventSetup const&) const
{
  auto fedRawDataCol = std::make_unique<FEDRawDataCollection>();

  // Take digis from the event
  edm::Handle<GEMDigiCollection> gemDigis;
  iEvent.getByToken( digi_token, gemDigis );

  auto gemROMap = runCache(iEvent.getRun().index());
  
  std::vector<std::unique_ptr<AMC13Event>> amc13Events;
  amc13Events.reserve(FEDNumbering::MAXGEMFEDID-FEDNumbering::MINGEMFEDID+1);
  
  unsigned int fedId = 0;
  uint8_t amcNum = 0, gebId = 0;
  std::unique_ptr<AMC13Event> amc13Event;
  std::unique_ptr<AMCdata> amcData;
  std::unique_ptr<GEBdata> gebData;
  
  const std::map<GEMROmap::vfatEC,GEMROmap::vfatDC> *vfatMapED = gemROMap->getVfatMap();
  for (auto ro=vfatMapED->begin(); ro!=vfatMapED->end(); ++ro) {
    
    GEMROmap::vfatEC vfat_ec = ro->first;
    GEMROmap::vfatDC vfat_dc = ro->second;
    GEMDetId  gemId  = vfat_dc.detId;

    GEMROmap::chamEC ec = gemROMap->chamberPos(gemId);
    GEMROmap::chamDC dc = gemROMap->chamberPos(ec);
    
    if (fedId != ec.fedId || !amc13Event) {
      fedId = ec.fedId;
      amc13Event = std::make_unique<AMC13Event>();
    }
    if (amcNum != ec.amcNum || !amcData) {
      amcNum = ec.amcNum;
      amcData = std::make_unique<AMCdata>();
    }      
    if (gebId != ec.gebId || !gebData) {
      gebId = ec.gebId;
      gebData = std::make_unique<GEBdata>();
    }
    
    uint8_t   EC     = 0;               ///<Event Counter, 8 bits
    uint16_t  vfatId = vfat_ec.vfatAdd; ///<Calculated chip position
    
    for (uint16_t bc = 0; bc < 2*GEMELMap::amcBX_; ++bc) {
      bool hasDigi = false;

      uint64_t lsData  = 0;             ///<channels from 1to64 
      uint64_t msData  = 0;             ///<channels from 65to128
	
      GEMDigiCollection::Range range = gemDigis->get(gemId);
      for (GEMDigiCollection::const_iterator digiIt = range.first; digiIt!=range.second; ++digiIt) {

        const GEMDigi & digi = (*digiIt);
        if (digi.bx() != bc-GEMELMap::amcBX_) continue;
	  
        int localStrip = digi.strip() - vfat_dc.localPhi*GEMELMap::maxChan_;	  
        // skip strips not in current vFat

        if (localStrip < 0 || localStrip > GEMELMap::maxChan_ -1) continue;

        hasDigi = true;
        GEMROmap::stripNum stMap = {vfat_dc.vfatType, localStrip};
        GEMROmap::channelNum chMap = gemROMap->hitPos(stMap);
	  
        int chan = chMap.chNum;
        uint64_t oneBit = 0x1;
        if (chan < 64) lsData = lsData | (oneBit << chan);
        else msData = msData | (oneBit << (chan-64));

        LogDebug("GEMDigiToRawModule") <<" vfatId "<<int(vfatId)
                                       <<" gemDetId "<< gemId
                                       <<" chan "<< chMap.chNum
                                       <<" strip "<< digi.strip()
                                       <<" bx "<< digi.bx();
	  
      }
      
      if (!hasDigi) continue;
      // only make vfat with hits
      auto vfatData = std::make_unique<VFATdata>(dc.vfatVer, bc, EC, vfatId, lsData, msData);
      gebData->addVFAT(*vfatData);
    }
    
    bool saveGeb = false;
    bool saveAMC = false;
    bool saveFED = false;
    auto nx = std::next(ro);      
    // last vfat, save
    if (nx == vfatMapED->end()) {
      saveGeb = true;
      saveAMC = true;
      saveFED = true;
    }
    else {
      // check if next vfat is in new geb or amc
      GEMDetId nextId = nx->first.detId;
      GEMROmap::chamEC ecNext = gemROMap->chamberPos(nextId);
      
      if (ecNext.gebId != gebId) saveGeb = true;
      if (ecNext.amcNum != amcNum) saveAMC = true;
      if (ecNext.fedId != fedId) saveFED = true;
    }
      
    if (!gebData->vFATs()->empty() && saveGeb) {
      // gebData->setVfatWordCnt(gebData->vFATs()->size()*3);
      // gebData->setInputID(gebId);	
      gebData->setChamberHeader(gebData->vFATs()->size()*3, gebId);
      gebData->setChamberTrailer(0, 0, gebData->vFATs()->size()*3);
      amcData->addGEB(*gebData);
    }
    if (!amcData->gebs()->empty() && saveAMC) {
      // amcData->setboardId(amcNum);
      // amcData->setbx(GEMELMap::amcBX_);
      // amcData->setdavCnt(amcData->gebs()->size());
      amcData->setAMCheader1(0, GEMELMap::amcBX_, 0, 0);
      amcData->setAMCheader2(amcNum, 0, 1);
      amcData->setGEMeventHeader(amcData->gebs()->size(), 0);
        
      amc13Event->addAMCpayload(*amcData);
    }
  
    if (!amc13Event->getAMCpayloads()->empty() && saveFED) {
      // CDFHeader
      uint8_t Evt_ty = event_type_;
      uint32_t LV1_id = iEvent.id().event();
      uint16_t BX_id = iEvent.bunchCrossing();
      amc13Event->setCDFHeader(Evt_ty, LV1_id, BX_id, fedId);

      // AMC13header
      uint8_t CalTyp = 1;
      uint8_t nAMC = amc13Event->getAMCpayloads()->size();
      uint32_t OrN = 2;
      amc13Event->setAMC13Header(CalTyp, nAMC, OrN);

      for (unsigned short i = 0; i < amc13Event->nAMC(); ++i) {
        uint32_t AMC_size = 0;
        uint8_t Blk_No = 0;
        uint8_t AMC_No = 0;
        uint16_t BoardID = 0;
        amc13Event->addAMCheader(AMC_size, Blk_No, AMC_No, BoardID);
      }
    
      //AMC13 trailer
      uint8_t Blk_NoT = 0;
      uint8_t LV1_idT = 0;
      uint16_t BX_idT = BX_id;
      amc13Event->setAMC13Trailer(Blk_NoT, LV1_idT, BX_idT);
      //CDF trailer
      uint32_t EvtLength = 0;
      amc13Event->setCDFTrailer(EvtLength);  
      amc13Events.emplace_back(std::move(amc13Event));
    }// finished making amc13Event data
  }
  
  // read out amc13Events into fedRawData
  for (const auto & amc13e : amc13Events) {
    std::vector<uint64_t> words;    
    words.emplace_back(amc13e->getCDFHeader());
    words.emplace_back(amc13e->getAMC13Header());    
    
    for (const auto & w: *amc13e->getAMCheaders())
      words.emplace_back(w);

    for (const auto & amc : *amc13e->getAMCpayloads()) {
      words.emplace_back(amc.getAMCheader1());
      words.emplace_back(amc.getAMCheader2());
      words.emplace_back(amc.getGEMeventHeader());
      
      for (const auto & geb: *amc.gebs()) {
        words.emplace_back(geb.getChamberHeader());

        for (const auto & vfat: *geb.vFATs()) {
          words.emplace_back(vfat.get_fw());
          words.emplace_back(vfat.get_sw());
          words.emplace_back(vfat.get_tw());
        }
	
        words.emplace_back(geb.getChamberTrailer());
      }
      
      words.emplace_back(amc.getGEMeventTrailer());
      words.emplace_back(amc.getAMCTrailer());
    }
    
    words.emplace_back(amc13e->getAMC13Trailer());
    words.emplace_back(amc13e->getCDFTrailer());

    FEDRawData & fedRawData = fedRawDataCol->FEDData(amc13e->sourceId());
    
    int dataSize = (words.size()) * sizeof(uint64_t);
    fedRawData.resize(dataSize);
    
    uint64_t * w = reinterpret_cast<uint64_t* >(fedRawData.data());  
    for (const auto & word: words) *(w++) = word;
    
    LogDebug("GEMDigiToRawModule") <<" words " << words.size();
  }

  iEvent.put(std::move(fedRawDataCol));
}
