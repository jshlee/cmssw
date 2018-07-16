#ifndef VALIDATION_MUONGEMDIGIS_INTERFACE_GEMPADDIGIVALIDATION_H_
#define VALIDATION_MUONGEMDIGIS_INTERFACE_GEMPADDIGIVALIDATION_H_

#include "Validation/MuonGEMHits/interface/GEMBaseValidation.h"
#include "Validation/MuonGEMHits/interface/GEMValidationUtils.h"
#include "DataFormats/GEMDigi/interface/GEMPadDigiCollection.h"

//#include "DataFormats/Common/interface/Handle.h"
class GEMPadDigiValidation : public GEMBaseValidation {
public:
  explicit GEMPadDigiValidation( const edm::ParameterSet& );
  ~GEMPadDigiValidation() override;
  void analyze(const edm::Event& e, const edm::EventSetup&) override;
  void bookHistograms(DQMStore::IBooker &, edm::Run const &, edm::EventSetup const &) override;

private:
  // Simple plots
  MEMap2Key me_occ_det_; // key = make_tuple(region_id, station_id);
  MEMap1Key me_occ_zr_; // key = region_id

  // Detail plots
  // key(region_id, station_id, layer_id);
  MEMap3Key me_detail_occ_zr_;
  MEMap3Key me_detail_occ_xy_; 
  MEMap3Key me_detail_occ_phi_pad_;
  MEMap3Key me_detail_occ_pad_; // DIGI Occupancy per Pad number
  MEMap3Key me_detail_bx_;

  // ParameterSet
  edm::EDGetToken InputTagToken_;
  int nBinXY_;
  bool detailPlot_;
  std::string folder_;

  // Constants
  std::string kLogCategory_ = "GEMPadDigiValidation";
};

#endif // VALIDATION_MUONGEMDIGIS_GEMPADDIGIVALIDATION_H_
