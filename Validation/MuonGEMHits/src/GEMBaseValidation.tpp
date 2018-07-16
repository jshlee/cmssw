#include "Validation/MuonGEMHits/interface/GEMValidationUtils.h"
#include "TMath.h"

template <typename MEMapKey>
MonitorElement* GEMBaseValidation::bookZROccupancy(DQMStore::IBooker& ibooker,
                                                   const MEMapKey & key,
                                                   const char* name_prefix,
                                                   const char* title_prefix) {
  // TODO Logging
  const unsigned long key_size = std::tuple_size<MEMapKey>::value;
  if(key_size < 2) {
    MonitorElement* me = nullptr;
    return me;
  }

  Int_t station_id = std::get<1>(key);

  const char* name_suffix = GEMUtils::getSuffixName(key).Data();
  const char* title_suffix = GEMUtils::getSuffixTitle(key).Data();

  TString name = TString::Format("%s_occ_zr%s", name_prefix, name_suffix);
  TString title = TString::Format("%s ZR Occupancy%s; |Z| [cm] ; R [cm]",
                                  title_prefix, title_suffix);

  // st1, st2 of xbin, st1, st2 of ybin
  int nbinsx = nBinZR_[station_id - 1];
  int nbinsy = nBinZR_[station_id + 1]; 

  // st1 xmin xmax, ymin, ymax | st2 xmin, xmax, ymin ymax
  // station1 --> 0 1 
  // start index
  unsigned i = station_id == 1 ? 0 : 4;
  // z
  double xlow = RangeZR_[i];
  double xup = RangeZR_[i+1];
  // r
  double ylow = RangeZR_[i+2];
  double yup = RangeZR_[i+3];

  return ibooker.book2D(name, title, nbinsx, xlow, xup, nbinsy, ylow, yup);
}


template <typename MEMapKey>
MonitorElement* GEMBaseValidation::bookXYOccupancy(DQMStore::IBooker& ibooker,
                                                   const MEMapKey & key,
                                                   const char* name_prefix,
                                                   const char* title_prefix) {
  const char* name_suffix  = GEMUtils::getSuffixName(key).Data();
  const char* title_suffix = GEMUtils::getSuffixTitle(key).Data();
  TString name = TString::Format("%s_occ_xy%s", name_prefix, name_suffix);
  TString title = TString::Format("%s XY Occupancy%s;X [cm];Y [cm]",
                                  title_prefix, title_suffix);
  return ibooker.book2D(name, title, nBinXY_, -360, 360, nBinXY_, -360, 360); 
}


template <typename MEMapKey>
MonitorElement* GEMBaseValidation::bookPolarOccupancy(DQMStore::IBooker& ibooker,
                                                      const MEMapKey & key,
                                                      const char* name_prefix,
                                                      const char* title_prefix) {
  const char* name_suffix  = GEMUtils::getSuffixName(key).Data();
  const char* title_suffix = GEMUtils::getSuffixTitle(key).Data();
  TString name = TString::Format("%s_occ_polar%s", name_prefix, name_suffix);
  TString title = TString::Format("%s Polar Occupancy%s", title_prefix, title_suffix);
  // TODO # of bins
  MonitorElement* me = ibooker.book2D(name, title,
                                      101, -TMath::Pi(), TMath::Pi(),
                                      101, 0.0, 2160); 
  return me;
}


template <typename MEMapKey>
MonitorElement* GEMBaseValidation::bookDetectorOccupancy(DQMStore::IBooker& ibooker,
                                                         const MEMapKey & key,
                                                         const GEMStation* station,
                                                         const char* name_prefix,
                                                         const char* title_prefix) {

  const char* name_suffix = GEMUtils::getSuffixName(key).Data();
  const char* title_suffix = GEMUtils::getSuffixTitle(key).Data();

  TString name = TString::Format("%s_occ_det%s", name_prefix, name_suffix); 
  TString title = TString::Format("%s Occupancy for detector component%s;;#eta-partition",
                                  title_prefix, title_suffix);

  // int nXbins = station->rings().front()->nSuperChambers()*2; //Fine for the complete geometry, but not for the Slice Test geometry
  // FIXME
  int nXbins = 72; //Maximum number of chambers is for GE1/1
  int nYbins = station->rings().front()->superChambers().front()->chambers().front()->nEtaPartitions();

  auto hist = new TH2F(title, name, nXbins, 0, nXbins, nYbins, 1, nYbins+1);

  int bin = 0 ;
  // for(unsigned sCh = 1; sCh <= station->superChambers().size(); sCh++){
  // FIXME
  for(unsigned sCh = 1; sCh <= 36; sCh++) {
    for(unsigned Ch = 1; Ch <= 2; Ch++) {
      bin++;
      TString label = TString::Format("C%dL%d", sCh, Ch);
      hist->GetXaxis()->SetBinLabel(bin, label.Data());
    }
  }

  MonitorElement* me = ibooker.book2D(name, hist);
  return me;
}  


template <typename MEMapKey>
MonitorElement* GEMBaseValidation::bookHist1D(DQMStore::IBooker& ibooker,
                                              const MEMapKey & key,
                                              const char* name, const char* title,
                                              int nbinsx, double xlow, double xup,
                                              const char* x_title,
                                              const char* y_title) {
  const char* name_suffix = GEMUtils::getSuffixName(key);
  const char* title_suffix = GEMUtils::getSuffixTitle(key);
  
  TString hist_name  = TString::Format("%s%s", name, name_suffix);
  TString hist_title = TString::Format("%s :%s;%s;%s",
                                       title, title_suffix, x_title, y_title);

  return ibooker.book1D(hist_name, hist_title, nbinsx, xlow, xup);
}
    

template <typename MEMapKey>
MonitorElement* GEMBaseValidation::bookHist2D(DQMStore::IBooker& ibooker,
                                              const MEMapKey & key,
                                              const char* name, const char* title,
                                              int nbinsx, double xlow, double xup,
                                              int nbinsy, double ylow, double yup,
                                              const char* x_title,
                                              const char* y_title) {
  const char* name_suffix = GEMUtils::getSuffixName(key);
  const char* title_suffix = GEMUtils::getSuffixTitle(key);
  
  TString hist_name  = TString::Format("%s%s", name, name_suffix);
  TString hist_title = TString::Format("%s :%s;%s;%s",
                                       title, title_suffix, x_title, y_title);

  return ibooker.book2D(hist_name, hist_title, nbinsx, xlow, xup, nbinsy, ylow, yup);
}
