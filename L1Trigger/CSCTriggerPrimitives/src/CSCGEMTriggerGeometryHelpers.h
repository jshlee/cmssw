#ifndef CSCTriggerPrimitives_CSCTriggerPrimitivesBuilder_h
#define CSCTriggerPrimitives_CSCTriggerPrimitivesBuilder_h

#include <DataFormats/MuonDetId/interface/CSCTriggerNumbering.h>
#include <Geometry/GEMGeometry/interface/GEMGeometry.h>
#include <Geometry/GEMGeometry/interface/GEMEtaPartitionSpecs.h>
#include <L1Trigger/CSCCommonTrigger/interface/CSCTriggerGeometry.h>
#include <DataFormats/Math/interface/deltaPhi.h>
#include <DataFormats/Math/interface/normalizedPhi.h>

class CSCGEMTriggerGeometryHelpers
{
 public:
  CSCGEMTriggerGeometryHelpers();
  ~CSCGEMTriggerGeometryHelpers();

  int cscHalfStripToGEMStrip(int cscHalfStrip, bool isEven);
  int cscHalfStripToGEMPad(int cscHalfStrip, bool isEven);
  int cscStripToGEMStrip(int cscStrip, bool isEven);
  int cscStripToGEMPad(int cscStrip, bool isEven);

  int gemStripToCSCHalfStrip(int gemStrip, bool isEven);
  int gemStripToCSCStrip(int gemStrip, bool isEven);
  int gemPadToCSCHalfStrip(int gemPad, bool isEven);
  int gemPadToCSCStrip(int gemPad, bool isEven);

 private:
};

#endif

/*
  // loop on all wiregroups to create a LUT <WG,rollMin,rollMax>
  int numberOfWG(cscChamber->layer(1)->geometry()->numberOfWireGroups());
  std::cout <<"detId " << cscChamber->id() << std::endl;
  for (int i = 0; i< numberOfWG; ++i){
    // find low-eta of WG
    auto length(cscChamber->layer(1)->geometry()->lengthOfWireGroup(i));
//     auto gp(cscChamber->layer(1)->centerOfWireGroup(i));
    auto lpc(cscChamber->layer(1)->geometry()->localCenterOfWireGroup(i));
    auto wireEnds(cscChamber->layer(1)->geometry()->wireTopology()->wireEnds(i));
    auto gpMin(cscChamber->layer(1)->toGlobal(wireEnds.first));
    auto gpMax(cscChamber->layer(1)->toGlobal(wireEnds.second));
    auto etaMin(gpMin.eta());
    auto etaMax(gpMax.eta());
    if (etaMax < etaMin)
      std::swap(etaMin,etaMax);
    //print the eta min and eta max
    //    std::cout << i << " " << etaMin << " " << etaMax << std::endl;
    auto x1(lpc.x() + cos(cscChamber->layer(1)->geometry()->wireAngle())*length/2.);
    auto x2(lpc.x() - cos(cscChamber->layer(1)->geometry()->wireAngle())*length/2.);
    auto z(lpc.z());
    auto y1(cscChamber->layer(1)->geometry()->yOfWireGroup(i,x1));
    auto y2(cscChamber->layer(1)->geometry()->yOfWireGroup(i,x2));
    auto lp1(LocalPoint(x1,y1,z));
    auto lp2(LocalPoint(x2,y2,z));
    auto gp1(cscChamber->layer(1)->toGlobal(lp1));
    auto gp2(cscChamber->layer(1)->toGlobal(lp2));
    auto eta1(gp1.eta());
    auto eta2(gp2.eta());
    if (eta1 < eta2)
      std::swap(eta1,eta2);
    std::cout << "{" << i << ", " << eta1 << ", " << eta2 << "},"<< std::endl;
    
    
//     Std ::cout << "WG "<< i << std::endl;
//    wireGroupGEMRollMap_[i] = assignGEMRoll(gp.eta());
  }

//   // print-out
//   for(auto it = wireGroupGEMRollMap_.begin(); it != wireGroupGEMRollMap_.end(); it++) {
//     std::cout << "WG "<< it->first << " GEM pad " << it->second << std::endl;
//   }
*/
