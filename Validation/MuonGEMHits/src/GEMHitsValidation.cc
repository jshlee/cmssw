#include "Validation/MuonGEMHits/interface/GEMHitsValidation.h"
#include "Validation/MuonGEMHits/interface/GEMValidationUtils.h"

#include "DataFormats/Common/interface/Handle.h"
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"

#include <exception>

using namespace std;

GEMHitsValidation::GEMHitsValidation(const edm::ParameterSet& ps) : GEMBaseValidation(ps) {
  edm::LogInfo(kLogCategory_) << "Call ctor\n";

  auto simhit_label = ps.getParameter<edm::InputTag>("simInputLabel");
  SimHitToken_ = consumes<edm::PSimHitContainer>(simhit_label);

  TOFRange_ = ps.getUntrackedParameter<std::vector<Double_t> >("TOFRange");
  detailPlot_ = ps.getParameter<Bool_t>("detailPlot");
  folder_ = ps.getParameter<std::string>("folder");

  edm::LogInfo(kLogCategory_) << "Exit ctor\n";
}


GEMHitsValidation::~GEMHitsValidation() {
  edm::LogInfo(kLogCategory_) << "Start dtor\n";
  edm::LogInfo(kLogCategory_) << "Finsih off dtor\n";
}


void GEMHitsValidation::bookHistograms(DQMStore::IBooker & ibooker,
                                       edm::Run const & Run,
                                       edm::EventSetup const & iSetup) {
  edm::LogInfo(kLogCategory_) << "Call bookHistograms\n";

  const GEMGeometry* kGEMGeom = initGeometry(iSetup);
  if ( kGEMGeom == nullptr) {
    edm::LogError(kLogCategory_) << "Cannot initialise GEMGeometry in the "
                                       << "bookHistograms step.\n";
    return;
  }

  ibooker.setCurrentFolder(folder_);

  LogDebug("MuonGEMHitsValidation") << "+++ Region independant part.\n";
  for(const auto & station : kGEMGeom->regions()[0]->stations()) {
    Int_t station_id = station->station();

    Double_t tof_min, tof_max;
    std::tie(tof_min, tof_max) = getTOFRange(station_id);

    // FIXME
    const char* name_tof  = TString::Format("simhit_tof_mu_st%d", station_id).Data();
    const char* title_tof = TString::Format("SimHit TOF (Muon only) : Station %d ; Time of flight [ns] ; entries", station_id).Data();
    me_tof_mu_[station_id] = ibooker.book1D(name_tof, title_tof, 40, tof_min, tof_max);

    const char* name_eloss  = TString::Format("simhit_eloss_mu_st%d", station_id).Data();
    const char* title_eloss = TString::Format("SimHit Energy Loss (Muon only) : Station %d ; Energy loss [eV] ; entries", station_id).Data();
    me_eloss_mu_[station_id] = ibooker.book1D(name_eloss, title_eloss, 60, 0.0, 6000.0);
  } // STATION LOOP END


  LogDebug("MuonGEMHitsValidation") << "+++ Region+Station part.\n";
  // Regions, Region+station
  for(const auto & region : kGEMGeom->regions()) {
    Int_t region_id = region->region();

    if(auto tmp_me = bookZROccupancy(ibooker, region_id, "simhit", "SimHit")) {
      me_occ_zr_[region_id] = tmp_me;
    } else {
      // TODO LogError MEssage
      edm::LogError(kLogCategory_) << "failed to book\n";
    }

    for(const auto & station : region->stations()) {
      Int_t station_id = station->station();
      ME2IdsKey key(region_id, station_id);

      if(auto tmp_me = bookDetectorOccupancy(ibooker, key, station, "simhit", "SimHit")) {
        me_occ_det_[key] = tmp_me;
      } else {
        edm::LogError(kLogCategory_) << "Cannot book "
                                           << "GEMHits Detector Occ "
                                           << "Region " << region_id << " "
                                           << "Station " << station_id << " \n";
      }

    } // STATION LOOP END
  } // REGION LOOP END


  LogDebug("MuonGEMHitsValidation") << "+++ Begining Detail Plots\n";
  if(detailPlot_ ) {
    for(const auto & region : kGEMGeom->regions()) {
      Int_t region_id = region->region();

      for(const auto & station : region->stations()) {
        Int_t station_id = station->station();

        Double_t tof_min, tof_max;
        std::tie(tof_min, tof_max) = getTOFRange(station_id);

        for(Int_t layer_id : {1, 2}) {

          ME3IdsKey key(region_id, station_id, layer_id);

          me_detail_occ_zr_[key] = bookZROccupancy(ibooker, key, "simhit", "SimHit");
          me_detail_occ_xy_[key] = bookXYOccupancy(ibooker, key, "simhit", "SimHit");

          me_detail_tof_[key] = bookHist1D(
              ibooker, key,
              "tof", "SimHit TOF",
              40, tof_min, tof_max,
              "Time of Flight [ns]", "entries");

          me_detail_tof_mu_[key] = bookHist1D(
              ibooker, key,
              "tof_muon", "SimHit TOF (Muon only)",
              40, tof_min, tof_max,
              "Time of Flight [ns]", "entries");

          me_detail_eloss_[key] = bookHist1D(
              ibooker, key,
              "eloss", "SimHit Energy Loss",
              60, 0.0, 6000.0,
              "Energy los [eV]", "entries");

          me_detail_eloss_mu_[key] = bookHist1D(
              ibooker, key,
              "eloss_muon", "SimHit Energy Loss (Muon Only)",
              60, 0., 6000.0,
              "Energy loss [eV]", "entries");

        } // LAYER LOOP END

      } // STATION LOOP END
    } // REGION LOOP END
  } // detailPlot IF END

  me_gem_geom_xyz_ = ibooker.book3D(
      "gem_geom_xyz",
      "GEM Roll Position;x [cm];y [cm];z [cm]",
      160, -800.0, 800.0,
      160, -800.0, 800.0, 
      240, -1200.0, 1200.0);
      

  me_gem_geom_eta_phi_ = ibooker.book2D(
      "gem_geom_eta_phi",
      "GEM Roll Position; #eta; #phi",
      101, -4, 4,
      101, -TMath::Pi(), TMath::Pi());

  // XXX This histogram does not need to be filled repeatedly.
  const LocalPoint kLocalOrigin(0.0, 0.0, 0.0);
  for(const auto & det_id : kGEMGeom->detUnitIds()) {
    GEMDetId gem_id(det_id);

    const GEMEtaPartition* kEtaPartition = kGEMGeom->etaPartition(gem_id);
    GlobalPoint gp = kEtaPartition->toGlobal(kLocalOrigin);

    me_gem_geom_xyz_->Fill(gp.x(), gp.y(), gp.z());
    me_gem_geom_eta_phi_->Fill(gp.eta(), gp.phi());
  }

  edm::LogInfo(kLogCategory_) << "Exit bookHistograms.\n";
}


std::tuple<Double_t, Double_t> GEMHitsValidation::getTOFRange(Int_t station_id) {
  edm::LogInfo(kLogCategory_) << "Call GEMHitsValidation::getTOFRange." << std::endl;
  unsigned start_index = station_id == 1 ? 0 : 2;
  Double_t tof_min = TOFRange_[start_index];
  Double_t tof_max = TOFRange_[start_index + 1];
  edm::LogInfo(kLogCategory_) << "Exit getTOFRange." << std::endl;
  return std::make_tuple(tof_min, tof_max);
}


void GEMHitsValidation::analyze(const edm::Event& e,
                                const edm::EventSetup& iSetup) {
  edm::LogInfo(kLogCategory_) << "Call GEMHitsValidation::analyze." << std::endl;

  const GEMGeometry* kGEMGeom = initGeometry(iSetup) ;
  if(kGEMGeom == nullptr) {
    edm::LogError(kLogCategory_) << "Failed to init GEMGeometry." << std::endl; 
    return ;
  }

  edm::Handle<edm::PSimHitContainer> simhit_container;
  e.getByToken(SimHitToken_, simhit_container);
  if (not simhit_container.isValid()) {
    edm::LogError(kLogCategory_) << "Cannot get GEMHits by Token simInputTagToken";
    return ;
  }

  for(const auto & simhit : *simhit_container.product()) {
    const GEMDetId gem_id(simhit.detUnitId());
    if ( kGEMGeom->idToDet(gem_id) == nullptr) {
      edm::LogInfo(kLogCategory_) << "simHit did not matched with GEMGeometry.\n";
      continue;
    }

    Int_t region_id  = gem_id.region();
    Int_t station_id =  gem_id.station();
    Int_t layer_id   = gem_id.layer();
    Int_t chamber_id =  gem_id.chamber();
    Int_t roll_id    =  gem_id.roll(); // eta partition

    ME2IdsKey key2(region_id, station_id);
    ME3IdsKey key3(region_id, station_id, layer_id);

    const LocalPoint kSimHitLocal(simhit.localPosition());
    const GlobalPoint kSimHitGlobal(kGEMGeom->idToDet(simhit.detUnitId())->surface().toGlobal(kSimHitLocal));

    Float_t g_r = kSimHitGlobal.perp();
    Float_t g_x = kSimHitGlobal.x();
    Float_t g_y = kSimHitGlobal.y();
    Float_t g_abs_z = std::fabs(kSimHitGlobal.z());

    Float_t energy_loss = kEnergyCF_ * simhit.energyLoss();
    Float_t tof = simhit.timeOfFlight();

    me_occ_zr_[region_id]->Fill(g_abs_z, g_r);

    Int_t bin_x = getDetOccBinX(chamber_id, layer_id);

    // if(me_occ_det_.find(key2) == me_occ_det_.end()) {
    //  edm::LogError(kLogCategory_) << "Cannot find key";
    me_occ_det_[key2]->Fill(bin_x, roll_id);

    if (std::abs(simhit.particleType()) == kMuonPDGId_) {
      me_tof_mu_[station_id]->Fill(tof);
      me_eloss_mu_[station_id]->Fill(energy_loss);
    }

    if( detailPlot_ ) {

      // First, fill variable has no condition.
      me_detail_occ_zr_[key3]->Fill(g_abs_z, g_r);
      me_detail_occ_xy_[key3]->Fill(g_x, g_y);

      me_detail_tof_[key3]->Fill(tof);
      me_detail_eloss_[key3]->Fill(energy_loss);

      if (std::abs(simhit.particleType()) == kMuonPDGId_) {
        me_detail_tof_mu_[key3]->Fill(tof);
        me_detail_eloss_mu_[key3]->Fill(energy_loss);
      }

    } // detailPlot

  } // simhit loop END
}

