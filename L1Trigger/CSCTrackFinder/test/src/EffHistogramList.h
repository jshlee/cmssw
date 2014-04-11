
#ifndef jhugon_EffHistogramList_h
#define jhugon_EffHistogramList_h
// system include files
#include <vector>
#include <string>
#include <fstream>
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
#include <TLatex.h>
#include <TF1.h>
#include <TH2.h>
#include "L1Trigger/CSCTrackFinder/test/src/TrackHistogramList.h"
#include <TMath.h>

namespace csctf_analysis
{
  class  EffHistogramList
  {
    public:
	EffHistogramList(const std::string dirname, const edm::ParameterSet* parameters);
    	void ComputeEff(TrackHistogramList*);
    	void Print();
  	TH1F* EffPhi_mod_10_Q3_endcap1, *EffPhi_mod_10_Q2_endcap1;
  	TH1F* EffPhi_mod_10_Q3_endcap2, *EffPhi_mod_10_Q2_endcap2;
  	TH1F* modeOcc;
  	TH1F* EffEtaAll, *EffEtaQ3, *EffEtaQ2, *EffEtaQ1;
  	TH1F* EffSignedEtaAll, *EffSignedEtaQ3, *EffSignedEtaQ2, *EffSignedEtaQ1;
  	TH1F* EffPhiQ3, *EffPhiQ2, *EffPhiQ1, *EffPhi;
  	TH1F* EffPtOverall, *EffPtCSCOnly, *EffPtOverlap, *EffPtHighEta, *EffPtDTOnly, *EffPtCSCRestricted, *EffPtGEM;
	//std::vector<TH1F*> EffPtOverallQ, EffPtCSCOnlyQ, EffPtOverlapQ, EffPtHighEtaQ, EffPtDTOnlyQ, EffPtCSCRestrictedQ, EffPtGEMQ;
	TH1F* EffPtOverallQ1, *EffPtCSCOnlyQ1, *EffPtOverlapQ1, *EffPtHighEtaQ1, *EffPtDTOnlyQ1, *EffPtCSCRestrictedQ1, *EffPtGEMQ1;
	TH1F* EffPtOverallQ2, *EffPtCSCOnlyQ2, *EffPtOverlapQ2, *EffPtHighEtaQ2, *EffPtDTOnlyQ2, *EffPtCSCRestrictedQ2, *EffPtGEMQ2;
	TH1F* EffPtOverallQ3, *EffPtCSCOnlyQ3, *EffPtOverlapQ3, *EffPtHighEtaQ3, *EffPtDTOnlyQ3, *EffPtCSCRestrictedQ3, *EffPtGEMQ3;
  	TH1F* EffTFPt10Overall, *EffTFPt12Overall,*EffTFPt16Overall, *EffTFPt20Overall, *EffTFPt40Overall,*EffTFPt60Overall;
  	//std::vector<TH1F*> EffTFPt10OverallQ, EffTFPt12OverallQ, EffTFPt16OverallQ, EffTFPt20OverallQ, EffTFPt40OverallQ, EffTFPt60OverallQ;
  	TH1F* EffTFPt10OverallQ1, *EffTFPt12OverallQ1, *EffTFPt16OverallQ1, *EffTFPt20OverallQ1, *EffTFPt40OverallQ1, *EffTFPt60OverallQ1;
  	TH1F* EffTFPt10OverallQ2, *EffTFPt12OverallQ2, *EffTFPt16OverallQ2, *EffTFPt20OverallQ2, *EffTFPt40OverallQ2, *EffTFPt60OverallQ2;
  	TH1F* EffTFPt10OverallQ3, *EffTFPt12OverallQ3, *EffTFPt16OverallQ3, *EffTFPt20OverallQ3, *EffTFPt40OverallQ3, *EffTFPt60OverallQ3;
  	TH1F* EffTFPt10CSCOnly,*EffTFPt12CSCOnly,*EffTFPt16CSCOnly,*EffTFPt20CSCOnly, *EffTFPt40CSCOnly, *EffTFPt60CSCOnly;
  	//std::vector<TH1F*> EffTFPt10CSCOnlyQ, EffTFPt12CSCOnlyQ, EffTFPt16CSCOnlyQ, EffTFPt20CSCOnlyQ,  EffTFPt40CSCOnlyQ,  EffTFPt60CSCOnlyQ;
  	TH1F* EffTFPt10CSCOnlyQ1, *EffTFPt12CSCOnlyQ1, *EffTFPt16CSCOnlyQ1, *EffTFPt20CSCOnlyQ1,  *EffTFPt40CSCOnlyQ1,  *EffTFPt60CSCOnlyQ1;
  	TH1F* EffTFPt10CSCOnlyQ2, *EffTFPt12CSCOnlyQ2, *EffTFPt16CSCOnlyQ2, *EffTFPt20CSCOnlyQ2,  *EffTFPt40CSCOnlyQ2,  *EffTFPt60CSCOnlyQ2;
  	TH1F* EffTFPt10CSCOnlyQ3, *EffTFPt12CSCOnlyQ3, *EffTFPt16CSCOnlyQ3, *EffTFPt20CSCOnlyQ3,  *EffTFPt40CSCOnlyQ3,  *EffTFPt60CSCOnlyQ3;
  	TH1F* EffTFPt10GEM,*EffTFPt12GEM,*EffTFPt16GEM,*EffTFPt20GEM, *EffTFPt40GEM, *EffTFPt60GEM;
  	//std::vector<TH1F*>  EffTFPt10GEMQ, EffTFPt12GEMQ, EffTFPt16GEMQ, EffTFPt20GEMQ, EffTFPt40GEMQ, EffTFPt60GEMQ;
  	TH1F* EffTFPt10GEMQ1, *EffTFPt12GEMQ1, *EffTFPt16GEMQ1, *EffTFPt20GEMQ1, *EffTFPt40GEMQ1, *EffTFPt60GEMQ1;
  	TH1F* EffTFPt10GEMQ2, *EffTFPt12GEMQ2, *EffTFPt16GEMQ2, *EffTFPt20GEMQ2, *EffTFPt40GEMQ2, *EffTFPt60GEMQ2;
  	TH1F* EffTFPt10GEMQ3, *EffTFPt12GEMQ3, *EffTFPt16GEMQ3, *EffTFPt20GEMQ3, *EffTFPt40GEMQ3, *EffTFPt60GEMQ3;
  	TH1F* EffTFPt10CSCRestricted,*EffTFPt12CSCRestricted,*EffTFPt16CSCRestricted, *EffTFPt20CSCRestricted, *EffTFPt40CSCRestricted, *EffTFPt60CSCRestricted;
  	//std::vector<TH1F*> EffTFPt10CSCRestrictedQ,EffTFPt12CSCRestrictedQ,EffTFPt16CSCRestrictedQ, EffTFPt20CSCRestrictedQ, EffTFPt40CSCRestrictedQ, EffTFPt60CSCRestrictedQ;
  	TH1F* EffTFPt10CSCRestrictedQ1,*EffTFPt12CSCRestrictedQ1,*EffTFPt16CSCRestrictedQ1, *EffTFPt20CSCRestrictedQ1, *EffTFPt40CSCRestrictedQ1, *EffTFPt60CSCRestrictedQ1;
  	TH1F* EffTFPt10CSCRestrictedQ2,*EffTFPt12CSCRestrictedQ2,*EffTFPt16CSCRestrictedQ2, *EffTFPt20CSCRestrictedQ2, *EffTFPt40CSCRestrictedQ2, *EffTFPt60CSCRestrictedQ2;
  	TH1F* EffTFPt10CSCRestrictedQ3,*EffTFPt12CSCRestrictedQ3,*EffTFPt16CSCRestrictedQ3, *EffTFPt20CSCRestrictedQ3, *EffTFPt40CSCRestrictedQ3, *EffTFPt60CSCRestrictedQ3;
  	TH1F* EffTFPt10DTOnly,*EffTFPt12DTOnly,*EffTFPt16DTOnly, *EffTFPt20DTOnly, *EffTFPt40DTOnly, *EffTFPt60DTOnly;
  	//std::vector<TH1F*> EffTFPt10DTOnlyQ,EffTFPt12DTOnlyQ,EffTFPt16DTOnlyQ, EffTFPt20DTOnlyQ, EffTFPt40DTOnlyQ, EffTFPt60DTOnlyQ;
  	TH1F* EffTFPt10DTOnlyQ1,*EffTFPt12DTOnlyQ1,*EffTFPt16DTOnlyQ1, *EffTFPt20DTOnlyQ1, *EffTFPt40DTOnlyQ1, *EffTFPt60DTOnlyQ1;
  	TH1F* EffTFPt10DTOnlyQ2,*EffTFPt12DTOnlyQ2,*EffTFPt16DTOnlyQ2, *EffTFPt20DTOnlyQ2, *EffTFPt40DTOnlyQ2, *EffTFPt60DTOnlyQ2;
  	TH1F* EffTFPt10DTOnlyQ3,*EffTFPt12DTOnlyQ3,*EffTFPt16DTOnlyQ3, *EffTFPt20DTOnlyQ3, *EffTFPt40DTOnlyQ3, *EffTFPt60DTOnlyQ3;
  	TH1F* EffTFPt10Overlap,*EffTFPt12Overlap,*EffTFPt16Overlap, *EffTFPt20Overlap, *EffTFPt40Overlap, *EffTFPt60Overlap;
  	//std::vector<TH1F*> EffTFPt10OverlapQ,EffTFPt12OverlapQ,EffTFPt16OverlapQ, EffTFPt20OverlapQ, EffTFPt40OverlapQ, EffTFPt60OverlapQ;
  	TH1F* EffTFPt10OverlapQ1,*EffTFPt12OverlapQ1,*EffTFPt16OverlapQ1, *EffTFPt20OverlapQ1, *EffTFPt40OverlapQ1, *EffTFPt60OverlapQ1;
  	TH1F* EffTFPt10OverlapQ2,*EffTFPt12OverlapQ2,*EffTFPt16OverlapQ2, *EffTFPt20OverlapQ2, *EffTFPt40OverlapQ2, *EffTFPt60OverlapQ2;
  	TH1F* EffTFPt10OverlapQ3,*EffTFPt12OverlapQ3,*EffTFPt16OverlapQ3, *EffTFPt20OverlapQ3, *EffTFPt40OverlapQ3, *EffTFPt60OverlapQ3;
  	TH1F* EffTFPt10HighEta,*EffTFPt12HighEta,*EffTFPt16HighEta, *EffTFPt20HighEta, *EffTFPt40HighEta, *EffTFPt60HighEta;
  	//std::vector<TH1F*> EffTFPt10HighEtaQ,EffTFPt12HighEtaQ,EffTFPt16HighEtaQ, EffTFPt20HighEtaQ, EffTFPt40HighEtaQ, EffTFPt60HighEtaQ;
  	TH1F* EffTFPt10HighEtaQ1,*EffTFPt12HighEtaQ1,*EffTFPt16HighEtaQ1, *EffTFPt20HighEtaQ1, *EffTFPt40HighEtaQ1, *EffTFPt60HighEtaQ1;
  	TH1F* EffTFPt10HighEtaQ2,*EffTFPt12HighEtaQ2,*EffTFPt16HighEtaQ2, *EffTFPt20HighEtaQ2, *EffTFPt40HighEtaQ2, *EffTFPt60HighEtaQ2;
  	TH1F* EffTFPt10HighEtaQ3,*EffTFPt12HighEtaQ3,*EffTFPt16HighEtaQ3, *EffTFPt20HighEtaQ3, *EffTFPt40HighEtaQ3, *EffTFPt60HighEtaQ3;
  	TLegend* TrackerLeg1, *TrackerLeg2, *TrackerLeg3;
  	TLegend* TrackerLeg1CSCOnly, *TrackerLeg1Overlap, *TrackerLeg1HighEta, *TrackerLeg1DTOnly, *TrackerLeg1CSCRestricted, *TrackerLeg1Overall, *TrackerLeg1GEM;
  	TCanvas* PhiEff, *EtaEff, *SignedEtaEff, *PtEffAllOverall, *PtEffAllCSCOnly, *PtEffAllOverlap, *PtEffAllHighEta, *PtEffAllDTOnly, *PtEffAllCSCRestricted, *PtEffAllGEM;
  	TCanvas* PhiEffQ1, *EtaEffQ1, *SignedEtaEffQ1, *PtEffAllOverallQ1, *PtEffAllCSCOnlyQ1, *PtEffAllOverlapQ1, *PtEffAllHighEtaQ1, *PtEffAllDTOnlyQ1, *PtEffAllCSCRestrictedQ1, *PtEffAllGEMQ1;
  	TCanvas* PhiEffQ2, *EtaEffQ2, *SignedEtaEffQ2, *PtEffAllOverallQ2, *PtEffAllCSCOnlyQ2, *PtEffAllOverlapQ2, *PtEffAllHighEtaQ2, *PtEffAllDTOnlyQ2, *PtEffAllCSCRestrictedQ2, *PtEffAllGEMQ2;
  	TCanvas* PhiEffQ3, *EtaEffQ3, *SignedEtaEffQ3, *PtEffAllOverallQ3, *PtEffAllCSCOnlyQ3, *PtEffAllOverlapQ3, *PtEffAllHighEtaQ3, *PtEffAllDTOnlyQ3, *PtEffAllCSCRestrictedQ3, *PtEffAllGEMQ3;
	TF1* fitThreshOverall, *fitThreshCSCOnly,*fitThreshDTOnly, *fitThreshCSCRestricted, *fitThreshOverlap, *fitThreshHighEta, *fitThreshGEM;


    private:
	edm::Service<TFileService> fs;
	std::string PtEffStatsFilename;
	void DrawPtEffHists(std::string region, TCanvas* canvas, TF1* fit, TLegend* legend, std::vector<std::string> thresholds, std::vector<TH1F*> PtEffHists);
	void computeErrors(TrackHistogramList*);
	void divideHistograms(TrackHistogramList*);
	void computePtPlateauEff(std::ofstream* PtStats, std::vector<double> PlateauDefinitions, std::vector<std::string> thresholds, std::vector<TH1F*> PtEffHists);
	TLatex* latexDescription;
  };
  Double_t thresh(Double_t* pt, Double_t* par);
}
#endif
