/** 
 * \file NEWGEMCSCSegmentBuilder.cc
 *
 */

#include <RecoLocalMuon/GEMCSCSegment/plugins/NEWGEMCSCSegmentBuilder.h>
#include <RecoLocalMuon/GEMCSCSegment/plugins/GEMCSCSegmentAlgorithm.h>
#include <RecoLocalMuon/GEMCSCSegment/plugins/GEMCSCSegmentBuilderPluginFactory.h>


#include "FWCore/Framework/interface/ESHandle.h"
#include <FWCore/Utilities/interface/Exception.h>
#include <FWCore/MessageLogger/interface/MessageLogger.h> 

#include <Geometry/Records/interface/MuonGeometryRecord.h>
#include <Geometry/CSCGeometry/interface/CSCChamberSpecs.h>
#include <Geometry/CSCGeometry/interface/CSCLayer.h>
#include <Geometry/CSCGeometry/interface/CSCGeometry.h>
#include <Geometry/GEMGeometry/interface/GEMGeometry.h>
#include <Geometry/GEMGeometry/interface/GEMEtaPartition.h>

#include <DataFormats/MuonDetId/interface/CSCDetId.h>
#include <DataFormats/MuonDetId/interface/GEMDetId.h>
#include <DataFormats/CSCRecHit/interface/CSCRecHit2D.h>
#include <DataFormats/CSCRecHit/interface/CSCRangeMapAccessor.h>
#include <DataFormats/CSCRecHit/interface/CSCRecHit2DCollection.h>
#include <DataFormats/CSCRecHit/interface/CSCSegmentCollection.h>
#include <DataFormats/CSCRecHit/interface/CSCSegment.h>
#include <DataFormats/GEMRecHit/interface/GEMRecHit.h>
#include <DataFormats/GEMRecHit/interface/GEMRecHitCollection.h>


NEWGEMCSCSegmentBuilder::NEWGEMCSCSegmentBuilder(const edm::ParameterSet& ps) : gemgeom_(nullptr), cscgeom_(nullptr) 
{

  // Algo name
  std::string algoName = ps.getParameter<std::string>("algo_name");
  edm::LogVerbatim("NEWGEMCSCSegmentBuilder")<< "[NEWGEMCSCSegmentBuilder :: ctor] algorithm name: " << algoName;

  // SegAlgo parameter set 	  
  edm::ParameterSet segAlgoPSet = ps.getParameter<edm::ParameterSet>("algo_psets");
 
  // Ask factory to build this algorithm, giving it appropriate ParameterSet
  algo = NEWGEMCSCSegmentBuilderPluginFactory::get()->create(algoName, segAlgoPSet);

}

NEWGEMCSCSegmentBuilder::~NEWGEMCSCSegmentBuilder() 
{
  delete algo;
  edm::LogVerbatim("NEWGEMCSCSegmentBuilder")<<"[NEWGEMCSCSegmentBuilder :: dstor] deleted the algorithm";
}

void NEWGEMCSCSegmentBuilder::LinkGEMRollsToCSCChamberIndex(const GEMGeometry* gemGeo, const CSCGeometry* cscGeo) 
{

  for (TrackingGeometry::DetContainer::const_iterator it=gemGeo->dets().begin();it<gemGeo->dets().end();it++)
    {
      const GEMChamber* ch = dynamic_cast< const GEMChamber* >( *it );
      if(ch != nullptr )
	{
	  std::vector< const GEMEtaPartition*> rolls = (ch->etaPartitions());
	  for(std::vector<const GEMEtaPartition*>::const_iterator r = rolls.begin(); r != rolls.end(); ++r)
	    {
	      GEMDetId gemId = (*r)->id();
	      int region=gemId.region();
	      if(region!=0)
		{
		  int station    = gemId.station();
		  int ring       = gemId.ring();
		  int gemchamber = gemId.chamber();
		  int layer      = gemId.layer();
		  int cscring    = ring;
		  int cscstation = station;
		  int cscchamber = gemchamber;
		  int csclayer   = layer;
		  CSCStationIndex ind(region,cscstation,cscring,cscchamber,csclayer);
		  std::set<GEMDetId> myrolls;
		  if (rollstoreCSC.find(ind)!=rollstoreCSC.end()) myrolls=rollstoreCSC[ind];
		  myrolls.insert(gemId);
		  rollstoreCSC[ind]=myrolls;
		}
	    }
	}
    }

  // edm::LogVerbatim to print details of std::map< CSCIndex, std::set<GEMRolls> >
  for(std::map<CSCStationIndex,std::set<GEMDetId> >::iterator mapit = rollstoreCSC.begin();
      mapit != rollstoreCSC.end(); ++mapit) 
    {
      CSCStationIndex    map_first = mapit->first;
      std::set<GEMDetId> map_secnd = mapit->second;
      std::stringstream GEMRollsstream;
      for(std::set<GEMDetId>::iterator setit=map_secnd.begin(); setit!=map_secnd.end(); ++setit) 
	{ 
	  GEMRollsstream<<"[ GEM Id: "<<setit->rawId()<<" ("<<*setit<<")"<<"],"<<std::endl; 
	}
      std::string GEMRollsstr = GEMRollsstream.str();
      edm::LogVerbatim("NEWGEMCSCSegmentBuilder")<<"[NEWGEMCSCSegmentBuilder :: LinkGEMRollsToCSCChamberIndex] CSC Station Index :: ["
						 <<map_first.region()<<","<<map_first.station()<<","<<map_first.ring()<<","<<map_first.chamber()<<","<<map_first.layer()
						 <<"] has following GEM rolls: ["<<GEMRollsstr<<"]"<<std::endl;
    }
}

void NEWGEMCSCSegmentBuilder::build(const GEMRecHitCollection* recHits, const CSCRecHit2DCollection* cscrechits, GEMCSCSegmentCollection& oc) 
{  	
  edm::LogVerbatim("NEWGEMCSCSegmentBuilder")<< "[NEWGEMCSCSegmentBuilder :: build] Total number of GEM rechits in this event: " << recHits->size() << " Total number of CSC segments in this event: " << cscrechits->size();
  
  
  edm::LogVerbatim("NEWGEMCSCSegmentBuilder")<<"[NEWGEMCSCSegmentBuilder :: build] job done !!!";
}


void NEWGEMCSCSegmentBuilder::setGeometry(const GEMGeometry* gemgeom, const CSCGeometry* cscgeom) 
{
  gemgeom_ = gemgeom;
  cscgeom_ = cscgeom;
}


