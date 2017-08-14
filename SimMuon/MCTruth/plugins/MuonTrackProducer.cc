//
// modified & integrated by Giovanni Abbiendi
// from code by Arun Luthra: UserCode/luthra/MuonTrackSelector/src/MuonTrackSelector.cc
//
#include "SimMuon/MCTruth/plugins/MuonTrackProducer.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include "DataFormats/MuonDetId/interface/DTChamberId.h"
#include "DataFormats/MuonDetId/interface/MuonSubdetId.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include <sstream>

#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
#include "Geometry/Records/interface/TrackerTopologyRcd.h"

std::vector<double> MuonTrackProducer::findSimVtx(edm::Event& iEvent){

  edm::Handle<reco::GenParticleCollection> genParticles;
  iEvent.getByToken(genP_Token, genParticles);
    
  std::vector<double> vtxCoord;
  vtxCoord.push_back(0);
  vtxCoord.push_back(0);
  vtxCoord.push_back(0);
  vtxCoord.push_back(0);
  vtxCoord.push_back(0);
  vtxCoord.push_back(0);
  vtxCoord.push_back(0);

  if(genParticles.isValid()){

  	for(reco::GenParticleCollection::const_iterator itg = genParticles->begin(); itg != genParticles->end(); ++itg ){

		int id = itg->pdgId();
		int status = itg->status();
		//int nDaughters = itg->numberOfDaughters();
		int nMothers = itg->numberOfMothers();
//		double phiGen = itg->phi();
//		double etaGen = itg->eta();
//		if( abs(id) == 6 ) std::cout<<"id "<<id<<" "<<status<<" "<<nMothers<<" "<<phiGen<<" "<<etaGen<<std::endl;

		if((abs(id) == 23 || abs(id) == 22) && status == 22){//For DY samples

	 		vtxCoord[0] = 1;

			vtxCoord[4] = (double)(itg->vx()); 
			vtxCoord[5] = (double)(itg->vy());
			vtxCoord[6] = (double)(itg->vz());

		}
        else if((abs(id) == 23 || abs(id) == 22) && nMothers == 2){//For ZMM samples
            
            vtxCoord[0] = 1;
            
            vtxCoord[4] = (double)(itg->vx());
            vtxCoord[5] = (double)(itg->vy());
            vtxCoord[6] = (double)(itg->vz());
            
        }

		//if(fabs(id) == 23 && nMothers == 2) std::cout<<"ID "<<id<<" Status "<<status<<std::endl;

		else if(abs(id) == 13 && status == 1 && nMothers == 0){//For muon gun samples

	 		vtxCoord[0] = 2;

			vtxCoord[1] = (double)(itg->vx()); 
			vtxCoord[2] = (double)(itg->vy());
			vtxCoord[3] = (double)(itg->vz());

		}

		else if(abs(id) == 12 && status == 1 && nMothers == 0){//For nu gun samples

	 		vtxCoord[0] = 3;

			vtxCoord[1] = (double)(itg->vx()); 
			vtxCoord[2] = (double)(itg->vy());
			vtxCoord[3] = (double)(itg->vz());

		}
        else if(abs(id) == 6 && nMothers == 2){//For nu gun samples

	 		vtxCoord[0] = 1;

			vtxCoord[4] = (double)(itg->vx());
			vtxCoord[5] = (double)(itg->vy());
			vtxCoord[6] = (double)(itg->vz());
            
//            std::cout<<"ttbar x: "<<vtxCoord[4]<<", y: "<<vtxCoord[5]<<", z: "<<vtxCoord[6]<<std::endl;

		}

	}

  }

  //std::cout<<vtxCoord.size()<<" "<<vtxCoord[0]<<std::endl;
  return vtxCoord;

}

bool MuonTrackProducer::isGlobalTightMuon( const reco::MuonCollection::const_iterator muonRef ) {

 //if ( !muonRef.isNonnull() ) return false;

 if ( !muonRef->isGlobalMuon() ) return false;
 if ( !muonRef->isStandAloneMuon() ) return false;
 
 
 if ( muonRef->isTrackerMuon() ) { 
   
   bool result = muon::isGoodMuon(*muonRef,muon::GlobalMuonPromptTight);
   
   bool isTM2DCompatibilityTight =  muon::isGoodMuon(*muonRef,muon::TM2DCompatibilityTight);   
   int nMatches = muonRef->numberOfMatches();
   bool quality = nMatches > 2 || isTM2DCompatibilityTight;
   
   return result && quality;
   
 } else {
 
   reco::TrackRef standAloneMu = muonRef->standAloneMuon();
   
    // No tracker muon -> Request a perfect stand-alone muon, or an even better global muon
    bool result = false;
      
    // Check the quality of the stand-alone muon : 
    // good chi**2 and large number of hits and good pt error
    if ( ( standAloneMu->hitPattern().numberOfValidMuonDTHits() < 22 &&
	   standAloneMu->hitPattern().numberOfValidMuonCSCHits() < 15 ) ||
	 standAloneMu->normalizedChi2() > 10. || 
	 standAloneMu->ptError()/standAloneMu->pt() > 0.20 ) {
      result = false;
    } else { 
      
      reco::TrackRef combinedMu = muonRef->combinedMuon();
      reco::TrackRef trackerMu = muonRef->track();
            
      // If the stand-alone muon is good, check the global muon
      if ( combinedMu->normalizedChi2() > standAloneMu->normalizedChi2() ) {
	// If the combined muon is worse than the stand-alone, it 
	// means that either the corresponding tracker track was not 
	// reconstructed, or that the sta muon comes from a late 
	// pion decay (hence with a momentum smaller than the track)
	// Take the stand-alone muon only if its momentum is larger
	// than that of the track
	result = standAloneMu->pt() > trackerMu->pt() ;
     } else { 
	// If the combined muon is better (and good enough), take the 
	// global muon
	result = 
	  combinedMu->ptError()/combinedMu->pt() < 
	  std::min(0.20,standAloneMu->ptError()/standAloneMu->pt());
      }
    }      

    return result;    
  }

  return false;

}

bool MuonTrackProducer::isTrackerTightMuon( const reco::MuonCollection::const_iterator muonRef ) {

  //if ( !muonRef.isNonnull() ) return false;
    
  if( !muonRef->isTrackerMuon() ) return false;
  
  reco::TrackRef trackerMu = muonRef->track();
  const reco::Track& track = *trackerMu;
  
  unsigned nTrackerHits =  track.hitPattern().numberOfValidTrackerHits();
  
  if(nTrackerHits<=12) return false;
  
  bool isAllArbitrated = muon::isGoodMuon(*muonRef,muon::AllArbitrated);
  
  bool isTM2DCompatibilityTight = muon::isGoodMuon(*muonRef,muon::TM2DCompatibilityTight);
  
  if(!isAllArbitrated || !isTM2DCompatibilityTight)  return false;

  if((trackerMu->ptError()/trackerMu->pt() > 0.10)){
    //std::cout<<" PT ERROR > 10 % "<< trackerMu->pt() <<std::endl;
    return false;
  }
  return true;
  
}

bool MuonTrackProducer::isIsolatedMuon( const reco::MuonCollection::const_iterator muonRef ){


  //if ( !muonRef.isNonnull() ) return false;
  if ( !muonRef->isIsolationValid() ) return false;
  
  // Isolated Muons which are missed by standard cuts are nearly always global+tracker
  if ( !muonRef->isGlobalMuon() ) return false;

  // If it's not a tracker muon, only take it if there are valid muon hits

  reco::TrackRef standAloneMu = muonRef->standAloneMuon();

  if ( !muonRef->isTrackerMuon() ){
    if(standAloneMu->hitPattern().numberOfValidMuonDTHits() == 0 &&
       standAloneMu->hitPattern().numberOfValidMuonCSCHits() ==0) return false;
  }
  
  // for isolation, take the smallest pt available to reject fakes

  reco::TrackRef combinedMu = muonRef->combinedMuon();
  double smallestMuPt = combinedMu->pt();
  
  if(standAloneMu->pt()<smallestMuPt) smallestMuPt = standAloneMu->pt();
  
  if(muonRef->isTrackerMuon())
    {
      reco::TrackRef trackerMu = muonRef->track();
      if(trackerMu->pt() < smallestMuPt) smallestMuPt= trackerMu->pt();
    }
     
  double sumPtR03 = muonRef->isolationR03().sumPt;
  double emEtR03 = muonRef->isolationR03().emEt;
  double hadEtR03 = muonRef->isolationR03().hadEt;
  
  double relIso = (sumPtR03 + emEtR03 + hadEtR03)/smallestMuPt;

  if(relIso<0.1) return true;
  else return false;
}

bool MuonTrackProducer::isME0MuonSel(reco::MuonCollection::const_iterator muon, double pullXCut, double dXCut, double pullYCut, double dYCut, double dPhiCut)
{
    
  bool result = false;
  bool isME0 = muon->isME0Muon();
    
  if(isME0){
  
      double deltaX = 999;
      double deltaY = 999;
      double pullX = 999;
      double pullY = 999;
      double deltaPhi = 999;
    
      bool X_MatchFound = false, Y_MatchFound = false, Dir_MatchFound = false;
      
      const std::vector<reco::MuonChamberMatch>& chambers = muon->matches();
      for( std::vector<reco::MuonChamberMatch>::const_iterator chamber = chambers.begin(); chamber != chambers.end(); ++chamber ){
          
          if (chamber->detector() == 5){
          
              for ( std::vector<reco::MuonSegmentMatch>::const_iterator segment = chamber->me0Matches.begin(); segment != chamber->me0Matches.end(); ++segment ){
              
                  deltaX   = fabs(chamber->x - segment->x );
                  deltaY   = fabs(chamber->y - segment->y );
                  pullX    = fabs(chamber->x - segment->x ) / std::sqrt(chamber->xErr + segment->xErr);
                  pullY    = fabs(chamber->y - segment->y ) / std::sqrt(chamber->yErr + segment->yErr);
                  deltaPhi = fabs(atan(chamber->dXdZ) - atan(segment->dXdZ));
                  
                  if ( (pullX < pullXCut) || (deltaX < dXCut) ) X_MatchFound = true;
                  if ( (pullY < pullYCut) || (deltaY < dYCut) ) Y_MatchFound = true;
                  if ( deltaPhi < dPhiCut ) Dir_MatchFound = true;
                  
                  if (X_MatchFound && Y_MatchFound && Dir_MatchFound) result = true;
                  
              }
          }
      }
      
  }
    
  return result;
    
}

bool MuonTrackProducer::isME0MuonSelNew(edm::Event& iEvent, const edm::EventSetup& iSetup, reco::MuonCollection::const_iterator muon, double dEtaCut, double dPhiCut, double dPhiBendCut)
{
    
    bool result = false;
    bool isME0 = muon->isME0Muon();
    
    if(isME0){
        
        double deltaEta = 999;
        double deltaPhi = 999;
        double deltaPhiBend = 999;

        const std::vector<reco::MuonChamberMatch>& chambers = muon->matches();
        for( std::vector<reco::MuonChamberMatch>::const_iterator chamber = chambers.begin(); chamber != chambers.end(); ++chamber ){
            
            if (chamber->detector() == 5){
            
                for ( std::vector<reco::MuonSegmentMatch>::const_iterator segment = chamber->me0Matches.begin(); segment != chamber->me0Matches.end(); ++segment ){
                
                    LocalPoint trk_loc_coord(chamber->x, chamber->y, 0);
                    LocalPoint seg_loc_coord(segment->x, segment->y, 0);
                    LocalVector trk_loc_vec(chamber->dXdZ, chamber->dYdZ, 1);
                    
                    edm::ESHandle<ME0Geometry> hGeom;
                    iSetup.get<MuonGeometryRecord>().get(hGeom);
                    const ME0Geometry* ME0Geometry_ =( &*hGeom);
                    const ME0Chamber * me0chamber = ME0Geometry_->chamber(chamber->id);
                    
                    GlobalPoint trk_glb_coord = me0chamber->toGlobal(trk_loc_coord);
                    GlobalPoint seg_glb_coord = me0chamber->toGlobal(seg_loc_coord);
                    
                    double segDPhi = segment->me0SegmentRef->deltaPhi();
                    double trackDPhi = me0chamber->computeDeltaPhi(trk_loc_coord, trk_loc_vec);
                    
                    deltaEta = fabs(trk_glb_coord.eta() - seg_glb_coord.eta() );
                    deltaPhi = fabs(trk_glb_coord.phi() - seg_glb_coord.phi() );
                    deltaPhiBend = fabs(segDPhi - trackDPhi);
                    
                    if (deltaEta < dEtaCut && deltaPhi < dPhiCut && deltaPhiBend < dPhiBendCut) result = true;
                    
//                    if(result && ((muon->pt()) < 5) && (fabs(muon->eta()) > 1.5) && (fabs(muon->eta()) < 2.0)) std::cout<<"Muon pT: "<<(muon->pt())<<", muon p: "<<(muon->p())<<", muon |eta|: "<<fabs(muon->eta())<<", muon vz: "<<(muon->vz())<<", extrapolated track |eta|: "<<fabs(trk_glb_coord.eta())<<", me0 segment |eta|: "<<fabs(seg_glb_coord.eta())<<std::endl;
                    
                }
            }
        }
        
    }
    
    return result;
    
}

bool MuonTrackProducer::isLoose(edm::Event& iEvent, reco::MuonCollection::const_iterator muon)
{
  bool isPF = muon->isPFMuon();
  bool isGLB = muon->isGlobalMuon();
  bool isTrk = muon->isTrackerMuon();

  return ( isPF && ( isGLB || isTrk ) );
}

bool MuonTrackProducer::isLooseMod(edm::Event& iEvent, reco::MuonCollection::const_iterator muon)
{
    bool isPF = isGlobalTightMuon(muon) || isTrackerTightMuon(muon) || isIsolatedMuon(muon);
    bool isGLB = muon->isGlobalMuon();
    bool isTrk = muon->isTrackerMuon();
    
    return ( isPF && ( isGLB || isTrk ) );
}

std::vector<bool> MuonTrackProducer::isLooseModExt(edm::Event& iEvent, const edm::EventSetup& iSetup, reco::MuonCollection::const_iterator muon)
{
//    bool isPF = isGlobalTightMuon(muon) || isTrackerTightMuon(muon) || isIsolatedMuon(muon);
    bool isPF = muon->isPFMuon();
    bool isGLB = muon->isGlobalMuon();
    bool isTrk = muon->isTrackerMuon();
    
    double mom = muon->p();
    double dPhiCut_ = std::min(std::max(1.2/mom,1.2/100),0.056);
    double dPhiBendCut_ = std::min(std::max(0.2/mom,0.2/100),0.0096);
    bool isME0 = isME0MuonSelNew(iEvent, iSetup, muon, 0.077, dPhiCut_, dPhiBendCut_);
    bool looseID = isPF && (isGLB || isTrk);
//    bool isME0 = isME0MuonSel(muon, 3, 4, 3, 4, 0.1);
    
    std::vector<bool> allResults;
    allResults.push_back(looseID);
    allResults.push_back(isME0);
    
    return allResults;
}

bool MuonTrackProducer::isME0MuonLoose(edm::Event& iEvent, const edm::EventSetup& iSetup, reco::MuonCollection::const_iterator muon)
{
    
    bool isME0 = false;
    bool validPxlHit = false;
    bool highPurity = false;
    
    if (muon->muonBestTrack().isNonnull() && muon->innerTrack().isNonnull()){
        
        double mom = muon->p();
        double dPhiCut_ = std::min(std::max(1.2/mom,1.2/100),0.056);
        double dPhiBendCut_ = std::min(std::max(0.2/mom,0.2/100),0.0096);
        isME0 = isME0MuonSelNew(iEvent, iSetup, muon, 0.077, dPhiCut_, dPhiBendCut_);
        validPxlHit = muon->innerTrack()->hitPattern().numberOfValidPixelHits() > 0;
        highPurity = muon->innerTrack()->quality(reco::Track::highPurity);
    //    isME0 = isME0MuonSel(muon, 3, 4, 3, 4, 0.1);
        
    }
    
    return (isME0 && validPxlHit && highPurity);
}

bool::MuonTrackProducer::isTightClassic(edm::Event& iEvent, reco::MuonCollection::const_iterator muon, bool useIPxy, bool useIPz)
{
    
    bool result = false;
    edm::Handle<reco::VertexCollection> vertexHandle;
    iEvent.getByToken(vtx_Token,vertexHandle);
    
    //result = muon::isTightMuon(*muon, (*vertexHandle)[0]);
    
    if (muon->muonBestTrack().isNonnull() && muon->innerTrack().isNonnull() && muon->globalTrack().isNonnull()){
        
        edm::Handle<reco::VertexCollection> vertexHandle;
        iEvent.getByToken(vtx_Token,vertexHandle);
        const reco::VertexCollection* vertices = vertexHandle.product();
        
        bool trkLayMeas = muon->innerTrack()->hitPattern().trackerLayersWithMeasurement() > 5;
        bool isGlb = muon->isGlobalMuon();
        bool isPF= isGlobalTightMuon(muon) || isTrackerTightMuon(muon) || isIsolatedMuon(muon); //Workaround!!!
        bool chi2 = muon->globalTrack()->normalizedChi2() < 10.;
        bool validHits = muon->globalTrack()->hitPattern().numberOfValidMuonHits() > 0;
        bool matchedSt = muon->numberOfMatchedStations() > 1;
        bool validPxlHit = muon->innerTrack()->hitPattern().numberOfValidPixelHits() > 0;
        
        bool ipxy = false;
        bool ipz = false;
        if(useIPxy == true){
            
            if(vertices->size() !=0){
                
                ipxy = fabs(muon->muonBestTrack()->dxy((*vertices)[0].position())) < 0.2;
                //std::cout<<"vx: "<<pointDY.x()<<" vy: "<<pointDY.y()<<" vz: "<<pointDY.z()<<" |Dxy|: "<<ipxy<<std::endl;
                
            }
            
        }
        else if(useIPxy == false) ipxy = true;
        
        if(useIPz == true){
            
            if(vertices->size() !=0){
                
                ipz = fabs(muon->muonBestTrack()->dz((*vertices)[0].position())) < 0.5;
                //std::cout<<"vx: "<<pointDY.x()<<" vy: "<<pointDY.y()<<" vz: "<<pointDY.z()<<" |Dz|: "<<ipz<<std::endl;
                
            }
            
        }
        else if(useIPz == false) ipz = true;
        
        if(trkLayMeas && isGlb && isPF && chi2 && validHits && matchedSt && ipxy && ipz && validPxlHit) result = true;
        
    }
    
    return result;
    
}

bool MuonTrackProducer::isTight(edm::Event& iEvent, reco::MuonCollection::const_iterator muon, bool useIPxy, bool useIPz)
{
  bool result = false;

  if (muon->muonBestTrack().isNonnull() && muon->innerTrack().isNonnull() && muon->globalTrack().isNonnull()){

	std::vector<double> vtxCoord = findSimVtx(iEvent);
    GlobalPoint point(vtxCoord[1],vtxCoord[2],vtxCoord[3]);
    GlobalPoint pointDY(vtxCoord[4],vtxCoord[5],vtxCoord[6]);

	//double muonX = muon->vx();
	//double muonY = muon->vy();
	//double muonZ = muon->vz();

	double muonZ = pointDY.z();
		
 	edm::Handle<reco::VertexCollection> vertexHandle;
  	iEvent.getByToken(vtx_Token,vertexHandle);
  	const reco::VertexCollection* vertices = vertexHandle.product();

	double distInit = 24;
	int indexFinal = 0;
	for(int i = 0; i < (int)vertices->size(); i++){

		//double vtxX = (*vertices)[i].x();
		//double vtxY = (*vertices)[i].y();
		double vtxZ = (*vertices)[i].z();

		double dist = fabs(muonZ - vtxZ);
		//std::cout<<"dist "<<dist<<std::endl;
		if(dist < distInit){
				
			distInit = dist;
			indexFinal = i;

		}

	}
	//std::cout<<distInit<<" "<<indexFinal<<std::endl;

	double ipxySim = 999;
	double ipzSim = 999;
	
	if(vtxCoord[0] > 1.5 && vtxCoord[0] < 3.5){//Mu and nu gun samples

        ipxySim = fabs(muon->muonBestTrack()->dxy(math::XYZPoint(point.x(),point.y(),point.z())));
        ipzSim = fabs(muon->muonBestTrack()->dz(math::XYZPoint(point.x(),point.y(),point.z())));
        
	}
	else if(vtxCoord[0] > 0.5 && vtxCoord[0] < 1.5){//DY samples

		ipxySim = fabs(muon->muonBestTrack()->dxy(math::XYZPoint(pointDY.x(),pointDY.y(),pointDY.z())));
        ipzSim = fabs(muon->muonBestTrack()->dz(math::XYZPoint(pointDY.x(),pointDY.y(),pointDY.z())));

	}
	bool ipxySimBool = ipxySim < 0.2;
	bool ipzSimBool = ipzSim < 0.5;
    //std::cout<<"vx: "<<point.x()<<" vy: "<<point.y()<<" vz: "<<point.z()<<" |Dxy|: "<<ipxySim<<" "<<ipxySimBool<<" |Dz|: "<<ipzSim<<" "<<ipzSimBool<<std::endl;
    //std::cout<<"vx: "<<pointDY.x()<<" vy: "<<pointDY.y()<<" vz: "<<pointDY.z()<<" |Dxy|: "<<ipxySim<<" "<<ipxySimBool<<" |Dz|: "<<ipzSim<<" "<<ipzSimBool<<std::endl;

	bool trkLayMeas = muon->innerTrack()->hitPattern().trackerLayersWithMeasurement() > 5; 
	bool isGlb = muon->isGlobalMuon(); 
	bool isPF = muon->isPFMuon(); 
	bool chi2 = muon->globalTrack()->normalizedChi2() < 10.; 
	bool validHits = muon->globalTrack()->hitPattern().numberOfValidMuonHits() > 0; 
	bool matchedSt = muon->numberOfMatchedStations() > 1;

	bool ipxy = false;
	bool ipz = false;
	if(useIPxy == true){

        if(vertices->size() !=0 && vtxCoord[0] > 0.5 && vtxCoord[0] < 1.5){
            
            ipxy = fabs(muon->muonBestTrack()->dxy((*vertices)[indexFinal].position())) < 0.2;
            //std::cout<<"vx: "<<pointDY.x()<<" vy: "<<pointDY.y()<<" vz: "<<pointDY.z()<<" |Dxy|: "<<ipxy<<std::endl;

        }
        else if(vtxCoord[0] > 1.5 && vtxCoord[0] < 3.5){
            
            ipxy = ipxySimBool;
            //std::cout<<"vx: "<<point.x()<<" vy: "<<point.y()<<" vz: "<<point.z()<<" |Dxy|: "<<ipxy<<std::endl;
            
        }

	}
	else if(useIPxy == false) ipxy = true;

 	if(useIPz == true){

        if(vertices->size() !=0 && vtxCoord[0] > 0.5 && vtxCoord[0] < 1.5){
            
            ipz = fabs(muon->muonBestTrack()->dz((*vertices)[indexFinal].position())) < 0.5;
            //std::cout<<"vx: "<<pointDY.x()<<" vy: "<<pointDY.y()<<" vz: "<<pointDY.z()<<" |Dz|: "<<ipz<<std::endl;

        }
        else if(vtxCoord[0] > 1.5 && vtxCoord[0] < 3.5){
            
            ipz = ipzSimBool;
            //std::cout<<"vx: "<<point.x()<<" vy: "<<point.y()<<" vz: "<<point.z()<<" |Dz|: "<<ipz<<std::endl;

        }

	}
	else if(useIPz == false) ipz = true;
      
	bool validPxlHit = muon->innerTrack()->hitPattern().numberOfValidPixelHits() > 0;
	//bool validPxlHit = muon->innerTrack()->hitPattern().pixelLayersWithMeasurement(3,2) > 0;
	//bool validPxlHit = muon->innerTrack()->hitPattern().pixelLayersWithMeasurement(4,3) > 0;
      
    //std::cout<<trkLayMeas<<" "<<isGlb<<" "<<isPF<<" "<<chi2<<" "<<validHits<<" "<<matchedSt<<" "<<ipxy<<" "<<ipz<<" "<<validPxlHit<<std::endl;

	if(trkLayMeas && isGlb && isPF && chi2 && validHits && matchedSt && ipxy && ipz && validPxlHit) result = true;

  }

  return result;
}

bool MuonTrackProducer::isTightMod(edm::Event& iEvent, reco::MuonCollection::const_iterator muon, bool useIPxy, bool useIPz)
{
    bool result = false;
    
    if (muon->muonBestTrack().isNonnull() && muon->innerTrack().isNonnull() && muon->globalTrack().isNonnull()){
        
        std::vector<double> vtxCoord = findSimVtx(iEvent);
        GlobalPoint point(vtxCoord[1],vtxCoord[2],vtxCoord[3]);
        GlobalPoint pointDY(vtxCoord[4],vtxCoord[5],vtxCoord[6]);
        
        //double muonX = muon->vx();
        //double muonY = muon->vy();
        //double muonZ = muon->vz();
        
        double muonZ = pointDY.z();
        
        edm::Handle<reco::VertexCollection> vertexHandle;
        iEvent.getByToken(vtx_Token,vertexHandle);
        const reco::VertexCollection* vertices = vertexHandle.product();
        
        double distInit = 24;
        int indexFinal = 0;
        for(int i = 0; i < (int)vertices->size(); i++){
            
            //double vtxX = (*vertices)[i].x();
            //double vtxY = (*vertices)[i].y();
            double vtxZ = (*vertices)[i].z();
            
            double dist = fabs(muonZ - vtxZ);
            //std::cout<<"dist "<<dist<<std::endl;
            if(dist < distInit){
                
                distInit = dist;
                indexFinal = i;
                
            }
            
        }
        //std::cout<<distInit<<" "<<indexFinal<<std::endl;
        
        double ipxySim = 999;
        double ipzSim = 999;
        
        if(vtxCoord[0] > 1.5 && vtxCoord[0] < 3.5){//Mu and nu gun samples
            
            ipxySim = fabs(muon->muonBestTrack()->dxy(math::XYZPoint(point.x(),point.y(),point.z())));
            ipzSim = fabs(muon->muonBestTrack()->dz(math::XYZPoint(point.x(),point.y(),point.z())));
            
        }
        else if(vtxCoord[0] > 0.5 && vtxCoord[0] < 1.5){//DY samples
            
            ipxySim = fabs(muon->muonBestTrack()->dxy(math::XYZPoint(pointDY.x(),pointDY.y(),pointDY.z())));
            ipzSim = fabs(muon->muonBestTrack()->dz(math::XYZPoint(pointDY.x(),pointDY.y(),pointDY.z())));
            
        }
        bool ipxySimBool = ipxySim < 0.2;
        bool ipzSimBool = ipzSim < 0.5;
        //std::cout<<"vx: "<<point.x()<<" vy: "<<point.y()<<" vz: "<<point.z()<<" |Dxy|: "<<ipxySim<<" "<<ipxySimBool<<" |Dz|: "<<ipzSim<<" "<<ipzSimBool<<std::endl;
        //std::cout<<"vx: "<<pointDY.x()<<" vy: "<<pointDY.y()<<" vz: "<<pointDY.z()<<" |Dxy|: "<<ipxySim<<" "<<ipxySimBool<<" |Dz|: "<<ipzSim<<" "<<ipzSimBool<<std::endl;
        
        bool trkLayMeas = muon->innerTrack()->hitPattern().trackerLayersWithMeasurement() > 5;
        bool isGlb = muon->isGlobalMuon();
        bool isPF= isGlobalTightMuon(muon) || isTrackerTightMuon(muon) || isIsolatedMuon(muon);
        bool chi2 = muon->globalTrack()->normalizedChi2() < 10.;
        bool validHits = muon->globalTrack()->hitPattern().numberOfValidMuonHits() > 0;
        bool matchedSt = muon->numberOfMatchedStations() > 1;
        
        bool ipxy = false;
        bool ipz = false;
        if(useIPxy == true){
            
            if(vertices->size() !=0 && vtxCoord[0] > 0.5 && vtxCoord[0] < 1.5){
                
                ipxy = fabs(muon->muonBestTrack()->dxy((*vertices)[indexFinal].position())) < 0.2;
                //std::cout<<"vx: "<<pointDY.x()<<" vy: "<<pointDY.y()<<" vz: "<<pointDY.z()<<" |Dxy|: "<<ipxy<<std::endl;
                
            }
            else if(vtxCoord[0] > 1.5 && vtxCoord[0] < 3.5){
                
                ipxy = ipxySimBool;
                //std::cout<<"vx: "<<point.x()<<" vy: "<<point.y()<<" vz: "<<point.z()<<" |Dxy|: "<<ipxy<<std::endl;
                
            }
            
        }
        else if(useIPxy == false) ipxy = true;
        
        if(useIPz == true){
            
            if(vertices->size() !=0 && vtxCoord[0] > 0.5 && vtxCoord[0] < 1.5){
                
                ipz = fabs(muon->muonBestTrack()->dz((*vertices)[indexFinal].position())) < 0.5;
                //std::cout<<"vx: "<<pointDY.x()<<" vy: "<<pointDY.y()<<" vz: "<<pointDY.z()<<" |Dz|: "<<ipz<<std::endl;
                
            }
            else if(vtxCoord[0] > 1.5 && vtxCoord[0] < 3.5){
                
                ipz = ipzSimBool;
                //std::cout<<"vx: "<<point.x()<<" vy: "<<point.y()<<" vz: "<<point.z()<<" |Dz|: "<<ipz<<std::endl;
                
            }
            
        }
        else if(useIPz == false) ipz = true;
        
        bool validPxlHit = muon->innerTrack()->hitPattern().numberOfValidPixelHits() > 0;
        //bool validPxlHit = muon->innerTrack()->hitPattern().pixelLayersWithMeasurement(3,2) > 0;
        //bool validPxlHit = muon->innerTrack()->hitPattern().pixelLayersWithMeasurement(4,3) > 0;
        
        //std::cout<<trkLayMeas<<" "<<isGlb<<" "<<isPF<<" "<<chi2<<" "<<validHits<<" "<<matchedSt<<" "<<ipxy<<" "<<ipz<<" "<<validPxlHit<<std::endl;
        
        if(trkLayMeas && isGlb && isPF && chi2 && validHits && matchedSt && ipxy && ipz && validPxlHit) result = true;
        
    }
    
    return result;
}

std::vector<bool> MuonTrackProducer::isTightModExt(edm::Event& iEvent, const edm::EventSetup& iSetup, reco::MuonCollection::const_iterator muon, bool useIPxy, bool useIPz)
{
    bool tightID = false;
    
    bool trkLayMeas = false;
    bool isGlb = false;
    bool isPF = false;
    bool chi2 = false;
    bool validHits = false;
    bool matchedSt = false;
    bool ipxy = false;
    bool ipz = false;
    bool validPxlHit = false;
    bool highPurity = false;
    
    bool glbExist = muon->globalTrack().isNonnull();
    bool bestExist = muon->muonBestTrack().isNonnull();
    bool innerExist = muon->innerTrack().isNonnull();
    
    isGlb = muon->isGlobalMuon();
    //isPF= isGlobalTightMuon(muon) || isTrackerTightMuon(muon) || isIsolatedMuon(muon);
    isPF = muon->isPFMuon();
    matchedSt = muon->numberOfMatchedStations() > 1;

    if(glbExist){

        chi2 = muon->globalTrack()->normalizedChi2() < 10.;
        validHits = muon->globalTrack()->hitPattern().numberOfValidMuonHits() > 0;
        
    }
    
    if(innerExist){
        
        trkLayMeas = muon->innerTrack()->hitPattern().trackerLayersWithMeasurement() > 5;
        highPurity = muon->innerTrack()->quality(reco::Track::highPurity);
        validPxlHit = muon->innerTrack()->hitPattern().numberOfValidPixelHits() > 0;
        //validPxlHit = muon->innerTrack()->hitPattern().pixelLayersWithMeasurement(3,2) > 0;
        //validPxlHit = muon->innerTrack()->hitPattern().pixelLayersWithMeasurement(4,3) > 0;
        
    }
    
    if (bestExist || glbExist || innerExist){
        
        std::vector<double> vtxCoord = findSimVtx(iEvent);
        GlobalPoint point(vtxCoord[1],vtxCoord[2],vtxCoord[3]);
        GlobalPoint pointDY(vtxCoord[4],vtxCoord[5],vtxCoord[6]);
        
        //double muonX = muon->vx();
        //double muonY = muon->vy();
        //double muonZ = muon->vz();
        
        double muonZ = pointDY.z();
        
        edm::Handle<reco::VertexCollection> vertexHandle;
        iEvent.getByToken(vtx_Token,vertexHandle);
        const reco::VertexCollection* vertices = vertexHandle.product();
        
        double distInit = 24;
        int indexFinal = 0;
        
//        std::cout<<"Vtx[0] x: "<<(*vertices)[0].x()<<", y: "<<(*vertices)[0].y()<<", z: "<<(*vertices)[0].z()<<std::endl;
        for(int i = 0; i < (int)vertices->size(); i++){
            
            //double vtxX = (*vertices)[i].x();
            //double vtxY = (*vertices)[i].y();
            double vtxZ = (*vertices)[i].z();
            
            double dist = fabs(muonZ - vtxZ);
            //std::cout<<"dist "<<dist<<std::endl;
            if(dist < distInit){
                
                distInit = dist;
                indexFinal = i;
                
            }
            
        }
//        double ipxy_reco = fabs(muon->muonBestTrack()->dxy((*vertices)[0].position()));
//        double ipz_reco = fabs(muon->muonBestTrack()->dz((*vertices)[0].position()));
//        std::cout<<"Distance in z: "<<distInit<<", final index: "<<indexFinal<<", reco dxy: "<<ipxy_reco<<", reco ipz: "<<ipz_reco<<std::endl;
        
        double ipxySim = 999;
        double ipzSim = 999;
        
        if(vtxCoord[0] > 1.5 && vtxCoord[0] < 3.5){//Mu and nu gun samples
            
            ipxySim = fabs(muon->muonBestTrack()->dxy(math::XYZPoint(point.x(),point.y(),point.z())));
            ipzSim = fabs(muon->muonBestTrack()->dz(math::XYZPoint(point.x(),point.y(),point.z())));
            
        }
        else if(vtxCoord[0] > 0.5 && vtxCoord[0] < 1.5){//DY samples
            
            ipxySim = fabs(muon->muonBestTrack()->dxy(math::XYZPoint(pointDY.x(),pointDY.y(),pointDY.z())));
            ipzSim = fabs(muon->muonBestTrack()->dz(math::XYZPoint(pointDY.x(),pointDY.y(),pointDY.z())));
            
        }
        bool ipxySimBool = ipxySim < 0.2;
        bool ipzSimBool = ipzSim < 0.5;
//        std::cout<<"vx: "<<point.x()<<" vy: "<<point.y()<<" vz: "<<point.z()<<" |Dxy|: "<<ipxySim<<" "<<ipxySimBool<<" |Dz|: "<<ipzSim<<" "<<ipzSimBool<<std::endl;
//        std::cout<<"vx: "<<pointDY.x()<<" vy: "<<pointDY.y()<<" vz: "<<pointDY.z()<<" |Dxy|: "<<ipxySim<<" "<<ipxySimBool<<" |Dz|: "<<ipzSim<<" "<<ipzSimBool<<std::endl;

        if(useIPxy == true){
            
            if(vertices->size() !=0 && vtxCoord[0] > 0.5 && vtxCoord[0] < 1.5){
                
                double ipxy_value = fabs(muon->muonBestTrack()->dxy((*vertices)[indexFinal].position()));
                ipxy = ipxy_value < 0.2;
//                std::cout<<"vx: "<<pointDY.x()<<" vy: "<<pointDY.y()<<" vz: "<<pointDY.z()<<" |Dxy|: "<<ipxy_value<<" Bool: "<<ipxy<<std::endl;
                
            }
            else if(vtxCoord[0] > 1.5 && vtxCoord[0] < 3.5){
                
                ipxy = ipxySimBool;
                //std::cout<<"vx: "<<point.x()<<" vy: "<<point.y()<<" vz: "<<point.z()<<" |Dxy|: "<<ipxy<<std::endl;
                
            }
            
        }
        else if(useIPxy == false) ipxy = true;
        
        if(useIPz == true){
            
            if(vertices->size() !=0 && vtxCoord[0] > 0.5 && vtxCoord[0] < 1.5){
                
                double ipz_value = fabs(muon->muonBestTrack()->dz((*vertices)[indexFinal].position()));
                ipz = ipz_value < 0.5;
//                std::cout<<"vx: "<<pointDY.x()<<" vy: "<<pointDY.y()<<" vz: "<<pointDY.z()<<" |Dz|: "<<ipz_value<<" Bool: "<<ipz<<std::endl;
                
            }
            else if(vtxCoord[0] > 1.5 && vtxCoord[0] < 3.5){
                
                ipz = ipzSimBool;
                //std::cout<<"vx: "<<point.x()<<" vy: "<<point.y()<<" vz: "<<point.z()<<" |Dz|: "<<ipz<<std::endl;
                
            }
            
        }
        else if(useIPz == false) ipz = true;
        
    }
    
    //std::cout<<trkLayMeas<<" "<<isGlb<<" "<<isPF<<" "<<chi2<<" "<<validHits<<" "<<matchedSt<<" "<<ipxy<<" "<<ipz<<" "<<validPxlHit<<std::endl;
    if(trkLayMeas && isGlb && isPF && chi2 && validHits && matchedSt && ipxy && ipz && validPxlHit) tightID = true;
    
    double mom = muon->p();
    double dPhiCut_ = std::min(std::max(1.2/mom,1.2/100),0.032);
    double dPhiBendCut_ = std::min(std::max(0.2/mom,0.2/100),0.0041);
    bool isME0 = isME0MuonSelNew(iEvent, iSetup, muon, 0.048, dPhiCut_, dPhiBendCut_) && ipxy && ipz && validPxlHit && highPurity;
    
    std::vector<bool> allResults;
    allResults.push_back(tightID);
    allResults.push_back(isME0);

    return allResults;
}

bool MuonTrackProducer::isME0MuonTight(edm::Event& iEvent, const edm::EventSetup& iSetup, reco::MuonCollection::const_iterator muon, bool useIPxy, bool useIPz)
{

    bool ipxy = false;
    bool ipz = false;
    bool validPxlHit = false;
    bool highPurity = false;
    
    bool bestExist = muon->muonBestTrack().isNonnull();
    bool innerExist = muon->innerTrack().isNonnull();
    if(innerExist){
        
        highPurity = muon->innerTrack()->quality(reco::Track::highPurity);
        validPxlHit = muon->innerTrack()->hitPattern().numberOfValidPixelHits() > 0;
        //validPxlHit = muon->innerTrack()->hitPattern().pixelLayersWithMeasurement(3,2) > 0;
        //validPxlHit = muon->innerTrack()->hitPattern().pixelLayersWithMeasurement(4,3) > 0;
        
    }
    
    if (bestExist || innerExist){
        
        std::vector<double> vtxCoord = findSimVtx(iEvent);
        GlobalPoint point(vtxCoord[1],vtxCoord[2],vtxCoord[3]);
        GlobalPoint pointDY(vtxCoord[4],vtxCoord[5],vtxCoord[6]);
        
        //double muonX = muon->vx();
        //double muonY = muon->vy();
        //double muonZ = muon->vz();
        
        double muonZ = pointDY.z();
        
        edm::Handle<reco::VertexCollection> vertexHandle;
        iEvent.getByToken(vtx_Token,vertexHandle);
        const reco::VertexCollection* vertices = vertexHandle.product();
        
        double distInit = 24;
        int indexFinal = 0;
        
        //        std::cout<<"Vtx[0] x: "<<(*vertices)[0].x()<<", y: "<<(*vertices)[0].y()<<", z: "<<(*vertices)[0].z()<<std::endl;
        for(int i = 0; i < (int)vertices->size(); i++){
            
            //double vtxX = (*vertices)[i].x();
            //double vtxY = (*vertices)[i].y();
            double vtxZ = (*vertices)[i].z();
            
            double dist = fabs(muonZ - vtxZ);
            //std::cout<<"dist "<<dist<<std::endl;
            if(dist < distInit){
                
                distInit = dist;
                indexFinal = i;
                
            }
            
        }
        //        double ipxy_reco = fabs(muon->muonBestTrack()->dxy((*vertices)[0].position()));
        //        double ipz_reco = fabs(muon->muonBestTrack()->dz((*vertices)[0].position()));
        //        std::cout<<"Distance in z: "<<distInit<<", final index: "<<indexFinal<<", reco dxy: "<<ipxy_reco<<", reco ipz: "<<ipz_reco<<std::endl;
        
        double ipxySim = 999;
        double ipzSim = 999;
        
        if(vtxCoord[0] > 1.5 && vtxCoord[0] < 3.5){//Mu and nu gun samples
            
            ipxySim = fabs(muon->muonBestTrack()->dxy(math::XYZPoint(point.x(),point.y(),point.z())));
            ipzSim = fabs(muon->muonBestTrack()->dz(math::XYZPoint(point.x(),point.y(),point.z())));
            
        }
        else if(vtxCoord[0] > 0.5 && vtxCoord[0] < 1.5){//DY samples
            
            ipxySim = fabs(muon->muonBestTrack()->dxy(math::XYZPoint(pointDY.x(),pointDY.y(),pointDY.z())));
            ipzSim = fabs(muon->muonBestTrack()->dz(math::XYZPoint(pointDY.x(),pointDY.y(),pointDY.z())));
            
        }
        bool ipxySimBool = ipxySim < 0.2;
        bool ipzSimBool = ipzSim < 0.5;
        //        std::cout<<"vx: "<<point.x()<<" vy: "<<point.y()<<" vz: "<<point.z()<<" |Dxy|: "<<ipxySim<<" "<<ipxySimBool<<" |Dz|: "<<ipzSim<<" "<<ipzSimBool<<std::endl;
        //        std::cout<<"vx: "<<pointDY.x()<<" vy: "<<pointDY.y()<<" vz: "<<pointDY.z()<<" |Dxy|: "<<ipxySim<<" "<<ipxySimBool<<" |Dz|: "<<ipzSim<<" "<<ipzSimBool<<std::endl;
        
        if(useIPxy == true){
            
            if(vertices->size() !=0 && vtxCoord[0] > 0.5 && vtxCoord[0] < 1.5){
                
                double ipxy_value = fabs(muon->muonBestTrack()->dxy((*vertices)[indexFinal].position()));
                ipxy = ipxy_value < 0.2;
                //                std::cout<<"vx: "<<pointDY.x()<<" vy: "<<pointDY.y()<<" vz: "<<pointDY.z()<<" |Dxy|: "<<ipxy_value<<" Bool: "<<ipxy<<std::endl;
                
            }
            else if(vtxCoord[0] > 1.5 && vtxCoord[0] < 3.5){
                
                ipxy = ipxySimBool;
                //std::cout<<"vx: "<<point.x()<<" vy: "<<point.y()<<" vz: "<<point.z()<<" |Dxy|: "<<ipxy<<std::endl;
                
            }
            
        }
        else if(useIPxy == false) ipxy = true;
        
        if(useIPz == true){
            
            if(vertices->size() !=0 && vtxCoord[0] > 0.5 && vtxCoord[0] < 1.5){
                
                double ipz_value = fabs(muon->muonBestTrack()->dz((*vertices)[indexFinal].position()));
                ipz = ipz_value < 0.5;
                //                std::cout<<"vx: "<<pointDY.x()<<" vy: "<<pointDY.y()<<" vz: "<<pointDY.z()<<" |Dz|: "<<ipz_value<<" Bool: "<<ipz<<std::endl;
                
            }
            else if(vtxCoord[0] > 1.5 && vtxCoord[0] < 3.5){
                
                ipz = ipzSimBool;
                //std::cout<<"vx: "<<point.x()<<" vy: "<<point.y()<<" vz: "<<point.z()<<" |Dz|: "<<ipz<<std::endl;
                
            }
            
        }
        else if(useIPz == false) ipz = true;
        
    }

    double mom = muon->p();
    double dPhiCut_ = std::min(std::max(1.2/mom,1.2/100),0.032);
    double dPhiBendCut_ = std::min(std::max(0.2/mom,0.2/100),0.0041);
    bool isME0 = isME0MuonSelNew(iEvent, iSetup, muon, 0.048, dPhiCut_, dPhiBendCut_) && ipxy && ipz && validPxlHit && highPurity;
    
    return isME0;
}

bool MuonTrackProducer::isTightModExtSim(edm::Event& iEvent, reco::MuonCollection::const_iterator muon)
{
    bool resultTight = false;
    bool result = false;
    
    if (muon->muonBestTrack().isNonnull() && muon->innerTrack().isNonnull() && muon->globalTrack().isNonnull()){
        
        std::vector<double> vtxCoord = findSimVtx(iEvent);
        GlobalPoint point(vtxCoord[1],vtxCoord[2],vtxCoord[3]);
        GlobalPoint pointDY(vtxCoord[4],vtxCoord[5],vtxCoord[6]);
        
        double ipxySim = 999;
        double ipzSim = 999;
        
        if(vtxCoord[0] > 1.5 && vtxCoord[0] < 3.5){//Mu and nu gun samples
            
            ipxySim = fabs(muon->muonBestTrack()->dxy(math::XYZPoint(point.x(),point.y(),point.z())));
            ipzSim = fabs(muon->muonBestTrack()->dz(math::XYZPoint(point.x(),point.y(),point.z())));
            
        }
        else if(vtxCoord[0] > 0.5 && vtxCoord[0] < 1.5){//DY samples
            
            ipxySim = fabs(muon->muonBestTrack()->dxy(math::XYZPoint(pointDY.x(),pointDY.y(),pointDY.z())));
            ipzSim = fabs(muon->muonBestTrack()->dz(math::XYZPoint(pointDY.x(),pointDY.y(),pointDY.z())));
            
        }
        bool ipxySimBool = ipxySim < 0.2;
        bool ipzSimBool = ipzSim < 0.5;
        //std::cout<<"vx: "<<point.x()<<" vy: "<<point.y()<<" vz: "<<point.z()<<" |Dxy|: "<<ipxySim<<" "<<ipxySimBool<<" |Dz|: "<<ipzSim<<" "<<ipzSimBool<<std::endl;
        //std::cout<<"vx: "<<pointDY.x()<<" vy: "<<pointDY.y()<<" vz: "<<pointDY.z()<<" |Dxy|: "<<ipxySim<<" "<<ipxySimBool<<" |Dz|: "<<ipzSim<<" "<<ipzSimBool<<std::endl;
        
        bool trkLayMeas = muon->innerTrack()->hitPattern().trackerLayersWithMeasurement() > 5;
        bool isGlb = muon->isGlobalMuon();
        bool isPF= isGlobalTightMuon(muon) || isTrackerTightMuon(muon) || isIsolatedMuon(muon);
        bool chi2 = muon->globalTrack()->normalizedChi2() < 10.;
        bool validHits = muon->globalTrack()->hitPattern().numberOfValidMuonHits() > 0;
        bool matchedSt = muon->numberOfMatchedStations() > 1;
        bool validPxlHit = muon->innerTrack()->hitPattern().numberOfValidPixelHits() > 0;
        //bool validPxlHit = muon->innerTrack()->hitPattern().pixelLayersWithMeasurement(3,2) > 0;
        //bool validPxlHit = muon->innerTrack()->hitPattern().pixelLayersWithMeasurement(4,3) > 0;
        
        //std::cout<<trkLayMeas<<" "<<isGlb<<" "<<isPF<<" "<<chi2<<" "<<validHits<<" "<<matchedSt<<" "<<ipxy<<" "<<ipz<<" "<<validPxlHit<<std::endl;
        
        if(trkLayMeas && isGlb && isPF && chi2 && validHits && matchedSt && ipxySimBool && ipzSimBool && validPxlHit) resultTight = true;
        
    }
    
    bool isME0 = isME0MuonSel(muon, 3, 4, 3, 4, 0.1);
    double eta = muon->eta();
    
    if(fabs(eta) > 0 && fabs(eta) < 2.4) result = resultTight;
    else if(fabs(eta) > 2.4) result = isME0;
    
    return result;
}


MuonTrackProducer::MuonTrackProducer(const edm::ParameterSet& parset) :
  muonsToken(consumes<reco::MuonCollection>(parset.getParameter< edm::InputTag >("muonsTag"))),
  vtx_Token(consumes<reco::VertexCollection>(parset.getParameter< edm::InputTag >("vtxTag"))),
  genP_Token(consumes<reco::GenParticleCollection>(edm::InputTag("genParticles"))),
  bs_Token(consumes<reco::BeamSpot>(edm::InputTag("offlineBeamSpot"))),
  useIPxy(parset.getUntrackedParameter< bool >("useIPxy", true)),
  useIPz(parset.getUntrackedParameter< bool >("useIPz", true)),
  inputDTRecSegment4DToken_(consumes<DTRecSegment4DCollection>(parset.getParameter<edm::InputTag>("inputDTRecSegment4DCollection"))),
  inputCSCSegmentToken_(consumes<CSCSegmentCollection>(parset.getParameter<edm::InputTag>("inputCSCSegmentCollection"))),
  selectionTags(parset.getParameter< std::vector<std::string> >("selectionTags")),
  trackType(parset.getParameter< std::string >("trackType")),
  parset_(parset)
{
  edm::LogVerbatim("MuonTrackProducer") << "constructing  MuonTrackProducer" << parset_.dump();
  produces<reco::TrackCollection>();
  produces<reco::TrackExtraCollection>();
  produces<TrackingRecHitCollection>();
}

MuonTrackProducer::~MuonTrackProducer() {
}

void MuonTrackProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) 
{
  iEvent.getByToken(muonsToken,muonCollectionH);
  iEvent.getByToken(inputDTRecSegment4DToken_, dtSegmentCollectionH_);
  iEvent.getByToken(inputCSCSegmentToken_, cscSegmentCollectionH_);

  edm::ESHandle<TrackerTopology> httopo;
  iSetup.get<TrackerTopologyRcd>().get(httopo);
  const TrackerTopology& ttopo = *httopo;

  std::unique_ptr<reco::TrackCollection> selectedTracks(new reco::TrackCollection);
  std::unique_ptr<reco::TrackExtraCollection> selectedTrackExtras( new reco::TrackExtraCollection() );
  std::unique_ptr<TrackingRecHitCollection> selectedTrackHits( new TrackingRecHitCollection() );

  reco::TrackRefProd rTracks = iEvent.getRefBeforePut<reco::TrackCollection>();
  reco::TrackExtraRefProd rTrackExtras = iEvent.getRefBeforePut<reco::TrackExtraCollection>();
  TrackingRecHitRefProd rHits = iEvent.getRefBeforePut<TrackingRecHitCollection>();

  edm::Ref<reco::TrackExtraCollection>::key_type idx = 0;
  edm::Ref<reco::TrackExtraCollection>::key_type hidx = 0;

  edm::LogVerbatim("MuonTrackProducer") <<"\nThere are "<< dtSegmentCollectionH_->size()<<" DT segments.";
  unsigned int index_dt_segment = 0;
  for(DTRecSegment4DCollection::const_iterator segment = dtSegmentCollectionH_->begin();
      segment != dtSegmentCollectionH_->end(); ++segment , index_dt_segment++) {
    LocalPoint  segmentLocalPosition       = segment->localPosition();
    LocalVector segmentLocalDirection      = segment->localDirection();
    LocalError  segmentLocalPositionError  = segment->localPositionError();
    LocalError  segmentLocalDirectionError = segment->localDirectionError();
    DetId geoid = segment->geographicalId();
    DTChamberId dtdetid = DTChamberId(geoid);
    int wheel = dtdetid.wheel();
    int station = dtdetid.station();
    int sector = dtdetid.sector();
    
    float segmentX = segmentLocalPosition.x();
    float segmentY = segmentLocalPosition.y();
    float segmentdXdZ = segmentLocalDirection.x()/segmentLocalDirection.z();
    float segmentdYdZ = segmentLocalDirection.y()/segmentLocalDirection.z();
    float segmentXerr = sqrt(segmentLocalPositionError.xx());
    float segmentYerr = sqrt(segmentLocalPositionError.yy());
    float segmentdXdZerr = sqrt(segmentLocalDirectionError.xx());
    float segmentdYdZerr = sqrt(segmentLocalDirectionError.yy());

    edm::LogVerbatim("MuonTrackProducer") 
      <<"\nDT segment index :"<<index_dt_segment
      <<"\nchamber Wh:"<<wheel<<",St:"<<station<<",Se:"<<sector
      <<"\nLocal Position (X,Y)=("<<segmentX<<","<<segmentY<<") +/- ("<<segmentXerr<<","<<segmentYerr<<"), " 
      <<"Local Direction (dXdZ,dYdZ)=("<<segmentdXdZ<<","<<segmentdYdZ<<") +/- ("<<segmentdXdZerr<<","<<segmentdYdZerr<<")"; 
  }

  edm::LogVerbatim("MuonTrackProducer") <<"\nThere are "<< cscSegmentCollectionH_->size()<<" CSC segments.";
  unsigned int index_csc_segment = 0;
  for(CSCSegmentCollection::const_iterator segment = cscSegmentCollectionH_->begin();
      segment != cscSegmentCollectionH_->end(); ++segment , index_csc_segment++) {
    LocalPoint  segmentLocalPosition       = segment->localPosition();
    LocalVector segmentLocalDirection      = segment->localDirection();
    LocalError  segmentLocalPositionError  = segment->localPositionError();
    LocalError  segmentLocalDirectionError = segment->localDirectionError();

    DetId geoid = segment->geographicalId();
    CSCDetId cscdetid = CSCDetId(geoid);
    int endcap = cscdetid.endcap();
    int station = cscdetid.station();
    int ring = cscdetid.ring();
    int chamber = cscdetid.chamber(); 
    
    float segmentX = segmentLocalPosition.x();
    float segmentY = segmentLocalPosition.y();
    float segmentdXdZ = segmentLocalDirection.x()/segmentLocalDirection.z();
    float segmentdYdZ = segmentLocalDirection.y()/segmentLocalDirection.z();
    float segmentXerr = sqrt(segmentLocalPositionError.xx());
    float segmentYerr = sqrt(segmentLocalPositionError.yy());
    float segmentdXdZerr = sqrt(segmentLocalDirectionError.xx());
    float segmentdYdZerr = sqrt(segmentLocalDirectionError.yy());

    edm::LogVerbatim("MuonTrackProducer") 
      <<"\nCSC segment index :"<<index_csc_segment
      <<"\nchamber Endcap:"<<endcap<<",St:"<<station<<",Ri:"<<ring<<",Ch:"<<chamber
      <<"\nLocal Position (X,Y)=("<<segmentX<<","<<segmentY<<") +/- ("<<segmentXerr<<","<<segmentYerr<<"), " 
      <<"Local Direction (dXdZ,dYdZ)=("<<segmentdXdZ<<","<<segmentdYdZ<<") +/- ("<<segmentdXdZerr<<","<<segmentdYdZerr<<")"; 
  }

  edm::LogVerbatim("MuonTrackProducer") <<"\nThere are "<< muonCollectionH->size() <<" reco::Muons.";
  unsigned int muon_index = 0;
  for(reco::MuonCollection::const_iterator muon = muonCollectionH->begin();
       muon != muonCollectionH->end(); ++muon, muon_index++) {
    edm::LogVerbatim("MuonTrackProducer") <<"\n******* muon index : "<<muon_index;
    
    std::vector<bool> isGood;    
    for(unsigned int index=0; index<selectionTags.size(); ++index) {
      isGood.push_back(false);

      muon::SelectionType muonType = muon::selectionTypeFromString(selectionTags[index]);
      isGood[index] = muon::isGoodMuon(*muon, muonType);
    }

    bool isGoodResult=true;
    for(unsigned int index=0; index<isGood.size(); ++index) {
      edm::LogVerbatim("MuonTrackProducer") << "selectionTag = "<<selectionTags[index]<< ": "<<isGood[index]<<"\n";
      isGoodResult *= isGood[index];
    }

    if (isGoodResult) {
      // new copy of Track
      reco::TrackRef trackref;

      bool loose = isLoose(iEvent, muon);
      bool tightClassic = isTightClassic(iEvent, muon, useIPxy, useIPz);
      bool tight = isTight(iEvent, muon, useIPxy, useIPz);
      bool looseMod = isLooseMod(iEvent, muon);
      bool tightMod = isTightMod(iEvent, muon, useIPxy, useIPz);
      std::vector<bool> looseModExt = isLooseModExt(iEvent, iSetup, muon);
      std::vector<bool> tightModExt = isTightModExt(iEvent, iSetup, muon, useIPxy, useIPz);
      bool me0MuonLoose = isME0MuonLoose(iEvent, iSetup, muon);
      bool me0MuonTight = isME0MuonTight(iEvent, iSetup, muon, useIPxy, useIPz);
      bool tightModExtSim = isTightModExtSim(iEvent, muon);
      bool usingInner = false;
        
      if (trackType == "innerTrack") {
        if (muon->innerTrack().isNonnull()) trackref = muon->innerTrack();
        else continue;
      } 
      else if (trackType == "outerTrack") {
        if (muon->outerTrack().isNonnull()) trackref = muon->outerTrack();
        else continue;
      } 
      else if (trackType == "globalTrack") {
        if (muon->globalTrack().isNonnull()) trackref = muon->globalTrack();
        else continue;
      }
      else if (trackType == "bestMuon") {
        if (muon->muonBestTrack().isNonnull()) trackref = muon->muonBestTrack();
      else continue;
      }
      else if (trackType == "globalTrackLoose") {

          if(loose){
              
          	if( muon->globalTrack().isNonnull() )
                {
                   	trackref = muon->globalTrack();
                    usingInner = false;
                }
          	else if ( muon->innerTrack().isNonnull() )
                {
                   	trackref = muon->innerTrack();
                   	usingInner = true;
                }
          	else continue;

          }

        //if (muon->muonBestTrack().isNonnull() && loose) trackref = muon->muonBestTrack();
        else continue;

      }
      else if (trackType == "globalTrackLooseMod") {

          if(looseMod){

          	if( muon->globalTrack().isNonnull() )
                {
                   	trackref = muon->globalTrack();
                    usingInner = false;
                }
          	else if ( muon->innerTrack().isNonnull() )
                {
                   	trackref = muon->innerTrack();
                   	usingInner = true;
                }
          	else continue;

          }

          //if (muon->muonBestTrack().isNonnull() && loose) trackref = muon->muonBestTrack();
          else continue;

      }
      else if (trackType == "globalTrackLooseModExt") {

          if(looseModExt[1]){

              if ( muon->innerTrack().isNonnull() && me0MuonLoose ) trackref = muon->innerTrack();
              else continue;
          
          }
          else if(looseModExt[0]){
          
              if( muon->globalTrack().isNonnull() )
              {
                  trackref = muon->globalTrack();
                  usingInner = false;
              }
              else if ( muon->innerTrack().isNonnull() )
              {
                  trackref = muon->innerTrack();
                  usingInner = true;
              }
              else continue;
              
           }

           else continue;
        
      }
      else if (trackType == "me0MuonLoose") {

           if ( muon->innerTrack().isNonnull() && me0MuonLoose ) trackref = muon->innerTrack();
           else continue;
        
      }
      else if (trackType == "globalTrackTight") {
        if (muon->globalTrack().isNonnull() && tight) trackref = muon->globalTrack();
        else continue;
      }
      else if (trackType == "globalTrackTightClassic") {
        if (muon->globalTrack().isNonnull() && tightClassic) trackref = muon->globalTrack();
        else continue;
      }
      else if (trackType == "globalTrackTightMod") {
        if (muon->globalTrack().isNonnull() && tightMod) trackref = muon->globalTrack();
        else continue;
      }
      else if (trackType == "globalTrackTightModExt") {
          
          if(tightModExt[1] == true && muon->innerTrack().isNonnull()) trackref = muon->innerTrack();
          else if (tightModExt[0] == true && muon->globalTrack().isNonnull()) trackref = muon->globalTrack();
          else continue;
          
      }
      else if (trackType == "me0MuonTight") {
        if (muon->innerTrack().isNonnull() && me0MuonTight) trackref = muon->innerTrack();
        else continue;
      }
      else if (trackType == "globalTrackTightModExtSim") {
          if (fabs(muon->eta()) < 2.4 && muon->globalTrack().isNonnull() && tightModExtSim) trackref = muon->globalTrack();
          else if(fabs(muon->eta()) > 2.4 && muon->innerTrack().isNonnull() && tightModExtSim) trackref = muon->innerTrack();
          else continue;
      }
      else if (trackType == "innerTrackPlusSegments") {
        if (muon->innerTrack().isNonnull()) trackref = muon->innerTrack();
        else continue;
      }
      else if (trackType == "gemMuonTrack") {
        if (muon->innerTrack().isNonnull() && muon->isGEMMuon()){
            trackref = muon->innerTrack();
        }
        else continue;
      }
      else if (trackType == "me0MuonTrack") {
        if (muon->innerTrack().isNonnull() && muon->isME0Muon()){
            trackref = muon->innerTrack();
        }
        else continue;
      }

      const reco::Track* trk = &(*trackref);
      // pointer to old track:
      reco::Track* newTrk = new reco::Track(*trk);

      newTrk->setExtra( reco::TrackExtraRef( rTrackExtras, idx++ ) );
      PropagationDirection seedDir = trk->seedDirection();
      // new copy of track Extras
      reco::TrackExtra * newExtra = new reco::TrackExtra( trk->outerPosition(), trk->outerMomentum(), 
                                        trk->outerOk(), trk->innerPosition(), 
                                        trk->innerMomentum(), trk->innerOk(),
                                        trk->outerStateCovariance(), trk->outerDetId(),
                                        trk->innerStateCovariance(), trk->innerDetId() , seedDir ) ;

      // new copy of the silicon hits; add hit refs to Extra and hits to hit collection
      
      //      edm::LogVerbatim("MuonTrackProducer")<<"\n printing initial hit_pattern";
      //      trk->hitPattern().print();
      unsigned int nHitsToAdd = 0;
      for (trackingRecHit_iterator iHit = trk->recHitsBegin(); iHit != trk->recHitsEnd(); iHit++) {
        TrackingRecHit* hit = (*iHit)->clone();
        selectedTrackHits->push_back( hit );
        ++nHitsToAdd;
      }
      newExtra->setHits( rHits, hidx, nHitsToAdd );
      hidx += nHitsToAdd;
        
    if (trackType == "innerTrackPlusSegments" || usingInner) {
     
	int wheel, station, sector;
	int endcap, /*station, */ ring, chamber;
	
	edm::LogVerbatim("MuonTrackProducer") <<"Number of chambers: "<<muon->matches().size()
					      <<", arbitrated: "<<muon->numberOfMatches(reco::Muon::SegmentAndTrackArbitration);
	unsigned int index_chamber = 0;
	
	for(std::vector<reco::MuonChamberMatch>::const_iterator chamberMatch = muon->matches().begin();
	    chamberMatch != muon->matches().end(); ++chamberMatch, index_chamber++) {
	  std::stringstream chamberStr;
	  chamberStr <<"\nchamber index: "<<index_chamber; 
	  
	  int subdet = chamberMatch->detector();
	  DetId did = chamberMatch->id;
	  
	  if (subdet == MuonSubdetId::DT) {
	    DTChamberId dtdetid = DTChamberId(did);
	    wheel = dtdetid.wheel();
	    station = dtdetid.station();
	    sector = dtdetid.sector();
	    chamberStr << ", DT chamber Wh:"<<wheel<<",St:"<<station<<",Se:"<<sector;
	  } 
	  else if (subdet == MuonSubdetId::CSC) {
	    CSCDetId cscdetid = CSCDetId(did);
	    endcap = cscdetid.endcap();
	    station = cscdetid.station();
	    ring = cscdetid.ring();
	    chamber = cscdetid.chamber();
	    chamberStr << ", CSC chamber End:"<<endcap<<",St:"<<station<<",Ri:"<<ring<<",Ch:"<<chamber;
	  }
	  
	  chamberStr << ", Number of segments: "<<chamberMatch->segmentMatches.size();
	  edm::LogVerbatim("MuonTrackProducer") << chamberStr.str();

	  unsigned int index_segment = 0;
	  
	  for(std::vector<reco::MuonSegmentMatch>::const_iterator segmentMatch = chamberMatch->segmentMatches.begin();
	      segmentMatch != chamberMatch->segmentMatches.end(); ++segmentMatch, index_segment++) {
	    
	    float segmentX = segmentMatch->x;
	    float segmentY = segmentMatch->y ;
	    float segmentdXdZ = segmentMatch->dXdZ;
	    float segmentdYdZ = segmentMatch->dYdZ;
	    float segmentXerr = segmentMatch->xErr;
	    float segmentYerr = segmentMatch->yErr;
	    float segmentdXdZerr = segmentMatch->dXdZErr;
	    float segmentdYdZerr = segmentMatch->dYdZErr;
	    
	    CSCSegmentRef segmentCSC = segmentMatch->cscSegmentRef;
	    DTRecSegment4DRef segmentDT = segmentMatch->dtSegmentRef;
	    
	    bool segment_arbitrated_Ok = (segmentMatch->isMask(reco::MuonSegmentMatch::BestInChamberByDR) && 
					  segmentMatch->isMask(reco::MuonSegmentMatch::BelongsToTrackByDR));
	    
	    std::string ARBITRATED(" ***Arbitrated Off*** ");
	    if (segment_arbitrated_Ok) ARBITRATED = " ***ARBITRATED OK*** ";

	    if (subdet == MuonSubdetId::DT) {	      
	      edm::LogVerbatim("MuonTrackProducer")
		<<"\n\t segment index: "<<index_segment << ARBITRATED
		<<"\n\t  Local Position (X,Y)=("<<segmentX<<","<<segmentY<<") +/- ("<<segmentXerr<<","<<segmentYerr<<"), " 
		<<"\n\t  Local Direction (dXdZ,dYdZ)=("<<segmentdXdZ<<","<<segmentdYdZ<<") +/- ("<<segmentdXdZerr<<","<<segmentdYdZerr<<")"; 
	      
	      if (!segment_arbitrated_Ok) continue;
	      
	      if (segmentDT.get() != 0) {
		const DTRecSegment4D* segment = segmentDT.get();
		
		edm::LogVerbatim("MuonTrackProducer")<<"\t ===> MATCHING with DT segment with index = "<<segmentDT.key();
		
		if(segment->hasPhi()) {
		  const DTChamberRecSegment2D* phiSeg = segment->phiSegment();
		  std::vector<const TrackingRecHit*> phiHits = phiSeg->recHits();
                  unsigned int nHitsAdded = 0;
		  for(std::vector<const TrackingRecHit*>::const_iterator ihit = phiHits.begin();
		      ihit != phiHits.end(); ++ihit) {
		    TrackingRecHit* seghit = (*ihit)->clone();
		    newTrk->appendHitPattern(*seghit, ttopo);
		    //		    edm::LogVerbatim("MuonTrackProducer")<<"hit pattern for position "<<index_hit<<" set to:";
		    //		    newTrk->hitPattern().printHitPattern(index_hit, std::cout);
		    selectedTrackHits->push_back( seghit );
                    ++nHitsAdded;
		  }
                  newExtra->setHits( rHits, hidx, nHitsAdded );
                  hidx += nHitsAdded;
		}
		
		if(segment->hasZed()) {
		  const DTSLRecSegment2D* zSeg = (*segment).zSegment();
		  std::vector<const TrackingRecHit*> zedHits = zSeg->recHits();
                  unsigned int nHitsAdded = 0;
		  for(std::vector<const TrackingRecHit*>::const_iterator ihit = zedHits.begin();
		      ihit != zedHits.end(); ++ihit) {
		    TrackingRecHit* seghit = (*ihit)->clone();
		    newTrk->appendHitPattern(*seghit, ttopo);
		    //		    edm::LogVerbatim("MuonTrackProducer")<<"hit pattern for position "<<index_hit<<" set to:";
		    //		    newTrk->hitPattern().printHitPattern(index_hit, std::cout);
		    selectedTrackHits->push_back( seghit );
                    ++nHitsAdded;
		  }
                  newExtra->setHits( rHits, hidx, nHitsAdded );
                  hidx += nHitsAdded;
		}
	      } else edm::LogWarning("MuonTrackProducer")<<"\n***WARNING: UNMATCHED DT segment ! \n";
	    } // if (subdet == MuonSubdetId::DT)

	    else if (subdet == MuonSubdetId::CSC) {
	      edm::LogVerbatim("MuonTrackProducer")
		<<"\n\t segment index: "<<index_segment << ARBITRATED
		<<"\n\t  Local Position (X,Y)=("<<segmentX<<","<<segmentY<<") +/- ("<<segmentXerr<<","<<segmentYerr<<"), " 
		<<"\n\t  Local Direction (dXdZ,dYdZ)=("<<segmentdXdZ<<","<<segmentdYdZ<<") +/- ("<<segmentdXdZerr<<","<<segmentdYdZerr<<")"; 
	      
	      if (!segment_arbitrated_Ok) continue;
	      
	      if (segmentCSC.get() != 0) {
		const CSCSegment* segment = segmentCSC.get();
		
		edm::LogVerbatim("MuonTrackProducer")<<"\t ===> MATCHING with CSC segment with index = "<<segmentCSC.key();
		
		std::vector<const TrackingRecHit*> hits = segment->recHits();
                unsigned int nHitsAdded = 0;
		for(std::vector<const TrackingRecHit*>::const_iterator ihit = hits.begin();
		    ihit != hits.end(); ++ihit) {
		  TrackingRecHit* seghit = (*ihit)->clone();
		  newTrk->appendHitPattern(*seghit, ttopo);
		  //		    edm::LogVerbatim("MuonTrackProducer")<<"hit pattern for position "<<index_hit<<" set to:";
		  //		    newTrk->hitPattern().printHitPattern(index_hit, std::cout);
		  selectedTrackHits->push_back( seghit );
                  ++nHitsAdded;
		}
                newExtra->setHits( rHits, hidx, nHitsAdded );
                hidx += nHitsAdded;
	      } else edm::LogWarning("MuonTrackProducer")<<"\n***WARNING: UNMATCHED CSC segment ! \n";
	    }  //  else if (subdet == MuonSubdetId::CSC)

	  } // loop on vector<MuonSegmentMatch>	  
	} // loop on vector<MuonChamberMatch>	
      } // if (trackType == "innerTrackPlusSegments")
      
      //      edm::LogVerbatim("MuonTrackProducer")<<"\n printing final hit_pattern";
      //      newTrk->hitPattern().print();
      
      selectedTracks->push_back( *newTrk );
      selectedTrackExtras->push_back( *newExtra );

    } // if (isGoodResult)
  }  // loop on reco::MuonCollection
    
  //std::cout<<selectedTracks->size()<<std::endl;
  
  iEvent.put(std::move(selectedTracks));
  iEvent.put(std::move(selectedTrackExtras));
  iEvent.put(std::move(selectedTrackHits));

}
