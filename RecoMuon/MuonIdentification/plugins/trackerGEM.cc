#include <RecoMuon/MuonIdentification/plugins/trackerGEM.h>

#include <FWCore/PluginManager/interface/ModuleDef.h>
#include <FWCore/Framework/interface/MakerMacros.h>

#include <DataFormats/Common/interface/Handle.h>
#include <FWCore/MessageLogger/interface/MessageLogger.h> 

#include <DataFormats/MuonReco/interface/Muon.h>

#include "DataFormats/GeometryVector/interface/GlobalPoint.h"

#include "TrackingTools/Records/interface/TrackingComponentsRecord.h"

#include "DataFormats/GeometrySurface/interface/Plane.h"
#include "TrackPropagation/SteppingHelixPropagator/interface/SteppingHelixPropagator.h"
#include "TrackPropagation/SteppingHelixPropagator/interface/SteppingHelixStateInfo.h"
#include "DataFormats/Math/interface/deltaR.h"


#include "DataFormats/GeometrySurface/interface/LocalError.h"


#include "TLorentzVector.h"

#include "DataFormats/TrajectoryState/interface/LocalTrajectoryParameters.h"
#include "TrackingTools/AnalyticalJacobians/interface/JacobianCartesianToLocal.h"
#include "TrackingTools/AnalyticalJacobians/interface/JacobianLocalToCartesian.h"


#include "Geometry/GEMGeometry/interface/GEMGeometry.h"
#include <Geometry/Records/interface/MuonGeometryRecord.h>
#include "Geometry/GEMGeometry/interface/GEMEtaPartitionSpecs.h"
#include "Geometry/CommonTopologies/interface/StripTopology.h"
#include <DataFormats/GeometrySurface/interface/SimpleDiskBounds.h>

trackerGEM::trackerGEM(const edm::ParameterSet& iConfig) {
  gemSegmentsToken_ = consumes<GEMSegmentCollection >(iConfig.getParameter<edm::InputTag>("gemSegmentsToken"));
  generalTracksToken_ = consumes<reco::TrackCollection >(iConfig.getParameter<edm::InputTag>("generalTracksToken"));

  maxPullX_   = iConfig.getParameter<double>("maxPullX");
  maxDiffX_   = iConfig.getParameter<double>("maxDiffX");
  maxPullY_   = iConfig.getParameter<double>("maxPullY");
  maxDiffY_   = iConfig.getParameter<double>("maxDiffY");
  maxDiffPhiDirection_ = iConfig.getParameter<double>("maxDiffPhiDirection");

  produces<std::vector<reco::Muon> >();
}

trackerGEM::~trackerGEM() {}

void trackerGEM::produce(edm::Event& ev, const edm::EventSetup& setup) {
  using namespace edm;
  using namespace reco;
  using namespace std;
  
  ESHandle<MagneticField> bField;
  setup.get<IdealMagneticFieldRecord>().get(bField);
  const SteppingHelixPropagator* ThisshProp;
  ThisshProp = new SteppingHelixPropagator(&*bField,alongMomentum);


  Handle<GEMSegmentCollection> gemSegments;
  ev.getByToken(gemSegmentsToken_,gemSegments);

  Handle <TrackCollection > generalTracks;
  ev.getByToken(generalTracksToken_,generalTracks);

  std::auto_ptr<std::vector<Muon> > muons( new std::vector<Muon> ); 

  int TrackNumber = 0;
  for (std::vector<Track>::const_iterator thisTrack = generalTracks->begin();
       thisTrack != generalTracks->end(); ++thisTrack,++TrackNumber){
    //Initializing gem plane

    //Remove later
    if (std::abs(thisTrack->eta()) < 1.5) continue;

    float zSign = thisTrack->pz() > 0 ? 1.0f : -1.0f;

    int SegmentNumber = 0;
    reco::Muon MuonCandidate;
    double ClosestDelR2 = 999.;
    for (auto thisSegment = gemSegments->begin(); thisSegment != gemSegments->end(); 
	 ++thisSegment,++SegmentNumber){
      //GEMDetId id = thisSegment->gemDetId();
      // should be segment det ID, but not working currently
      GEMDetId id = thisSegment->specificRecHits()[0].gemId();

      //cout << "thisSegment->nRecHits() "<< thisSegment->nRecHits()<< endl;
      //cout << "thisSegment->specificRecHits().size() "<< thisSegment->specificRecHits().size()<< endl;
      //cout << "id.station() "<< id.station()<< endl;
      auto roll = gemGeom->etaPartition(id); 

      //      if ( zSign * roll->toGlobal(thisSegment->localPosition()).z() < 0 ) continue;
      if ( zSign * id.region() < 0 ) continue;
      // add in deltaR cut
      LocalPoint thisPosition(thisSegment->localPosition());
      GlobalPoint SegPos(roll->toGlobal(thisPosition));
      
      const float zValue = SegPos.z();

      Plane *plane = new Plane(Surface::PositionType(0,0,zValue),Surface::RotationType());

      //Getting the initial variables for propagation

      int chargeReco = thisTrack->charge(); 
      GlobalVector p3reco, r3reco;

      p3reco = GlobalVector(thisTrack->outerPx(), thisTrack->outerPy(), thisTrack->outerPz());
      r3reco = GlobalVector(thisTrack->outerX(), thisTrack->outerY(), thisTrack->outerZ());

      AlgebraicSymMatrix66 covReco;
      //This is to fill the cov matrix correctly
      AlgebraicSymMatrix55 covReco_curv;
      covReco_curv = thisTrack->outerStateCovariance();
      FreeTrajectoryState initrecostate = getFTS(p3reco, r3reco, chargeReco, covReco_curv, &*bField);
      getFromFTS(initrecostate, p3reco, r3reco, chargeReco, covReco);

      //Now we propagate and get the propagated variables from the propagated state
      SteppingHelixStateInfo startrecostate(initrecostate);
      SteppingHelixStateInfo lastrecostate;

      //const SteppingHelixPropagator* ThisshProp = 
      //dynamic_cast<const SteppingHelixPropagator*>(&*shProp);

      
	
      //lastrecostate = ThisshProp->propagate(startrecostate, *plane);
      //lastrecostate = ThisshProp->propagateWithPath(startrecostate, *plane);
      ThisshProp->propagate(startrecostate, *plane,lastrecostate);
	
      FreeTrajectoryState finalrecostate;
      lastrecostate.getFreeState(finalrecostate);

      AlgebraicSymMatrix66 covFinalReco;
      GlobalVector p3FinalReco_glob, r3FinalReco_globv;
      getFromFTS(finalrecostate, p3FinalReco_glob, r3FinalReco_globv, chargeReco, covFinalReco);


      //To transform the global propagated track to local coordinates


      GlobalPoint r3FinalReco_glob(r3FinalReco_globv.x(),r3FinalReco_globv.y(),r3FinalReco_globv.z());

      LocalPoint r3FinalReco = roll->toLocal(r3FinalReco_glob);
      LocalVector p3FinalReco=roll->toLocal(p3FinalReco_glob);

      //LocalPoint thisPosition(thisSegment->localPosition());
      //LocalVector thisDirection(thisSegment->localDirection().x(),thisSegment->localDirection().y(),thisSegment->localDirection().z());  //FIXME

      //The same goes for the error
      AlgebraicMatrix thisCov(4,4,0);   
      for (int i = 1; i <=4; i++){
	for (int j = 1; j <=4; j++){
	  thisCov(i,j) = thisSegment->parametersError()(i,j);
	}
      }

      /////////////////////////////////////////////////////////////////////////////////////////


      LocalTrajectoryParameters ltp(r3FinalReco,p3FinalReco,chargeReco);
      JacobianCartesianToLocal jctl(roll->surface(),ltp);
      AlgebraicMatrix56 jacobGlbToLoc = jctl.jacobian(); 

      AlgebraicMatrix55 Ctmp =  (jacobGlbToLoc * covFinalReco) * ROOT::Math::Transpose(jacobGlbToLoc); 
      AlgebraicSymMatrix55 C;  // I couldn't find any other way, so I resort to the brute force
      for(int i=0; i<5; ++i) {
	for(int j=0; j<5; ++j) {
	  C[i][j] = Ctmp[i][j]; 

	}
      }  

      Double_t sigmax = sqrt(C[3][3]+thisSegment->localPositionError().xx() );      
      Double_t sigmay = sqrt(C[4][4]+thisSegment->localPositionError().yy() );

      bool X_MatchFound = false, Y_MatchFound = false, Dir_MatchFound = false;
	

      // if ( (std::abs(thisPosition.x()-r3FinalReco.x()) < (3.0 * sigmax)) || (std::abs(thisPosition.x()-r3FinalReco.x()) < 2.0 ) ) X_MatchFound = true;
      // if ( (std::abs(thisPosition.y()-r3FinalReco.y()) < (3.0 * sigmay)) || (std::abs(thisPosition.y()-r3FinalReco.y()) < 2.0 ) ) Y_MatchFound = true;

      // if ( std::abs(p3FinalReco_glob.phi()-roll->toGlobal(thisSegment->localDirection()).phi()) < 0.15) Dir_MatchFound = true;


      if ( (std::abs(thisPosition.x()-r3FinalReco.x()) < (maxPullX_ * sigmax)) || (std::abs(thisPosition.x()-r3FinalReco.x()) < maxDiffX_ ) ) X_MatchFound = true;
      if ( (std::abs(thisPosition.y()-r3FinalReco.y()) < (maxPullY_ * sigmay)) || (std::abs(thisPosition.y()-r3FinalReco.y()) < maxDiffY_ ) ) Y_MatchFound = true;

      if (reco::deltaPhi(p3FinalReco_glob.phi(),roll->toGlobal(thisSegment->localDirection()).phi()) < maxDiffPhiDirection_) Dir_MatchFound = true;

      //Check for a Match, and if there is a match, check the delR from the segment, keeping only the closest in MuonCandidate
      if (X_MatchFound && Y_MatchFound && Dir_MatchFound) {
	   
	TrackRef thisTrackRef(generalTracks,TrackNumber);
	   
	//GlobalPoint SegPos(roll->toGlobal(thisSegment->localPosition()));
	GlobalPoint TkPos(r3FinalReco_globv.x(),r3FinalReco_globv.y(),r3FinalReco_globv.z());
	   
	double thisDelR2 = reco::deltaR2(SegPos,TkPos);
	if (thisDelR2 < ClosestDelR2){
	  ClosestDelR2 = thisDelR2;
	  // temp settting the muon to track p4
	  Particle::Charge q = thisTrackRef->charge();
	  Particle::LorentzVector p4(thisTrackRef->px(), thisTrackRef->py(), thisTrackRef->pz(), thisTrackRef->p());
	  Particle::Point vtx(thisTrackRef->vx(),thisTrackRef->vy(), thisTrackRef->vz());

	  MuonCandidate = reco::Muon(q, p4, vtx);

	  MuonCandidate.setTrack(thisTrackRef);
	  // need to make track from gem seg
	  MuonCandidate.setOuterTrack(thisTrackRef);
	  MuonCandidate.setType(thisSegment->nRecHits());
	      
	  //MuonCandidate.setGlobalTrackPosAtSurface(r3FinalReco_glob);
	  //MuonCandidate.setGlobalTrackMomAtSurface(p3FinalReco_glob);
	  //MuonCandidate.setLocalTrackPosAtSurface(r3FinalReco);
	  //MuonCandidate.setLocalTrackMomAtSurface(p3FinalReco);
	  //MuonCandidate.setGlobalTrackCov(covFinalReco);
	  //MuonCandidate.setLocalTrackCov(C);
	}
      }
    }//End loop for (auto thisSegment = gemSegments->begin(); thisSegment != gemSegments->end(); ++thisSegment,++SegmentNumber)

    //As long as the delR of the MuonCandidate is sensible, store the track-segment pair
    if (ClosestDelR2 < 500.) {
      muons->push_back(MuonCandidate);
    }
  }

  // put collection in event

  ev.put(muons);
}

FreeTrajectoryState
trackerGEM::getFTS(const GlobalVector& p3, const GlobalVector& r3, 
		   int charge, const AlgebraicSymMatrix55& cov,
		   const MagneticField* field){

  GlobalVector p3GV(p3.x(), p3.y(), p3.z());
  GlobalPoint r3GP(r3.x(), r3.y(), r3.z());
  GlobalTrajectoryParameters tPars(r3GP, p3GV, charge, field);

  CurvilinearTrajectoryError tCov(cov);
  
  return cov.kRows == 5 ? FreeTrajectoryState(tPars, tCov) : FreeTrajectoryState(tPars) ;
}

FreeTrajectoryState
trackerGEM::getFTS(const GlobalVector& p3, const GlobalVector& r3, 
		   int charge, const AlgebraicSymMatrix66& cov,
		   const MagneticField* field){

  GlobalVector p3GV(p3.x(), p3.y(), p3.z());
  GlobalPoint r3GP(r3.x(), r3.y(), r3.z());
  GlobalTrajectoryParameters tPars(r3GP, p3GV, charge, field);

  CartesianTrajectoryError tCov(cov);
  
  return cov.kRows == 6 ? FreeTrajectoryState(tPars, tCov) : FreeTrajectoryState(tPars) ;
}

void trackerGEM::getFromFTS(const FreeTrajectoryState& fts,
			    GlobalVector& p3, GlobalVector& r3, 
			    int& charge, AlgebraicSymMatrix66& cov){
  GlobalVector p3GV = fts.momentum();
  GlobalPoint r3GP = fts.position();

  GlobalVector p3T(p3GV.x(), p3GV.y(), p3GV.z());
  GlobalVector r3T(r3GP.x(), r3GP.y(), r3GP.z());
  p3 = p3T;
  r3 = r3T;  
  // p3.set(p3GV.x(), p3GV.y(), p3GV.z());
  // r3.set(r3GP.x(), r3GP.y(), r3GP.z());
  
  charge = fts.charge();
  cov = fts.hasError() ? fts.cartesianError().matrix() : AlgebraicSymMatrix66();

}


void trackerGEM::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup)
{
  iSetup.get<MuonGeometryRecord>().get(gemGeom);
}


DEFINE_FWK_MODULE(trackerGEM);
