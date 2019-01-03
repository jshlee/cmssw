#ifndef CondFormats_GEMObjects_GEMELMap_h
#define CondFormats_GEMObjects_GEMELMap_h

#include "CondFormats/Serialization/interface/Serializable.h"
#include <string>
#include <vector>

class GEMROmap;

class GEMELMap {
 public:
  GEMELMap();
  explicit GEMELMap(const std::string & version);

  virtual ~GEMELMap();

  const std::string & version() const;
  void convert(GEMROmap & romap);
  void convertDummy(GEMROmap & romap);

  struct GEMChamberMap{
    std::vector<unsigned int> fedId;
    std::vector<uint8_t> amcNum;
    std::vector<uint8_t> gebId;
    std::vector<int> gemNum;
    std::vector<int> vfatVer; 

    COND_SERIALIZABLE;
  };

  struct GEMVFatMap {
    std::vector<int> gemNum;
    std::vector<uint16_t> vfatAdd;
    std::vector<int> vfatType;
    std::vector<int> iEta;
    std::vector<int> localPhi;

    COND_SERIALIZABLE;
  };
  struct GEMStripMap {
    std::vector<int> vfatType;
    std::vector<int> vfatCh;
    std::vector<int> vfatStrip;
 
    COND_SERIALIZABLE;
  };

  std::vector<GEMChamberMap>  theChamberMap_;
  std::vector<GEMVFatMap>  theVFatMap_;
  std::vector<GEMStripMap> theStripMap_;
  
 private:
  std::string theVersion;

  COND_SERIALIZABLE;
  
 public:
  // size of ID bits
  static const int vfatVerV3_  = 3;       // VFAT v3
  static const int vfatTypeV3_ = 11;       // VFAT v3
  static const int chipIdBits_ = 12;     // ID size from VFat
  static const int chipIdMask_ = 0xfff;  // chipId mask for 12 bits
  static const int gebIdBits_  = 5;      // ID size from GEB
  static const int maxGEBs_    = 24;     // 24 gebs per amc
  static const int maxVFatGE0_ = 12;     // vFat per eta partition, not known yet for ME0
  static const int maxVFatGE11_= 3;      // vFat per eta partition in GE11
  static const int maxVFatGE21_= 6;      // vFat per eta partition in GE21
  static const int maxChan_    = 128;    // channels per vFat
  static const int amcBX_      = 25;     // amc BX to get strip bx
  static const int maxEtaPartition_ = 8; // etaPartitions in chamber

};
#endif // GEMELMap_H
