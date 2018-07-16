#include "Validation/MuonGEMHits/interface/GEMBaseValidation.h"
#include "Validation/MuonGEMHits/interface/GEMValidationUtils.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
//#include "DataFormats/GEMDigi/interface/GEMDigiCollection.h"
#include "Geometry/GEMGeometry/interface/GEMEtaPartitionSpecs.h"

#include <memory>

#include "TMath.h"

using namespace std;


GEMBaseValidation::GEMBaseValidation(const edm::ParameterSet& ps) {
  nBinZR_ = ps.getUntrackedParameter< std::vector<Double_t> >("nBinGlobalZR") ;
  RangeZR_ = ps.getUntrackedParameter< std::vector<Double_t> >("RangeGlobalZR");
  nBinXY_ = ps.getUntrackedParameter<Int_t>("nBinGlobalXY", 360) ;
}


const GEMGeometry* GEMBaseValidation::initGeometry(edm::EventSetup const & iSetup) {
  const GEMGeometry* kGEMGeometry = nullptr;

  try {
    edm::ESHandle<GEMGeometry> hGeom;
    iSetup.get<MuonGeometryRecord>().get(hGeom);
    kGEMGeometry = &*hGeom;
  }
  catch( edm::eventsetup::NoProxyException<GEMGeometry>& e) {
    edm::LogError("MuonGEMBaseValidation") << "+++ Error : GEM geometry is unavailable on event loop. +++\n";
    return nullptr;
  }
  return kGEMGeometry;
}


GEMBaseValidation::~GEMBaseValidation() { }


MonitorElement* GEMBaseValidation::bookZROccupancy(DQMStore::IBooker& ibooker,
                                                   Int_t region_id,
                                                   const char* name_prefix,
                                                   const char* title_prefix) {
  const char* name_suffix = GEMUtils::getSuffixName(region_id).Data();
  const char* title_suffix = GEMUtils::getSuffixTitle(region_id).Data();

  TString name = TString::Format("%s_occ_zr%s", name_prefix, name_suffix);
  TString title = TString::Format("%s ZR Occupancy%s;|Z| [cm];R [cm]",
                                  title_prefix, title_suffix);


  Double_t station1_xmin = RangeZR_[0];
  Double_t station1_xmax = RangeZR_[1];
  Double_t station2_xmin = RangeZR_[4];
  Double_t station2_xmax = RangeZR_[5];

  std::vector<Double_t> xbins_vector;
  for(Double_t i = station1_xmin - 1 ; i< station2_xmax + 1; i += 0.25  ) {
    if ( i > station1_xmax + 1 and i < station2_xmin - 1 ) {
      continue; 
    }
    xbins_vector.push_back(i);
  }

  Int_t nbinsx = xbins_vector.size() - 1;
  // auto xbins = static_cast<Double_t*>(&xbins_vector[0]);

  Int_t nbinsy = nBinZR_[2]; 
  Double_t ylow = std::min({RangeZR_[2], RangeZR_[6]});
  Double_t yup = std::max({RangeZR_[3], RangeZR_[7]});

  auto hist = new TH2F(name, title, nbinsx, &xbins_vector[0], nbinsy, ylow, yup);
  return ibooker.book2D(name, hist);
}



