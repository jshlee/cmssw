#ifndef MuonReco_MuonGEMSegmentMatch_h
#define MuonReco_MuonGEMSegmentMatch_h

#include "DataFormats/GEMRecHit/interface/GEMSegmentCollection.h"

#include <cmath>

namespace reco {
   class MuonGEMSegmentMatch {
      public:
         float x;              // X position of the matched segment
         float y;
         float phi;
         float x_over_sigma;
         float y_over_sigma;       
         int station;

	 MuonGEMSegmentMatch():x(0),y(0),phi(0),x_over_sigma(0),y_over_sigma(0),station(0){}
   };
}

#endif
