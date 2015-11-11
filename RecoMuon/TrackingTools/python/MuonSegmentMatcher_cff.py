import FWCore.ParameterSet.Config as cms

MuonSegmentMatcher = cms.PSet(
    MatchParameters = cms.PSet(
        DTsegments = cms.InputTag("dt4DSegments"),
        DTradius = cms.double(0.01),
        CSCsegments = cms.InputTag("cscSegments"),
        GEMsegments = cms.InputTag("gemSegments"),
        RPChits = cms.InputTag("rpcRecHits"),
        TightMatchDT = cms.bool(False),
        TightMatchCSC = cms.bool(True),
        TightMatchGEM = cms.bool(False)
    )
)
