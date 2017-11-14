#ifndef GEMEMap_H
#define GEMEMap_H
#include "CondFormats/Serialization/interface/Serializable.h"
#include <string>
#include <vector>
#include <iostream>

class GEMROmap;

class GEMEMap {
 public:
  GEMEMap();
  explicit GEMEMap(const std::string & version);

  virtual ~GEMEMap();

  const std::string & version() const;
  GEMROmap* convert() const;
  GEMROmap* convertDummy() const;

  int consistent(int verbose, std::ostream &out=std::cout) const;
  //int not_consistent() const { return !not_consistent(deepCheck); }

  struct GEMEMapItem {
    int ChamberID;
    std::vector<int> VFatIDs;
    std::vector<int> positions;

    int consistent() const;
    int not_consistent() const;
    void print(std::ostream &out, int idx) const;
    void printLast(std::ostream &out=std::cout) const;

    friend std::ostream& operator<<(std::ostream &out, const GEMEMap::GEMEMapItem &item)
    { item.printLast(out); return out; }

    COND_SERIALIZABLE;
  };  
  struct GEMVFatMaptype {
    int VFATmapTypeId;
    std::vector<int> vfat_position;
    std::vector<int> z_direction;
    std::vector<int> iEta;
    std::vector<int> iPhi;
    std::vector<int> depth;
    std::vector<int> strip_number;
    std::vector<int> vfat_chnnel_number;
    std::vector<uint16_t> vfatId;
    std::vector<int> sec; 

    int consistent() const;
    int not_consistent() const; // returns 0 or error code
    void print(std::ostream &out, int idx) const;
    void printLast(std::ostream &out=std::cout) const;

    friend std::ostream& operator<<(std::ostream &out, const GEMEMap::GEMVFatMaptype &mt)
    { mt.printLast(out); return out; }

    COND_SERIALIZABLE;
  };
  struct GEMVFatMapInPos {
    int position;
    int VFATmapTypeId;

    int not_consistent() const { return 0; } // dummy function

    COND_SERIALIZABLE;
  };

  std::vector<GEMEMapItem>     theEMapItem;
  std::vector<GEMVFatMaptype>  theVFatMaptype;
  std::vector<GEMVFatMapInPos> theVFatMapInPos;

 private:
  std::string theVersion;

  COND_SERIALIZABLE;
};

#endif // GEMEMap_H
