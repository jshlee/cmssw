# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step3 --conditions auto:phase2_realistic -n -1 --era Phase2C2 --eventcontent DQM --runUnscheduled -s VALIDATION:@baseValidation+@muonOnlyValidation --datatier DQMIO --geometry Extended2023D17 --filein file:step3.root --fileout file:step31.root --python Val_2023D17_2.py --no_exec
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('VALIDATION',eras.Phase2C2)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.Geometry.GeometryExtended2023D17Reco_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.Validation_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
'/store/relval/CMSSW_9_1_1/RelValTenMuExtendedE_0_200/GEN-SIM-RECO/91X_upgrade2023_realistic_v1_D17-v1/10000/06DB069F-233F-E711-BB95-0025905A607A.root',
'/store/relval/CMSSW_9_1_1/RelValTenMuExtendedE_0_200/GEN-SIM-RECO/91X_upgrade2023_realistic_v1_D17-v1/10000/10F25F3C-253F-E711-9ECD-0CC47A7C35B2.root',
'/store/relval/CMSSW_9_1_1/RelValTenMuExtendedE_0_200/GEN-SIM-RECO/91X_upgrade2023_realistic_v1_D17-v1/10000/1E787C33-233F-E711-998E-0CC47A7C35B2.root',
'/store/relval/CMSSW_9_1_1/RelValTenMuExtendedE_0_200/GEN-SIM-RECO/91X_upgrade2023_realistic_v1_D17-v1/10000/32E2E793-243F-E711-9769-0CC47A4D75EC.root',
'/store/relval/CMSSW_9_1_1/RelValTenMuExtendedE_0_200/GEN-SIM-RECO/91X_upgrade2023_realistic_v1_D17-v1/10000/46B5BC11-223F-E711-9D0B-0CC47A4D76D0.root',
'/store/relval/CMSSW_9_1_1/RelValTenMuExtendedE_0_200/GEN-SIM-RECO/91X_upgrade2023_realistic_v1_D17-v1/10000/66972C1F-253F-E711-81A1-0025905A60EE.root',
'/store/relval/CMSSW_9_1_1/RelValTenMuExtendedE_0_200/GEN-SIM-RECO/91X_upgrade2023_realistic_v1_D17-v1/10000/8E912784-253F-E711-8064-0CC47A4C8E1E.root',
'/store/relval/CMSSW_9_1_1/RelValTenMuExtendedE_0_200/GEN-SIM-RECO/91X_upgrade2023_realistic_v1_D17-v1/10000/9CE30B0B-233F-E711-863D-0025905A6132.root',
'/store/relval/CMSSW_9_1_1/RelValTenMuExtendedE_0_200/GEN-SIM-RECO/91X_upgrade2023_realistic_v1_D17-v1/10000/9EA8EAE7-223F-E711-8F3E-0025905B859A.root',
'/store/relval/CMSSW_9_1_1/RelValTenMuExtendedE_0_200/GEN-SIM-RECO/91X_upgrade2023_realistic_v1_D17-v1/10000/A03B45A2-233F-E711-8E59-0025905A60EE.root',
'/store/relval/CMSSW_9_1_1/RelValTenMuExtendedE_0_200/GEN-SIM-RECO/91X_upgrade2023_realistic_v1_D17-v1/10000/A0A626E7-233F-E711-8287-0CC47A7C3458.root',
'/store/relval/CMSSW_9_1_1/RelValTenMuExtendedE_0_200/GEN-SIM-RECO/91X_upgrade2023_realistic_v1_D17-v1/10000/AEABD696-243F-E711-8BD5-0CC47A745250.root',
'/store/relval/CMSSW_9_1_1/RelValTenMuExtendedE_0_200/GEN-SIM-RECO/91X_upgrade2023_realistic_v1_D17-v1/10000/BE1074B8-223F-E711-ABF3-0CC47A4D7674.root',
'/store/relval/CMSSW_9_1_1/RelValTenMuExtendedE_0_200/GEN-SIM-RECO/91X_upgrade2023_realistic_v1_D17-v1/10000/DE717AB8-223F-E711-8FF1-0CC47A4D75EE.root',
'/store/relval/CMSSW_9_1_1/RelValTenMuExtendedE_0_200/GEN-SIM-RECO/91X_upgrade2023_realistic_v1_D17-v1/10000/EC6FC7E3-253F-E711-BC4C-0CC47A4C8E70.root'),
    secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('step3 nevts:-1'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.DQMoutput = cms.OutputModule("DQMRootOutputModule",
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('DQMIO'),
        filterName = cms.untracked.string('')
    ),
    fileName = cms.untracked.string('file:step31.root'),
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
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase2_realistic', '')

# Path and EndPath definitions
process.prevalidation_step = cms.Path(process.baseCommonPreValidation)
process.prevalidation_step1 = cms.Path(process.globalPrevalidationMuons)
process.validation_step = cms.EndPath(process.baseCommonValidation)
process.validation_step1 = cms.EndPath(process.globalValidationMuons)
process.DQMoutput_step = cms.EndPath(process.DQMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.prevalidation_step,process.prevalidation_step1,process.validation_step,process.validation_step1,process.DQMoutput_step)
from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)

# customisation of the process.

# Automatic addition of the customisation function from SimGeneral.MixingModule.fullMixCustomize_cff
from SimGeneral.MixingModule.fullMixCustomize_cff import setCrossingFrameOn 

#call to customisation function setCrossingFrameOn imported from SimGeneral.MixingModule.fullMixCustomize_cff
process = setCrossingFrameOn(process)

# End of customisation functions
#do not add changes to your config after this point (unless you know what you are doing)
from FWCore.ParameterSet.Utilities import convertToUnscheduled
process=convertToUnscheduled(process)


# Customisation from command line

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion
