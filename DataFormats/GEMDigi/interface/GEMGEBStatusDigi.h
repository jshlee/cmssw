#ifndef DataFormats_GEMDigi_GEMGEBStatusDigi_H
#define DataFormats_GEMDigi_GEMGEBStatusDigi_H

#include <cstdint>
#include <vector>

class GEMGEBStatusDigi {

 public:
  explicit GEMGEBStatusDigi(uint32_t ZeroSup, 
<<<<<<< HEAD
			    uint8_t InputID, 
=======
>>>>>>> cms-sw/refs/pull/21305/head
			    uint16_t Vwh, 
			    uint16_t ErrorC, 
			    uint16_t OHCRC, 
			    uint16_t Vwt,
<<<<<<< HEAD
=======
			    uint8_t InputID, 
>>>>>>> cms-sw/refs/pull/21305/head
			    uint8_t InFu,
			    uint8_t Stuckd,
			    std::vector<uint8_t> v_GEBflags);

  GEMGEBStatusDigi(){}
  
  uint32_t getZeroSup() const {return ZeroSup_;}   ///<Returns Zero Suppression flags
<<<<<<< HEAD
  uint8_t  getInputID() const {return InputID_;}   ///<Returns GLIB input ID
=======
>>>>>>> cms-sw/refs/pull/21305/head
  uint16_t getVwh() const     {return Vwh_;}       ///<Returns VFAT word count (size of VFAT payload)
  uint16_t getErrorC() const  {return ErrorC_;}    ///<Returns thirteen flags in GEM Chamber Header
  uint16_t getOHCRC() const   {return OHCRC_;}     ///<Returns OH CRC 
  uint16_t getVwt() const     {return Vwt_;}       ///<Returns VFAT word count
<<<<<<< HEAD
=======
  uint8_t  getInputID() const {return InputID_;}   ///<Returns GLIB input ID
>>>>>>> cms-sw/refs/pull/21305/head
  uint8_t  getInFu() const    {return InFu_;}      ///<Returns InFIFO underflow flag
  uint8_t  getStuckd() const  {return Stuckd_;}    ///<Returns Stuck data flag
  const std::vector<uint8_t> & get_GEBflags() const {return v_GEBflags_;}

 private:

  uint32_t ZeroSup_;
<<<<<<< HEAD
  uint8_t  InputID_;   
=======
>>>>>>> cms-sw/refs/pull/21305/head
  uint16_t Vwh_;
  uint16_t ErrorC_;
  uint16_t OHCRC_;     
  uint16_t Vwt_;      
<<<<<<< HEAD
=======
  uint8_t  InputID_;   
>>>>>>> cms-sw/refs/pull/21305/head
  uint8_t  InFu_;    
  uint8_t  Stuckd_; 
  std::vector<uint8_t> v_GEBflags_;

};
#endif
