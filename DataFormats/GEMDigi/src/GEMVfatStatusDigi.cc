#include "DataFormats/GEMDigi/interface/GEMVfatStatusDigi.h"
#include <iostream>

GEMVfatStatusDigi::GEMVfatStatusDigi(gem::VFATdata &vfat)
{
  quality_  = vfat.quality();
  flag_     = vfat.flag();
  position_ = vfat.phiPos();
  ec_       = vfat.ec();
  bc_       = vfat.bc();
};
