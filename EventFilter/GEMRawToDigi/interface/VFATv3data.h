#ifndef EventFilter_GEMRawToDigi_VFATv3data_h
#define EventFilter_GEMRawToDigi_VFATv3data_h

#include <stdint.h>

namespace gem {
  class VFATv3data 
  {
  public:    
    VFATv3data(){}
    ~VFATv3data(){}
  VFATv3data(const uint8_t &Pos_, 
	   const uint8_t &CRCcheck_,
	   const uint8_t &Header_,
	   const uint8_t &EC_,
	   const uint16_t &BC_,
	   const uint64_t &lsData_, 
	   const uint64_t &msData_, 
	   const uint16_t &crc_,
	   const uint16_t &crc_calc_,
	   const int &SlotNumber_,
	   const bool &isBlockGood_) : 
    m_Pos(Pos_),
      m_CRCcheck(CRCcheck_),
      m_Header(Header_),
      m_EC(EC_),
      m_BC(BC_),
      m_lsData(lsData_),
      m_msData(msData_),
      m_crc(crc_),
      m_crc_calc(crc_calc_),
      m_SlotNumber(SlotNumber_),
      m_isBlockGood(isBlockGood_)
      {
	if (m_crc == 0)
	  m_crc = this->checkCRC();
      }

    //!Read first word from the block.
    void read_fw(uint64_t word)
    {
      m_Pos = 0x3f & (word >> 56);
      m_CRCcheck = 0xff & (word >> 48);
      m_Header = 0xff & (word >> 40);
      m_EC = 0xff & (word >> 32);
      m_BC = 0xffff & (word >> 16);
      m_msData = 0xffff000000000000 & (word << 48);      
    }
    uint64_t get_fw() const
    {
      return
	(static_cast<uint64_t>(m_Pos & 0x3f) <<  56) |
	(static_cast<uint64_t>(m_CRCcheck & 0xff) <<  48) |
	(static_cast<uint64_t>(m_Header & 0xff) <<  40) |
	(static_cast<uint64_t>(m_EC & 0xff) <<  32) |
	(static_cast<uint64_t>(m_BC & 0xffff) <<  16) |
	(static_cast<uint64_t>(m_msData & 0xffff000000000000) >> 48);
    }

    //!Read second word from the block.
    void read_sw(uint64_t word)
    {
      m_msData = m_msData | (0x0000ffffffffffff & word >> 16);
      m_lsData = 0xffff000000000000 & (word << 48);
    }
    uint64_t get_sw() const
    {
      return
	(static_cast<uint64_t>(m_msData & 0x0000ffffffffffff) <<  16) |
	(static_cast<uint64_t>(m_lsData & 0xffff000000000000) >>  48);
    }
    
    //!Read third word from the block.
    void read_tw(uint64_t word)
    {
      m_lsData = m_lsData | (0x0000ffffffffffff & word >> 16);
      m_crc = 0xffff & word;
    }
    // make write_word function
    uint64_t get_tw() const
    {
      return
	(static_cast<uint64_t>(m_lsData & 0x0000ffffffffffff) <<  16) |
	(static_cast<uint64_t>(m_crc));
    }

    uint8_t   pos        () const { return m_Pos;        }
    uint16_t  bc         () const { return m_BC;         }
    uint8_t   header     () const { return m_Header;     }
    uint8_t   ec         () const { return m_EC;         }
    uint8_t   crcCheck   () const { return m_CRCcheck;   }
    uint64_t  lsData     () const { return m_lsData;     }
    uint64_t  msData     () const { return m_msData;     }
    uint16_t  crc        () const { return m_crc;        }
    uint16_t  crc_calc   () const { return m_crc_calc;   }
    int       slotNumber () const { return m_SlotNumber; }
    bool      isBlockGood() const { return m_isBlockGood;}
    
    uint16_t crc_cal(uint16_t crc_in, uint16_t dato)
    {
      uint16_t v = 0x0001;
      uint16_t mask = 0x0001;
      uint16_t d=0x0000;
      uint16_t crc_temp = crc_in;
      unsigned char datalen = 16;
      for (int i=0; i<datalen; i++){
	if (dato & v) d = 0x0001;
	else d = 0x0000;
	if ((crc_temp & mask)^d) crc_temp = crc_temp>>1 ^ 0x8408;
	else crc_temp = crc_temp>>1;
	v<<=1;
      }
      return(crc_temp);
    }
    
    uint16_t checkCRC()
    {
      uint16_t vfatBlockWords[12];
      /* vfatBlockWords[11] = ((0x000f & m_b1010)<<12) | m_BC; */
      /* vfatBlockWords[10] = ((0x000f & m_b1100)<<12) | ((0x00ff & m_EC) <<4) | (0x000f & m_Flag); */
      /* vfatBlockWords[9]  = ((0x000f & m_b1110)<<12) | m_ChipID; */
      /* vfatBlockWords[8]  = (0xffff000000000000 & m_msData) >> 48; */
      /* vfatBlockWords[7]  = (0x0000ffff00000000 & m_msData) >> 32; */
      /* vfatBlockWords[6]  = (0x00000000ffff0000 & m_msData) >> 16; */
      /* vfatBlockWords[5]  = (0x000000000000ffff & m_msData); */
      /* vfatBlockWords[4]  = (0xffff000000000000 & m_lsData) >> 48; */
      /* vfatBlockWords[3]  = (0x0000ffff00000000 & m_lsData) >> 32; */
      /* vfatBlockWords[2]  = (0x00000000ffff0000 & m_lsData) >> 16; */
      /* vfatBlockWords[1]  = (0x000000000000ffff & m_lsData); */

      uint16_t crc_fin = 0xffff;
      for (int i = 11; i >= 1; i--){
	crc_fin = this->crc_cal(crc_fin, vfatBlockWords[i]);
      }
      return(crc_fin);
    }
    
    uint16_t checkCRC(uint8_t b1010, uint16_t BC, uint8_t b1100,
		      uint8_t EC, uint8_t Flag, uint8_t b1110,
		      uint16_t ChipID, uint64_t msData, uint64_t lsData)
    {
      uint16_t vfatBlockWords[12];
      /* vfatBlockWords[11] = ((0x000f & b1010)<<12) | BC; */
      /* vfatBlockWords[10] = ((0x000f & b1100)<<12) | ((0x00ff & EC) <<4) | (0x000f & Flag); */
      /* vfatBlockWords[9]  = ((0x000f & b1110)<<12) | ChipID; */
      /* vfatBlockWords[8]  = (0xffff000000000000 & msData) >> 48; */
      /* vfatBlockWords[7]  = (0x0000ffff00000000 & msData) >> 32; */
      /* vfatBlockWords[6]  = (0x00000000ffff0000 & msData) >> 16; */
      /* vfatBlockWords[5]  = (0x000000000000ffff & msData); */
      /* vfatBlockWords[4]  = (0xffff000000000000 & lsData) >> 48; */
      /* vfatBlockWords[3]  = (0x0000ffff00000000 & lsData) >> 32; */
      /* vfatBlockWords[2]  = (0x00000000ffff0000 & lsData) >> 16; */
      /* vfatBlockWords[1]  = (0x000000000000ffff & lsData); */

      uint16_t crc_fin = 0xffff;
      for (int i = 11; i >= 1; i--){
	crc_fin = this->crc_cal(crc_fin, vfatBlockWords[i]);
      }
      return(crc_fin);
    }
    
    uint16_t checkCRC(const VFATv3data * vfatData)
    {
      uint16_t vfatBlockWords[12]; 
      /* vfatBlockWords[11] = ((0x000f & vfatData->b1010())<<12) | vfatData->bc(); */
      /* vfatBlockWords[10] = ((0x000f & vfatData->b1100())<<12) | ((0x00ff & vfatData->ec()) <<4) | (0x000f & vfatData->flag()); */
      /* vfatBlockWords[9]  = ((0x000f & vfatData->b1110())<<12) | vfatData->chipID(); */
      /* vfatBlockWords[8]  = (0xffff000000000000 & vfatData->msData()) >> 48; */
      /* vfatBlockWords[7]  = (0x0000ffff00000000 & vfatData->msData()) >> 32; */
      /* vfatBlockWords[6]  = (0x00000000ffff0000 & vfatData->msData()) >> 16; */
      /* vfatBlockWords[5]  = (0x000000000000ffff & vfatData->msData()); */
      /* vfatBlockWords[4]  = (0xffff000000000000 & vfatData->lsData()) >> 48; */
      /* vfatBlockWords[3]  = (0x0000ffff00000000 & vfatData->lsData()) >> 32; */
      /* vfatBlockWords[2]  = (0x00000000ffff0000 & vfatData->lsData()) >> 16; */
      /* vfatBlockWords[1]  = (0x000000000000ffff & vfatData->lsData()); */

      uint16_t crc_fin = 0xffff;
      for (int i = 11; i >= 1; i--){
	crc_fin = this->crc_cal(crc_fin, vfatBlockWords[i]);
      }
      return(crc_fin);
    }    

    static const int nChannels = 128;
    
  private:

    uint8_t  m_Pos;                     ///<an 8bit value indicating the VFAT position on this GEB (it can be 0 to 23, so really only 6 bits are used, but anyway...)
    uint8_t  m_CRCcheck;                ///<bits 183:177 are not used, should be 0, bit 176 is 1 if CTP7 detected a CRC mismatch
    uint8_t  m_Header;                  /// Either 0x5E (normally it should be 0x1E, if it is 0x5E that indicates that the VFAT3 internal buffer is half-full, so it's like a warning)
    uint8_t  m_EC;                      ///<Event Counter, 8 bits
    uint16_t m_BC;                     ///<Bunch counter
    uint64_t m_lsData;                  ///<channels from 1to64 
    uint64_t m_msData;                  ///<channels from 65to128
    uint16_t m_crc;                     ///<Check Sum value, 16 bits
    uint16_t m_crc_calc;                ///<Check Sum value recalculated, 16 bits
    int      m_SlotNumber;              ///<Calculated chip position
    bool     m_isBlockGood;             ///<Shows if block is good (control bits, chip ID and CRC checks)
  
  };
}

#endif
