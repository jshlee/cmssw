#include <RecoMuon/DetLayers/src/MuonGEMDetLayerGeometryBuilder.h>

#include <DataFormats/MuonDetId/interface/GEMDetId.h>
#include <Geometry/CommonDetUnit/interface/GeomDet.h>
#include <RecoMuon/DetLayers/interface/MuRingForwardDoubleLayer.h>
#include <RecoMuon/DetLayers/interface/MuRodBarrelLayer.h>
#include <RecoMuon/DetLayers/interface/MuDetRing.h>
#include <RecoMuon/DetLayers/interface/MuDetRod.h>

#include <Utilities/General/interface/precomputed_value_sort.h>
#include <Geometry/CommonDetUnit/interface/DetSorting.h>
#include "Utilities/BinningTools/interface/ClusterizingHistogram.h"

#include <FWCore/MessageLogger/interface/MessageLogger.h>

#include <iostream>

using namespace std;

MuonGEMDetLayerGeometryBuilder::~MuonGEMDetLayerGeometryBuilder() {}

// Builds the forward (first) and backward (second) layers
// Builds etaPartitions (for rechits)
pair<vector<DetLayer*>, vector<DetLayer*> > MuonGEMDetLayerGeometryBuilder::buildEndcapLayers(const GEMGeometry& geo)
{
  vector<DetLayer*> endcapLayers[2];

  //  auto sc = geo.superChambers()
  for (auto sc : geo.superChambers()) {
    cout << "MuonGEMDetLayerGeometryBuilder:" << sc->id() << endl;
    ForwardDetLayer* fowardLayer = nullptr;
    vector<const ForwardDetRing*> frontRings, backRings;

    for (int roll = GEMDetId::minRollId + 1; roll <= GEMDetId::maxRollId; ++roll) {
      vector<const GeomDet*> frontDets, backDets;
    
      for (auto ch : sc->chambers()) {

        auto etaP = ch->etaPartition(roll);
        
        if (etaP) {
          bool isInFront = isFront(etaP->id());
          if (isInFront) {
            frontDets.push_back(etaP);
          } else {
            backDets.push_back(etaP);
          }
          cout << "get GEM Endcap roll " << etaP->id() << (isInFront ? "front" : "back ")
               << " at R=" << etaP->position().perp() << ", phi=" << etaP->position().phi()
               << ", Z=" << etaP->position().z() << endl;          
        }
      }
      
      if (!frontDets.empty()) {
        precomputed_value_sort(frontDets.begin(), frontDets.end(), geomsort::DetPhi());
        frontRings.push_back(new MuDetRing(frontDets));
        LogTrace(metname) << "New front ring with " << frontDets.size()
                          << " chambers at z=" << frontRings.back()->position().z();
      }
      if (!backDets.empty()) {
        precomputed_value_sort(backDets.begin(), backDets.end(), geomsort::DetPhi());
        backRings.push_back(new MuDetRing(backDets));
        LogTrace(metname) << "New back ring with " << backDets.size()
                          << " chambers at z=" << backRings.back()->position().z();
      }
      
    }

    if (!backRings.empty() && !frontRings.empty() && sc->id().station() != GEMDetId::minStationId0) {
      fowardLayer = new MuRingForwardDoubleLayer(frontRings, backRings);
      cout << "st - " << sc->id().station() << endl;
    }
    else if (!frontRings.empty() && sc->id().station() == GEMDetId::minStationId0) {
      fowardLayer = new MuRingForwardLayer(frontRings);
      cout << "st 0 " << endl;
    }
    
    if (fowardLayer != nullptr) {
      LogTrace(metname) << "New MuRingForwardLayer with " << frontRings.size() << " and " << backRings.size()
                        << " rings, at Z " << fowardLayer->position().z() << " R1: " << fowardLayer->specificSurface().innerRadius()
                        << " R2: " << fowardLayer->specificSurface().outerRadius();

      endcapLayers[sc->id().region()].push_back(fowardLayer);      
    }    
  }

  pair<vector<DetLayer*>, vector<DetLayer*> > res_pair(endcapLayers[0], endcapLayers[1]);
  return res_pair;
}

MuRingForwardDoubleLayer* MuonGEMDetLayerGeometryBuilder::buildLayer(int endcap,
                                                                     vector<int>& rings,
                                                                     int station,
                                                                     int layer,
                                                                     vector<int>& chambers,
                                                                     vector<int>& rolls,
                                                                     const GEMGeometry& geo) {
  const std::string metname = "Muon|RecoMuon|RecoMuonDetLayers|MuonGEMDetLayerGeometryBuilder";
  MuRingForwardDoubleLayer* result = nullptr;
  vector<const ForwardDetRing*> frontRings, backRings;

  for (std::vector<int>::iterator ring = rings.begin(); ring != rings.end() - 2; ring++) {
    for (vector<int>::iterator roll = rolls.begin(); roll != rolls.end(); roll++) {
      vector<const GeomDet*> frontDets, backDets;

      for (std::vector<int>::iterator chamber = chambers.begin(); chamber < chambers.end(); chamber++) {
        GEMDetId gemId(endcap, (*ring), station, layer, (*chamber), (*roll));

        const GeomDet* geomDet = geo.idToDet(gemId);

        if (geomDet != nullptr) {
          bool isInFront = isFront(gemId);
          if (isInFront) {
            frontDets.push_back(geomDet);
          } else {
            backDets.push_back(geomDet);
          }
          cout << "get GEM Endcap roll " << gemId << (isInFront ? "front" : "back ")
                            << " at R=" << geomDet->position().perp() << ", phi=" << geomDet->position().phi()
               << ", Z=" << geomDet->position().z() << endl;
        }
      }

      if (!frontDets.empty()) {
        precomputed_value_sort(frontDets.begin(), frontDets.end(), geomsort::DetPhi());
        frontRings.push_back(new MuDetRing(frontDets));
        LogTrace(metname) << "New front ring with " << frontDets.size()
                          << " chambers at z=" << frontRings.back()->position().z();
      }
      if (!backDets.empty()) {
        precomputed_value_sort(backDets.begin(), backDets.end(), geomsort::DetPhi());
        backRings.push_back(new MuDetRing(backDets));
        LogTrace(metname) << "New back ring with " << backDets.size()
                          << " chambers at z=" << backRings.back()->position().z();
      }
    }
  }

  // How should they be sorted?
  //    precomputed_value_sort(muDetRods.begin(), muDetRods.end(), geomsort::ExtractZ<GeometricSearchDet,float>());
  if (!backRings.empty() && !frontRings.empty())
    result = new MuRingForwardDoubleLayer(frontRings, backRings);
  else
    result = nullptr;
  if (result != nullptr) {
    LogTrace(metname) << "New MuRingForwardLayer with " << frontRings.size() << " and " << backRings.size()
                      << " rings, at Z " << result->position().z() << " R1: " << result->specificSurface().innerRadius()
                      << " R2: " << result->specificSurface().outerRadius();
  }
  return result;
}

bool MuonGEMDetLayerGeometryBuilder::isFront(const GEMDetId& gemId) {
  // ME0s do not currently have an arrangement of which are front and which are back, going to always return true  
  if (gemId.station() == GEMDetId::minStationId0)
    return true;

  if (gemId.chamber() % 2 == 0)
    return true;

  return false;

}

MuDetRing* MuonGEMDetLayerGeometryBuilder::makeDetRing(vector<const GeomDet*>& geomDets) {
  const std::string metname = "Muon|RecoMuon|RecoMuonDetLayers|MuonGEMDetLayerGeometryBuilder";

  precomputed_value_sort(geomDets.begin(), geomDets.end(), geomsort::DetPhi());
  MuDetRing* result = new MuDetRing(geomDets);
  LogTrace(metname) << "New MuDetRing with " << geomDets.size() << " chambers at z=" << result->position().z()
                    << " R1: " << result->specificSurface().innerRadius()
                    << " R2: " << result->specificSurface().outerRadius();
  return result;
}
