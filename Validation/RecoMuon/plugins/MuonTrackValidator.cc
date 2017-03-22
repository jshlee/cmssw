#include "Validation/RecoMuon/plugins/MuonTrackValidator.h"
#include "DQMServices/ClientConfig/interface/FitSlicesYTool.h"

#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"
#include "SimDataFormats/Vertex/interface/SimVertexContainer.h"
#include "SimTracker/TrackerHitAssociation/interface/TrackerHitAssociator.h"
#include "SimTracker/Records/interface/TrackAssociatorRecord.h"
#include "TrackingTools/TrajectoryState/interface/FreeTrajectoryState.h"
#include "TrackingTools/PatternTools/interface/TSCBLBuilderNoMaterial.h"
#include "SimTracker/TrackAssociation/plugins/ParametersDefinerForTPESProducer.h"
#include "SimTracker/TrackAssociation/plugins/CosmicParametersDefinerForTPESProducer.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include <DataFormats/MuonDetId/interface/CSCDetId.h>
#include <DataFormats/MuonDetId/interface/GEMDetId.h>
#include <DataFormats/MuonDetId/interface/RPCDetId.h>

#include "TMath.h"
#include <TF1.h>

using namespace std;
using namespace edm;

bool MuonTrackValidator::isSignalFromZgamma(TrackingParticle* tpRtS, bool debug = false){

	bool isSignalMuon = abs(tpRtS->pdgId())==13 && !tpRtS->genParticles().empty() && (tpRtS->eventId().event() == 0) && (tpRtS->eventId().bunchCrossing() == 0); //segnale muone

	bool isFromZgamma = false;
	if (isSignalMuon){

		int hitsPdgId = tpRtS->pdgId();
		int hitsStatus = tpRtS->status();
		double prodRho = tpRtS->vertex().Rho();
		double prodZ = tpRtS->vz();
		if(debug) cout << "\t Particle pdgId = "<< hitsPdgId << " status:" << hitsStatus << " produced at rho = " << prodRho << ", z = " << prodZ <<endl;

		reco::GenParticleRef genp   = tpRtS->genParticles()[0];
		reco::GenParticleRef genMom = genp->numberOfMothers() > 0 ? genp->motherRef() : reco::GenParticleRef();
		if(debug) cout << "\t Number of mothers = "<< genp->numberOfMothers() << std::endl;
		if (genMom.isNonnull()) {

			int momPdgId  = genMom->pdgId();
			int momStatus = genMom->status();
			if( (momPdgId == 23 || momPdgId == 3000022 ) && momStatus == 3) isFromZgamma = true;
			double momRho = genMom->vertex().Rho();
			double momZ = genMom->vz();
			if(debug) cout << "\t Particle pdgId = "<<hitsPdgId << ", has GEN mother pdgId = " << momPdgId << ", mom status: " << momStatus <<endl;
			reco::GenParticleRef genGMom = genMom->numberOfMothers() > 0 ? genMom->motherRef() : reco::GenParticleRef();
			if(debug) cout << "\t\t Number of gmoms = " << genMom->numberOfMothers() << std::endl;
			if (genGMom.isNonnull()) {

				int gmomPdgId = genGMom->pdgId();
				int gmomStatus = genGMom->status();
				if( (gmomPdgId == 23 || gmomPdgId == 3000022) && gmomStatus == 3) isFromZgamma = true;
				if(debug) cout << "\t\t mother prod. vertex rho = " << momRho << ", z = " << momZ << ", grand-mom pdgId = " << gmomPdgId << ", status: " << gmomStatus << " mass = " <<genGMom->mass()<<endl;

				reco::GenParticleRef genGGMom = genGMom->numberOfMothers() > 0 ? genGMom->motherRef() : reco::GenParticleRef();
				if(debug) cout << "\t\t Number of ggmoms = " << genGMom->numberOfMothers() << std::endl;
				if (genGGMom.isNonnull()) {

					int ggmomPdgId = genGGMom->pdgId();
					int ggmomStatus = genGGMom->status();
					if( (ggmomPdgId == 23 || ggmomPdgId == 3000022) && ggmomStatus == 3) isFromZgamma = true;
					if(debug) cout << "\t\t grand-grand-mom pdgId = " << ggmomPdgId << ", status: " << ggmomStatus <<endl;

					reco::GenParticleRef genGGGMom = genGGMom->numberOfMothers() > 0 ? genGGMom->motherRef() : reco::GenParticleRef();
					if(debug) cout << "\t\t Number of gggmoms = " << genGGMom->numberOfMothers() << std::endl;
					if (genGGGMom.isNonnull()) {

						int gggmomPdgId = genGGGMom->pdgId();
						int gggmomStatus = genGGGMom->status();
						if( (gggmomPdgId == 23 || gggmomPdgId == 3000022) && gggmomStatus == 3) isFromZgamma = true;
						if(debug) cout << "\t\t grand-grand-grand-mom pdgId = " << gggmomPdgId << ", status: " << gggmomStatus <<endl;

                    			}

                    		}

                    	}

                }

	}
    
    //cout << "Result: " << isFromZgamma << endl;
	return isFromZgamma;

}

void MuonTrackValidator::bookHistograms(DQMStore::IBooker& ibooker, edm::Run const&, edm::EventSetup const& setup) {

  int j=0;
  for (unsigned int ww=0;ww<associators.size();ww++){
    for (unsigned int www=0;www<label.size();www++){

      ibooker.cd();
      InputTag algo = label[www];
      string dirName=dirName_;
      if (algo.process()!="")
        dirName+=algo.process()+"_";
      if(algo.label()!="")
        dirName+=algo.label()+"_";
      if(algo.instance()!="")
        dirName+=algo.instance()+"_";
      if (dirName.find("Tracks")<dirName.length()){
        dirName.replace(dirName.find("Tracks"),6,"");
      }
      string assoc= associators[ww];
      if (assoc.find("Track")<assoc.length()){
        assoc.replace(assoc.find("Track"),5,"");
      }
      dirName+=assoc;
      std::replace(dirName.begin(), dirName.end(), ':', '_');
      ibooker.setCurrentFolder(dirName.c_str());

      setUpVectors();

      ibooker.goUp();
      string subDirName = dirName + "/simulation";
      ibooker.setCurrentFolder(subDirName.c_str());
      h_ptSIM.push_back( ibooker.book1D("ptSIM", "generated p_{t}", 5500, 0, 110 ) );
      h_etaSIM.push_back( ibooker.book1D("etaSIM", "generated pseudorapidity", 500, -2.5, 2.5 ) );
      h_tracksSIM.push_back( ibooker.book1D("tracksSIM","number of simulated tracks",200,-0.5,99.5) );
      h_vertposSIM.push_back( ibooker.book1D("vertposSIM","Transverse position of sim vertices",100,0.,120.) );
      
      ibooker.cd();
      ibooker.setCurrentFolder(dirName.c_str());
      h_tracks.push_back( ibooker.book1D("tracks","number of reconstructed tracks",200,-0.5,19.5) );
      h_fakes.push_back( ibooker.book1D("fakes","number of fake reco tracks",20,-0.5,19.5) );
      h_charge.push_back( ibooker.book1D("charge","charge",3,-1.5,1.5) );
      h_hits.push_back( ibooker.book1D("hits", "number of hits per track", nintHit,minHit,maxHit ) );
      h_losthits.push_back( ibooker.book1D("losthits", "number of lost hits per track", nintHit,minHit,maxHit) );
      h_nchi2.push_back( ibooker.book1D("chi2", "normalized #chi^{2}", 200, 0, 20 ) );
      h_nchi2_prob.push_back( ibooker.book1D("chi2_prob", "normalized #chi^{2} probability",100,0,1));

      /// this are needed to calculate efficiency during tha harvesting for the automated validation
      h_recoeta.push_back( ibooker.book1D("num_reco_eta","N of reco track vs eta",nint,min,max) );
      h_assoceta.push_back( ibooker.book1D("num_assoc(simToReco)_eta","N of associated tracks (simToReco) vs eta",nint,min,max) );
      h_assoc2eta.push_back( ibooker.book1D("num_assoc(recoToSim)_eta","N of associated (recoToSim) tracks vs eta",nint,min,max) );
      h_simuleta.push_back( ibooker.book1D("num_simul_eta","N of simulated tracks vs eta",nint,min,max) );
      h_recopT.push_back( ibooker.book1D("num_reco_pT","N of reco track vs pT",nintpT,minpT,maxpT) );
      h_assocpT.push_back( ibooker.book1D("num_assoc(simToReco)_pT","N of associated tracks (simToReco) vs pT",nintpT,minpT,maxpT) );
      h_assoc2pT.push_back( ibooker.book1D("num_assoc(recoToSim)_pT","N of associated (recoToSim) tracks vs pT",nintpT,minpT,maxpT) );
      h_simulpT.push_back( ibooker.book1D("num_simul_pT","N of simulated tracks vs pT",nintpT,minpT,maxpT) );
      //
      h_recohit.push_back( ibooker.book1D("num_reco_hit","N of reco track vs hit",nintHit,minHit,maxHit) );
      h_assochit.push_back( ibooker.book1D("num_assoc(simToReco)_hit","N of associated tracks (simToReco) vs hit",nintHit,minHit,maxHit) );
      h_assoc2hit.push_back( ibooker.book1D("num_assoc(recoToSim)_hit","N of associated (recoToSim) tracks vs hit",nintHit,minHit,maxHit) );
      h_simulhit.push_back( ibooker.book1D("num_simul_hit","N of simulated tracks vs hit",nintHit,minHit,maxHit) );
      //
      h_recophi.push_back( ibooker.book1D("num_reco_phi","N of reco track vs phi",nintPhi,minPhi,maxPhi) );
      h_assocphi.push_back( ibooker.book1D("num_assoc(simToReco)_phi","N of associated tracks (simToReco) vs phi",nintPhi,minPhi,maxPhi) );
      h_assoc2phi.push_back( ibooker.book1D("num_assoc(recoToSim)_phi","N of associated (recoToSim) tracks vs phi",nintPhi,minPhi,maxPhi) );
      h_simulphi.push_back( ibooker.book1D("num_simul_phi","N of simulated tracks vs phi",nintPhi,minPhi,maxPhi) );

      h_recodxy.push_back( ibooker.book1D("num_reco_dxy","N of reco track vs dxy",nintDxy,minDxy,maxDxy) );
      h_assocdxy.push_back( ibooker.book1D("num_assoc(simToReco)_dxy","N of associated tracks (simToReco) vs dxy",nintDxy,minDxy,maxDxy) );
      h_assoc2dxy.push_back( ibooker.book1D("num_assoc(recoToSim)_dxy","N of associated (recoToSim) tracks vs dxy",nintDxy,minDxy,maxDxy) );
      h_simuldxy.push_back( ibooker.book1D("num_simul_dxy","N of simulated tracks vs dxy",nintDxy,minDxy,maxDxy) );
      
      h_recodz.push_back( ibooker.book1D("num_reco_dz","N of reco track vs dz",nintDz,minDz,maxDz) );
      h_assocdz.push_back( ibooker.book1D("num_assoc(simToReco)_dz","N of associated tracks (simToReco) vs dz",nintDz,minDz,maxDz) );
      h_assoc2dz.push_back( ibooker.book1D("num_assoc(recoToSim)_dz","N of associated (recoToSim) tracks vs dz",nintDz,minDz,maxDz) );
      h_simuldz.push_back( ibooker.book1D("num_simul_dz","N of simulated tracks vs dz",nintDz,minDz,maxDz) );

      h_assocvertpos.push_back( ibooker.book1D("num_assoc(simToReco)_vertpos","N of associated tracks (simToReco) vs transverse vert position",nintVertpos,minVertpos,maxVertpos) );
      h_simulvertpos.push_back( ibooker.book1D("num_simul_vertpos","N of simulated tracks vs transverse vert position",nintVertpos,minVertpos,maxVertpos) );

      h_assoczpos.push_back( ibooker.book1D("num_assoc(simToReco)_zpos","N of associated tracks (simToReco) vs z vert position",nintZpos,minZpos,maxZpos) );
      h_simulzpos.push_back( ibooker.book1D("num_simul_zpos","N of simulated tracks vs z vert position",nintZpos,minZpos,maxZpos) );

      /////////////////////////////////

      h_eta.push_back( ibooker.book1D("eta", "pseudorapidity residue", 1000, -0.1, 0.1 ) );
      h_pt.push_back( ibooker.book1D("pullPt", "pull of p_{t}", 100, -10, 10 ) );
      h_pullTheta.push_back( ibooker.book1D("pullTheta","pull of #theta parameter",250,-25,25) );
      h_pullPhi.push_back( ibooker.book1D("pullPhi","pull of #phi parameter",250,-25,25) );
      h_pullDxy.push_back( ibooker.book1D("pullDxy","pull of dxy parameter",250,-25,25) );
      h_pullDz.push_back( ibooker.book1D("pullDz","pull of dz parameter",250,-25,25) );
      h_pullQoverp.push_back( ibooker.book1D("pullQoverp","pull of qoverp parameter",250,-25,25) );
      
      if (associators[ww]=="trackAssociatorByChi2"){
	h_assochi2.push_back( ibooker.book1D("assocChi2","track association #chi^{2}",1000000,0,100000) );
	h_assochi2_prob.push_back(ibooker.book1D("assocChi2_prob","probability of association #chi^{2}",100,0,1));
      } else if (associators[ww]=="trackAssociatorByHits"){
	h_assocFraction.push_back( ibooker.book1D("assocFraction","fraction of shared hits",200,0,2) );
	h_assocSharedHit.push_back(ibooker.book1D("assocSharedHit","number of shared hits",20,0,20));
      }
      
      chi2_vs_nhits.push_back( ibooker.book2D("chi2_vs_nhits","#chi^{2} vs nhits",25,0,25,100,0,10) );
      h_chi2meanhitsh.push_back( ibooker.bookProfile("chi2mean_vs_nhits","mean #chi^{2} vs nhits",25,0,25,100,0,10) );

      etares_vs_eta.push_back( ibooker.book2D("etares_vs_eta","etaresidue vs eta",nint,min,max,200,-0.1,0.1) );
      nrec_vs_nsim.push_back( ibooker.book2D("nrec_vs_nsim","nrec vs nsim",20,-0.5,19.5,20,-0.5,19.5) );

      chi2_vs_eta.push_back( ibooker.book2D("chi2_vs_eta","chi2_vs_eta",nint,min,max, 200, 0, 20 ));
      h_chi2meanh.push_back( ibooker.bookProfile("chi2mean","mean #chi^{2} vs #eta",nint,min,max, 200, 0, 20) );
      chi2_vs_phi.push_back( ibooker.book2D("chi2_vs_phi","#chi^{2} vs #phi",nintPhi,minPhi,maxPhi, 200, 0, 20 ) );
      h_chi2mean_vs_phi.push_back( ibooker.bookProfile("chi2mean_vs_phi","mean of #chi^{2} vs #phi",nintPhi,minPhi,maxPhi, 200, 0, 20) );

      nhits_vs_eta.push_back( ibooker.book2D("nhits_vs_eta","nhits vs eta",nint,min,max,nintHit,minHit,maxHit) );
      nDThits_vs_eta.push_back( ibooker.book2D("nDThits_vs_eta","# DT hits vs eta",nint,min,max,nintHit,minHit,maxHit) );
      nCSChits_vs_eta.push_back( ibooker.book2D("nCSChits_vs_eta","# CSC hits vs eta",nint,min,max,nintHit,minHit,maxHit) );
      nRPChits_vs_eta.push_back( ibooker.book2D("nRPChits_vs_eta","# RPC hits vs eta",nint,min,max,nintHit,minHit,maxHit) );
      if(useGEMs_) nGEMhits_vs_eta.push_back( ibooker.book2D("nGEMhits_vs_eta","# GEM hits vs eta",nint,min,max,nintHit,minHit,maxHit) );
      if(useME0_) nME0hits_vs_eta.push_back( ibooker.book2D("nME0hits_vs_eta","# ME0 hits vs eta",nint,min,max,nintHit,minHit,maxHit) );

      h_DThits_eta.push_back( ibooker.bookProfile("DThits_eta","mean # DT hits vs eta",nint,min,max,nintHit,minHit,maxHit) );
      h_CSChits_eta.push_back( ibooker.bookProfile("CSChits_eta","mean # CSC hits vs eta",nint,min,max,nintHit,minHit,maxHit) );
      h_RPChits_eta.push_back( ibooker.bookProfile("RPChits_eta","mean # RPC hits vs eta",nint,min,max,nintHit,minHit,maxHit) );
      if(useGEMs_) h_GEMhits_eta.push_back( ibooker.bookProfile("GEMhits_eta","mean # GEM hits vs eta",nint,min,max,nintHit,minHit,maxHit) );
      if(useME0_) h_ME0hits_eta.push_back( ibooker.bookProfile("ME0hits_eta","mean # ME0 hits vs eta",nint,min,max,nintHit,minHit,maxHit) );

      h_hits_eta.push_back( ibooker.bookProfile("hits_eta","mean #hits vs eta",nint,min,max,nintHit,minHit,maxHit) );
      nhits_vs_phi.push_back( ibooker.book2D("nhits_vs_phi","#hits vs #phi",nintPhi,minPhi,maxPhi,nintHit,minHit,maxHit) );
      h_hits_phi.push_back( ibooker.bookProfile("hits_phi","mean #hits vs #phi",nintPhi,minPhi,maxPhi, nintHit,minHit,maxHit) );

      nlosthits_vs_eta.push_back( ibooker.book2D("nlosthits_vs_eta","nlosthits vs eta",nint,min,max,nintHit,minHit,maxHit) );
      h_losthits_eta.push_back( ibooker.bookProfile("losthits_eta","losthits_eta",nint,min,max,nintHit,minHit,maxHit) );

      ptres_vs_eta.push_back(ibooker.book2D("ptres_vs_eta","ptres_vs_eta",nint,min,max, ptRes_nbin, ptRes_rangeMin, ptRes_rangeMax));
      ptres_vs_phi.push_back( ibooker.book2D("ptres_vs_phi","p_{t} res vs #phi",nintPhi,minPhi,maxPhi, ptRes_nbin, ptRes_rangeMin, ptRes_rangeMax));
      ptres_vs_pt.push_back(ibooker.book2D("ptres_vs_pt","ptres_vs_pt",nintpT,minpT,maxpT, ptRes_nbin, ptRes_rangeMin, ptRes_rangeMax));

      cotThetares_vs_eta.push_back(ibooker.book2D("cotThetares_vs_eta","cotThetares_vs_eta",nint,min,max,cotThetaRes_nbin, cotThetaRes_rangeMin, cotThetaRes_rangeMax));
      cotThetares_vs_pt.push_back(ibooker.book2D("cotThetares_vs_pt","cotThetares_vs_pt",nintpT,minpT,maxpT, cotThetaRes_nbin, cotThetaRes_rangeMin, cotThetaRes_rangeMax));

      phires_vs_eta.push_back(ibooker.book2D("phires_vs_eta","phires_vs_eta",nint,min,max, phiRes_nbin, phiRes_rangeMin, phiRes_rangeMax));
      phires_vs_pt.push_back(ibooker.book2D("phires_vs_pt","phires_vs_pt",nintpT,minpT,maxpT, phiRes_nbin, phiRes_rangeMin, phiRes_rangeMax));
      phires_vs_phi.push_back(ibooker.book2D("phires_vs_phi","#phi res vs #phi",nintPhi,minPhi,maxPhi,phiRes_nbin, phiRes_rangeMin, phiRes_rangeMax));

      dxyres_vs_eta.push_back(ibooker.book2D("dxyres_vs_eta","dxyres_vs_eta",nint,min,max,dxyRes_nbin, dxyRes_rangeMin, dxyRes_rangeMax));
      dxyres_vs_pt.push_back( ibooker.book2D("dxyres_vs_pt","dxyres_vs_pt",nintpT,minpT,maxpT,dxyRes_nbin, dxyRes_rangeMin, dxyRes_rangeMax));

      dzres_vs_eta.push_back(ibooker.book2D("dzres_vs_eta","dzres_vs_eta",nint,min,max,dzRes_nbin, dzRes_rangeMin, dzRes_rangeMax));
      dzres_vs_pt.push_back(ibooker.book2D("dzres_vs_pt","dzres_vs_pt",nintpT,minpT,maxpT,dzRes_nbin, dzRes_rangeMin, dzRes_rangeMax));

      ptmean_vs_eta_phi.push_back(ibooker.bookProfile2D("ptmean_vs_eta_phi","mean p_{t} vs #eta and #phi",nintPhi,minPhi,maxPhi,nint,min,max,1000,0,1000));
      phimean_vs_eta_phi.push_back(ibooker.bookProfile2D("phimean_vs_eta_phi","mean #phi vs #eta and #phi",nintPhi,minPhi,maxPhi,nint,min,max,nintPhi,minPhi,maxPhi));

      //pulls of track params vs eta: to be used with fitslicesytool
      dxypull_vs_eta.push_back(ibooker.book2D("dxypull_vs_eta","dxypull_vs_eta",nint,min,max,100,-10,10));
      ptpull_vs_eta.push_back(ibooker.book2D("ptpull_vs_eta","ptpull_vs_eta",nint,min,max,100,-10,10));
      dzpull_vs_eta.push_back(ibooker.book2D("dzpull_vs_eta","dzpull_vs_eta",nint,min,max,100,-10,10));
      phipull_vs_eta.push_back(ibooker.book2D("phipull_vs_eta","phipull_vs_eta",nint,min,max,100,-10,10));
      thetapull_vs_eta.push_back(ibooker.book2D("thetapull_vs_eta","thetapull_vs_eta",nint,min,max,100,-10,10));

      //pulls of track params vs phi
      ptpull_vs_phi.push_back(ibooker.book2D("ptpull_vs_phi","p_{t} pull vs #phi",nintPhi,minPhi,maxPhi,100,-10,10));
      phipull_vs_phi.push_back(ibooker.book2D("phipull_vs_phi","#phi pull vs #phi",nintPhi,minPhi,maxPhi,100,-10,10));
      thetapull_vs_phi.push_back(ibooker.book2D("thetapull_vs_phi","#theta pull vs #phi",nintPhi,minPhi,maxPhi,100,-10,10));

      nrecHit_vs_nsimHit_sim2rec.push_back( ibooker.book2D("nrecHit_vs_nsimHit_sim2rec","nrecHit vs nsimHit (Sim2RecAssoc)",nintHit,minHit,maxHit, nintHit,minHit,maxHit ));
      nrecHit_vs_nsimHit_rec2sim.push_back( ibooker.book2D("nrecHit_vs_nsimHit_rec2sim","nrecHit vs nsimHit (Rec2simAssoc)",nintHit,minHit,maxHit, nintHit,minHit,maxHit ));
        
      //////////////// Cesare
        
      h_numEvt.push_back( ibooker.book1D("numEvt","N of events",2,0,2) );
      h_recoetaBin1.push_back( ibooker.book1D("num_reco_eta_bin1","N of reco track vs eta bin1",nint,min,max) );
      h_recoetaBin2.push_back( ibooker.book1D("num_reco_eta_bin2","N of reco track vs eta bin2",nint,min,max) );
      h_recoetaBin3.push_back( ibooker.book1D("num_reco_eta_bin3","N of reco track vs eta bin3",nint,min,max) );
      h_recoetaBin4.push_back( ibooker.book1D("num_reco_eta_bin4","N of reco track vs eta bin4",nint,min,max) );
      h_recoetaPt10.push_back( ibooker.book1D("num_reco_eta_pt10","N of reco track vs eta bin2",nint,min,max) );
        
      h_assocetaBin1.push_back( ibooker.book1D("num_assoc(simToReco)_eta_bin1","N of associated tracks (simToReco) vs eta bin1",nint,min,max) );
      h_assocetaBin2.push_back( ibooker.book1D("num_assoc(simToReco)_eta_bin2","N of associated tracks (simToReco) vs eta bin2",nint,min,max) );
      h_assocetaBin3.push_back( ibooker.book1D("num_assoc(simToReco)_eta_bin3","N of associated tracks (simToReco) vs eta bin3",nint,min,max) );
      h_assocetaBin4.push_back( ibooker.book1D("num_assoc(simToReco)_eta_bin4","N of associated tracks (simToReco) vs eta bin4",nint,min,max) );
      h_assocetaPt10.push_back( ibooker.book1D("num_assoc(simToReco)_eta_pt10","N of associated tracks (simToReco) vs eta bin2",nint,min,max) );

      h_assocvtx.push_back( ibooker.book1D("num_assoc(simToReco)_vtx","N of associated tracks (simToReco) vs vtx",nintVtx,minVtx,maxVtx) );
      h_assocvtxBarrel.push_back( ibooker.book1D("num_assoc(simToReco)_vtx_barrel","N of associated tracks (simToReco) vs vtx barrel",nintVtx,minVtx,maxVtx) );
      h_assocvtxEndcap.push_back( ibooker.book1D("num_assoc(simToReco)_vtx_endcap","N of associated tracks (simToReco) vs vtx endcap",nintVtx,minVtx,maxVtx) );
      h_assocvtxOverlap.push_back( ibooker.book1D("num_assoc(simToReco)_vtx_overlap","N of associated tracks (simToReco) vs vtx overlap",nintVtx,minVtx,maxVtx) );

      h_numRecVtx.push_back( ibooker.book1D("num_rec_vtx","N of reco vtx",nintVtx,minVtx,maxVtx) );
      h_numSimVtx.push_back( ibooker.book1D("num_sim_vtx","N of sim vtx",nintVtx,minVtx,maxVtx) );
      h_numSimVtxBx.push_back( ibooker.book1D("num_sim_vtx_bx","N of sim vtx BX = 0",nintVtx,minVtx,maxVtx) );
      h_numTrueInt.push_back( ibooker.book1D("num_trueInt","N of true interactions",nintVtx,minVtx,maxVtx) );

      h_dxyDistr.push_back( ibooker.book1D("distr_reco_dxy","dxy distributions",nintDxy,minDxy,maxDxy) );
      h_dzDistr.push_back( ibooker.book1D("distr_reco_dz","dz distribution",nintDz,minDz,maxDz) );
        
      h_assoc2etaphi.push_back( ibooker.book2D("num_assoc(recoToSim)_etaphi","N of associated (recoToSim) tracks vs eta",nint,min,max,nintPhi,minPhi,maxPhi) );

      h_assoc2eta_SgnMu.push_back( ibooker.book1D("num_assoc(recoToSim)_eta_SgnMu","N of associated (recoToSim) tracks vs eta",nint,min,max) );
      h_assoc2eta_SgnBkg.push_back( ibooker.book1D("num_assoc(recoToSim)_eta_SgnBkg","N of associated (recoToSim) tracks vs eta",nint,min,max) );
      h_assoc2eta_Pu.push_back( ibooker.book1D("num_assoc(recoToSim)_eta_Pu","N of associated (recoToSim) tracks vs eta",nint,min,max) );
      h_assoc2eta_PuIT.push_back( ibooker.book1D("num_assoc(recoToSim)_eta_PuIT","N of associated (recoToSim) tracks vs eta",nint,min,max) );
      h_assoc2eta_PuOOT.push_back( ibooker.book1D("num_assoc(recoToSim)_eta_PuOOT","N of associated (recoToSim) tracks vs eta",nint,min,max) );
      h_assoc2eta_PuMu.push_back( ibooker.book1D("num_assoc(recoToSim)_eta_PuMu","N of associated (recoToSim) tracks vs eta",nint,min,max) );
      h_assoc2eta_PuITMu.push_back( ibooker.book1D("num_assoc(recoToSim)_eta_PuITMu","N of associated (recoToSim) tracks vs eta",nint,min,max) );
      h_assoc2eta_PuOOTMu.push_back( ibooker.book1D("num_assoc(recoToSim)_eta_PuOOTMu","N of associated (recoToSim) tracks vs eta",nint,min,max) );

      h_assoc2etaBin1.push_back( ibooker.book1D("num_assoc(recoToSim)_eta_bin1","N of associated (recoToSim) tracks vs eta bin1",nint,min,max) );
      h_assoc2etaBin2.push_back( ibooker.book1D("num_assoc(recoToSim)_eta_bin2","N of associated (recoToSim) tracks vs eta bin2",nint,min,max) );
      h_assoc2etaBin3.push_back( ibooker.book1D("num_assoc(recoToSim)_eta_bin3","N of associated (recoToSim) tracks vs eta bin3",nint,min,max) );
      h_assoc2etaBin4.push_back( ibooker.book1D("num_assoc(recoToSim)_eta_bin4","N of associated (recoToSim) tracks vs eta bin4",nint,min,max) );
      h_assoc2etaPt10.push_back( ibooker.book1D("num_assoc(recoToSim)_eta_pt10","N of associated (recoToSim) tracks vs eta bin2",nint,min,max) );
        
      h_simuletaBin1.push_back( ibooker.book1D("num_simul_eta_bin1","N of simulated tracks vs eta bin1",nint,min,max) );
      h_simuletaBin2.push_back( ibooker.book1D("num_simul_eta_bin2","N of simulated tracks vs eta bin2",nint,min,max) );
      h_simuletaBin3.push_back( ibooker.book1D("num_simul_eta_bin3","N of simulated tracks vs eta bin3",nint,min,max) );
      h_simuletaBin4.push_back( ibooker.book1D("num_simul_eta_bin4","N of simulated tracks vs eta bin4",nint,min,max) );
      h_simuletaPt10.push_back( ibooker.book1D("num_simul_eta_pt10","N of simulated tracks vs eta bin2",nint,min,max) );

      h_simulvtx.push_back( ibooker.book1D("num_simul_vtx","N of simulated tracks vs vtx",nintVtx,minVtx,maxVtx) );
      h_simulvtxBarrel.push_back( ibooker.book1D("num_simul_vtx_barrel","N of simulated tracks vs vtx barrel",nintVtx,minVtx,maxVtx) );
      h_simulvtxEndcap.push_back( ibooker.book1D("num_simul_vtx_endcap","N of simulated tracks vs vtx endcap",nintVtx,minVtx,maxVtx) );
      h_simulvtxOverlap.push_back( ibooker.book1D("num_simul_vtx_overlap","N of simulated tracks vs vtx overlap",nintVtx,minVtx,maxVtx) );

      h_recopTBin1.push_back( ibooker.book1D("num_reco_pT_barrel","N of reco track vs pT barrel",nintpT,minpT,maxpT) );
      h_recopTBin2.push_back( ibooker.book1D("num_reco_pT_overlap","N of reco track vs pT overlap",nintpT,minpT,maxpT) );
      h_recopTBin3.push_back( ibooker.book1D("num_reco_pT_endcap","N of reco track vs pT endcap",nintpT,minpT,maxpT) );

      h_assocpTvsEta.push_back( ibooker.book2D("num_assoc(simToReco)_pTEta","N of associated tracks (simToReco) vs pT",nintpT,minpT,maxpT,nint,min,max) );
      h_assocpTvsPhi.push_back( ibooker.book2D("num_assoc(simToReco)_pTPhi","N of associated tracks (simToReco) vs pT",nintpT,minpT,maxpT,nintPhi,minPhi,maxPhi) );
      h_assocpTBin1.push_back( ibooker.book1D("num_assoc(simToReco)_pT_barrel","N of associated tracks (simToReco) vs pT barrel",nintpT,minpT,maxpT) );
      h_assocpTBin2.push_back( ibooker.book1D("num_assoc(simToReco)_pT_overlap","N of associated tracks (simToReco) vs pT overlap",nintpT,minpT,maxpT) );
      h_assocpTBin3.push_back( ibooker.book1D("num_assoc(simToReco)_pT_endcap","N of associated tracks (simToReco) vs pT endcap",nintpT,minpT,maxpT) );
        
      h_assoc2pT_SgnMu.push_back( ibooker.book1D("num_assoc(recoToSim)_pT_SgnMu","N of associated (recoToSim) tracks vs pT",nintpT,minpT,maxpT) );
      h_assoc2pT_SgnBkg.push_back( ibooker.book1D("num_assoc(recoToSim)_pT_SgnBkg","N of associated (recoToSim) tracks vs pT",nintpT,minpT,maxpT) );
      h_assoc2pT_Pu.push_back( ibooker.book1D("num_assoc(recoToSim)_pT_Pu","N of associated (recoToSim) tracks vs pT",nintpT,minpT,maxpT) );
      h_assoc2pT_PuIT.push_back( ibooker.book1D("num_assoc(recoToSim)_pT_PuIT","N of associated (recoToSim) tracks vs pT",nintpT,minpT,maxpT) );
      h_assoc2pT_PuOOT.push_back( ibooker.book1D("num_assoc(recoToSim)_pT_PuOOT","N of associated (recoToSim) tracks vs pT",nintpT,minpT,maxpT) );
      h_assoc2pT_PuMu.push_back( ibooker.book1D("num_assoc(recoToSim)_pT_PuMu","N of associated (recoToSim) tracks vs pT",nintpT,minpT,maxpT) );
      h_assoc2pT_PuITMu.push_back( ibooker.book1D("num_assoc(recoToSim)_pT_PuITMu","N of associated (recoToSim) tracks vs pT",nintpT,minpT,maxpT) );
      h_assoc2pT_PuOOTMu.push_back( ibooker.book1D("num_assoc(recoToSim)_pT_PuOOTMu","N of associated (recoToSim) tracks vs pT",nintpT,minpT,maxpT) );

      h_assoc2pTBin1.push_back( ibooker.book1D("num_assoc(recoToSim)_pT_barrel","N of associated (recoToSim) tracks vs pT barrel",nintpT,minpT,maxpT) );
      h_assoc2pTBin2.push_back( ibooker.book1D("num_assoc(recoToSim)_pT_overlap","N of associated (recoToSim) tracks vs pT overlap",nintpT,minpT,maxpT) );
      h_assoc2pTBin3.push_back( ibooker.book1D("num_assoc(recoToSim)_pT_endcap","N of associated (recoToSim) tracks vs pT endcap",nintpT,minpT,maxpT) );

      h_simulpTvsEta.push_back( ibooker.book2D("num_simul_pTEta","N of simulated tracks (simToReco) vs pT",nintpT,minpT,maxpT,nint,min,max) );
      h_simulpTvsPhi.push_back( ibooker.book2D("num_simul_pTPhi","N of simulated tracks (simToReco) vs pT",nintpT,minpT,maxpT,nintPhi,minPhi,maxPhi) );

      h_simulpTBin1.push_back( ibooker.book1D("num_simul_pT_barrel","N of simulated tracks vs pT barrel",nintpT,minpT,maxpT) );
      h_simulpTBin2.push_back( ibooker.book1D("num_simul_pT_overlap","N of simulated tracks vs pT overlap",nintpT,minpT,maxpT) );
      h_simulpTBin3.push_back( ibooker.book1D("num_simul_pT_endcap","N of simulated tracks vs pT endcap",nintpT,minpT,maxpT) );

      h_recoetaphi.push_back( ibooker.book2D("num_reco_etaphi","N of reco track vs phi",nint,min,max,nintPhi,minPhi,maxPhi) );
        
      h_assocLxy.push_back( ibooker.book1D("num_assoc(simToReco)_lxy","N of associated tracks (simToReco) vs Lxy",nintLxy,minLxy,maxLxy) );
      h_simulLxy.push_back( ibooker.book1D("num_simul_lxy","N of simulated tracks vs Lxy",nintLxy,minLxy,maxLxy) );
        
      h_assocLz.push_back( ibooker.book1D("num_assoc(simToReco)_lz","N of associated tracks (simToReco) vs Lz",nintLz,minLz,maxLz) );
      h_simulLz.push_back( ibooker.book1D("num_simul_lz","N of simulated tracks vs Lz",nintLz,minLz,maxLz) );
        
      h_assocLr.push_back( ibooker.book1D("num_assoc(simToReco)_lr","N of associated tracks (simToReco) vs R",nintLr,minLr,maxLr) );
      h_simulLr.push_back( ibooker.book1D("num_simul_lr","N of simulated tracks vs R",nintLr,minLr,maxLr) );
        
              h_Qoverptres.push_back( ibooker.book1D("qoverptres","qoverpt res.",250,-6,6) );
      h_invptres.push_back( ibooker.book1D("invptres","invpt res.",250,-6,6) );
      h_Qoverptres_sim.push_back( ibooker.book1D("qoverptres_sim","qoverpt res. sim",250,-6,6) );
      h_invptres_sim.push_back( ibooker.book1D("invptres_sim","invpt res. sim",250,-6,6) );

      n_trkLayersOld.push_back( ibooker.book1D("nhits_trkLayersOld","nhits trkLayersOld",nintHit,minHit,maxHit) );
      n_pxlHitsOld.push_back( ibooker.book1D("nhits_pxlHitsOld","nhits pixelHitsOld",nintHit,minHit,maxHit) );
      n_pxlHitsNew.push_back( ibooker.book1D("nhits_pxlHitsNew","nhits pxlHitsNew",nintHit,minHit,maxHit) );
      n_pxlHitsNew43Inn.push_back( ibooker.book1D("nhits_pxlHitsNew43Inn","nhits pxlHitsNew43Inn",nintHit,minHit,maxHit) );
      n_pxlHitsNew32Inn.push_back( ibooker.book1D("nhits_pxlHitsNew32Inn","nhits pxlHitsNew32Inn",nintHit,minHit,maxHit) );

      invptres_vs_eta.push_back(ibooker.book2D("invptres_vs_eta","invptres_vs_eta",nint,min,max, ptRes_nbin, ptRes_rangeMin, ptRes_rangeMax));
      invptres_vs_phi.push_back( ibooker.book2D("invptres_vs_phi","1/p_{t} res vs #phi",nintPhi,minPhi,maxPhi, ptRes_nbin, ptRes_rangeMin, ptRes_rangeMax));
      invptres_vs_pt.push_back(ibooker.book2D("invptres_vs_pt","invptres_vs_pt",nintpT,minpT,maxpT, ptRes_nbin, ptRes_rangeMin, ptRes_rangeMax));

      qOverPtres_vs_eta.push_back(ibooker.book2D("qOverPtres_vs_eta","qOverPtres_vs_eta",nint,min,max, ptRes_nbin, ptRes_rangeMin, ptRes_rangeMax));
      qOverPtres_vs_phi.push_back( ibooker.book2D("qOverPtres_vs_phi","q/p_{t} res vs #phi",nintPhi,minPhi,maxPhi, ptRes_nbin, ptRes_rangeMin, ptRes_rangeMax));
      qOverPtres_vs_pt.push_back(ibooker.book2D("qOverPtres_vs_pt","qOverPtres_vs_pt",nintpT,minpT,maxpT, ptRes_nbin, ptRes_rangeMin, ptRes_rangeMax));

      qOverPtresXL_vs_eta.push_back(ibooker.book2D("qOverPtresXL_vs_eta","qOverPtres_vs_eta",nint,min,max, 20*ptRes_nbin, 20*ptRes_rangeMin, 20*ptRes_rangeMax));
      qOverPtresXL_vs_phi.push_back( ibooker.book2D("qOverPtresXL_vs_phi","q/p_{t} res vs #phi",nintPhi,minPhi,maxPhi, 20*ptRes_nbin, 20*ptRes_rangeMin, 20*ptRes_rangeMax));
      qOverPtresXL_vs_pt.push_back(ibooker.book2D("qOverPtresXL_vs_pt","qOverPtres_vs_pt",nintpT,minpT,maxpT, 20*ptRes_nbin, 20*ptRes_rangeMin, 20*ptRes_rangeMax));

      chargeMisID_vs_eta.push_back(ibooker.book2D("chargeMisID_vs_eta","chargeMisID_vs_eta",nint,min,max,5,-2.5,2.5));
      chargeMisID_vs_phi.push_back(ibooker.book2D("chargeMisID_vs_phi","chargeMisID vs #phi",nintPhi,minPhi,maxPhi,5,-2.5,2.5));
      chargeMisID_vs_pt.push_back(ibooker.book2D("chargeMisID_vs_pt","chargeMisID_vs_pt",nintpT,minpT,maxpT,5,-2.5,2.5));

      ptres_vs_eta_sim.push_back(ibooker.book2D("ptres_vs_eta_sim","ptres_vs_eta sim",nintRes,minRes,maxRes, ptRes_nbin, ptRes_rangeMin, ptRes_rangeMax));
      ptres_vs_phi_sim.push_back( ibooker.book2D("ptres_vs_phi_sim","p_{t} res vs #phi sim",nintPhi,minPhi,maxPhi, ptRes_nbin, ptRes_rangeMin, ptRes_rangeMax));
      ptres_vs_pt_sim.push_back(ibooker.book2D("ptres_vs_pt_sim","ptres_vs_pt sim",nintpT,minpT,maxpT, ptRes_nbin, ptRes_rangeMin, ptRes_rangeMax));

      invptres_vs_eta_sim.push_back(ibooker.book2D("invptres_vs_eta_sim","invptres_vs_eta sim",nintRes,minRes,maxRes, ptRes_nbin, ptRes_rangeMin, ptRes_rangeMax));
      invptres_vs_phi_sim.push_back( ibooker.book2D("invptres_vs_phi_sim","1/p_{t} res vs #phi sim",nintPhi,minPhi,maxPhi, ptRes_nbin, ptRes_rangeMin, ptRes_rangeMax));
      invptres_vs_pt_sim.push_back(ibooker.book2D("invptres_vs_pt_sim","invptres_vs_pt sim",nintpT,minpT,maxpT, ptRes_nbin, ptRes_rangeMin, ptRes_rangeMax));

      qOverPtres_vs_eta_sim.push_back(ibooker.book2D("qOverPtres_vs_eta_sim","qOverPtres_vs_eta sim",nintRes,minRes,maxRes, ptRes_nbin, ptRes_rangeMin, ptRes_rangeMax));
      qOverPtres1_vs_eta_sim.push_back(ibooker.book2D("qOverPtres1_vs_eta_sim","qOverPtres1_vs_eta sim",nintRes,minRes,maxRes, ptRes_nbin, ptRes_rangeMin, ptRes_rangeMax));
      qOverPtres2_vs_eta_sim.push_back(ibooker.book2D("qOverPtres2_vs_eta_sim","qOverPtres2_vs_eta sim",nintRes,minRes,maxRes, ptRes_nbin, ptRes_rangeMin, ptRes_rangeMax));
      qOverPtres3_vs_eta_sim.push_back(ibooker.book2D("qOverPtres3_vs_eta_sim","qOverPtres3_vs_eta sim",nintRes,minRes,maxRes, ptRes_nbin, ptRes_rangeMin, ptRes_rangeMax));
      qOverPtres4_vs_eta_sim.push_back(ibooker.book2D("qOverPtres4_vs_eta_sim","qOverPtres4_vs_eta sim",nintRes,minRes,maxRes, ptRes_nbin, ptRes_rangeMin, ptRes_rangeMax));

      qOverPtres_vs_phi_sim.push_back( ibooker.book2D("qOverPtres_vs_phi_sim","q/p_{t} res vs #phi sim",nintPhi,minPhi,maxPhi, ptRes_nbin, ptRes_rangeMin, ptRes_rangeMax));
      qOverPtres_vs_pt_sim.push_back(ibooker.book2D("qOverPtres_vs_pt_sim","qOverPtres_vs_pt sim",nintpT,minpT,maxpT, ptRes_nbin, ptRes_rangeMin, ptRes_rangeMax));
      qOverPtresB_vs_pt_sim.push_back(ibooker.book2D("qOverPtresB_vs_pt_sim","qOverPtresB_vs_pt sim",nintpT,minpT,maxpT, ptRes_nbin, ptRes_rangeMin, ptRes_rangeMax));
      qOverPtresO_vs_pt_sim.push_back(ibooker.book2D("qOverPtresO_vs_pt_sim","qOverPtresO_vs_pt sim",nintpT,minpT,maxpT, ptRes_nbin, ptRes_rangeMin, ptRes_rangeMax));
      qOverPtresE_vs_pt_sim.push_back(ibooker.book2D("qOverPtresE_vs_pt_sim","qOverPtresE_vs_pt sim",nintpT,minpT,maxpT, ptRes_nbin, ptRes_rangeMin, ptRes_rangeMax));

      qOverPtresXL_vs_eta_sim.push_back(ibooker.book2D("qOverPtresXL_vs_eta_sim","qOverPtres_vs_eta sim",nintRes,minRes,maxRes, 20*ptRes_nbin, 20*ptRes_rangeMin, 20*ptRes_rangeMax));
      qOverPtresXL1_vs_eta_sim.push_back(ibooker.book2D("qOverPtresXL1_vs_eta_sim","qOverPtres1_vs_eta sim",nintRes,minRes,maxRes, 20*ptRes_nbin, 20*ptRes_rangeMin, 20*ptRes_rangeMax));
      qOverPtresXL2_vs_eta_sim.push_back(ibooker.book2D("qOverPtresXL2_vs_eta_sim","qOverPtres2_vs_eta sim",nintRes,minRes,maxRes, 20*ptRes_nbin, 20*ptRes_rangeMin, 20*ptRes_rangeMax));
      qOverPtresXL3_vs_eta_sim.push_back(ibooker.book2D("qOverPtresXL3_vs_eta_sim","qOverPtres3_vs_eta sim",nintRes,minRes,maxRes, 20*ptRes_nbin, 20*ptRes_rangeMin, 20*ptRes_rangeMax));
      qOverPtresXL4_vs_eta_sim.push_back(ibooker.book2D("qOverPtresXL4_vs_eta_sim","qOverPtres4_vs_eta sim",nintRes,minRes,maxRes, 20*ptRes_nbin, 20*ptRes_rangeMin, 20*ptRes_rangeMax));

      qOverPtresXL_vs_phi_sim.push_back( ibooker.book2D("qOverPtresXL_vs_phi_sim","q/p_{t} res vs #phi sim",nintPhi,minPhi,maxPhi, 20*ptRes_nbin, 20*ptRes_rangeMin, 20*ptRes_rangeMax));
      qOverPtresXL_vs_pt_sim.push_back(ibooker.book2D("qOverPtresXL_vs_pt_sim","qOverPtres_vs_pt sim",nintpT,minpT,maxpT, 20*ptRes_nbin, 20*ptRes_rangeMin, 20*ptRes_rangeMax));
      qOverPtresXLB_vs_pt_sim.push_back(ibooker.book2D("qOverPtresXLB_vs_pt_sim","qOverPtresB_vs_pt sim",nintpT,minpT,maxpT, 20*ptRes_nbin, 20*ptRes_rangeMin, 20*ptRes_rangeMax));
      qOverPtresXLO_vs_pt_sim.push_back(ibooker.book2D("qOverPtresXLO_vs_pt_sim","qOverPtresO_vs_pt sim",nintpT,minpT,maxpT, 20*ptRes_nbin, 20*ptRes_rangeMin, 20*ptRes_rangeMax));
      qOverPtresXLE_vs_pt_sim.push_back(ibooker.book2D("qOverPtresXLE_vs_pt_sim","qOverPtresE_vs_pt sim",nintpT,minpT,maxpT, 20*ptRes_nbin, 20*ptRes_rangeMin, 20*ptRes_rangeMax));

      chargeMisID_vs_eta_sim.push_back(ibooker.book2D("chargeMisID_vs_eta_sim","chargeMisID_vs_eta sim",nint,min,max,5,-2.5,2.5));
      chargeMisID_vs_phi_sim.push_back(ibooker.book2D("chargeMisID_vs_phi_sim","chargeMisID vs #phi sim",nintPhi,minPhi,maxPhi,5,-2.5,2.5));
      chargeMisID_vs_pt_sim.push_back(ibooker.book2D("chargeMisID_vs_pt_sim","chargeMisID_vs_pt sim",nintpT,minpT,maxpT,5,-2.5,2.5));

      chargeMisID_vs_etaNum.push_back(ibooker.book1D("chargeMisID_vs_etaNum","chargeMisID_vs_etaNum",nint,min,max));
      chargeMisID_vs_etaDen.push_back(ibooker.book1D("chargeMisID_vs_etaDen","chargeMisID_vs_etaDen",nint,min,max));
      chargeMisID_vs_ptNum.push_back(ibooker.book1D("chargeMisID_vs_ptNum","chargeMisID_vs_ptNum",nintpT,minpT,maxpT));
      chargeMisID_vs_ptDen.push_back(ibooker.book1D("chargeMisID_vs_ptDen","chargeMisID_vs_ptDen",nintpT,minpT,maxpT));
      chargeMisID_vs_phiNum.push_back(ibooker.book1D("chargeMisID_vs_phiNum","chargeMisID_vs_phiNum",nintPhi,minPhi,maxPhi));
      chargeMisID_vs_phiDen.push_back(ibooker.book1D("chargeMisID_vs_phiDen","chargeMisID_vs_phiDen",nintPhi,minPhi,maxPhi));

      chargeMisID_vs_etaNum_sim.push_back(ibooker.book1D("chargeMisID_vs_etaNum_sim","chargeMisID_vs_etaNum sim",nint,min,max));
      chargeMisID_vs_etaDen_sim.push_back(ibooker.book1D("chargeMisID_vs_etaDen_sim","chargeMisID_vs_etaDen sim",nint,min,max));
      chargeMisID_vs_ptNum_sim.push_back(ibooker.book1D("chargeMisID_vs_ptNum_sim","chargeMisID_vs_ptNum sim",nintpT,minpT,maxpT));
      chargeMisID_vs_ptDen_sim.push_back(ibooker.book1D("chargeMisID_vs_ptDen_sim","chargeMisID_vs_ptDen sim",nintpT,minpT,maxpT));
      chargeMisID_vs_phiNum_sim.push_back(ibooker.book1D("chargeMisID_vs_phiNum_sim","chargeMisID_vs_phiNum sim",nintPhi,minPhi,maxPhi));
      chargeMisID_vs_phiDen_sim.push_back(ibooker.book1D("chargeMisID_vs_phiDen_sim","chargeMisID_vs_phiDen sim",nintPhi,minPhi,maxPhi));

        
      ///////////////
      
      if (MABH) {
	h_PurityVsQuality.push_back
	  (ibooker.book2D("PurityVsQuality","Purity vs Quality (MABH)",20,0.01,1.01,20,0.01,1.01) );
	h_assoceta_Quality05.push_back
	  (ibooker.book1D("num_assoc(simToReco)_eta_Q05","N of associated tracks (simToReco) vs eta (Quality>0.5)",nint,min,max) );
	h_assoceta_Quality075.push_back
	  (ibooker.book1D("num_assoc(simToReco)_eta_Q075","N of associated tracks (simToReco) vs eta (Quality>0.75)",nint,min,max) );
	h_assocpT_Quality05.push_back
	  (ibooker.book1D("num_assoc(simToReco)_pT_Q05","N of associated tracks (simToReco) vs pT (Quality>0.5)",nintpT,minpT,maxpT) );
	h_assocpT_Quality075.push_back
	  (ibooker.book1D("num_assoc(simToReco)_pT_Q075","N of associated tracks (simToReco) vs pT (Quality>0.75)",nintpT,minpT,maxpT) );
	h_assocphi_Quality05.push_back
	  (ibooker.book1D("num_assoc(simToReco)_phi_Q05","N of associated tracks (simToReco) vs phi (Quality>0.5)",nintPhi,minPhi,maxPhi) );
	h_assocphi_Quality075.push_back
	  (ibooker.book1D("num_assoc(simToReco)_phi_Q075","N of associated tracks (simToReco) vs phi (Quality>0.75)",nintPhi,minPhi,maxPhi) );
          
    h_quality.push_back( ibooker.book1D("quality","Quality (MABH)",20,0.01,1.01) );
	h_qualityVsEta.push_back( ibooker.book2D("qualityVsEta","Quality vs eta (MABH)",nint,min,max,20,0.01,1.01) );

    h_assoc2eta_075.push_back( ibooker.book1D("num_assoc(recoToSim)_eta_075","N of associated (recoToSim) tracks vs eta Q>75%",nint,min,max) );
    h_assoc2eta_050.push_back( ibooker.book1D("num_assoc(recoToSim)_eta_050","N of associated (recoToSim) tracks vs eta Q>50%",nint,min,max) );
    h_assoc2pT_075.push_back( ibooker.book1D("num_assoc(recoToSim)_pt_075","N of associated (recoToSim) tracks vs pt Q>75%",nintpT,minpT,maxpT) );
    h_assoc2pT_050.push_back( ibooker.book1D("num_assoc(recoToSim)_pt_050","N of associated (recoToSim) tracks vs pt Q>50%",nintpT,minpT,maxpT) );
      }

      if(useLogPt){
	BinLogX(dzres_vs_pt[j]->getTH2F());
	BinLogX(dxyres_vs_pt[j]->getTH2F());
	BinLogX(phires_vs_pt[j]->getTH2F());
	BinLogX(cotThetares_vs_pt[j]->getTH2F());
	BinLogX(ptres_vs_pt[j]->getTH2F());
	BinLogX(h_recopT[j]->getTH1F());
	BinLogX(h_assocpT[j]->getTH1F());
	BinLogX(h_assoc2pT[j]->getTH1F());
	BinLogX(h_simulpT[j]->getTH1F());
          
	BinLogX(h_recopTBin1[j]->getTH1F());
	BinLogX(h_recopTBin2[j]->getTH1F());
	BinLogX(h_recopTBin3[j]->getTH1F());
	BinLogX(h_assocpTvsEta[j]->getTH1F());
	BinLogX(h_assocpTvsPhi[j]->getTH1F());
	BinLogX(h_assocpTBin1[j]->getTH1F());
	BinLogX(h_assocpTBin2[j]->getTH1F());
	BinLogX(h_assocpTBin3[j]->getTH1F());
 	BinLogX(h_assoc2pT_075[j]->getTH1F());
	BinLogX(h_assoc2pT_050[j]->getTH1F());
	BinLogX(h_assoc2pTBin1[j]->getTH1F());
	BinLogX(h_assoc2pTBin2[j]->getTH1F());
	BinLogX(h_assoc2pTBin3[j]->getTH1F());

	BinLogX(h_assoc2pT_SgnMu[j]->getTH1F());
	BinLogX(h_assoc2pT_SgnBkg[j]->getTH1F());
	BinLogX(h_assoc2pT_Pu[j]->getTH1F());
	BinLogX(h_assoc2pT_PuIT[j]->getTH1F());
	BinLogX(h_assoc2pT_PuOOT[j]->getTH1F());
	BinLogX(h_assoc2pT_PuMu[j]->getTH1F());
	BinLogX(h_assoc2pT_PuITMu[j]->getTH1F());
	BinLogX(h_assoc2pT_PuOOTMu[j]->getTH1F());

	BinLogX(h_simulpTvsEta[j]->getTH1F());
	BinLogX(h_simulpTvsPhi[j]->getTH1F());
	BinLogX(h_simulpTBin1[j]->getTH1F());
	BinLogX(h_simulpTBin2[j]->getTH1F());
    BinLogX(h_simulpTBin3[j]->getTH1F());
          
	if (MABH) {
	  BinLogX(h_assocpT_Quality05[j]->getTH1F());
	  BinLogX(h_assocpT_Quality075[j]->getTH1F());
	}
	j++;
      }

    }
  }
}

void MuonTrackValidator::analyze(const edm::Event& event, const edm::EventSetup& setup){
  using namespace reco;

  edm::LogInfo("MuonTrackValidator") << "\n====================================================" << "\n"
				     << "Analyzing new event" << "\n"
				     << "====================================================\n" << "\n";

  edm::ESHandle<ParametersDefinerForTP> Lhc_parametersDefinerTP;
  std::unique_ptr<ParametersDefinerForTP> Cosmic_parametersDefinerTP;

  if(parametersDefiner=="LhcParametersDefinerForTP") {
    setup.get<TrackAssociatorRecord>().get(parametersDefiner, Lhc_parametersDefinerTP);
  }
  else if(parametersDefiner=="CosmicParametersDefinerForTP") {
    edm::ESHandle<CosmicParametersDefinerForTP>  _Cosmic_parametersDefinerTP;
    setup.get<TrackAssociatorRecord>().get(parametersDefiner, _Cosmic_parametersDefinerTP);

     //Since we modify the object, we must clone it
    Cosmic_parametersDefinerTP = _Cosmic_parametersDefinerTP->clone();

    edm::Handle<SimHitTPAssociationProducer::SimHitTPAssociationList> simHitsTPAssoc;
    //warning: make sure the TP collection used in the map is the same used here
    event.getByToken(_simHitTpMapTag,simHitsTPAssoc);
    Cosmic_parametersDefinerTP->initEvent(simHitsTPAssoc);
    cosmictpSelector.initEvent(simHitsTPAssoc);
  }
  else {
    edm::LogError("MuonTrackValidator")
      << "Unexpected label: parametersDefiner = "<< parametersDefiner.c_str() << "\n";
  }

  edm::Handle<TrackingParticleCollection> TPCollectionHeff ;
  event.getByToken(tp_effic_Token,TPCollectionHeff);
  TrackingParticleCollection const & tPCeff = *(TPCollectionHeff.product());

  edm::Handle<TrackingParticleCollection> TPCollectionHfake ;
  event.getByToken(tp_fake_Token,TPCollectionHfake);
  
  edm::Handle<reco::BeamSpot> recoBeamSpotHandle;
  event.getByToken(bsSrc_Token,recoBeamSpotHandle);
  reco::BeamSpot bs = *recoBeamSpotHandle;

  std::vector<const reco::TrackToTrackingParticleAssociator*> associator;
  if (UseAssociators) {
    edm::Handle<reco::TrackToTrackingParticleAssociator> theAssociator;
    for (unsigned int w=0;w<associators.size();w++) {
      event.getByLabel(associators[w],theAssociator);
      associator.push_back( theAssociator.product() );
    }
  }
    
  Handle<std::vector< PileupSummaryInfo > > PupInfo;
  event.getByToken(PuInfo_Token, PupInfo);
  std::vector<PileupSummaryInfo>::const_iterator PVI;

  int npv = -1;
  int trueInt = -1;
  int trueInt_BX = -1;
  for(PVI = PupInfo->begin(); PVI != PupInfo->end(); ++PVI) {

	int BX = PVI->getBunchCrossing();
	trueInt = PVI->getTrueNumInteractions();
	npv = PVI->getPU_NumInteractions();
	if(BX == 0) trueInt_BX = npv;

  }

  int nVertices = -1;
  edm::Handle<reco::VertexCollection> primaryVertices;
  if(vtxInputTag.label() != ""){
      
      event.getByToken(vtxTag_Token, primaryVertices);
      nVertices = primaryVertices->size();
      
  }
      
  int nTrueVertices = trueInt_BX;
  
  int w=0;
  for (unsigned int ww=0;ww<associators.size();ww++){
    for (unsigned int www=0;www<label.size();www++){
      //
      //get collections from the event
      //
      edm::Handle<edm::View<Track> > trackCollection;

      reco::RecoToSimCollection recSimColl;
      reco::SimToRecoCollection simRecColl;
      unsigned int trackCollectionSize = 0;

      if(!event.getByToken(track_Collection_Token[www], trackCollection)&&ignoremissingtkcollection_) {

	recSimColl.post_insert();
	simRecColl.post_insert();
	
      }

      else {

	trackCollectionSize = trackCollection->size();

	//associate tracks
	if(UseAssociators){
	  edm::LogVerbatim("MuonTrackValidator") << "Analyzing "
						 << label[www].process()<<":"
						 << label[www].label()<<":"
						 << label[www].instance()<<" with "
						 << associators[ww].c_str() <<"\n";
	  
	  LogTrace("MuonTrackValidator") << "Calling associateRecoToSim method" << "\n";
	  recSimColl=associator[ww]->associateRecoToSim(trackCollection,
							TPCollectionHfake);
	  LogTrace("MuonTrackValidator") << "Calling associateSimToReco method" << "\n";
	  simRecColl=associator[ww]->associateSimToReco(trackCollection,
							TPCollectionHeff);
	}
	else{
	  edm::LogVerbatim("MuonTrackValidator") << "Analyzing "
						 << label[www].process()<<":"
						 << label[www].label()<<":"
						 << label[www].instance()<<" with "
						 << associatormap.process()<<":"
						 << associatormap.label()<<":"
						 << associatormap.instance()<<"\n";
	  
	  Handle<reco::SimToRecoCollection > simtorecoCollectionH;
	  event.getByToken(simToRecoCollection_Token,simtorecoCollectionH);
	  simRecColl= *(simtorecoCollectionH.product());
	  
	  Handle<reco::RecoToSimCollection > recotosimCollectionH;
	  event.getByToken(recoToSimCollection_Token,recotosimCollectionH);
	  recSimColl= *(recotosimCollectionH.product());
	}
	
      }
        
      h_numEvt[w]->Fill(1);
      h_numSimVtx[w]->Fill(npv);
      h_numTrueInt[w]->Fill(trueInt);
      h_numSimVtxBx[w]->Fill(trueInt_BX);

      h_numRecVtx[w]->Fill(nVertices);
      
      //
      //fill simulation histograms
      //compute number of tracks per eta interval
      //
      edm::LogVerbatim("MuonTrackValidator") << "\n# of TrackingParticles: " << tPCeff.size() << "\n";
      int ats = 0;
      int st = 0;
      for (TrackingParticleCollection::size_type i=0; i<tPCeff.size(); i++){
	bool TP_is_matched = false;
	double quality = 0.;
	bool Quality05 = false;
	bool Quality075 = false;
	
	TrackingParticleRef tpr(TPCollectionHeff, i);
	TrackingParticle* tp = const_cast<TrackingParticle*>(tpr.get());
	
	TrackingParticle::Vector momentumTP;
	TrackingParticle::Point vertexTP;
	double dxySim = 0;
	double dzSim = 0;
    double prodRho = 0;
    double prodZ = 0;
    double prodR = 0;
	
	//If the TrackingParticle is collision-like, get the momentum and vertex at production state
	//and the impact parameters w.r.t. PCA
	if(parametersDefiner=="LhcParametersDefinerForTP")
	  {
	    LogTrace("MuonTrackValidator") <<"TrackingParticle "<< i;
	    if(! tpSelector(*tp)) continue;
	    momentumTP = tp->momentum();
	    vertexTP = tp->vertex();
          
        prodRho = sqrt(vertexTP.perp2());
		prodZ = vertexTP.z();
        prodR = sqrt(prodRho*prodRho + prodZ*prodZ);
//      int hitsPdgId = tp->pdgId();
//		int hitsStatus = tp->status();
//      int bx = tp->eventId().bunchCrossing();
//      int evtID = tp->eventId().event();
//          
//      bool isSignalMuon = (abs(tp->pdgId())==13 || abs(tp->pdgId())==11 || abs(tp->pdgId())==15) && !tp->genParticles().empty() && (tp->eventId().event() == 0) && (tp->eventId().bunchCrossing() == 0); //segnale muone
//      if(isSignalMuon) {
//          cout<<"-----------------------------------------------------------------------------------------------"<<endl;
//          cout << "\t Particle pdgId = "<< hitsPdgId << " status: " << hitsStatus << " rho = " << prodRho << ", z = " << prodZ << ", evtID = " << evtID << ", bx = " << bx << ", L = " << prodR << ", pT: " << sqrt(momentumTP.perp2()) << endl;
//          cout<<"-----------------------------------------------------------------------------------------------"<<endl;
//      }
          
        if(!(fabs(prodRho) < prodRho_ && fabs(prodZ) < prodZ_)) continue;
	    //if(! isSignalFromZgamma(tp)) continue;
	    //if(isSignalFromZgamma(tp)) cout<<"Signal: 1"<<endl;
        //else cout<<"Signal: 0"<<endl;
          
	    TrackingParticle::Vector momentum = Lhc_parametersDefinerTP->momentum(event,setup,tpr);
	    TrackingParticle::Point vertex = Lhc_parametersDefinerTP->vertex(event,setup,tpr);
	    dxySim = (-vertex.x()*sin(momentum.phi())+vertex.y()*cos(momentum.phi()));
	    dzSim = vertex.z() - (vertex.x()*momentum.x()+vertex.y()*momentum.y()) /
	                          sqrt(momentum.perp2()) * momentum.z()/sqrt(momentum.perp2());
	  }
	//for cosmics get the momentum and vertex at PCA
	else if(parametersDefiner=="CosmicParametersDefinerForTP")
	  {
	    edm::LogVerbatim("MuonTrackValidator") <<"TrackingParticle "<< i;
	    if(! cosmictpSelector(tpr,&bs,event,setup)) continue;	
	    momentumTP = Cosmic_parametersDefinerTP->momentum(event,setup,tpr);
	    vertexTP = Cosmic_parametersDefinerTP->vertex(event,setup,tpr);
	    dxySim = (-vertexTP.x()*sin(momentumTP.phi())+vertexTP.y()*cos(momentumTP.phi()));
	    dzSim = vertexTP.z() - (vertexTP.x()*momentumTP.x()+vertexTP.y()*momentumTP.y()) /
	                            sqrt(momentumTP.perp2()) * momentumTP.z()/sqrt(momentumTP.perp2());
	  }
	edm::LogVerbatim("MuonTrackValidator") <<"--------------------Selected TrackingParticle #"<<tpr.key();
	edm::LogVerbatim("MuonTrackValidator") <<"momentumTP: pt = "<<sqrt(momentumTP.perp2())<<", pz = "<<momentumTP.z() 
					       <<", \t vertexTP: radius = "<<sqrt(vertexTP.perp2())<<  ",  z = "<<vertexTP.z() <<"\n";
	st++;

	h_ptSIM[w]->Fill(sqrt(momentumTP.perp2()));
	h_etaSIM[w]->Fill(momentumTP.eta());
          
    h_simulpTvsEta[w]->Fill(sqrt(momentumTP.perp2()), momentumTP.eta());
    h_simulpTvsPhi[w]->Fill(sqrt(momentumTP.perp2()), momentumTP.phi());
          
	h_vertposSIM[w]->Fill(sqrt(vertexTP.perp2()));
	
	std::vector<std::pair<RefToBase<Track>, double> > rt;
	if(simRecColl.find(tpr) != simRecColl.end()){
	  rt = (std::vector<std::pair<RefToBase<Track>, double> >) simRecColl[tpr];
	  if (rt.size()!=0) {
	    RefToBase<Track> assoc_recoTrack = rt.begin()->first;
	    edm::LogVerbatim("MuonTrackValidator")<<"-----------------------------associated Track #"<<assoc_recoTrack.key();
          
        //if(assoc_recoTrack->hitPattern().muonStationsWithValidHits() > 1){
            TP_is_matched = true;
            ats++;
            quality = rt.begin()->second;
            edm::LogVerbatim("MuonTrackValidator") << "TrackingParticle #" <<tpr.key()
						   << " with pt=" << sqrt(momentumTP.perp2())
						   << " associated with quality:" << quality <<"\n";
            if (MABH) {
                
                h_quality[w]->Fill(quality);
                h_qualityVsEta[w]->Fill(fabs(momentumTP.eta()),quality);
                
                if (quality > 0.75) {
                    Quality075 = true;
                    Quality05 = true;
                }
                else if (quality > 0.5) {
                    Quality05 = true;
                }
            }
        //}
	  }
	}else{
	  edm::LogVerbatim("MuonTrackValidator")
	    << "TrackingParticle #" << tpr.key()
	    << " with pt,eta,phi: "
	    << sqrt(momentumTP.perp2()) << " , "
	    << momentumTP.eta() << " , "
	    << momentumTP.phi() << " , "
	    << " NOT associated to any reco::Track" << "\n";
	}
          
    for (unsigned int f=0; f<vtxintervals[w].size()-1; f++){
	  if (nTrueVertices>=vtxintervals[w][f]&&
	      nTrueVertices<vtxintervals[w][f+1]) {
	    totSIMvtx[w][f]++;
	    if(fabs(momentumTP.eta()) < 0.9) totSIMvtxBarrel[w][f]++;
	    else if(fabs(momentumTP.eta()) > 0.9 && fabs(momentumTP.eta()) < 1.2) totSIMvtxOverlap[w][f]++;
	    else if(fabs(momentumTP.eta()) > 1.2) totSIMvtxEndcap[w][f]++;
	    if (TP_is_matched) {
	      totASSvtx[w][f]++;
	      if(fabs(momentumTP.eta()) < 0.9) totASSvtxBarrel[w][f]++;
	      else if(fabs(momentumTP.eta()) > 0.9 && fabs(momentumTP.eta()) < 1.2) totASSvtxOverlap[w][f]++;
	      else if(fabs(momentumTP.eta()) > 1.2) totASSvtxEndcap[w][f]++;

	      /*if (MABH) {
		if (Quality075) {
		  totASSeta_Quality075[w][f]++;
		  totASSeta_Quality05[w][f]++;
		}
		else if (Quality05) {
		  totASSeta_Quality05[w][f]++;
		}
	      }*/
	    }
	  }
    } // END for (unsigned int f=0; f<vtxintervals[w].size()-1; f++){
          
	
	for (unsigned int f=0; f<etaintervals[w].size()-1; f++){
	  if (getEta(momentumTP.eta())>etaintervals[w][f]&&
	      getEta(momentumTP.eta())<etaintervals[w][f+1]) {
	    totSIMeta[w][f]++;
        if(sqrt(momentumTP.perp2()) > 5 && sqrt(momentumTP.perp2()) < 10) totSIMetaBin1[w][f]++;
	    if(sqrt(momentumTP.perp2()) > 10 && sqrt(momentumTP.perp2()) < 15) totSIMetaBin2[w][f]++;
	    if(sqrt(momentumTP.perp2()) > 15 && sqrt(momentumTP.perp2()) < 20) totSIMetaBin3[w][f]++;
	    if(sqrt(momentumTP.perp2()) > 20) totSIMetaBin4[w][f]++;
        if(sqrt(momentumTP.perp2()) > 10) totSIMetaPt10[w][f]++;
	    if (TP_is_matched) {
	      totASSeta[w][f]++;
          if(sqrt(momentumTP.perp2()) > 5 && sqrt(momentumTP.perp2()) < 10) totASSetaBin1[w][f]++;
	      if(sqrt(momentumTP.perp2()) > 10 && sqrt(momentumTP.perp2()) < 15) totASSetaBin2[w][f]++;
	      if(sqrt(momentumTP.perp2()) > 15 && sqrt(momentumTP.perp2()) < 20) totASSetaBin3[w][f]++;
          if(sqrt(momentumTP.perp2()) > 20) totASSetaBin4[w][f]++;
	      
	      if (MABH) {
		if (Quality075) {
		  totASSeta_Quality075[w][f]++;
		  totASSeta_Quality05[w][f]++;
          if(sqrt(momentumTP.perp2()) > 10) totASSetaPt10[w][f]++;
		}
		else if (Quality05) {
		  totASSeta_Quality05[w][f]++;
          if(sqrt(momentumTP.perp2()) > 10) totASSetaPt10[w][f]++;
		}
	      }
	    }
	  }
	} // END for (unsigned int f=0; f<etaintervals[w].size()-1; f++){
	
	for (unsigned int f=0; f<phiintervals[w].size()-1; f++){
	  if (momentumTP.phi() > phiintervals[w][f]&&
	      momentumTP.phi() <phiintervals[w][f+1]) {
	    totSIM_phi[w][f]++;
	    if (TP_is_matched) {
	      totASS_phi[w][f]++;
	      
	      if (MABH) {
		if (Quality075) {
		  totASS_phi_Quality075[w][f]++;
		  totASS_phi_Quality05[w][f]++;
		}
		else if (Quality05) {
		  totASS_phi_Quality05[w][f]++;
		}
	      }
	    }
	  }
	} // END for (unsigned int f=0; f<phiintervals[w].size()-1; f++){
	
	for (unsigned int f=0; f<pTintervals[w].size()-1; f++){
          if (getPt(sqrt(momentumTP.perp2()))>pTintervals[w][f]&&
              getPt(sqrt(momentumTP.perp2()))<pTintervals[w][f+1]) {
            totSIMpT[w][f]++;
            if(fabs(momentumTP.eta()) < 0.9) totSIMpTBin1[w][f]++;
            if(fabs(momentumTP.eta()) > 0.9 && fabs(momentumTP.eta()) < 1.2) totSIMpTBin2[w][f]++;
            if(fabs(momentumTP.eta()) > 1.2) totSIMpTBin3[w][f]++;
	    if (TP_is_matched) {
	      totASSpT[w][f]++;
          if(fabs(momentumTP.eta()) < 0.9) totASSpTBin1[w][f]++;
          if(fabs(momentumTP.eta()) > 0.9 && fabs(momentumTP.eta()) < 1.2) totASSpTBin2[w][f]++;
          if(fabs(momentumTP.eta()) > 1.2) totASSpTBin3[w][f]++;
	      
	      if (MABH) {
		if (Quality075) {
		  totASSpT_Quality075[w][f]++;
		  totASSpT_Quality05[w][f]++;
		}
		else if (Quality05) {
		  totASSpT_Quality05[w][f]++;
		}
	      }
	    }
	  }
	} // END for (unsigned int f=0; f<pTintervals[w].size()-1; f++){
	
	for (unsigned int f=0; f<dxyintervals[w].size()-1; f++){
	  if (dxySim>dxyintervals[w][f]&&
	      dxySim<dxyintervals[w][f+1]) {
	    totSIM_dxy[w][f]++;
	    if (TP_is_matched) {
	      totASS_dxy[w][f]++;
	    }
	  }
	} // END for (unsigned int f=0; f<dxyintervals[w].size()-1; f++){
	
	for (unsigned int f=0; f<dzintervals[w].size()-1; f++){
	  if (dzSim>dzintervals[w][f]&&
	      dzSim<dzintervals[w][f+1]) {
	    totSIM_dz[w][f]++;
	    if (TP_is_matched) {
	      totASS_dz[w][f]++;
	    }
	  }
	} // END for (unsigned int f=0; f<dzintervals[w].size()-1; f++){
          
    for (unsigned int f=0; f<lxyintervals[w].size()-1; f++){
	  if (fabs(prodRho)>lxyintervals[w][f] && fabs(prodRho)<lxyintervals[w][f+1]) {
	    totSIM_lxy[w][f]++;
	    if (TP_is_matched) {
	      totASS_lxy[w][f]++;
	    }
	  }
	} // END for (unsigned int f=0; f<lxyintervals[w].size()-1; f++){
          
	for (unsigned int f=0; f<lzintervals[w].size()-1; f++){
	  if (fabs(prodZ)>lzintervals[w][f] && fabs(prodZ)<lzintervals[w][f+1]) {
	    totSIM_lz[w][f]++;
	    if (TP_is_matched) {
	      totASS_lz[w][f]++;
	    }
	  }
	} // END for (unsigned int f=0; f<lxyintervals[w].size()-1; f++){
          
	for (unsigned int f=0; f<lrintervals[w].size()-1; f++){
	  if (prodR>lrintervals[w][f] && prodR<lrintervals[w][f+1]) {
	    totSIM_lr[w][f]++;
	    if (TP_is_matched) {
	      totASS_lr[w][f]++;
	    }
	  }
    } // END for (unsigned int f=0; f<lxyintervals[w].size()-1; f++){
	
	for (unsigned int f=0; f<vertposintervals[w].size()-1; f++){
	  if (sqrt(vertexTP.perp2())>vertposintervals[w][f]&&
	      sqrt(vertexTP.perp2())<vertposintervals[w][f+1]) {
	    totSIM_vertpos[w][f]++;
	    if (TP_is_matched) {
	      totASS_vertpos[w][f]++;
	    }
	  }
	} // END for (unsigned int f=0; f<vertposintervals[w].size()-1; f++){
	
	for (unsigned int f=0; f<zposintervals[w].size()-1; f++){
	  if (vertexTP.z()>zposintervals[w][f]&&
	      vertexTP.z()<zposintervals[w][f+1]) {
	    totSIM_zpos[w][f]++;
	    if (TP_is_matched) {
	      totASS_zpos[w][f]++;
	    }
	  }
	} // END for (unsigned int f=0; f<zposintervals[w].size()-1; f++){
	
	int nSimHits = 0;
	if (usetracker && usemuon) {
	  nSimHits= tpr.get()->numberOfHits();
	}
	else if (!usetracker && usemuon) {
	  nSimHits= tpr.get()->numberOfHits() - tpr.get()->numberOfTrackerHits();
	}
	else if (usetracker && !usemuon) {
	  nSimHits=tpr.get()->numberOfTrackerHits();
	}
	
        int tmp = std::min(nSimHits,int(maxHit-1));
	edm::LogVerbatim("MuonTrackValidator") << "\t N simhits = "<< nSimHits<<"\n";
	
	totSIM_hit[w][tmp]++;
	if (TP_is_matched) totASS_hit[w][tmp]++;
	
	if (TP_is_matched)	
	  {
	    RefToBase<Track> assoctrack = rt.begin()->first;
	    nrecHit_vs_nsimHit_sim2rec[w]->Fill( assoctrack->numberOfValidHits(),nSimHits);
	  }
      } // End for (TrackingParticleCollection::size_type i=0; i<tPCeff.size(); i++){
      if (st!=0) h_tracksSIM[w]->Fill(st);
      
      //
      //fill reconstructed track histograms
      //
      edm::LogVerbatim("MuonTrackValidator") << "\n# of reco::Tracks with "
					     << label[www].process()<<":"
					     << label[www].label()<<":"
					     << label[www].instance()
					     << ": " << trackCollectionSize << "\n";
      int at = 0;
      int rT = 0;
      for(edm::View<Track>::size_type i=0; i<trackCollectionSize; ++i){
        bool Track_is_matched = false;
        bool Track_is_matched_SignalMuon = false;
        bool Track_is_matched_SignalBkg = false;
        bool Track_is_matched_Pu = false;
        bool Track_is_matched_PuIT = false;
        bool Track_is_matched_PuOOT = false;
        bool Track_is_matched_PuMuon = false;
        bool Track_is_matched_PuITMuon = false;
        bool Track_is_matched_PuOOTMuon = false;
        bool Track_is_matched_075 = false;
        bool Track_is_matched_050 = false;
	RefToBase<Track> track(trackCollection, i);
    //if(track->hitPattern().muonStationsWithValidHits() <= 1) continue;
    //if((track->numberOfValidHits()) == 0) continue;
	rT++;
          
    double ipxy = -1, ipz = -1;

  	if (track.isNonnull() && vtxInputTag.label() != ""){
		
  		const reco::VertexCollection* vertexes = primaryVertices.product();

		if(vertexes->size()!=0){ 

			ipxy = fabs(track->dxy((*vertexes)[0].position()));
 			ipz = fabs(track->dz((*vertexes)[0].position()));

		}

  	}

	h_dxyDistr[w]->Fill(ipxy);
    h_dzDistr[w]->Fill(ipz);
	
	std::vector<std::pair<TrackingParticleRef, double> > tp;
	TrackingParticleRef tpr;

	// new logic (bidirectional)
	if (BiDirectional_RecoToSim_association) {	
	  edm::LogVerbatim("MuonTrackValidator")<<"----------------------------------------Track #"<< track.key();
	  
	  if(recSimColl.find(track) != recSimColl.end()) {
	    tp = recSimColl[track];	
	    if (tp.size() != 0) {
	      tpr = tp.begin()->first;	
	      // RtS and StR must associate the same pair !
	      if(simRecColl.find(tpr) != simRecColl.end()) {
		std::vector<std::pair<RefToBase<Track>, double> > track_checkback = simRecColl[tpr];
		RefToBase<Track> assoc_track_checkback;
		assoc_track_checkback = track_checkback.begin()->first;
		
		if ( assoc_track_checkback.key() == track.key() ) {
		  edm::LogVerbatim("MuonTrackValidator")<<"------------------associated TrackingParticle #"<<tpr.key();

		  TrackingParticle* tpRtS=const_cast<TrackingParticle*>(tpr.get());

		  bool signalEvent = (tpRtS->eventId().event() == 0) && (tpRtS->eventId().bunchCrossing() == 0);

		  bool isSignalMuon = abs(tpRtS->pdgId())==13 && !tpRtS->genParticles().empty() && signalEvent; //segnale muone
		  //bool isBkgTPGen = abs(tpRtS->pdgId())!=13 && !tpRtS->genParticles().empty() && signalEvent; //segnale non muone
		  //bool isBkgTPGeant = tpRtS->genParticles().empty() && signalEvent; //GEANT
		  bool bkgSignalEventMatchedMuon = signalEvent && ( tpRtS->genParticles().empty() || abs(tpRtS->pdgId())!=13 );

		  bool isPuTP = !signalEvent; //PU
		  bool isPuTPMuon = abs(tpRtS->pdgId())==13 && isPuTP; //PU muon
		  bool isPuTPMuonIT = abs(tpRtS->pdgId())==13 && (tpRtS->eventId().event() != 0) && tpRtS->eventId().bunchCrossing()==0; //PU muon IT
		  bool isPuTPMuonOOT = abs(tpRtS->pdgId())==13 && (tpRtS->eventId().event() != 0) && tpRtS->eventId().bunchCrossing()!=0; //PU muon OOT
		  bool isPuTPIT = (tpRtS->eventId().event() != 0) && tpRtS->eventId().bunchCrossing()==0; //PU IT
		  bool isPuTPOOT = (tpRtS->eventId().event() != 0) && tpRtS->eventId().bunchCrossing()!=0; //PU OOT

		  //std::cout<<isSignalMuon<<" "<<isBkgTP<<" "<<isPuTP<<" "<<isPuTPMuon<<" "<<isPuTPMuonIT<<" "<<isPuTPMuonOOT<<" "<<isPuTPIT<<" "<<isPuTPOOT<<std::endl;

		  for(TrackingParticle::g4t_iterator g4T=(*tpRtS).g4Track_begin(); g4T!=(*tpRtS).g4Track_end(); ++g4T) {

			edm::LogVerbatim("MuonTrackValidator")<<"\t"<< " Id:"<<(*g4T).trackId()<<"/Evt:("<<(*g4T).eventId().event()<<","<<(*g4T).eventId().bunchCrossing()<<")";

		  }

		  if (isSignalMuon) {

			int hitsPdgId = tpRtS->pdgId();
			int hitsStatus = tpRtS->status();
			double prodRho = tpRtS->vertex().Rho();
			double prodZ = tpRtS->vz();
			edm::LogVerbatim("MuonTrackValidator") << "\t Particle pdgId = "<< hitsPdgId << " status:" << hitsStatus << " produced at rho = " << prodRho << ", z = " << prodZ;

                	reco::GenParticleRef genp   = tpRtS->genParticles()[0];
                	reco::GenParticleRef genMom = genp->numberOfMothers() > 0 ? genp->motherRef() : reco::GenParticleRef();
			//cout << "\t Number of mothers: "<< genp->numberOfMothers() << std::endl;
                	if (genMom.isNonnull()) {

                    		int momPdgId  = genMom->pdgId();
                    		int momStatus = genMom->status();
                    		double momRho = genMom->vertex().Rho(); 
				double momZ = genMom->vz();
                    		edm::LogVerbatim("MuonTrackValidator") << "\t Particle pdgId = "<<hitsPdgId << ", has GEN mother pdgId = " << momPdgId << ", mom status: " << momStatus;
                    		reco::GenParticleRef genGMom = genMom->numberOfMothers() > 0 ? genMom->motherRef() : reco::GenParticleRef();
				//cout << "\t\t Number of gmoms: " << genMom->numberOfMothers() << std::endl;
                    		if (genGMom.isNonnull()) {

                        		int gmomPdgId = genGMom->pdgId();
					int gmomStatus = genGMom->status();
                        		edm::LogVerbatim("MuonTrackValidator") << "\t\t mother prod. vertex rho = " << momRho << ", z = " << momZ << ", grand-mom pdgId = " << gmomPdgId << ", status: " << gmomStatus;

                    			reco::GenParticleRef genGGMom = genGMom->numberOfMothers() > 0 ? genGMom->motherRef() : reco::GenParticleRef();
					//cout << "\t\t Number of ggmoms: " << genGMom->numberOfMothers() << std::endl;
                    			if (genGGMom.isNonnull()) {

                        			int ggmomPdgId = genGGMom->pdgId();
                        			int ggmomStatus = genGGMom->status();
                        			edm::LogVerbatim("MuonTrackValidator") << "\t\t grand-grand-mom pdgId = " << ggmomPdgId << ", status: " << ggmomStatus;

                    				reco::GenParticleRef genGGGMom = genGGMom->numberOfMothers() > 0 ? genGGMom->motherRef() : reco::GenParticleRef();
						//cout << "\t\t Number of gggmoms: " << genGGMom->numberOfMothers() << std::endl;
                    				if (genGGGMom.isNonnull()) {

                        				int gggmomPdgId = genGGGMom->pdgId();
                        				int gggmomStatus = genGGGMom->status();
                        				edm::LogVerbatim("MuonAssociatorByHitsHelper") << "\t\t grand-grand-grand-mom pdgId = " << gggmomPdgId << ", status: " << gggmomStatus;

                    				}	

                    			}

                    		}

                	}

		  }
		  if (!useMCTruth_ || (useMCTruth_ && isSignalMuon)) {

			Track_is_matched = true;
			at++;
			double Purity = tp.begin()->second;
			double Quality = track_checkback.begin()->second;

	 		if(MABH && Quality > 0.75) Track_is_matched_075 = true;
			if(MABH && Quality > 0.50) Track_is_matched_050 = true;

			edm::LogVerbatim("MuonTrackValidator") << "reco::Track #" << track.key() << " with pt=" << track->pt() 
								 << " associated with quality:" << Purity <<"\n";
			if (MABH) h_PurityVsQuality[w]->Fill(Quality,Purity);

		  }

        	  if(isSignalMuon) Track_is_matched_SignalMuon = true;
        	  if(bkgSignalEventMatchedMuon) Track_is_matched_SignalBkg = true;
        	  if(isPuTP) Track_is_matched_Pu = true;
        	  if(isPuTPIT) Track_is_matched_PuIT = true;
        	  if(isPuTPOOT) Track_is_matched_PuOOT = true;
        	  if(isPuTPMuon) Track_is_matched_PuMuon = true;
        	  if(isPuTPMuonIT) Track_is_matched_PuITMuon = true;
        	  if(isPuTPMuonOOT) Track_is_matched_PuOOTMuon = true; 

		}
	      }
	    }
	  }
	  
	  if (!Track_is_matched)
	    edm::LogVerbatim("MuonTrackValidator")
	      << "reco::Track #" << track.key() << " with pt=" << track->pt() << " NOT associated to any TrackingParticle" << "\n";
	}
	// old logic (bugged for collision scenario, still valid for cosmics 2 legs reco)
	else {
	  if(recSimColl.find(track) != recSimColl.end()){
	    tp = recSimColl[track];
	    if (tp.size()!=0) {
	      Track_is_matched = true;
	      tpr = tp.begin()->first;
	      at++;
	      edm::LogVerbatim("MuonTrackValidator") << "reco::Track #" << track.key() << " with pt=" << track->pt()
						     << " associated with quality:" << tp.begin()->second <<"\n";
	    }
	  } else {
	    edm::LogVerbatim("MuonTrackValidator") << "reco::Track #" << track.key() << " with pt=" << track->pt()
						   << " NOT associated to any TrackingParticle" << "\n";	
	  }
	}
          
          
    h_recoetaphi[w]->Fill(track->momentum().eta(), track->momentum().phi());
    if(Track_is_matched_SignalMuon) h_assoc2etaphi[w]->Fill(track->momentum().eta(), track->momentum().phi());

	//Compute fake rate vs eta
	for (unsigned int f=0; f<etaintervals[w].size()-1; f++){
	  if (getEta(track->momentum().eta())>etaintervals[w][f]&&
	      getEta(track->momentum().eta())<etaintervals[w][f+1]) {
	    totRECeta[w][f]++;
        if(sqrt(track->momentum().perp2()) > 5 && sqrt(track->momentum().perp2()) < 10) totRECetaBin1[w][f]++;
        if(sqrt(track->momentum().perp2()) > 10 && sqrt(track->momentum().perp2()) < 15) totRECetaBin2[w][f]++;
        if(sqrt(track->momentum().perp2()) > 15 && sqrt(track->momentum().perp2()) < 20) totRECetaBin3[w][f]++;
        if(sqrt(track->momentum().perp2()) > 20) totRECetaBin4[w][f]++;
        if(sqrt(track->momentum().perp2()) > 10) totRECetaPt10[w][f]++;
	    if (Track_is_matched) {
	      totASS2eta[w][f]++;
          if(sqrt(track->momentum().perp2()) > 5 && sqrt(track->momentum().perp2()) < 10) totASS2etaBin1[w][f]++;
          if(sqrt(track->momentum().perp2()) > 10 && sqrt(track->momentum().perp2()) < 15) totASS2etaBin2[w][f]++;
          if(sqrt(track->momentum().perp2()) > 15 && sqrt(track->momentum().perp2()) < 20) totASS2etaBin3[w][f]++;
          if(sqrt(track->momentum().perp2()) > 20) totASS2etaBin4[w][f]++;
	    }
          
        if(Track_is_matched_SignalMuon) totASS2eta_SgnMu[w][f]++;
        if(Track_is_matched_SignalBkg) totASS2eta_SgnBkg[w][f]++;
        if(Track_is_matched_Pu) totASS2eta_Pu[w][f]++;
        if(Track_is_matched_PuIT) totASS2eta_PuIT[w][f]++;
        if(Track_is_matched_PuOOT) totASS2eta_PuOOT[w][f]++;
        if(Track_is_matched_PuMuon) totASS2eta_PuMu[w][f]++;
        if(Track_is_matched_PuITMuon) totASS2eta_PuITMu[w][f]++;
        if(Track_is_matched_PuOOTMuon) totASS2eta_PuOOTMu[w][f]++;
          
        if (MABH && Track_is_matched_075) totASS2eta_075[w][f]++;
        if (MABH && Track_is_matched_050){
           totASS2eta_050[w][f]++;
           if(sqrt(track->momentum().perp2()) > 10) totASS2etaPt10[w][f]++;
        }
          
	  }
	} // End for (unsigned int f=0; f<etaintervals[w].size()-1; f++){

	for (unsigned int f=0; f<phiintervals[w].size()-1; f++){
	  if (track->momentum().phi()>phiintervals[w][f]&&
	      track->momentum().phi()<phiintervals[w][f+1]) {
	    totREC_phi[w][f]++;
	    if (Track_is_matched) {
	      totASS2_phi[w][f]++;
	    }
	  }
	} // End for (unsigned int f=0; f<phiintervals[w].size()-1; f++){
	
	for (unsigned int f=0; f<pTintervals[w].size()-1; f++){
	  if (getPt(sqrt(track->momentum().perp2()))>pTintervals[w][f]&&
	      getPt(sqrt(track->momentum().perp2()))<pTintervals[w][f+1]) {
	    totRECpT[w][f]++;
        if(fabs(track->momentum().eta()) < 0.9) totRECpTBin1[w][f]++;
        if(fabs(track->momentum().eta()) > 0.9 && fabs(track->momentum().eta()) < 1.2) totRECpTBin2[w][f]++;
        if(fabs(track->momentum().eta()) > 1.2) totRECpTBin3[w][f]++;
	    if (Track_is_matched) {
	      totASS2pT[w][f]++;
          if(fabs(track->momentum().eta()) < 0.9) totASS2pTBin1[w][f]++;
          if(fabs(track->momentum().eta()) > 0.9 && fabs(track->momentum().eta()) < 1.2) totASS2pTBin2[w][f]++;
          if(fabs(track->momentum().eta()) > 1.2) totASS2pTBin3[w][f]++;
	    }
          
        if(Track_is_matched_SignalMuon) totASS2pT_SgnMu[w][f]++;
        if(Track_is_matched_SignalBkg) totASS2pT_SgnBkg[w][f]++;
        if(Track_is_matched_Pu) totASS2pT_Pu[w][f]++;
        if(Track_is_matched_PuIT) totASS2pT_PuIT[w][f]++;
        if(Track_is_matched_PuOOT) totASS2pT_PuOOT[w][f]++;
        if(Track_is_matched_PuMuon) totASS2pT_PuMu[w][f]++;
        if(Track_is_matched_PuITMuon) totASS2pT_PuITMu[w][f]++;
        if(Track_is_matched_PuOOTMuon) totASS2pT_PuOOTMu[w][f]++;
          
        if (MABH && Track_is_matched_075) totASS2pT_075[w][f]++;
        if (MABH && Track_is_matched_050) totASS2pT_050[w][f]++;
          
	  }
	} // End for (unsigned int f=0; f<pTintervals[w].size()-1; f++){
	
	for (unsigned int f=0; f<dxyintervals[w].size()-1; f++){
	  if (track->dxy(bs.position())>dxyintervals[w][f]&&
	      track->dxy(bs.position())<dxyintervals[w][f+1]) {
	    totREC_dxy[w][f]++;
	    if (Track_is_matched) {
	      totASS2_dxy[w][f]++;
	    }
	  }
	} // End for (unsigned int f=0; f<dxyintervals[w].size()-1; f++){
	
	for (unsigned int f=0; f<dzintervals[w].size()-1; f++){
	  if (track->dz(bs.position())>dzintervals[w][f]&&
	      track->dz(bs.position())<dzintervals[w][f+1]) {
	    totREC_dz[w][f]++;
	    if (Track_is_matched) {
	      totASS2_dz[w][f]++;
	    }
	  }
	} // End for (unsigned int f=0; f<dzintervals[w].size()-1; f++){

	int tmp = std::min((int)track->found(),int(maxHit-1));
	totREC_hit[w][tmp]++;
	if (Track_is_matched) totASS2_hit[w][tmp]++;
	
	edm::LogVerbatim("MuonTrackValidator") << "\t N valid rechits = "<< (int)track->found() <<"\n";
	
	// Fill other histos
	TrackingParticle* tpp = const_cast<TrackingParticle*>(tpr.get());
	// TrackingParticle parameters at point of closest approach to the beamline
	TrackingParticle::Vector momentumTP;
	TrackingParticle::Point vertexTP;
	
	if (parametersDefiner=="LhcParametersDefinerForTP") {
	  // following reco plots are made only from tracks associated to selected signal TPs 
	  if (! (Track_is_matched && tpSelector(*tpp)) ) continue;
	  else {
	    momentumTP = Lhc_parametersDefinerTP->momentum(event,setup,tpr) ;
	    vertexTP = Lhc_parametersDefinerTP->vertex(event,setup,tpr);
	  }
	}
	else if (parametersDefiner=="CosmicParametersDefinerForTP") {
	  // following reco plots are made only from tracks associated to selected signal TPs 
	  if (! (Track_is_matched && cosmictpSelector(tpr,&bs,event,setup)) ) continue;
	  else {
	    momentumTP = Cosmic_parametersDefinerTP->momentum(event,setup,tpr) ;
	    vertexTP = Cosmic_parametersDefinerTP->vertex(event,setup,tpr);	    
	  }
	}
	
	if (associators[ww]=="trackAssociatorByChi2"){
	  //association chi2
	  double assocChi2 = -tp.begin()->second;//in association map is stored -chi2
	  h_assochi2[www]->Fill(assocChi2);
	  h_assochi2_prob[www]->Fill(TMath::Prob((assocChi2)*5,5));
	}
	else if (associators[ww]=="trackAssociatorByHits"){
	  double fraction = tp.begin()->second;
	  h_assocFraction[www]->Fill(fraction);
	  h_assocSharedHit[www]->Fill(fraction*track->numberOfValidHits());
	}
	
	//nchi2 and hits global distributions
	h_nchi2[w]->Fill(track->normalizedChi2());
	h_nchi2_prob[w]->Fill(TMath::Prob(track->chi2(),(int)track->ndof()));
	h_hits[w]->Fill(track->numberOfValidHits());
	h_losthits[w]->Fill(track->numberOfLostHits());
	chi2_vs_nhits[w]->Fill(track->numberOfValidHits(),track->normalizedChi2());
	h_charge[w]->Fill( track->charge() );
	
	double ptSim = sqrt(momentumTP.perp2());
    double chargeSim = tpr->charge();
	double qoverpSim = tpr->charge()/sqrt(momentumTP.x()*momentumTP.x()+momentumTP.y()*momentumTP.y()+momentumTP.z()*momentumTP.z());
	double thetaSim = momentumTP.theta();
    double etaSim = momentumTP.eta();
	double lambdaSim = M_PI/2-momentumTP.theta();
	double phiSim = momentumTP.phi();
	double dxySim = (-vertexTP.x()*sin(momentumTP.phi())+vertexTP.y()*cos(momentumTP.phi()));
	double dzSim = vertexTP.z() - (vertexTP.x()*momentumTP.x()+vertexTP.y()*momentumTP.y()) /
	                               sqrt(momentumTP.perp2()) * momentumTP.z()/sqrt(momentumTP.perp2());
    
          
    h_assocpTvsEta[w]->Fill(ptSim, etaSim);
    h_assocpTvsPhi[w]->Fill(ptSim, phiSim);
	
	// removed unused variable, left this in case it has side effects
	track->parameters();
	
	double qoverpRec(0);
	double qoverpErrorRec(0);
	double ptRec(0);
	double ptErrorRec(0);
	double lambdaRec(0);
	double lambdaErrorRec(0);
	double phiRec(0);
	double phiErrorRec(0);
	
	//loop to decide whether to take gsfTrack (utilisation of mode-function) or common track
	const GsfTrack* gsfTrack(0);
	if(useGsf){
	  gsfTrack = dynamic_cast<const GsfTrack*>(&(*track));
	  if (gsfTrack==0) edm::LogInfo("MuonTrackValidator") << "Trying to access mode for a non-GsfTrack";
	}
	
	if (gsfTrack) {
	  // get values from mode
	  getRecoMomentum(*gsfTrack, ptRec, ptErrorRec, qoverpRec, qoverpErrorRec,
			  lambdaRec,lambdaErrorRec, phiRec, phiErrorRec);
	}
	
	else {
	  // get values from track (without mode)
	  getRecoMomentum(*track, ptRec, ptErrorRec, qoverpRec, qoverpErrorRec,
			  lambdaRec,lambdaErrorRec, phiRec, phiErrorRec);
	}
	
	double thetaRec = track->theta();
    double chargeRec = track->charge();
	double ptError = ptErrorRec;
	double ptres = ptRec - ptSim;
	double etares = track->eta()-momentumTP.Eta();
	double dxyRec = track->dxy(bs.position());
	double dzRec = track->dz(bs.position());
	// eta residue; pt, k, theta, phi, dxy, dz pulls
	double qoverpPull=(qoverpRec-qoverpSim)/qoverpErrorRec;
	double thetaPull=(lambdaRec-lambdaSim)/lambdaErrorRec;
	double phiDiff = phiRec - phiSim;
	if (abs(phiDiff) > M_PI) {
	  if (phiDiff >0.) phiDiff = phiDiff - 2.*M_PI;
	  else phiDiff = phiDiff + 2.*M_PI;
	}
	double phiPull=phiDiff/phiErrorRec;
	double dxyPull=(dxyRec-dxySim)/track->dxyError();
	double dzPull=(dzRec-dzSim)/track->dzError();
	
	double contrib_Qoverp = ((qoverpRec-qoverpSim)/qoverpErrorRec)*((qoverpRec-qoverpSim)/qoverpErrorRec)/5;
	double contrib_dxy = ((dxyRec-dxySim)/track->dxyError())*((dxyRec-dxySim)/track->dxyError())/5;
	double contrib_dz = ((dzRec-dzSim)/track->dzError())*((dzRec-dzSim)/track->dzError())/5;
	double contrib_theta = ((lambdaRec-lambdaSim)/lambdaErrorRec)*((lambdaRec-lambdaSim)/lambdaErrorRec)/5;
	double contrib_phi = (phiDiff/phiErrorRec)*(phiDiff/phiErrorRec)/5;
	
	edm::LogVerbatim("MuonTrackValidator") << "assocChi2=" << tp.begin()->second << "\n"
					       << "" << "\n"
					       << "ptREC=" << ptRec << "\n"
					       << "etaREC=" << track->eta() << "\n"
					       << "qoverpREC=" << qoverpRec << "\n"
					       << "dxyREC=" << dxyRec << "\n"
					       << "dzREC=" << dzRec << "\n"
					       << "thetaREC=" << track->theta() << "\n"
					       << "phiREC=" << phiRec << "\n"
					       << "" << "\n"
					       << "qoverpError()=" << qoverpErrorRec << "\n"
					       << "dxyError()=" << track->dxyError() << "\n"
					       << "dzError()=" << track->dzError() << "\n"
					       << "thetaError()=" << lambdaErrorRec << "\n"
					       << "phiError()=" << phiErrorRec << "\n"
					       << "" << "\n"
					       << "ptSIM=" << ptSim << "\n"
					       << "etaSIM=" << momentumTP.Eta() << "\n"
					       << "qoverpSIM=" << qoverpSim << "\n"
					       << "dxySIM=" << dxySim << "\n"
					       << "dzSIM=" << dzSim << "\n"
					       << "thetaSIM=" << M_PI/2-lambdaSim << "\n"
					       << "phiSIM=" << phiSim << "\n"
					       << "" << "\n"
					       << "contrib_Qoverp=" << contrib_Qoverp << "\n"
					       << "contrib_dxy=" << contrib_dxy << "\n"
					       << "contrib_dz=" << contrib_dz << "\n"
					       << "contrib_theta=" << contrib_theta << "\n"
					       << "contrib_phi=" << contrib_phi << "\n"
					       << "" << "\n"
					       <<"chi2PULL="<<contrib_Qoverp+contrib_dxy+contrib_dz+contrib_theta+contrib_phi<<"\n";
	
	h_pullQoverp[w]->Fill(qoverpPull);
	h_pullTheta[w]->Fill(thetaPull);
	h_pullPhi[w]->Fill(phiPull);
	h_pullDxy[w]->Fill(dxyPull);
	h_pullDz[w]->Fill(dzPull);
    h_Qoverptres_sim[w]->Fill((chargeRec/ptRec-chargeSim/ptSim)/(chargeSim/ptSim));
    h_invptres_sim[w]->Fill((1/ptRec-1/ptSim)/(1/ptSim));
    h_Qoverptres[w]->Fill((chargeRec/ptRec-chargeSim/ptSim)/(chargeRec/ptRec));
    h_invptres[w]->Fill((1/ptRec-1/ptSim)/(1/ptRec));
	
	h_pt[w]->Fill(ptres/ptError);
	h_eta[w]->Fill(etares);
	etares_vs_eta[w]->Fill(getEta(track->eta()),etares);
	
	//chi2 and #hit vs eta: fill 2D histos
	chi2_vs_eta[w]->Fill(getEta(track->eta()),track->normalizedChi2());
	nhits_vs_eta[w]->Fill(getEta(track->eta()),track->numberOfValidHits());
	nDThits_vs_eta[w]->Fill(getEta(track->eta()),track->hitPattern().numberOfValidMuonDTHits());
	nCSChits_vs_eta[w]->Fill(getEta(track->eta()),track->hitPattern().numberOfValidMuonCSCHits());
	nRPChits_vs_eta[w]->Fill(getEta(track->eta()),track->hitPattern().numberOfValidMuonRPCHits());
    if(useGEMs_) nGEMhits_vs_eta[w]->Fill(getEta(track->eta()),track->hitPattern().numberOfValidMuonGEMHits());
    if(useME0_) nME0hits_vs_eta[w]->Fill(getEta(track->eta()),track->hitPattern().numberOfValidMuonME0Hits());
    n_trkLayersOld[w]->Fill(track->hitPattern().trackerLayersWithMeasurement());
    n_pxlHitsOld[w]->Fill(track->hitPattern().numberOfValidPixelHits());
    n_pxlHitsNew[w]->Fill(track->hitPattern().pixelLayersWithMeasurement());
    n_pxlHitsNew43Inn[w]->Fill(track->hitPattern().pixelLayersWithMeasurement());
    n_pxlHitsNew32Inn[w]->Fill(track->hitPattern().pixelLayersWithMeasurement());//Ask tracker people
	nlosthits_vs_eta[w]->Fill(getEta(track->eta()),track->numberOfLostHits());//Ask tracker people
	
	//resolution of track params: fill 2D histos
	dxyres_vs_eta[w]->Fill(getEta(track->eta()),dxyRec-dxySim);
	ptres_vs_eta[w]->Fill(getEta(track->eta()),(ptRec-ptSim)/ptRec);
	dzres_vs_eta[w]->Fill(getEta(track->eta()),dzRec-dzSim);
	phires_vs_eta[w]->Fill(getEta(track->eta()),phiDiff);
	cotThetares_vs_eta[w]->Fill(getEta(track->eta()), cos(thetaRec)/sin(thetaRec) - cos(thetaSim)/sin(thetaSim));
          
    invptres_vs_eta[w]->Fill(getEta(track->eta()),(1/ptRec-1/ptSim)/(1/ptRec));
    qOverPtres_vs_eta[w]->Fill(getEta(track->eta()),(chargeRec/ptRec-chargeSim/ptSim)/(chargeRec/ptRec));
    qOverPtresXL_vs_eta[w]->Fill(getEta(track->eta()),(chargeRec/ptRec-chargeSim/ptSim)/(chargeRec/ptRec));

    //dxyres_vs_eta[w]->Fill(getEta(etaSim),dxyRec-dxySim);
    ptres_vs_eta_sim[w]->Fill(getEta(etaSim),(ptRec-ptSim)/ptSim);
    invptres_vs_eta_sim[w]->Fill(getEta(etaSim),(1/ptRec-1/ptSim)/(1/ptSim));
    qOverPtres_vs_eta_sim[w]->Fill(getEta(etaSim),(chargeRec/ptRec-chargeSim/ptSim)/(chargeSim/ptSim));

    if(ptSim > 5 && ptSim < 10){
 		qOverPtres1_vs_eta_sim[w]->Fill(getEta(etaSim),(chargeRec/ptRec-chargeSim/ptSim)/(chargeSim/ptSim));
	  	qOverPtresXL1_vs_eta_sim[w]->Fill(getEta(etaSim),(chargeRec/ptRec-chargeSim/ptSim)/(chargeSim/ptSim));
    }
    else if(ptSim > 10 && ptSim < 15){
	 	qOverPtres2_vs_eta_sim[w]->Fill(getEta(etaSim),(chargeRec/ptRec-chargeSim/ptSim)/(chargeSim/ptSim));
	  	qOverPtresXL2_vs_eta_sim[w]->Fill(getEta(etaSim),(chargeRec/ptRec-chargeSim/ptSim)/(chargeSim/ptSim));
    }
    else if(ptSim > 15 && ptSim < 20){
 		qOverPtres3_vs_eta_sim[w]->Fill(getEta(etaSim),(chargeRec/ptRec-chargeSim/ptSim)/(chargeSim/ptSim));
		qOverPtresXL3_vs_eta_sim[w]->Fill(getEta(etaSim),(chargeRec/ptRec-chargeSim/ptSim)/(chargeSim/ptSim));
    }
    else if(ptSim > 20){
 		qOverPtres4_vs_eta_sim[w]->Fill(getEta(etaSim),(chargeRec/ptRec-chargeSim/ptSim)/(chargeSim/ptSim));
	  	qOverPtresXL4_vs_eta_sim[w]->Fill(getEta(etaSim),(chargeRec/ptRec-chargeSim/ptSim)/(chargeSim/ptSim));
    }

    qOverPtresXL_vs_eta_sim[w]->Fill(getEta(etaSim),(chargeRec/ptRec-chargeSim/ptSim)/(chargeSim/ptSim));
    //dzres_vs_eta[w]->Fill(getEta(etaSim),dzRec-dzSim);
    //phires_vs_eta[w]->Fill(getEta(etaSim),phiDiff);
    //cotThetares_vs_eta[w]->Fill(getEta(etaSim), cos(thetaRec)/sin(thetaRec) - cos(thetaSim)/sin(thetaSim));
	
	//same as before but vs pT
	dxyres_vs_pt[w]->Fill(getPt(ptRec),dxyRec-dxySim);
	ptres_vs_pt[w]->Fill(getPt(ptRec),(ptRec-ptSim)/ptRec);
	dzres_vs_pt[w]->Fill(getPt(ptRec),dzRec-dzSim);
	phires_vs_pt[w]->Fill(getPt(ptRec),phiDiff);
	cotThetares_vs_pt[w]->Fill(getPt(ptRec), cos(thetaRec)/sin(thetaRec) - cos(thetaSim)/sin(thetaSim));
          
    invptres_vs_pt[w]->Fill(getPt(ptSim),(1/ptRec-1/ptSim)/(1/ptRec));
    qOverPtres_vs_pt[w]->Fill(getPt(ptRec),(chargeRec/ptRec-chargeSim/ptSim)/(chargeRec/ptRec));
    qOverPtresXL_vs_pt[w]->Fill(getPt(ptRec),(chargeRec/ptRec-chargeSim/ptSim)/(chargeRec/ptRec));

    chargeMisID_vs_eta[w]->Fill(getEta(track->eta()),chargeSim*chargeRec);
    chargeMisID_vs_pt[w]->Fill(getPt(ptRec),chargeSim*chargeRec);
    chargeMisID_vs_phi[w]->Fill(phiRec,chargeSim*chargeRec);
    //dxyres_vs_pt[w]->Fill(getPt(ptSim),dxyRec-dxySim);
    ptres_vs_pt_sim[w]->Fill(getPt(ptSim),(ptRec-ptSim)/ptSim);
    invptres_vs_pt_sim[w]->Fill(getPt(ptSim),(1/ptRec-1/ptSim)/(1/ptSim));
    qOverPtres_vs_pt_sim[w]->Fill(getPt(ptSim),(chargeRec/ptRec-chargeSim/ptSim)/(chargeSim/ptSim));
    qOverPtresXL_vs_pt_sim[w]->Fill(getPt(ptSim),(chargeRec/ptRec-chargeSim/ptSim)/(chargeSim/ptSim));

    if(fabs(etaSim) > 0 && fabs(etaSim) < 0.9){
	  	qOverPtresB_vs_pt_sim[w]->Fill(getPt(ptSim),(chargeRec/ptRec-chargeSim/ptSim)/(chargeSim/ptSim));
	  	qOverPtresXLB_vs_pt_sim[w]->Fill(getPt(ptSim),(chargeRec/ptRec-chargeSim/ptSim)/(chargeSim/ptSim));
    }
    else if(fabs(etaSim) > 0.9 && fabs(etaSim) < 1.2){
	  	qOverPtresO_vs_pt_sim[w]->Fill(getPt(ptSim),(chargeRec/ptRec-chargeSim/ptSim)/(chargeSim/ptSim));
	  	qOverPtresXLO_vs_pt_sim[w]->Fill(getPt(ptSim),(chargeRec/ptRec-chargeSim/ptSim)/(chargeSim/ptSim));
    }
    else if(fabs(etaSim) > 1.2){
	  	qOverPtresE_vs_pt_sim[w]->Fill(getPt(ptSim),(chargeRec/ptRec-chargeSim/ptSim)/(chargeSim/ptSim));
	  	qOverPtresXLE_vs_pt_sim[w]->Fill(getPt(ptSim),(chargeRec/ptRec-chargeSim/ptSim)/(chargeSim/ptSim));
    }

    //dzres_vs_pt[w]->Fill(getPt(ptSim),dzRec-dzSim);
    //phires_vs_pt[w]->Fill(getPt(ptSim),phiDiff);
    //cotThetares_vs_pt[w]->Fill(getPt(ptSim), cos(thetaRec)/sin(thetaRec) - cos(thetaSim)/sin(thetaSim));

    chargeMisID_vs_eta_sim[w]->Fill(getEta(etaSim),chargeSim*chargeRec);
    chargeMisID_vs_pt_sim[w]->Fill(getPt(ptSim),chargeSim*chargeRec);
    chargeMisID_vs_phi_sim[w]->Fill(phiSim,chargeSim*chargeRec);
	
	//pulls of track params vs eta: fill 2D histos
	dxypull_vs_eta[w]->Fill(getEta(track->eta()),dxyPull);
	ptpull_vs_eta[w]->Fill(getEta(track->eta()),ptres/ptError);
	dzpull_vs_eta[w]->Fill(getEta(track->eta()),dzPull);
	phipull_vs_eta[w]->Fill(getEta(track->eta()),phiPull);
	thetapull_vs_eta[w]->Fill(getEta(track->eta()),thetaPull);
          
    //dxypull_vs_eta[w]->Fill(getEta(etaSim),dxyPull);
    //ptpull_vs_eta[w]->Fill(getEta(etaSim),ptres/ptError);
    //dzpull_vs_eta[w]->Fill(getEta(etaSim),dzPull);
    //phipull_vs_eta[w]->Fill(getEta(etaSim),phiPull);
    //thetapull_vs_eta[w]->Fill(getEta(etaSim),thetaPull);
	
	//plots vs phi
	nhits_vs_phi[w]->Fill(phiRec,track->numberOfValidHits());
	chi2_vs_phi[w]->Fill(phiRec,track->normalizedChi2());
	ptmean_vs_eta_phi[w]->Fill(phiRec,getEta(track->eta()),ptRec);
	phimean_vs_eta_phi[w]->Fill(phiRec,getEta(track->eta()),phiRec);
	ptres_vs_phi[w]->Fill(phiRec,(ptRec-ptSim)/ptRec);
	phires_vs_phi[w]->Fill(phiRec,phiDiff);
	ptpull_vs_phi[w]->Fill(phiRec,ptres/ptError);
	phipull_vs_phi[w]->Fill(phiRec,phiPull);
	thetapull_vs_phi[w]->Fill(phiRec,thetaPull);

    invptres_vs_phi[w]->Fill(phiRec,(1/ptRec-1/ptSim)/(1/ptRec));
    qOverPtres_vs_phi[w]->Fill(phiRec,(chargeRec/ptRec-chargeSim/ptSim)/(chargeRec/ptRec));
    qOverPtresXL_vs_phi[w]->Fill(phiRec,(chargeRec/ptRec-chargeSim/ptSim)/(chargeRec/ptRec));

    ptres_vs_phi_sim[w]->Fill(phiSim,(ptRec-ptSim)/ptSim);
    invptres_vs_phi_sim[w]->Fill(phiSim,(1/ptRec-1/ptSim)/(1/ptSim));
    qOverPtres_vs_phi_sim[w]->Fill(phiSim,(chargeRec/ptRec-chargeSim/ptSim)/(chargeSim/ptSim));
    qOverPtresXL_vs_phi_sim[w]->Fill(phiSim,(chargeRec/ptRec-chargeSim/ptSim)/(chargeSim/ptSim));
    //phires_vs_phi[w]->Fill(phiSim,phiDiff);
    //ptpull_vs_phi[w]->Fill(phiSim,ptres/ptError);
    //phipull_vs_phi[w]->Fill(phiSim,phiPull);
    //thetapull_vs_phi[w]->Fill(phiSim,thetaPull);
	
	int nSimHits = 0;
	if (usetracker && usemuon) {
	  nSimHits= tpr.get()->numberOfHits();
	}
	else if (!usetracker && usemuon) {
	  nSimHits= tpr.get()->numberOfHits() - tpr.get()->numberOfTrackerHits();
	}
	else if (usetracker && !usemuon) {
	  nSimHits=tpr.get()->numberOfTrackerHits();
	}
	
	nrecHit_vs_nsimHit_rec2sim[w]->Fill(track->numberOfValidHits(), nSimHits);
	
      } // End of for(View<Track>::size_type i=0; i<trackCollectionSize; ++i){
      
      if (at!=0) h_tracks[w]->Fill(at);
      h_fakes[w]->Fill(rT-at);
      edm::LogVerbatim("MuonTrackValidator") << "Total Simulated: " << st << "\n"
					     << "Total Associated (simToReco): " << ats << "\n"
					     << "Total Reconstructed: " << rT << "\n"
					     << "Total Associated (recoToSim): " << at << "\n"
					     << "Total Fakes: " << rT-at << "\n";
      nrec_vs_nsim[w]->Fill(rT,st);
      w++;
    } // End of for (unsigned int www=0;www<label.size();www++){
  } //END of for (unsigned int ww=0;ww<associators.size();ww++){
}

void MuonTrackValidator::endRun(Run const&, EventSetup const&) {
  
  int w=0;
  for (unsigned int ww=0;ww<associators.size();ww++){
    for (unsigned int www=0;www<label.size();www++){
        
      extractCharge(chargeMisID_vs_eta[w], chargeMisID_vs_etaNum[w], chargeMisID_vs_etaDen[w]);
      extractCharge(chargeMisID_vs_pt[w], chargeMisID_vs_ptNum[w], chargeMisID_vs_ptDen[w]);
      extractCharge(chargeMisID_vs_phi[w], chargeMisID_vs_phiNum[w], chargeMisID_vs_phiDen[w]);
        
      extractCharge(chargeMisID_vs_eta_sim[w], chargeMisID_vs_etaNum_sim[w], chargeMisID_vs_etaDen_sim[w]);
      extractCharge(chargeMisID_vs_pt_sim[w], chargeMisID_vs_ptNum_sim[w], chargeMisID_vs_ptDen_sim[w]);
      extractCharge(chargeMisID_vs_phi_sim[w], chargeMisID_vs_phiNum_sim[w], chargeMisID_vs_phiDen_sim[w]);
      
      //chi2 and #hit vs eta: get mean from 2D histos
      doProfileX(chi2_vs_eta[w],h_chi2meanh[w]);
      doProfileX(nhits_vs_eta[w],h_hits_eta[w]);
      doProfileX(nDThits_vs_eta[w],h_DThits_eta[w]);
      doProfileX(nCSChits_vs_eta[w],h_CSChits_eta[w]);
      doProfileX(nRPChits_vs_eta[w],h_RPChits_eta[w]);
      if (useGEMs_) doProfileX(nGEMhits_vs_eta[w],h_GEMhits_eta[w]);
      if (useME0_) doProfileX(nME0hits_vs_eta[w],h_ME0hits_eta[w]);
      
      doProfileX(nlosthits_vs_eta[w],h_losthits_eta[w]);
      //vs phi
      doProfileX(chi2_vs_nhits[w],h_chi2meanhitsh[w]);
      doProfileX(chi2_vs_phi[w],h_chi2mean_vs_phi[w]);
      doProfileX(nhits_vs_phi[w],h_hits_phi[w]);

      fillPlotFromVector(h_recoeta[w],totRECeta[w]);
      fillPlotFromVector(h_recoetaBin1[w],totRECetaBin1[w]);
      fillPlotFromVector(h_recoetaBin2[w],totRECetaBin2[w]);
      fillPlotFromVector(h_recoetaBin3[w],totRECetaBin3[w]);
      fillPlotFromVector(h_recoetaBin4[w],totRECetaBin4[w]);
      fillPlotFromVector(h_recoetaPt10[w],totRECetaPt10[w]);
        
      fillPlotFromVector(h_simuleta[w],totSIMeta[w]);
      fillPlotFromVector(h_simuletaBin1[w],totSIMetaBin1[w]);
      fillPlotFromVector(h_simuletaBin2[w],totSIMetaBin2[w]);
      fillPlotFromVector(h_simuletaBin3[w],totSIMetaBin3[w]);
      fillPlotFromVector(h_simuletaBin4[w],totSIMetaBin4[w]);
      fillPlotFromVector(h_simuletaPt10[w],totSIMetaPt10[w]);
        
      fillPlotFromVector(h_simulvtx[w],totSIMvtx[w]);
      fillPlotFromVector(h_simulvtxBarrel[w],totSIMvtxBarrel[w]);
      fillPlotFromVector(h_simulvtxEndcap[w],totSIMvtxEndcap[w]);
      fillPlotFromVector(h_simulvtxOverlap[w],totSIMvtxOverlap[w]);
        
      fillPlotFromVector(h_assoceta[w],totASSeta[w]);
      fillPlotFromVector(h_assocetaBin1[w],totASSetaBin1[w]);
      fillPlotFromVector(h_assocetaBin2[w],totASSetaBin2[w]);
      fillPlotFromVector(h_assocetaBin3[w],totASSetaBin3[w]);
      fillPlotFromVector(h_assocetaBin4[w],totASSetaBin4[w]);
      fillPlotFromVector(h_assocetaPt10[w],totASSetaPt10[w]);
        
      fillPlotFromVector(h_assocvtx[w],totASSvtx[w]);
      fillPlotFromVector(h_assocvtxBarrel[w],totASSvtxBarrel[w]);
      fillPlotFromVector(h_assocvtxEndcap[w],totASSvtxEndcap[w]);
      fillPlotFromVector(h_assocvtxOverlap[w],totASSvtxOverlap[w]);
        
      fillPlotFromVector(h_assoc2eta[w],totASS2eta[w]);
      fillPlotFromVector(h_assoc2etaBin1[w],totASS2etaBin1[w]);
      fillPlotFromVector(h_assoc2etaBin2[w],totASS2etaBin2[w]);
      fillPlotFromVector(h_assoc2etaBin3[w],totASS2etaBin3[w]);
      fillPlotFromVector(h_assoc2etaBin4[w],totASS2etaBin4[w]);
      fillPlotFromVector(h_assoc2etaPt10[w],totASS2etaPt10[w]);
        
      fillPlotFromVector(h_assoc2eta_SgnMu[w],totASS2eta_SgnMu[w]);
      fillPlotFromVector(h_assoc2eta_SgnBkg[w],totASS2eta_SgnBkg[w]);
      fillPlotFromVector(h_assoc2eta_Pu[w],totASS2eta_Pu[w]);
      fillPlotFromVector(h_assoc2eta_PuIT[w],totASS2eta_PuIT[w]);
      fillPlotFromVector(h_assoc2eta_PuOOT[w],totASS2eta_PuOOT[w]);
      fillPlotFromVector(h_assoc2eta_PuMu[w],totASS2eta_PuMu[w]);
      fillPlotFromVector(h_assoc2eta_PuITMu[w],totASS2eta_PuITMu[w]);
      fillPlotFromVector(h_assoc2eta_PuOOTMu[w],totASS2eta_PuOOTMu[w]);

      fillPlotFromVector(h_recopT[w],totRECpT[w]);
      fillPlotFromVector(h_recopTBin1[w],totRECpTBin1[w]);
      fillPlotFromVector(h_recopTBin2[w],totRECpTBin2[w]);
      fillPlotFromVector(h_recopTBin3[w],totRECpTBin3[w]);
        
      fillPlotFromVector(h_simulpT[w],totSIMpT[w]);
      fillPlotFromVector(h_simulpTBin1[w],totSIMpTBin1[w]);
      fillPlotFromVector(h_simulpTBin2[w],totSIMpTBin2[w]);
      fillPlotFromVector(h_simulpTBin3[w],totSIMpTBin3[w]);
        
      fillPlotFromVector(h_assocpT[w],totASSpT[w]);
      fillPlotFromVector(h_assocpTBin1[w],totASSpTBin1[w]);
      fillPlotFromVector(h_assocpTBin2[w],totASSpTBin2[w]);
      fillPlotFromVector(h_assocpTBin3[w],totASSpTBin3[w]);
        
      fillPlotFromVector(h_assoc2pT[w],totASS2pT[w]);
        
      fillPlotFromVector(h_assoc2pT_SgnMu[w],totASS2pT_SgnMu[w]);
      fillPlotFromVector(h_assoc2pT_SgnBkg[w],totASS2pT_SgnBkg[w]);
      fillPlotFromVector(h_assoc2pT_Pu[w],totASS2pT_Pu[w]);
      fillPlotFromVector(h_assoc2pT_PuIT[w],totASS2pT_PuIT[w]);
      fillPlotFromVector(h_assoc2pT_PuOOT[w],totASS2pT_PuOOT[w]);
      fillPlotFromVector(h_assoc2pT_PuMu[w],totASS2pT_PuMu[w]);
      fillPlotFromVector(h_assoc2pT_PuITMu[w],totASS2pT_PuITMu[w]);
      fillPlotFromVector(h_assoc2pT_PuOOTMu[w],totASS2pT_PuOOTMu[w]);
        
      fillPlotFromVector(h_assoc2pTBin1[w],totASS2pTBin1[w]);
      fillPlotFromVector(h_assoc2pTBin2[w],totASS2pTBin2[w]);
      fillPlotFromVector(h_assoc2pTBin3[w],totASS2pTBin3[w]);

      fillPlotFromVector(h_recohit[w],totREC_hit[w]);
      fillPlotFromVector(h_simulhit[w],totSIM_hit[w]);
      fillPlotFromVector(h_assochit[w],totASS_hit[w]);
      fillPlotFromVector(h_assoc2hit[w],totASS2_hit[w]);

      fillPlotFromVector(h_recophi[w],totREC_phi[w]);
      fillPlotFromVector(h_simulphi[w],totSIM_phi[w]);
      fillPlotFromVector(h_assocphi[w],totASS_phi[w]);
      fillPlotFromVector(h_assoc2phi[w],totASS2_phi[w]);

      fillPlotFromVector(h_recodxy[w],totREC_dxy[w]);
      fillPlotFromVector(h_simuldxy[w],totSIM_dxy[w]);
      fillPlotFromVector(h_assocdxy[w],totASS_dxy[w]);
      fillPlotFromVector(h_assoc2dxy[w],totASS2_dxy[w]);

      fillPlotFromVector(h_recodz[w],totREC_dz[w]);
      fillPlotFromVector(h_simuldz[w],totSIM_dz[w]);
      fillPlotFromVector(h_assocdz[w],totASS_dz[w]);
      fillPlotFromVector(h_assoc2dz[w],totASS2_dz[w]);

      fillPlotFromVector(h_simulvertpos[w],totSIM_vertpos[w]);
      fillPlotFromVector(h_assocvertpos[w],totASS_vertpos[w]);

      fillPlotFromVector(h_simulzpos[w],totSIM_zpos[w]);
      fillPlotFromVector(h_assoczpos[w],totASS_zpos[w]);
        
      fillPlotFromVector(h_simulLxy[w],totSIM_lxy[w]);
      fillPlotFromVector(h_assocLxy[w],totASS_lxy[w]);
        
      fillPlotFromVector(h_simulLz[w],totSIM_lz[w]);
      fillPlotFromVector(h_assocLz[w],totASS_lz[w]);
        
      fillPlotFromVector(h_simulLr[w],totSIM_lr[w]);
      fillPlotFromVector(h_assocLr[w],totASS_lr[w]);
      
      if (MABH) {
	fillPlotFromVector(h_assoceta_Quality05[w] ,totASSeta_Quality05[w]);
	fillPlotFromVector(h_assoceta_Quality075[w],totASSeta_Quality075[w]);
	fillPlotFromVector(h_assocpT_Quality05[w] ,totASSpT_Quality05[w]);
	fillPlotFromVector(h_assocpT_Quality075[w],totASSpT_Quality075[w]);
	fillPlotFromVector(h_assocphi_Quality05[w] ,totASS_phi_Quality05[w]);
	fillPlotFromVector(h_assocphi_Quality075[w],totASS_phi_Quality075[w]);
    fillPlotFromVector(h_assoc2eta_075[w],totASS2eta_075[w]);
    fillPlotFromVector(h_assoc2eta_050[w],totASS2eta_050[w]);
    fillPlotFromVector(h_assoc2pT_075[w],totASS2pT_075[w]);
    fillPlotFromVector(h_assoc2pT_050[w],totASS2pT_050[w]);
      }
      
      w++;
    }
  }
  
  if ( out.size() != 0 && dbe_ ) dbe_->save(out);
}

void MuonTrackValidator::getRecoMomentum (const reco::Track& track,
					  double& pt, double& ptError, double& qoverp, double& qoverpError,
					  double& lambda,double& lambdaError, double& phi, double& phiError ) const {
  pt = track.pt();
  ptError = track.ptError();
  qoverp = track.qoverp();
  qoverpError = track.qoverpError();
  lambda = track.lambda();
  lambdaError = track.lambdaError();
  phi = track.phi();
  phiError = track.phiError();
}

void MuonTrackValidator::getRecoMomentum (const reco::GsfTrack& gsfTrack,
					  double& pt, double& ptError, double& qoverp, double& qoverpError,
					  double& lambda,double& lambdaError, double& phi, double& phiError ) const {
  pt = gsfTrack.ptMode();
  ptError = gsfTrack.ptModeError();
  qoverp = gsfTrack.qoverpMode();
  qoverpError = gsfTrack.qoverpModeError();
  lambda = gsfTrack.lambdaMode();
  lambdaError = gsfTrack.lambdaModeError();
  phi = gsfTrack.phiMode();
  phiError = gsfTrack.phiModeError(); 
}
