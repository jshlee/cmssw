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
'/store/relval/CMSSW_9_1_1_patch1/RelValTenMuDisplacedDxy_0_500/GEN-SIM-RECO/91X_upgrade2023_realistic_v3_D17noPU-v1/00000/1EB71429-D96B-E711-9F73-0025905A60E0.root',
'/store/relval/CMSSW_9_1_1_patch1/RelValTenMuDisplacedDxy_0_500/GEN-SIM-RECO/91X_upgrade2023_realistic_v3_D17noPU-v1/00000/485FAE45-D86B-E711-9D8F-0CC47A4D75F0.root',
'/store/relval/CMSSW_9_1_1_patch1/RelValTenMuDisplacedDxy_0_500/GEN-SIM-RECO/91X_upgrade2023_realistic_v3_D17noPU-v1/00000/8E5005E6-D16B-E711-BBCC-0CC47A4D75F2.root',
'/store/relval/CMSSW_9_1_1_patch1/RelValTenMuDisplacedDxy_0_500/GEN-SIM-RECO/91X_upgrade2023_realistic_v3_D17noPU-v1/00000/94AD6B65-D96B-E711-A7DB-0CC47A78A4B8.root',
'/store/relval/CMSSW_9_1_1_patch1/RelValTenMuDisplacedDxy_0_500/GEN-SIM-RECO/91X_upgrade2023_realistic_v3_D17noPU-v1/00000/E884A103-D56B-E711-A8D0-0025905B85F6.root',
                                      ),
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
