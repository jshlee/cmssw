#include "Validation/MuonGEMDigis/interface/GEMStripDigiValidation.h"
#include "Validation/MuonGEMHits/interface/GEMValidationUtils.h"

#include "DataFormats/GEMDigi/interface/GEMDigiCollection.h"

#include "Geometry/CommonTopologies/interface/StripTopology.h"

#include <TMath.h>
#include <iomanip>


GEMStripDigiValidation::GEMStripDigiValidation(const edm::ParameterSet& ps): GEMBaseValidation(ps) {
  auto input_label = ps.getParameter<edm::InputTag>("stripLabel");
  InputTagToken_ = consumes<GEMDigiCollection>(input_label);

  detailPlot_ = ps.getParameter<Bool_t>("detailPlot");

  nStripsGE11_ = ps.getUntrackedParameter<Int_t>("nStripsGE11");
  nStripsGE21_ = ps.getUntrackedParameter<Int_t>("nStripsGE21");

  folder_ = ps.getParameter<std::string>("folder");
}


void GEMStripDigiValidation::bookHistograms(DQMStore::IBooker & ibooker,
                                            edm::Run const & Run,
                                            edm::EventSetup const & iSetup ) {
    
  const GEMGeometry* kGEMGeom  = initGeometry(iSetup);
  if ( kGEMGeom == nullptr) {
    edm::LogError(kLogCategory) << "Failed to initialise kGEMGeom\n";
    return ;
  }

  ibooker.setCurrentFolder(folder_);

  for(const auto & region : kGEMGeom->regions()) {
    Int_t region_id = region->region();

    if(auto tmp_zr = bookZROccupancy(ibooker, region_id, "strip", "Strip")) {
      me_occ_zr_[region_id] = tmp_zr;
    } else {
      edm::LogError(kLogCategory) << "cannot book ";  // TODO
    }

    for(const auto & station : region->stations()) {
      Int_t station_id = station->station();
      ME2IdsKey key(region_id, station_id);

      if(auto tmp_me = bookDetectorOccupancy(ibooker, key, station, "strip", "Strip")) {
        me_occ_det_[key] = tmp_me;
      } else {
        edm::LogError(kLogCategory) << "cannot book ";  // TODO
      }

    } // station loop end
  } // region loop end

  // Booking detail plot.
  if(detailPlot_) {
    for(const auto & region : kGEMGeom->regions()) {
      Int_t region_id = region->region();

      for(const auto & station : region->stations()) {
        Int_t station_id = station->station();
        Int_t num_strips = station_id == 1 ? nStripsGE11_ : nStripsGE21_;

        for(Int_t layer_id : {1, 2}) {

          ME3IdsKey key3(region_id, station_id, layer_id);

          me_detail_occ_zr_[key3] = bookZROccupancy(ibooker, key3, "strip", "Strip Digi");
          me_detail_occ_xy_[key3] = bookXYOccupancy(ibooker, key3, "strip", "Strip Digi");

          me_detail_occ_phi_strip_[key3] = bookHist2D(ibooker, key3,
                                                      "strip_occ_phi_strip",
                                                      "Strip DIGI Occupancy",
                                                      280, -TMath::Pi(), TMath::Pi(),
                                                      num_strips / 2, 0, num_strips,
                                                      "#phi [rad]", "strip number");

          me_detail_occ_strip_[key3] = bookHist1D(ibooker, key3,
                                                  "strip_occ_strip",
                                                  "Strip DIGI Occupancy per strip number",
                                                  num_strips, 0.5, num_strips + 0.5,
                                                  "strip number");

          me_detail_bx_[key3] = bookHist1D(ibooker, key3,
                                           "strip_bx",
                                           "Strip DIGI Bunch Crossing",
                                           11, -5.5, 5.5,
                                           "bunch crossing");

        } // Layer Loop End
      } // Station Loop End
    } // Region Loop End
  } // detailPlot if End

  LogDebug("GEMStripDigiValidation")<<"Booking End.\n";
}


GEMStripDigiValidation::~GEMStripDigiValidation() {
}


void GEMStripDigiValidation::analyze(const edm::Event& e,
                                     const edm::EventSetup& iSetup) {

  const GEMGeometry* kGEMGeom = initGeometry(iSetup);
  if ( kGEMGeom == nullptr) {
    edm::LogError(kLogCategory) << "Failed to initialise kGEMGeom\n";
    return ;
  }

  edm::Handle<GEMDigiCollection> gem_digis;
  e.getByToken(InputTagToken_, gem_digis);
  if (not gem_digis.isValid()) {
    edm::LogError(kLogCategory) << "Cannot get strips by Token stripToken.\n";
    return ;
  }

  for (auto range_iter = gem_digis->begin(); range_iter != gem_digis->end(); range_iter++) {
    GEMDetId id = (*range_iter).first;
    const GEMDigiCollection::Range& range = (*range_iter).second;

    const GeomDet* geom_det = kGEMGeom->idToDet(id);
    if (geom_det == nullptr) { 
      std::cout << "Getting DetId failed. Discard this gem strip hit. "
                << "Maybe it comes from unmatched geometry."
                << std::endl;
      continue; 
    }

    const BoundPlane & surface = geom_det->surface();
    const GEMEtaPartition* roll = kGEMGeom->etaPartition(id);

    Int_t region_id  = id.region();
    Int_t layer_id   = id.layer();
    Int_t station_id = id.station();
    Int_t chamber_id = id.chamber();
    Int_t roll_id    = id.roll();

    // keys for MonitorElement* map.
    ME2IdsKey key2(region_id, station_id);
    ME3IdsKey key3(region_id, station_id, layer_id);

    for (auto digi = range.first; digi != range.second; ++digi) {
      Int_t strip = digi->strip();
      Int_t bx = digi->bx();

      LocalPoint digi_local = roll->centreOfStrip(digi->strip());
      GlobalPoint digi_global = surface.toGlobal(digi_local);

      Float_t g_r   = digi_global.perp();
      Float_t g_phi = digi_global.phi();
      Float_t g_x   = digi_global.x();
      Float_t g_y   = digi_global.y();
      Float_t g_z   = digi_global.z();

      // Simple Plots
      me_occ_zr_[region_id]->Fill(std::fabs(g_z), g_r);

      Int_t bin_x = getDetOccBinX(chamber_id, layer_id);
      me_occ_det_[key2]->Fill(bin_x, roll_id); 

      // Detail Plots
      if ( detailPlot_) {
        me_detail_occ_zr_[key3]->Fill(std::fabs(g_z), g_r);
        me_detail_occ_xy_[key3]->Fill(g_x, g_y);     
        me_detail_occ_phi_strip_[key3]->Fill(g_phi, strip);
        me_detail_occ_strip_[key3]->Fill(strip);
        me_detail_bx_[key3]->Fill(bx);

      } // detailPlot_ if end
    } // digi loop end
  }
}
