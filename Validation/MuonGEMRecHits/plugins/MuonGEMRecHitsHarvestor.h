#ifndef VALIDATION_MUONGEMRECHITS_PLUGINS_MUONGEMRECHITSHARVESTOR_H_
#define VALIDATION_MUONGEMRECHITS_PLUGINS_MUONGEMRECHITSHARVESTOR_H_

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/DQMEDHarvester.h"
#include "Geometry/GEMGeometry/interface/GEMGeometry.h"

#include "TProfile.h"

class MuonGEMRecHitsHarvestor : public DQMEDHarvester
{
public:
  /// constructor
  explicit MuonGEMRecHitsHarvestor(const edm::ParameterSet&);
  /// destructor
  ~MuonGEMRecHitsHarvestor() override;

  void dqmEndJob(DQMStore::IBooker &, DQMStore::IGetter &) override;
  void ProcessBooking( DQMStore::IBooker& , DQMStore::IGetter&, const char* label, TString suffix, TH1F* track_hist, TH1F* sh_hist );
  TProfile* computeEff(const TH1F & passed, const TH1F & total);

private:
  std::string dbe_path_;
  std::vector<int> region_ids_, station_ids_, layer_ids_;


  // Coonst
  const std::string kLogCategory_ = "MuonGEMRecHitsHarvestor";
  const std::vector<const char*> kAxes_ = {"eta", "phi"};
};

#endif // VALIDATION_MUONGEMRECHITS_PLUGINS_MUONGEMRECHITSHARVESTOR_H_
