#include "L1Trigger/CSCTrackFinder/test/src/EffHistogramList.h"
#include "L1Trigger/CSCTrackFinder/test/src/TrackHistogramList.h"
#include <TMath.h>
#include <iostream>

namespace csctf_analysis
{

EffHistogramList::EffHistogramList(const std::string dirname, const edm::ParameterSet* parameters)
{
	TFileDirectory dir = fs->mkdir(dirname);
	TFileDirectory ptSubdir = dir.mkdir("Pt_Efficiencies");
	TFileDirectory ptSubdirOverall = ptSubdir.mkdir("Overall");
	TFileDirectory ptSubdirOverallQ1 = ptSubdirOverall.mkdir("Q1");
	TFileDirectory ptSubdirOverallQ2 = ptSubdirOverall.mkdir("Q2");
	TFileDirectory ptSubdirOverallQ3 = ptSubdirOverall.mkdir("Q3");
	TFileDirectory ptSubdirCSCOnly = ptSubdir.mkdir("CSCOnly");
	TFileDirectory ptSubdirCSCOnlyQ1 = ptSubdirCSCOnly.mkdir("Q1");
	TFileDirectory ptSubdirCSCOnlyQ2 = ptSubdirCSCOnly.mkdir("Q2");
	TFileDirectory ptSubdirCSCOnlyQ3 = ptSubdirCSCOnly.mkdir("Q3");
	TFileDirectory ptSubdirCSCRestricted = ptSubdir.mkdir("CSCRestricted");
	TFileDirectory ptSubdirCSCRestrictedQ1 = ptSubdirCSCRestricted.mkdir("Q1");
	TFileDirectory ptSubdirCSCRestrictedQ2 = ptSubdirCSCRestricted.mkdir("Q2");
	TFileDirectory ptSubdirCSCRestrictedQ3 = ptSubdirCSCRestricted.mkdir("Q3");
	TFileDirectory ptSubdirDTOnly = ptSubdir.mkdir("DTOnly");
	TFileDirectory ptSubdirDTOnlyQ1 = ptSubdirDTOnly.mkdir("Q1");
	TFileDirectory ptSubdirDTOnlyQ2 = ptSubdirDTOnly.mkdir("Q2");
	TFileDirectory ptSubdirDTOnlyQ3 = ptSubdirDTOnly.mkdir("Q3");
	TFileDirectory ptSubdirOverlap = ptSubdir.mkdir("Overlap");
	TFileDirectory ptSubdirOverlapQ1 = ptSubdirOverlap.mkdir("Q1");
	TFileDirectory ptSubdirOverlapQ2 = ptSubdirOverlap.mkdir("Q2");
	TFileDirectory ptSubdirOverlapQ3 = ptSubdirOverlap.mkdir("Q3");
	TFileDirectory ptSubdirHighEta = ptSubdir.mkdir("HighEta");
	TFileDirectory ptSubdirHighEtaQ1 = ptSubdirHighEta.mkdir("Q1");
	TFileDirectory ptSubdirHighEtaQ2 = ptSubdirHighEta.mkdir("Q2");
	TFileDirectory ptSubdirHighEtaQ3 = ptSubdirHighEta.mkdir("Q3");
	TFileDirectory ptSubdirGEM = ptSubdir.mkdir("GEM");
	TFileDirectory ptSubdirGEMQ1 = ptSubdirGEM.mkdir("Q1");
	TFileDirectory ptSubdirGEMQ2 = ptSubdirGEM.mkdir("Q2");
	TFileDirectory ptSubdirGEMQ3 = ptSubdirGEM.mkdir("Q3");
	
	TFileDirectory etaSubdir = dir.mkdir("Eta_Efficiency");
	TFileDirectory phiSubdir = dir.mkdir("Phi_Efficiency");
    	
	double maxpt=parameters->getUntrackedParameter<double>("MaxPtHist");
	double minpt=parameters->getUntrackedParameter<double>("MinPtHist");
	int ptbins=parameters->getUntrackedParameter<double>("BinsPtHist");
	PtEffStatsFilename=parameters->getUntrackedParameter<std::string>("PtEffStatsFilename");
	
	std::string histoDescription = parameters->getUntrackedParameter<std::string>("HistoDescription");
	latexDescription = new TLatex(0.121,0.883,histoDescription.c_str());

	latexDescription->SetTextAlign(13);
	latexDescription->SetNDC();

    	EffPhi = phiSubdir.make<TH1F>("EffPhi","Efficiency v #phi",144,0,6.283);
    	EffPhi_mod_10_Q2_endcap1 = phiSubdir.make<TH1F>("EffPhi_mod_10_Q2_endcap1","Efficiency v #phi mod 10,Q>=2, Endcap 1",140,-2,12);
    	EffPhi_mod_10_Q3_endcap1 = phiSubdir.make<TH1F>("EffPhi_mod_10_Q3_endcap1","Efficiency v #phi mod 10,Q>=3, Endcap 1",140,-2,12);
    	EffPhi_mod_10_Q2_endcap2 = phiSubdir.make<TH1F>("EffPhi_mod_10_Q2_endcap2","Efficiency v #phi mod 10,Q>=2, Endcap 2",140,-2,12);
    	EffPhi_mod_10_Q3_endcap2 = phiSubdir.make<TH1F>("EffPhi_mod_10_Q3_endcap2","Efficiency v #phi mod 10,Q>=3, Endcap 2",140,-2,12);
    	//EffPt = dir.make<TH1F>("EffPt","Efficiency v Pt; 1.2 <= #eta <= 2.1",ptbins, minpt, maxpt);
    	EffPtDTOnly = ptSubdirDTOnly.make<TH1F>("EffPtDTOnly","Efficiency v Pt; 0<= #eta <=0.9",ptbins, minpt, maxpt);
    	EffPtCSCOnly = ptSubdirCSCOnly.make<TH1F>("EffPtCSCOnly","Efficiency v Pt; 1.2<= #eta <=2.4",ptbins, minpt, maxpt);
    	EffPtCSCRestricted = ptSubdirCSCRestricted.make<TH1F>("EffPtCSCRestricted","Efficiency v Pt; 1.2<= #eta <=2.1",ptbins, minpt, maxpt);
	EffPtOverall = ptSubdirOverall.make<TH1F>("EffPtOverall","Efficiency v Overall Pt",ptbins, minpt, maxpt);
    	EffPtOverlap = ptSubdirOverlap.make<TH1F>("EffPtOverlap","Efficiency v Pt; 0.9<= #eta <=1.2",ptbins, minpt, maxpt);
    	EffPtHighEta = ptSubdirHighEta.make<TH1F>("EffPtHighEta","Efficiency v Pt; 2.1<= #eta",ptbins, minpt, maxpt);
    	EffPtDTOnlyQ1 = ptSubdirDTOnlyQ1.make<TH1F>("EffPtDTOnlyQ1","Efficiency v Pt; 0<= #eta <=0.9,Q>=1",ptbins, minpt, maxpt);
    	EffPtCSCOnlyQ1 = ptSubdirCSCOnlyQ1.make<TH1F>("EffPtCSCOnlyQ1","Efficiency v Pt; 1.2<= #eta <=2.4,Q>=1",ptbins, minpt, maxpt);
    	EffPtCSCRestrictedQ1 = ptSubdirCSCRestrictedQ1.make<TH1F>("EffPtCSCRestrictedQ1","Efficiency v Pt; 1.2<= #eta <=2.1,Q>=1",ptbins, minpt, maxpt);
	EffPtOverallQ1 = ptSubdirOverallQ1.make<TH1F>("EffPtOverallQ1","Efficiency v Overall Pt,Q>=1",ptbins, minpt, maxpt);
    	EffPtOverlapQ1 = ptSubdirOverlapQ1.make<TH1F>("EffPtOverlapQ1","Efficiency v Pt; 0.9<= #eta <=1.2,Q>=1",ptbins, minpt, maxpt);
    	EffPtHighEtaQ1 = ptSubdirHighEtaQ1.make<TH1F>("EffPtHighEtaQ1","Efficiency v Pt; 2.1<= #eta,Q>=1",ptbins, minpt, maxpt);
    	EffPtDTOnlyQ2 = ptSubdirDTOnlyQ2.make<TH1F>("EffPtDTOnlyQ2","Efficiency v Pt; 0<= #eta <=0.9,Q>=2",ptbins, minpt, maxpt);
    	EffPtCSCOnlyQ2 = ptSubdirCSCOnlyQ2.make<TH1F>("EffPtCSCOnlyQ2","Efficiency v Pt; 1.2<= #eta <=2.4,Q>=2",ptbins, minpt, maxpt);
    	EffPtCSCRestrictedQ2 = ptSubdirCSCRestrictedQ2.make<TH1F>("EffPtCSCRestrictedQ2","Efficiency v Pt; 1.2<= #eta <=2.1,Q>=2",ptbins, minpt, maxpt);
	EffPtOverallQ2 = ptSubdirOverallQ2.make<TH1F>("EffPtOverallQ2","Efficiency v Overall Pt,Q>=2",ptbins, minpt, maxpt);
    	EffPtOverlapQ2 = ptSubdirOverlapQ2.make<TH1F>("EffPtOverlapQ2","Efficiency v Pt; 0.9<= #eta <=1.2,Q>=2",ptbins, minpt, maxpt);
    	EffPtHighEtaQ2 = ptSubdirHighEtaQ2.make<TH1F>("EffPtHighEtaQ2","Efficiency v Pt; 2.1<= #eta,Q>=2",ptbins, minpt, maxpt);
    	EffPtDTOnlyQ3 = ptSubdirDTOnlyQ3.make<TH1F>("EffPtDTOnlyQ3","Efficiency v Pt; 0<= #eta <=0.9,Q>=3",ptbins, minpt, maxpt);
    	EffPtCSCOnlyQ3 = ptSubdirCSCOnlyQ3.make<TH1F>("EffPtCSCOnlyQ3","Efficiency v Pt; 1.2<= #eta <=2.4,Q>=3",ptbins, minpt, maxpt);
    	EffPtCSCRestrictedQ3 = ptSubdirCSCRestrictedQ3.make<TH1F>("EffPtCSCRestrictedQ3","Efficiency v Pt; 1.2<= #eta <=2.1,Q>=3",ptbins, minpt, maxpt);
	EffPtOverallQ3 = ptSubdirOverallQ3.make<TH1F>("EffPtOverallQ3","Efficiency v Overall Pt,Q>=3",ptbins, minpt, maxpt);
    	EffPtOverlapQ3 = ptSubdirOverlapQ3.make<TH1F>("EffPtOverlapQ3","Efficiency v Pt; 0.9<= #eta <=1.2,Q>=3",ptbins, minpt, maxpt);
    	EffPtHighEtaQ3 = ptSubdirHighEtaQ3.make<TH1F>("EffPtHighEtaQ3","Efficiency v Pt; 2.1<= #eta,Q>=3",ptbins, minpt, maxpt);
    	EffTFPt10Overall = ptSubdirOverall.make<TH1F>("EffTFPt10Overall","Efficiency v Overall Pt Tf > 10",ptbins, minpt, maxpt);
	EffTFPt12Overall = ptSubdirOverall.make<TH1F>("EffTFPt12Overall","Efficiency v Overall Pt Tf > 12",ptbins, minpt, maxpt);
    	EffTFPt16Overall = ptSubdirOverall.make<TH1F>("EffTFPt16Overall","Efficiency v Overall Pt Tf > 16",ptbins, minpt, maxpt);
	EffTFPt20Overall = ptSubdirOverall.make<TH1F>("EffTFPt20Overall","Efficiency v Overall Pt Tf > 20",ptbins, minpt, maxpt);
    	EffTFPt40Overall = ptSubdirOverall.make<TH1F>("EffTFPt40Overall","Efficiency v Overall Pt Tf > 40",ptbins, minpt, maxpt);
    	EffTFPt60Overall = ptSubdirOverall.make<TH1F>("EffTFPt60Overall","Efficiency v Overall Pt Tf > 60",ptbins, minpt, maxpt);
    	EffTFPt10OverallQ1 = ptSubdirOverallQ1.make<TH1F>("EffTFPt10OverallQ1","Efficiency v Overall Pt Tf > 10,Q>=1",ptbins, minpt, maxpt);
	EffTFPt12OverallQ1 = ptSubdirOverallQ1.make<TH1F>("EffTFPt12OverallQ1","Efficiency v Overall Pt Tf > 12,Q>=1",ptbins, minpt, maxpt);
    	EffTFPt16OverallQ1 = ptSubdirOverallQ1.make<TH1F>("EffTFPt16OverallQ1","Efficiency v Overall Pt Tf > 16,Q>=1",ptbins, minpt, maxpt);
	EffTFPt20OverallQ1 = ptSubdirOverallQ1.make<TH1F>("EffTFPt20OverallQ1","Efficiency v Overall Pt Tf > 20,Q>=1",ptbins, minpt, maxpt);
    	EffTFPt40OverallQ1 = ptSubdirOverallQ1.make<TH1F>("EffTFPt40OverallQ1","Efficiency v Overall Pt Tf > 40,Q>=1",ptbins, minpt, maxpt);
    	EffTFPt60OverallQ1 = ptSubdirOverallQ1.make<TH1F>("EffTFPt60OverallQ1","Efficiency v Overall Pt Tf > 60,Q>=1",ptbins, minpt, maxpt);
    	EffTFPt10OverallQ2 = ptSubdirOverallQ2.make<TH1F>("EffTFPt10OverallQ2","Efficiency v Overall Pt Tf > 10,Q>=2",ptbins, minpt, maxpt);
	EffTFPt12OverallQ2 = ptSubdirOverallQ2.make<TH1F>("EffTFPt12OverallQ2","Efficiency v Overall Pt Tf > 12,Q>=2",ptbins, minpt, maxpt);
    	EffTFPt16OverallQ2 = ptSubdirOverallQ2.make<TH1F>("EffTFPt16OverallQ2","Efficiency v Overall Pt Tf > 16,Q>=2",ptbins, minpt, maxpt);
	EffTFPt20OverallQ2 = ptSubdirOverallQ2.make<TH1F>("EffTFPt20OverallQ2","Efficiency v Overall Pt Tf > 20,Q>=2",ptbins, minpt, maxpt);
    	EffTFPt40OverallQ2 = ptSubdirOverallQ2.make<TH1F>("EffTFPt40OverallQ2","Efficiency v Overall Pt Tf > 40,Q>=2",ptbins, minpt, maxpt);
    	EffTFPt60OverallQ2 = ptSubdirOverallQ2.make<TH1F>("EffTFPt60OverallQ2","Efficiency v Overall Pt Tf > 60,Q>=2",ptbins, minpt, maxpt);
    	EffTFPt10OverallQ3 = ptSubdirOverallQ3.make<TH1F>("EffTFPt10OverallQ3","Efficiency v Overall Pt Tf > 10,Q>=3",ptbins, minpt, maxpt);
	EffTFPt12OverallQ3 = ptSubdirOverallQ3.make<TH1F>("EffTFPt12OverallQ3","Efficiency v Overall Pt Tf > 12,Q>=3",ptbins, minpt, maxpt);
    	EffTFPt16OverallQ3 = ptSubdirOverallQ3.make<TH1F>("EffTFPt16OverallQ3","Efficiency v Overall Pt Tf > 16,Q>=3",ptbins, minpt, maxpt);
	EffTFPt20OverallQ3 = ptSubdirOverallQ3.make<TH1F>("EffTFPt20OverallQ3","Efficiency v Overall Pt Tf > 20,Q>=3",ptbins, minpt, maxpt);
    	EffTFPt40OverallQ3 = ptSubdirOverallQ3.make<TH1F>("EffTFPt40OverallQ3","Efficiency v Overall Pt Tf > 40,Q>=3",ptbins, minpt, maxpt);
    	EffTFPt60OverallQ3 = ptSubdirOverallQ3.make<TH1F>("EffTFPt60OverallQ3","Efficiency v Overall Pt Tf > 60,Q>=3",ptbins, minpt, maxpt);
    	EffTFPt10CSCOnly = ptSubdirCSCOnly.make<TH1F>("EffTFPt10CSCOnly","Efficiency v Pt Tf > 10; 1.2<= #eta <=2.4",ptbins, minpt, maxpt);
	EffTFPt12CSCOnly = ptSubdirCSCOnly.make<TH1F>("EffTFPt12CSCOnly","Efficiency v Pt Tf > 12; 1.2<= #eta <=2.4",ptbins, minpt, maxpt);
	EffTFPt16CSCOnly = ptSubdirCSCOnly.make<TH1F>("EffTFPt16CSCOnly","Efficiency v Pt Tf > 16; 1.2<= #eta <=2.4",ptbins, minpt, maxpt);
    	EffTFPt20CSCOnly = ptSubdirCSCOnly.make<TH1F>("EffTFPt20CSCOnly","Efficiency v Pt Tf > 20; 1.2<= #eta <=2.4",ptbins, minpt, maxpt);
    	EffTFPt40CSCOnly = ptSubdirCSCOnly.make<TH1F>("EffTFPt40CSCOnly","Efficiency v Pt Tf > 40; 1.2<= #eta <=2.4",ptbins, minpt, maxpt);
    	EffTFPt60CSCOnly = ptSubdirCSCOnly.make<TH1F>("EffTFPt60CSCOnly","Efficiency v Pt Tf > 60; 1.2<= #eta <=2.4",ptbins, minpt, maxpt);
    	EffTFPt10CSCOnlyQ1 = ptSubdirCSCOnlyQ1.make<TH1F>("EffTFPt10CSCOnlyQ1","Efficiency v Pt Tf > 10; 1.2<= #eta <=2.4,Q>=1",ptbins, minpt, maxpt);
	EffTFPt12CSCOnlyQ1 = ptSubdirCSCOnlyQ1.make<TH1F>("EffTFPt12CSCOnlyQ1","Efficiency v Pt Tf > 12; 1.2<= #eta <=2.4,Q>=1",ptbins, minpt, maxpt);
	EffTFPt16CSCOnlyQ1 = ptSubdirCSCOnlyQ1.make<TH1F>("EffTFPt16CSCOnlyQ1","Efficiency v Pt Tf > 16; 1.2<= #eta <=2.4,Q>=1",ptbins, minpt, maxpt);
    	EffTFPt20CSCOnlyQ1 = ptSubdirCSCOnlyQ1.make<TH1F>("EffTFPt20CSCOnlyQ1","Efficiency v Pt Tf > 20; 1.2<= #eta <=2.4,Q>=1",ptbins, minpt, maxpt);
    	EffTFPt40CSCOnlyQ1 = ptSubdirCSCOnlyQ1.make<TH1F>("EffTFPt40CSCOnlyQ1","Efficiency v Pt Tf > 40; 1.2<= #eta <=2.4,Q>=1",ptbins, minpt, maxpt);
    	EffTFPt60CSCOnlyQ1 = ptSubdirCSCOnlyQ1.make<TH1F>("EffTFPt60CSCOnlyQ1","Efficiency v Pt Tf > 60; 1.2<= #eta <=2.4,Q>=1",ptbins, minpt, maxpt);
    	EffTFPt10CSCOnlyQ2 = ptSubdirCSCOnlyQ2.make<TH1F>("EffTFPt10CSCOnlyQ2","Efficiency v Pt Tf > 10; 1.2<= #eta <=2.4,Q>=2",ptbins, minpt, maxpt);
	EffTFPt12CSCOnlyQ2 = ptSubdirCSCOnlyQ2.make<TH1F>("EffTFPt12CSCOnlyQ2","Efficiency v Pt Tf > 12; 1.2<= #eta <=2.4,Q>=2",ptbins, minpt, maxpt);
	EffTFPt16CSCOnlyQ2 = ptSubdirCSCOnlyQ2.make<TH1F>("EffTFPt16CSCOnlyQ2","Efficiency v Pt Tf > 16; 1.2<= #eta <=2.4,Q>=2",ptbins, minpt, maxpt);
    	EffTFPt20CSCOnlyQ2 = ptSubdirCSCOnlyQ2.make<TH1F>("EffTFPt20CSCOnlyQ2","Efficiency v Pt Tf > 20; 1.2<= #eta <=2.4,Q>=2",ptbins, minpt, maxpt);
    	EffTFPt40CSCOnlyQ2 = ptSubdirCSCOnlyQ2.make<TH1F>("EffTFPt40CSCOnlyQ2","Efficiency v Pt Tf > 40; 1.2<= #eta <=2.4,Q>=2",ptbins, minpt, maxpt);
    	EffTFPt60CSCOnlyQ2 = ptSubdirCSCOnlyQ2.make<TH1F>("EffTFPt60CSCOnlyQ2","Efficiency v Pt Tf > 60; 1.2<= #eta <=2.4,Q>=2",ptbins, minpt, maxpt);
    	EffTFPt10CSCOnlyQ3 = ptSubdirCSCOnlyQ3.make<TH1F>("EffTFPt10CSCOnlyQ3","Efficiency v Pt Tf > 10; 1.2<= #eta <=2.4,Q>=3",ptbins, minpt, maxpt);
	EffTFPt12CSCOnlyQ3 = ptSubdirCSCOnlyQ3.make<TH1F>("EffTFPt12CSCOnlyQ3","Efficiency v Pt Tf > 12; 1.2<= #eta <=2.4,Q>=3",ptbins, minpt, maxpt);
	EffTFPt16CSCOnlyQ3 = ptSubdirCSCOnlyQ3.make<TH1F>("EffTFPt16CSCOnlyQ3","Efficiency v Pt Tf > 16; 1.2<= #eta <=2.4,Q>=3",ptbins, minpt, maxpt);
    	EffTFPt20CSCOnlyQ3 = ptSubdirCSCOnlyQ3.make<TH1F>("EffTFPt20CSCOnlyQ3","Efficiency v Pt Tf > 20; 1.2<= #eta <=2.4,Q>=3",ptbins, minpt, maxpt);
    	EffTFPt40CSCOnlyQ3 = ptSubdirCSCOnlyQ3.make<TH1F>("EffTFPt40CSCOnlyQ3","Efficiency v Pt Tf > 40; 1.2<= #eta <=2.4,Q>=3",ptbins, minpt, maxpt);
    	EffTFPt60CSCOnlyQ3 = ptSubdirCSCOnlyQ3.make<TH1F>("EffTFPt60CSCOnlyQ3","Efficiency v Pt Tf > 60; 1.2<= #eta <=2.4,Q>=3",ptbins, minpt, maxpt);
    	EffTFPt10CSCRestricted = ptSubdirCSCRestricted.make<TH1F>("EffTFPt10CSCRestricted","Efficiency v Pt Tf > 10; 1.2<= #eta <=2.1",ptbins, minpt, maxpt);
	EffTFPt12CSCRestricted = ptSubdirCSCRestricted.make<TH1F>("EffTFPt12CSCRestricted","Efficiency v Pt Tf > 12; 1.2<= #eta <=2.1",ptbins, minpt, maxpt);
	EffTFPt16CSCRestricted = ptSubdirCSCRestricted.make<TH1F>("EffTFPt16CSCRestricted","Efficiency v Pt Tf > 16; 1.2<= #eta <=2.1",ptbins, minpt, maxpt);
    	EffTFPt20CSCRestricted = ptSubdirCSCRestricted.make<TH1F>("EffTFPt20CSCRestricted","Efficiency v Pt Tf > 20; 1.2<= #eta <=2.1",ptbins, minpt, maxpt);
    	EffTFPt40CSCRestricted = ptSubdirCSCRestricted.make<TH1F>("EffTFPt40CSCRestricted","Efficiency v Pt Tf > 40; 1.2<= #eta <=2.1",ptbins, minpt, maxpt);
    	EffTFPt60CSCRestricted = ptSubdirCSCRestricted.make<TH1F>("EffTFPt60CSCRestricted","Efficiency v Pt Tf > 60; 1.2<= #eta <=2.1",ptbins, minpt, maxpt);
    	EffTFPt10CSCRestrictedQ1 = ptSubdirCSCRestrictedQ1.make<TH1F>("EffTFPt10CSCRestrictedQ1","Efficiency v Pt Tf > 10; 1.2<= #eta <=2.1,Q>=1",ptbins, minpt, maxpt);
	EffTFPt12CSCRestrictedQ1 = ptSubdirCSCRestrictedQ1.make<TH1F>("EffTFPt12CSCRestrictedQ1","Efficiency v Pt Tf > 12; 1.2<= #eta <=2.1,Q>=1",ptbins, minpt, maxpt);
	EffTFPt16CSCRestrictedQ1 = ptSubdirCSCRestrictedQ1.make<TH1F>("EffTFPt16CSCRestrictedQ1","Efficiency v Pt Tf > 16; 1.2<= #eta <=2.1,Q>=1",ptbins, minpt, maxpt);
    	EffTFPt20CSCRestrictedQ1 = ptSubdirCSCRestrictedQ1.make<TH1F>("EffTFPt20CSCRestrictedQ1","Efficiency v Pt Tf > 20; 1.2<= #eta <=2.1,Q>=1",ptbins, minpt, maxpt);
    	EffTFPt40CSCRestrictedQ1 = ptSubdirCSCRestrictedQ1.make<TH1F>("EffTFPt40CSCRestrictedQ1","Efficiency v Pt Tf > 40; 1.2<= #eta <=2.1,Q>=1",ptbins, minpt, maxpt);
    	EffTFPt60CSCRestrictedQ1 = ptSubdirCSCRestrictedQ1.make<TH1F>("EffTFPt60CSCRestrictedQ1","Efficiency v Pt Tf > 60; 1.2<= #eta <=2.1,Q>=1",ptbins, minpt, maxpt);
    	EffTFPt10CSCRestrictedQ2 = ptSubdirCSCRestrictedQ2.make<TH1F>("EffTFPt10CSCRestrictedQ2","Efficiency v Pt Tf > 10; 1.2<= #eta <=2.1,Q>=2",ptbins, minpt, maxpt);
	EffTFPt12CSCRestrictedQ2 = ptSubdirCSCRestrictedQ2.make<TH1F>("EffTFPt12CSCRestrictedQ2","Efficiency v Pt Tf > 12; 1.2<= #eta <=2.1,Q>=2",ptbins, minpt, maxpt);
	EffTFPt16CSCRestrictedQ2 = ptSubdirCSCRestrictedQ2.make<TH1F>("EffTFPt16CSCRestrictedQ2","Efficiency v Pt Tf > 16; 1.2<= #eta <=2.1,Q>=2",ptbins, minpt, maxpt);
    	EffTFPt20CSCRestrictedQ2 = ptSubdirCSCRestrictedQ2.make<TH1F>("EffTFPt20CSCRestrictedQ2","Efficiency v Pt Tf > 20; 1.2<= #eta <=2.1,Q>=2",ptbins, minpt, maxpt);
    	EffTFPt40CSCRestrictedQ2 = ptSubdirCSCRestrictedQ2.make<TH1F>("EffTFPt40CSCRestrictedQ2","Efficiency v Pt Tf > 40; 1.2<= #eta <=2.1,Q>=2",ptbins, minpt, maxpt);
    	EffTFPt60CSCRestrictedQ2 = ptSubdirCSCRestrictedQ2.make<TH1F>("EffTFPt60CSCRestrictedQ2","Efficiency v Pt Tf > 60; 1.2<= #eta <=2.1,Q>=2",ptbins, minpt, maxpt);
    	EffTFPt10CSCRestrictedQ3 = ptSubdirCSCRestrictedQ3.make<TH1F>("EffTFPt10CSCRestrictedQ3","Efficiency v Pt Tf > 10; 1.2<= #eta <=2.1,Q>=3",ptbins, minpt, maxpt);
	EffTFPt12CSCRestrictedQ3 = ptSubdirCSCRestrictedQ3.make<TH1F>("EffTFPt12CSCRestrictedQ3","Efficiency v Pt Tf > 12; 1.2<= #eta <=2.1,Q>=3",ptbins, minpt, maxpt);
	EffTFPt16CSCRestrictedQ3 = ptSubdirCSCRestrictedQ3.make<TH1F>("EffTFPt16CSCRestrictedQ3","Efficiency v Pt Tf > 16; 1.2<= #eta <=2.1,Q>=3",ptbins, minpt, maxpt);
    	EffTFPt20CSCRestrictedQ3 = ptSubdirCSCRestrictedQ3.make<TH1F>("EffTFPt20CSCRestrictedQ3","Efficiency v Pt Tf > 20; 1.2<= #eta <=2.1,Q>=3",ptbins, minpt, maxpt);
    	EffTFPt40CSCRestrictedQ3 = ptSubdirCSCRestrictedQ3.make<TH1F>("EffTFPt40CSCRestrictedQ3","Efficiency v Pt Tf > 40; 1.2<= #eta <=2.1,Q>=3",ptbins, minpt, maxpt);
    	EffTFPt60CSCRestrictedQ3 = ptSubdirCSCRestrictedQ3.make<TH1F>("EffTFPt60CSCRestrictedQ3","Efficiency v Pt Tf > 60; 1.2<= #eta <=2.1,Q>=3",ptbins, minpt, maxpt);
    	EffTFPt10DTOnly = ptSubdirDTOnly.make<TH1F>("EffTFPt10DTOnly","Efficiency v Pt Tf > 10; 0<= #eta <=0.9",ptbins, minpt, maxpt);
	EffTFPt12DTOnly = ptSubdirDTOnly.make<TH1F>("EffTFPt12DTOnly","Efficiency v Pt Tf > 12; 0<= #eta <=0.9",ptbins, minpt, maxpt);
	EffTFPt16DTOnly = ptSubdirDTOnly.make<TH1F>("EffTFPt16DTOnly","Efficiency v Pt Tf > 16; 0<= #eta <=0.9",ptbins, minpt, maxpt);
    	EffTFPt20DTOnly = ptSubdirDTOnly.make<TH1F>("EffTFPt20DTOnly","Efficiency v Pt Tf > 20; 0<= #eta <=0.9",ptbins, minpt, maxpt);
    	EffTFPt40DTOnly = ptSubdirDTOnly.make<TH1F>("EffTFPt40DTOnly","Efficiency v Pt Tf > 40; 0<= #eta <=0.9",ptbins, minpt, maxpt);
    	EffTFPt60DTOnly = ptSubdirDTOnly.make<TH1F>("EffTFPt60DTOnly","Efficiency v Pt Tf > 60; 0<= #eta <=0.9",ptbins, minpt, maxpt);
    	EffTFPt10DTOnlyQ1 = ptSubdirDTOnlyQ1.make<TH1F>("EffTFPt10DTOnlyQ1","Efficiency v Pt Tf > 10; 0<= #eta <=0.9,Q>=1",ptbins, minpt, maxpt);
	EffTFPt12DTOnlyQ1 = ptSubdirDTOnlyQ1.make<TH1F>("EffTFPt12DTOnlyQ1","Efficiency v Pt Tf > 12; 0<= #eta <=0.9,Q>=1",ptbins, minpt, maxpt);
	EffTFPt16DTOnlyQ1 = ptSubdirDTOnlyQ1.make<TH1F>("EffTFPt16DTOnlyQ1","Efficiency v Pt Tf > 16; 0<= #eta <=0.9,Q>=1",ptbins, minpt, maxpt);
    	EffTFPt20DTOnlyQ1 = ptSubdirDTOnlyQ1.make<TH1F>("EffTFPt20DTOnlyQ1","Efficiency v Pt Tf > 20; 0<= #eta <=0.9,Q>=1",ptbins, minpt, maxpt);
    	EffTFPt40DTOnlyQ1 = ptSubdirDTOnlyQ1.make<TH1F>("EffTFPt40DTOnlyQ1","Efficiency v Pt Tf > 40; 0<= #eta <=0.9,Q>=1",ptbins, minpt, maxpt);
    	EffTFPt60DTOnlyQ1 = ptSubdirDTOnlyQ1.make<TH1F>("EffTFPt60DTOnlyQ1","Efficiency v Pt Tf > 60; 0<= #eta <=0.9,Q>=1",ptbins, minpt, maxpt);
    	EffTFPt10DTOnlyQ2 = ptSubdirDTOnlyQ2.make<TH1F>("EffTFPt10DTOnlyQ2","Efficiency v Pt Tf > 10; 0<= #eta <=0.9,Q>=2",ptbins, minpt, maxpt);
	EffTFPt12DTOnlyQ2 = ptSubdirDTOnlyQ2.make<TH1F>("EffTFPt12DTOnlyQ2","Efficiency v Pt Tf > 12; 0<= #eta <=0.9,Q>=2",ptbins, minpt, maxpt);
	EffTFPt16DTOnlyQ2 = ptSubdirDTOnlyQ2.make<TH1F>("EffTFPt16DTOnlyQ2","Efficiency v Pt Tf > 16; 0<= #eta <=0.9,Q>=2",ptbins, minpt, maxpt);
    	EffTFPt20DTOnlyQ2 = ptSubdirDTOnlyQ2.make<TH1F>("EffTFPt20DTOnlyQ2","Efficiency v Pt Tf > 20; 0<= #eta <=0.9,Q>=2",ptbins, minpt, maxpt);
    	EffTFPt40DTOnlyQ2 = ptSubdirDTOnlyQ2.make<TH1F>("EffTFPt40DTOnlyQ2","Efficiency v Pt Tf > 40; 0<= #eta <=0.9,Q>=2",ptbins, minpt, maxpt);
    	EffTFPt60DTOnlyQ2 = ptSubdirDTOnlyQ2.make<TH1F>("EffTFPt60DTOnlyQ2","Efficiency v Pt Tf > 60; 0<= #eta <=0.9,Q>=2",ptbins, minpt, maxpt);
    	EffTFPt10DTOnlyQ3 = ptSubdirDTOnlyQ3.make<TH1F>("EffTFPt10DTOnlyQ3","Efficiency v Pt Tf > 10; 0<= #eta <=0.9,Q>=3",ptbins, minpt, maxpt);
	EffTFPt12DTOnlyQ3 = ptSubdirDTOnlyQ3.make<TH1F>("EffTFPt12DTOnlyQ3","Efficiency v Pt Tf > 12; 0<= #eta <=0.9,Q>=3",ptbins, minpt, maxpt);
	EffTFPt16DTOnlyQ3 = ptSubdirDTOnlyQ3.make<TH1F>("EffTFPt16DTOnlyQ3","Efficiency v Pt Tf > 16; 0<= #eta <=0.9,Q>=3",ptbins, minpt, maxpt);
    	EffTFPt20DTOnlyQ3 = ptSubdirDTOnlyQ3.make<TH1F>("EffTFPt20DTOnlyQ3","Efficiency v Pt Tf > 20; 0<= #eta <=0.9,Q>=3",ptbins, minpt, maxpt);
    	EffTFPt40DTOnlyQ3 = ptSubdirDTOnlyQ3.make<TH1F>("EffTFPt40DTOnlyQ3","Efficiency v Pt Tf > 40; 0<= #eta <=0.9,Q>=3",ptbins, minpt, maxpt);
    	EffTFPt60DTOnlyQ3 = ptSubdirDTOnlyQ3.make<TH1F>("EffTFPt60DTOnlyQ3","Efficiency v Pt Tf > 60; 0<= #eta <=0.9,Q>=3",ptbins, minpt, maxpt);
    	EffTFPt10Overlap = ptSubdirOverlap.make<TH1F>("EffTFPt10Overlap","Efficiency v Pt Tf > 10; 0.9<= #eta <=1.2",ptbins, minpt, maxpt);
	EffTFPt12Overlap = ptSubdirOverlap.make<TH1F>("EffTFPt12Overlap","Efficiency v Pt Tf > 12; 0.9<= #eta <=1.2",ptbins, minpt, maxpt);
	EffTFPt16Overlap = ptSubdirOverlap.make<TH1F>("EffTFPt16Overlap","Efficiency v Pt Tf > 16; 0.9<= #eta <=1.2",ptbins, minpt, maxpt);
    	EffTFPt20Overlap = ptSubdirOverlap.make<TH1F>("EffTFPt20Overlap","Efficiency v Pt Tf > 20; 0.9<= #eta <=1.2",ptbins, minpt, maxpt);
    	EffTFPt40Overlap = ptSubdirOverlap.make<TH1F>("EffTFPt40Overlap","Efficiency v Pt Tf > 40; 0.9<= #eta <=1.2",ptbins, minpt, maxpt);
    	EffTFPt60Overlap = ptSubdirOverlap.make<TH1F>("EffTFPt60Overlap","Efficiency v Pt Tf > 60; 0.9<= #eta <=1.2",ptbins, minpt, maxpt);
    	EffTFPt10OverlapQ1 = ptSubdirOverlapQ1.make<TH1F>("EffTFPt10OverlapQ1","Efficiency v Pt Tf > 10; 0.9<= #eta <=1.2,Q>=1",ptbins, minpt, maxpt);
	EffTFPt12OverlapQ1 = ptSubdirOverlapQ1.make<TH1F>("EffTFPt12OverlapQ1","Efficiency v Pt Tf > 12; 0.9<= #eta <=1.2,Q>=1",ptbins, minpt, maxpt);
	EffTFPt16OverlapQ1 = ptSubdirOverlapQ1.make<TH1F>("EffTFPt16OverlapQ1","Efficiency v Pt Tf > 16; 0.9<= #eta <=1.2,Q>=1",ptbins, minpt, maxpt);
    	EffTFPt20OverlapQ1 = ptSubdirOverlapQ1.make<TH1F>("EffTFPt20OverlapQ1","Efficiency v Pt Tf > 20; 0.9<= #eta <=1.2,Q>=1",ptbins, minpt, maxpt);
    	EffTFPt40OverlapQ1 = ptSubdirOverlapQ1.make<TH1F>("EffTFPt40OverlapQ1","Efficiency v Pt Tf > 40; 0.9<= #eta <=1.2,Q>=1",ptbins, minpt, maxpt);
    	EffTFPt60OverlapQ1 = ptSubdirOverlapQ1.make<TH1F>("EffTFPt60OverlapQ1","Efficiency v Pt Tf > 60; 0.9<= #eta <=1.2,Q>=1",ptbins, minpt, maxpt);
    	EffTFPt10OverlapQ2 = ptSubdirOverlapQ2.make<TH1F>("EffTFPt10OverlapQ2","Efficiency v Pt Tf > 10; 0.9<= #eta <=1.2,Q>=2",ptbins, minpt, maxpt);
	EffTFPt12OverlapQ2 = ptSubdirOverlapQ2.make<TH1F>("EffTFPt12OverlapQ2","Efficiency v Pt Tf > 12; 0.9<= #eta <=1.2,Q>=2",ptbins, minpt, maxpt);
	EffTFPt16OverlapQ2 = ptSubdirOverlapQ2.make<TH1F>("EffTFPt16OverlapQ2","Efficiency v Pt Tf > 16; 0.9<= #eta <=1.2,Q>=2",ptbins, minpt, maxpt);
    	EffTFPt20OverlapQ2 = ptSubdirOverlapQ2.make<TH1F>("EffTFPt20OverlapQ2","Efficiency v Pt Tf > 20; 0.9<= #eta <=1.2,Q>=2",ptbins, minpt, maxpt);
    	EffTFPt40OverlapQ2 = ptSubdirOverlapQ2.make<TH1F>("EffTFPt40OverlapQ2","Efficiency v Pt Tf > 40; 0.9<= #eta <=1.2,Q>=2",ptbins, minpt, maxpt);
    	EffTFPt60OverlapQ2 = ptSubdirOverlapQ2.make<TH1F>("EffTFPt60OverlapQ2","Efficiency v Pt Tf > 60; 0.9<= #eta <=1.2,Q>=2",ptbins, minpt, maxpt);
    	EffTFPt10OverlapQ3 = ptSubdirOverlapQ3.make<TH1F>("EffTFPt10OverlapQ3","Efficiency v Pt Tf > 10; 0.9<= #eta <=1.2,Q>=3",ptbins, minpt, maxpt);
	EffTFPt12OverlapQ3 = ptSubdirOverlapQ3.make<TH1F>("EffTFPt12OverlapQ3","Efficiency v Pt Tf > 12; 0.9<= #eta <=1.2,Q>=3",ptbins, minpt, maxpt);
	EffTFPt16OverlapQ3 = ptSubdirOverlapQ3.make<TH1F>("EffTFPt16OverlapQ3","Efficiency v Pt Tf > 16; 0.9<= #eta <=1.2,Q>=3",ptbins, minpt, maxpt);
    	EffTFPt20OverlapQ3 = ptSubdirOverlapQ3.make<TH1F>("EffTFPt20OverlapQ3","Efficiency v Pt Tf > 20; 0.9<= #eta <=1.2,Q>=3",ptbins, minpt, maxpt);
    	EffTFPt40OverlapQ3 = ptSubdirOverlapQ3.make<TH1F>("EffTFPt40OverlapQ3","Efficiency v Pt Tf > 40; 0.9<= #eta <=1.2,Q>=3",ptbins, minpt, maxpt);
    	EffTFPt60OverlapQ3 = ptSubdirOverlapQ3.make<TH1F>("EffTFPt60OverlapQ3","Efficiency v Pt Tf > 60; 0.9<= #eta <=1.2,Q>=3",ptbins, minpt, maxpt);


    	EffTFPt10HighEta = ptSubdirHighEta.make<TH1F>("EffTFPt10HighEta","Efficiency v Pt Tf > 10; 2.1<= #eta",ptbins, minpt, maxpt);
	EffTFPt12HighEta = ptSubdirHighEta.make<TH1F>("EffTFPt12HighEta","Efficiency v Pt Tf > 12; 2.1<= #eta",ptbins, minpt, maxpt);
	EffTFPt16HighEta = ptSubdirHighEta.make<TH1F>("EffTFPt16HighEta","Efficiency v Pt Tf > 16; 2.1<= #eta",ptbins, minpt, maxpt);
    	EffTFPt20HighEta = ptSubdirHighEta.make<TH1F>("EffTFPt20HighEta","Efficiency v Pt Tf > 20; 2.1<= #eta",ptbins, minpt, maxpt);
    	EffTFPt40HighEta = ptSubdirHighEta.make<TH1F>("EffTFPt40HighEta","Efficiency v Pt Tf > 40; 2.1<= #eta",ptbins, minpt, maxpt);
    	EffTFPt60HighEta = ptSubdirHighEta.make<TH1F>("EffTFPt60HighEta","Efficiency v Pt Tf > 60; 2.1<= #eta",ptbins, minpt, maxpt);
    	EffTFPt10HighEtaQ1 = ptSubdirHighEtaQ1.make<TH1F>("EffTFPt10HighEtaQ1","Efficiency v Pt Tf > 10; 2.1<= #eta,Q>=1",ptbins, minpt, maxpt);
	EffTFPt12HighEtaQ1 = ptSubdirHighEtaQ1.make<TH1F>("EffTFPt12HighEtaQ1","Efficiency v Pt Tf > 12; 2.1<= #eta,Q>=1",ptbins, minpt, maxpt);
	EffTFPt16HighEtaQ1 = ptSubdirHighEtaQ1.make<TH1F>("EffTFPt16HighEtaQ1","Efficiency v Pt Tf > 16; 2.1<= #eta,Q>=1",ptbins, minpt, maxpt);
    	EffTFPt20HighEtaQ1 = ptSubdirHighEtaQ1.make<TH1F>("EffTFPt20HighEtaQ1","Efficiency v Pt Tf > 20; 2.1<= #eta,Q>=1",ptbins, minpt, maxpt);
    	EffTFPt40HighEtaQ1 = ptSubdirHighEtaQ1.make<TH1F>("EffTFPt40HighEtaQ1","Efficiency v Pt Tf > 40; 2.1<= #eta,Q>=1",ptbins, minpt, maxpt);
    	EffTFPt60HighEtaQ1 = ptSubdirHighEtaQ1.make<TH1F>("EffTFPt60HighEtaQ1","Efficiency v Pt Tf > 60; 2.1<= #eta,Q>=1",ptbins, minpt, maxpt);
    	EffTFPt10HighEtaQ2 = ptSubdirHighEtaQ2.make<TH1F>("EffTFPt10HighEtaQ2","Efficiency v Pt Tf > 10; 2.1<= #eta,Q>=2",ptbins, minpt, maxpt);
	EffTFPt12HighEtaQ2 = ptSubdirHighEtaQ2.make<TH1F>("EffTFPt12HighEtaQ2","Efficiency v Pt Tf > 12; 2.1<= #eta,Q>=2",ptbins, minpt, maxpt);
	EffTFPt16HighEtaQ2 = ptSubdirHighEtaQ2.make<TH1F>("EffTFPt16HighEtaQ2","Efficiency v Pt Tf > 16; 2.1<= #eta,Q>=2",ptbins, minpt, maxpt);
    	EffTFPt20HighEtaQ2 = ptSubdirHighEtaQ2.make<TH1F>("EffTFPt20HighEtaQ2","Efficiency v Pt Tf > 20; 2.1<= #eta,Q>=2",ptbins, minpt, maxpt);
    	EffTFPt40HighEtaQ2 = ptSubdirHighEtaQ2.make<TH1F>("EffTFPt40HighEtaQ2","Efficiency v Pt Tf > 40; 2.1<= #eta,Q>=2",ptbins, minpt, maxpt);
    	EffTFPt60HighEtaQ2 = ptSubdirHighEtaQ2.make<TH1F>("EffTFPt60HighEtaQ2","Efficiency v Pt Tf > 60; 2.1<= #eta,Q>=2",ptbins, minpt, maxpt);
    	EffTFPt10HighEtaQ3 = ptSubdirHighEtaQ3.make<TH1F>("EffTFPt10HighEtaQ3","Efficiency v Pt Tf > 10; 2.1<= #eta,Q>=3",ptbins, minpt, maxpt);
	EffTFPt12HighEtaQ3 = ptSubdirHighEtaQ3.make<TH1F>("EffTFPt12HighEtaQ3","Efficiency v Pt Tf > 12; 2.1<= #eta,Q>=3",ptbins, minpt, maxpt);
	EffTFPt16HighEtaQ3 = ptSubdirHighEtaQ3.make<TH1F>("EffTFPt16HighEtaQ3","Efficiency v Pt Tf > 16; 2.1<= #eta,Q>=3",ptbins, minpt, maxpt);
    	EffTFPt20HighEtaQ3 = ptSubdirHighEtaQ3.make<TH1F>("EffTFPt20HighEtaQ3","Efficiency v Pt Tf > 20; 2.1<= #eta,Q>=3",ptbins, minpt, maxpt);
    	EffTFPt40HighEtaQ3 = ptSubdirHighEtaQ3.make<TH1F>("EffTFPt40HighEtaQ3","Efficiency v Pt Tf > 40; 2.1<= #eta,Q>=3",ptbins, minpt, maxpt);
    	EffTFPt60HighEtaQ3 = ptSubdirHighEtaQ3.make<TH1F>("EffTFPt60HighEtaQ3","Efficiency v Pt Tf > 60; 2.1<= #eta,Q>=3",ptbins, minpt, maxpt);

    	EffPtGEM = ptSubdirGEM.make<TH1F>("EffPtGEM","Efficiency v Pt; 1.2<= #eta <=2.4",ptbins, minpt, maxpt);
    	EffPtGEMQ1 = ptSubdirGEMQ1.make<TH1F>("EffPtGEMQ1","Efficiency v Pt; 1.2<= #eta <=2.4,Q>=1",ptbins, minpt, maxpt);
    	EffPtGEMQ2 = ptSubdirGEMQ2.make<TH1F>("EffPtGEMQ2","Efficiency v Pt; 1.2<= #eta <=2.4,Q>=2",ptbins, minpt, maxpt);
    	EffPtGEMQ3 = ptSubdirGEMQ3.make<TH1F>("EffPtGEMQ3","Efficiency v Pt; 1.2<= #eta <=2.4,Q>=3",ptbins, minpt, maxpt);
    	EffTFPt10GEM = ptSubdirGEM.make<TH1F>("EffTFPt10GEM","Efficiency v Pt Tf > 10; 1.2<= #eta <=2.4",ptbins, minpt, maxpt);
	EffTFPt12GEM = ptSubdirGEM.make<TH1F>("EffTFPt12GEM","Efficiency v Pt Tf > 12; 1.2<= #eta <=2.4",ptbins, minpt, maxpt);
	EffTFPt16GEM = ptSubdirGEM.make<TH1F>("EffTFPt16GEM","Efficiency v Pt Tf > 16; 1.2<= #eta <=2.4",ptbins, minpt, maxpt);
    	EffTFPt20GEM = ptSubdirGEM.make<TH1F>("EffTFPt20GEM","Efficiency v Pt Tf > 20; 1.2<= #eta <=2.4",ptbins, minpt, maxpt);
    	EffTFPt40GEM = ptSubdirGEM.make<TH1F>("EffTFPt40GEM","Efficiency v Pt Tf > 40; 1.2<= #eta <=2.4",ptbins, minpt, maxpt);
    	EffTFPt60GEM = ptSubdirGEM.make<TH1F>("EffTFPt60GEM","Efficiency v Pt Tf > 60; 1.2<= #eta <=2.4",ptbins, minpt, maxpt);
    	EffTFPt10GEMQ1 = ptSubdirGEMQ1.make<TH1F>("EffTFPt10GEMQ1","Efficiency v Pt Tf > 10; 1.2<= #eta <=2.4,Q>=1",ptbins, minpt, maxpt);
	EffTFPt12GEMQ1 = ptSubdirGEMQ1.make<TH1F>("EffTFPt12GEMQ1","Efficiency v Pt Tf > 12; 1.2<= #eta <=2.4,Q>=1",ptbins, minpt, maxpt);
	EffTFPt16GEMQ1 = ptSubdirGEMQ1.make<TH1F>("EffTFPt16GEMQ1","Efficiency v Pt Tf > 16; 1.2<= #eta <=2.4,Q>=1",ptbins, minpt, maxpt);
    	EffTFPt20GEMQ1 = ptSubdirGEMQ1.make<TH1F>("EffTFPt20GEMQ1","Efficiency v Pt Tf > 20; 1.2<= #eta <=2.4,Q>=1",ptbins, minpt, maxpt);
    	EffTFPt40GEMQ1 = ptSubdirGEMQ1.make<TH1F>("EffTFPt40GEMQ1","Efficiency v Pt Tf > 40; 1.2<= #eta <=2.4,Q>=1",ptbins, minpt, maxpt);
    	EffTFPt60GEMQ1 = ptSubdirGEMQ1.make<TH1F>("EffTFPt60GEMQ1","Efficiency v Pt Tf > 60; 1.2<= #eta <=2.4,Q>=1",ptbins, minpt, maxpt);
    	EffTFPt10GEMQ2 = ptSubdirGEMQ2.make<TH1F>("EffTFPt10GEMQ2","Efficiency v Pt Tf > 10; 1.2<= #eta <=2.4,Q>=2",ptbins, minpt, maxpt);
	EffTFPt12GEMQ2 = ptSubdirGEMQ2.make<TH1F>("EffTFPt12GEMQ2","Efficiency v Pt Tf > 12; 1.2<= #eta <=2.4,Q>=2",ptbins, minpt, maxpt);
	EffTFPt16GEMQ2 = ptSubdirGEMQ2.make<TH1F>("EffTFPt16GEMQ2","Efficiency v Pt Tf > 16; 1.2<= #eta <=2.4,Q>=2",ptbins, minpt, maxpt);
    	EffTFPt20GEMQ2 = ptSubdirGEMQ2.make<TH1F>("EffTFPt20GEMQ2","Efficiency v Pt Tf > 20; 1.2<= #eta <=2.4,Q>=2",ptbins, minpt, maxpt);
    	EffTFPt40GEMQ2 = ptSubdirGEMQ2.make<TH1F>("EffTFPt40GEMQ2","Efficiency v Pt Tf > 40; 1.2<= #eta <=2.4,Q>=2",ptbins, minpt, maxpt);
    	EffTFPt60GEMQ2 = ptSubdirGEMQ2.make<TH1F>("EffTFPt60GEMQ2","Efficiency v Pt Tf > 60; 1.2<= #eta <=2.4,Q>=2",ptbins, minpt, maxpt);
    	EffTFPt10GEMQ3 = ptSubdirGEMQ3.make<TH1F>("EffTFPt10GEMQ3","Efficiency v Pt Tf > 10; 1.2<= #eta <=2.4,Q>=3",ptbins, minpt, maxpt);
	EffTFPt12GEMQ3 = ptSubdirGEMQ3.make<TH1F>("EffTFPt12GEMQ3","Efficiency v Pt Tf > 12; 1.2<= #eta <=2.4,Q>=3",ptbins, minpt, maxpt);
	EffTFPt16GEMQ3 = ptSubdirGEMQ3.make<TH1F>("EffTFPt16GEMQ3","Efficiency v Pt Tf > 16; 1.2<= #eta <=2.4,Q>=3",ptbins, minpt, maxpt);
    	EffTFPt20GEMQ3 = ptSubdirGEMQ3.make<TH1F>("EffTFPt20GEMQ3","Efficiency v Pt Tf > 20; 1.2<= #eta <=2.4,Q>=3",ptbins, minpt, maxpt);
    	EffTFPt40GEMQ3 = ptSubdirGEMQ3.make<TH1F>("EffTFPt40GEMQ3","Efficiency v Pt Tf > 40; 1.2<= #eta <=2.4,Q>=3",ptbins, minpt, maxpt);
    	EffTFPt60GEMQ3 = ptSubdirGEMQ3.make<TH1F>("EffTFPt60GEMQ3","Efficiency v Pt Tf > 60; 1.2<= #eta <=2.4,Q>=3",ptbins, minpt, maxpt);

    	EffEtaAll = etaSubdir.make<TH1F>("EffEtaAll","Efficiency v eta for all Tracks", 50, 0, 2.5);
    	EffEtaQ3 = etaSubdir.make<TH1F>("EffEtaQ3","Efficiency v #eta for Quality >= 3 Tracks", 50, 0, 2.5);
    	EffEtaQ2 = etaSubdir.make<TH1F>("EffEtaQ2","Efficiency v #eta for Quality >= 2 Tracks", 50, 0, 2.5);
    	EffEtaQ1 = etaSubdir.make<TH1F>("EffEtaQ1","Efficiency v #eta for Quality >= 1 Tracks", 50, 0, 2.5);	
    	EffSignedEtaAll = etaSubdir.make<TH1F>("EffSignedEtaAll","Efficiency v eta for all Tracks", 100, -2.5, 2.5);
    	EffSignedEtaQ3 = etaSubdir.make<TH1F>("EffSignedEtaQ3","Efficiency v #eta for Quality >= 3 Tracks", 100, -2.5, 2.5);
    	EffSignedEtaQ2 = etaSubdir.make<TH1F>("EffSignedEtaQ2","Efficiency v #eta for Quality >= 2 Tracks", 100, -2.5, 2.5);
    	EffSignedEtaQ1 = etaSubdir.make<TH1F>("EffSignedEtaQ1","Efficiency v #eta for Quality >= 1 Tracks", 100, -2.5, 2.5);	
    	EffPhiQ3 = phiSubdir.make<TH1F>("EffPhiQ3","Efficiency v #phi for Quality >= 3 Tracks",144,0,6.283);
    	EffPhiQ2 = phiSubdir.make<TH1F>("EffPhiQ2","Efficiency v #phi for Quality >= 2 Tracks",144,0,6.283);
    	EffPhiQ1 = phiSubdir.make<TH1F>("EffPhiQ1","Efficiency v #phi for Quality >= 1 Tracks",144,0,6.283);
}


void EffHistogramList::ComputeEff(TrackHistogramList* refHists)
{
    
    divideHistograms(refHists);
	
////////////////////
//// Pt Eff ///////
//////////////////
   
    //Putting Histograms in vectors for use in later functions
    std::vector<TH1F*> Overallhists;
    Overallhists.push_back(EffPtOverall);
    Overallhists.push_back(EffTFPt12Overall);
    Overallhists.push_back(EffTFPt20Overall);
    Overallhists.push_back(EffTFPt40Overall);
    Overallhists.push_back(EffTFPt60Overall);
    std::vector<TH1F*> OverallhistsQ1;
    OverallhistsQ1.push_back(EffPtOverallQ1);
    OverallhistsQ1.push_back(EffTFPt12OverallQ1);
    OverallhistsQ1.push_back(EffTFPt20OverallQ1);
    OverallhistsQ1.push_back(EffTFPt40OverallQ1);
    OverallhistsQ1.push_back(EffTFPt60OverallQ1);
    std::vector<TH1F*> OverallhistsQ2;
    OverallhistsQ2.push_back(EffPtOverallQ2);
    OverallhistsQ2.push_back(EffTFPt12OverallQ2);
    OverallhistsQ2.push_back(EffTFPt20OverallQ2);
    OverallhistsQ2.push_back(EffTFPt40OverallQ2);
    OverallhistsQ2.push_back(EffTFPt60OverallQ2);
    std::vector<TH1F*> OverallhistsQ3;
    OverallhistsQ3.push_back(EffPtOverallQ3);
    OverallhistsQ3.push_back(EffTFPt12OverallQ3);
    OverallhistsQ3.push_back(EffTFPt20OverallQ3);
    OverallhistsQ3.push_back(EffTFPt40OverallQ3);
    OverallhistsQ3.push_back(EffTFPt60OverallQ3);
    
    std::vector<TH1F*> CSCOnlyhists;
    CSCOnlyhists.push_back(EffPtCSCOnly);
    CSCOnlyhists.push_back(EffTFPt12CSCOnly);
    CSCOnlyhists.push_back(EffTFPt20CSCOnly);
    CSCOnlyhists.push_back(EffTFPt40CSCOnly);
    CSCOnlyhists.push_back(EffTFPt60CSCOnly);
    std::vector<TH1F*> CSCOnlyhistsQ1;
    CSCOnlyhistsQ1.push_back(EffPtCSCOnlyQ1);
    CSCOnlyhistsQ1.push_back(EffTFPt12CSCOnlyQ1);
    CSCOnlyhistsQ1.push_back(EffTFPt20CSCOnlyQ1);
    CSCOnlyhistsQ1.push_back(EffTFPt40CSCOnlyQ1);
    CSCOnlyhistsQ1.push_back(EffTFPt60CSCOnlyQ1);
    std::vector<TH1F*> CSCOnlyhistsQ2;
    CSCOnlyhistsQ2.push_back(EffPtCSCOnlyQ2);
    CSCOnlyhistsQ2.push_back(EffTFPt12CSCOnlyQ2);
    CSCOnlyhistsQ2.push_back(EffTFPt20CSCOnlyQ2);
    CSCOnlyhistsQ2.push_back(EffTFPt40CSCOnlyQ2);
    CSCOnlyhistsQ2.push_back(EffTFPt60CSCOnlyQ2);
    std::vector<TH1F*> CSCOnlyhistsQ3;
    CSCOnlyhistsQ3.push_back(EffPtCSCOnlyQ3);
    CSCOnlyhistsQ3.push_back(EffTFPt12CSCOnlyQ3);
    CSCOnlyhistsQ3.push_back(EffTFPt20CSCOnlyQ3);
    CSCOnlyhistsQ3.push_back(EffTFPt40CSCOnlyQ3);
    CSCOnlyhistsQ3.push_back(EffTFPt60CSCOnlyQ3);

    std::vector<TH1F*> GEMhists;
    GEMhists.push_back(EffPtGEM);
    GEMhists.push_back(EffTFPt12GEM);
    GEMhists.push_back(EffTFPt20GEM);
    GEMhists.push_back(EffTFPt40GEM);
    GEMhists.push_back(EffTFPt60GEM);
    std::vector<TH1F*> GEMhistsQ1;
    GEMhistsQ1.push_back(EffPtGEMQ1);
    GEMhistsQ1.push_back(EffTFPt12GEMQ1);
    GEMhistsQ1.push_back(EffTFPt20GEMQ1);
    GEMhistsQ1.push_back(EffTFPt40GEMQ1);
    GEMhistsQ1.push_back(EffTFPt60GEMQ1);
    std::vector<TH1F*> GEMhistsQ2;
    GEMhistsQ2.push_back(EffPtGEMQ2);
    GEMhistsQ2.push_back(EffTFPt12GEMQ2);
    GEMhistsQ2.push_back(EffTFPt20GEMQ2);
    GEMhistsQ2.push_back(EffTFPt40GEMQ2);
    GEMhistsQ2.push_back(EffTFPt60GEMQ2);
    std::vector<TH1F*> GEMhistsQ3;
    GEMhistsQ3.push_back(EffPtGEMQ3);
    GEMhistsQ3.push_back(EffTFPt12GEMQ3);
    GEMhistsQ3.push_back(EffTFPt20GEMQ3);
    GEMhistsQ3.push_back(EffTFPt40GEMQ3);
    GEMhistsQ3.push_back(EffTFPt60GEMQ3);

    std::vector<TH1F*> CSCRestrictedhists;
    CSCRestrictedhists.push_back(EffPtCSCRestricted);
    CSCRestrictedhists.push_back(EffTFPt12CSCRestricted);
    CSCRestrictedhists.push_back(EffTFPt20CSCRestricted);
    CSCRestrictedhists.push_back(EffTFPt40CSCRestricted);
    CSCRestrictedhists.push_back(EffTFPt60CSCRestricted);
    std::vector<TH1F*> CSCRestrictedhistsQ1;
    CSCRestrictedhistsQ1.push_back(EffPtCSCRestrictedQ1);
    CSCRestrictedhistsQ1.push_back(EffTFPt12CSCRestrictedQ1);
    CSCRestrictedhistsQ1.push_back(EffTFPt20CSCRestrictedQ1);
    CSCRestrictedhistsQ1.push_back(EffTFPt40CSCRestrictedQ1);
    CSCRestrictedhistsQ1.push_back(EffTFPt60CSCRestrictedQ1);
    std::vector<TH1F*> CSCRestrictedhistsQ2;
    CSCRestrictedhistsQ2.push_back(EffPtCSCRestrictedQ2);
    CSCRestrictedhistsQ2.push_back(EffTFPt12CSCRestrictedQ2);
    CSCRestrictedhistsQ2.push_back(EffTFPt20CSCRestrictedQ2);
    CSCRestrictedhistsQ2.push_back(EffTFPt40CSCRestrictedQ2);
    CSCRestrictedhistsQ2.push_back(EffTFPt60CSCRestrictedQ2);
    std::vector<TH1F*> CSCRestrictedhistsQ3;
    CSCRestrictedhistsQ3.push_back(EffPtCSCRestrictedQ3);
    CSCRestrictedhistsQ3.push_back(EffTFPt12CSCRestrictedQ3);
    CSCRestrictedhistsQ3.push_back(EffTFPt20CSCRestrictedQ3);
    CSCRestrictedhistsQ3.push_back(EffTFPt40CSCRestrictedQ3);
    CSCRestrictedhistsQ3.push_back(EffTFPt60CSCRestrictedQ3);
    
    std::vector<TH1F*> DTOnlyhists;
    DTOnlyhists.push_back(EffPtDTOnly);
    DTOnlyhists.push_back(EffTFPt12DTOnly);
    DTOnlyhists.push_back(EffTFPt20DTOnly);
    DTOnlyhists.push_back(EffTFPt40DTOnly);
    DTOnlyhists.push_back(EffTFPt60DTOnly);
    std::vector<TH1F*> DTOnlyhistsQ1;
    DTOnlyhistsQ1.push_back(EffPtDTOnlyQ1);
    DTOnlyhistsQ1.push_back(EffTFPt12DTOnlyQ1);
    DTOnlyhistsQ1.push_back(EffTFPt20DTOnlyQ1);
    DTOnlyhistsQ1.push_back(EffTFPt40DTOnlyQ1);
    DTOnlyhistsQ1.push_back(EffTFPt60DTOnlyQ1);
    std::vector<TH1F*> DTOnlyhistsQ2;
    DTOnlyhistsQ2.push_back(EffPtDTOnlyQ2);
    DTOnlyhistsQ2.push_back(EffTFPt12DTOnlyQ2);
    DTOnlyhistsQ2.push_back(EffTFPt20DTOnlyQ2);
    DTOnlyhistsQ2.push_back(EffTFPt40DTOnlyQ2);
    DTOnlyhistsQ2.push_back(EffTFPt60DTOnlyQ2);
    std::vector<TH1F*> DTOnlyhistsQ3;
    DTOnlyhistsQ3.push_back(EffPtDTOnlyQ3);
    DTOnlyhistsQ3.push_back(EffTFPt12DTOnlyQ3);
    DTOnlyhistsQ3.push_back(EffTFPt20DTOnlyQ3);
    DTOnlyhistsQ3.push_back(EffTFPt40DTOnlyQ3);
    DTOnlyhistsQ3.push_back(EffTFPt60DTOnlyQ3);
    
    std::vector<TH1F*> Overlaphists;
    Overlaphists.push_back(EffPtOverlap);
    Overlaphists.push_back(EffTFPt12Overlap);
    Overlaphists.push_back(EffTFPt20Overlap);
    Overlaphists.push_back(EffTFPt40Overlap);
    Overlaphists.push_back(EffTFPt60Overlap);
    std::vector<TH1F*> OverlaphistsQ1;
    OverlaphistsQ1.push_back(EffPtOverlapQ1);
    OverlaphistsQ1.push_back(EffTFPt12OverlapQ1);
    OverlaphistsQ1.push_back(EffTFPt20OverlapQ1);
    OverlaphistsQ1.push_back(EffTFPt40OverlapQ1);
    OverlaphistsQ1.push_back(EffTFPt60OverlapQ1);
    std::vector<TH1F*> OverlaphistsQ2;
    OverlaphistsQ2.push_back(EffPtOverlapQ2);
    OverlaphistsQ2.push_back(EffTFPt12OverlapQ2);
    OverlaphistsQ2.push_back(EffTFPt20OverlapQ2);
    OverlaphistsQ2.push_back(EffTFPt40OverlapQ2);
    OverlaphistsQ2.push_back(EffTFPt60OverlapQ2);
    std::vector<TH1F*> OverlaphistsQ3;
    OverlaphistsQ3.push_back(EffPtOverlapQ3);
    OverlaphistsQ3.push_back(EffTFPt12OverlapQ3);
    OverlaphistsQ3.push_back(EffTFPt20OverlapQ3);
    OverlaphistsQ3.push_back(EffTFPt40OverlapQ3);
    OverlaphistsQ3.push_back(EffTFPt60OverlapQ3);

    std::vector<TH1F*> HighEtahists;
    HighEtahists.push_back(EffPtHighEta);
    HighEtahists.push_back(EffTFPt12HighEta);
    HighEtahists.push_back(EffTFPt20HighEta);
    HighEtahists.push_back(EffTFPt40HighEta);
    HighEtahists.push_back(EffTFPt60HighEta);
    std::vector<TH1F*> HighEtahistsQ1;
    HighEtahistsQ1.push_back(EffPtHighEtaQ1);
    HighEtahistsQ1.push_back(EffTFPt12HighEtaQ1);
    HighEtahistsQ1.push_back(EffTFPt20HighEtaQ1);
    HighEtahistsQ1.push_back(EffTFPt40HighEtaQ1);
    HighEtahistsQ1.push_back(EffTFPt60HighEtaQ1);
    std::vector<TH1F*> HighEtahistsQ2;
    HighEtahistsQ2.push_back(EffPtHighEtaQ2);
    HighEtahistsQ2.push_back(EffTFPt12HighEtaQ2);
    HighEtahistsQ2.push_back(EffTFPt20HighEtaQ2);
    HighEtahistsQ2.push_back(EffTFPt40HighEtaQ2);
    HighEtahistsQ2.push_back(EffTFPt60HighEtaQ2);
    std::vector<TH1F*> HighEtahistsQ3;
    HighEtahistsQ3.push_back(EffPtHighEtaQ3);
    HighEtahistsQ3.push_back(EffTFPt12HighEtaQ3);
    HighEtahistsQ3.push_back(EffTFPt20HighEtaQ3);
    HighEtahistsQ3.push_back(EffTFPt40HighEtaQ3);
    HighEtahistsQ3.push_back(EffTFPt60HighEtaQ3);
    
    //must match the threshold order of the histograms pushed into
    //the vectors directly above
    std::vector<std::string> thresholds;
    thresholds.push_back("");
    thresholds.push_back("10");
    thresholds.push_back("12");
    thresholds.push_back("16");
    thresholds.push_back("20");
    thresholds.push_back("40");
    thresholds.push_back("60");
    
    
    //Here you define where your Pt Histograms "Plateau"
    //the indices should line up with the OverallHists, 
    //CSCOnlyhists, DTOnlyhists, etc. above.
    std::vector<double> PlateauDefinitions;
    PlateauDefinitions.push_back(80);
    PlateauDefinitions.push_back(24);
    PlateauDefinitions.push_back(30);
    PlateauDefinitions.push_back(60);
    PlateauDefinitions.push_back(80);
    
    //Computing the plateau Efficiencies of Pt Histograms and writing them to a file
    std::ofstream* PtStats=new std::ofstream(PtEffStatsFilename.c_str());
    (*PtStats)<<"Pt Plateau Efficiencies for Overall region (|eta|<=2.4)";
    computePtPlateauEff(PtStats, PlateauDefinitions,thresholds,Overallhists);
    computePtPlateauEff(PtStats, PlateauDefinitions,thresholds,OverallhistsQ1);
    computePtPlateauEff(PtStats, PlateauDefinitions,thresholds,OverallhistsQ2);
    computePtPlateauEff(PtStats, PlateauDefinitions,thresholds,OverallhistsQ3);
    (*PtStats)<<"\n\nPt Plateau Efficiencies for CSC Only region (1.2<=|eta|<=2.4)";
    computePtPlateauEff(PtStats, PlateauDefinitions,thresholds,CSCOnlyhists);
    computePtPlateauEff(PtStats, PlateauDefinitions,thresholds,CSCOnlyhistsQ1);
    computePtPlateauEff(PtStats, PlateauDefinitions,thresholds,CSCOnlyhistsQ2);
    computePtPlateauEff(PtStats, PlateauDefinitions,thresholds,CSCOnlyhistsQ3);
    (*PtStats)<<"\n\nPt Plateau Efficiencies for GEM GE2/1 region (1.6<=|eta|<=2.4)";
    computePtPlateauEff(PtStats, PlateauDefinitions,thresholds,GEMhists);
    computePtPlateauEff(PtStats, PlateauDefinitions,thresholds,GEMhistsQ1);
    computePtPlateauEff(PtStats, PlateauDefinitions,thresholds,GEMhistsQ2);
    computePtPlateauEff(PtStats, PlateauDefinitions,thresholds,GEMhistsQ3);
    (*PtStats)<<"\n\nPt Plateau Efficiencies for CSC Restricted region (1.2<=|eta|<=2.1)";
    computePtPlateauEff(PtStats, PlateauDefinitions,thresholds,CSCRestrictedhists);
    computePtPlateauEff(PtStats, PlateauDefinitions,thresholds,CSCRestrictedhistsQ1);
    computePtPlateauEff(PtStats, PlateauDefinitions,thresholds,CSCRestrictedhistsQ2);
    computePtPlateauEff(PtStats, PlateauDefinitions,thresholds,CSCRestrictedhistsQ3);
    (*PtStats)<<"\n\nPt Plateau Efficiencies for DT Only region (|eta|<=0.9)";
    computePtPlateauEff(PtStats, PlateauDefinitions,thresholds,DTOnlyhists);
    computePtPlateauEff(PtStats, PlateauDefinitions,thresholds,DTOnlyhistsQ1);
    computePtPlateauEff(PtStats, PlateauDefinitions,thresholds,DTOnlyhistsQ2);
    computePtPlateauEff(PtStats, PlateauDefinitions,thresholds,DTOnlyhistsQ3);
    (*PtStats)<<"\n\nPt Plateau Efficiencies for Overlap region (1.2<=|eta|<=0.9)";
    computePtPlateauEff(PtStats, PlateauDefinitions,thresholds,Overlaphists);
    computePtPlateauEff(PtStats, PlateauDefinitions,thresholds,OverlaphistsQ1);
    computePtPlateauEff(PtStats, PlateauDefinitions,thresholds,OverlaphistsQ2);
    computePtPlateauEff(PtStats, PlateauDefinitions,thresholds,OverlaphistsQ3);
    (*PtStats)<<"\n\nPt Plateau Efficiencies for HighEta region (2.1<=|eta|)";
    computePtPlateauEff(PtStats, PlateauDefinitions,thresholds,HighEtahists);
    computePtPlateauEff(PtStats, PlateauDefinitions,thresholds,HighEtahistsQ1);
    computePtPlateauEff(PtStats, PlateauDefinitions,thresholds,HighEtahistsQ2);
    computePtPlateauEff(PtStats, PlateauDefinitions,thresholds,HighEtahistsQ3);
    PtStats->close();
    
    //Drawing Pt Histograms
    DrawPtEffHists("Overall",PtEffAllOverall,fitThreshOverall,TrackerLeg1Overall,thresholds,Overallhists);
    DrawPtEffHists("CSCOnly",PtEffAllCSCOnly,fitThreshCSCOnly,TrackerLeg1CSCOnly,thresholds,CSCOnlyhists);
    DrawPtEffHists("GEM",PtEffAllGEM,fitThreshGEM,TrackerLeg1GEM,thresholds,GEMhists);
    DrawPtEffHists("CSCRestricted",PtEffAllCSCRestricted,fitThreshCSCRestricted,TrackerLeg1CSCRestricted,thresholds,CSCRestrictedhists);
    DrawPtEffHists("DTOnly",PtEffAllDTOnly,fitThreshDTOnly,TrackerLeg1DTOnly,thresholds,DTOnlyhists);
    DrawPtEffHists("Overlap",PtEffAllOverlap,fitThreshOverlap,TrackerLeg1Overlap,thresholds,Overlaphists);
    DrawPtEffHists("HighEta",PtEffAllHighEta,fitThreshHighEta,TrackerLeg1HighEta,thresholds,HighEtahists);
   
    DrawPtEffHists("OverallQ1",PtEffAllOverallQ1,fitThreshOverall,TrackerLeg1Overall,thresholds,OverallhistsQ1);
    DrawPtEffHists("CSCOnlyQ1",PtEffAllCSCOnlyQ1,fitThreshCSCOnly,TrackerLeg1CSCOnly,thresholds,CSCOnlyhistsQ1);
    DrawPtEffHists("GEMQ1",PtEffAllGEMQ1,fitThreshGEM,TrackerLeg1GEM,thresholds,GEMhistsQ1);
    DrawPtEffHists("CSCRestrictedQ1",PtEffAllCSCRestrictedQ1,fitThreshCSCRestricted,TrackerLeg1CSCRestricted,thresholds,CSCRestrictedhistsQ1);
    DrawPtEffHists("DTOnlyQ1",PtEffAllDTOnlyQ1,fitThreshDTOnly,TrackerLeg1DTOnly,thresholds,DTOnlyhistsQ1);
    DrawPtEffHists("OverlapQ1",PtEffAllOverlapQ1,fitThreshOverlap,TrackerLeg1Overlap,thresholds,OverlaphistsQ1);
    DrawPtEffHists("HighEtaQ1",PtEffAllHighEtaQ1,fitThreshHighEta,TrackerLeg1HighEta,thresholds,HighEtahistsQ1);
   
    DrawPtEffHists("OverallQ2",PtEffAllOverallQ2,fitThreshOverall,TrackerLeg1Overall,thresholds,OverallhistsQ2);
    DrawPtEffHists("CSCOnlyQ2",PtEffAllCSCOnlyQ2,fitThreshCSCOnly,TrackerLeg1CSCOnly,thresholds,CSCOnlyhistsQ2);
    DrawPtEffHists("GEMQ2",PtEffAllGEMQ2,fitThreshGEM,TrackerLeg1GEM,thresholds,GEMhistsQ2);
    DrawPtEffHists("CSCRestrictedQ2",PtEffAllCSCRestrictedQ2,fitThreshCSCRestricted,TrackerLeg1CSCRestricted,thresholds,CSCRestrictedhistsQ2);
    DrawPtEffHists("DTOnlyQ2",PtEffAllDTOnlyQ2,fitThreshDTOnly,TrackerLeg1DTOnly,thresholds,DTOnlyhistsQ2);
    DrawPtEffHists("OverlapQ2",PtEffAllOverlapQ2,fitThreshOverlap,TrackerLeg1Overlap,thresholds,OverlaphistsQ2);
    DrawPtEffHists("HighEtaQ2",PtEffAllHighEtaQ2,fitThreshHighEta,TrackerLeg1HighEta,thresholds,HighEtahistsQ2);
   
    DrawPtEffHists("OverallQ3",PtEffAllOverallQ3,fitThreshOverall,TrackerLeg1Overall,thresholds,OverallhistsQ3);
    DrawPtEffHists("CSCOnlyQ3",PtEffAllCSCOnlyQ3,fitThreshCSCOnly,TrackerLeg1CSCOnly,thresholds,CSCOnlyhistsQ3);
    DrawPtEffHists("GEMQ3",PtEffAllGEMQ3,fitThreshGEM,TrackerLeg1GEM,thresholds,GEMhistsQ3);
    DrawPtEffHists("CSCRestrictedQ3",PtEffAllCSCRestrictedQ3,fitThreshCSCRestricted,TrackerLeg1CSCRestricted,thresholds,CSCRestrictedhistsQ3);
    DrawPtEffHists("DTOnlyQ3",PtEffAllDTOnlyQ3,fitThreshDTOnly,TrackerLeg1DTOnly,thresholds,DTOnlyhistsQ3);
    DrawPtEffHists("OverlapQ3",PtEffAllOverlapQ3,fitThreshOverlap,TrackerLeg1Overlap,thresholds,OverlaphistsQ3);
    DrawPtEffHists("HighEtaQ3",PtEffAllHighEtaQ3,fitThreshHighEta,TrackerLeg1HighEta,thresholds,HighEtahistsQ3);
   
   
    ///////////////////
    //Overall Eta Eff//
    //////////////////
    EtaEff = fs->make<TCanvas>("EtaEff");
    EffEtaQ1->GetXaxis()->SetTitle("Eta Sim");
    EffEtaQ1->GetYaxis()->SetTitle("Efficiency");
    EffEtaQ2->GetXaxis()->SetTitle("Eta Sim");
    EffEtaQ2->GetYaxis()->SetTitle("Efficiency");
    EffEtaQ3->GetXaxis()->SetTitle("Eta Sim");
    EffEtaQ3->GetYaxis()->SetTitle("Efficiency");
    EffEtaAll->GetYaxis()->SetRangeUser(0.0,1.1);
    EffEtaQ1->GetYaxis()->SetRangeUser(0.0,1.1);
    EffEtaQ2->GetYaxis()->SetRangeUser(0.0,1.1);
    EffEtaQ3->GetYaxis()->SetRangeUser(0.0,1.1);
    EffEtaQ1->SetTitle("Efficiency for Quality 1, 2, and 3 Tracks");
    EffEtaQ1->SetFillColor(1);
    EffEtaQ2->SetFillColor(4);
    EffEtaQ3->SetFillColor(3);

    EffEtaQ1->Draw();
    EffEtaQ2->Draw("same");
    EffEtaQ3->Draw("same");
    TrackerLeg2 = new TLegend(0.12,0.12,0.32,0.27);
    TrackerLeg2->AddEntry(EffEtaQ1,"All Tracks","f");
    TrackerLeg2->AddEntry(EffEtaQ2,"Quality > 1","f");
    TrackerLeg2->AddEntry(EffEtaQ3,"Quality > 2","f");
    TrackerLeg2->Draw();
    latexDescription->Draw();
    gPad->SetTicks(1,0);
    //EtaEff->Print("EffEta.png","png");
    
    
    //////////////////////////
    //Overall Signed Eta Eff// 
    /////////////////////////
    SignedEtaEff = fs->make<TCanvas>("SignedEtaEff");
    EffSignedEtaQ1->GetXaxis()->SetTitle("Eta Sim");
    EffSignedEtaQ1->GetYaxis()->SetTitle("Efficiency");
    EffSignedEtaQ2->GetXaxis()->SetTitle("Eta Sim");
    EffSignedEtaQ2->GetYaxis()->SetTitle("Efficiency");
    EffSignedEtaQ3->GetXaxis()->SetTitle("Eta Sim");
    EffSignedEtaQ3->GetYaxis()->SetTitle("Efficiency");
    EffSignedEtaAll->GetYaxis()->SetRangeUser(0.0,1.1);
    EffSignedEtaQ1->GetYaxis()->SetRangeUser(0.0,1.1);
    EffSignedEtaQ2->GetYaxis()->SetRangeUser(0.0,1.1);
    EffSignedEtaQ3->GetYaxis()->SetRangeUser(0.0,1.1);
    EffSignedEtaQ1->SetTitle("Efficiency for Quality 1, 2, 3, and 4 Tracks");
    EffSignedEtaQ1->SetFillColor(1);
    EffSignedEtaQ2->SetFillColor(4);
    EffSignedEtaQ3->SetFillColor(3);
    EffSignedEtaQ1->Draw();
    EffSignedEtaQ2->Draw("same");
    EffSignedEtaQ3->Draw("same");
    TrackerLeg3 = new TLegend(0.12,0.12,0.32,0.27);
    TrackerLeg3->AddEntry(EffSignedEtaQ1,"All Tracks","f");
    TrackerLeg3->AddEntry(EffSignedEtaQ2,"Quality > 1","f");
    TrackerLeg3->AddEntry(EffSignedEtaQ3,"Quality > 2","f");
    TrackerLeg3->Draw();
    latexDescription->Draw();
    gPad->SetTicks(1,0);
    //EtaEff->Print("EffEta.png","png");
    
    //////////////////////
    //// Overall Phi Eff//
    //////////////////////
    PhiEff = fs->make<TCanvas>("PhiEff");
    EffPhiQ1->GetXaxis()->SetTitle("Phi Sim");
    EffPhiQ1->GetYaxis()->SetTitle("Efficiency");
    EffPhiQ2->GetXaxis()->SetTitle("Phi Sim");
    EffPhiQ2->GetYaxis()->SetTitle("Efficiency");
    EffPhiQ3->GetXaxis()->SetTitle("Phi Sim");
    EffPhiQ3->GetYaxis()->SetTitle("Efficiency");
    EffPhi->GetYaxis()->SetRangeUser(0.0,1.1);
    EffPhiQ1->GetYaxis()->SetRangeUser(0.0,1.1);
    EffPhiQ2->GetYaxis()->SetRangeUser(0.0,1.1);
    EffPhiQ3->GetYaxis()->SetRangeUser(0.0,1.1);
    EffPhiQ1->SetTitle("Efficiency for Quality 1, 2, 3, and 4 Tracks");
    EffPhiQ1->SetFillColor(1);
    EffPhiQ2->SetFillColor(4);
    EffPhiQ3->SetFillColor(3);
    EffPhiQ1->Draw(); 
    EffPhiQ2->Draw("same");
    EffPhiQ3->Draw("same");
    TrackerLeg2 = new TLegend(0.4,0.12,0.6,0.27);
    TrackerLeg2->AddEntry(EffPhiQ1,"All Tracks","f");
    TrackerLeg2->AddEntry(EffPhiQ2,"Quality > 1","f"); 
    TrackerLeg2->AddEntry(EffPhiQ3,"Quality > 2","f");
    TrackerLeg2->Draw();
    latexDescription->Draw();
    gPad->SetTicks(1,0);
    //PhiEff->Print("EffPhi.png","png");
    
    EffPhi_mod_10_Q2_endcap1->GetXaxis()->SetTitle("Phi%10 (deg)");
    EffPhi_mod_10_Q3_endcap1->GetXaxis()->SetTitle("Phi%10 (deg)");
    EffPhi_mod_10_Q2_endcap2->GetXaxis()->SetTitle("Phi%10 (deg)");
    EffPhi_mod_10_Q3_endcap2->GetXaxis()->SetTitle("Phi%10 (deg)");
    EffPhi_mod_10_Q2_endcap1->GetYaxis()->SetTitle("Efficiency");
    EffPhi_mod_10_Q3_endcap1->GetYaxis()->SetTitle("Efficiency");
    EffPhi_mod_10_Q2_endcap2->GetYaxis()->SetTitle("Efficiency");
    EffPhi_mod_10_Q3_endcap2->GetYaxis()->SetTitle("Efficiency");

}
void EffHistogramList::Print()
{
    PtEffAllOverall->Print("EffPtOverall.png","png");
    PtEffAllOverlap->Print("EffPtOverlap.png","png");
    PtEffAllHighEta->Print("EffPtHighEta.png","png");
    PtEffAllCSCOnly->Print("EffPtCSCOnly.png","png");
    PtEffAllGEM->Print("EffPtGEM.png","png");
    EtaEff->Print("EffEta.png","png");
    SignedEtaEff->Print("EffSignedEta.png","png");
    PhiEff->Print("EffPhi.png","png");
    PtEffAllOverallQ1->Print("EffPtOverallQ1.png","png");
    PtEffAllOverlapQ1->Print("EffPtOverlapQ1.png","png");
    PtEffAllHighEtaQ1->Print("EffPtHighEtaQ1.png","png");
    PtEffAllCSCOnlyQ1->Print("EffPtCSCOnlyQ1.png","png");
    PtEffAllGEMQ1->Print("EffPtGEMQ1.png","png");
    PtEffAllOverallQ2->Print("EffPtOverallQ2.png","png");
    PtEffAllOverlapQ2->Print("EffPtOverlapQ2.png","png");
    PtEffAllHighEtaQ2->Print("EffPtHighEtaQ2.png","png");
    PtEffAllCSCOnlyQ2->Print("EffPtCSCOnlyQ2.png","png");
    PtEffAllGEMQ2->Print("EffPtGEMQ2.png","png");
    PtEffAllOverallQ3->Print("EffPtOverallQ3.png","png");
    PtEffAllOverlapQ3->Print("EffPtOverlapQ3.png","png");
    PtEffAllHighEtaQ3->Print("EffPtHighEtaQ3.png","png");
    PtEffAllCSCOnlyQ3->Print("EffPtCSCOnlyQ3.png","png");
    PtEffAllGEMQ3->Print("EffPtGEMQ3.png","png");
}
  
  
  
void EffHistogramList::DrawPtEffHists(std::string region, TCanvas* canvas, TF1* fit, TLegend* legend, std::vector<std::string> thresholds, std::vector<TH1F*> PtEffHists)
{
    std::string tmp;
    
    tmp="PtEffAll"+region;
    canvas = fs->make<TCanvas>(tmp.c_str());
//    tmp="fitThresh"+region;
//    fit = new TF1(tmp.c_str(), csctf_analysis::thresh, 0, 100, 4);
    legend = new TLegend(0.7,0.15,0.85,0.35);
	
    std::vector<TH1F*>::iterator iHist;
    std::vector<std::string>::iterator iThreshold;

    int i=0;
    for(iHist=PtEffHists.begin();iHist!=PtEffHists.end();iHist++)
    {
	PtEffHists[i]->GetXaxis()->SetTitle("Pt Sim (GeV/c)");
	PtEffHists[i]->GetYaxis()->SetTitle("Efficiency");
	PtEffHists[i]->GetYaxis()->SetRangeUser(0.0,1.1);
	tmp=region+" Pt Efficiency";
	PtEffHists[i]->SetTitle(tmp.c_str());
	PtEffHists[i]->SetFillColor(7-i);
//	tmp="Pt"+thresholds[i];
//	fit->SetParNames(tmp.c_str(),"Resol","Constant","Slope");
	
//	tmp="fitThresh"+region;	
//	PtEffHists[i]->Fit(tmp.c_str());
	
	if(i==0) tmp="All Tracks";
	else tmp="Pt_{TF} > "+thresholds[i];
	
	legend->AddEntry(PtEffHists[i],tmp.c_str(),"f");
	
	i++;
    }
    
    i=0;
    PtEffHists[0]->Draw("Hist");
    for(iHist=PtEffHists.begin();iHist!=PtEffHists.end();iHist++){PtEffHists[i]->Draw("Hist Same"); i++;}
 
    legend->Draw("same");
    latexDescription->Draw();
    gPad->SetTicks(1,0);
}


void EffHistogramList::computeErrors(TrackHistogramList* refHists)
{
	refHists->matchTFPt10CSCRestricted->Sumw2();	refHists->matchTFPt12CSCRestricted->Sumw2();	refHists->matchTFPt16CSCRestricted->Sumw2();
	refHists->matchTFPt20CSCRestricted->Sumw2();	refHists->matchTFPt40CSCRestricted->Sumw2();	refHists->matchTFPt60CSCRestricted->Sumw2();
	
	refHists->matchTFPt10Overall->Sumw2();	refHists->matchTFPt12Overall->Sumw2();	refHists->matchTFPt16Overall->Sumw2();
	refHists->matchTFPt20Overall->Sumw2();	refHists->matchTFPt40Overall->Sumw2();	refHists->matchTFPt60Overall->Sumw2();
	
	refHists->matchTFPt10CSCOnly->Sumw2();	refHists->matchTFPt12CSCOnly->Sumw2();	refHists->matchTFPt16CSCOnly->Sumw2();
	refHists->matchTFPt20CSCOnly->Sumw2();	refHists->matchTFPt40CSCOnly->Sumw2();	refHists->matchTFPt60CSCOnly->Sumw2();

	refHists->matchTFPt10GEM->Sumw2();	refHists->matchTFPt12GEM->Sumw2();	refHists->matchTFPt16GEM->Sumw2();
	refHists->matchTFPt20GEM->Sumw2();	refHists->matchTFPt40GEM->Sumw2();	refHists->matchTFPt60GEM->Sumw2();

	refHists->matchTFPt10GEMQ1->Sumw2();	refHists->matchTFPt12GEMQ1->Sumw2();	refHists->matchTFPt16GEMQ1->Sumw2();
	refHists->matchTFPt20GEMQ1->Sumw2();	refHists->matchTFPt40GEMQ1->Sumw2();	refHists->matchTFPt60GEMQ1->Sumw2();

	refHists->matchTFPt10GEMQ2->Sumw2();	refHists->matchTFPt12GEMQ2->Sumw2();	refHists->matchTFPt16GEMQ2->Sumw2();
	refHists->matchTFPt20GEMQ2->Sumw2();	refHists->matchTFPt40GEMQ2->Sumw2();	refHists->matchTFPt60GEMQ2->Sumw2();

	refHists->matchTFPt10GEMQ3->Sumw2();	refHists->matchTFPt12GEMQ3->Sumw2();	refHists->matchTFPt16GEMQ3->Sumw2();
	refHists->matchTFPt20GEMQ3->Sumw2();	refHists->matchTFPt40GEMQ3->Sumw2();	refHists->matchTFPt60GEMQ3->Sumw2();


	refHists->matchTFPt10DTOnly->Sumw2();	refHists->matchTFPt12DTOnly->Sumw2();	refHists->matchTFPt16DTOnly->Sumw2();
	refHists->matchTFPt20DTOnly->Sumw2();	refHists->matchTFPt40DTOnly->Sumw2();	refHists->matchTFPt60DTOnly->Sumw2();

	refHists->matchTFPt10Overlap->Sumw2();	refHists->matchTFPt12Overlap->Sumw2();	refHists->matchTFPt16Overlap->Sumw2();
	refHists->matchTFPt20Overlap->Sumw2();	refHists->matchTFPt40Overlap->Sumw2();	refHists->matchTFPt60Overlap->Sumw2();
	refHists->matchTFPt10HighEta->Sumw2();	refHists->matchTFPt12HighEta->Sumw2();	refHists->matchTFPt16HighEta->Sumw2();
	refHists->matchTFPt20HighEta->Sumw2();	refHists->matchTFPt40HighEta->Sumw2();	refHists->matchTFPt60HighEta->Sumw2();

	refHists->matchPtOverall->Sumw2();	refHists->matchPtCSCOnly->Sumw2();	refHists->matchPtDTOnly->Sumw2();
	refHists->matchPtHighEta->Sumw2();	refHists->ptDenHighEta->Sumw2();	refHists->ptDenCSCOnly->Sumw2();

	refHists->matchPtOverallQ1->Sumw2();	refHists->matchPtCSCOnlyQ1->Sumw2();	refHists->matchPtDTOnlyQ1->Sumw2();

	refHists->matchPtOverallQ2->Sumw2();	refHists->matchPtCSCOnlyQ2->Sumw2();	refHists->matchPtDTOnlyQ2->Sumw2();

	refHists->matchPtOverallQ3->Sumw2();	refHists->matchPtCSCOnlyQ3->Sumw2();	refHists->matchPtDTOnlyQ3->Sumw2();


	refHists->matchPtGEM->Sumw2();
	refHists->matchPtGEMQ1->Sumw2();
	refHists->matchPtGEMQ2->Sumw2();
	refHists->matchPtGEMQ3->Sumw2();
	refHists->ptDenGEM->Sumw2();

	refHists->ptDenDTOnly->Sumw2();	refHists->ptDenCSCRestricted->Sumw2();	refHists->ptDenOverall->Sumw2();
}

void EffHistogramList::divideHistograms(TrackHistogramList* refHists)
{
	computeErrors(refHists);


	EffTFPt10Overall->Divide(refHists->matchTFPt10Overall, refHists->ptDenOverall);
	EffTFPt12Overall->Divide(refHists->matchTFPt12Overall, refHists->ptDenOverall);
	EffTFPt16Overall->Divide(refHists->matchTFPt16Overall, refHists->ptDenOverall);
    	EffTFPt20Overall->Divide(refHists->matchTFPt20Overall, refHists->ptDenOverall);
    	EffTFPt40Overall->Divide(refHists->matchTFPt40Overall, refHists->ptDenOverall);
    	EffTFPt60Overall->Divide(refHists->matchTFPt60Overall, refHists->ptDenOverall);

	EffTFPt10OverallQ1->Divide(refHists->matchTFPt10OverallQ1, refHists->ptDenOverall);
	EffTFPt12OverallQ1->Divide(refHists->matchTFPt12OverallQ1, refHists->ptDenOverall);
	EffTFPt16OverallQ1->Divide(refHists->matchTFPt16OverallQ1, refHists->ptDenOverall);
    	EffTFPt20OverallQ1->Divide(refHists->matchTFPt20OverallQ1, refHists->ptDenOverall);
    	EffTFPt40OverallQ1->Divide(refHists->matchTFPt40OverallQ1, refHists->ptDenOverall);
    	EffTFPt60OverallQ1->Divide(refHists->matchTFPt60OverallQ1, refHists->ptDenOverall);

	EffTFPt10OverallQ2->Divide(refHists->matchTFPt10OverallQ2, refHists->ptDenOverall);
	EffTFPt12OverallQ2->Divide(refHists->matchTFPt12OverallQ2, refHists->ptDenOverall);
	EffTFPt16OverallQ2->Divide(refHists->matchTFPt16OverallQ2, refHists->ptDenOverall);
    	EffTFPt20OverallQ2->Divide(refHists->matchTFPt20OverallQ2, refHists->ptDenOverall);
    	EffTFPt40OverallQ2->Divide(refHists->matchTFPt40OverallQ2, refHists->ptDenOverall);
    	EffTFPt60OverallQ2->Divide(refHists->matchTFPt60OverallQ2, refHists->ptDenOverall);

	EffTFPt10OverallQ3->Divide(refHists->matchTFPt10OverallQ3, refHists->ptDenOverall);
	EffTFPt12OverallQ3->Divide(refHists->matchTFPt12OverallQ3, refHists->ptDenOverall);
	EffTFPt16OverallQ3->Divide(refHists->matchTFPt16OverallQ3, refHists->ptDenOverall);
    	EffTFPt20OverallQ3->Divide(refHists->matchTFPt20OverallQ3, refHists->ptDenOverall);
    	EffTFPt40OverallQ3->Divide(refHists->matchTFPt40OverallQ3, refHists->ptDenOverall);
    	EffTFPt60OverallQ3->Divide(refHists->matchTFPt60OverallQ3, refHists->ptDenOverall);


    	EffTFPt10CSCOnly->Divide(refHists->matchTFPt10CSCOnly, refHists->ptDenCSCOnly);
	EffTFPt12CSCOnly->Divide(refHists->matchTFPt12CSCOnly, refHists->ptDenCSCOnly);
	EffTFPt16CSCOnly->Divide(refHists->matchTFPt16CSCOnly, refHists->ptDenCSCOnly);
    	EffTFPt20CSCOnly->Divide(refHists->matchTFPt20CSCOnly, refHists->ptDenCSCOnly);
    	EffTFPt40CSCOnly->Divide(refHists->matchTFPt40CSCOnly, refHists->ptDenCSCOnly);
    	EffTFPt60CSCOnly->Divide(refHists->matchTFPt60CSCOnly, refHists->ptDenCSCOnly);

    	EffTFPt10CSCOnlyQ1->Divide(refHists->matchTFPt10CSCOnlyQ1, refHists->ptDenCSCOnly);
	EffTFPt12CSCOnlyQ1->Divide(refHists->matchTFPt12CSCOnlyQ1, refHists->ptDenCSCOnly);
	EffTFPt16CSCOnlyQ1->Divide(refHists->matchTFPt16CSCOnlyQ1, refHists->ptDenCSCOnly);
    	EffTFPt20CSCOnlyQ1->Divide(refHists->matchTFPt20CSCOnlyQ1, refHists->ptDenCSCOnly);
    	EffTFPt40CSCOnlyQ1->Divide(refHists->matchTFPt40CSCOnlyQ1, refHists->ptDenCSCOnly);
    	EffTFPt60CSCOnlyQ1->Divide(refHists->matchTFPt60CSCOnlyQ1, refHists->ptDenCSCOnly);

    	EffTFPt10CSCOnlyQ2->Divide(refHists->matchTFPt10CSCOnlyQ2, refHists->ptDenCSCOnly);
	EffTFPt12CSCOnlyQ2->Divide(refHists->matchTFPt12CSCOnlyQ2, refHists->ptDenCSCOnly);
	EffTFPt16CSCOnlyQ2->Divide(refHists->matchTFPt16CSCOnlyQ2, refHists->ptDenCSCOnly);
    	EffTFPt20CSCOnlyQ2->Divide(refHists->matchTFPt20CSCOnlyQ2, refHists->ptDenCSCOnly);
    	EffTFPt40CSCOnlyQ2->Divide(refHists->matchTFPt40CSCOnlyQ2, refHists->ptDenCSCOnly);
    	EffTFPt60CSCOnlyQ2->Divide(refHists->matchTFPt60CSCOnlyQ2, refHists->ptDenCSCOnly);

    	EffTFPt10CSCOnlyQ3->Divide(refHists->matchTFPt10CSCOnlyQ3, refHists->ptDenCSCOnly);
	EffTFPt12CSCOnlyQ3->Divide(refHists->matchTFPt12CSCOnlyQ3, refHists->ptDenCSCOnly);
	EffTFPt16CSCOnlyQ3->Divide(refHists->matchTFPt16CSCOnlyQ3, refHists->ptDenCSCOnly);
    	EffTFPt20CSCOnlyQ3->Divide(refHists->matchTFPt20CSCOnlyQ3, refHists->ptDenCSCOnly);
    	EffTFPt40CSCOnlyQ3->Divide(refHists->matchTFPt40CSCOnlyQ3, refHists->ptDenCSCOnly);
    	EffTFPt60CSCOnlyQ3->Divide(refHists->matchTFPt60CSCOnlyQ3, refHists->ptDenCSCOnly);


    	EffTFPt10GEM->Divide(refHists->matchTFPt10GEM, refHists->ptDenGEM);
	EffTFPt12GEM->Divide(refHists->matchTFPt12GEM, refHists->ptDenGEM);
	EffTFPt16GEM->Divide(refHists->matchTFPt16GEM, refHists->ptDenGEM);
    	EffTFPt20GEM->Divide(refHists->matchTFPt20GEM, refHists->ptDenGEM);
    	EffTFPt40GEM->Divide(refHists->matchTFPt40GEM, refHists->ptDenGEM);
    	EffTFPt60GEM->Divide(refHists->matchTFPt60GEM, refHists->ptDenGEM);

    	EffTFPt10GEMQ1->Divide(refHists->matchTFPt10GEMQ1, refHists->ptDenGEM);
	EffTFPt12GEMQ1->Divide(refHists->matchTFPt12GEMQ1, refHists->ptDenGEM);
	EffTFPt16GEMQ1->Divide(refHists->matchTFPt16GEMQ1, refHists->ptDenGEM);
    	EffTFPt20GEMQ1->Divide(refHists->matchTFPt20GEMQ1, refHists->ptDenGEM);
    	EffTFPt40GEMQ1->Divide(refHists->matchTFPt40GEMQ1, refHists->ptDenGEM);
    	EffTFPt60GEMQ1->Divide(refHists->matchTFPt60GEMQ1, refHists->ptDenGEM);

    	EffTFPt10GEMQ2->Divide(refHists->matchTFPt10GEMQ2, refHists->ptDenGEM);
	EffTFPt12GEMQ2->Divide(refHists->matchTFPt12GEMQ2, refHists->ptDenGEM);
	EffTFPt16GEMQ2->Divide(refHists->matchTFPt16GEMQ2, refHists->ptDenGEM);
    	EffTFPt20GEMQ2->Divide(refHists->matchTFPt20GEMQ2, refHists->ptDenGEM);
    	EffTFPt40GEMQ2->Divide(refHists->matchTFPt40GEMQ2, refHists->ptDenGEM);
    	EffTFPt60GEMQ2->Divide(refHists->matchTFPt60GEMQ2, refHists->ptDenGEM);

    	EffTFPt10GEMQ3->Divide(refHists->matchTFPt10GEMQ3, refHists->ptDenGEM);
	EffTFPt12GEMQ3->Divide(refHists->matchTFPt12GEMQ3, refHists->ptDenGEM);
	EffTFPt16GEMQ3->Divide(refHists->matchTFPt16GEMQ3, refHists->ptDenGEM);
    	EffTFPt20GEMQ3->Divide(refHists->matchTFPt20GEMQ3, refHists->ptDenGEM);
    	EffTFPt40GEMQ3->Divide(refHists->matchTFPt40GEMQ3, refHists->ptDenGEM);
    	EffTFPt60GEMQ3->Divide(refHists->matchTFPt60GEMQ3, refHists->ptDenGEM);


    	EffTFPt10CSCRestricted->Divide(refHists->matchTFPt10CSCRestricted, refHists->ptDenCSCRestricted);
	EffTFPt12CSCRestricted->Divide(refHists->matchTFPt12CSCRestricted, refHists->ptDenCSCRestricted);
	EffTFPt16CSCRestricted->Divide(refHists->matchTFPt16CSCRestricted, refHists->ptDenCSCRestricted);
    	EffTFPt20CSCRestricted->Divide(refHists->matchTFPt20CSCRestricted, refHists->ptDenCSCRestricted);
    	EffTFPt40CSCRestricted->Divide(refHists->matchTFPt40CSCRestricted, refHists->ptDenCSCRestricted);
    	EffTFPt60CSCRestricted->Divide(refHists->matchTFPt60CSCRestricted, refHists->ptDenCSCRestricted);    

    	EffTFPt10CSCRestrictedQ1->Divide(refHists->matchTFPt10CSCRestrictedQ1, refHists->ptDenCSCRestricted);
	EffTFPt12CSCRestrictedQ1->Divide(refHists->matchTFPt12CSCRestrictedQ1, refHists->ptDenCSCRestricted);
	EffTFPt16CSCRestrictedQ1->Divide(refHists->matchTFPt16CSCRestrictedQ1, refHists->ptDenCSCRestricted);
    	EffTFPt20CSCRestrictedQ1->Divide(refHists->matchTFPt20CSCRestrictedQ1, refHists->ptDenCSCRestricted);
    	EffTFPt40CSCRestrictedQ1->Divide(refHists->matchTFPt40CSCRestrictedQ1, refHists->ptDenCSCRestricted);
    	EffTFPt60CSCRestrictedQ1->Divide(refHists->matchTFPt60CSCRestrictedQ1, refHists->ptDenCSCRestricted);    

    	EffTFPt10CSCRestrictedQ2->Divide(refHists->matchTFPt10CSCRestrictedQ2, refHists->ptDenCSCRestricted);
	EffTFPt12CSCRestrictedQ2->Divide(refHists->matchTFPt12CSCRestrictedQ2, refHists->ptDenCSCRestricted);
	EffTFPt16CSCRestrictedQ2->Divide(refHists->matchTFPt16CSCRestrictedQ2, refHists->ptDenCSCRestricted);
    	EffTFPt20CSCRestrictedQ2->Divide(refHists->matchTFPt20CSCRestrictedQ2, refHists->ptDenCSCRestricted);
    	EffTFPt40CSCRestrictedQ2->Divide(refHists->matchTFPt40CSCRestrictedQ2, refHists->ptDenCSCRestricted);
    	EffTFPt60CSCRestrictedQ2->Divide(refHists->matchTFPt60CSCRestrictedQ2, refHists->ptDenCSCRestricted);    

    	EffTFPt10CSCRestrictedQ3->Divide(refHists->matchTFPt10CSCRestrictedQ3, refHists->ptDenCSCRestricted);
	EffTFPt12CSCRestrictedQ3->Divide(refHists->matchTFPt12CSCRestrictedQ3, refHists->ptDenCSCRestricted);
	EffTFPt16CSCRestrictedQ3->Divide(refHists->matchTFPt16CSCRestrictedQ3, refHists->ptDenCSCRestricted);
    	EffTFPt20CSCRestrictedQ3->Divide(refHists->matchTFPt20CSCRestrictedQ3, refHists->ptDenCSCRestricted);
    	EffTFPt40CSCRestrictedQ3->Divide(refHists->matchTFPt40CSCRestrictedQ3, refHists->ptDenCSCRestricted);
    	EffTFPt60CSCRestrictedQ3->Divide(refHists->matchTFPt60CSCRestrictedQ3, refHists->ptDenCSCRestricted);    


    	EffTFPt10DTOnly->Divide(refHists->matchTFPt10DTOnly, refHists->ptDenDTOnly);
	EffTFPt12DTOnly->Divide(refHists->matchTFPt12DTOnly, refHists->ptDenDTOnly);
	EffTFPt16DTOnly->Divide(refHists->matchTFPt16DTOnly, refHists->ptDenDTOnly);
    	EffTFPt20DTOnly->Divide(refHists->matchTFPt20DTOnly, refHists->ptDenDTOnly);
    	EffTFPt40DTOnly->Divide(refHists->matchTFPt40DTOnly, refHists->ptDenDTOnly);
    	EffTFPt60DTOnly->Divide(refHists->matchTFPt60DTOnly, refHists->ptDenDTOnly);

    	EffTFPt10DTOnlyQ1->Divide(refHists->matchTFPt10DTOnlyQ1, refHists->ptDenDTOnly);
	EffTFPt12DTOnlyQ1->Divide(refHists->matchTFPt12DTOnlyQ1, refHists->ptDenDTOnly);
	EffTFPt16DTOnlyQ1->Divide(refHists->matchTFPt16DTOnlyQ1, refHists->ptDenDTOnly);
    	EffTFPt20DTOnlyQ1->Divide(refHists->matchTFPt20DTOnlyQ1, refHists->ptDenDTOnly);
    	EffTFPt40DTOnlyQ1->Divide(refHists->matchTFPt40DTOnlyQ1, refHists->ptDenDTOnly);
    	EffTFPt60DTOnlyQ1->Divide(refHists->matchTFPt60DTOnlyQ1, refHists->ptDenDTOnly);

    	EffTFPt10DTOnlyQ2->Divide(refHists->matchTFPt10DTOnlyQ2, refHists->ptDenDTOnly);
	EffTFPt12DTOnlyQ2->Divide(refHists->matchTFPt12DTOnlyQ2, refHists->ptDenDTOnly);
	EffTFPt16DTOnlyQ2->Divide(refHists->matchTFPt16DTOnlyQ2, refHists->ptDenDTOnly);
    	EffTFPt20DTOnlyQ2->Divide(refHists->matchTFPt20DTOnlyQ2, refHists->ptDenDTOnly);
    	EffTFPt40DTOnlyQ2->Divide(refHists->matchTFPt40DTOnlyQ2, refHists->ptDenDTOnly);
    	EffTFPt60DTOnlyQ2->Divide(refHists->matchTFPt60DTOnlyQ2, refHists->ptDenDTOnly);

    	EffTFPt10DTOnlyQ3->Divide(refHists->matchTFPt10DTOnlyQ3, refHists->ptDenDTOnly);
	EffTFPt12DTOnlyQ3->Divide(refHists->matchTFPt12DTOnlyQ3, refHists->ptDenDTOnly);
	EffTFPt16DTOnlyQ3->Divide(refHists->matchTFPt16DTOnlyQ3, refHists->ptDenDTOnly);
    	EffTFPt20DTOnlyQ3->Divide(refHists->matchTFPt20DTOnlyQ3, refHists->ptDenDTOnly);
    	EffTFPt40DTOnlyQ3->Divide(refHists->matchTFPt40DTOnlyQ3, refHists->ptDenDTOnly);
    	EffTFPt60DTOnlyQ3->Divide(refHists->matchTFPt60DTOnlyQ3, refHists->ptDenDTOnly);


    	EffTFPt10Overlap->Divide(refHists->matchTFPt10Overlap, refHists->ptDenOverlap);
	EffTFPt12Overlap->Divide(refHists->matchTFPt12Overlap, refHists->ptDenOverlap);
    	EffTFPt16Overlap->Divide(refHists->matchTFPt16Overlap, refHists->ptDenOverlap);
	EffTFPt20Overlap->Divide(refHists->matchTFPt20Overlap, refHists->ptDenOverlap);
    	EffTFPt40Overlap->Divide(refHists->matchTFPt40Overlap, refHists->ptDenOverlap);
    	EffTFPt60Overlap->Divide(refHists->matchTFPt60Overlap, refHists->ptDenOverlap);

    	EffTFPt10OverlapQ1->Divide(refHists->matchTFPt10OverlapQ1, refHists->ptDenOverlap);
	EffTFPt12OverlapQ1->Divide(refHists->matchTFPt12OverlapQ1, refHists->ptDenOverlap);
    	EffTFPt16OverlapQ1->Divide(refHists->matchTFPt16OverlapQ1, refHists->ptDenOverlap);
	EffTFPt20OverlapQ1->Divide(refHists->matchTFPt20OverlapQ1, refHists->ptDenOverlap);
    	EffTFPt40OverlapQ1->Divide(refHists->matchTFPt40OverlapQ1, refHists->ptDenOverlap);
    	EffTFPt60OverlapQ1->Divide(refHists->matchTFPt60OverlapQ1, refHists->ptDenOverlap);

    	EffTFPt10OverlapQ2->Divide(refHists->matchTFPt10OverlapQ2, refHists->ptDenOverlap);
	EffTFPt12OverlapQ2->Divide(refHists->matchTFPt12OverlapQ2, refHists->ptDenOverlap);
    	EffTFPt16OverlapQ2->Divide(refHists->matchTFPt16OverlapQ2, refHists->ptDenOverlap);
	EffTFPt20OverlapQ2->Divide(refHists->matchTFPt20OverlapQ2, refHists->ptDenOverlap);
    	EffTFPt40OverlapQ2->Divide(refHists->matchTFPt40OverlapQ2, refHists->ptDenOverlap);
    	EffTFPt60OverlapQ2->Divide(refHists->matchTFPt60OverlapQ2, refHists->ptDenOverlap);

    	EffTFPt10OverlapQ3->Divide(refHists->matchTFPt10OverlapQ3, refHists->ptDenOverlap);
	EffTFPt12OverlapQ3->Divide(refHists->matchTFPt12OverlapQ3, refHists->ptDenOverlap);
    	EffTFPt16OverlapQ3->Divide(refHists->matchTFPt16OverlapQ3, refHists->ptDenOverlap);
	EffTFPt20OverlapQ3->Divide(refHists->matchTFPt20OverlapQ3, refHists->ptDenOverlap);
    	EffTFPt40OverlapQ3->Divide(refHists->matchTFPt40OverlapQ3, refHists->ptDenOverlap);
    	EffTFPt60OverlapQ3->Divide(refHists->matchTFPt60OverlapQ3, refHists->ptDenOverlap);


    	EffTFPt10HighEta->Divide(refHists->matchTFPt10HighEta, refHists->ptDenHighEta);
	EffTFPt12HighEta->Divide(refHists->matchTFPt12HighEta, refHists->ptDenHighEta);
    	EffTFPt16HighEta->Divide(refHists->matchTFPt16HighEta, refHists->ptDenHighEta);
	EffTFPt20HighEta->Divide(refHists->matchTFPt20HighEta, refHists->ptDenHighEta);
    	EffTFPt40HighEta->Divide(refHists->matchTFPt40HighEta, refHists->ptDenHighEta);
    	EffTFPt60HighEta->Divide(refHists->matchTFPt60HighEta, refHists->ptDenHighEta);

    	EffTFPt10HighEtaQ1->Divide(refHists->matchTFPt10HighEtaQ1, refHists->ptDenHighEta);
	EffTFPt12HighEtaQ1->Divide(refHists->matchTFPt12HighEtaQ1, refHists->ptDenHighEta);
    	EffTFPt16HighEtaQ1->Divide(refHists->matchTFPt16HighEtaQ1, refHists->ptDenHighEta);
	EffTFPt20HighEtaQ1->Divide(refHists->matchTFPt20HighEtaQ1, refHists->ptDenHighEta);
    	EffTFPt40HighEtaQ1->Divide(refHists->matchTFPt40HighEtaQ1, refHists->ptDenHighEta);
    	EffTFPt60HighEtaQ1->Divide(refHists->matchTFPt60HighEtaQ1, refHists->ptDenHighEta);

    	EffTFPt10HighEtaQ2->Divide(refHists->matchTFPt10HighEtaQ2, refHists->ptDenHighEta);
	EffTFPt12HighEtaQ2->Divide(refHists->matchTFPt12HighEtaQ2, refHists->ptDenHighEta);
    	EffTFPt16HighEtaQ2->Divide(refHists->matchTFPt16HighEtaQ2, refHists->ptDenHighEta);
	EffTFPt20HighEtaQ2->Divide(refHists->matchTFPt20HighEtaQ2, refHists->ptDenHighEta);
    	EffTFPt40HighEtaQ2->Divide(refHists->matchTFPt40HighEtaQ2, refHists->ptDenHighEta);
    	EffTFPt60HighEtaQ2->Divide(refHists->matchTFPt60HighEtaQ2, refHists->ptDenHighEta);

    	EffTFPt10HighEtaQ3->Divide(refHists->matchTFPt10HighEtaQ3, refHists->ptDenHighEta);
	EffTFPt12HighEtaQ3->Divide(refHists->matchTFPt12HighEtaQ3, refHists->ptDenHighEta);
    	EffTFPt16HighEtaQ3->Divide(refHists->matchTFPt16HighEtaQ3, refHists->ptDenHighEta);
	EffTFPt20HighEtaQ3->Divide(refHists->matchTFPt20HighEtaQ3, refHists->ptDenHighEta);
    	EffTFPt40HighEtaQ3->Divide(refHists->matchTFPt40HighEtaQ3, refHists->ptDenHighEta);
    	EffTFPt60HighEtaQ3->Divide(refHists->matchTFPt60HighEtaQ3, refHists->ptDenHighEta);



    	//EffPt->Divide(refHists->matchPt, refHists->fidPtDen);
	EffPtOverall->Divide(refHists->matchPtOverall, refHists->ptDenOverall);
	EffPtOverallQ1->Divide(refHists->matchPtOverallQ1, refHists->ptDenOverall);
	EffPtOverallQ2->Divide(refHists->matchPtOverallQ2, refHists->ptDenOverall);
	EffPtOverallQ3->Divide(refHists->matchPtOverallQ3, refHists->ptDenOverall);
    	EffPtCSCOnly->Divide(refHists->matchPtCSCOnly, refHists->ptDenCSCOnly);
    	EffPtCSCOnlyQ1->Divide(refHists->matchPtCSCOnlyQ1, refHists->ptDenCSCOnly);
    	EffPtCSCOnlyQ2->Divide(refHists->matchPtCSCOnlyQ2, refHists->ptDenCSCOnly);
    	EffPtCSCOnlyQ3->Divide(refHists->matchPtCSCOnlyQ3, refHists->ptDenCSCOnly);
    	EffPtGEM->Divide(refHists->matchPtGEM, refHists->ptDenGEM);
    	EffPtGEMQ1->Divide(refHists->matchPtGEMQ1, refHists->ptDenGEM);
    	EffPtGEMQ2->Divide(refHists->matchPtGEMQ2, refHists->ptDenGEM);
    	EffPtGEMQ3->Divide(refHists->matchPtGEMQ3, refHists->ptDenGEM);
	EffPtCSCRestricted->Divide(refHists->matchPtCSCRestricted, refHists->ptDenCSCRestricted);
	EffPtCSCRestrictedQ1->Divide(refHists->matchPtCSCRestrictedQ1, refHists->ptDenCSCRestricted);
	EffPtCSCRestrictedQ2->Divide(refHists->matchPtCSCRestrictedQ2, refHists->ptDenCSCRestricted);
	EffPtCSCRestrictedQ3->Divide(refHists->matchPtCSCRestrictedQ3, refHists->ptDenCSCRestricted);
    	EffPtDTOnly->Divide(refHists->matchPtDTOnly, refHists->ptDenDTOnly); 
    	EffPtDTOnlyQ1->Divide(refHists->matchPtDTOnlyQ1, refHists->ptDenDTOnly); 
    	EffPtDTOnlyQ2->Divide(refHists->matchPtDTOnlyQ2, refHists->ptDenDTOnly); 
    	EffPtDTOnlyQ3->Divide(refHists->matchPtDTOnlyQ3, refHists->ptDenDTOnly); 
    	EffPtOverlap->Divide(refHists->matchPtOverlap, refHists->ptDenOverlap);
    	EffPtOverlapQ1->Divide(refHists->matchPtOverlapQ1, refHists->ptDenOverlap);
    	EffPtOverlapQ2->Divide(refHists->matchPtOverlapQ2, refHists->ptDenOverlap);
    	EffPtOverlapQ3->Divide(refHists->matchPtOverlapQ3, refHists->ptDenOverlap);
    	EffPtHighEta->Divide(refHists->matchPtHighEta, refHists->ptDenHighEta);
    	EffPtHighEtaQ1->Divide(refHists->matchPtHighEtaQ1, refHists->ptDenHighEta);
    	EffPtHighEtaQ2->Divide(refHists->matchPtHighEtaQ2, refHists->ptDenHighEta);
    	EffPtHighEtaQ3->Divide(refHists->matchPtHighEtaQ3, refHists->ptDenHighEta);
	
    	EffEtaAll->Divide(refHists->matchEta, refHists->Eta);
	EffEtaQ3->Divide(refHists->EtaQ3, refHists->Eta);
    	EffEtaQ2->Divide(refHists->EtaQ2, refHists->Eta);
    	EffEtaQ1->Divide(refHists->EtaQ1, refHists->Eta);
	EffSignedEtaAll->Divide(refHists->signedMatchEta, refHists->signedEta);
    	EffSignedEtaQ3->Divide(refHists->signedEtaQ3, refHists->signedEta);
    	EffSignedEtaQ2->Divide(refHists->signedEtaQ2, refHists->signedEta);
    	EffSignedEtaQ1->Divide(refHists->signedEtaQ1, refHists->signedEta);

    	//EffPhi->Divide(refHists->matchPhi, refHists->Phi);
    	EffPhi_mod_10_Q2_endcap1->Divide(refHists->matchPhi_mod_10_Q2_endcap1,refHists->Phi_mod_10_endcap1);
    	EffPhi_mod_10_Q3_endcap1->Divide(refHists->matchPhi_mod_10_Q3_endcap1,refHists->Phi_mod_10_endcap1);
    	EffPhi_mod_10_Q2_endcap2->Divide(refHists->matchPhi_mod_10_Q2_endcap2,refHists->Phi_mod_10_endcap2);
    	EffPhi_mod_10_Q3_endcap2->Divide(refHists->matchPhi_mod_10_Q3_endcap2,refHists->Phi_mod_10_endcap2);

	EffPhi->Divide(refHists->matchPhi, refHists->Phi);
    	EffPhiQ3->Divide(refHists->PhiQ3, refHists->Phi);
    	EffPhiQ2->Divide(refHists->PhiQ2, refHists->Phi);
    	EffPhiQ1->Divide(refHists->PhiQ1, refHists->Phi);

}

void EffHistogramList::computePtPlateauEff(std::ofstream* PtStats, std::vector<double> PlateauDefinitions, std::vector<std::string> thresholds, std::vector<TH1F*> PtEffHists)
{
	std::vector<TH1F*>::iterator iHist;
	std::vector<std::string>::iterator iThreshold;
	
	TF1* constFit = new TF1("constFit", "[0]", 2., 140.);
	
	Double_t xmin;
	int i=0;
	for(iHist=PtEffHists.begin();iHist!=PtEffHists.end();iHist++)
	{
		xmin = PlateauDefinitions[i]; //define start of plateau
		PtEffHists[i] -> Fit(constFit,"0R","", xmin, 140.); //do fit
		if(i==0) (*PtStats)<<"\nDefault Pt thresh   Efficiency:  "<<constFit->GetParameter(0)<<"   "; // efficiency of plateau
		else (*PtStats)<<"\nPt>"<<thresholds[i].c_str()<<"               Efficiency:  "<<constFit->GetParameter(0)<<"   "; // efficiency of plateau
		
		(*PtStats)<<"Error:   "<<constFit->GetParError(0); // error on the plateau efficiency
		i++;
	}
}
  
  
  
  
  
  
Double_t thresh(Double_t* pt, Double_t* par)
  {
    Double_t fitval = (0.5*TMath::Erf((pt[0]/par[0] + 1.0)/(TMath::Sqrt(2.0)*par[1])) + 0.5*TMath::Erf((pt[0]/par[0] - 1.0)/(TMath::Sqrt(2.0)*par[1])) )*(par[2] + par[3]*pt[0]);
    return fitval;
  }
}
