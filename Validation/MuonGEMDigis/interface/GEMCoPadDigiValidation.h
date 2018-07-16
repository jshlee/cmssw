#ifndef VALIDATION_MUONGEMDIGIS_GEMCOPADDIGIVALIDATION_H_
#define VALIDATION_MUONGEMDIGIS_GEMCOPADDIGIVALIDATION_H_

#include "Validation/MuonGEMHits/interface/GEMBaseValidation.h"
#include "Validation/MuonGEMHits/interface/GEMValidationUtils.h"
#include "DataFormats/GEMDigi/interface/GEMCoPadDigiCollection.h"


class GEMCoPadDigiValidation : public GEMBaseValidation
{
public:
  explicit GEMCoPadDigiValidation(const edm::ParameterSet& );
  ~GEMCoPadDigiValidation() override;
  void analyze(const edm::Event& e, const edm::EventSetup&) override;
  void bookHistograms(DQMStore::IBooker &, edm::Run const &, edm::EventSetup const &) override;

private:
  // Simple plots
  MEMap1Key me_occ_zr_; // key = region_id
  MEMap2Key me_occ_det_; // key(region_id, station_id)

  // DetailPlots
  // key(region_id, station_id)
  MEMap2Key me_detail_occ_xy_;
  MEMap2Key me_detail_occ_zr_;
  // key(region_id, station_id)
  MEMap2Key me_detail_occ_phi_pad_;
  MEMap2Key me_detail_occ_pad_;
  MEMap2Key me_detail_bx_;

  // ParameterSet
  int minBXGEM_, maxBXGEM_;
  edm::EDGetToken InputTagToken_;
  int nBinXY_;
  bool detailPlot_;
  std::string folder_;

  // Custom Constants
  std::string kLogCategory_ = "GEMCoPadDigiValidation";
};

#endif // VALIDATION_MUONGEMDIGIS_GEMCOPADDIGIVALIDATION_H_
