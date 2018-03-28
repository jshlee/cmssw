# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step3 --processName RERECO --conditions auto:phase2_realistic --era Phase2C2_timing --eventcontent FEVTDEBUGHLT --runUnscheduled -s RECO --datatier GEN-SIM-RECO --geometry Extended2023D4 -n -1 --filein /store/relval/CMSSW_9_0_0_pre4/RelValZMM_14/GEN-SIM-RECO/90X_upgrade2023_realistic_v3_2023D4Timing-v1/10000/04013655-A9EC-E611-9783-0CC47A4C8E98.root --fileout step3.root
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('RERECO',eras.Phase2)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.Geometry.GeometryExtended2023D21Reco_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('/store/user/jlee/TenMuExtendedE_0_200_CMSSW_10_1_0_pre3/RECO/reco_978.root'),
    secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(
    allowUnscheduled = cms.untracked.bool(True)
)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('step3 nevts:-1'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition
process.FEVTDEBUGHLToutput = cms.OutputModule("PoolOutputModule",
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('GEN-SIM-RECO'),
        filterName = cms.untracked.string('')
    ),
    eventAutoFlushCompressedSize = cms.untracked.int32(10485760),
    fileName = cms.untracked.string('step3.root'),
    outputCommands = process.FEVTDEBUGHLTEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)
process.FEVTDEBUGHLTEventContent.outputCommands.append('keep *_me0Track_*_*')
process.FEVTDEBUGHLTEventContent.outputCommands.append('keep *_me0Muon_*_*')

# Additional output definition
from RecoMuon.GlobalTrackingTools.GlobalMuonRefitter_cff import *
from RecoMuon.GlobalTrackingTools.GlobalMuonTrackMatcher_cff import *
from RecoMuon.TrackingTools.MuonServiceProxy_cff import *
from RecoMuon.TrackingTools.MuonTrackLoader_cff import *

process.me0Track = cms.EDProducer("ME0TrackProducer",
MuonServiceProxy,
RefitterParameters = cms.PSet(GlobalMuonRefitter),
TrackLoaderParameters = MuonTrackLoaderForGLB.TrackLoaderParameters,
trackLabel = cms.InputTag('generalTracks'),
muonLabel = cms.InputTag('muons'),
)
process.me0Muon = cms.EDProducer("ME0MuonProducer",
me0TrackLabel = cms.InputTag('me0Track'),
muonLabel = cms.InputTag('muons'),
)

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase2_realistic', '')

# Path and EndPath definitions
process.reconstruction_step = cms.Path(process.me0Track+process.me0Muon)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.FEVTDEBUGHLToutput_step = cms.EndPath(process.FEVTDEBUGHLToutput)

# Schedule definition
process.schedule = cms.Schedule(process.reconstruction_step,process.endjob_step,process.FEVTDEBUGHLToutput_step)

#do not add changes to your config after this point (unless you know what you are doing)
from FWCore.ParameterSet.Utilities import convertToUnscheduled
process=convertToUnscheduled(process)
from FWCore.ParameterSet.Utilities import cleanUnscheduled
process=cleanUnscheduled(process)


# Customisation from command line

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion
