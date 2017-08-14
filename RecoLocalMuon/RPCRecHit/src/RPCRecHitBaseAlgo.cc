/*
 *  See header file for a description of this class.
 *
 *  \author M. Maggi -- INFN Bari
 */

#include "RecoLocalMuon/RPCRecHit/interface/RPCRecHitBaseAlgo.h"
#include "RecoLocalMuon/RPCRecHit/src/RPCClusterContainer.h"
#include "RecoLocalMuon/RPCRecHit/src/RPCCluster.h"
#include "RecoLocalMuon/RPCRecHit/src/RPCClusterizer.h"
#include "RecoLocalMuon/RPCRecHit/src/RPCMaskReClusterizer.h"

RPCRecHitBaseAlgo::RPCRecHitBaseAlgo(const edm::ParameterSet& config):
  stationNotToUse_(config.getUntrackedParameter<int>("stationNotToUse",3))
{  //  theSync = RPCTTrigSyncFactory::get()->create(config.getParameter<string>("tTrigMode"),
  //config.getParameter<ParameterSet>("tTrigModeConfig"));
}

// Build all hits in the range associated to the layerId, at the 1st step.
edm::OwnVector<RPCRecHit> RPCRecHitBaseAlgo::reconstruct(const RPCRoll& roll,
                                                         const RPCDetId& rpcId,
                                                         const RPCDigiCollection::Range& digiRange,
                                                         const RollMask& mask) {
  edm::OwnVector<RPCRecHit> result;

  RPCClusterizer clizer;
  RPCClusterContainer tcls = clizer.doAction(digiRange);
  RPCMaskReClusterizer mrclizer;
  RPCClusterContainer cls = mrclizer.doAction(rpcId,tcls,mask);

  for ( auto cl : cls ) {
    LocalError tmpErr;
    LocalPoint point;
    float time = 0, timeErr = -1;

    // Call the compute method
    const bool OK = this->compute(roll, cl, point, tmpErr, time, timeErr);
    if (!OK) continue;

    // Build a new pair of 1D rechit
    const int firstClustStrip = cl.firstStrip();
    const int clusterSize = cl.clusterSize();
    RPCRecHit* recHit = new RPCRecHit(rpcId,cl.bx(),firstClustStrip,clusterSize,point,tmpErr);
    if ( timeErr >= 0 ) recHit->setTimeAndError(time, timeErr);
      
    int station = (int) rpcId.station();
    int ring = (int) rpcId.ring();

    switch(stationNotToUse_){

	case 0: if(!((station == 3 || station == 4) && ring == 1)) result.push_back(recHit); // NO RPC UPGRADE
		break;
	case 1: if(!(station == 4 && ring == 1)) result.push_back(recHit); // No RE4/1
		break;
	case 2: if(!(station == 3 && ring == 1)) result.push_back(recHit); // No RE3/1
		break;
	case 3: result.push_back(recHit); // RE3/1 + RE4/1
		break;
	default: result.push_back(recHit); // RE3/1 + RE4/1
		break;

    }
      
  }

  return result;
}
