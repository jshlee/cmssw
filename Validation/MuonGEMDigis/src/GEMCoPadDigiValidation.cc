#include "Validation/MuonGEMDigis/interface/GEMCoPadDigiValidation.h"
#include "Validation/MuonGEMHits/interface/GEMValidationUtils.h"
#include <TMath.h>


GEMCoPadDigiValidation::GEMCoPadDigiValidation(const edm::ParameterSet& param_set): GEMBaseValidation(param_set) {
  auto input_label = param_set.getParameter<edm::InputTag>("CopadLabel");
  InputTagToken_ = consumes<GEMCoPadDigiCollection>(input_label);
  detailPlot_ = param_set.getParameter<Bool_t>("detailPlot");
  minBXGEM_ = param_set.getParameter<Int_t>("minBXGEM"); 
  maxBXGEM_ = param_set.getParameter<Int_t>("maxBXGEM"); 
  folder_ = param_set.getParameter<std::string>("folder");
}


void GEMCoPadDigiValidation::bookHistograms(DQMStore::IBooker & ibooker,
                                            edm::Run const & Run,
                                            edm::EventSetup const & iSetup) {

  const GEMGeometry* GEMGeometry_ = initGeometry(iSetup);
  if ( GEMGeometry_ == nullptr) {
    LogDebug("GEMCoPadDigiValidation") << "failed\n";
    return ;
  }

  ibooker.setCurrentFolder(folder_);

  // FIXME
  Int_t npadsGE11 = GEMGeometry_->regions()[0]->stations()[0]->superChambers()[0]->chambers()[0]->etaPartitions()[0]->npads();
  Int_t npadsGE21 = 0;
  if (GEMGeometry_->regions()[0]->stations().size() > 1 and not (GEMGeometry_->regions()[0]->stations()[1]->superChambers().empty())) {
    npadsGE21  = GEMGeometry_->regions()[0]->stations()[1]->superChambers()[0]->chambers()[0]->etaPartitions()[0]->npads();
  }

  for(const auto & region : GEMGeometry_->regions()) {
    Int_t region_id = region->region();

    if(auto tmp_zr = bookZROccupancy(ibooker, region_id, "copad", "CoPad") ) {
      me_occ_zr_[region_id] = tmp_zr;
    } else {
      edm::LogError(kLogCategory_) << "Failed to create simpleZR histograms" << std::endl;
    }

    for(const auto & station : region->stations()) {
      Int_t station_id = station->station();
      ME2IdsKey key(region_id, station_id);
      if(auto tmp_det = bookDetectorOccupancy(ibooker, key, station, "copad", "CoPad")) {
        me_occ_det_[key] = tmp_det;
      } else {
        edm::LogError(kLogCategory_) << "Failed to create DCEta histograms" << std::endl;
      }
    } // station loop end
  } // region loop end


  if ( detailPlot_) {
    for(const auto &  region : GEMGeometry_->regions()) {
      Int_t region_id = region->region();

      for(const auto & station : region->stations() ) {
        Int_t station_id = station->station();

        ME2IdsKey key2(region_id, station_id);
          
        Int_t num_pads = station_id == 1 ? npadsGE11 : npadsGE21;

        me_detail_occ_zr_[key2] = bookZROccupancy(ibooker, key2, "copad_dg","CoPad Digi");
        me_detail_occ_xy_[key2] = bookXYOccupancy(ibooker, key2, "copad_dg","CoPad Digi");

        me_detail_occ_phi_pad_[key2] = bookHist2D(ibooker, key2,
                                                  "copad_digi_occ_phi_pad",
                                                  "CoPad DIGI Occupancy",
                                                  280, -TMath::Pi(), TMath::Pi(),
                                                  num_pads/2, 0, num_pads,
                                                  "#phi [rad]", "Pad number");

        me_detail_occ_pad_[key2] = bookHist1D(ibooker, key2,
                                              "copad_digi_occ_pad",
                                              "CoPad DIGI Ocupancy per pad number",
                                              num_pads, 0.5, num_pads+0.5,
                                              "Pad number");

        me_detail_bx_[key2] = bookHist1D(ibooker, key2,
                                         "copad_digi_bx",
                                         "CoPad DIGI Bunch Crossing",
                                         11, -5.5, 5.5,
                                         "bunch crossing");

      } // station for loop
    } // region for loop
  } // detailPlot if statement
}


GEMCoPadDigiValidation::~GEMCoPadDigiValidation() {


}


void GEMCoPadDigiValidation::analyze(const edm::Event& e,
                                     const edm::EventSetup& iSetup) {
  const GEMGeometry* GEMGeometry_ = initGeometry(iSetup);
  if ( GEMGeometry_ == nullptr) {
    LogDebug("GEMCoPadDigiValidation") << "failed\n";
    return ;
  }

  edm::Handle<GEMCoPadDigiCollection> gem_copad_digis;
  e.getByToken(InputTagToken_, gem_copad_digis);
  if (not gem_copad_digis.isValid()) {
    edm::LogError(kLogCategory_) << "Cannot get pads by token.";
    return ;
  }

  // GEMCoPadDigiCollection::DigiRangeIterator
  for (auto range_iter = gem_copad_digis->begin(); range_iter != gem_copad_digis->end(); range_iter++) {
    GEMDetId id = (*range_iter).first;
    const GEMCoPadDigiCollection::Range& range = (*range_iter).second;

    Int_t region_id  = id.region();
    Int_t station_id = id.station();
    Int_t layer_id   = id.layer();
    Int_t chamber_id = id.chamber();

    ME2IdsKey key2(region_id, station_id);

    //loop over digis of given roll
    // GEMCoPadDigiCollection::const_iterator digi;
    for (auto digi = range.first; digi != range.second; ++digi) {
      // GEM copads are stored per super chamber!
      GEMDetId schId = GEMDetId(region_id, id.ring(), station_id, 0, chamber_id, 0);

      Int_t roll_id = (*digi).roll();

      const GeomDet* geom_det = GEMGeometry_->idToDet(schId);
      if ( geom_det == nullptr) {
        edm::LogError(kLogCategory_)<<schId<<" : This detId cannot be loaded from GEMGeometry // Original"<<id<<" station : "<<station_id;
        edm::LogError(kLogCategory_)<<"Getting DetId failed. Discard this gem copad hit. ";
        continue; 
      }

      const BoundPlane & surface = geom_det->surface();
      const GEMSuperChamber * superChamber = GEMGeometry_->superChamber(schId);

      Short_t pad1 = (Short_t) digi->pad(1);
      Short_t pad2 = (Short_t) digi->pad(2);
      Short_t bx1  = (Short_t) digi->bx(1);
      Short_t bx2  = (Short_t) digi->bx(2);

      LogDebug("GEMCoPadDigiValidation")<<" copad #1 pad : "<<pad1<<"  bx : "<<bx1;
      LogDebug("GEMCoPadDigiValidation")<<" copad #2 pad : "<<pad2<<"  bx : "<<bx2;

      // Filtered using BX
      if ( bx1 < (Short_t) minBXGEM_ or bx1 > (Short_t) maxBXGEM_) continue;
      if ( bx2 < (Short_t) minBXGEM_ or bx2 > (Short_t) maxBXGEM_) continue;

      //  const GEMChamber* chamber(nnt layer) const;
      LocalPoint lp1 = superChamber->chamber(1)->etaPartition(roll_id)->centreOfPad(pad1);
      LocalPoint lp2 = superChamber->chamber(2)->etaPartition(roll_id)->centreOfPad(pad2);

      GlobalPoint gp1 = surface.toGlobal(lp1);
      GlobalPoint gp2 = surface.toGlobal(lp2);

      Float_t g_r1 =  gp1.perp();
      Float_t g_r2 =  gp2.perp();

      Float_t g_z1 =  gp1.z();
      Float_t g_z2 =  gp2.z();

      Float_t g_phi =  gp1.phi();
      Float_t g_x =  gp1.x();
      Float_t g_y =  gp1.y();

      // Fill normal plots.
      me_occ_zr_[region_id]->Fill(std::fabs(g_z1), g_r1);
      me_occ_zr_[region_id]->Fill(std::fabs(g_z2), g_r2);

      Int_t binX = getDetOccBinX(chamber_id, layer_id);

      me_occ_det_[key2]->Fill(binX, roll_id);
      me_occ_det_[key2]->Fill(binX + 1, roll_id);

      // Fill detail plots.
      if ( detailPlot_) {

        me_detail_occ_xy_[key2]->Fill(g_x, g_y);     
        me_detail_occ_phi_pad_[key2]->Fill(g_phi, pad1);
        me_detail_occ_pad_[key2]->Fill(pad1);
        me_detail_bx_[key2]->Fill(bx1);
        me_detail_bx_[key2]->Fill(bx2);
        me_detail_occ_zr_[key2]->Fill(std::fabs(g_z1), g_r1);
        me_detail_occ_zr_[key2]->Fill(std::fabs(g_z2), g_r2);

      } // detailPlot_ 

    }
  }
}
