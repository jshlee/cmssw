/** Implementation of the GEM Geometry Builder from GEM record stored in CondDB
 *
 *  \author M. Maggi - INFN Bari
 */
#include "Geometry/GEMGeometryBuilder/src/GEMGeometryBuilderFromCondDB.h"
#include "Geometry/GEMGeometry/interface/GEMEtaPartitionSpecs.h"

#include <DetectorDescription/Core/interface/DDFilter.h>
#include <DetectorDescription/Core/interface/DDFilteredView.h>
#include <DetectorDescription/Core/interface/DDSolid.h>

#include "Geometry/MuonNumbering/interface/MuonDDDNumbering.h"
#include "Geometry/MuonNumbering/interface/MuonBaseNumber.h"
#include "Geometry/MuonNumbering/interface/GEMNumberingScheme.h"

#include "DataFormats/GeometrySurface/interface/RectangularPlaneBounds.h"
#include "DataFormats/GeometrySurface/interface/TrapezoidalPlaneBounds.h"

#include "DataFormats/GeometryVector/interface/Basic3DVector.h"

#include "CLHEP/Units/GlobalSystemOfUnits.h"

#include <FWCore/MessageLogger/interface/MessageLogger.h>

#include <iostream>
#include <algorithm>

GEMGeometryBuilderFromCondDB::GEMGeometryBuilderFromCondDB() 
{ }

GEMGeometryBuilderFromCondDB::~GEMGeometryBuilderFromCondDB() 
{ }

void
GEMGeometryBuilderFromCondDB::build(const std::shared_ptr<GEMGeometry>& theGeometry,
				    const RecoIdealGeometry& rgeo )
{
  const std::vector<DetId>& detids( rgeo.detIds());
  std::vector<GEMSuperChamber*> superChambers;

  for( unsigned int id = 0; id < detids.size(); ++id )
    {  
      GEMDetId gemid( detids[id] );
      std::cout <<"GEMGeometryBuilderFromCondDB  " << gemid<< std::endl;
      if (gemid.roll() == 0){
	if (gemid.layer() == 0){
	  GEMSuperChamber* gsc = buildSuperChamber( rgeo, id, gemid );
	  theGeometry->add(gsc);
	}
	else {
	  GEMChamber* gch = buildChamber( rgeo, id, gemid );
	  theGeometry->add(gch);
	}
      }
      else {      
	GEMEtaPartition* gep = buildEtaPartition( rgeo, id, gemid );
	theGeometry->add(gep);
      }
    }
  
  // construct the regions, stations and rings. 
  for (int re = -1; re <= 1; re = re+2) {    
    GEMRegion* region = new GEMRegion( re );
    for( int st = 1; st <= GEMDetId::maxStationId; ++st ) {
      GEMStation* station = new GEMStation(re, st);
      std::string sign( re==-1 ? "-" : "");
      std::string name("GE" + sign + std::to_string(st) + "/1");
      station->setName(name);
      // only 1 ring
      GEMRing* ring = new GEMRing( re, st, 1 );

      for (int ch = GEMDetId::minChamberId; ch<=GEMDetId::maxChamberId; ++ch) {

	const GEMSuperChamber* superChamber = theGeometry->superChamber( GEMDetId( re, 1, st, 0, ch, 0 ));
	if (superChamber){

	  for (int ly = 1; ly<=2; ++ly) {
	    const GEMChamber* chamber = theGeometry->chamber( GEMDetId( re, 1, st, ly, ch, 0 ));
	    
	    for (int roll = 1; roll<=GEMDetId::maxRollId; ++roll) {
	      
	      const GEMEtaPartition* etaPartition = theGeometry->etaPartition( GEMDetId( re, 1, st, ly, ch, roll ));
	      if (etaPartition)
		chamber->add(etaPartition);
	    }
	    superChamber->add(chamber);
	  }
	  ring->add( superChamber );
	}
      }
      LogDebug("GEMGeometryBuilderFromCondDB") << "Adding ring " <<  ri << " to station " << "re " << re << " st " << st << std::endl;
      station->add( ring );
      theGeometry->add( ring );
      
      LogDebug("GEMGeometryBuilderFromCondDB") << "Adding station " << st << " to region " << re << std::endl;
      region->add( station );
      theGeometry->add( station );
    }
    LogDebug("GEMGeometryBuilderFromCondDB") << "Adding region " << re << " to the geometry " << std::endl;
    theGeometry->add( region );
  }
}

GEMSuperChamber* GEMGeometryBuilderFromCondDB::buildSuperChamber(const RecoIdealGeometry& rgeo, unsigned int gid, GEMDetId detId) const
{
  LogDebug("GEMGeometryBuilderFromCondDB") << "buildSuperChamber "<< detId <<std::endl;
  
  bool isOdd = detId.superChamber()%2;
  RCPBoundPlane surf(boundPlane(rgeo, gid, isOdd ));
  
  GEMSuperChamber* superChamber = new GEMSuperChamber(detId, surf);
  return superChamber;
  
}

GEMChamber* GEMGeometryBuilderFromCondDB::buildChamber(const RecoIdealGeometry& rgeo, unsigned int gid, GEMDetId detId) const
{
  LogDebug("GEMGeometryBuilderFromCondDB") << "buildChamber "<< detId <<std::endl;
  
  bool isOdd = detId.chamber()%2;
  RCPBoundPlane surf(boundPlane(rgeo, gid, isOdd ));
  
  GEMChamber* chamber = new GEMChamber(detId, surf);
  return chamber;
}

GEMEtaPartition* GEMGeometryBuilderFromCondDB::buildEtaPartition(const RecoIdealGeometry& rgeo, unsigned int gid, GEMDetId detId) const
{
  std::vector<double>::const_iterator strStart = rgeo.strStart( gid );
  std::string name = *( strStart );
  LogDebug("GEMGeometryBuilderFromCondDB") << "buildEtaPartition "<< name<<" "<< detId <<std::endl;
  
  std::vector<double>::const_iterator shapeStart = rgeo.shapeStart( gid );
  float be = *(shapeStart+0)/cm;
  float te = *(shapeStart+1)/cm;
  float ap = *(shapeStart+2)/cm;
  float ti = *(shapeStart+3)/cm;
  float nstrip = *(shapeStart+4);
  float npad = *(shapeStart+5);

  std::vector<float> pars;
  pars.emplace_back(be); 
  pars.emplace_back(te); 
  pars.emplace_back(ap); 
  pars.emplace_back(nstrip);
  pars.emplace_back(npad);
  
  bool isOdd = detId.chamber()%2;
  RCPBoundPlane surf(boundPlane(rgeo, gid, isOdd ));
  GEMEtaPartitionSpecs* e_p_specs = new GEMEtaPartitionSpecs(GeomDetEnumerators::GEM, name, pars);
  
  LogDebug("GEMGeometryBuilderFromCondDB") << "size "<< be << " " << te << " " << ap << " " << ti <<std::endl;
  GEMEtaPartition* etaPartition = new GEMEtaPartition(detId, surf, e_p_specs);
  return etaPartition;
}

GEMGeometryBuilderFromCondDB::RCPBoundPlane 
GEMGeometryBuilderFromCondDB::boundPlane(const RecoIdealGeometry& rgeo, unsigned int gid, bool isOddChamber) const
{
  std::vector<double>::const_iterator shapeStart = rgeo.shapeStart( gid );
  float be = *(shapeStart+0)/cm;
  float te = *(shapeStart+1)/cm;
  float ap = *(shapeStart+2)/cm;
  float ti = *(shapeStart+3)/cm;
  //  TrapezoidalPlaneBounds* 
  Bounds* bounds = new TrapezoidalPlaneBounds( be, te, ap, ti );

  std::vector<double>::const_iterator tranStart = rgeo.tranStart( gid );
  Surface::PositionType posResult(*(tranStart)/cm, *(tranStart+1)/cm, *(tranStart+2)/cm );

  std::vector<double>::const_iterator rotStart = rgeo.rotStart( gid );
  Surface::RotationType rotResult(*(rotStart+0), *(rotStart+1), *(rotStart+2),
				  *(rotStart+3), *(rotStart+4), *(rotStart+5),
				  *(rotStart+6), *(rotStart+7), *(rotStart+8));
  
  //Change of axes for the forward
  Basic3DVector<float> newX(1.,0.,0.);
  Basic3DVector<float> newY(0.,0.,1.);
  Basic3DVector<float> newZ(0.,1.,0.);
  // Odd chambers are inverted in gem.xml
  if (isOddChamber) newY *= -1;
  
  rotResult.rotateAxes(newX, newY, newZ);

  return RCPBoundPlane( new BoundPlane( posResult, rotResult, bounds));
}
