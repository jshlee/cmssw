#include <vector>
#include <fstream>
#include <sstream>
#include "TTree.h"
#include "TFile.h"
#include "TMath.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TGraphAsymmErrors.h"
#include "TPaveStats.h"
#include "TStyle.h"
#include "TEfficiency.h"

#include "TStyle.h"

template<typename T>

TH1F * makeRatio2(TH1F * plot1, TH1F * plot2, float a1 = 0, float a2 = 2){

	TH1F * plotTMP = (TH1F*)plot2->Clone();
	plotTMP->SetStats(kFALSE);
	plotTMP->SetLineColor(1);
  	plotTMP->SetMaximum(a2);
  	plotTMP->SetMinimum(a1);
	plotTMP->Divide(plot1);
	return plotTMP;

}

void extractPerformance(TH1F * histo1, TH1F * histo2, std::string name, std::string range){

	//denominator numerator

	TH1F * plotTMP = (TH1F*)histo2->Clone();
	plotTMP->Divide(histo1);

	cout<<"Range: "<<range.c_str()<<endl;

	for(int i = 1; i <= plotTMP->GetNbinsX(); i++){

		double etaMin = 0, etaMax = 0;
		double binWidth = plotTMP->GetBinWidth(i);
		double eff1 = histo1->GetBinContent(i);
		double eff2 = histo2->GetBinContent(i);
		double scale = plotTMP->GetBinContent(i);
		etaMin = binWidth*(i-1);
		etaMax = binWidth*i;
		cout<<"|Eta|: "<<etaMin<<"-"<<etaMax<<(" "+name).c_str()<<" NoAge(50): "<<eff1<<" Age(140): "<<eff2<<" ScaleFactor: "<<scale<<endl;

	}

}

//---------------------------------------------------------------------------------------------------------------------------

typedef std::vector<TH1D*> vecHist;

std::vector<TH1D*> projectAndFit(TH2F * Histo2D, std::string postFix = "bin"){

	std::vector<TH1D*> vec;

	for(int i = 1; i <= Histo2D->GetNbinsX(); i++){

		std::stringstream ss;
		ss<<postFix.c_str()<<i;	
		std::string pf = ss.str();

		TH1D * proj1 = Histo2D->ProjectionY(pf.c_str(),i,i);
		if(proj1->GetEntries() == 0) continue;

		//size_t found1 = postFix.find("200GeV");
		//size_t found2 = postFix.find("500GeV");
		//size_t found3 = postFix.find("1000GeV");

		vec.push_back(proj1);

	}

	return vec;

}

TH1F * produceYield(TH1F * h1, TH1F * h2, int num, bool rebinAsym = false, bool rebin = false){

	//Double_t bin[] = {0.,5.,10.,15.,20.,25.,30.,40.,50.,70.,100.,2500.};
	Double_t bin[] = {0.,5.,15.,25.,35.,50.,70.,100.,2500.};
	Double_t binEta[] = {0.0,0.2,0.4,0.6,0.8,1.0,1.2,1.4,1.6,1.8,2.0,2.2,2.4,2.5};

	h1->Sumw2();
	h2->Sumw2();

	if(rebinAsym){

		TH1F * h1New = h1->Rebin(8,h1->GetName(),bin);
		TH1F * h2New = h2->Rebin(8,h2->GetName(),bin);
		h1 = (TH1F*)h1New->Clone();
		h2 = (TH1F*)h2New->Clone();

	}

	if(rebin){

		TH1F * h1EtaNew = h1->Rebin(13,h1->GetName(),binEta);
		TH1F * h2EtaNew = h2->Rebin(13,h2->GetName(),binEta);
		h1 = (TH1F*)h1EtaNew->Clone();
		h2 = (TH1F*)h2EtaNew->Clone();

	}

	TH1F * hTemp = (TH1F*)h1->Clone();
	hTemp->Reset();

	for(int i = 1; i < hTemp->GetSize(); i++){

		float diff = (h1->GetBinContent(i) - h2->GetBinContent(i));
		float div = diff/(float)num;
		if(rebinAsym || rebin) div /= (float)h1->GetBinWidth(i);
		float err = sqrt(diff)/(float)num;
		if(rebinAsym || rebin) err /= (float)h1->GetBinWidth(i);
		hTemp->SetBinContent(i, div);
		hTemp->SetBinError(i, err);

		//std::cout<<diff<<" "<<err<<std::endl;

	}
 
	return hTemp;

}

TEfficiency * produceFR(TH1F * h1, TH1F * h2, TH1F * h3, bool rebinAsym = false, bool rebin = false){

	//Double_t bin[] = {0.,5.,10.,15.,20.,25.,30.,40.,50.,70.,100.,2500.};
	Double_t bin[] = {0.,5.,15.,25.,35.,50.,70.,100.,2500.};
	Double_t binEta[] = {0.0,0.2,0.4,0.6,0.8,1.0,1.2,1.4,1.6,1.8,2.0,2.2,2.4,2.5};

	h1->Sumw2();
	h2->Sumw2();

	if(rebinAsym){

		TH1F * h1New = h1->Rebin(8,h1->GetName(),bin);
		TH1F * h2New = h2->Rebin(8,h2->GetName(),bin);
		h1 = (TH1F*)h1New->Clone();
		h2 = (TH1F*)h2New->Clone();

	}

	if(rebin){

		TH1F * h1EtaNew = h1->Rebin(13,h1->GetName(),binEta);
		TH1F * h2EtaNew = h2->Rebin(13,h2->GetName(),binEta);
		h1 = (TH1F*)h1EtaNew->Clone();
		h2 = (TH1F*)h2EtaNew->Clone();

	}

	TH1F * hTemp = (TH1F*)h1->Clone();
	hTemp->Sumw2();
	hTemp->Add(h2, -1);
	TH1F * ngul = (TH1F*)h1->Clone();
	ngul->Reset();

	for(int i = 1; i < hTemp->GetSize(); i++){

		if(hTemp->GetBinContent(i) == 0) continue;
		for(int j = 1; j <= hTemp->GetBinContent(i); j++){

			ngul->Fill(hTemp->GetBinCenter(i));

		}

	}

        TEfficiency* effTMP = 0;
        if(TEfficiency::CheckConsistency(*ngul,*h1))
        {
            	effTMP = new TEfficiency(*ngul,*h1);
		//effTMP->SetStatisticOption(TEfficiency::kFFC);
		//effTMP->SetConfidenceLevel(0.683);
        }

	//hTemp->Divide(hTemp,h1,1,1,"cl=0.683 b(1,1) mode");
 
	return effTMP;

}

void makeSuperImp(vecHist p1, vecHist p2, vecHist p3, std::string name){

	gStyle->SetOptStat(112200);
	//gStyle->SetOptFit(12);

	TCanvas * canvasTMP = new TCanvas("canvasTMP","canvas",700,700);
	canvasTMP->SetLogy();

	for(int i = 0; i<(int)p1.size(); i++){

		std::string bin = "_bin";
		std::stringstream ss;
	  	ss<<bin.c_str()<<i;	
		std::string pf = ss.str();

		TH1D * tmp1 = p1[i];
		TH1D * tmp2 = p2[i];
		TH1D * tmp3 = p3[i];

		tmp1->Rebin(10);
		tmp2->Rebin(10);
		tmp3->Rebin(10);

		tmp1->Scale(1/tmp1->GetEntries());
		tmp2->Scale(1/tmp2->GetEntries());
		tmp3->Scale(1/tmp3->GetEntries());

		//tmp1->GetXaxis()->SetRangeUser(-1,+1);
		//tmp2->GetXaxis()->SetRangeUser(-1,+1);
		//tmp3->GetXaxis()->SetRangeUser(-1,+1);
		
		tmp1->SetLineWidth(2);
		tmp2->SetLineWidth(2);
		tmp3->SetLineWidth(2);

		tmp1->SetMaximum(1.0);
	 	tmp1->SetLineColor(9);
		tmp1->SetTitle("");
		tmp1->GetXaxis()->SetTitle("q/p_{T}");
		tmp1->GetYaxis()->SetTitle("entries");
		tmp1->Draw("HIST");

	 	tmp2->SetLineColor(2);
		tmp2->Draw("HISTSAMES");
	 	tmp3->SetLineColor(kGreen-3);
		//tmp3->SetLineStyle(2);
		tmp3->Draw("HISTSAMES");

		canvasTMP->Update();
		TPaveStats *stats1 = (TPaveStats*)tmp1->GetListOfFunctions()->FindObject("stats");
		stats1->SetX1NDC(0.10);
		stats1->SetX2NDC(0.40);
		stats1->SetY1NDC(0.80);
		stats1->SetY2NDC(0.95);
   		stats1->SetFillColor(9);
		canvasTMP->Update();
		TPaveStats *stats2 = (TPaveStats*)tmp2->GetListOfFunctions()->FindObject("stats");
		stats2->SetX1NDC(0.60);
		stats2->SetX2NDC(0.90);
		stats2->SetY1NDC(0.80);
		stats2->SetY2NDC(0.95);
   		stats2->SetFillColor(2);
		canvasTMP->Update();
		TPaveStats *stats3 = (TPaveStats*)tmp3->GetListOfFunctions()->FindObject("stats");
		stats3->SetX1NDC(0.10);
		stats3->SetX2NDC(0.40);
		stats3->SetY1NDC(0.60);
		stats3->SetY2NDC(0.75);
   		stats3->SetFillColor(kGreen-3);

		canvasTMP->SaveAs((name+pf+".png").c_str());

	}

}

void setLeg(TLegend * leg, string header = ""){

	leg->SetFillColor(0);
	leg->SetFillStyle(0);
	leg->SetLineColor(0);
	leg->SetBorderSize(0); 
	leg->SetHeader(header.c_str());
   	leg->SetTextSize(0.03);

}

void changeOffsetY(TAxis * yaxis){

  yaxis->SetTitleOffset(1.5);

}

void changeAxis(TAxis * yaxis, double offSet = 1.5, double offSetLabels = 0.006, double titleSize = 0.035){

// For the axis titles:

  yaxis->SetTitleColor(1);
  yaxis->SetTitleFont(42);
  yaxis->SetTitleSize(titleSize);
  yaxis->SetTitleOffset(offSet);
  // tdrStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
  // tdrStyle->SetTitleYSize(Float_t size = 0.02);
  // yaxis->SetTitleXOffset(0.9);
  // yaxis->SetTitleYOffset(1.25);
  // tdrStyle->SetTitleOffset(1.1, "Y"); // Another way to set the Offset

// For the axis labels:

  yaxis->SetLabelColor(1);
  yaxis->SetLabelFont(42);
  yaxis->SetLabelOffset(offSetLabels);
  yaxis->SetLabelSize(0.035);

// For the axis:

  yaxis->SetAxisColor(1);
  //yaxis->SetStripDecimals(kTRUE);
  yaxis->SetTickLength(0.03);
  yaxis->SetNdivisions(510);
  //yaxis->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
  //yaxis->SetPadTickY(1);

}

void setMarkerStyle(TH1F * histo, int color, int style){

	histo->SetMarkerColor(color);
	histo->SetLineColor(color);
	histo->SetLineWidth(2);
	histo->SetMarkerStyle(style);
	histo->SetMarkerSize(1.5);

}

void setMarkerStyle(TProfile * histo, int color, int style){

	histo->SetMarkerColor(color);
	histo->SetLineColor(color);
	histo->SetLineWidth(2);
	histo->SetMarkerStyle(style);
	histo->SetMarkerSize(1.5);

}

void setMarkerStyle(TEfficiency * histo, int color, int style){

	histo->SetMarkerColor(color);
	histo->SetLineColor(color);
	histo->SetLineWidth(2);
	histo->SetMarkerStyle(style);
	histo->SetMarkerSize(1.5);

}

void setMarkerStyle2(TH1F * histo, Color_t color, int style){

	histo->SetMarkerColor(color);
	histo->SetLineColor(color);
	histo->SetLineWidth(2);
	histo->SetMarkerStyle(style);
	histo->SetMarkerSize(1.5);

}

void setMarkerStyle2(TProfile * histo, Color_t color, int style){

	histo->SetMarkerColor(color);
	histo->SetLineColor(color);
	histo->SetLineWidth(2);
	histo->SetMarkerStyle(style);
	histo->SetMarkerSize(1.5);

}

void setMarkerStyle2(TEfficiency * histo, Color_t color, int style){

	histo->SetMarkerColor(color);
	histo->SetLineColor(color);
	histo->SetLineWidth(2);
	histo->SetMarkerStyle(style);
	histo->SetMarkerSize(1.5);

}

void saveAll(TCanvas * canvas, string name, string type){

	canvas->SaveAs((type + "/" + name + ".png").c_str());
	canvas->SaveAs((type + "/" + name + ".pdf").c_str());

}

TEfficiency * calcEff(TH1F* num, TH1F* den, bool remove = false, bool rebinAsym = false, bool rebin = false, int i){

	Double_t bin[] = {0.,5.,15.,25.,35.,50.,70.,100.,2500.};
	Double_t binEta[] = {0.0,0.2,0.4,0.6,0.8,1.0,1.2,1.4,1.6,1.8,2.0,2.2,2.4,2.5};
    
    //std::cout<<i<<" "<<num->GetMean()<<" "<<den->GetMean()<<std::endl;
    
    std::vector<int> vec, vec2, vec3;
    vec.push_back(0);
    vec2.push_back(10);
    vec2.push_back(11);
    vec2.push_back(19);
    vec2.push_back(18);
    vec3.push_back(14);
    vec3.push_back(15);
    vec3.push_back(16);
    vec3.push_back(24);
    vec3.push_back(25);
    
    if(i == 1 || i == 4) vec = vec2;
    else if(i == 2 || i == 5) vec = vec3;

	if(remove){
        
        for(int k = 0; k < vec.size(); k++){
        
            removeBin(num, vec[k]);
            removeBin(den, vec[k]);
        
        }

	}

    /*for (int i = 1; i < den->GetNbinsX()+1; i++){

		const double errLo = TEfficiency::FeldmanCousins((int)den->GetBinContent(i), 
							       (int)num->GetBinContent(i),
							       0.683,false);
		const double errUp = TEfficiency::FeldmanCousins((int)den->GetBinContent(i), 
							       (int)num->GetBinContent(i),
							       0.683,true);

		std::cout<<" "<<errLo<<" "<<errUp<<std::endl;

	}*/

	if(rebin){

		TH1F * numEtaNew = num->Rebin(13,num->GetName(),binEta);
		TH1F * denEtaNew = den->Rebin(13,den->GetName(),binEta);
		num = numEtaNew;
		den = denEtaNew;

	}

	if(rebinAsym){

		TH1F * numNew = num->Rebin(8,num->GetName(),bin);
		TH1F * denNew = den->Rebin(8,den->GetName(),bin);
		num = numNew;
		den = denNew;

	}

        TEfficiency* effTMP = 0;
        if(TEfficiency::CheckConsistency(*num,*den))
        {
            	effTMP = new TEfficiency(*num,*den);
		//effTMP->SetStatisticOption(TEfficiency::kFFC);
		//effTMP->SetConfidenceLevel(0.683);
        }

	return effTMP;

}

void removeBin(TH1F * histo, int bin){

	histo->SetBinContent(bin, 0);
	histo->SetBinError(bin, 0);

}

void makeDiff(std::vector<TH1F*> genericVector, std::vector<TH1F*> genericVector2){

	for(int i = 0; i < genericVector.size(); i++){

		//std::cout<<genericVector[i]->GetBinContent(10)<<std::endl;
		genericVector[i]->Add(genericVector2[i], -1);
		//std::cout<<genericVector[i]->GetBinContent(10)<<std::endl;

	}

}

std::vector<TH1F*> makeMismatch(std::vector<TH1F*> genericVector, std::vector<TH1F*> genericVector2){

	//loose tight
	
	std::vector<TH1F*> tmpVec;

	for(int i = 0; i < genericVector.size(); i++){//loop sugli istogrammi

		TH1F * tmp = (TH1F*)genericVector[i]->Clone();
		tmp->Reset();

		for(int j = 1; j < genericVector[i]->GetSize(); j++){

			float bin1 = genericVector[i]->GetBinContent(j);
			float bin2 = genericVector2[i]->GetBinContent(j);

			float ratio = 0;
			float err = 0;
			if(bin1 > 0) {

				ratio = (bin1-bin2)/bin1;
				err = sqrt(bin1*ratio*(1-ratio))/bin1;

			}

			//std::cout<<bin1<<" "<<bin2<<" "<<ratio<<" "<<err<<std::endl;

			tmp->SetBinContent(j, ratio);
			tmp->SetBinError(j, err);

		}

		tmpVec.push_back(tmp);

	}

	//std::cout<<tmpVec.size()<<std::endl;
	return tmpVec;

}

std::vector<TH1F*> sumHistos(std::vector<TH1F*> v1, std::vector<TH1F*> v2, std::vector<TH1F*> v3, std::vector<TH1F*> v4){
	
	std::vector<TH1F*> tmpVec;

	for(int i = 0; i < v1.size(); i++){

		TH1F * tmp = (TH1F*)v1[i]->Clone();
		tmp->Sumw2();
		v1[i]->Sumw2();
		v2[i]->Sumw2();
		v3[i]->Sumw2();
		v4[i]->Sumw2();
		tmp->Add(v2[i]);
		tmp->Add(v3[i]);
		tmp->Add(v4[i]);
		tmpVec.push_back(tmp);

		//std::cout<<v1[i]->GetBinContent(1)<<" "<<v2[i]->GetBinContent(1)<<" "<<v3[i]->GetBinContent(1)<<" "<<v4[i]->GetBinContent(1)<<" "<<tmp->GetBinContent(1)<<std::endl;

	}

	return tmpVec;

}

void setEverything(std::vector<TH1F*> genericVector, double min, double max, bool log, double x1, double y1, double x2, double y2, std::string name3, std::string type, std::string title, std::string save, bool logx = false, std::string ptRange = ""){

        std::vector<int> markers;
		markers.push_back(20);
		markers.push_back(21);
		markers.push_back(22);
		markers.push_back(23);
		markers.push_back(34);
		markers.push_back(24);
		markers.push_back(26);
		markers.push_back(25);

  		gROOT->LoadMacro("tdrstyle.C");
  		setTDRStyle();

  		TPaveText * cmsPreliminaryLabel = new TPaveText(0.13, 0.93, 0.99, 1.02, "NDC");
  		cmsPreliminaryLabel->AddText("CMS Simulation Preliminary              #sqrt{s} = 14 TeV");
  		cmsPreliminaryLabel->SetTextAlign(12);
  		cmsPreliminaryLabel->SetTextFont(42);
  		cmsPreliminaryLabel->SetTextSize(0.04);
  		cmsPreliminaryLabel->SetFillStyle(0);
  		cmsPreliminaryLabel->SetBorderSize(0);
		//float a1 = 0.223, a2 = 0.42, b1 = 0.99, b2 = 0.52;
		float a1 = 0.55, a2 = 0.85, b1 = 0.8, b2 = 0.95; 
		/*if(save.find("Fakes") != std::string::npos){

			a1 = 0.172;
			a2 = 0.82;
			b1 = 0.99; 
			b2 = 0.92;

		}*/
  		TPaveText * label = new TPaveText(a1, a2, b1, b2, "NDC");
  		//label->AddText("Single Muon p_{T} = 100 GeV");
  		label->AddText("NO MUON SYSTEM AGING");
  		label->SetTextAlign(12);
  		label->SetTextFont(62);
  		label->SetTextSize(0.03);
  		label->SetFillStyle(0);
  		label->SetBorderSize(0);

	  	TLegend *leg2 = new TLegend(x1,y1,x2,y2);
		std::string header = "";
		if(name3 == "Muon10") header = "Muon p_{T} = 10 GeV";
		//else if(name3 == "Muon100") header = "Muon p_{T} = 100 GeV/c";
		else if(name3 == "Muon100") header = "Standalone Muon reconstruction";
		else if (name3 == "DY") header = "Z/#gamma* #rightarrow #mu#mu";
		header += ptRange;

		std::string addString2 = "";
		/*if(type.find("MuonLoose") != std::string::npos) addString2 = ", Loose Muon ID";
		else if(type.find("MuonTight") != std::string::npos) addString2 = ", Tight Muon ID";
		else addString2 = "";*/

		std::string addString3 = "";
		if(type.find("5") != std::string::npos && ptRange == "") addString3 = ", p_{T} > 5 GeV";
		//else if(type.find("10") != std::string::npos) addString3 = ", p_{T}^{#mu} > 10 GeV/c";
		else if(type.find("20") != std::string::npos) addString3 = ", p_{T} > 20 GeV";
		else addString3 = "";

  		setLeg(leg2, header+addString2+addString3);

		TCanvas * canvas2 = new TCanvas("canvas2","canvas",700,700);
		//canvas2->Divide(1,2);
		//canvas2->cd(1);
		//gPad->SetGridx();
		//gPad->SetGridy();

		if(log) gPad->SetLogy();
		else gPad->SetLogy(kFALSE);
		if(logx) gPad->SetLogx();
		else gPad->SetLogx(kFALSE);
		changeOffsetY(genericVector[1]->GetYaxis());
		genericVector[1]->SetStats(kFALSE);
		genericVector[1]->SetMinimum(min);
		genericVector[1]->SetMaximum(max);

		double maxScale = genericVector[1]->GetXaxis()->GetXmax();
		cout<<maxScale<<endl;

		if(maxScale == 2.5 || maxScale == 2.4){

			genericVector[1]->GetXaxis()->SetTitle("Muon |#eta|");
			if(genericVector[1]->GetSize() == 27) genericVector[1]->GetXaxis()->SetRange(0,24);
			else if(genericVector[1]->GetSize() == 15) genericVector[1]->GetXaxis()->SetRangeUser(0,2.4);
            //TF1 *myfit = new TF1("myfit","pol0", 1.5, 2.1);
			//genericVector[1]->Fit("myfit","R");

		}
		else if(maxScale == 2.8 || maxScale == 2.9){

			genericVector[1]->GetXaxis()->SetTitle("Muon |#eta|");
			if(genericVector[1]->GetSize() == 27) genericVector[1]->GetXaxis()->SetRange(20,28);
			else if(genericVector[1]->GetSize() == 15) genericVector[1]->GetXaxis()->SetRangeUser(2.0,2.8);
            //TF1 *myfit = new TF1("myfit","pol0", 1.5, 2.1);
			//genericVector[1]->Fit("myfit","R");

		}
		else if(maxScale == 2500){

			genericVector[1]->GetXaxis()->SetTitle("Muon p_{T} (GeV)");
			genericVector[1]->GetXaxis()->SetRangeUser(0,99);
			if(ptRange == "") addString3 = ", |#eta| < 2.4";
  			setLeg(leg2, header+addString2+addString3);

		}
		else if(maxScale == 250){

			genericVector[1]->GetXaxis()->SetTitle("Number of pile-up collisions");
//			removeBin(genericVector[0], 9);
//			removeBin(genericVector[1], 9);
//			removeBin(genericVector[2], 9);
//			removeBin(genericVector[3], 9);
//			removeBin(genericVector[4], 9);
//
//			removeBin(genericVector[0], 14);
//			removeBin(genericVector[1], 14);
			removeBin(genericVector[2], 14);
//			removeBin(genericVector[3], 14);
//			removeBin(genericVector[4], 14);
            
//			removeBin(genericVector[0], 15);
//			removeBin(genericVector[1], 15);
			removeBin(genericVector[2], 15);
//			removeBin(genericVector[3], 15);
//			removeBin(genericVector[4], 15);
            
//			removeBin(genericVector[0], 16);
//			removeBin(genericVector[1], 16);
//			removeBin(genericVector[2], 16);
//			removeBin(genericVector[3], 16);
//			removeBin(genericVector[4], 16);
            
//			removeBin(genericVector[0], 20);
//			removeBin(genericVector[1], 20);
//			removeBin(genericVector[2], 20);
//			removeBin(genericVector[3], 20);
//			removeBin(genericVector[4], 20);
            
//			removeBin(genericVector[0], 25);
//			removeBin(genericVector[1], 25);
//			removeBin(genericVector[2], 25);
//			removeBin(genericVector[3], 25);
//			removeBin(genericVector[4], 25);
            
//
//			removeBin(genericVector[0], 10);
			removeBin(genericVector[1], 10);
//			removeBin(genericVector[2], 10);
//			removeBin(genericVector[3], 10);
//			removeBin(genericVector[4], 10);
//
//			removeBin(genericVector[0], 19);
			removeBin(genericVector[1], 19);
//			removeBin(genericVector[2], 19);
//			removeBin(genericVector[3], 19);
//			removeBin(genericVector[4], 19);

			std::string addString4 = "";
			if(ptRange == "") addString4 = ", |#eta| < 2.4";
  			setLeg(leg2, header+addString2+addString3+addString4);

		}
		else if(maxScale == 20){

			genericVector[1]->GetXaxis()->SetTitle("#chi^{2}/ndof");
			genericVector[1]->GetXaxis()->SetRangeUser(0,5);

			genericVector[0]->Sumw2();
			genericVector[0]->Scale(1/(float)genericVector[0]->GetEntries());
			genericVector[1]->Sumw2();
			genericVector[1]->Scale(1/(float)genericVector[1]->GetEntries());
			genericVector[2]->Sumw2();
			genericVector[2]->Scale(1/(float)genericVector[2]->GetEntries());
			genericVector[3]->Sumw2();
			genericVector[3]->Scale(1/(float)genericVector[3]->GetEntries());
			genericVector[4]->Sumw2();
			genericVector[4]->Scale(1/(float)genericVector[4]->GetEntries());

			genericVector[1]->SetMinimum(min);
			genericVector[1]->SetMaximum(max);

		}

		genericVector[1]->GetYaxis()->SetTitle(title.c_str());
		genericVector[1]->SetTitle("");
		setMarkerStyle(genericVector[1], 0, markers[4]);
		changeAxis(genericVector[1]->GetXaxis(), 1.0, 0.005, 0.045);
		changeAxis(genericVector[1]->GetYaxis(), 1.5, 0.005, 0.045);
		genericVector[1]->Draw("E1P");
  		cmsPreliminaryLabel->Draw();
		//label->Draw();
		//hRMSNoGEM->SetMaximum(hRMSGEM->GetMaximum());
		setMarkerStyle(genericVector[2], 2, markers[1]);
		genericVector[2]->Draw("E1PSAME");

		setMarkerStyle2(genericVector[3], kGreen+3, markers[2]);
		genericVector[3]->Draw("E1PSAME");

		setMarkerStyle(genericVector[5], 2, markers[7]);
		genericVector[5]->Draw("E1PSAME");

		setMarkerStyle2(genericVector[0], kBlue+2, markers[0]);
		genericVector[0]->Draw("E1PSAME");
    
		setMarkerStyle(genericVector[4], 12, markers[1]);
		//genericVector[4]->Draw("E1PSAME");
    
//		leg2->AddEntry(genericVector[0], "PhaseII 2023D3 PU0", "p");
//  	leg2->AddEntry(genericVector[1], "PhaseII 2023D3 PU140", "p");
//  	leg2->AddEntry(genericVector[2], "PhaseII 2023D3 PU200", "p");
//		leg2->AddEntry(genericVector[3], "PhaseII 2023D4 PU0", "p");
//  	leg2->AddEntry(genericVector[4], "PhaseII 2023D4 PU140", "p");
//  	leg2->AddEntry(genericVector[5], "PhaseII 2023D4 PU200", "p");
    
        leg2->AddEntry(genericVector[0], "PhaseII 2023D4Timing PU0, pre2", "p");
//  	leg2->AddEntry(genericVector[1], "PhaseII 2023D4Timing PU140, pre2", "p");
  	    leg2->AddEntry(genericVector[2], "PhaseII 2023D4Timing PU200, pre2", "p");
		leg2->AddEntry(genericVector[3], "PhaseII 2023D4 PU0, pre1", "p");
//  	leg2->AddEntry(genericVector[4], "PhaseII 2023D4 PU140, pre1", "p");
  		leg2->AddEntry(genericVector[5], "PhaseII 2023D4 PU200, pre1", "p");
		leg2->Draw();

		/*canvas2->cd(2);
  		TH1F * ratioComp1 = makeRatio2(genericVector[1], genericVector[2], 0, 2);
  		ratioComp1->SetMarkerColor(1);
  		ratioComp1->GetXaxis()->SetTitle("|#eta|");
  		ratioComp1->GetYaxis()->SetTitle("chargeMisID^{staMuonsGEM} / chargeMisID^{staMuonsNoGEM}");
  		ratioComp1->Draw("E1P");*/

		saveAll(canvas2,(save + name3).c_str(), type);

}

void setEverything(std::vector<TProfile*> genericVector, double min, double max, bool log, double x1, double y1, double x2, double y2, std::string name3, std::string type, std::string title, std::string save, bool logx = false, std::string ptRange = ""){

        std::vector<int> markers;
		markers.push_back(20);
		markers.push_back(21);
		markers.push_back(22);
		markers.push_back(23);
		markers.push_back(34);
		markers.push_back(24);
		markers.push_back(26);
		markers.push_back(25);

  		gROOT->LoadMacro("tdrstyle.C");
  		setTDRStyle();

  		TPaveText * cmsPreliminaryLabel = new TPaveText(0.13, 0.93, 0.99, 1.02, "NDC");
  		cmsPreliminaryLabel->AddText("CMS Simulation Preliminary              #sqrt{s} = 14 TeV");
  		cmsPreliminaryLabel->SetTextAlign(12);
  		cmsPreliminaryLabel->SetTextFont(42);
  		cmsPreliminaryLabel->SetTextSize(0.04);
  		cmsPreliminaryLabel->SetFillStyle(0);
  		cmsPreliminaryLabel->SetBorderSize(0);
		//float a1 = 0.223, a2 = 0.42, b1 = 0.99, b2 = 0.52;
		float a1 = 0.55, a2 = 0.85, b1 = 0.8, b2 = 0.95; 
		/*if(save.find("Fakes") != std::string::npos){

			a1 = 0.172;
			a2 = 0.82;
			b1 = 0.99; 
			b2 = 0.92;

		}*/
  		TPaveText * label = new TPaveText(a1, a2, b1, b2, "NDC");
  		//label->AddText("Single Muon p_{T} = 100 GeV");
  		label->AddText("NO MUON SYSTEM AGING");
  		label->SetTextAlign(12);
  		label->SetTextFont(62);
  		label->SetTextSize(0.03);
  		label->SetFillStyle(0);
  		label->SetBorderSize(0);

	  	TLegend *leg2 = new TLegend(x1,y1,x2,y2);
		std::string header = "";
		if(name3 == "Muon10") header = "Muon p_{T} = 10 GeV";
		//else if(name3 == "Muon100") header = "Muon p_{T} = 100 GeV/c";
		else if(name3 == "Muon100") header = "Standalone Muon reconstruction";
		else if (name3 == "DY") header = "Z/#gamma* #rightarrow #mu#mu";
		header += ptRange;

		std::string addString2 = "";
		/*if(type.find("MuonLoose") != std::string::npos) addString2 = ", Loose Muon ID";
		else if(type.find("MuonTight") != std::string::npos) addString2 = ", Tight Muon ID";
		else addString2 = "";*/

		std::string addString3 = "";
		if(type.find("5") != std::string::npos && ptRange == "") addString3 = ", p_{T} > 5 GeV";
		//else if(type.find("10") != std::string::npos) addString3 = ", p_{T}^{#mu} > 10 GeV/c";
		else if(type.find("20") != std::string::npos) addString3 = ", p_{T} > 20 GeV";
		else addString3 = "";

  		setLeg(leg2, header+addString2+addString3);

		TCanvas * canvas2 = new TCanvas("canvas2","canvas",700,700);
		//canvas2->Divide(1,2);
		//canvas2->cd(1);
		//gPad->SetGridx();
		//gPad->SetGridy();

		if(log) gPad->SetLogy();
		else gPad->SetLogy(kFALSE);
		if(logx) gPad->SetLogx();
		else gPad->SetLogx(kFALSE);
		changeOffsetY(genericVector[1]->GetYaxis());
		genericVector[1]->SetStats(kFALSE);
		genericVector[1]->SetMinimum(min);
		genericVector[1]->SetMaximum(max);

		double maxScale = genericVector[1]->GetXaxis()->GetXmax();
		cout<<maxScale<<endl;

        if(maxScale == 2.5 || maxScale == 2.4){

			genericVector[1]->GetXaxis()->SetTitle("Muon |#eta|");
			if(genericVector[1]->GetSize() > 25) genericVector[1]->GetXaxis()->SetRange(1,24);
			else if(genericVector[1]->GetSize() == 15) genericVector[1]->GetXaxis()->SetRangeUser(0,2.4);
            //TF1 *myfit = new TF1("myfit","pol0", 1.5, 2.1);
			//genericVector[1]->Fit("myfit","R");

		}
        else if(maxScale == 2.8 || maxScale == 2.9){

			genericVector[1]->GetXaxis()->SetTitle("Muon |#eta|");
			if(genericVector[1]->GetSize() == 27) genericVector[1]->GetXaxis()->SetRange(20,28);
			else if(genericVector[1]->GetSize() == 15) genericVector[1]->GetXaxis()->SetRangeUser(2.0,2.8);
            //TF1 *myfit = new TF1("myfit","pol0", 1.5, 2.1);
			//genericVector[1]->Fit("myfit","R");

		}
		else if(maxScale == 2500){

			genericVector[1]->GetXaxis()->SetTitle("Muon p_{T} (GeV)");
			genericVector[1]->GetXaxis()->SetRangeUser(0,99);
			if(ptRange == "") addString3 = ", |#eta| < 2.4";
  			setLeg(leg2, header+addString2+addString3);

		}
		else if(maxScale == 250){

			genericVector[1]->GetXaxis()->SetTitle("Number of pile-up collisions");
//			removeBin(genericVector[0], 9);
//			removeBin(genericVector[1], 9);
//			removeBin(genericVector[2], 9);
//			removeBin(genericVector[3], 9);
//			removeBin(genericVector[4], 9);
//
//			removeBin(genericVector[0], 14);
//			removeBin(genericVector[1], 14);
			removeBin(genericVector[2], 14);
//			removeBin(genericVector[3], 14);
//			removeBin(genericVector[4], 14);
            
//			removeBin(genericVector[0], 15);
//			removeBin(genericVector[1], 15);
//			removeBin(genericVector[2], 15);
//			removeBin(genericVector[3], 15);
//			removeBin(genericVector[4], 15);
            
//			removeBin(genericVector[0], 16);
//			removeBin(genericVector[1], 16);
			removeBin(genericVector[2], 16);
//			removeBin(genericVector[3], 16);
//			removeBin(genericVector[4], 16);
            
//			removeBin(genericVector[0], 20);
//			removeBin(genericVector[1], 20);
//			removeBin(genericVector[2], 20);
//			removeBin(genericVector[3], 20);
//			removeBin(genericVector[4], 20);
            
//			removeBin(genericVector[0], 25);
//			removeBin(genericVector[1], 25);
//			removeBin(genericVector[2], 25);
//			removeBin(genericVector[3], 25);
//			removeBin(genericVector[4], 25);
            
//
//			removeBin(genericVector[0], 10);
			removeBin(genericVector[1], 10);
//			removeBin(genericVector[2], 10);
//			removeBin(genericVector[3], 10);
//			removeBin(genericVector[4], 10);
//
//			removeBin(genericVector[0], 19);
			removeBin(genericVector[1], 19);
//			removeBin(genericVector[2], 19);
//			removeBin(genericVector[3], 19);
//			removeBin(genericVector[4], 19);

			std::string addString4 = "";
			if(ptRange == "") addString4 = ", |#eta| < 2.4";
  			setLeg(leg2, header+addString2+addString3+addString4);

		}
		else if(maxScale == 20){

			genericVector[1]->GetXaxis()->SetTitle("#chi^{2}/ndof");
			genericVector[1]->GetXaxis()->SetRangeUser(0,5);

			genericVector[0]->Sumw2();
			genericVector[0]->Scale(1/(float)genericVector[0]->GetEntries());
			genericVector[1]->Sumw2();
			genericVector[1]->Scale(1/(float)genericVector[1]->GetEntries());
			genericVector[2]->Sumw2();
			genericVector[2]->Scale(1/(float)genericVector[2]->GetEntries());
			genericVector[3]->Sumw2();
			genericVector[3]->Scale(1/(float)genericVector[3]->GetEntries());
			genericVector[4]->Sumw2();
			genericVector[4]->Scale(1/(float)genericVector[4]->GetEntries());

			genericVector[1]->SetMinimum(min);
			genericVector[1]->SetMaximum(max);

		}

		genericVector[1]->GetYaxis()->SetTitle(title.c_str());
		genericVector[1]->SetTitle("");
		setMarkerStyle(genericVector[1], 0, markers[4]);
		changeAxis(genericVector[1]->GetXaxis(), 1.0, 0.005, 0.045);
		changeAxis(genericVector[1]->GetYaxis(), 1.5, 0.005, 0.045);
		genericVector[1]->Draw("E1P");
  		cmsPreliminaryLabel->Draw();
		//label->Draw();
		//hRMSNoGEM->SetMaximum(hRMSGEM->GetMaximum());
		setMarkerStyle(genericVector[2], 2, markers[1]);
		genericVector[2]->Draw("E1PSAME");

		setMarkerStyle2(genericVector[3], kGreen+3, markers[2]);
		genericVector[3]->Draw("E1PSAME");

		setMarkerStyle(genericVector[5], 2, markers[7]);
		genericVector[5]->Draw("E1PSAME");

		setMarkerStyle2(genericVector[0], kBlue+2, markers[0]);
		genericVector[0]->Draw("E1PSAME");
    
		setMarkerStyle(genericVector[4], 12, markers[1]);
		//genericVector[4]->Draw("E1PSAME");
    
//		leg2->AddEntry(genericVector[0], "PhaseII 2023D3 PU0", "p");
//  	leg2->AddEntry(genericVector[1], "PhaseII 2023D3 PU140", "p");
//  	leg2->AddEntry(genericVector[2], "PhaseII 2023D3 PU200", "p");
//		leg2->AddEntry(genericVector[3], "PhaseII 2023D4 PU0", "p");
//  	leg2->AddEntry(genericVector[4], "PhaseII 2023D4 PU140", "p");
//  	leg2->AddEntry(genericVector[5], "PhaseII 2023D4 PU200", "p");
    
        leg2->AddEntry(genericVector[0], "PhaseII 2023D4Timing PU0, pre2", "p");
//  	leg2->AddEntry(genericVector[1], "PhaseII 2023D4Timing PU140, pre2", "p");
  	    leg2->AddEntry(genericVector[2], "PhaseII 2023D4Timing PU200, pre2", "p");
		leg2->AddEntry(genericVector[3], "PhaseII 2023D4 PU0, pre1", "p");
//  	leg2->AddEntry(genericVector[4], "PhaseII 2023D4 PU140, pre1", "p");
  		leg2->AddEntry(genericVector[5], "PhaseII 2023D4 PU200, pre1", "p");
		leg2->Draw();

		/*canvas2->cd(2);
  		TH1F * ratioComp1 = makeRatio2(genericVector[1], genericVector[2], 0, 2);
  		ratioComp1->SetMarkerColor(1);
  		ratioComp1->GetXaxis()->SetTitle("|#eta|");
  		ratioComp1->GetYaxis()->SetTitle("chargeMisID^{staMuonsGEM} / chargeMisID^{staMuonsNoGEM}");
  		ratioComp1->Draw("E1P");*/

		saveAll(canvas2,(save + name3).c_str(), type);

}


void setEverything2(std::vector<TEfficiency*> genericVector, double min, double max, bool log, double x1, double y1, double x2, double y2, std::string name3, std::string type, std::string title, std::string save, bool logx = false, std::string ptRange = ""){

		std::vector<int> markers;
		markers.push_back(20);
		markers.push_back(21);
		markers.push_back(22);
		markers.push_back(23);
		markers.push_back(34);
		markers.push_back(24);
		markers.push_back(26);
		markers.push_back(25);

  		gROOT->LoadMacro("tdrstyle.C");
  		setTDRStyle();

  		TPaveText * cmsPreliminaryLabel = new TPaveText(0.13, 0.93, 0.99, 1.02, "NDC");
  		cmsPreliminaryLabel->AddText("CMS Simulation Preliminary              #sqrt{s} = 14 TeV");
  		cmsPreliminaryLabel->SetTextAlign(12);
  		cmsPreliminaryLabel->SetTextFont(42);
  		cmsPreliminaryLabel->SetTextSize(0.04);
  		cmsPreliminaryLabel->SetFillStyle(0);
  		cmsPreliminaryLabel->SetBorderSize(0);
		//float a1 = 0.223, a2 = 0.42, b1 = 0.99, b2 = 0.52;
		float a1 = 0.55, a2 = 0.85, b1 = 0.8, b2 = 0.95; 
		/*if(save.find("Fakes") != std::string::npos){

			a1 = 0.172;
			a2 = 0.82;
			b1 = 0.99; 
			b2 = 0.92;

		}*/
  		TPaveText * label = new TPaveText(a1, a2, b1, b2, "NDC");
  		//label->AddText("Single Muon p_{T} = 100 GeV");
  		label->AddText("NO MUON SYSTEM AGING");
  		label->SetTextAlign(12);
  		label->SetTextFont(62);
  		label->SetTextSize(0.03);
  		label->SetFillStyle(0);
  		label->SetBorderSize(0);

	  	TLegend *leg2 = new TLegend(x1,y1,x2,y2);
		std::string header = "";
		if(name3 == "Muon10") header = "Muon p_{T} = 10 GeV";
		else if(name3 == "Muon100") header = "Muon p_{T} = 100 GeV";
		else if (name3 == "DY") header = "Z/#gamma* #rightarrow #mu#mu";
		header += ptRange;

		std::string addString2 = "";
		/*if(type.find("MuonLoose") != std::string::npos) addString2 = ", Loose Muon ID";
		else if(type.find("MuonTight") != std::string::npos) addString2 = ", Tight Muon ID";
		else addString2 = "";*/

		std::string addString3 = "";
		if(type.find("5") != std::string::npos && ptRange == "") addString3 = ", p_{T} > 5 GeV";
		//else if(type.find("10") != std::string::npos) addString3 = ", p_{T}^{#mu} > 10 GeV/c";
		else if(type.find("20") != std::string::npos) addString3 = ", p_{T} > 20 GeV";
		else addString3 = "";

  		setLeg(leg2, header+addString2+addString3);

		TCanvas * canvas3 = new TCanvas("canvas3","canvas",700,700);
		//canvas3->Divide(1,2);
		//canvas3->cd(1);
		//gPad->SetGridx();
		//gPad->SetGridy();

		if(log) gPad->SetLogy();
		else gPad->SetLogy(kFALSE);
		if(logx) gPad->SetLogx();
		else gPad->SetLogx(kFALSE);
		genericVector[1]->Draw("AP");
		gPad->Update();
		changeAxis(genericVector[1]->GetPaintedGraph()->GetXaxis(), 1.0, 0.005, 0.045);
		changeAxis(genericVector[1]->GetPaintedGraph()->GetYaxis(), 1.5, 0.005, 0.045);
		//genericVector[1]->SetStats(kFALSE);
		genericVector[1]->GetPaintedGraph()->SetMinimum(min);
		genericVector[1]->GetPaintedGraph()->SetMaximum(max);

		double maxScale = genericVector[1]->GetPaintedGraph()->GetXaxis()->GetXmax();
		cout<<maxScale<<endl;

		if(maxScale > 2.3 && maxScale < 2.7){
            
			genericVector[1]->SetTitle((" ;Muon |#eta|;"+title).c_str());
			genericVector[1]->GetPaintedGraph()->GetXaxis()->SetLimits(0,2.4);

		}
		else if(maxScale > 2.8 && maxScale < 3.0){
            
			genericVector[1]->SetTitle((" ;Muon |#eta|;"+title).c_str());
			genericVector[1]->GetPaintedGraph()->GetXaxis()->SetLimits(2.0,2.8);

		}
		else if(maxScale > 700 && maxScale < 2800){
            
			genericVector[1]->SetTitle((" ;Muon p_{T} (GeV);"+title).c_str());
			genericVector[1]->GetPaintedGraph()->GetXaxis()->SetLimits(0,100);
			if(ptRange == "") addString3 = ", |#eta| < 2.4";
  			setLeg(leg2, header+addString2+addString3);

        }
		else if(maxScale > 170 && maxScale < 270){

			genericVector[1]->SetTitle((" ;Number of pile-up collisions;"+title).c_str());
			genericVector[1]->GetPaintedGraph()->GetXaxis()->SetLimits(0,250);
			if(ptRange == "") std::string addString4 = ", |#eta| < 2.4";
  			setLeg(leg2, header+addString2+addString3+addString4);

		}
		else if(maxScale == 20){

			genericVector[1]->GetXaxis()->SetTitle("#chi^{2}/ndof");
			genericVector[1]->GetXaxis()->SetRangeUser(0,5);

			genericVector[0]->Sumw2();
			genericVector[0]->Scale(1/(float)genericVector[0]->GetEntries());
			genericVector[1]->Sumw2();
			genericVector[1]->Scale(1/(float)genericVector[1]->GetEntries());
			genericVector[2]->Sumw2();
			genericVector[2]->Scale(1/(float)genericVector[2]->GetEntries());
			genericVector[3]->Sumw2();
			genericVector[3]->Scale(1/(float)genericVector[3]->GetEntries());
			genericVector[4]->Sumw2();
			genericVector[4]->Scale(1/(float)genericVector[4]->GetEntries());

        }

		//genericVector[1]->GetPaintedGraph()->GetYaxis()->SetTitle(title.c_str());
		genericVector[1]->GetPaintedGraph()->SetTitle("");
		setMarkerStyle(genericVector[1], 0, markers[4]);

  		cmsPreliminaryLabel->Draw();
		//label->Draw();
		//hRMSNoGEM->SetMaximum(hRMSGEM->GetMaximum());
		setMarkerStyle(genericVector[2], 2, markers[1]);
		genericVector[2]->Draw("SAME");

		setMarkerStyle2(genericVector[3], kGreen+3, markers[2]);
		genericVector[3]->Draw("SAME");

		setMarkerStyle(genericVector[5], 2, markers[7]);
		genericVector[5]->Draw("E1PSAME");
    
		setMarkerStyle(genericVector[4], 12, markers[1]);
		//genericVector[4]->Draw("E1PSAME");

		setMarkerStyle2(genericVector[0], kBlue+2, markers[0]);
		genericVector[0]->Draw("SAME");
    
//		leg2->AddEntry(genericVector[0], "PhaseII 2023D3 PU0", "p");
//  	leg2->AddEntry(genericVector[1], "PhaseII 2023D3 PU140", "p");
//  	leg2->AddEntry(genericVector[2], "PhaseII 2023D3 PU200", "p");
//		leg2->AddEntry(genericVector[3], "PhaseII 2023D4 PU0", "p");
//  	leg2->AddEntry(genericVector[4], "PhaseII 2023D4 PU140", "p");
//  	leg2->AddEntry(genericVector[5], "PhaseII 2023D4 PU200", "p");
    
        leg2->AddEntry(genericVector[0], "PhaseII 2023D4Timing PU0, pre2", "p");
//  	leg2->AddEntry(genericVector[1], "PhaseII 2023D4Timing PU140, pre2", "p");
  	    leg2->AddEntry(genericVector[2], "PhaseII 2023D4Timing PU200, pre2", "p");
		leg2->AddEntry(genericVector[3], "PhaseII 2023D4 PU0, pre1", "p");
//  	leg2->AddEntry(genericVector[4], "PhaseII 2023D4 PU140, pre1", "p");
  		leg2->AddEntry(genericVector[5], "PhaseII 2023D4 PU200, pre1", "p");
		leg2->Draw();

		/*canvas3->cd(2);
  		TH1F * ratioComp1 = makeRatio2(genericVector[1], genericVector[2], 0, 2);
  		ratioComp1->SetMarkerColor(1);
  		ratioComp1->GetXaxis()->SetTitle("|#eta|");
  		ratioComp1->GetYaxis()->SetTitle("chargeMisID^{staMuonsGEM} / chargeMisID^{staMuonsNoGEM}");
  		ratioComp1->Draw("E1P");*/

		saveAll(canvas3,(save + name3).c_str(), type);

}

//---------------------------------------------------------------------------------------------------------------------------

void makePlots(std::vector<std::string> allNames, std::string name3){

  std::vector<TH1F*> chargeMisIDProb_vs_eta;
  std::vector<TH1F*> effic_vs_eta;
  std::vector<TH1F*> effic_vs_eta_050;
  std::vector<TH1F*> effic_vs_eta_050_pt10;
  std::vector<TH1F*> effic_vs_vtx;
  std::vector<TH1F*> effic_vs_vtxB;
  std::vector<TH1F*> effic_vs_vtxO;
  std::vector<TH1F*> effic_vs_vtxE;
  std::vector<TH1F*> efficFakes_vs_eta;
  std::vector<TH1F*> efficFakes_vs_eta_sgn;
  std::vector<TH1F*> efficFakes_vs_eta_050;
  std::vector<TH1F*> efficFakes_vs_eta_050_pt10;
  std::vector<TH1F*> chi2;

  std::vector<TH1F*> quality_vs_eta;

  std::vector<TH1F*> efficFakes1_vs_eta;
  std::vector<TH1F*> efficFakes2_vs_eta;
  std::vector<TH1F*> efficFakes3_vs_eta;
  std::vector<TH1F*> efficFakes4_vs_eta;

  std::vector<TH1F*> qOverPtRes1_vs_eta;
  std::vector<TH1F*> qOverPtResRMS1_vs_eta;
  std::vector<TH1F*> qOverPtRes2_vs_eta;
  std::vector<TH1F*> qOverPtResRMS2_vs_eta;
  std::vector<TH1F*> qOverPtRes3_vs_eta;
  std::vector<TH1F*> qOverPtResRMS3_vs_eta;
  std::vector<TH1F*> qOverPtRes4_vs_eta;
  std::vector<TH1F*> qOverPtResRMS4_vs_eta;

  std::vector<TH1F*> qOverPtRes_vs_eta;
  std::vector<TH1F*> qOverPtResRMS_vs_eta;
  std::vector<TH1F*> numRecoPt_vs_eta;
  std::vector<TH1F*> numRecoPtAsso_vs_eta;
  std::vector<TH1F*> yield_vs_eta;
  std::vector<TH1F*> yield_vs_eta_sgn;
  std::vector<TH1F*> yield_vs_eta_050;
  std::vector<TH1F*> yield_vs_eta_050_pt10;

  std::vector<TH1F*> chargeMisIDProb_vs_pt;
  std::vector<TH1F*> effic_vs_pt;
  std::vector<TH1F*> efficFakes_vs_pt;
  std::vector<TH1F*> efficFakes_vs_pt_sgn;
  std::vector<TH1F*> qOverPtRes_vs_pt;
  std::vector<TH1F*> qOverPtResRMS_vs_pt;
  std::vector<TH1F*> numRecoPt_vs_pt;
  std::vector<TH1F*> numRecoPtAsso_vs_pt;
  std::vector<TH1F*> yield_vs_pt;
  std::vector<TH1F*> yield_vs_pt_sgn;

  std::vector<TH1F*> effic_vs_eta1;
  std::vector<TH1F*> effic_vs_eta2;
  std::vector<TH1F*> effic_vs_eta3;
  std::vector<TH1F*> effic_vs_eta4;
  std::vector<TH1F*> numRecoPt1_vs_eta;
  std::vector<TH1F*> numRecoPtAsso1_vs_eta;
  std::vector<TH1F*> numRecoPt2_vs_eta;
  std::vector<TH1F*> numRecoPtAsso2_vs_eta;
  std::vector<TH1F*> numRecoPt3_vs_eta;
  std::vector<TH1F*> numRecoPtAsso3_vs_eta;
  std::vector<TH1F*> numRecoPt4_vs_eta;
  std::vector<TH1F*> numRecoPtAsso4_vs_eta;
  std::vector<TH1F*> yield1_vs_eta;
  std::vector<TH1F*> yield2_vs_eta;
  std::vector<TH1F*> yield3_vs_eta;
  std::vector<TH1F*> yield4_vs_eta;

  std::vector<TH1F*> efficB_vs_pt;
  std::vector<TH1F*> efficO_vs_pt;
  std::vector<TH1F*> efficE_vs_pt;
  std::vector<TH1F*> numRecoPtB_vs_pt;
  std::vector<TH1F*> numRecoPtAssoB_vs_pt;
  std::vector<TH1F*> yieldB_vs_pt;
  std::vector<TH1F*> numRecoPtO_vs_pt;
  std::vector<TH1F*> numRecoPtAssoO_vs_pt;
  std::vector<TH1F*> yieldO_vs_pt;
  std::vector<TH1F*> numRecoPtE_vs_pt;
  std::vector<TH1F*> numRecoPtAssoE_vs_pt;
  std::vector<TH1F*> yieldE_vs_pt;

  std::vector<TEfficiency*> effErr_vs_eta;
  std::vector<TEfficiency*> effErr_vs_eta_050;
  std::vector<TEfficiency*> effErr_vs_pt;
  std::vector<TEfficiency*> effErr_vs_vtx;

  std::vector<TEfficiency*> fr_vs_eta;
  std::vector<TEfficiency*> fr_vs_eta_50;
  std::vector<TEfficiency*> fr_vs_eta_sgn;
  std::vector<TEfficiency*> fr_vs_pt;
  std::vector<TEfficiency*> fr_vs_pt_sgn;
    
  std::vector<TProfile*> cscHits_vs_eta;
  std::vector<TProfile*> dtHits_vs_eta;
  std::vector<TProfile*> rpcHits_vs_eta;
  std::vector<TProfile*> gemHits_vs_eta;
  std::vector<TProfile*> me0Hits_vs_eta;
  std::vector<TProfile*> nHits_vs_eta;

  for(int i = 0; i < allNames.size()-1; i++){
      
    //if( i > 0 && allNames[6].find("Cut5/bestMuonTight_tpToTightSel0MuonAssociation")!=std::string::npos ) allNames[6] = "Cut5/bestMuonTight_tpToTightSel0BSMuonAssociation";
      
  	TFile * f1 = TFile::Open(allNames[i].c_str());
  	f1->cd(("/DQMData/Run 1/Muons/Run summary/RecoMuonV/MultiTrack/"+allNames[6]).c_str());

  	TH1F * numEvt_NoGEM = (TH1F*)gDirectory->Get("numEvt");
  	//std::cout<<numEvt_NoGEM->GetBinContent(2)<<std::endl;
  	int numEvt = numEvt_NoGEM->GetBinContent(2);
  	std::cout<<i<<" "<<numEvt<<std::endl;
      
    cscHits_vs_eta.push_back((TProfile*)gDirectory->Get("CSChits_eta"));
    dtHits_vs_eta.push_back((TProfile*)gDirectory->Get("DThits_eta"));
    rpcHits_vs_eta.push_back((TProfile*)gDirectory->Get("RPChits_eta"));
    gemHits_vs_eta.push_back((TProfile*)gDirectory->Get("GEMhits_eta"));
    me0Hits_vs_eta.push_back((TProfile*)gDirectory->Get("ME0hits_eta"));
    nHits_vs_eta.push_back((TProfile*)gDirectory->Get("hits_eta"));

	chargeMisIDProb_vs_eta.push_back((TH1F*)gDirectory->Get("chargeMisIDProb_vs_eta_sim"));
	effic_vs_eta.push_back((TH1F*)gDirectory->Get("effic"));
	effic_vs_eta_050.push_back((TH1F*)gDirectory->Get("effic_Q05"));
	effic_vs_eta_050_pt10.push_back((TH1F*)gDirectory->Get("effic_Q05Pt10"));
	chi2.push_back((TH1F*)gDirectory->Get("chi2"));
	effic_vs_vtx.push_back((TH1F*)gDirectory->Get("efficVtx"));
	effic_vs_vtxB.push_back((TH1F*)gDirectory->Get("efficVtx_barrel"));
	effic_vs_vtxO.push_back((TH1F*)gDirectory->Get("efficVtx_overlap"));
	effic_vs_vtxE.push_back((TH1F*)gDirectory->Get("efficVtx_endcap"));
	efficFakes_vs_eta.push_back((TH1F*)gDirectory->Get("fakerate"));
	efficFakes_vs_eta_sgn.push_back((TH1F*)gDirectory->Get("fakerate_sgn"));
	efficFakes_vs_eta_050.push_back((TH1F*)gDirectory->Get("fakerate050"));
	efficFakes_vs_eta_050_pt10.push_back((TH1F*)gDirectory->Get("fakerate050Pt10"));

	TH2F * qualityVsEta = (TH2F*)gDirectory->Get("qualityVsEta");
	TH1F * tmp = (TH1F*)qualityVsEta->ProfileX();
	quality_vs_eta.push_back(tmp);

	efficFakes1_vs_eta.push_back((TH1F*)gDirectory->Get("fakerateBin1"));
	efficFakes2_vs_eta.push_back((TH1F*)gDirectory->Get("fakerateBin2"));
	efficFakes3_vs_eta.push_back((TH1F*)gDirectory->Get("fakerateBin3"));
	efficFakes4_vs_eta.push_back((TH1F*)gDirectory->Get("fakerateBin4"));

	qOverPtRes1_vs_eta.push_back((TH1F*)gDirectory->Get("qOverPtres1_vs_eta_sim_Sigma"));
	qOverPtResRMS1_vs_eta.push_back((TH1F*)gDirectory->Get("qOverPtres1_vs_eta_sim_RMS"));
	qOverPtRes2_vs_eta.push_back((TH1F*)gDirectory->Get("qOverPtres2_vs_eta_sim_Sigma"));
	qOverPtResRMS2_vs_eta.push_back((TH1F*)gDirectory->Get("qOverPtres2_vs_eta_sim_RMS"));
	qOverPtRes3_vs_eta.push_back((TH1F*)gDirectory->Get("qOverPtres3_vs_eta_sim_Sigma"));
	qOverPtResRMS3_vs_eta.push_back((TH1F*)gDirectory->Get("qOverPtres3_vs_eta_sim_RMS"));
	qOverPtRes4_vs_eta.push_back((TH1F*)gDirectory->Get("qOverPtres4_vs_eta_sim_Sigma"));
	qOverPtResRMS4_vs_eta.push_back((TH1F*)gDirectory->Get("qOverPtres4_vs_eta_sim_RMS"));

	qOverPtRes_vs_eta.push_back((TH1F*)gDirectory->Get("qOverPtres_vs_eta_sim_Sigma"));
	qOverPtResRMS_vs_eta.push_back((TH1F*)gDirectory->Get("qOverPtres_vs_eta_sim_RMS"));
	numRecoPt_vs_eta.push_back((TH1F*)gDirectory->Get("num_reco_eta"));
	numRecoPtAsso_vs_eta.push_back((TH1F*)gDirectory->Get("num_assoc(recoToSim)_eta"));
	yield_vs_eta.push_back(produceYield((TH1F*)gDirectory->Get("num_reco_eta"), (TH1F*)gDirectory->Get("num_assoc(recoToSim)_eta"), numEvt, false, false));
	yield_vs_eta_sgn.push_back(produceYield((TH1F*)gDirectory->Get("num_reco_eta"), (TH1F*)gDirectory->Get("num_assoc(recoToSim)_eta_SgnMu"), numEvt ,false, false));
	yield_vs_eta_050.push_back(produceYield((TH1F*)gDirectory->Get("num_reco_eta"), (TH1F*)gDirectory->Get("num_assoc(recoToSim)_eta_050"), numEvt, false, false));
	yield_vs_eta_050_pt10.push_back(produceYield((TH1F*)gDirectory->Get("num_reco_eta_pt10"), (TH1F*)gDirectory->Get("num_assoc(recoToSim)_eta_pt10"), numEvt, false, false));

	chargeMisIDProb_vs_pt.push_back((TH1F*)gDirectory->Get("chargeMisIDProb_vs_pt_sim"));
	effic_vs_pt.push_back((TH1F*)gDirectory->Get("efficPt"));
	efficFakes_vs_pt.push_back((TH1F*)gDirectory->Get("fakeratePt"));
	efficFakes_vs_pt_sgn.push_back((TH1F*)gDirectory->Get("fakeratePt_Sgn"));
	qOverPtRes_vs_pt.push_back((TH1F*)gDirectory->Get("qOverPtres_vs_pt_sim_Sigma"));
	qOverPtResRMS_vs_pt.push_back((TH1F*)gDirectory->Get("qOverPtres_vs_pt_sim_RMS"));
	numRecoPt_vs_pt.push_back((TH1F*)gDirectory->Get("num_reco_pT"));
	numRecoPtAsso_vs_pt.push_back((TH1F*)gDirectory->Get("num_assoc(recoToSim)_pT"));
	yield_vs_pt.push_back(produceYield((TH1F*)gDirectory->Get("num_reco_pT"), (TH1F*)gDirectory->Get("num_assoc(recoToSim)_pT"), numEvt, true));
	yield_vs_pt_sgn.push_back(produceYield((TH1F*)gDirectory->Get("num_reco_pT"), (TH1F*)gDirectory->Get("num_assoc(recoToSim)_pT_SgnMu"), numEvt, true));

	effic_vs_eta1.push_back((TH1F*)gDirectory->Get("efficBin1"));
	effic_vs_eta2.push_back((TH1F*)gDirectory->Get("efficBin2"));
	effic_vs_eta3.push_back((TH1F*)gDirectory->Get("efficBin3"));
	effic_vs_eta4.push_back((TH1F*)gDirectory->Get("efficBin4"));
	numRecoPt1_vs_eta.push_back((TH1F*)gDirectory->Get("num_reco_eta_bin1"));
	numRecoPtAsso1_vs_eta.push_back((TH1F*)gDirectory->Get("num_assoc(recoToSim)_eta_bin1"));
	numRecoPt2_vs_eta.push_back((TH1F*)gDirectory->Get("num_reco_eta_bin2"));
	numRecoPtAsso2_vs_eta.push_back((TH1F*)gDirectory->Get("num_assoc(recoToSim)_eta_bin2"));
	numRecoPt3_vs_eta.push_back((TH1F*)gDirectory->Get("num_reco_eta_bin3"));
	numRecoPtAsso3_vs_eta.push_back((TH1F*)gDirectory->Get("num_assoc(recoToSim)_eta_bin3"));
	numRecoPt4_vs_eta.push_back((TH1F*)gDirectory->Get("num_reco_eta_bin4"));
	numRecoPtAsso4_vs_eta.push_back((TH1F*)gDirectory->Get("num_assoc(recoToSim)_eta_bin4"));
	yield1_vs_eta.push_back(produceYield((TH1F*)gDirectory->Get("num_reco_eta_bin1"), (TH1F*)gDirectory->Get("num_assoc(recoToSim)_eta_bin1"), numEvt, false, true));
	yield2_vs_eta.push_back(produceYield((TH1F*)gDirectory->Get("num_reco_eta_bin2"), (TH1F*)gDirectory->Get("num_assoc(recoToSim)_eta_bin2"), numEvt, false, true));
	yield3_vs_eta.push_back(produceYield((TH1F*)gDirectory->Get("num_reco_eta_bin3"), (TH1F*)gDirectory->Get("num_assoc(recoToSim)_eta_bin3"), numEvt, false, true));
	yield4_vs_eta.push_back(produceYield((TH1F*)gDirectory->Get("num_reco_eta_bin4"), (TH1F*)gDirectory->Get("num_assoc(recoToSim)_eta_bin4"), numEvt, false, true));

	efficB_vs_pt.push_back((TH1F*)gDirectory->Get("efficPtBarrel"));
	efficO_vs_pt.push_back((TH1F*)gDirectory->Get("efficPtOverlap"));
	efficE_vs_pt.push_back((TH1F*)gDirectory->Get("efficPtEndcap"));
	numRecoPtB_vs_pt.push_back((TH1F*)gDirectory->Get("num_reco_pT_barrel"));
	numRecoPtAssoB_vs_pt.push_back((TH1F*)gDirectory->Get("num_assoc(recoToSim)_pT_barrel"));
	yieldB_vs_pt.push_back(produceYield((TH1F*)gDirectory->Get("num_reco_pT_barrel"), (TH1F*)gDirectory->Get("num_assoc(recoToSim)_pT_barrel"), numEvt, true));
	numRecoPtO_vs_pt.push_back((TH1F*)gDirectory->Get("num_reco_pT_overlap"));
	numRecoPtAssoO_vs_pt.push_back((TH1F*)gDirectory->Get("num_assoc(recoToSim)_pT_overlap"));
	yieldO_vs_pt.push_back(produceYield((TH1F*)gDirectory->Get("num_reco_pT_overlap"), (TH1F*)gDirectory->Get("num_assoc(recoToSim)_pT_overlap"), numEvt, true));
	numRecoPtE_vs_pt.push_back((TH1F*)gDirectory->Get("num_reco_pT_endcap"));
	numRecoPtAssoE_vs_pt.push_back((TH1F*)gDirectory->Get("num_assoc(recoToSim)_pT_endcap"));
	yieldE_vs_pt.push_back(produceYield((TH1F*)gDirectory->Get("num_reco_pT_endcap"), (TH1F*)gDirectory->Get("num_assoc(recoToSim)_pT_endcap"), numEvt, true));

	TEfficiency * effEta = calcEff((TH1F*)gDirectory->Get("num_assoc(simToReco)_eta"), (TH1F*)gDirectory->Get("num_simul_eta"), false, false, false, i);
	effErr_vs_eta.push_back(effEta);
	TEfficiency * effEta050 = calcEff((TH1F*)gDirectory->Get("num_assoc(simToReco)_eta_Q05"), (TH1F*)gDirectory->Get("num_simul_eta"), false, false, false, i);
	effErr_vs_eta_050.push_back(effEta050);
	TEfficiency * effPt = calcEff((TH1F*)gDirectory->Get("num_assoc(simToReco)_pT"), (TH1F*)gDirectory->Get("num_simul_pT"), false, false, false, i);
	effErr_vs_pt.push_back(effPt);
	TEfficiency * effVtx = calcEff((TH1F*)gDirectory->Get("num_assoc(simToReco)_vtx"), (TH1F*)gDirectory->Get("num_simul_vtx"), true, false, false, i);
      
	effErr_vs_vtx.push_back(effVtx);

	fr_vs_eta.push_back(produceFR((TH1F*)gDirectory->Get("num_reco_eta"), (TH1F*)gDirectory->Get("num_assoc(recoToSim)_eta"), (TH1F*)gDirectory->Get("num_reco_eta"), false, false));
	fr_vs_eta_sgn.push_back(produceFR((TH1F*)gDirectory->Get("num_reco_eta"), (TH1F*)gDirectory->Get("num_assoc(recoToSim)_eta_SgnMu"), (TH1F*)gDirectory->Get("num_reco_eta") ,false, false));
	fr_vs_pt.push_back(produceFR((TH1F*)gDirectory->Get("num_reco_pT"), (TH1F*)gDirectory->Get("num_assoc(recoToSim)_pT"), (TH1F*)gDirectory->Get("num_reco_pT"), false));
	fr_vs_pt_sgn.push_back(produceFR((TH1F*)gDirectory->Get("num_reco_pT"), (TH1F*)gDirectory->Get("num_assoc(recoToSim)_pT_SgnMu"), (TH1F*)gDirectory->Get("num_reco_pT"), false));
      
    fr_vs_eta_50.push_back(produceFR((TH1F*)gDirectory->Get("num_reco_eta"), (TH1F*)gDirectory->Get("num_assoc(recoToSim)_eta_050"), (TH1F*)gDirectory->Get("num_reco_eta"), false, false));

  }

  //TH2F * qOverPtResRMS_vs_eta[0]_2D = (TH2F*)gDirectory->Get("qOverPtresXL_vs_eta_sim");
  //vecHist v5 = projectAndFit(qOverPtResRMS_vs_eta[0]_2D);
  //makeSuperImp(v1, v2, v3, ("distr_"+name3).c_str(), allNames[6]);

  /*extractPerformance(effic_vs_eta_GEM5, effic_vs_eta_GEM, "Efficiency", "5 < pT < 10 GeV/c");
  extractPerformance(effic_vs_vtx_GEM5, effic_vs_vtx_GEM, "Efficiency", "10 < pT < 20 GeV/c");
  extractPerformance(effic_vs_vtxB_GEM5, effic_vs_vtxB_GEM, "Efficiency", "20 < pT < 40 GeV/c");
  extractPerformance(effic_vs_vtxE_GEM5, effic_vs_vtxE_GEM, "Efficiency", "pT > 40 GeV/c");

  extractPerformance(qOverPtRes_vs_eta_GEM5, qOverPtRes_vs_eta_GEM, "Resolution", "pT > 0");

  extractPerformance(yield_vs_eta_GEM5, yield_vs_eta_GEM, "Bkg Yield", "5 < pT < 10 GeV/c");
  extractPerformance(yield2_vs_eta_GEM5, yield2_vs_eta_GEM, "Bkg Yield", "10 < pT < 20 GeV/c");
  extractPerformance(yield3_vs_eta_GEM5, yield3_vs_eta_GEM, "Bkg Yield", "20 < pT < 40 GeV/c");
  extractPerformance(yield4_vs_eta_GEM5, yield4_vs_eta_GEM, "Bkg Yield", "pT > 40 GeV/c");*/

//---------------------------------------------------------------------------------------------------------------------------

 	std::vector<TH1F*> numRecoTOT_vs_eta = sumHistos(numRecoPt1_vs_eta, numRecoPt2_vs_eta, numRecoPt3_vs_eta, numRecoPt4_vs_eta);
  	std::vector<TH1F*> numRecoAssoTOT_vs_eta = sumHistos(numRecoPtAsso1_vs_eta, numRecoPtAsso2_vs_eta, numRecoPtAsso3_vs_eta, numRecoPtAsso4_vs_eta);
	std::vector<TH1F*> fakeRateTOT_vs_eta = makeMismatch(numRecoTOT_vs_eta, numRecoAssoTOT_vs_eta);

	std::string addString = "";
	if(allNames[6].find("MuonLoose") != std::string::npos) addString = "Loose Muon ";
	else if(allNames[6].find("bestMuonTight") != std::string::npos) addString = "Tight Muon ";
    else if(allNames[6].find("extractedSTA") != std::string::npos) addString = "StandAlone Muon ";
    else if(allNames[6].find("Me0Muons") != std::string::npos) addString = "ME0Muon ";
    else if(allNames[6].find("GemMuons") != std::string::npos) addString = "GEMMuon ";
    else if(allNames[6].find("standAloneMuons") != std::string::npos) addString = "STAMuon ";
    else if(allNames[6].find("extractedGlobalMuons") != std::string::npos) addString = "GLBMuons ";
	else addString = "Muon ";

	double minEff = 0.3;
	if( allNames[6].find("LooseSel3") != std::string::npos || allNames[6].find("TightSel3") != std::string::npos ) minEff = 0.0;
    if( allNames[6].find("Me0Muons") != std::string::npos ) minEff = 0.1;
	double maxYield = 0.2;
    if( allNames[6].find("TightSel0") != std::string::npos ) { maxYield = 0.004, minEff = 0.0; };
    if( allNames[6].find("LooseSel0") != std::string::npos ) { minEff = 0.0; };
	if( allNames[6].find("LooseSel3") != std::string::npos || allNames[6].find("TightSel3") != std::string::npos ) maxYield = 0.05;
    if( allNames[6].find("extractedSTA") != std::string::npos ){ maxYield = 0.4; minEff = 0.0; }
    if( allNames[6].find("GemMuons") != std::string::npos ){ maxYield = 500.0; }
    if( allNames[6].find("Me0Muons") != std::string::npos ){ maxYield = 1000.0; }
    if( allNames[6].find("bestMuonLoose") != std::string::npos ){ maxYield = 50.0; }
    if( allNames[6].find("standAloneMuons") != std::string::npos ){ maxYield = 50.0; }
    if( allNames[6].find("extractedGlobalMuons") != std::string::npos ){ maxYield = 50.0; }

	setEverything2(effErr_vs_eta, minEff, 1.3, false, 0.25, 0.72, 0.50, 0.92, name3, allNames[6], (addString + "Efficiency"), "efficErr_");
	setEverything2(effErr_vs_eta_050, minEff, 1.1, false, 0.5, 0.30, 0.8, 0.45, name3, allNames[6], (addString + "Efficiency"), "efficErr050_");
	setEverything2(effErr_vs_pt, 0.0, 1.1, false, 0.25, 0.20, 0.75, 0.45, name3, allNames[6], (addString + "Efficiency"), "efficPtErr_");
	setEverything2(effErr_vs_vtx, 0.5, 1.1, false, 0.25, 0.20, 0.75, 0.45, name3, allNames[6], (addString + "Efficiency"), "efficVtxErr_");
    
    setEverything(cscHits_vs_eta, 0.0, 40, false, 0.20, 0.65, 0.70, 0.85, name3, allNames[6], "Average number of CSC tracking recHits", "cscHits_");
    setEverything(dtHits_vs_eta, 0.0, 60, false, 0.20, 0.65, 0.70, 0.85, name3, allNames[6], "Average number of DT tracking recHits", "dtHits_");
    setEverything(rpcHits_vs_eta, 0.0, 10, false, 0.20, 0.65, 0.70, 0.85, name3, allNames[6], "Average number of RPC tracking recHits", "rpcHits_");
    setEverything(gemHits_vs_eta, 0.0, 10, false, 0.20, 0.65, 0.70, 0.85, name3, allNames[6], "Average number of GEM tracking recHits", "gemHits_");
    setEverything(me0Hits_vs_eta, 0.0, 10, false, 0.20, 0.65, 0.70, 0.85, name3, allNames[6], "Average number of ME0 tracking recHits", "me0Hits_");
    setEverything(nHits_vs_eta, 25.0, 80, false, 0.20, 0.70, 0.70, 0.90, name3, allNames[6], "Average number of tracking recHits", "nHits_");

	setEverything(chargeMisIDProb_vs_eta, 0.000001, 0.1, true, 0.20, 0.65, 0.70, 0.85, name3, allNames[6], "Charge mis-ID Prob.", "chargeMisID_");
	setEverything(effic_vs_eta, minEff, 1.3, false, 0.25, 0.20, 0.75, 0.45, name3, allNames[6], (addString + "Efficiency"), "effic_");
	setEverything(effic_vs_eta_050, minEff, 1.1, false, 0.25, 0.20, 0.75, 0.45, name3, allNames[6], (addString + "Efficiency"), "effic050_", false, ", Quality > 50%");
	setEverything(effic_vs_eta_050_pt10, minEff, 1.1, false, 0.25, 0.20, 0.75, 0.45, name3, allNames[6], (addString + "Efficiency"), "effic050Pt10_", false, ", Quality > 50%, p_{T} > 10 GeV");
	setEverything(efficFakes_vs_eta, 0.0, 1.0, false, 0.20, 0.60, 0.75, 0.85, name3, allNames[6], "Fake Rate", "efficFakes_");
	setEverything(efficFakes_vs_eta_sgn, 0.0, 1.0, false, 0.20, 0.60, 0.75, 0.85, name3, allNames[6], "Fake Rate", "efficFakesSgn_");
	setEverything(efficFakes_vs_eta_050, 0.0, 1.0, false, 0.20, 0.60, 0.75, 0.85, name3, allNames[6], "Fake Rate", "efficFakes050_", false, ", Quality > 50%");
	setEverything(efficFakes_vs_eta_050_pt10, 0.0, 1.0, false, 0.20, 0.60, 0.75, 0.85, name3, allNames[6], "Fake Rate", "efficFakes050Pt10_", false, ", Quality > 50% , p_{T} > 10 GeV");

	setEverything(quality_vs_eta, 0.0, 1.0, false, 0.25, 0.20, 0.75, 0.45, name3, allNames[6], (addString + "Quality"), "qualityVsEta_");

	setEverything(efficFakes1_vs_eta, 0.0, 1.0, false, 0.20, 0.60, 0.75, 0.85, name3, allNames[6], "Fake Rate", "efficFakes1_", false, ", 5 < p_{T} < 10 GeV");
	setEverything(efficFakes2_vs_eta, 0.0, 1.0, false, 0.20, 0.60, 0.75, 0.85, name3, allNames[6], "Fake Rate", "efficFakes2_", false, ", 10 < p_{T} < 15 GeV");
	setEverything(efficFakes3_vs_eta, 0.0, 1.0, false, 0.20, 0.60, 0.75, 0.85, name3, allNames[6], "Fake Rate", "efficFakes3_", false, ", 15 < p_{T} < 20 GeV");
	setEverything(efficFakes4_vs_eta, 0.0, 1.0, false, 0.20, 0.60, 0.75, 0.85, name3, allNames[6], "Fake Rate", "efficFakes4_", false, ", p_{T} > 20 GeV");
	setEverything(fakeRateTOT_vs_eta, 0.0, 1.0, false, 0.20, 0.60, 0.75, 0.85, name3, allNames[6], "Fake Rate", "efficFakesTOT_", false, ", p_{T} > 5 GeV");
    
    double maxRes = 0.1, minRes = 0.003;
    if( allNames[6].find("extractedSTA") != std::string::npos || allNames[6].find("standAloneMuons") != std::string::npos ) {maxRes = 0.3; minRes = 0.05; }
    //RMS min era 0.005

	setEverything(qOverPtRes1_vs_eta, 0.003, 0.1, true, 0.20, 0.65, 0.70, 0.85, name3, allNames[6], "#sigma(p_{T})/p_{T}", "invRes1_", false, ", 5 < p_{T} < 10 GeV");
	setEverything(qOverPtResRMS1_vs_eta, 0.005, 0.1, true, 0.20, 0.65, 0.70, 0.85, name3, allNames[6], "q/p_{T} RMS", "invResRMS1_", false, ", 5 < p_{T} < 10 GeV");
	setEverything(qOverPtRes2_vs_eta, 0.003, 0.1, true, 0.20, 0.65, 0.70, 0.85, name3, allNames[6], "#sigma(p_{T})/p_{T}", "invRes2_", false, ", 10 < p_{T} < 20 GeV");
	setEverything(qOverPtResRMS2_vs_eta, 0.005, 0.1, true, 0.20, 0.65, 0.70, 0.85, name3, allNames[6], "q/p_{T} RMS", "invResRMS2_", false, ", 10 < p_{T} < 20 GeV");
	setEverything(qOverPtRes3_vs_eta, 0.003, 0.1, true, 0.20, 0.65, 0.70, 0.85, name3, allNames[6], "#sigma(p_{T})/p_{T}", "invRes3_", false, ", 20 < p_{T} < 40 GeV");
	setEverything(qOverPtResRMS3_vs_eta, 0.005, 0.1, true, 0.20, 0.65, 0.70, 0.85, name3, allNames[6], "q/p_{T} RMS", "invResRMS3_", false, ", 20 < p_{T} < 40 GeV");
	setEverything(qOverPtRes4_vs_eta, 0.003, 0.1, true, 0.20, 0.65, 0.70, 0.85, name3, allNames[6], "#sigma(p_{T})/p_{T}", "invRes4_", false, ", p_{T} > 40 GeV");
	setEverything(qOverPtResRMS4_vs_eta, 0.005, 0.1, true, 0.20, 0.65, 0.70, 0.85, name3, allNames[6], "q/p_{T} RMS", "invResRMS4_", false, ", p_{T} > 40 GeV");

	setEverything(effic_vs_vtx, minEff, 1.3, false, 0.25, 0.20, 0.75, 0.45, name3, allNames[6], (addString + "Efficiency"), "efficVtx_");
	setEverything(effic_vs_vtxB, minEff, 1.3, false, 0.25, 0.20, 0.75, 0.45, name3, allNames[6], (addString + "Efficiency"), "efficVtxB_", false, ", Barrel");
	setEverything(effic_vs_vtxO, minEff, 1.3, false, 0.25, 0.20, 0.75, 0.45, name3, allNames[6], (addString + "Efficiency"), "efficVtxO_", false, ", Overlap");
	setEverything(effic_vs_vtxE, minEff, 1.3, false, 0.25, 0.20, 0.75, 0.45, name3, allNames[6], (addString + "Efficiency"), "efficVtxE_", false, ", Endcap");

	setEverything(qOverPtRes_vs_eta, minRes, maxRes, true, 0.20, 0.65, 0.70, 0.85, name3, allNames[6], "#sigma(p_{T})/p_{T}", "invRes_");
	setEverything(qOverPtResRMS_vs_eta, minRes, maxRes, true, 0.20, 0.65, 0.70, 0.85, name3, allNames[6], "q/p_{T} RMS", "invResRMS_");

	//double maxForEta = 0.05;
	//if(allNames[6].find("MuonLoose") || allNames[6].find("MuonTight") || allNames[6].find("extracted")) max = 0.02;

	setEverything(yield_vs_eta, 0.0, maxYield, false, 0.20, 0.65, 0.70, 0.85, name3, allNames[6], "Average " + addString + "Bkg Multiplicity / 0.2", "yield_");
	setEverything(yield_vs_eta_sgn, 0.0, maxYield, false, 0.20, 0.65, 0.70, 0.85, name3, allNames[6], "Average " + addString + "Bkg Multiplicity / 0.2", "yieldSgn_");
	setEverything(yield_vs_eta_050, 0.0, maxYield, false, 0.20, 0.65, 0.70, 0.85, name3, allNames[6], "Average " + addString + "Bkg Multiplicity / 0.2", "yield050_", false, ", Quality > 50%");

	/*std::vector<std::string> nomi;
	nomi.push_back("yield_2019");
	nomi.push_back("yield_2019_2");
	nomi.push_back("yield_2019Age");
	nomi.push_back("yield_2023");
	nomi.push_back("yield_2023_2");
	TFile *f1 = new TFile("assoByHits_eta.root","recreate");
	for(int i = 0; i < yield_vs_eta.size(); i++){

		yield_vs_eta[i]->Write(nomi[i].c_str());

	}

	TFile *f2 = new TFile("assoByHits_pt.root","recreate");
	for(int i = 0; i < yield_vs_pt.size(); i++){

		yield_vs_pt[i]->Write(nomi[i].c_str());

	}*/

	double max = 0.05;
	if(allNames[6].find("MuonLoose") || allNames[6].find("MuonTight") || allNames[6].find("extractedGLB")) max = 0.02;
    if(allNames[6].find("extractedSTA")) max = 0.4;

	setEverything(yield_vs_eta_050_pt10, 0.0, maxYield, false, 0.20, 0.65, 0.70, 0.85, name3, allNames[6], "Average Muon Bkg Multiplicity / 0.2", "yield050Pt10_", false, ", Q > 50%, p_{T} > 10 GeV");

	setEverything(chargeMisIDProb_vs_pt, 0.000001, 0.1, true, 0.20, 0.65, 0.70, 0.85, name3, allNames[6], "Charge mis-ID Prob.", "chargeMisIDPt_");
	setEverything(effic_vs_pt, 0.0, 1.1, false, 0.25, 0.20, 0.75, 0.45, name3, allNames[6], (addString + "Efficiency"), "efficPt_");
	setEverything(efficFakes_vs_pt, 0.0, 1.0, false, 0.40, 0.6, 0.90, 0.8, name3, allNames[6], "Fake Rate", "efficFakesPt_");
	setEverything(efficFakes_vs_pt_sgn, 0.0, 1.0, false, 0.40, 0.6, 0.90, 0.8, name3, allNames[6], "Fake Rate", "efficFakesPtSgn_");

	setEverything(qOverPtRes_vs_pt, 0.003, 0.1, true, 0.20, 0.65, 0.70, 0.85, name3, allNames[6], "#sigma(p_{T})/p_{T}", "invResPt_");
	setEverything(qOverPtResRMS_vs_pt, 0.005, 0.1, true, 0.20, 0.65, 0.70, 0.85, name3, allNames[6], "q/p_{T} RMS", "invResRMSPt_");

	setEverything(yield_vs_pt, 0.000001, 50.0, true, 0.30, 0.6, 0.80, 0.8, name3, allNames[6], "Average " + addString + "Bkg Multiplicity", "yieldPt_");
	setEverything(yield_vs_pt_sgn, 0.000001, maxYield, true, 0.25, 0.6, 0.75, 0.8, name3, allNames[6], "Average " + addString + "Bkg Multiplicity / GeV", "yieldPtSgn_");
 
 	setEverything(effic_vs_eta1, minEff, 1.1, false, 0.25, 0.20, 0.75, 0.45, name3, allNames[6], (addString + "Efficiency"), "effic1_", false, ", 5 < p_{T} < 10 GeV");
	setEverything(effic_vs_eta2, minEff, 1.1, false, 0.25, 0.20, 0.75, 0.45, name3, allNames[6], (addString + "Efficiency"), "effic2_", false, ", 10 < p_{T} < 20 GeV/c");
	setEverything(effic_vs_eta3, minEff, 1.1, false, 0.25, 0.20, 0.75, 0.45, name3, allNames[6], (addString + "Efficiency"), "effic3_", false, ", 20 < p_{T} < 40 GeV");
	setEverything(effic_vs_eta4, minEff, 1.1, false, 0.25, 0.20, 0.75, 0.45, name3, allNames[6], (addString + "Efficiency"), "effic4_", false, ", p_{T} > 40 GeV");

	setEverything(yield1_vs_eta, 0.0, maxYield, false, 0.20, 0.65, 0.70, 0.85, name3, allNames[6], "Average " + addString + "Bkg Multiplicity / 0.2", "yield1_", false, ", 5 < p_{T} < 10 GeV");
	setEverything(yield2_vs_eta, 0.0, maxYield, false, 0.20, 0.65, 0.70, 0.85, name3, allNames[6], "Average " + addString + "Bkg Multiplicity / 0.2", "yield2_", false, ", 10 < p_{T} < 20 GeV");
	setEverything(yield3_vs_eta, 0.0, maxYield, false, 0.20, 0.65, 0.70, 0.85, name3, allNames[6], "Average " + addString + "Bkg Multiplicity / 0.2", "yield3_", false, ", 20 < p_{T} < 40 GeV");
	setEverything(yield4_vs_eta, 0.0, maxYield, false, 0.20, 0.65, 0.70, 0.85, name3, allNames[6], "Average " + addString + "Bkg Multiplicity / 0.2", "yield4_", false, ", p_{T} > 40 GeV");

	setEverything(efficB_vs_pt, 0.0, 1.1, false, 0.25, 0.20, 0.75, 0.45, name3, allNames[6], (addString + "Efficiency"), "efficPtB_", false, ", Barrel");
	setEverything(efficO_vs_pt, 0.0, 1.1, false, 0.25, 0.20, 0.75, 0.45, name3, allNames[6], (addString + "Efficiency"), "efficPtO_", false, ", Overlap");
	setEverything(efficE_vs_pt, 0.0, 1.1, false, 0.25, 0.20, 0.75, 0.45, name3, allNames[6], (addString + "Efficiency"), "efficPtE_", false, ", Endcap");

	setEverything(yieldB_vs_pt, 0.000001, 50.0, true, 0.30, 0.6, 0.80, 0.8, name3, allNames[6], "Average " + addString + "Bkg Multiplicity / GeV", "yieldPtB_", false, ", Barrel");
	setEverything(yieldO_vs_pt, 0.000001, 50.0, true, 0.30, 0.6, 0.80, 0.8, name3, allNames[6], "Average " + addString + "Bkg Multiplicity / GeV", "yieldPtO_", false, ", Overlap");
	setEverything(yieldE_vs_pt, 0.000001, 50.0, true, 0.30, 0.6, 0.80, 0.8, name3, allNames[6], "Average " + addString + "Bkg Multiplicity / GeV", "yieldPtE_", false, ", Endcap");

	setEverything(chi2, 0.0, 0.25, false, 0.55, 0.25, 0.85, 0.65, name3, allNames[6], (addString + "#chi^{2}/ndof"), "chi2_");
	setEverything(numRecoPtAsso_vs_pt, 0.1, 50000.0, true, 0.35, 0.25, 0.85, 0.45, name3, allNames[6], "Entries", "numReco_");
    
    double maxFR = 1.0;
    if(allNames[6].find("MuonTight")) maxFR = 0.01;
    if(allNames[6].find("extractedSTA")) maxFR = 1.0;

	setEverything2(fr_vs_eta, 0.0, 1, false, 0.20, 0.65, 0.70, 0.85, name3, allNames[6], "Average Bkg Muon Rate", "fr_");
    setEverything2(fr_vs_eta_50, 0.0, 1, false, 0.20, 0.65, 0.70, 0.85, name3, allNames[6], "Average Bkg Muon Rate", "fr_50_");
	setEverything2(fr_vs_eta_sgn, 0.0, maxFR, false, 0.20, 0.65, 0.70, 0.85, name3, allNames[6], "Average Bkg Muon Rate", "frSgn_");
	setEverything2(fr_vs_pt, 0.0001, 50, true, 0.30, 0.6, 0.80, 0.8, name3, allNames[6], "Average Bkg Muon Rate", "frPt_");
	setEverything2(fr_vs_pt_sgn, 0.0001, 50, true, 0.30, 0.6, 0.80, 0.8, name3, allNames[6], "Average Bkg Muon Rate", "frPtSgn_");

}

void makePlots2(std::vector<std::string> allNames, std::vector<std::string> allNames2, std::string name3){

  std::vector<TH1F*> effic_vs_eta;
  std::vector<TH1F*> efficFakes_vs_eta;
  std::vector<TH1F*> efficFakes1_vs_eta;
  std::vector<TH1F*> efficFakes2_vs_eta;
  std::vector<TH1F*> efficFakes3_vs_eta;
  std::vector<TH1F*> efficFakes4_vs_eta;

  std::vector<TH1F*> numRecoAsso_vs_eta;
  std::vector<TH1F*> numRecoAsso1_vs_eta;
  std::vector<TH1F*> numRecoAsso2_vs_eta;
  std::vector<TH1F*> numRecoAsso3_vs_eta;
  std::vector<TH1F*> numRecoAsso4_vs_eta;

  std::vector<TH1F*> numSimAsso_vs_eta;
  std::vector<TH1F*> numSimAsso1_vs_eta;
  std::vector<TH1F*> numSimAsso2_vs_eta;
  std::vector<TH1F*> numSimAsso3_vs_eta;
  std::vector<TH1F*> numSimAsso4_vs_eta;

  std::vector<TH1F*> numRecoAsso_vs_pt;
  std::vector<TH1F*> numRecoAsso1_vs_pt;
  std::vector<TH1F*> numRecoAsso2_vs_pt;
  std::vector<TH1F*> numRecoAsso3_vs_pt;

  std::vector<TH1F*> numSimAsso_vs_pt;
  std::vector<TH1F*> numSimAsso1_vs_pt;
  std::vector<TH1F*> numSimAsso2_vs_pt;
  std::vector<TH1F*> numSimAsso3_vs_pt;

  std::vector<TH1F*> chi2;

  //1 = loose, 2 = tight 

  for(int i = 0; i < allNames.size()-1; i++){

  	TFile * f1 = TFile::Open(allNames[i].c_str());
  	f1->cd(("/DQMData/Run 1/Muons/Run summary/RecoMuonV/MultiTrack/"+allNames[6]).c_str());

	effic_vs_eta.push_back((TH1F*)gDirectory->Get("effic"));
	efficFakes_vs_eta.push_back((TH1F*)gDirectory->Get("fakerate"));
	efficFakes1_vs_eta.push_back((TH1F*)gDirectory->Get("fakerateBin1"));
	efficFakes2_vs_eta.push_back((TH1F*)gDirectory->Get("fakerateBin2"));
	efficFakes3_vs_eta.push_back((TH1F*)gDirectory->Get("fakerateBin3"));
	efficFakes4_vs_eta.push_back((TH1F*)gDirectory->Get("fakerateBin4"));

	numRecoAsso_vs_eta.push_back((TH1F*)gDirectory->Get("num_assoc(recoToSim)_eta"));
	numRecoAsso1_vs_eta.push_back((TH1F*)gDirectory->Get("num_assoc(recoToSim)_eta_bin1"));
	numRecoAsso2_vs_eta.push_back((TH1F*)gDirectory->Get("num_assoc(recoToSim)_eta_bin2"));
	numRecoAsso3_vs_eta.push_back((TH1F*)gDirectory->Get("num_assoc(recoToSim)_eta_bin3"));
	numRecoAsso4_vs_eta.push_back((TH1F*)gDirectory->Get("num_assoc(recoToSim)_eta_bin4"));

	numSimAsso_vs_eta.push_back((TH1F*)gDirectory->Get("num_assoc(simToReco)_eta"));
	numSimAsso1_vs_eta.push_back((TH1F*)gDirectory->Get("num_assoc(simToReco)_eta_bin1"));
	numSimAsso2_vs_eta.push_back((TH1F*)gDirectory->Get("num_assoc(simToReco)_eta_bin2"));
	numSimAsso3_vs_eta.push_back((TH1F*)gDirectory->Get("num_assoc(simToReco)_eta_bin3"));
	numSimAsso4_vs_eta.push_back((TH1F*)gDirectory->Get("num_assoc(simToReco)_eta_bin4"));

	numRecoAsso_vs_pt.push_back((TH1F*)gDirectory->Get("num_assoc(recoToSim)_pT"));
	numRecoAsso1_vs_pt.push_back((TH1F*)gDirectory->Get("num_assoc(recoToSim)_pT_barrel"));
	numRecoAsso2_vs_pt.push_back((TH1F*)gDirectory->Get("num_assoc(recoToSim)_pT_overlap"));
	numRecoAsso3_vs_pt.push_back((TH1F*)gDirectory->Get("num_assoc(recoToSim)_pT_endcap"));

	numSimAsso_vs_pt.push_back((TH1F*)gDirectory->Get("num_assoc(simToReco)_pT"));
	numSimAsso1_vs_pt.push_back((TH1F*)gDirectory->Get("num_assoc(simToReco)_pT_barrel"));
	numSimAsso2_vs_pt.push_back((TH1F*)gDirectory->Get("num_assoc(simToReco)_pT_overlap"));
	numSimAsso3_vs_pt.push_back((TH1F*)gDirectory->Get("num_assoc(simToReco)_pT_endcap"));

	chi2.push_back((TH1F*)gDirectory->Get("chi2"));

  }

  std::vector<TH1F*> numRecoAssoTOT_vs_eta = sumHistos(numRecoAsso1_vs_eta, numRecoAsso2_vs_eta, numRecoAsso3_vs_eta, numRecoAsso4_vs_eta);
  std::vector<TH1F*> numSimAssoTOT_vs_eta = sumHistos(numSimAsso1_vs_eta, numSimAsso2_vs_eta, numSimAsso3_vs_eta, numSimAsso4_vs_eta);

  std::vector<TH1F*> effic_vs_eta_2;
  std::vector<TH1F*> efficFakes_vs_eta_2;
  std::vector<TH1F*> efficFakes1_vs_eta_2;
  std::vector<TH1F*> efficFakes2_vs_eta_2;
  std::vector<TH1F*> efficFakes3_vs_eta_2;
  std::vector<TH1F*> efficFakes4_vs_eta_2;

  std::vector<TH1F*> numRecoAsso_vs_eta_2;
  std::vector<TH1F*> numRecoAsso1_vs_eta_2;
  std::vector<TH1F*> numRecoAsso2_vs_eta_2;
  std::vector<TH1F*> numRecoAsso3_vs_eta_2;
  std::vector<TH1F*> numRecoAsso4_vs_eta_2;

  std::vector<TH1F*> numSimAsso_vs_eta_2;
  std::vector<TH1F*> numSimAsso1_vs_eta_2;
  std::vector<TH1F*> numSimAsso2_vs_eta_2;
  std::vector<TH1F*> numSimAsso3_vs_eta_2;
  std::vector<TH1F*> numSimAsso4_vs_eta_2;

  std::vector<TH1F*> numRecoAsso_vs_pt_2;
  std::vector<TH1F*> numRecoAsso1_vs_pt_2;
  std::vector<TH1F*> numRecoAsso2_vs_pt_2;
  std::vector<TH1F*> numRecoAsso3_vs_pt_2;

  std::vector<TH1F*> numSimAsso_vs_pt_2;
  std::vector<TH1F*> numSimAsso1_vs_pt_2;
  std::vector<TH1F*> numSimAsso2_vs_pt_2;
  std::vector<TH1F*> numSimAsso3_vs_pt_2;

  std::vector<TH1F*> chi2_2;

  for(int i = 0; i < allNames2.size()-1; i++){

  	TFile * f1 = TFile::Open(allNames2[i].c_str());
  	f1->cd(("/DQMData/Run 1/Muons/Run summary/RecoMuonV/MultiTrack/"+allNames2[5]).c_str());

	effic_vs_eta_2.push_back((TH1F*)gDirectory->Get("effic"));
	efficFakes_vs_eta_2.push_back((TH1F*)gDirectory->Get("fakerate"));
	efficFakes1_vs_eta_2.push_back((TH1F*)gDirectory->Get("fakerateBin1"));
	efficFakes2_vs_eta_2.push_back((TH1F*)gDirectory->Get("fakerateBin2"));
	efficFakes3_vs_eta_2.push_back((TH1F*)gDirectory->Get("fakerateBin3"));
	efficFakes4_vs_eta_2.push_back((TH1F*)gDirectory->Get("fakerateBin4"));

	numRecoAsso_vs_eta_2.push_back((TH1F*)gDirectory->Get("num_assoc(recoToSim)_eta"));
	numRecoAsso1_vs_eta_2.push_back((TH1F*)gDirectory->Get("num_assoc(recoToSim)_eta_bin1"));
	numRecoAsso2_vs_eta_2.push_back((TH1F*)gDirectory->Get("num_assoc(recoToSim)_eta_bin2"));
	numRecoAsso3_vs_eta_2.push_back((TH1F*)gDirectory->Get("num_assoc(recoToSim)_eta_bin3"));
	numRecoAsso4_vs_eta_2.push_back((TH1F*)gDirectory->Get("num_assoc(recoToSim)_eta_bin4"));

	numSimAsso_vs_eta_2.push_back((TH1F*)gDirectory->Get("num_assoc(simToReco)_eta"));
	numSimAsso1_vs_eta_2.push_back((TH1F*)gDirectory->Get("num_assoc(simToReco)_eta_bin1"));
	numSimAsso2_vs_eta_2.push_back((TH1F*)gDirectory->Get("num_assoc(simToReco)_eta_bin2"));
	numSimAsso3_vs_eta_2.push_back((TH1F*)gDirectory->Get("num_assoc(simToReco)_eta_bin3"));
	numSimAsso4_vs_eta_2.push_back((TH1F*)gDirectory->Get("num_assoc(simToReco)_eta_bin4"));

	numRecoAsso_vs_pt_2.push_back((TH1F*)gDirectory->Get("num_assoc(recoToSim)_pT"));
	numRecoAsso1_vs_pt_2.push_back((TH1F*)gDirectory->Get("num_assoc(recoToSim)_pT_barrel"));
	numRecoAsso2_vs_pt_2.push_back((TH1F*)gDirectory->Get("num_assoc(recoToSim)_pT_overlap"));
	numRecoAsso3_vs_pt_2.push_back((TH1F*)gDirectory->Get("num_assoc(recoToSim)_pT_endcap"));

	numSimAsso_vs_pt_2.push_back((TH1F*)gDirectory->Get("num_assoc(simToReco)_pT"));
	numSimAsso1_vs_pt_2.push_back((TH1F*)gDirectory->Get("num_assoc(simToReco)_pT_barrel"));
	numSimAsso2_vs_pt_2.push_back((TH1F*)gDirectory->Get("num_assoc(simToReco)_pT_overlap"));
	numSimAsso3_vs_pt_2.push_back((TH1F*)gDirectory->Get("num_assoc(simToReco)_pT_endcap"));

	chi2_2.push_back((TH1F*)gDirectory->Get("chi2"));

  }

  std::vector<TH1F*> numRecoAssoTOT_vs_eta_2 = sumHistos(numRecoAsso1_vs_eta_2, numRecoAsso2_vs_eta_2, numRecoAsso3_vs_eta_2, numRecoAsso4_vs_eta_2);
  std::vector<TH1F*> numSimAssoTOT_vs_eta_2 = sumHistos(numSimAsso1_vs_eta_2, numSimAsso2_vs_eta_2, numSimAsso3_vs_eta_2, numSimAsso4_vs_eta_2);

	std::string addString = "";
	if(allNames[6].find("MuonLoose") != std::string::npos) addString = "Loose Muon ";
	else if(allNames[6].find("bestMuonTight") != std::string::npos) addString = "Tight Muon ";
    else if(allNames[6].find("Me0Muons") != std::string::npos) addString = "ME0Muon ";
    else if(allNames[6].find("GemMuons") != std::string::npos) addString = "GEMMuon ";
	else addString = "Muon ";

	makeDiff(effic_vs_eta, effic_vs_eta_2);
	makeDiff(efficFakes_vs_eta_2, efficFakes_vs_eta);
	makeDiff(efficFakes1_vs_eta_2, efficFakes1_vs_eta);
	makeDiff(efficFakes2_vs_eta_2, efficFakes2_vs_eta);
	makeDiff(efficFakes3_vs_eta_2, efficFakes3_vs_eta);
	makeDiff(efficFakes4_vs_eta_2, efficFakes4_vs_eta);

	makeDiff(chi2, chi2_2);
	makeDiff(numRecoAsso_vs_pt, numRecoAsso_vs_pt_2);

	std::vector<TH1F*> mismatchReco = makeMismatch(numRecoAsso_vs_eta, numRecoAsso_vs_eta_2);
	std::vector<TH1F*> mismatchReco1 = makeMismatch(numRecoAsso1_vs_eta, numRecoAsso1_vs_eta_2);
	std::vector<TH1F*> mismatchReco2 = makeMismatch(numRecoAsso2_vs_eta, numRecoAsso2_vs_eta_2);
	std::vector<TH1F*> mismatchReco3 = makeMismatch(numRecoAsso3_vs_eta, numRecoAsso3_vs_eta_2);
	std::vector<TH1F*> mismatchReco4 = makeMismatch(numRecoAsso4_vs_eta, numRecoAsso4_vs_eta_2);
	std::vector<TH1F*> mismatchRecoTOT = makeMismatch(numRecoAssoTOT_vs_eta, numRecoAssoTOT_vs_eta_2);

	std::vector<TH1F*> mismatchSim = makeMismatch(numSimAsso_vs_eta, numSimAsso_vs_eta_2);
	std::vector<TH1F*> mismatchSim1 = makeMismatch(numSimAsso1_vs_eta, numSimAsso1_vs_eta_2);
	std::vector<TH1F*> mismatchSim2 = makeMismatch(numSimAsso2_vs_eta, numSimAsso2_vs_eta_2);
	std::vector<TH1F*> mismatchSim3 = makeMismatch(numSimAsso3_vs_eta, numSimAsso3_vs_eta_2);
	std::vector<TH1F*> mismatchSim4 = makeMismatch(numSimAsso4_vs_eta, numSimAsso4_vs_eta_2);
	std::vector<TH1F*> mismatchSimTOT = makeMismatch(numSimAssoTOT_vs_eta, numSimAssoTOT_vs_eta_2);

	std::vector<TH1F*> mismatchReco_pt = makeMismatch(numRecoAsso_vs_pt, numRecoAsso_vs_pt_2);
	std::vector<TH1F*> mismatchReco1_pt = makeMismatch(numRecoAsso1_vs_pt, numRecoAsso1_vs_pt_2);
	std::vector<TH1F*> mismatchReco2_pt = makeMismatch(numRecoAsso2_vs_pt, numRecoAsso2_vs_pt_2);
	std::vector<TH1F*> mismatchReco3_pt = makeMismatch(numRecoAsso3_vs_pt, numRecoAsso3_vs_pt_2);

	std::vector<TH1F*> mismatchSim_pt = makeMismatch(numSimAsso_vs_pt, numSimAsso_vs_pt_2);
	std::vector<TH1F*> mismatchSim1_pt = makeMismatch(numSimAsso1_vs_pt, numSimAsso1_vs_pt_2);
	std::vector<TH1F*> mismatchSim2_pt = makeMismatch(numSimAsso2_vs_pt, numSimAsso2_vs_pt_2);
	std::vector<TH1F*> mismatchSim3_pt = makeMismatch(numSimAsso3_vs_pt, numSimAsso3_vs_pt_2);

	setEverything(chi2, 0.0, 0.25, false, 0.35, 0.75, 0.85, 0.95, name3, allNames[6], (addString + "#chi^{2}/ndof"), "chi2Diff_");
	setEverything(numRecoAsso_vs_pt, 0.1, 50000.0, true, 0.35, 0.75, 0.85, 0.95, name3, allNames[6], "Entries", "numRecoDiff_");

	setEverything(effic_vs_eta, 0.0, 1.0, false, 0.25, 0.20, 0.75, 0.45, name3, allNames[6], (addString + "Efficiency"), "efficDiff_");
	setEverything(efficFakes_vs_eta_2, 0.0, 1.0, false, 0.15, 0.75, 0.65, 0.95, name3, allNames[6], "Fake Rate", "efficFakesDiff_");

	setEverything(efficFakes1_vs_eta_2, 0.0, 1.0, false, 0.15, 0.75, 0.65, 0.95, name3, allNames[6], "Fake Rate [(5 < p_{T} < 10) GeV/c]", "efficFakesDiff1_");
	setEverything(efficFakes2_vs_eta_2, 0.0, 1.0, false, 0.15, 0.75, 0.65, 0.95, name3, allNames[6], "Fake Rate [(10 < p_{T} < 15) GeV/c]", "efficFakesDiff2_");
	setEverything(efficFakes3_vs_eta_2, 0.0, 1.0, false, 0.15, 0.75, 0.65, 0.95, name3, allNames[6], "Fake Rate [(15 < p_{T} < 20) GeV/c]", "efficFakesDiff3_");
	setEverything(efficFakes4_vs_eta_2, 0.0, 1.0, false, 0.15, 0.75, 0.65, 0.95, name3, allNames[6], "Fake Rate [p_{T} > 20 GeV/c]", "efficFakesDiff4_");


	setEverything(mismatchReco, 0.001, 10.0, true, 0.15, 0.75, 0.65, 0.95, name3, allNames[6], "STA-TRK Mismatch (SGN + BKG muons)", "mismatchReco_");
	setEverything(mismatchReco1, 0.001, 10.0, true, 0.15, 0.75, 0.65, 0.95, name3, allNames[6], "STA-TRK Mismatch (SGN + BKG muons)", "mismatchReco1_");
	setEverything(mismatchReco2, 0.001, 10.0, true, 0.15, 0.75, 0.65, 0.95, name3, allNames[6], "STA-TRK Mismatch (SGN + BKG muons)", "mismatchReco2_");
	setEverything(mismatchReco3, 0.001, 10.0, true, 0.15, 0.75, 0.65, 0.95, name3, allNames[6], "STA-TRK Mismatch (SGN + BKG muons)", "mismatchReco3_");
	setEverything(mismatchReco4, 0.001, 10.0, true, 0.15, 0.75, 0.65, 0.95, name3, allNames[6], "STA-TRK Mismatch (SGN + BKG muons)", "mismatchReco4_");
	setEverything(mismatchRecoTOT, 0.001, 10.0, true, 0.15, 0.75, 0.65, 0.95, name3, allNames[6], "STA-TRK Mismatch (SGN + BKG muons, p_{T} > 5 GeV/c)", "mismatchRecoTOT_");

	setEverything(mismatchSim, 0.001, 10.0, true, 0.15, 0.75, 0.65, 0.95, name3, allNames[6], "STA-TRK Mismatch (only SGN muons)", "mismatchSim_");
	setEverything(mismatchSim1, 0.001, 10.0, true, 0.15, 0.75, 0.65, 0.95, name3, allNames[6], "STA-TRK Mismatch (only SGN muons)", "mismatchSim1_");
	setEverything(mismatchSim2, 0.001, 10.0, true, 0.15, 0.75, 0.65, 0.95, name3, allNames[6], "STA-TRK Mismatch (only SGN muons)", "mismatchSim2_");
	setEverything(mismatchSim3, 0.001, 10.0, true, 0.15, 0.75, 0.65, 0.95, name3, allNames[6], "STA-TRK Mismatch (only SGN muons)", "mismatchSim3_");
	setEverything(mismatchSim4, 0.001, 10.0, true, 0.15, 0.75, 0.65, 0.95, name3, allNames[6], "STA-TRK Mismatch (only SGN muons)", "mismatchSim4_");
	setEverything(mismatchSimTOT, 0.001, 10.0, true, 0.15, 0.75, 0.65, 0.95, name3, allNames[6], "STA-TRK Mismatch (only SGN muons, p_{T} > 5 GeV/c)", "mismatchSimTOT_");


	setEverything(mismatchReco_pt, 0.001, 10.0, true, 0.15, 0.75, 0.65, 0.95, name3, allNames[6], "STA-TRK Mismatch (SGN + BKG muons)", "mismatchReco_pt_");
	setEverything(mismatchReco1_pt, 0.001, 10.0, true, 0.15, 0.75, 0.65, 0.95, name3, allNames[6], "STA-TRK Mismatch (SGN + BKG muons, Barrel)", "mismatchReco1_pt_");
	setEverything(mismatchReco2_pt, 0.001, 10.0, true, 0.15, 0.75, 0.65, 0.95, name3, allNames[6], "STA-TRK Mismatch (SGN + BKG muons, Overlap)", "mismatchReco2_pt_");
	setEverything(mismatchReco3_pt, 0.001, 10.0, true, 0.15, 0.75, 0.65, 0.95, name3, allNames[6], "STA-TRK Mismatch (SGN + BKG muons, Endcap)", "mismatchReco3_pt_");

	setEverything(mismatchSim_pt, 0.001, 10.0, true, 0.15, 0.75, 0.65, 0.95, name3, allNames[6], "STA-TRK Mismatch (only SGN muons)", "mismatchSim_pt_");
	setEverything(mismatchSim1_pt, 0.001, 10.0, true, 0.15, 0.75, 0.65, 0.95, name3, allNames[6], "STA-TRK Mismatch (only SGN muons, Barrel)", "mismatchSim1_pt_");
	setEverything(mismatchSim2_pt, 0.001, 10.0, true, 0.15, 0.75, 0.65, 0.95, name3, allNames[6], "STA-TRK Mismatch (only SGN muons, Overlap)", "mismatchSim2_pt_");
	setEverything(mismatchSim3_pt, 0.001, 10.0, true, 0.15, 0.75, 0.65, 0.95, name3, allNames[6], "STA-TRK Mismatch (only SGN muons, Endcap)", "mismatchSim3_pt_");

}

void makeAll(std::string type = "bestMuonLoose_AssociatorByPull"){

    std::string path = "/Users/cesarecalabria/Desktop/ByHits_90X/";
    
//    std::string nome_2023_FS =    "DQM_V0001_R000000001__Global__CMSSW_9_0_0_pre1__RECO_PU0_D3_ZMM.root";
//    std::string nome_2023_FSAge = "DQM_V0001_R000000001__Global__CMSSW_9_0_0_pre1__RECO_PU140_D3_ZMM.root";
//    std::string nome_2023_235 =   "DQM_V0001_R000000001__Global__CMSSW_9_0_0_pre1__RECO_PU200_D3_ZMM.root";
//    std::string nome_2023_200 =   "DQM_V0001_R000000001__Global__CMSSW_9_0_0_pre1__RECO_PU0_D4_ZMM.root";
//    std::string nome_2023 =       "DQM_V0001_R000000001__Global__CMSSW_9_0_0_pre1__RECO_PU140_D4_ZMM.root";
//    std::string nome_2023No =     "DQM_V0001_R000000001__Global__CMSSW_9_0_0_pre1__RECO_PU200_D4_ZMM.root";
    
    std::string nome_2023_FS =    "DQM_V0001_R000000001__Global__CMSSW_9_0_0_pre2__RECO_PU0_D4Timing_ZMM.root";
    std::string nome_2023_FSAge = "DQM_V0001_R000000001__Global__CMSSW_9_0_0_pre2__RECO_PU200_D4Timing_ZMM.root";
    std::string nome_2023_235 =   "DQM_V0001_R000000001__Global__CMSSW_9_0_0_pre2__RECO_PU200_D4Timing_ZMM.root";
    std::string nome_2023_200 =   "DQM_V0001_R000000001__Global__CMSSW_9_0_0_pre1__RECO_PU0_D4_ZMM.root";
    std::string nome_2023 =       "DQM_V0001_R000000001__Global__CMSSW_9_0_0_pre1__RECO_PU140_D4_ZMM.root";
    std::string nome_2023No =     "DQM_V0001_R000000001__Global__CMSSW_9_0_0_pre1__RECO_PU200_D4_ZMM.root";

    //Order: 2019, 2019NoUpg, 2019Age, 2023, 2023NoUpg
    
    std::vector<std::string> allNames;
    allNames.push_back((path+nome_2023_FS));
    allNames.push_back((path+nome_2023_FSAge));
    allNames.push_back((path+nome_2023_235));
    allNames.push_back((path+nome_2023_200));
    allNames.push_back((path+nome_2023));
    allNames.push_back((path+nome_2023No));
    allNames.push_back(type);
    
    makePlots(allNames,"DY");

}

void makeAll2(std::string type = "bestMuonLoose_AssociatorByPull", std::string type2 = "bestMuonLoose_AssociatorByPull"){

    std::string path = "/Users/cesarecalabria/Desktop/ByHits_90X/";
    
    std::string nome_2023_FS =    "DQM_V0001_R000000001__Global__CMSSW_9_0_0_pre1__RECO_PU0_D3_ZMM.root";
    std::string nome_2023_FSAge = "DQM_V0001_R000000001__Global__CMSSW_9_0_0_pre1__RECO_PU140_D3_ZMM.root";
    std::string nome_2023_235 =   "DQM_V0001_R000000001__Global__CMSSW_9_0_0_pre1__RECO_PU200_D3_ZMM.root";
    std::string nome_2023_200 =   "DQM_V0001_R000000001__Global__CMSSW_9_0_0_pre1__RECO_PU0_D4_ZMM.root";
    std::string nome_2023 =       "DQM_V0001_R000000001__Global__CMSSW_9_0_0_pre1__RECO_PU140_D4_ZMM.root";
    std::string nome_2023No =     "DQM_V0001_R000000001__Global__CMSSW_9_0_0_pre1__RECO_PU200_D4_ZMM.root";
	
	//Order: 2019, 2019NoUpg, 2019Age, 2023, 2023NoUpg

    std::vector<std::string> allNames;
    allNames.push_back((path+nome_2023_FS));
    allNames.push_back((path+nome_2023_FSAge));
    allNames.push_back((path+nome_2023_235));
    allNames.push_back((path+nome_2023_200));
    allNames.push_back((path+nome_2023));
    allNames.push_back((path+nome_2023No));
    allNames.push_back(type);

	std::vector<std::string> allNames2;
	allNames2.push_back((path+nome_2019));
	allNames2.push_back((path+nome_2019No));
	allNames2.push_back((path+nome_2019Age));
	allNames2.push_back((path+nome_2023));
	allNames2.push_back((path+nome_2023No));
	allNames2.push_back((path+nome_2019AgeNo));
	allNames2.push_back(type);

	makePlots2(allNames,allNames2,"DY");

}

void makeAllAll(){

	makeAll("extractedSTAMuons2StatTiming_tpToStaUpdSel2MuonAssociation2StTime"); //valid hit + muon purity
    makeAll("Cut5/extractedSTAMuons2StatTiming_tpToStaUpdSel2MuonAssociation2StTime"); //valid hit + muon purity

    makeAll("standAloneMuons_UpdatedAtVtx_tpToStaUpdMuonAssociation"); //default
	makeAll("extractedGlobalMuons_tpToGlbMuonAssociation"); //default
	
//	makeAll("bestMuonLoose_tpToLooseMuonAssociation");
//	makeAll("bestMuonTight_tpToTightMuonAssociation");
//
//	makeAll("bestMuonLoose_tpToLooseSelMuonAssociation");
//	makeAll("bestMuonTight_tpToTightSelMuonAssociation");
//
//	makeAll("bestMuonLoose_tpToLooseSel2MuonAssociation");
//	makeAll("bestMuonTight_tpToTightSel2MuonAssociation");
    
	////makeAll("Cut5/extractedSTAMuons2StatTiming_tpToStaUpdSel2MuonAssociation2StTime");

	makeAll("Cut5/bestMuonLoose_tpToLooseSel0MuonAssociation");
	makeAll("Cut5/bestMuonTight_tpToTightSel0MuonAssociation");
    
    makeAll("Cut5/bestMuonLooseMod_tpToLooseModSel0MuonAssociation");
    makeAll("Cut5/bestMuonTightMod_tpToTightModSel0MuonAssociation");

	makeAll("bestMuonLoose_tpToLooseSel0MuonAssociation");
	makeAll("bestMuonTight_tpToTightSel0MuonAssociation");
    
    makeAll("bestMuonLooseMod_tpToLooseModSel0MuonAssociation");
    makeAll("bestMuonTightMod_tpToTightModSel0MuonAssociation");
    
    makeAll("extractGemMuons_tpToGEMMuonMuonAssociation");
    makeAll("extractMe0Muons_tpToME0MuonMuonAssociation");
    
//    makeAll("Cut5/extractGemMuons_tpToGEMMuonMuonAssociation");
//    makeAll("Cut5/extractMe0Muons_tpToME0MuonMuonAssociation");

    ////makeAll("bestMuonLoose5_tpToLooseSel05MuonAssociation");
	////makeAll("bestMuonTight5_tpToTightSel05MuonAssociation");

	//makeAll("bestMuonLoose_tpToLooseSel3MuonAssociation");
	//makeAll("bestMuonTight_tpToTightSel3MuonAssociation");

	//makeAll("bestMuonLoose5_tpToLooseSel35MuonAssociation");
	//makeAll("bestMuonTight5_tpToTightSel35MuonAssociation");

	//makeAll("Cut5/bestMuonLoose_tpToLooseSel3MuonAssociation");
	//makeAll("Cut5/bestMuonTight_tpToTightSel3MuonAssociation");
	
	/*makeAll2("extractedGlobalMuons_tpToGlbSel2MuonAssociation", "extractedGlobalMuons_tpToGlbSelMuonAssociation");
	makeAll2("extractedGlobalMuons_tpToGlbSel4MuonAssociation", "extractedGlobalMuons_tpToGlbSel3MuonAssociation");*/
}

