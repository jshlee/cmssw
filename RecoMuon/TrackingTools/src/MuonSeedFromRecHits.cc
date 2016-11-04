/**
 *  See header file for a description of this class.
 *
 *
 *  \author A. Vitelli - INFN Torino, V.Palichik
 *  \author porting  R. Bellan
 *
 */
#include "RecoMuon/TrackingTools/interface/MuonSeedFromRecHits.h"

#include "RecoMuon/TransientTrackingRecHit/interface/MuonTransientTrackingRecHit.h"

#include "RecoMuon/TrackingTools/interface/MuonPatternRecoDumper.h"

#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"
#include "TrackingTools/DetLayers/interface/DetLayer.h"

#include "DataFormats/TrajectoryState/interface/PTrajectoryStateOnDet.h"
#include "DataFormats/Common/interface/OwnVector.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "gsl/gsl_statistics.h"



using namespace std;

template <class T> T sqr(const T& t) {return t*t;}

MuonSeedFromRecHits::MuonSeedFromRecHits()
: theField(0)
{
}


TrajectorySeed MuonSeedFromRecHits::createSeed(float ptmean,
					       float sptmean,
					       ConstMuonRecHitPointer last) const
{
  
  const std::string metname = "Muon|RecoMuon|MuonSeedFromRecHits";

  MuonPatternRecoDumper debug;
      std::cout<<" LL1 " <<std::endl;
  // FIXME: put it into a parameter set!
  double theMinMomentum = 3.0;
  int charge=std::copysign(1,ptmean);
      std::cout<<" LL2 " <<std::endl;

  // Minimal pt
  if ( fabs(ptmean) < theMinMomentum ) ptmean = theMinMomentum * charge ;
      std::cout<<" LL3 " <<std::endl;

  AlgebraicVector t(4);
  AlgebraicSymMatrix mat(5,0) ;
      std::cout<<" LL4 " <<std::endl;

  // Fill the LocalTrajectoryParameters
  LocalPoint segPos=last->localPosition();
  std::cout<<" LL41 " <<last->isCSC()<<" "<<last->isGEM()<<" "<<std::endl;

  std::cout<<" LL413" << GlobalPoint() <<std::endl;
  std::cout<<" LL412" << last->globalPosition() <<std::endl;
  GlobalVector mom=last->globalPosition()-GlobalPoint();
      std::cout<<" LL42 " <<std::endl;


  GlobalVector polar(GlobalVector::Spherical(mom.theta(),
                                             last->globalDirection().phi(),
                                             1.));
      std::cout<<" LL5 " <<std::endl;

  polar *=fabs(ptmean)/polar.perp();
  LocalVector segDirFromPos=last->det()->toLocal(polar);
      std::cout<<" LL6 " <<std::endl;

  LocalTrajectoryParameters param(segPos,segDirFromPos, charge);
      std::cout<<" LL7 " <<std::endl;

  // this perform H.T() * parErr * H, which is the projection of the 
  // the measurement error (rechit rf) to the state error (TSOS rf)
  // Legenda:
  // H => is the 4x5 projection matrix
  // parError the 4x4 parameter error matrix of the RecHit

  // LogTrace(metname) << "Projection matrix:\n" << last->projectionMatrix();
  // LogTrace(metname) << "Error matrix:\n" << last->parametersError();

  mat = last->parametersError().similarityT( last->projectionMatrix() );
  

  float p_err = sqr(sptmean/(ptmean*ptmean));
  mat[0][0]= p_err;

        std::cout<<" LL8 " <<std::endl;


  LocalTrajectoryError error(asSMatrix<5>(mat));
  
  // Create the TrajectoryStateOnSurface
  TrajectoryStateOnSurface tsos(param, error, last->det()->surface(), theField);
      std::cout<<" LL9 " <<std::endl;

  // The following LogTraces must be moved somewhere else (StandAloneTrajectoryBuilder)
  // Here the TSOS does not have the magnetic field set, so dumpTSOS causes a crash
  // (when LogTrace/LogDebug is activated)
  //LogTrace(metname) << "Trajectory State on Surface before the extrapolation"<<endl;
  //LogTrace(metname) << debug.dumpTSOS(tsos);
  
  // Take the DetLayer on which relies the rechit
  DetId id = last->geographicalId();
  // Segment layer
  //LogTrace(metname) << "The RecSegment relies on: "<<endl;
  //LogTrace(metname) << debug.dumpMuonId(id);
  //LogTrace(metname) << debug.dumpTSOS(tsos);

  // Transform it in a TrajectoryStateOnSurface
  
        std::cout<<" LL10 " <<std::endl;

  PTrajectoryStateOnDet const & seedTSOS =
    trajectoryStateTransform::persistentState( tsos ,id.rawId());
       std::cout<<" LL11 " <<std::endl;
 
  edm::OwnVector<TrackingRecHit> container;
  for (unsigned l=0; l<theRhits.size(); l++) {
      container.push_back( theRhits[l]->hit()->clone() );
  }
      std::cout<<" LL12 " <<std::endl;

  TrajectorySeed theSeed(seedTSOS,container,alongMomentum);
      std::cout<<" LL13 " <<std::endl;

  return theSeed;
}




