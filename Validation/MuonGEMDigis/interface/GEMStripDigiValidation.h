#ifndef VALIDATION_MUONGEMDIGIS_GEMSTRIPDIGIVALIDATION_H_
#define VALIDATION_MUONGEMDIGIS_GEMSTRIPDIGIVALIDATION_H_

#include "Validation/MuonGEMHits/interface/GEMBaseValidation.h"
#include "Validation/MuonGEMHits/interface/GEMValidationUtils.h"

//#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/GEMDigi/interface/GEMDigiCollection.h"

class GEMStripDigiValidation : public GEMBaseValidation
{
public:
  explicit GEMStripDigiValidation(const edm::ParameterSet&);
  ~GEMStripDigiValidation() override;
  void analyze(const edm::Event& e, const edm::EventSetup&) override;
  void bookHistograms(DQMStore::IBooker &, edm::Run const &, edm::EventSetup const &) override;

private:
  // Simple plots
  MEMap1Key me_occ_zr_; // key = region_id
  MEMap2Key me_occ_det_; // key = make_tuple(region_id, station_id);

  // Detail plots
  // key = make_tuple(region_id, station_id, layer_id);
  MEMap3Key me_detail_occ_zr_;
  MEMap3Key me_detail_occ_xy_;
  MEMap3Key me_detail_occ_phi_strip_;
  MEMap3Key me_detail_occ_strip_;
  MEMap3Key me_detail_bx_;

  // ParameterSet
  edm::EDGetToken InputTagToken_;
  int nBinXY_;
  bool detailPlot_;
  int nStripsGE11_;
  int nStripsGE21_;
  std::string folder_;

  // Constants
  const char* kMENamePrefix_ = "strip_digi";
  std::string kLogCategory = "GEMStripDigiValidation";
};

#endif // VALIDATION_MUONGEMDIGIS_GEMSTRIPDIGIVALIDATION_H_
