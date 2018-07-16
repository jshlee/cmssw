#include "Validation/MuonGEMDigis/interface/GEMPadDigiValidation.h"
#include "Validation/MuonGEMHits/interface/GEMValidationUtils.h"
#include <TMath.h>

GEMPadDigiValidation::GEMPadDigiValidation(const edm::ParameterSet& ps): GEMBaseValidation(ps) {

  auto input_label = ps.getParameter<edm::InputTag>("PadLabel");
  InputTagToken_ = consumes<GEMPadDigiCollection>(input_label);
  detailPlot_ = ps.getParameter<Bool_t>("detailPlot");
  folder_ = ps.getParameter<std::string>("folder");
}


void GEMPadDigiValidation::bookHistograms(DQMStore::IBooker & ibooker,
                                          edm::Run const & Run,
                                          edm::EventSetup const & iSetup) {
    
  const GEMGeometry* kGEMGeom = initGeometry(iSetup);
  if ( kGEMGeom == nullptr) {
    edm::LogError(kLogCategory_) << "Failed to initialise GEMGeometry in 'bookHistograms' step\n";
    return ;
  }

  ibooker.setCurrentFolder(folder_);
  LogDebug("GEMPadDigiValidation")<<"ibooker set current folder\n";

  // FIXME
  Int_t npadsGE11 = kGEMGeom->regions()[0]->stations()[0]->superChambers()[0]->chambers()[0]->etaPartitions()[0]->npads();
  Int_t npadsGE21 = 0;
  if (kGEMGeom->regions()[0]->stations().size() > 1 and not kGEMGeom->regions()[0]->stations()[1]->superChambers().empty()) {
    npadsGE21 = kGEMGeom->regions()[0]->stations()[1]->superChambers()[0]->chambers()[0]->etaPartitions()[0]->npads();
  }


  for(const auto & region : kGEMGeom->regions()) {
    Int_t region_id = region->region();

    if(auto tmp_me = bookZROccupancy(ibooker, region_id, "pad_digi", "Pad DIGI")) {
      me_occ_zr_[region_id] = tmp_me;
    } else {
      // TODO
      edm::LogError(kLogCategory_) << "Failed to book\n";
    }

    for(const auto & station : region->stations()) {
      Int_t station_id = station->station();
      ME2IdsKey key(region_id, station_id);
      if(auto tmp_me = bookDetectorOccupancy(ibooker, key, station, "pad_digi", "Pad DIGI")) {
        me_occ_det_[key] = tmp_me;
      } else {
        // TODO
        edm::LogError(kLogCategory_) << "Failed to book\n";
      }

    } // STATION LOOP END
  } // REGION LOOP END


  if (detailPlot_) {
    for( auto& region : kGEMGeom->regions() ) {
      Int_t region_id = region->region();

      for( auto& station : region->stations() ) {
        Int_t station_id = station->station();

        Int_t nPads = station_id == 1 ? npadsGE11 : npadsGE21;

        for(Int_t layer_id : {1, 2}) {

          ME3IdsKey key3(region_id, station_id, layer_id);

          me_detail_occ_zr_[key3] = bookZROccupancy(ibooker, key3, "pad_dg", "Pad Digi");
          me_detail_occ_xy_[key3] = bookXYOccupancy(ibooker, key3, "pad_dg", "Pad Digi");

          me_detail_occ_phi_pad_[key3] = bookHist2D(ibooker, key3,
                                                    "pad_digi_occ_phi_pad",
                                                    "Pad DIGI Occupancy",
                                                    280, -TMath::Pi(), TMath::Pi(),
                                                    nPads/2, 0, nPads,
                                                    "#phi [rad]", "Pad number");

          me_detail_occ_pad_[key3] = bookHist1D(ibooker, key3,
                                                "pad_digi_occ_pad",
                                                "Pad DIGI Occupancy",
                                                nPads, 0.5, nPads + 0.5,
                                                "Pad Number");

          me_detail_bx_[key3] = bookHist1D(ibooker, key3,
                                           "pad_digi_bx",
                                           "Bunch Crossing",
                                           11, -5.5, 5.5,
                                           "Bunch Crossing");

        } // layer loop end
      } // station loop end
    } // region loop END
  } // detailPlot END
}


GEMPadDigiValidation::~GEMPadDigiValidation() { }


void GEMPadDigiValidation::analyze(const edm::Event& e,
                                   const edm::EventSetup& iSetup) {
  // FIXME unify
  const GEMGeometry* kGEMGeom = initGeometry(iSetup);
  if(kGEMGeom == nullptr) {
    edm::LogError(kLogCategory_) << "Failed to initialise kGEMGeom\n";
    return ;
  }

  // typedef MuonDigiCollection<GEMDetId, GEMPadDigi> GEMPadDigiCollection;
  edm::Handle<GEMPadDigiCollection> gem_pad_digis;
  e.getByToken(InputTagToken_, gem_pad_digis);
  if (not gem_pad_digis.isValid()) {
    edm::LogError(kLogCategory_) << "Cannot get pads by label GEMPadToken.";
    return;
  }

  // GEMPadDigiCollection::DigiRangeIterator;
  for (auto range_iter = gem_pad_digis->begin(); range_iter != gem_pad_digis->end(); range_iter++) {
    GEMDetId id = (*range_iter).first;
    const GEMPadDigiCollection::Range& range = (*range_iter).second;


    const GeomDet* geom_det = kGEMGeom->idToDet(id);

    if ( geom_det == nullptr) { 
      std::cout << "Getting DetId failed. Discard this gem pad hit. "
                << "Maybe it comes from unmatched geometry."<<std::endl;
      continue; 
    }

    const BoundPlane & surface = geom_det->surface();
    const GEMEtaPartition * roll = kGEMGeom->etaPartition(id);

    Int_t region_id  = id.region();
    Int_t station_id = id.station();
    Int_t layer_id   = id.layer();
    Int_t chamber_id = id.chamber();
    Int_t roll_id    = id.roll();

    ME2IdsKey key2(region_id, station_id);
    ME3IdsKey key3(region_id, station_id, layer_id);

    for(auto digi = range.first; digi != range.second; ++digi) {
      Int_t pad = digi->pad();
      Int_t bx  = digi->bx();

      LocalPoint lp = roll->centreOfPad(digi->pad());
      GlobalPoint gp = surface.toGlobal(lp);

      Float_t g_r   = gp.perp();
      Float_t g_phi = gp.phi();
      Float_t g_x   = gp.x();
      Float_t g_y   = gp.y();
      Float_t g_z   = gp.z();

      // Simple Plots
      me_occ_zr_[region_id]->Fill(std::fabs(g_z), g_r);

      Int_t bin_x = getDetOccBinX(chamber_id, layer_id);
      me_occ_det_[key2]->Fill(bin_x, roll_id); 

      if ( detailPlot_) {
        me_detail_occ_xy_[key3]->Fill(g_x,g_y);
        me_detail_occ_phi_pad_[key3]->Fill(g_phi,pad);
        me_detail_occ_pad_[key3]->Fill(pad);
        me_detail_bx_[key3]->Fill(bx);
        me_detail_occ_zr_[key3]->Fill(std::fabs(g_z), g_r);
      } // detailPlot_

    }
  }
}
