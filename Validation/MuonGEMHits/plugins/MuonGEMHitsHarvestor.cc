#include "Validation/MuonGEMHits/plugins/MuonGEMHitsHarvestor.h"

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "TTree.h"
#include "TFile.h"
#include "TGraphAsymmErrors.h"
#include "TEfficiency.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Utilities/interface/InputTag.h"

///Data Format
#include "DataFormats/GEMDigi/interface/GEMDigiCollection.h"
#include "DataFormats/MuonDetId/interface/GEMDetId.h"
#include "DataFormats/GeometrySurface/interface/LocalError.h"
#include "DataFormats/GeometryVector/interface/LocalPoint.h"
#include "DataFormats/Scalers/interface/DcsStatus.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Math/interface/deltaPhi.h"

#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"

///Geometry
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"

#include "Geometry/GEMGeometry/interface/GEMGeometry.h"
#include "Geometry/GEMGeometry/interface/GEMEtaPartition.h"
#include "Geometry/GEMGeometry/interface/GEMEtaPartitionSpecs.h"
#include "Geometry/CommonTopologies/interface/StripTopology.h"

#include "DQMServices/Core/interface/DQMStore.h"

///Log messages
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"


MuonGEMHitsHarvestor::MuonGEMHitsHarvestor(const edm::ParameterSet& ps)
{
  dbe_path_ = std::string("MuonGEMHitsV/GEMHitsTask/");
}


MuonGEMHitsHarvestor::~MuonGEMHitsHarvestor()
{
}


TProfile* MuonGEMHitsHarvestor::ComputeEff(TH1F* passed, TH1F* total)
{

  TEfficiency eff(*passed, *total);
  TGraphAsymmErrors* eff_graph = eff.CreateGraph();

  std::string name  = "eff_" + std::string(passed->GetName());
  std::string title = "Eff. " + std::string(passed->GetTitle());
  TProfile * eff_profile = new TProfile(name.c_str(),
                                        title.c_str(),
                                        total->GetXaxis()->GetNbins(),
                                        total->GetXaxis()->GetXmin(),
                                        total->GetXaxis()->GetXmax());

  for (int bin = 1; bin <= eff_graph->GetN(); bin++)
  {
    double x, y;
    eff_graph->GetPoint(bin, x, y);
    double error = eff_graph->GetErrorY(bin);

    eff_profile->SetBinContent(bin, y);
    eff_profile->SetBinEntries(bin, 1);
    eff_profile->SetBinError(bin, error);
  }
  return eff_profile;

  /*
  if ( num==nullptr || denum==nullptr) { std::cout<<"num or denum are missing"<<std::endl; } 
  std::string name = "eff_"+std::string(num->GetName());
  std::string title = "Eff. "+std::string(num->GetTitle());
  TProfile * efficHist = new TProfile(name.c_str(), title.c_str(),num->GetXaxis()->GetNbins(), num->GetXaxis()->GetXmin(),num->GetXaxis()->GetXmax());
  if ( num->GetNbinsX() != denum->GetNbinsX()) { std::cout<<"Wrong Xbin. Please, check histogram's name"<<std::endl; return nullptr;  }
  for (int i=1; i <= num->GetNbinsX(); i++) {
    const double nNum = num->GetBinContent(i);
    const double nDenum = denum->GetBinContent(i);
    if ( nDenum == 0 || nNum > nDenum ) continue;
    if ( nNum == 0 ) continue;
    const double effVal = nNum/nDenum;
    const double errLo = TEfficiency::ClopperPearson((int)nDenum,(int)nNum,0.683,false);
    const double errUp = TEfficiency::ClopperPearson((int)nDenum,(int)nNum,0.683,true);
    const double errVal = (effVal - errLo > errUp - effVal) ? effVal - errLo : errUp - effVal;
    efficHist->SetBinContent(i, effVal);
    efficHist->SetBinEntries(i, 1);
    efficHist->SetBinError(i, sqrt(effVal * effVal + errVal * errVal));
  }
  return efficHist;
  */
}


void MuonGEMHitsHarvestor::ProcessBooking( DQMStore::IBooker& ibooker, DQMStore::IGetter& ig, std::string label_suffix, TH1F* track_hist, TH1F* sh_hist )
{
  TString dbe_label = TString(dbe_path_)+label_suffix;
  //std::cout<<dbe_label<<"   "<<track_hist->GetName()<<std::endl;
  if( ig.get(dbe_label.Data()) != nullptr && track_hist !=nullptr ) {
    TH1F* hist = (TH1F*)ig.get( dbe_label.Data() )->getTH1F()->Clone();
    TProfile* profile = ComputeEff( hist, track_hist);
    TString x_axis_title = TString(hist->GetXaxis()->GetTitle());
    TString title = TString::Format("Eff. for a SimTrack to have an associated GEM Strip in %s;%s;Eff.",label_suffix.c_str(),x_axis_title.Data());
    profile->SetTitle( title.Data());
    ibooker.bookProfile( profile->GetName(),profile);
    if ( sh_hist!=nullptr) {
      TProfile* profile_sh = ComputeEff( hist, sh_hist );
      profile_sh->SetName( (profile->GetName()+std::string("_sh")).c_str());
      TString title2 = TString::Format("Eff. for a SimTrack to have an associated GEM Strip in %s with a matched SimHit;%s;Eff.",label_suffix.c_str(),x_axis_title.Data() );
      profile_sh->SetTitle( title2.Data() );
      ibooker.bookProfile( profile_sh->GetName(),profile_sh);
    }
  }
  return;
}

void MuonGEMHitsHarvestor::dqmEndJob(DQMStore::IBooker & ibooker,
                                     DQMStore::IGetter &ig ) {
  ig.setCurrentFolder(dbe_path_);
}



//define this as a plug-in
DEFINE_FWK_MODULE(MuonGEMHitsHarvestor);
