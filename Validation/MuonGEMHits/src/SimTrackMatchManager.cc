#include "Validation/MuonGEMHits/interface/SimTrackMatchManager.h"

SimTrackMatchManager::SimTrackMatchManager(const SimTrack& t, const SimVertex& v,
      const edm::ParameterSet& ps, const edm::Event& ev, const edm::EventSetup& es)
: simhits_(t, v, ps, ev, es)
<<<<<<< HEAD
, gem_digis_(simhits_)
=======
>>>>>>> gem-sw/gem-csc-trigger-development
{}

SimTrackMatchManager::~SimTrackMatchManager() {}
