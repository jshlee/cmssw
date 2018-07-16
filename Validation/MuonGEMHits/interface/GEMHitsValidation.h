#ifndef VALIDATION_MUONGEMHITS_INTERFACE_GEMHITSVALIDATION_H_
#define VALIDATION_MUONGEMHITS_INTERFACE_GEMHITSVALIDATION_H_

#include "Validation/MuonGEMHits/interface/GEMBaseValidation.h"
#include "Validation/MuonGEMHits/interface/GEMValidationUtils.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"


class GEMHitsValidation : public GEMBaseValidation
{
public:
  explicit GEMHitsValidation( const edm::ParameterSet& );
  ~GEMHitsValidation() override;
  void bookHistograms(DQMStore::IBooker &,
                      edm::Run const &,
                      edm::EventSetup const &) override;
  void analyze(const edm::Event& e, const edm::EventSetup&) override;

private:
  std::tuple<Double_t, Double_t> getTOFRange(Int_t station_id);

  // Simple plots
  MEMap1Key me_occ_zr_; // [regions]
  MEMap1Key me_tof_mu_; // [stations]
  MEMap1Key me_eloss_mu_; // [stations]

  MEMap2Key me_occ_det_; // [r][s]

  // Detail plots
  // [regions][stations][layers]
  MEMap3Key me_detail_occ_xy_;
  MEMap3Key me_detail_occ_zr_;

  MEMap3Key me_detail_tof_;
  MEMap3Key me_detail_tof_mu_;
  MEMap3Key me_detail_eloss_;
  MEMap3Key me_detail_eloss_mu_;

  MonitorElement* me_gem_geom_xyz_;
  MonitorElement* me_gem_geom_eta_phi_;

  // temporary constant
  // energy loss conversion factor
  const Float_t kEnergyCF_ = 1.e9;
  const std::string kLogCategory_ = "GEMHitsValidation";

  // TODO
  edm::EDGetToken SimHitToken_;
  Bool_t detailPlot_;
  std::vector<Double_t> TOFRange_;  
  std::string folder_;
};

#endif // VALIDATION_MUONGEMHITS_INTERFACE_GEMHITSVALIDATION_H_
