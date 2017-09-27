#include "FWCore/PluginManager/interface/ModuleDef.h"

#include "FWCore/Framework/interface/MakerMacros.h"

#include "RecoMuon/GlobalMuonProducer/src/GlobalMuonProducer.h"
#include "RecoMuon/GlobalMuonProducer/src/TevMuonProducer.h"
#include "RecoMuon/GlobalMuonProducer/src/ME0MuonProducer.h"
#include "RecoMuon/GlobalMuonProducer/src/ME0TrackProducer.h"


DEFINE_FWK_MODULE(GlobalMuonProducer);
DEFINE_FWK_MODULE(TevMuonProducer);
DEFINE_FWK_MODULE(ME0MuonProducer);
DEFINE_FWK_MODULE(ME0TrackProducer);

