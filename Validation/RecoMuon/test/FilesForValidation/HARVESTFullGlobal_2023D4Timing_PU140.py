# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step4 --conditions auto:phase2_realistic -s HARVESTING:@phase2Validation+@phase2 --era Phase2C2_timing --filein file:step31_inDQM_inDQM.root --scenario pp --filetype DQM --geometry Extended2023D4 --mc -n -1 --fileout file:step4.root --python HARVESTFullGlobal_2023D4Timing_PU200.py --no_exec
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('HARVESTING',eras.Phase2C2_timing)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.Geometry.GeometryExtended2023D4Reco_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.DQMSaverAtRunEnd_cff')
process.load('Configuration.StandardSequences.Harvesting_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# Input source
process.source = cms.Source("DQMRootSource",
    fileNames = cms.untracked.vstring(
                            
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_1.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_10.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_11.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_12.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_13.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_14.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_15.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_16.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_17.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_18.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_19.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_2.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_20.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_21.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_22.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_23.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_24.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_25.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_27.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_28.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_29.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_3.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_30.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_31.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_32.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_33.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_34.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_35.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_36.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_37.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_38.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_39.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_4.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_40.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_41.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_42.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_43.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_44.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_45.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_46.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_47.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_48.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_49.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_5.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_50.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_51.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_52.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_53.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_54.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_55.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_56.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_57.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_58.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_59.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_6.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_60.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_61.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_62.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_63.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_64.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_65.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_66.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_67.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_68.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_69.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_7.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_70.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_71.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_72.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_73.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_74.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_75.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_76.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_77.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_78.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_79.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_8.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_80.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_81.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_82.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_83.root',
'/store/user/calabria/RelValZMM_14/crab_Val_900_pre4_PU140_D4_1/170216_113710/0000/step31_inDQM_9.root'
                            
                            )
)

process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound'),
    fileMode = cms.untracked.string('FULLMERGE')
)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('step4 nevts:-1'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase2_realistic', '')

process.dqmSaver.workflow = "/Global/CMSSW_9_0_0_pre4/RECO_PU140_D4Timing_ZMM"

# Path and EndPath definitions
process.dqmHarvestingPOGMC = cms.Path(process.DQMOffline_SecondStep_PrePOGMC)
process.validationHarvesting = cms.Path(process.postValidation+process.hltpostvalidation+process.postValidation_gen)
process.validationprodHarvesting = cms.Path(process.hltpostvalidation_prod+process.postValidation_gen)
process.dqmHarvestingFakeHLT = cms.Path(process.DQMOffline_SecondStep_FakeHLT+process.DQMOffline_Certification)
process.validationHarvestingMiniAOD = cms.Path(process.JetPostProcessor+process.METPostProcessorHarvesting+process.postValidationMiniAOD)
process.validationHarvestingHI = cms.Path(process.postValidationHI)
process.genHarvesting = cms.Path(process.postValidation_gen)
process.dqmHarvestingPOG = cms.Path(process.DQMOffline_SecondStep_PrePOG)
process.alcaHarvesting = cms.Path()
process.dqmHarvesting = cms.Path(process.DQMOffline_SecondStep+process.DQMOffline_Certification)
process.validationHarvestingFS = cms.Path(process.postValidation+process.hltpostvalidation+process.postValidation_gen)
process.validationpreprodHarvesting = cms.Path(process.postValidation_preprod+process.hltpostvalidation_preprod+process.postValidation_gen)
process.postValidation_common_step = cms.Path(process.postValidation_common)
process.postValidation_muons_step = cms.Path(process.postValidation_muons)
process.dqmsave_step = cms.Path(process.DQMSaver)

# Schedule definition
process.schedule = cms.Schedule(process.postValidation_common_step,process.postValidation_muons_step,process.dqmsave_step)


# Customisation from command line

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion
