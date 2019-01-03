#include "CondFormats/GEMObjects/interface/GEMELMap.h"
#include "CondFormats/GEMObjects/interface/GEMROmap.h"
#include "DataFormats/MuonDetId/interface/GEMDetId.h"
#include "DataFormats/FEDRawData/interface/FEDNumbering.h"

GEMELMap::GEMELMap():
  theVersion("") {}

GEMELMap::GEMELMap(const std::string & version):
  theVersion(version) {}

GEMELMap::~GEMELMap() {}

const std::string & GEMELMap::version() const {
  return theVersion;
}

void GEMELMap::convert(GEMROmap & romap) {
  // fed->amc->geb mapping to GEMDetId
  for (auto imap : theChamberMap_) {
    for (unsigned int ix=0;ix<imap.fedId.size();ix++) {
      GEMROmap::chamEC ec;
      ec.fedId = imap.fedId[ix];
      ec.amcNum = imap.amcNum[ix];
      ec.gebId = imap.gebId[ix];

      GEMROmap::chamDC dc;
      dc.detId = GEMDetId((imap.gemNum[ix] > 0) ? 1:-1, 1, abs(imap.gemNum[ix]/1000), abs(imap.gemNum[ix]/100%10), abs(imap.gemNum[ix]%100), 0);
      dc.vfatVer = imap.vfatVer[ix];
      
      romap.add(ec, dc);
      romap.add(dc, ec);
    }
  }
  // chamberType to vfatType
  for (auto imap : theVFatMap_) {
    for (unsigned int ix=0;ix<imap.vfatAdd.size();ix++) {
      GEMROmap::vfatEC ec;
      ec.detId = GEMDetId((imap.gemNum[ix] > 0) ? 1:-1, 1, abs(imap.gemNum[ix]/1000), abs(imap.gemNum[ix]/100%10), abs(imap.gemNum[ix]%100), 0);
      ec.vfatAdd = imap.vfatAdd[ix] & chipIdMask_;
      
      GEMROmap::vfatDC dc;
      dc.vfatType = imap.vfatType[ix];
      dc.detId = GEMDetId(ec.detId.region(), 1, ec.detId.station(), ec.detId.layer(), ec.detId.chamber(), imap.iEta[ix]);
      dc.localPhi = imap.localPhi[ix];
  
      romap.add(ec, dc);
      romap.add(dc, ec);
    }
  }
  // channel mapping
  for (auto imap : theStripMap_) {
    for (unsigned int ix=0;ix<imap.vfatType.size();ix++) {
      GEMROmap::channelNum cMap;
      cMap.vfatType = imap.vfatType[ix];
      cMap.chNum = imap.vfatCh[ix];

      GEMROmap::stripNum sMap;
      sMap.vfatType = imap.vfatType[ix];
      sMap.stNum = imap.vfatStrip[ix];

      romap.add(cMap, sMap);
      romap.add(sMap, cMap);
    }
  }
}

void GEMELMap::convertDummy(GEMROmap & romap) {
  // 12 bits for vfat, 5 bits for geb, 8 bit long GLIB serial number
  unsigned int fedId = FEDNumbering::MINGEMFEDID;
  uint8_t amcNum = 1; //amc
  uint8_t gebId = 0; 

  for (int re = -1; re <= 1; re = re+2) {
    for (int st = GEMDetId::minStationId; st<=GEMDetId::maxStationId; ++st) {
      int maxVFat = maxVFatGE11_;
      if (st == 2) maxVFat = maxVFatGE21_;
      if (st == 0) maxVFat = maxVFatGE0_;

      for (int ch = 1; ch<=GEMDetId::maxChamberId; ++ch) {
        for (int ly = 1; ly<=GEMDetId::maxLayerId; ++ly) {
          GEMDetId gemId(re, 1, st, ly, ch, 0);

          GEMROmap::chamEC ec;
          ec.fedId = fedId;
          ec.gebId = gebId;
          ec.amcNum = amcNum;

          GEMROmap::chamDC dc;
          dc.detId = gemId;
          dc.vfatVer = vfatVerV3_;
     
          romap.add(ec, dc);
          romap.add(dc, ec);
          // 1 geb per chamber
          gebId++;

          uint16_t chipPos = 0;
          for (int lphi = 0; lphi < maxVFat; ++lphi) {
            for (int roll = 1; roll<=maxEtaPartition_; ++roll) {
              GEMROmap::vfatEC vec;
              vec.vfatAdd = chipPos;
              vec.detId = gemId;
      
              GEMROmap::vfatDC vdc;
              vdc.vfatType = vfatTypeV3_;// > 10 is vfat v3
              vdc.detId = GEMDetId(re, 1, st, ly, ch, roll);
              vdc.localPhi = lphi;
    
              romap.add(vec,vdc);
              romap.add(vdc,vec);
              chipPos++;
            }
          }

          // 5 bits for gebId 
          if (gebId == maxGEBs_) {
            // 24 gebs per amc
            gebId = 0;
            amcNum++;
          }
        }
      }
    }
  }

  for (int i = 0; i < maxChan_; ++i) {
    // only 1 vfat type for dummy map
    GEMROmap::channelNum cMap;
    cMap.vfatType = vfatTypeV3_;
    cMap.chNum = i;

    GEMROmap::stripNum sMap;
    sMap.vfatType = vfatTypeV3_;
    sMap.stNum = i;

    romap.add(cMap, sMap);
    romap.add(sMap, cMap);
  }
}
