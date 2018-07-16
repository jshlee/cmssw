#ifndef VALIDATION_MUONGEMHITS_INTERFACE_GEMVALIDATIONUTILS_H_
#define VALIDATION_MUONGEMHITS_INTERFACE_GEMVALIDATIONUTILS_H_

#include "DQMServices/Core/interface/MonitorElement.h"

#include "TString.h"

#include <map>
#include <tuple>

typedef std::tuple<Int_t, Int_t>           ME2IdsKey;
typedef std::tuple<Int_t, Int_t, Int_t>      ME3IdsKey;
typedef std::tuple<Int_t, Int_t, Int_t, Int_t> ME4IdsKey;

typedef std::map<Int_t, MonitorElement*> MEMap1Key;
typedef std::map<ME2IdsKey, MonitorElement*> MEMap2Key;
typedef std::map<ME3IdsKey, MonitorElement*> MEMap3Key;
typedef std::map<ME4IdsKey, MonitorElement*> MEMap4Key;

namespace GEMUtils {
  TString getSuffixName(Int_t region_id);
  TString getSuffixName(Int_t region_id, Int_t station_id);
  TString getSuffixName(Int_t region_id, Int_t station_id, Int_t layer_id);
  TString getSuffixName(Int_t region_id, Int_t station_id, Int_t layer_id, Bool_t is_odd_chamber);

  TString getSuffixName(const ME2IdsKey & key);
  TString getSuffixName(const ME3IdsKey & key);

  TString getSuffixTitle(Int_t region_id);
  TString getSuffixTitle(Int_t region_id, Int_t station_id);
  TString getSuffixTitle(Int_t region_id, Int_t station_id, Int_t layer_id);
  TString getSuffixTitle(Int_t region_id, Int_t station_id, Int_t layer_id, Bool_t is_odd_chamber);

  TString getSuffixTitle(const ME2IdsKey & key);
  TString getSuffixTitle(const ME3IdsKey & key);
}


#endif // VALIDATION_MUONGEMHITS_GEMVALIDATIONUTILS_H_
