import FWCore.ParameterSet.Config as cms

gemRecHits = cms.EDProducer("GEMRecHitProducer",
    recAlgoConfig = cms.PSet(

    ),
    recAlgo = cms.string('GEMRecHitStandardAlgo'),
    gemDigiLabel = cms.InputTag("simMuonGEMDigis"),
    maskSource = cms.string('File'),
    maskvecfile = cms.FileInPath('RecoLocalMuon/GEMRecHit/data/GEMMaskVec.dat'),
    deadSource = cms.string('File'),
    deadvecfile = cms.FileInPath('RecoLocalMuon/GEMRecHit/data/GEMDeadVec.dat'),
    badConnector = cms.bool(False),
    deadStripFraction = cms.double(0.0),
)

RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
  gemRecHits = cms.PSet(
    initialSeed = cms.untracked.uint32(123),
    engineName = cms.untracked.string('TRandom3')
  )
)
