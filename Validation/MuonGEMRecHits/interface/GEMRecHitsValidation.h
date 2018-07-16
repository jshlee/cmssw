#ifndef VALIDATION_MUONGEMRECHITS_INTERFACE_GEMRECHITSVALIDATION_H_
#define VALIDATION_MUONGEMRECHITS_INTERFACE_GEMRECHITSVALIDATION_H_

#include "Validation/MuonGEMHits/interface/GEMBaseValidation.h"
#include "Validation/MuonGEMHits/interface/GEMValidationUtils.h"

#include <DataFormats/GEMRecHit/interface/GEMRecHit.h>
#include <DataFormats/GEMRecHit/interface/GEMRecHitCollection.h>

class GEMRecHitsValidation : public GEMBaseValidation
{
public:
  explicit GEMRecHitsValidation( const edm::ParameterSet& );
  ~GEMRecHitsValidation() override;
  void bookHistograms(DQMStore::IBooker &, edm::Run const &, edm::EventSetup const &) override;
  void analyze(const edm::Event& e, const edm::EventSetup&) override;

private:
  //Simple Plots
  MEMap1Key me_occ_zr_; // [# of region]
  MEMap2Key me_occ_det_; // [# of region][# of station]
  MonitorElement* me_cls_;
  MEMap1Key me_pull_x_; // key: region
  MEMap1Key me_pull_y_; // key: region

  //Detaile Plots
  // key = (region_id, station_id, layer_id)
  MEMap3Key me_detail_occ_xy_;
  MEMap3Key me_detail_occ_zr_;
  MEMap3Key me_detail_occ_polar_;
  MEMap3Key me_detail_cls_;
  MEMap3Key me_detail_pull_x_;
  MEMap3Key me_detail_pull_y_;
  // moved from GEMRecHitTrackMatch
  MEMap3Key me_detail_sim_occ_eta_;
  MEMap3Key me_detail_rec_occ_eta_; // matched rechit
  MEMap3Key me_detail_sim_occ_phi_;
  MEMap3Key me_detail_rec_occ_phi_;


  edm::EDGetToken sim_hit_token_, rec_hit_token_;
  Int_t nBinXY_;
  Bool_t detailPlot_;
  std::string folder_;

  // Constatns
  std::string kLogCategory_ = "GEMRecHitsValidation";
};

#endif // VALIDATION_MUONGEMRECHITS_INTERFACE_GEMRECHITSVALIDATION_H_
