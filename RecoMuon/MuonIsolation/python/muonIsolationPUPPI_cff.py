import FWCore.ParameterSet.Config as cms

IsoConeDefinitions = cms.VPSet(
        cms.PSet( isolationAlgo = cms.string('MuonPFIsolationWithConeVeto'),
                  coneSize = cms.double(0.3),
                  VetoThreshold = cms.double(0.0),
                  VetoConeSize = cms.double(0.0001),# VetoConeSize is deltaR^2
                  isolateAgainst = cms.string('h+'),
                  miniAODVertexCodes = cms.vuint32(2,3) ),
        cms.PSet( isolationAlgo = cms.string('MuonPFIsolationWithConeVeto'),
                  coneSize = cms.double(0.3),
                  VetoThreshold = cms.double(0.0),
                  VetoConeSize = cms.double(0.01),# VetoConeSize is deltaR^2
                  isolateAgainst = cms.string('h0'),
                  miniAODVertexCodes = cms.vuint32(2,3) ),
        cms.PSet( isolationAlgo = cms.string('MuonPFIsolationWithConeVeto'),
                  coneSize = cms.double(0.3),
                  VetoThreshold = cms.double(0.0),
                  VetoConeSize = cms.double(0.01),# VetoConeSize is deltaR^2
                  isolateAgainst = cms.string('gamma'),
                  miniAODVertexCodes = cms.vuint32(2,3) ),                  
)

IsoTrackSelections = cms.PSet(
    Diff_z = cms.double(0.2),
    BeamSpotLabel = cms.InputTag("offlineBeamSpot"),
    DR_Max = cms.double(0.5),
    Diff_r = cms.double(0.1),
    Chi2Prob_Min = cms.double(-1.0),
    NHits_Min = cms.uint32(0),
    Chi2Ndof_Max = cms.double(1e+64),
    Pt_Min = cms.double(-1.0),
    BeamlineOption = cms.string('BeamSpotFromEvent')
)

muonIsolationAODPUPPI = cms.EDProducer( "CITKPFIsolationSumProducerForPUPPI",
                srcToIsolate = cms.InputTag("muons"),
                srcForIsolationCone = cms.InputTag(''),
                isolationConeDefinitions = IsoConeDefinitions,
                isolationTrackSelections = IsoTrackSelections,
                usePUPPI = cms.bool(True)
)

muonIsolationMiniAODPUPPI = cms.EDProducer( "CITKPFIsolationSumProducerForPUPPI",
                srcToIsolate = cms.InputTag("slimmedMuons"),
                srcForIsolationCone = cms.InputTag('packedPFCandidates'),
                puppiValueMap = cms.InputTag(''),
                isolationConeDefinitions = IsoConeDefinitions,
                isolationTrackSelections = IsoTrackSelections,
                usePUPPI = cms.bool(True)
)

muonIsolationMiniAODPUPPINoLeptons = cms.EDProducer( "CITKPFIsolationSumProducerForPUPPI",
                srcToIsolate = cms.InputTag("slimmedMuons"),
                srcForIsolationCone = cms.InputTag('packedPFCandidates'),
                puppiValueMap = cms.InputTag(''),
                usePUPPINoLepton = cms.bool(True),
                isolationConeDefinitions = IsoConeDefinitions,
                isolationTrackSelections = IsoTrackSelections,
                usePUPPI = cms.bool(True)
)
