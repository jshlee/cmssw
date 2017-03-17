# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step31 --conditions auto:run2_mc -n -1 --era Run2_2016 --eventcontent FEVTDEBUGHLT,DQM --runUnscheduled -s VALIDATION:recoMuonValidation --datatier GEN-SIM-RECO,DQMIO --python Val_Run2.py --no_exec --filein file:step3.root --fileout file:step31.root
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('VALIDATION',eras.Run2_2016)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.Validation_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('/store/relval/CMSSW_8_1_0_pre11/RelValZMM_13/GEN-SIM-RECO/PU25ns_81X_mcRun2_asymptotic_Candidate_2016_08_30_11_31_55_resub-v1/00000/1C0CD8A4-0F77-E611-91B0-0CC47A4D7650.root',),
    secondaryFileNames = cms.untracked.vstring(
    '/store/relval/CMSSW_8_1_0_pre11/RelValZMM_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_81X_mcRun2_asymptotic_Candidate_2016_08_30_11_31_55_resub-v1/00000/0C4278D0-0977-E611-9DA4-0CC47A4D760C.root',
'/store/relval/CMSSW_8_1_0_pre11/RelValZMM_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_81X_mcRun2_asymptotic_Candidate_2016_08_30_11_31_55_resub-v1/00000/206F573C-0A77-E611-9D72-0CC47A74527A.root',
'/store/relval/CMSSW_8_1_0_pre11/RelValZMM_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_81X_mcRun2_asymptotic_Candidate_2016_08_30_11_31_55_resub-v1/00000/20E4F641-0977-E611-9045-0025905A60A6.root',
'/store/relval/CMSSW_8_1_0_pre11/RelValZMM_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_81X_mcRun2_asymptotic_Candidate_2016_08_30_11_31_55_resub-v1/00000/283E9BD1-0977-E611-B222-0CC47A78A468.root',
'/store/relval/CMSSW_8_1_0_pre11/RelValZMM_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_81X_mcRun2_asymptotic_Candidate_2016_08_30_11_31_55_resub-v1/00000/5A515DCF-0977-E611-8821-0CC47A4D7678.root',
'/store/relval/CMSSW_8_1_0_pre11/RelValZMM_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_81X_mcRun2_asymptotic_Candidate_2016_08_30_11_31_55_resub-v1/00000/766B4245-0A77-E611-965E-0025905B858E.root',
'/store/relval/CMSSW_8_1_0_pre11/RelValZMM_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_81X_mcRun2_asymptotic_Candidate_2016_08_30_11_31_55_resub-v1/00000/8AFC9ACF-0977-E611-9CBD-0CC47A4D7678.root',
'/store/relval/CMSSW_8_1_0_pre11/RelValZMM_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_81X_mcRun2_asymptotic_Candidate_2016_08_30_11_31_55_resub-v1/00000/8C19E240-0A77-E611-BFF7-0CC47A4C8E66.root',
'/store/relval/CMSSW_8_1_0_pre11/RelValZMM_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_81X_mcRun2_asymptotic_Candidate_2016_08_30_11_31_55_resub-v1/00000/923AB13C-0977-E611-84D3-0CC47A4C8F12.root',
'/store/relval/CMSSW_8_1_0_pre11/RelValZMM_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_81X_mcRun2_asymptotic_Candidate_2016_08_30_11_31_55_resub-v1/00000/B8BDDC3E-0A77-E611-946A-0CC47A7C3444.root',
'/store/relval/CMSSW_8_1_0_pre11/RelValZMM_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_81X_mcRun2_asymptotic_Candidate_2016_08_30_11_31_55_resub-v1/00000/BE9D6ED6-0977-E611-977B-0025905B8564.root',
'/store/relval/CMSSW_8_1_0_pre11/RelValZMM_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_81X_mcRun2_asymptotic_Candidate_2016_08_30_11_31_55_resub-v1/00000/C49E383D-0977-E611-919D-0CC47A4D7640.root',
'/store/relval/CMSSW_8_1_0_pre11/RelValZMM_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_81X_mcRun2_asymptotic_Candidate_2016_08_30_11_31_55_resub-v1/00000/C4DD4447-0977-E611-9BAD-0CC47A4D7690.root',
'/store/relval/CMSSW_8_1_0_pre11/RelValZMM_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_81X_mcRun2_asymptotic_Candidate_2016_08_30_11_31_55_resub-v1/00000/C8B3573C-0977-E611-A889-0CC47A7C3444.root',
'/store/relval/CMSSW_8_1_0_pre11/RelValZMM_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_81X_mcRun2_asymptotic_Candidate_2016_08_30_11_31_55_resub-v1/00000/F8CAB8C6-0877-E611-BEF0-0CC47A4C8ECE.root'
                            )
)

process.options = cms.untracked.PSet(
    allowUnscheduled = cms.untracked.bool(True)
)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('step31 nevts:-1'),
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
    fileName = cms.untracked.string('file:step31.root'),
    outputCommands = process.FEVTDEBUGHLTEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

process.DQMoutput = cms.OutputModule("DQMRootOutputModule",
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('DQMIO'),
        filterName = cms.untracked.string('')
    ),
    fileName = cms.untracked.string('file:step31_inDQM.root'),
    outputCommands = process.DQMEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

# Additional output definition

# Other statements
process.mix.playback = True
process.mix.digitizers = cms.PSet()
for a in process.aliases: delattr(process, a)
process.RandomNumberGeneratorService.restoreStateLabel=cms.untracked.string("randomEngineStateProducer")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')

# Path and EndPath definitions
process.validation_step = cms.EndPath(process.recoMuonValidation)
process.FEVTDEBUGHLToutput_step = cms.EndPath(process.FEVTDEBUGHLToutput)
process.DQMoutput_step = cms.EndPath(process.DQMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.validation_step,process.FEVTDEBUGHLToutput_step,process.DQMoutput_step)

# customisation of the process.

# Automatic addition of the customisation function from SimGeneral.MixingModule.fullMixCustomize_cff
from SimGeneral.MixingModule.fullMixCustomize_cff import setCrossingFrameOn 

#call to customisation function setCrossingFrameOn imported from SimGeneral.MixingModule.fullMixCustomize_cff
process = setCrossingFrameOn(process)

# End of customisation functions
#do not add changes to your config after this point (unless you know what you are doing)
from FWCore.ParameterSet.Utilities import convertToUnscheduled
process=convertToUnscheduled(process)
from FWCore.ParameterSet.Utilities import cleanUnscheduled
process=cleanUnscheduled(process)


# Customisation from command line
