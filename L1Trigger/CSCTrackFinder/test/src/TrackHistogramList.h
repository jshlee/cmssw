
#ifndef jhugon_TrackHistogramList_h
#define jhugon_TrackHistogramList_h
// system include files
#include <vector>
#include <string>
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include <TCanvas.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TF1.h>
#include <TH2.h>
namespace csctf_analysis
{
  class  TrackHistogramList
  {
    public:
	TrackHistogramList(const std::string dirname, const edm::ParameterSet* parameters);
  	TH2F* SimPt_vs_TFPt_FWD;	
  	TH2F* SimPt_vs_TFPt_DT;
  	TH1F* matchedRefPt_FWD;
  	TH1F* matchedRefPt_DT;
  	TH1F* modeOcc, *modeOccDT, *modeOccCSCOnly, *modeOccOverlap, *modeOccHighEta, *modeOccGEM;
  	TH1F* BX;
  	TH1F* FR;
  	TH1F* Eta, *signedEta, *Phi, *Phi_mod_10_endcap1,*Phi_mod_10_endcap2, *Pt, *Pz, *P, *Quality, *Radius;//,*EHalo;
  	TH1F* matchPhi, *matchPhi_mod_10_Q3_endcap1, *matchPhi_mod_10_Q2_endcap1, *matchPhi_mod_10_Q3_endcap2, *matchPhi_mod_10_Q2_endcap2;
  	TH1F* matchEta, *signedMatchEta, *matchPt, *matchRadius;//, *HaloPRes;
  	TH1F* EtaQ3, *EtaQ2, *EtaQ1;
  	TH1F* signedEtaQ3, *signedEtaQ2, *signedEtaQ1;
  	TH1F* PhiQ3, *PhiQ2, *PhiQ1;
  	TH1F* PtQ3, *PtQ2, *PtQ1;
  	TH1F* ghostPhi, *ghostEta, *ghostSignedEta, *ghostPt, *ghostRadius;//, *ghostPhiSector;
  	TH1F* ghostEtaQ3, *ghostEtaQ2, *ghostEtaQ1;
  	TH1F* ghostSignedEtaQ3, *ghostSignedEtaQ2, *ghostSignedEtaQ1;
  	TH1F* ghostPhiQ3, *ghostPhiQ2, *ghostPhiQ1;
  	TH1F* ghostPtQ3, *ghostPtQ2, *ghostPtQ1;
  	TH1F* ghostQuality;//, *ghostPhiSectorTrk;
  	TH1F* matchTFPt10Overall, *matchTFPt12Overall, *matchTFPt16Overall,*matchTFPt20Overall, *matchTFPt40Overall, *matchTFPt60Overall;
  	TH1F* matchTFPt10OverallQ1, *matchTFPt12OverallQ1, *matchTFPt16OverallQ1,*matchTFPt20OverallQ1, *matchTFPt40OverallQ1, *matchTFPt60OverallQ1;
  	TH1F* matchTFPt10OverallQ2, *matchTFPt12OverallQ2, *matchTFPt16OverallQ2,*matchTFPt20OverallQ2, *matchTFPt40OverallQ2, *matchTFPt60OverallQ2;
  	TH1F* matchTFPt10OverallQ3, *matchTFPt12OverallQ3, *matchTFPt16OverallQ3,*matchTFPt20OverallQ3, *matchTFPt40OverallQ3, *matchTFPt60OverallQ3;
  	TH1F* matchTFPt10CSCOnly,*matchTFPt12CSCOnly,*matchTFPt16CSCOnly, *matchTFPt20CSCOnly, *matchTFPt40CSCOnly, *matchTFPt60CSCOnly;
  	TH1F* matchTFPt10CSCOnlyQ1,*matchTFPt12CSCOnlyQ1,*matchTFPt16CSCOnlyQ1, *matchTFPt20CSCOnlyQ1, *matchTFPt40CSCOnlyQ1, *matchTFPt60CSCOnlyQ1;
  	TH1F* matchTFPt10CSCOnlyQ2,*matchTFPt12CSCOnlyQ2,*matchTFPt16CSCOnlyQ2, *matchTFPt20CSCOnlyQ2, *matchTFPt40CSCOnlyQ2, *matchTFPt60CSCOnlyQ2;
  	TH1F* matchTFPt10CSCOnlyQ3,*matchTFPt12CSCOnlyQ3,*matchTFPt16CSCOnlyQ3, *matchTFPt20CSCOnlyQ3, *matchTFPt40CSCOnlyQ3, *matchTFPt60CSCOnlyQ3;
  	TH1F* matchTFPt10GEM,*matchTFPt12GEM,*matchTFPt16GEM, *matchTFPt20GEM, *matchTFPt40GEM, *matchTFPt60GEM;
  	TH1F* matchTFPt10GEMQ1,*matchTFPt12GEMQ1,*matchTFPt16GEMQ1, *matchTFPt20GEMQ1, *matchTFPt40GEMQ1, *matchTFPt60GEMQ1;
  	TH1F* matchTFPt10GEMQ2,*matchTFPt12GEMQ2,*matchTFPt16GEMQ2, *matchTFPt20GEMQ2, *matchTFPt40GEMQ2, *matchTFPt60GEMQ2;
  	TH1F* matchTFPt10GEMQ3,*matchTFPt12GEMQ3,*matchTFPt16GEMQ3, *matchTFPt20GEMQ3, *matchTFPt40GEMQ3, *matchTFPt60GEMQ3;
  	TH1F* matchTFPt10CSCRestricted,*matchTFPt12CSCRestricted,*matchTFPt16CSCRestricted, *matchTFPt20CSCRestricted, *matchTFPt40CSCRestricted, *matchTFPt60CSCRestricted;
  	TH1F* matchTFPt10CSCRestrictedQ1,*matchTFPt12CSCRestrictedQ1,*matchTFPt16CSCRestrictedQ1, *matchTFPt20CSCRestrictedQ1, *matchTFPt40CSCRestrictedQ1, *matchTFPt60CSCRestrictedQ1;
  	TH1F* matchTFPt10CSCRestrictedQ2,*matchTFPt12CSCRestrictedQ2,*matchTFPt16CSCRestrictedQ2, *matchTFPt20CSCRestrictedQ2, *matchTFPt40CSCRestrictedQ2, *matchTFPt60CSCRestrictedQ2;
  	TH1F* matchTFPt10CSCRestrictedQ3,*matchTFPt12CSCRestrictedQ3,*matchTFPt16CSCRestrictedQ3, *matchTFPt20CSCRestrictedQ3, *matchTFPt40CSCRestrictedQ3, *matchTFPt60CSCRestrictedQ3;
  	TH1F* matchTFPt10Overlap,*matchTFPt12Overlap,*matchTFPt16Overlap, *matchTFPt20Overlap, *matchTFPt40Overlap, *matchTFPt60Overlap;
  	TH1F* matchTFPt10OverlapQ1,*matchTFPt12OverlapQ1,*matchTFPt16OverlapQ1, *matchTFPt20OverlapQ1, *matchTFPt40OverlapQ1, *matchTFPt60OverlapQ1;
  	TH1F* matchTFPt10OverlapQ2,*matchTFPt12OverlapQ2,*matchTFPt16OverlapQ2, *matchTFPt20OverlapQ2, *matchTFPt40OverlapQ2, *matchTFPt60OverlapQ2;
  	TH1F* matchTFPt10OverlapQ3,*matchTFPt12OverlapQ3,*matchTFPt16OverlapQ3, *matchTFPt20OverlapQ3, *matchTFPt40OverlapQ3, *matchTFPt60OverlapQ3;
  	TH1F* matchTFPt10HighEta,*matchTFPt12HighEta,*matchTFPt16HighEta, *matchTFPt20HighEta, *matchTFPt40HighEta, *matchTFPt60HighEta;
  	TH1F* matchTFPt10HighEtaQ1,*matchTFPt12HighEtaQ1,*matchTFPt16HighEtaQ1, *matchTFPt20HighEtaQ1, *matchTFPt40HighEtaQ1, *matchTFPt60HighEtaQ1;
  	TH1F* matchTFPt10HighEtaQ2,*matchTFPt12HighEtaQ2,*matchTFPt16HighEtaQ2, *matchTFPt20HighEtaQ2, *matchTFPt40HighEtaQ2, *matchTFPt60HighEtaQ2;
  	TH1F* matchTFPt10HighEtaQ3,*matchTFPt12HighEtaQ3,*matchTFPt16HighEtaQ3, *matchTFPt20HighEtaQ3, *matchTFPt40HighEtaQ3, *matchTFPt60HighEtaQ3;
  	TH1F* matchTFPt10DTOnly,*matchTFPt12DTOnly,*matchTFPt16DTOnly, *matchTFPt20DTOnly, *matchTFPt40DTOnly, *matchTFPt60DTOnly;
  	TH1F* matchTFPt10DTOnlyQ1,*matchTFPt12DTOnlyQ1,*matchTFPt16DTOnlyQ1, *matchTFPt20DTOnlyQ1, *matchTFPt40DTOnlyQ1, *matchTFPt60DTOnlyQ1;
  	TH1F* matchTFPt10DTOnlyQ2,*matchTFPt12DTOnlyQ2,*matchTFPt16DTOnlyQ2, *matchTFPt20DTOnlyQ2, *matchTFPt40DTOnlyQ2, *matchTFPt60DTOnlyQ2;
  	TH1F* matchTFPt10DTOnlyQ3,*matchTFPt12DTOnlyQ3,*matchTFPt16DTOnlyQ3, *matchTFPt20DTOnlyQ3, *matchTFPt40DTOnlyQ3, *matchTFPt60DTOnlyQ3;
  	TH1F* matchPtCSCOnly, *matchPtOverlap, *matchPtHighEta, *matchPtOverall, *matchPtDTOnly, *matchPtCSCRestricted, *matchPtGEM;
  	TH1F* matchPtCSCOnlyQ1, *matchPtOverlapQ1, *matchPtHighEtaQ1, *matchPtOverallQ1, *matchPtDTOnlyQ1, *matchPtCSCRestrictedQ1, *matchPtGEMQ1;
  	TH1F* matchPtCSCOnlyQ2, *matchPtOverlapQ2, *matchPtHighEtaQ2, *matchPtOverallQ2, *matchPtDTOnlyQ2, *matchPtCSCRestrictedQ2, *matchPtGEMQ2;
  	TH1F* matchPtCSCOnlyQ3, *matchPtOverlapQ3, *matchPtHighEtaQ3, *matchPtOverallQ3, *matchPtDTOnlyQ3, *matchPtCSCRestrictedQ3, *matchPtGEMQ3;
	TH1F* matchMode;
  	TH1F* fidPtDen,*ptDenOverall, *ptDenCSCOnly, *ptDenOverlap, *ptDenHighEta, *ptDenDTOnly, *ptDenCSCRestricted, *ptDenGEM;
	TH1F* rateHist;
	double getPtStep(){return ptStep;};
    private:
	edm::Service<TFileService> fs;
	double ptStep;
  };
}
#endif
