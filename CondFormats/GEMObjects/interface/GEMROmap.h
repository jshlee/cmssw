#ifndef CondFormats_GEMObjects_GEMROmap_h
#define CondFormats_GEMObjects_GEMROmap_h
#include "DataFormats/MuonDetId/interface/GEMDetId.h"
#include <map>

class GEMROmap{
 public:
  
  struct chamEC{
    unsigned int fedId;
    uint8_t amcNum;
    uint8_t gebId;
    bool operator < (const chamEC& r) const{
      if (fedId == r.fedId){
        if ( amcNum == r.amcNum){
          return gebId < r.gebId;
        }
	else{
          return amcNum < r.amcNum;
	}
      }
      else{
	return fedId < r.fedId;
      }
    }
  };
  
  struct chamDC{
    GEMDetId detId;
    int vfatVer;
    bool operator < (const chamDC& r) const{
      // vfatVer is not needed
      return  detId <  r.detId;

      /* if (detId == r.detId){ */
      /*   return  vfatVer < r.vfatVer; */
      /* } */
      /* else{ */
      /*   return  detId <  r.detId; */
      /* } */
      
    }
  };

  struct vfatEC{
    uint16_t vfatAdd;
    GEMDetId detId;
    bool operator < (const vfatEC& r) const{
      if (vfatAdd == r.vfatAdd){
        return detId < r.detId;
      }
      else{
        return vfatAdd  < r.vfatAdd;
      }
    }
  };

  struct vfatDC{
    int vfatType;
    GEMDetId detId;
    int localPhi;
    bool operator < (const vfatDC& r)  const{
      if (vfatType == r.vfatType){
        if (detId == r.detId){
           return localPhi < r.localPhi;
        }
        else{
          return detId < r.detId;
        }
      }
      else{
        return vfatType < r.vfatType;
      }
    }
  };

  struct channelNum{
    int vfatType;
    int chNum;
    bool operator < (const channelNum& c) const{
      if (vfatType == c.vfatType)
        return chNum < c.chNum;
      else
        return vfatType < c.vfatType;
    }
  };

  struct stripNum{
    int vfatType;
    int stNum;
    bool operator < (const stripNum& s) const{
      if (vfatType == s.vfatType) 
        return stNum < s.stNum;
      else
        return vfatType < s.vfatType;
    }
  };

  GEMROmap(){};
  
  bool isValidChipID(const vfatEC& r) const {
    return vMapED_.find(r) != vMapED_.end();
  }
  const chamDC& chamberPos(const chamEC& r) const {return chamED_.at(r);}
  const chamEC& chamberPos(const chamDC& r) const {return chamDE_.at(r);}
  const chamEC& chamberPos(const GEMDetId& r) const {return chamDE_.at( chamDC{r.chamberId(),0} ); }
  
  void add(chamEC e,chamDC d) {chamED_[e]=d;}
  void add(chamDC d,chamEC e) {chamDE_[d]=e;}
  
  const std::map<chamEC, chamDC> * getChamMap() const {return &chamED_;}

  void add(vfatEC e,vfatDC d) {vMapED_[e]=d;}
  void add(vfatDC d,vfatEC e) {vMapDE_[d]=e;}

  const vfatDC& vfatPos(const vfatEC& r) const {return vMapED_.at(r);}
  const vfatEC& vfatPos(const vfatDC& r) const {return vMapDE_.at(r);}

  const std::map<vfatEC, vfatDC> * getVfatMap() const {return &vMapED_;}
  
  void add(channelNum c, stripNum s) {chStMap_[c]=s;} 
  void add(stripNum s, channelNum c) {stChMap_[s]=c;} 
 
  const channelNum& hitPos(const stripNum& s) const {return stChMap_.at(s);}
  const stripNum& hitPos(const channelNum& c) const {return chStMap_.at(c);}

 private:
  std::map<chamEC,chamDC> chamED_;
  std::map<chamDC,chamEC> chamDE_;

  std::map<vfatEC, vfatDC> vMapED_;
  std::map<vfatDC, vfatEC> vMapDE_;

  std::map<channelNum, stripNum> chStMap_;
  std::map<stripNum, channelNum> stChMap_;
  
};
#endif
