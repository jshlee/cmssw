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
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Utilities/interface/InputTag.h"

///Data Format
#include "DataFormats/GEMDigi/interface/GEMDigiCollection.h"
#include "DataFormats/GEMDigi/interface/GEMPadDigiCollection.h"
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

#include "Validation/MuonGEMDigis/plugins/MuonGEMDigisHarvestor.h"

MuonGEMDigisHarvestor::MuonGEMDigisHarvestor(const edm::ParameterSet& ps)
{
  dbe_path_ = ps.getParameter<std::string>("dbePath");
  dbe_hist_prefix_ = ps.getParameter<std::string>("dbeHistPrefix");
  compareable_dbe_path_ = ps.getParameter<std::string>("compareDBEPath");
  compareable_dbe_hist_prefix_ = ps.getParameter<std::string>("compareDBEHistPrefix");

}


MuonGEMDigisHarvestor::~MuonGEMDigisHarvestor()
{
}
TProfile* MuonGEMDigisHarvestor::ComputeEff(TH1F* num, TH1F* denum )
{
  std::string name  = "eff_"+std::string(num->GetName());
  std::string title = "Eff. "+std::string(num->GetTitle());
  TProfile * efficHist = new TProfile(name.c_str(), title.c_str(),denum->GetXaxis()->GetNbins(), denum->GetXaxis()->GetXmin(),denum->GetXaxis()->GetXmax());

  for (int i=1; i <= denum->GetNbinsX(); i++) {

    double nNum = num->GetBinContent(i);
    double nDenum = denum->GetBinContent(i);
    if ( nDenum == 0 || nNum ==0  ) {
      continue;
    }
    if ( nNum > nDenum ) {
      double temp = nDenum;
      nDenum = nNum;
      nNum = temp;
      LogDebug("MuonGEMDigisHarvestor")<<"Alert! specific bin's num is bigger than denum";
    }
    const double effVal = nNum/nDenum;
    efficHist->SetBinContent(i, effVal);
    efficHist->SetBinEntries(i,1);
    efficHist->SetBinError(i,0);
    const double errLo = TEfficiency::ClopperPearson((int)nDenum,(int)nNum,0.683,false);
    const double errUp = TEfficiency::ClopperPearson((int)nDenum,(int)nNum,0.683,true);
    const double errVal = (effVal - errLo > errUp - effVal) ? effVal - errLo : errUp - effVal;
    efficHist->SetBinError(i, sqrt(effVal * effVal + errVal * errVal));
  }
  return efficHist;
}

void MuonGEMDigisHarvestor::ProcessBooking( DQMStore::IBooker& ibooker, DQMStore::IGetter& ig, const char* label, TString suffix, TH1F* track_hist, TH1F* sh_hist )
{
  TString dbe_label = TString(dbe_path_)+label+suffix;
  if( ig.get(dbe_label.Data()) != nullptr && sh_hist !=nullptr && track_hist !=nullptr ) {
    TH1F* hist =    (TH1F*)ig.get( dbe_label.Data() )->getTH1F()->Clone();
    TProfile* profile = ComputeEff( hist, track_hist);
    TProfile* profile_sh = ComputeEff( hist, sh_hist );
    profile_sh->SetName( (profile->GetName()+std::string("_sh")).c_str());
    TString x_axis_title = TString(hist->GetXaxis()->GetTitle());
    TString title  = TString::Format("Eff. for a SimTrack to have an associated GEM digi in %s;%s;Eff.",suffix.Data(),x_axis_title.Data());
    TString title2 = TString::Format("Eff. for a SimTrack to have an associated GEM digi in %s with a matched SimHit;%s;Eff.",suffix.Data(),x_axis_title.Data() );
    profile->SetTitle( title.Data());
    profile_sh->SetTitle( title2.Data() );
    ibooker.bookProfile( profile->GetName(),profile); 
    ibooker.bookProfile( profile_sh->GetName(),profile_sh); 
  }
  else {
    LogDebug("MuonGEMDigisHarvestor")<<"Can not found histogram of "<<dbe_label; 
    if ( track_hist == nullptr) LogDebug("MuonGEMDigisHarvestor")<<"track not found";
    if ( sh_hist    == nullptr) LogDebug("MuonGEMDigisHarvestor")<<"sh_hist not found";
  }
  return;
}


void  MuonGEMDigisHarvestor::dqmEndJob(DQMStore::IBooker& ibooker, DQMStore::IGetter& ig)
{
  ig.setCurrentFolder(dbe_path_);
  LogDebug("MuonGEMDigisHarvestor")<<":D\n";
}


//define this as a plug-in
DEFINE_FWK_MODULE(MuonGEMDigisHarvestor);
