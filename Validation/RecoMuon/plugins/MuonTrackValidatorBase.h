#ifndef MuonTrackValidatorBase_h
#define MuonTrackValidatorBase_h

/** \class MuonTrackValidatorBase
* Base class for analyzers that produces histograms to validate Muon Track Reconstruction performances
*
*/

#include <memory>

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"

#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticleFwd.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include "DQMServices/Core/interface/MonitorElement.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "SimTracker/Common/interface/TrackingParticleSelector.h"
#include "CommonTools/RecoAlgos/interface/CosmicTrackingParticleSelector.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"

#include <iostream>
#include <sstream>
#include <string>
#include <TH1F.h>
#include <TH2F.h>

class DQMStore;
class MuonTrackValidatorBase {
 public:
  /// Constructor
  MuonTrackValidatorBase(const edm::ParameterSet& pset, edm::ConsumesCollector iC) : MuonTrackValidatorBase(pset)
    {
      bsSrc_Token = iC.consumes<reco::BeamSpot>(bsSrc);
      vtxTag_Token = iC.consumes<reco::VertexCollection>(vtxInputTag),
      tp_effic_Token = iC.consumes<TrackingParticleCollection>(label_tp_effic);
      tp_fake_Token = iC.consumes<TrackingParticleCollection>(label_tp_fake);
      for (unsigned int www=0;www<label.size();www++){
	track_Collection_Token[www] = iC.consumes<edm::View<reco::Track> >(label[www]);
      }
      PuInfo_Token = iC.consumes< std::vector< PileupSummaryInfo > >(edm::InputTag("addPileupInfo"));
      genP_Token = iC.consumes<reco::GenParticleCollection>(edm::InputTag("genParticles"));
    }

  MuonTrackValidatorBase(const edm::ParameterSet& pset):
    label(pset.getParameter< std::vector<edm::InputTag> >("label")),
    usetracker(pset.getParameter<bool>("usetracker")),
    usemuon(pset.getParameter<bool>("usemuon")),
    bsSrc(pset.getParameter< edm::InputTag >("beamSpot")),
    vtxInputTag(pset.getParameter<edm::InputTag>("vertexSrc")),
    label_tp_effic(pset.getParameter< edm::InputTag >("label_tp_effic")),
    label_tp_fake(pset.getParameter< edm::InputTag >("label_tp_fake")),
    associators(pset.getParameter< std::vector<std::string> >("associators")),
    out(pset.getParameter<std::string>("outputFile")),
    parametersDefiner(pset.getParameter<std::string>("parametersDefiner")),
    min(pset.getParameter<double>("min")),
    max(pset.getParameter<double>("max")),
    nint(pset.getParameter<int>("nint")),
    useFabs(pset.getParameter<bool>("useFabsEta")),
    
    minRes(pset.getParameter<double>("minRes")),
    maxRes(pset.getParameter<double>("maxRes")),
    nintRes(pset.getParameter<int>("nintRes")),
    
    minVtx(pset.getParameter<double>("minVtx")),
    maxVtx(pset.getParameter<double>("maxVtx")),
    nintVtx(pset.getParameter<int>("nintVtx")),
    
    minpT(pset.getParameter<double>("minpT")),
    maxpT(pset.getParameter<double>("maxpT")),
    nintpT(pset.getParameter<int>("nintpT")),
    minHit(pset.getParameter<double>("minHit")),
    maxHit(pset.getParameter<double>("maxHit")),
    nintHit(pset.getParameter<int>("nintHit")),
    minPhi(pset.getParameter<double>("minPhi")),
    maxPhi(pset.getParameter<double>("maxPhi")),
    nintPhi(pset.getParameter<int>("nintPhi")),
    minDxy(pset.getParameter<double>("minDxy")),
    maxDxy(pset.getParameter<double>("maxDxy")),
    nintDxy(pset.getParameter<int>("nintDxy")),
    minDz(pset.getParameter<double>("minDz")),
    maxDz(pset.getParameter<double>("maxDz")),
    nintDz(pset.getParameter<int>("nintDz")),
    minVertpos(pset.getParameter<double>("minVertpos")),
    maxVertpos(pset.getParameter<double>("maxVertpos")),
    nintVertpos(pset.getParameter<int>("nintVertpos")),
    minZpos(pset.getParameter<double>("minZpos")),
    maxZpos(pset.getParameter<double>("maxZpos")),
    nintZpos(pset.getParameter<int>("nintZpos")),
    
    minLxy(pset.getParameter<double>("minLxy")),
    maxLxy(pset.getParameter<double>("maxLxy")),
    nintLxy(pset.getParameter<int>("nintLxy")),
    minLz(pset.getParameter<double>("minLz")),
    maxLz(pset.getParameter<double>("maxLz")),
    nintLz(pset.getParameter<int>("nintLz")),
    minLr(pset.getParameter<double>("minLr")),
    maxLr(pset.getParameter<double>("maxLr")),
    nintLr(pset.getParameter<int>("nintLr")),
    
    useInvPt(pset.getParameter<bool>("useInvPt")),
    //
    ptRes_rangeMin(pset.getParameter<double>("ptRes_rangeMin")),
    ptRes_rangeMax(pset.getParameter<double>("ptRes_rangeMax")),
    phiRes_rangeMin(pset.getParameter<double>("phiRes_rangeMin")),
    phiRes_rangeMax(pset.getParameter<double>("phiRes_rangeMax")),
    cotThetaRes_rangeMin(pset.getParameter<double>("cotThetaRes_rangeMin")),
    cotThetaRes_rangeMax(pset.getParameter<double>("cotThetaRes_rangeMax")),
    dxyRes_rangeMin(pset.getParameter<double>("dxyRes_rangeMin")),
    dxyRes_rangeMax(pset.getParameter<double>("dxyRes_rangeMax")),
    dzRes_rangeMin(pset.getParameter<double>("dzRes_rangeMin")),
    dzRes_rangeMax(pset.getParameter<double>("dzRes_rangeMax")),
    ptRes_nbin(pset.getParameter<int>("ptRes_nbin")),
    cotThetaRes_nbin(pset.getParameter<int>("cotThetaRes_nbin")),
    phiRes_nbin(pset.getParameter<int>("phiRes_nbin")),
    dxyRes_nbin(pset.getParameter<int>("dxyRes_nbin")),
    dzRes_nbin(pset.getParameter<int>("dzRes_nbin")),
    ignoremissingtkcollection_(pset.getUntrackedParameter<bool>("ignoremissingtrackcollection",false)),
    useLogPt(pset.getUntrackedParameter<bool>("useLogPt",false))
    //
    {
      dbe_ = edm::Service<DQMStore>().operator->();
      if(useLogPt){
	maxpT=log10(maxpT);
	minpT=log10(minpT);
      }
    }
  
  /// Destructor
  virtual ~MuonTrackValidatorBase() noexcept(false) { }
  
  virtual void doProfileX(TH2 * th2, MonitorElement* me){
    if (th2->GetNbinsX()==me->getNbinsX()){
      TProfile * p1 = (TProfile*) th2->ProfileX();
      p1->Copy(*me->getTProfile());
      delete p1;
    } else {
      throw cms::Exception("MuonTrackValidator") << "Different number of bins!";
    }
  }

  virtual void doProfileX(MonitorElement * th2m, MonitorElement* me) {
    doProfileX(th2m->getTH2F(), me);
  }

  virtual double getEta(double eta) {
    if (useFabs) return fabs(eta);
    else return eta;
  }

  virtual double getPt(double pt) {
    if (useInvPt && pt!=0) return 1/pt;
    else return pt;
  }
  
  void fillPlotFromVector(MonitorElement* h, std::vector<int>& vec) {
    for (unsigned int j=0; j<vec.size(); j++){
      h->setBinContent(j+1, vec[j]);
    }
  }

  void fillPlotFromVectors(MonitorElement* h, std::vector<int>& numerator, std::vector<int>& denominator,std::string type){
    double value,err;
    for (unsigned int j=0; j<numerator.size(); j++){
      if (denominator[j]!=0){
if (type=="effic")
value = ((double) numerator[j])/((double) denominator[j]);
else if (type=="fakerate")
value = 1-((double) numerator[j])/((double) denominator[j]);
else return;
err = sqrt( value*(1-value)/(double) denominator[j] );
h->setBinContent(j+1, value);
h->setBinError(j+1,err);
      }
      else {
h->setBinContent(j+1, 0);
      }
    }
  }
    
  void extractCharge(MonitorElement* hIn, MonitorElement* h1, MonitorElement* h2){

	TH2F * plot2D = hIn->getTH2F();
  	for(int i = 1; i <= plot2D->GetNbinsX(); i++){

		double num = plot2D->GetBinContent(i,2);
		double den = plot2D->GetBinContent(i,2) + plot2D->GetBinContent(i,4);

		h1->setBinContent(i,num);
		h2->setBinContent(i,den);

  	}

  }

  void BinLogX(TH1*h)
  {
    
    TAxis *axis = h->GetXaxis();
    int bins = axis->GetNbins();
    
    float from = axis->GetXmin();
    float to = axis->GetXmax();
    float width = (to - from) / bins;
    float *new_bins = new float[bins + 1];
    
    for (int i = 0; i <= bins; i++) {
      new_bins[i] = TMath::Power(10, from + i * width);
      
    }
    axis->Set(bins, new_bins);
    delete[] new_bins;
  }

  void setUpVectors() {
    std::vector<double> etaintervalsv;
    std::vector<double> vtxintervalsv;
    std::vector<double> phiintervalsv;
    std::vector<double> pTintervalsv;
    std::vector<double> dxyintervalsv;
    std::vector<double> dzintervalsv;
    std::vector<double> vertposintervalsv;
    std::vector<double> zposintervalsv;
      
    std::vector<double> lxyintervalsv, lzintervalsv, lrintervalsv;
    std::vector<int>    totSIMvvtx,totASSvvtx,totASS2vvtx,totRECvvtx;
    std::vector<int>    totSIMvvtxBarrel,totASSvvtxBarrel,totASS2vvtxBarrel,totRECvvtxBarrel;
    std::vector<int>    totSIMvvtxEndcap,totASSvvtxEndcap,totASS2vvtxEndcap,totRECvvtxEndcap;
    std::vector<int>    totSIMvvtxOverlap,totASSvvtxOverlap,totASS2vvtxOverlap,totRECvvtxOverlap;
      
    std::vector<int> totSIMveta,totASSveta,totASS2veta,totRECveta;
    std::vector<int> totSIMvpT,totASSvpT,totASS2vpT,totRECvpT;
    std::vector<int> totSIMv_hit,totASSv_hit,totASS2v_hit,totRECv_hit;
    std::vector<int> totSIMv_phi,totASSv_phi,totASS2v_phi,totRECv_phi;
    std::vector<int> totSIMv_dxy,totASSv_dxy,totASS2v_dxy,totRECv_dxy;
    std::vector<int> totSIMv_dz,totASSv_dz,totASS2v_dz,totRECv_dz;
      
    std::vector<int>    totSIMv_vertpos,totASSv_vertpos,totSIMv_zpos,totASSv_zpos;
    std::vector<int>    totSIMv_lxy,totASSv_lxy,totSIMv_lz,totASSv_lz,totSIMv_lr,totASSv_lr;

    // for muon Validation
    std::vector<int> totASSveta_Quality05, totASSveta_Quality075;
    std::vector<int> totASSvpT_Quality05, totASSvpT_Quality075;
    std::vector<int> totASSv_phi_Quality05, totASSv_phi_Quality075;

    double step=(max-min)/nint;
    std::ostringstream title,name;
    etaintervalsv.push_back(min);
    for (int k=1;k<nint+1;k++) {
      double d=min+k*step;
      etaintervalsv.push_back(d);
      totSIMveta.push_back(0);
      totASSveta.push_back(0);
      totASS2veta.push_back(0);
      totRECveta.push_back(0);
      //
      totASSveta_Quality05.push_back(0);
      totASSveta_Quality075.push_back(0);
    }
    etaintervals.push_back(etaintervalsv);
    totSIMeta.push_back(totSIMveta);
    totSIMetaBin1.push_back(totSIMveta);
    totSIMetaBin2.push_back(totSIMveta);
    totSIMetaBin3.push_back(totSIMveta);
    totSIMetaBin4.push_back(totSIMveta);
    totSIMetaPt10.push_back(totSIMveta);
      
    totASSeta.push_back(totASSveta);
    totASSetaBin1.push_back(totASSveta);
    totASSetaBin2.push_back(totASSveta);
    totASSetaBin3.push_back(totASSveta);
    totASSetaBin4.push_back(totASSveta);
    totASSetaPt10.push_back(totASSveta);
      
    totASS2eta.push_back(totASS2veta);
    totASS2eta_SgnMu.push_back(totASS2veta);
    totASS2eta_SgnBkg.push_back(totASS2veta);
    totASS2eta_Pu.push_back(totASS2veta);
    totASS2eta_PuIT.push_back(totASS2veta);
    totASS2eta_PuOOT.push_back(totASS2veta);
    totASS2eta_PuMu.push_back(totASS2veta);
    totASS2eta_PuITMu.push_back(totASS2veta);
    totASS2eta_PuOOTMu.push_back(totASS2veta);
      
    totASS2eta_050.push_back(totASS2veta);
    totASS2eta_075.push_back(totASS2veta);
    totASS2etaBin1.push_back(totASS2veta);
    totASS2etaBin2.push_back(totASS2veta);
    totASS2etaBin3.push_back(totASS2veta);
    totASS2etaBin4.push_back(totASS2veta);
    totASS2etaPt10.push_back(totASS2veta);
      
    totRECeta.push_back(totRECveta);
    totRECetaBin1.push_back(totRECveta);
    totRECetaBin2.push_back(totRECveta);
    totRECetaBin3.push_back(totRECveta);
    totRECetaBin4.push_back(totRECveta);
    totRECetaPt10.push_back(totRECveta);
    //
    totASSeta_Quality05.push_back(totASSveta_Quality05);
    totASSeta_Quality075.push_back(totASSveta_Quality075);

    double stepVtx=(maxVtx-minVtx)/nintVtx;
    vtxintervalsv.push_back(minVtx);
    for (int k=1;k<nintVtx+1;k++) {
      double d=minVtx+k*stepVtx;
      vtxintervalsv.push_back(d);
      totSIMvvtx.push_back(0);
      totASSvvtx.push_back(0);
      totASS2vvtx.push_back(0);
      totRECvvtx.push_back(0);

      totSIMvvtxBarrel.push_back(0);
      totASSvvtxBarrel.push_back(0);
      totASS2vvtxBarrel.push_back(0);
      totRECvvtxBarrel.push_back(0);

      totSIMvvtxEndcap.push_back(0);
      totASSvvtxEndcap.push_back(0);
      totASS2vvtxEndcap.push_back(0);
      totRECvvtxEndcap.push_back(0);

      totSIMvvtxOverlap.push_back(0);
      totASSvvtxOverlap.push_back(0);
      totASS2vvtxOverlap.push_back(0);
      totRECvvtxOverlap.push_back(0);
      //
      totASSveta_Quality05.push_back(0);
      totASSveta_Quality075.push_back(0);
    }
    vtxintervals.push_back(vtxintervalsv);
    totSIMvtx.push_back(totSIMvvtx);
    totASSvtx.push_back(totASSvvtx);
    totASS2vtx.push_back(totASS2vvtx);
    totRECvtx.push_back(totRECvvtx);

    totSIMvtxBarrel.push_back(totSIMvvtxBarrel);
    totASSvtxBarrel.push_back(totASSvvtxBarrel);
    totASS2vtxBarrel.push_back(totASS2vvtxBarrel);
    totRECvtxBarrel.push_back(totRECvvtxBarrel);

    totSIMvtxEndcap.push_back(totSIMvvtxEndcap);
    totASSvtxEndcap.push_back(totASSvvtxEndcap);
    totASS2vtxEndcap.push_back(totASS2vvtxEndcap);
    totRECvtxEndcap.push_back(totRECvvtxEndcap);

    totSIMvtxOverlap.push_back(totSIMvvtxOverlap);
    totASSvtxOverlap.push_back(totASSvvtxOverlap);
    totASS2vtxOverlap.push_back(totASS2vvtxOverlap);
    totRECvtxOverlap.push_back(totRECvvtxOverlap);
      
    //
    totASSeta_Quality05.push_back(totASSveta_Quality05);
    totASSeta_Quality075.push_back(totASSveta_Quality075);
  
    double steppT = (maxpT-minpT)/nintpT;
    pTintervalsv.push_back(minpT);
    for (int k=1;k<nintpT+1;k++) {
      double d=0;
      if(useLogPt)d=pow(10,minpT+k*steppT);
      else d=minpT+k*steppT;
      pTintervalsv.push_back(d);
      totSIMvpT.push_back(0);
      totASSvpT.push_back(0);
      totASS2vpT.push_back(0);
      totRECvpT.push_back(0);
      //
      totASSvpT_Quality05.push_back(0);
      totASSvpT_Quality075.push_back(0);
    }
    pTintervals.push_back(pTintervalsv);
    totSIMpT.push_back(totSIMvpT);
    totSIMpTBin1.push_back(totSIMvpT);
    totSIMpTBin2.push_back(totSIMvpT);
    totSIMpTBin3.push_back(totSIMvpT);
      
    totASSpT.push_back(totASSvpT);
    totASSpTBin1.push_back(totASSvpT);
    totASSpTBin2.push_back(totASSvpT);
    totASSpTBin3.push_back(totASSvpT);
      
    totASS2pT.push_back(totASS2vpT);
    totASS2pT_050.push_back(totASS2vpT);
    totASS2pT_075.push_back(totASS2vpT);
    totASS2pTBin1.push_back(totASS2vpT);
    totASS2pTBin2.push_back(totASS2vpT);
    totASS2pTBin3.push_back(totASS2vpT);

    totASS2pT_SgnMu.push_back(totASS2vpT);
    totASS2pT_SgnBkg.push_back(totASS2vpT);
    totASS2pT_Pu.push_back(totASS2vpT);
    totASS2pT_PuIT.push_back(totASS2vpT);
    totASS2pT_PuOOT.push_back(totASS2vpT);
    totASS2pT_PuMu.push_back(totASS2vpT);
    totASS2pT_PuITMu.push_back(totASS2vpT);
    totASS2pT_PuOOTMu.push_back(totASS2vpT); 
  
      
    totRECpT.push_back(totRECvpT);
    totRECpTBin1.push_back(totRECvpT);
    totRECpTBin2.push_back(totRECvpT);
    totRECpTBin3.push_back(totRECvpT);
      
    //
    totASSpT_Quality05.push_back(totASSvpT_Quality05);
    totASSpT_Quality075.push_back(totASSvpT_Quality075);

    for (int k=1;k<nintHit+1;k++) {
      totSIMv_hit.push_back(0);
      totASSv_hit.push_back(0);
      totASS2v_hit.push_back(0);
      totRECv_hit.push_back(0);
    }
    totSIM_hit.push_back(totSIMv_hit);
    totASS_hit.push_back(totASSv_hit);
    totASS2_hit.push_back(totASS2v_hit);
    totREC_hit.push_back(totRECv_hit);

    double stepPhi = (maxPhi-minPhi)/nintPhi;
    phiintervalsv.push_back(minPhi);
    for (int k=1;k<nintPhi+1;k++) {
      double d=minPhi+k*stepPhi;
      phiintervalsv.push_back(d);
      totSIMv_phi.push_back(0);
      totASSv_phi.push_back(0);
      totASS2v_phi.push_back(0);
      totRECv_phi.push_back(0);
      //
      totASSv_phi_Quality05.push_back(0);
      totASSv_phi_Quality075.push_back(0);
    }
    phiintervals.push_back(phiintervalsv);
    totSIM_phi.push_back(totSIMv_phi);
    totASS_phi.push_back(totASSv_phi);
    totASS2_phi.push_back(totASS2v_phi);
    totREC_phi.push_back(totRECv_phi);
    //
    totASS_phi_Quality05.push_back(totASSv_phi_Quality05);
    totASS_phi_Quality075.push_back(totASSv_phi_Quality075);

    double stepDxy = (maxDxy-minDxy)/nintDxy;
    dxyintervalsv.push_back(minDxy);
    for (int k=1;k<nintDxy+1;k++) {
      double d=minDxy+k*stepDxy;
      dxyintervalsv.push_back(d);
      totSIMv_dxy.push_back(0);
      totASSv_dxy.push_back(0);
      totASS2v_dxy.push_back(0);
      totRECv_dxy.push_back(0);
    }
    dxyintervals.push_back(dxyintervalsv);
    totSIM_dxy.push_back(totSIMv_dxy);
    totASS_dxy.push_back(totASSv_dxy);
    totASS2_dxy.push_back(totASS2v_dxy);
    totREC_dxy.push_back(totRECv_dxy);


    double stepDz = (maxDz-minDz)/nintDz;
    dzintervalsv.push_back(minDz);
    for (int k=1;k<nintDz+1;k++) {
      double d=minDz+k*stepDz;
      dzintervalsv.push_back(d);
      totSIMv_dz.push_back(0);
      totASSv_dz.push_back(0);
      totASS2v_dz.push_back(0);
      totRECv_dz.push_back(0);
    }
    dzintervals.push_back(dzintervalsv);
    totSIM_dz.push_back(totSIMv_dz);
    totASS_dz.push_back(totASSv_dz);
    totASS2_dz.push_back(totASS2v_dz);
    totREC_dz.push_back(totRECv_dz);

    double stepVertpos = (maxVertpos-minVertpos)/nintVertpos;
    vertposintervalsv.push_back(minVertpos);
    for (int k=1;k<nintVertpos+1;k++) {
      double d=minVertpos+k*stepVertpos;
      vertposintervalsv.push_back(d);
      totSIMv_vertpos.push_back(0);
      totASSv_vertpos.push_back(0);
    }
    vertposintervals.push_back(vertposintervalsv);
    totSIM_vertpos.push_back(totSIMv_vertpos);
    totASS_vertpos.push_back(totASSv_vertpos);
      
    double stepZpos = (maxZpos-minZpos)/nintZpos;
    zposintervalsv.push_back(minZpos);
    for (int k=1;k<nintZpos+1;k++) {
      double d=minZpos+k*stepZpos;
      zposintervalsv.push_back(d);
      totSIMv_zpos.push_back(0);
      totASSv_zpos.push_back(0);
    }
    zposintervals.push_back(zposintervalsv);
    totSIM_zpos.push_back(totSIMv_zpos);
    totASS_zpos.push_back(totASSv_zpos);
      
    double stepLxy = (maxLxy-minLxy)/nintLxy;
    lxyintervalsv.push_back(minLxy);
    for (int k=1;k<nintLxy+1;k++) {
      double d=minLxy+k*stepLxy;
      lxyintervalsv.push_back(d);
      totSIMv_lxy.push_back(0);
      totASSv_lxy.push_back(0);
    }
    lxyintervals.push_back(lxyintervalsv);
    totSIM_lxy.push_back(totSIMv_lxy);
    totASS_lxy.push_back(totASSv_lxy);
      
    double stepLz = (maxLz-minLz)/nintLz;
    lzintervalsv.push_back(minLz);
    for (int k=1;k<nintLz+1;k++) {
      double d=minLz+k*stepLz;
      lzintervalsv.push_back(d);
      totSIMv_lz.push_back(0);
      totASSv_lz.push_back(0);
    }
    lzintervals.push_back(lzintervalsv);
    totSIM_lz.push_back(totSIMv_lz);
    totASS_lz.push_back(totASSv_lz);
      
    double stepLr = (maxLr-minLr)/nintLr;
    lrintervalsv.push_back(minLr);
    for (int k=1;k<nintLr+1;k++) {
      double d=minLr+k*stepLr;
      lrintervalsv.push_back(d);
      totSIMv_lr.push_back(0);
      totASSv_lr.push_back(0);
    }
    lrintervals.push_back(lrintervalsv);
    totSIM_lr.push_back(totSIMv_lr);
    totASS_lr.push_back(totASSv_lr);

  }

 protected:

  DQMStore* dbe_;

  std::vector<edm::InputTag> label;
  bool usetracker;
  bool usemuon;
  edm::InputTag bsSrc;
  edm::InputTag vtxInputTag;
  edm::InputTag label_tp_effic;
  edm::InputTag label_tp_fake;
  std::vector<std::string> associators;
  std::string out;
  std::string parametersDefiner;
  std::vector<edm::EDGetTokenT<edm::View<reco::Track> > > track_Collection_Token;
  edm::EDGetTokenT<reco::BeamSpot> bsSrc_Token;
  edm::EDGetTokenT<reco::VertexCollection> vtxTag_Token;
  edm::EDGetTokenT<TrackingParticleCollection> tp_effic_Token;
  edm::EDGetTokenT<TrackingParticleCollection> tp_fake_Token;
  edm::EDGetTokenT< std::vector< PileupSummaryInfo > > PuInfo_Token;
  edm::EDGetTokenT<reco::GenParticleCollection> genP_Token;
       
  double min, max;
  int nint;
  bool useFabs;
  double  minRes, maxRes;
  int nintRes;
  double  minVtx, maxVtx;
  int nintVtx;
  double minpT, maxpT;
  int nintpT;
  double minHit, maxHit;
  int nintHit;
  double minPhi, maxPhi;
  int nintPhi;
  double minDxy, maxDxy;
  int nintDxy;
  double minDz, maxDz;
  int nintDz;
  double minVertpos, maxVertpos;
  int nintVertpos;
  double minZpos, maxZpos;
  int nintZpos;
  double minLxy, maxLxy;
  int nintLxy;
  double minLz, maxLz;
  int nintLz;
  double minLr, maxLr;
  int nintLr;
  bool useInvPt;
  //
  double ptRes_rangeMin,ptRes_rangeMax,
    phiRes_rangeMin,phiRes_rangeMax, cotThetaRes_rangeMin,cotThetaRes_rangeMax,
    dxyRes_rangeMin,dxyRes_rangeMax, dzRes_rangeMin,dzRes_rangeMax;
  int ptRes_nbin, cotThetaRes_nbin, phiRes_nbin, dxyRes_nbin, dzRes_nbin;
  bool ignoremissingtkcollection_;
  bool useLogPt;

  edm::ESHandle<MagneticField> theMF;

  //sim
  std::vector<MonitorElement*> h_ptSIM, h_etaSIM, h_tracksSIM, h_vertposSIM;

  //1D
  std::vector<MonitorElement*> h_tracks, h_fakes, h_hits, h_charge;

  std::vector<MonitorElement*> h_assocpTvsEta, h_assocpTvsPhi;
  std::vector<MonitorElement*> h_simulpTvsEta, h_simulpTvsPhi;
  std::vector<MonitorElement*> h_numEvt, h_recoeta, h_assoc2eta, h_assoc2etaphi, h_assoc2etaBin1, h_assoc2etaBin2, h_assoc2etaBin3, h_assoc2etaBin4,h_assoc2etaPt10,h_assoc2eta_050, h_assoc2eta_075;

  std::vector<MonitorElement*> h_assoc2eta_SgnMu,h_assoc2eta_SgnBkg,h_assoc2eta_Pu,h_assoc2eta_PuIT,h_assoc2eta_PuOOT,h_assoc2eta_PuMu,h_assoc2eta_PuITMu,h_assoc2eta_PuOOTMu;

  std::vector<MonitorElement*> h_recoetaBin1, h_recoetaBin2, h_recoetaBin3, h_recoetaBin4, h_recoetaPt10;
  std::vector<MonitorElement*> h_assoceta, h_assocetaBin1, h_assocetaBin2, h_assocetaBin3, h_assocetaBin4, h_assocetaPt10;
  std::vector<MonitorElement*> h_simuleta, h_simuletaBin1, h_simuletaBin2, h_simuletaBin3, h_simuletaBin4, h_simuletaPt10;
  std::vector<MonitorElement*> h_recovtx, h_assocvtx, h_assoc2vtx, h_simulvtx, h_numSimVtx, h_numSimVtxBx, h_numRecVtx, h_numTrueInt;
  std::vector<MonitorElement*> h_recovtxBarrel, h_assocvtxBarrel, h_assoc2vtxBarrel, h_simulvtxBarrel;
  std::vector<MonitorElement*> h_recovtxEndcap, h_assocvtxEndcap, h_assoc2vtxEndcap, h_simulvtxEndcap;
  std::vector<MonitorElement*> h_recovtxOverlap, h_assocvtxOverlap, h_assoc2vtxOverlap, h_simulvtxOverlap;
  std::vector<MonitorElement*> h_recopT, h_assocpT, h_assoc2pT, h_assoc2pTBin1, h_assoc2pTBin2, h_assoc2pTBin3, h_simulpT, h_assoc2pT_050, h_assoc2pT_075;

  std::vector<MonitorElement*> h_assoc2pT_SgnMu,h_assoc2pT_SgnBkg,h_assoc2pT_Pu,h_assoc2pT_PuIT,h_assoc2pT_PuOOT,h_assoc2pT_PuMu,h_assoc2pT_PuITMu,h_assoc2pT_PuOOTMu;

  std::vector<MonitorElement*> h_recopTBin1, h_recopTBin2, h_recopTBin3;
  std::vector<MonitorElement*> h_assocpTBin1, h_assocpTBin2, h_assocpTBin3, h_simulpTBin1, h_simulpTBin2, h_simulpTBin3;
    
  std::vector<MonitorElement*> h_recohit, h_assochit, h_assoc2hit, h_simulhit;

  std::vector<MonitorElement*> h_recophi, h_recoetaphi, h_assocphi, h_assoc2phi, h_simulphi;
  std::vector<MonitorElement*> h_recodxy, h_dxyDistr, h_assocdxy, h_assoc2dxy, h_simuldxy;
  std::vector<MonitorElement*> h_recodz, h_dzDistr, h_assocdz, h_assoc2dz, h_simuldz;
    
  std::vector<MonitorElement*> h_assocvertpos, h_simulvertpos, h_assoczpos, h_simulzpos;

  std::vector<MonitorElement*> h_assocLxy, h_simulLxy, h_assocLz, h_simulLz,  h_assocLr, h_simulLr;
  std::vector<MonitorElement*> h_pt, h_eta, h_pullTheta,h_pullPhi,h_pullDxy,h_pullDz,h_pullQoverp, h_Qoverptres, h_invptres;
  std::vector<MonitorElement*> h_Qoverptres_sim, h_invptres_sim;
    
  std::vector<MonitorElement*> h_assoceta_Quality05, h_assoceta_Quality075;
  std::vector<MonitorElement*> h_assocpT_Quality05, h_assocpT_Quality075;
  std::vector<MonitorElement*> h_assocphi_Quality05, h_assocphi_Quality075;


  //2D
  std::vector<MonitorElement*> nrec_vs_nsim;
  std::vector<MonitorElement*> nrecHit_vs_nsimHit_sim2rec;
  std::vector<MonitorElement*> nrecHit_vs_nsimHit_rec2sim;

  //assoc hits
  std::vector<MonitorElement*> h_assocFraction, h_assocSharedHit;

  //#hit vs eta: to be used with doProfileX
  std::vector<MonitorElement*> nhits_vs_eta,nDThits_vs_eta,nCSChits_vs_eta,nRPChits_vs_eta,nGEMhits_vs_eta,nME0hits_vs_eta,n_trkLayersOld, n_pxlHitsOld, n_pxlHitsNew, n_pxlHitsNew43Inn, n_pxlHitsNew32Inn;
    
  std::vector<MonitorElement*> h_hits_eta,
    h_DThits_eta,h_CSChits_eta,h_RPChits_eta,h_GEMhits_eta,h_ME0hits_eta;
    

  std::vector< std::vector<double> > etaintervals;
  std::vector< std::vector<double> > vtxintervals;
  std::vector< std::vector<double> > pTintervals;
  std::vector< std::vector<double> > phiintervals;
  std::vector< std::vector<double> > dxyintervals;
  std::vector< std::vector<double> > dzintervals;
  std::vector< std::vector<double> > vertposintervals;
  std::vector< std::vector<double> > zposintervals;

  std::vector< std::vector<double> > lxyintervals, lzintervals, lrintervals;
  std::vector< std::vector<int> > totSIMeta,totRECeta,totASSeta,totASS2eta,totASS2etaBin1,totASS2etaBin2,totASS2etaBin3,totASS2etaBin4,totASS2etaPt10,totASS2eta_050, totASS2eta_075;
  std::vector< std::vector<int> > totRECetaBin1,totRECetaBin2,totRECetaBin3,totRECetaBin4,totRECetaPt10;

  std::vector< std::vector<int> > totASS2eta_SgnMu, totASS2eta_SgnBkg, totASS2eta_Pu, totASS2eta_PuIT, totASS2eta_PuOOT, totASS2eta_PuMu, totASS2eta_PuITMu, totASS2eta_PuOOTMu;

  std::vector< std::vector<int> > totSIMetaBin1,totSIMetaBin2,totSIMetaBin3,totSIMetaBin4,totSIMetaPt10,totASSetaBin1,totASSetaBin2,totASSetaBin3,totASSetaBin4,totASSetaPt10;
  std::vector< std::vector<int> > totSIMvtxBarrel,totRECvtxBarrel,totASSvtxBarrel,totASS2vtxBarrel;
  std::vector< std::vector<int> > totSIMvtxEndcap,totRECvtxEndcap,totASSvtxEndcap,totASS2vtxEndcap;
  std::vector< std::vector<int> > totSIMvtxOverlap,totRECvtxOverlap,totASSvtxOverlap,totASS2vtxOverlap;
  std::vector< std::vector<int> > totSIMvtx,totRECvtx,totASSvtx,totASS2vtx;
  std::vector< std::vector<int> > totSIMpT,totRECpT,totASSpT,totASS2pT,totASS2pTBin1,totASS2pTBin2,totASS2pTBin3, totASS2pT_050, totASS2pT_075;

  std::vector< std::vector<int> > totASS2pT_SgnMu, totASS2pT_SgnBkg, totASS2pT_Pu, totASS2pT_PuIT, totASS2pT_PuOOT, totASS2pT_PuMu, totASS2pT_PuITMu, totASS2pT_PuOOTMu; 

  std::vector< std::vector<int> > totRECpTBin1,totRECpTBin2,totRECpTBin3;
  std::vector< std::vector<int> > totSIMpTBin1,totSIMpTBin2,totSIMpTBin3,totASSpTBin1,totASSpTBin2,totASSpTBin3;
    
  std::vector< std::vector<int> > totSIM_hit,totREC_hit,totASS_hit,totASS2_hit;
  std::vector< std::vector<int> > totSIM_phi,totREC_phi,totASS_phi,totASS2_phi;
  std::vector< std::vector<int> > totSIM_dxy,totREC_dxy,totASS_dxy,totASS2_dxy;
  std::vector< std::vector<int> > totSIM_dz,totREC_dz,totASS_dz,totASS2_dz;
  std::vector< std::vector<int> > totSIM_vertpos,totASS_vertpos,totSIM_zpos,totASS_zpos;
    
  std::vector< std::vector<int> > totSIM_lxy,totASS_lxy,totSIM_lz,totASS_lz, totSIM_lr,totASS_lr;

  // for muon Validation (SimToReco distributions for Quality > 0.5, 0.75)
  std::vector<MonitorElement*> h_PurityVsQuality, h_quality, h_qualityVsEta;
  std::vector< std::vector<int> > totASSeta_Quality05,totASSeta_Quality075;
  std::vector< std::vector<int> > totASSpT_Quality05, totASSpT_Quality075;
  std::vector< std::vector<int> > totASS_phi_Quality05, totASS_phi_Quality075;

};


#endif
