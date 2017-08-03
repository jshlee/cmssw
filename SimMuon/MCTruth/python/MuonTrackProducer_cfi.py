import FWCore.ParameterSet.Config as cms

muonTrackProducer = cms.EDProducer("MuonTrackProducer",
   muonsTag = cms.InputTag("muons"),
   vtxTag = cms.InputTag("selectedVertices"),
   inputDTRecSegment4DCollection = cms.InputTag("dt4DSegments"),
   inputCSCSegmentCollection = cms.InputTag("cscSegments"),
   selectionTags = cms.vstring('TrackerMuonArbitrated'),
   trackType = cms.string('innerTrackPlusSegments')
)
