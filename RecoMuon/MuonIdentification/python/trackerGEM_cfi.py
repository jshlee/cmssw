import FWCore.ParameterSet.Config as cms


trackerGEM = cms.EDProducer("trackerGEM",
    maxPullX = cms.double (3.0),
    maxDiffX = cms.double (4.0),
    maxPullY = cms.double (10.0),
    maxDiffY = cms.double (10.0),
    maxDiffPhiDirection = cms.double (3.14),
    gemSegmentsToken = cms.InputTag("gemSegments"),
    generalTracksToken = cms.InputTag("generalTracks")
)
