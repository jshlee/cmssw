#include "DataFormats/GEMDigi/interface/GEMGEBStatusDigi.h"

GEMGEBStatusDigi::GEMGEBStatusDigi(uint32_t ZeroSup,
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
				   std::vector<uint8_t> v_GEBflags) :
  ZeroSup_(ZeroSup),
<<<<<<< HEAD
  InputID_(InputID),
=======
>>>>>>> cms-sw/refs/pull/21305/head
  Vwh_(Vwh),
  ErrorC_(ErrorC),
  OHCRC_(OHCRC),
  Vwt_(Vwt),
<<<<<<< HEAD
=======
  InputID_(InputID),
>>>>>>> cms-sw/refs/pull/21305/head
  InFu_(InFu),
  Stuckd_(Stuckd),
  v_GEBflags_(v_GEBflags)
{};
