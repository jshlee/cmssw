#include "Validation/MuonGEMHits/interface/GEMValidationUtils.h"
#include "TString.h"


TString GEMUtils::getSuffixName(int region_id) {
  auto suffix = TString::Format("_re%d", region_id);
  suffix = suffix.ReplaceAll("-", "_");
  return suffix;
}


TString GEMUtils::getSuffixName(int region_id, int station_id) {
  auto suffix = TString::Format("_re%d_st%d", region_id, station_id);
  suffix = suffix.ReplaceAll("-", "_");
  return suffix;
}

TString GEMUtils::getSuffixName(int region_id, int station_id, int layer_id) {
  auto suffix =  TString::Format("_re%d_st%d_la%d", region_id, station_id, layer_id);
  suffix = suffix.ReplaceAll("-", "_");
  return suffix;
}


TString GEMUtils::getSuffixName(int region_id,
                                int station_id,
                                int layer_id,
                                bool is_odd_chamber) {
  const char* chamber_parity = is_odd_chamber ? "odd" : "even";
  auto suffix =  TString::Format("_re%d_st%d_la%d_%s",
                                 region_id, station_id, layer_id, chamber_parity);
  suffix = suffix.ReplaceAll("-", "_");
  return suffix;
}


TString GEMUtils::getSuffixName(const ME2IdsKey & key) {
  int region_id, station_id;
  std::tie(region_id, station_id) = key;                           
  auto suffix = GEMUtils::getSuffixName(region_id, station_id);
  return suffix;
}


TString GEMUtils::getSuffixName(const ME3IdsKey & key) {
  int region_id, station_id, layer_id;
  std::tie(region_id, station_id, layer_id) = key;
  auto suffix = GEMUtils::getSuffixName(region_id, station_id, layer_id);
  return suffix;

}


TString GEMUtils::getSuffixTitle(int region_id) {
  return TString::Format(" Region %d", region_id);
}


TString GEMUtils::getSuffixTitle(int region_id, int station_id) {
  return TString::Format(" Region %d Station %d",
                         region_id, station_id);
}

TString GEMUtils::getSuffixTitle(int region_id, int station_id, int layer_id) {
  return TString::Format(" Region %d Station %d Layer %d",
                         region_id, station_id, layer_id);
}

TString GEMUtils::getSuffixTitle(int region_id,
                                 int station_id,
                                 int layer_id,
                                 bool is_odd_chamber) {
  const char* chamber_parity = is_odd_chamber ? "Odd" : "Even";
  return TString::Format(" Region %d Station %d Layer %d %s Chambers",
                           region_id, station_id, layer_id, chamber_parity);
}



TString GEMUtils::getSuffixTitle(const ME2IdsKey & key) {
  int region_id, station_id;
  std::tie(region_id, station_id) = key;                           
  TString suffix = getSuffixTitle(region_id, station_id);
  return suffix;
}


TString GEMUtils::getSuffixTitle(const ME3IdsKey & key) {
  int region_id, station_id, layer_id;
  std::tie(region_id, station_id, layer_id) = key;
  TString suffix = getSuffixTitle(region_id, station_id, layer_id);
  return suffix;
}
