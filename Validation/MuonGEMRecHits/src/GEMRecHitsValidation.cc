#include "Validation/MuonGEMRecHits/interface/GEMRecHitsValidation.h"
#include "Validation/MuonGEMHits/interface/GEMValidationUtils.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
#include <iomanip>

using namespace std;


GEMRecHitsValidation::GEMRecHitsValidation(const edm::ParameterSet& ps): GEMBaseValidation(ps) {
  auto sim_hit_label = ps.getParameter<edm::InputTag>("simInputLabel");
  auto rec_hit_label = ps.getParameter<edm::InputTag>("recHitsInputLabel");

  sim_hit_token_ = consumes<edm::PSimHitContainer>(sim_hit_label);
  rec_hit_token_ = consumes<GEMRecHitCollection>(rec_hit_label);

  detailPlot_ = ps.getParameter<Bool_t>("detailPlot");
  folder_ = ps.getParameter<std::string>("folder");
}


GEMRecHitsValidation::~GEMRecHitsValidation() {
}


void GEMRecHitsValidation::bookHistograms(DQMStore::IBooker & ibooker,
                                          edm::Run const & Run,
                                          edm::EventSetup const & iSetup) {

  const GEMGeometry* kGEMGeometry = initGeometry(iSetup);
  if ( kGEMGeometry == nullptr) {
    edm::LogError(kLogCategory_) << "Failed to initialise GEMGeometry in the bookHistograms step." << std::endl;
    return ;  
  }

  ibooker.setCurrentFolder(folder_);

  // Book histograms
  for(const auto & region : kGEMGeometry->regions()) {
    Int_t region_id = region->region();

    if(auto tmp_me = bookZROccupancy(ibooker, region_id, "rechit", "RecHit")) {
      me_occ_zr_[region_id] = tmp_me;
    } else {
      edm::LogError(kLogCategory_) << "Failed to create simpleZR histograms" << std::endl;
    }

    for(const auto & station : region->stations()) {
      Int_t station_id = station->station();
      ME2IdsKey key2(region_id, station_id);

      if (auto tmp_me = bookDetectorOccupancy(ibooker, key2, station, "rechit", "RecHit")) {
        me_occ_det_[key2] = tmp_me;
      } else {
        edm::LogError(kLogCategory_) << "Failed to create occupancy per detector component" << std::endl;
      }
    } // station loop end

    me_pull_x_[region_id] = bookHist1D(ibooker, region_id, "pull_x", "Pull Of X", 100, -50, 50);
    me_pull_y_[region_id] = bookHist1D(ibooker, region_id, "pull_y", "Pull Of Y", 100, -50, 50);

  } // region loop end

  me_cls_ = ibooker.book1D("cls_tot", "Cluster Size Distribution", 11, -0.5, 10.5);
 
  // TODO logging 
  const Double_t kPi = TMath::Pi();
  if(detailPlot_) {
    for(const auto & region : kGEMGeometry->regions()) {
      Int_t region_id = region->region();

      for(const auto & station : region->stations()) {
        Int_t station_id = station->station();

        // TODO
        for(Int_t layer_id : {1, 2}) {

          ME3IdsKey key(region_id, station_id, layer_id);
          me_detail_occ_zr_[key] = bookZROccupancy(ibooker, key, "rechit", "RecHits");
          me_detail_occ_xy_[key] = bookXYOccupancy(ibooker, key, "rechit", "RecHits");
          me_detail_occ_polar_[key] = bookPolarOccupancy(ibooker, key, "rechit", "recHits");

          // bookHist1D(ibooker, name, title, nbinsx, xlow, xup, region_id, station_id, layer_id)
          me_detail_cls_[key] = bookHist1D(
              ibooker, key, "cls", "Cluster Size Distribution", 11, -0.5, 10.5);

          // Occupancy histograms of SimHits and RecHits for Efficiency
          me_detail_pull_x_[key] = bookHist1D(
              ibooker, key, "pull_x", "Pull Of X", 100, -50, 50);

          me_detail_pull_y_[key] = bookHist1D(
              ibooker, key, "pull_y", "Pull Of Y", 100, -50, 50);

          me_detail_sim_occ_eta_[key] = bookHist1D(
              ibooker, key, "simhit_occ_eta", "SimHit Eta Occupancy", 51, -4, 4, "#eta");

          me_detail_rec_occ_eta_[key] = bookHist1D(
              ibooker, key, "rechit_occ_eta", "RecHit Eta Occupancy", 51, -4, 4, "#eta");

          me_detail_sim_occ_phi_[key] = bookHist1D(
              ibooker, key, "simhit_occ_phi", "SimHit Phi Occupancy", 51, -kPi, kPi, "#phi");

          me_detail_rec_occ_phi_[key] = bookHist1D(
              ibooker, key, "rechit_occ_phi", "RecHit Phi Occupancy", 51, -kPi, kPi, "#phi");

        } // layer loop
      } // station loop
    } // region loop

  } // detailPlot 


  LogDebug("GEMRecHitsValidation")<<"Booking End.\n";
}



void GEMRecHitsValidation::analyze(const edm::Event& e,
                                   const edm::EventSetup& iSetup) {

  const GEMGeometry* kGEMGeometry  = initGeometry(iSetup);
  if ( kGEMGeometry == nullptr) {
    edm::LogError(kLogCategory_) << "Failed to init GEMGeometry.\n";
    return; 
  }

  edm::Handle<edm::PSimHitContainer> simhit_container;
  e.getByToken(sim_hit_token_, simhit_container);
  if (not simhit_container.isValid()) {
    edm::LogError(kLogCategory_) << "Failed to get PSimHitContainer.\n";
    return ;
  }

  edm::Handle<GEMRecHitCollection> rechit_collection;
  e.getByToken(rec_hit_token_, rechit_collection);
  if (not rechit_collection.isValid()) {
    edm::LogError(kLogCategory_) << "Failed to get GEMRecHitCollection\n";
    return ;
  }

  // rechit occupancy only



  // TODO if(isMC) {
  for(const auto & simhit : *simhit_container.product()) {

    if (not (std::abs(simhit.particleType()) == kMuonPDGId_)) {
      std::cout << "PSimHit is not muon" << std::endl;
      continue;
    }

    if (not kGEMGeometry->idToDet(simhit.detUnitId())) {
      std::cout << "simHit did not matched with GEMGeometry." << std::endl;
      continue;
    }

    const unsigned kSimDetUnitId = simhit.detUnitId();
    GEMDetId sim_id(kSimDetUnitId);

    Int_t region_id = sim_id.region();
    Int_t station_id = sim_id.station();
    Int_t layer_id = sim_id.layer();
    Int_t chamber_id = sim_id.chamber();
    Int_t roll_id = sim_id.roll();

    ME2IdsKey key2(region_id, station_id);
    ME3IdsKey key3(region_id, station_id, layer_id);

    // FIXME
    LocalPoint sim_local = simhit.localPosition();
    GlobalPoint sim_global = kGEMGeometry->idToDet(sim_id)->surface().toGlobal(sim_local);

    // TODO +1 reason
    Int_t sim_fired_strip = kGEMGeometry->etaPartition(kSimDetUnitId)->strip(simhit.localPosition()) + 1;

    if(detailPlot_) {
      me_detail_sim_occ_eta_[key3]->Fill(sim_global.eta());
      me_detail_sim_occ_phi_[key3]->Fill(sim_global.phi());
    }

    GEMRecHitCollection::range range = rechit_collection->get(sim_id);
    // GEMRecHitCollection::const_iterator;
    for(auto rechit = range.first; rechit != range.second; ++rechit) {
      Int_t cls = rechit->clusterSize();

      // matching conditions
      Bool_t matched;
      if ( cls == 1 ) {
        matched = sim_fired_strip == rechit->firstClusterStrip();
      } else {
        Int_t rec_first_fired_strip = rechit->firstClusterStrip();
        Int_t rec_last_fired_strip = rec_first_fired_strip + cls - 1;
        matched = (sim_fired_strip >= rec_first_fired_strip) and (sim_fired_strip <= rec_last_fired_strip);
      }

      if(matched) {
        LocalPoint rec_local = rechit->localPosition();
        GlobalPoint rec_global = kGEMGeometry->idToDet(kSimDetUnitId)->surface().toGlobal(rec_local);

        // XXX
        Float_t  resolution_x = std::sqrt(rechit->localPositionError().xx());
        Float_t  resolution_y = std::sqrt(rechit->localPositionError().yy());

        Float_t residual_x = rec_local.x() - sim_local.x();
        Float_t residual_y = rec_local.y() - sim_local.y(); 
        Float_t pull_x = residual_x / resolution_x;
        Float_t pull_y = residual_y / resolution_y;

        // DataFormats/GeometryVector/interface/extBasic3DVector.h
        // ::perp() Magnitude of transverse component 
        me_occ_zr_[region_id]->Fill(std::fabs(rec_global.z()), rec_global.perp());

        Int_t bin_x = getDetOccBinX(chamber_id, layer_id);
        me_occ_det_[key2]->Fill(bin_x, roll_id);

        me_cls_->Fill(cls);
        me_pull_x_[region_id]->Fill(pull_x);
        me_pull_y_[region_id]->Fill(pull_y);

        if(detailPlot_) {
          me_detail_cls_[key3]->Fill(cls);
          me_detail_pull_x_[key3]->Fill(pull_x);
          me_detail_pull_y_[key3]->Fill(pull_y);
          me_detail_occ_zr_[key3]->Fill(std::fabs(rec_global.z()), rec_global.perp());
          me_detail_occ_xy_[key3]->Fill(rec_global.x(), rec_global.y());
          me_detail_occ_polar_[key3]->Fill(rec_global.phi().phi(), rec_global.perp());

          // FIXME If we use global position of rechit,
          // 'inconsistent bin contents' exception may occur.
          me_detail_rec_occ_eta_[key3]->Fill(sim_global.eta());
          me_detail_rec_occ_phi_[key3]->Fill(sim_global.phi());
        }

        break;
      } // matched if block
    } // RecHits Loop End
  } // SimHit Loop End

}
