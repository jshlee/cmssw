#include "Validation/MuonGEMRecHits/plugins/MuonGEMRecHitsHarvestor.h"

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
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "TTree.h"
#include "TFile.h"
#include "TGraphAsymmErrors.h"
#include "TH1F.h"
#include "TEfficiency.h"
#include "TProfile.h"
#include "TAxis.h"
#include "TSystem.h"


///Data Format
#include <DataFormats/GEMRecHit/interface/GEMRecHit.h>
#include <DataFormats/GEMRecHit/interface/GEMRecHitCollection.h>
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

#include "Validation/MuonGEMHits/interface/GEMValidationUtils.h"



MuonGEMRecHitsHarvestor::MuonGEMRecHitsHarvestor(const edm::ParameterSet& ps) {
  dbe_path_ = "MuonGEMRecHitsV/GEMRecHitsTask/";

  region_ids_ = ps.getUntrackedParameter< std::vector<int> >("regionIds");
  station_ids_ = ps.getUntrackedParameter< std::vector<int> >("stationIds");
  layer_ids_ = ps.getUntrackedParameter< std::vector<int> >("layerIds");
}


MuonGEMRecHitsHarvestor::~MuonGEMRecHitsHarvestor() {
}


TProfile* MuonGEMRecHitsHarvestor::computeEff(const TH1F & passed,
                                              const TH1F & total) {
  std::string name  = "eff_" + std::string(passed.GetName());
  std::string title = "Eff. " + std::string(passed.GetTitle());
  TProfile * eff_profile = new TProfile(name.c_str(),
                                        title.c_str(),
                                        total.GetXaxis()->GetNbins(),
                                        total.GetXaxis()->GetXmin(),
                                        total.GetXaxis()->GetXmax());


  if(TEfficiency::CheckConsistency(passed, total)) {
    TEfficiency        eff(passed, total);
    TGraphAsymmErrors* eff_graph = eff.CreateGraph();
    auto x_axis = passed.GetXaxis();
    // TH1F* eff_hist = dynamic_cast<TH1F*>(eff_graph->GetHistogram()->Clone());

    // for (int bin = 1; bin <= eff_hist->GetNbinsX(); bin++)
    for(int i = 0; i < eff_graph->GetN(); ++i) {

      double x, y, error;
      bool invalid_request = eff_graph->GetPoint(i, x, y) == -1;

      if(invalid_request) {
        edm::LogError("MuonGEMRecHitsHarvestor") << "GetPoint failed" << std::endl;
        continue;
      }

      error = eff_graph->GetErrorY(i);
      int bin = x_axis->FindBin(x);
      eff_profile->SetBinContent(bin, y);
      eff_profile->SetBinError(bin, error);
      eff_profile->SetBinEntries(bin, 1);
    }
  } else {
    edm::LogError("MuonGEMRecHitsHarvestor") << "TEfficiency Inconsistency Error" << std::endl;
    std::cout << passed.GetName() << std::endl;
    std::cout << total.GetName() << std::endl;

    name += "_inconsistent";
    title += " inconsistent";
    eff_profile->SetName(name.c_str());
    eff_profile->SetTitle(title.c_str());
  }

  return eff_profile;
}


void MuonGEMRecHitsHarvestor::dqmEndJob(DQMStore::IBooker& ibooker,
                                        DQMStore::IGetter& ig) {
  // ig.setCurrentFolder(dbe_path_);
  ig.cd(dbe_path_);

  for(Int_t region_id : region_ids_) {
    for(Int_t station_id : station_ids_) {
      for(Int_t layer_id : layer_ids_) {
        for(const char* axis : kAxes_) {
          TString name_suffix = GEMUtils::getSuffixName(region_id, station_id, layer_id);
          TString title_suffix = GEMUtils::getSuffixTitle(region_id, station_id, layer_id);

          TString sim_name = TString::Format("simhit_occ_%s%s", axis, name_suffix.Data());
          TString rec_name = TString::Format("rechit_occ_%s%s", axis, name_suffix.Data());

          const std::string sim_path = gSystem->ConcatFileName(dbe_path_.c_str(), sim_name);
          const std::string rec_path = gSystem->ConcatFileName(dbe_path_.c_str(), rec_name);

          TH1F* sim_occupancy;
          if(auto tmp_me = ig.get(sim_path)) {
            sim_occupancy = dynamic_cast<TH1F*>(tmp_me->getTH1F()->Clone());
          } else {
            edm::LogError(kLogCategory_) << "failed to get " << sim_name << std::endl;
            continue;
          }
          // XXX reason ?
          sim_occupancy->Sumw2();

          TH1F* rec_occupancy;
          if(auto tmp_me = ig.getElement(rec_path)) {
            rec_occupancy = dynamic_cast<TH1F*>(tmp_me->getTH1F()->Clone());
          } else {
            edm::LogError("MuonGEMRecHitsHarvestor") << "failed to get "
                                                     << rec_name
                                                     << std::endl;
            continue;
          }
          rec_occupancy->Sumw2();

          TProfile* eff = computeEff(*rec_occupancy, *sim_occupancy);

          TString title = TString::Format("Efficiency %s :%s", axis, title_suffix.Data());
          TString x_title = TString::Format("#%s", axis);

          eff->SetTitle(title);
          eff->SetXTitle(x_title);

          TString name = TString::Format("eff_%s%s", axis, name_suffix.Data());

          ibooker.bookProfile(name, eff);
        } // axis loop
      } // Layer Id END
    } // Station Id END
  } // Region Id END
}


//define this as a plug-in
DEFINE_FWK_MODULE(MuonGEMRecHitsHarvestor);

