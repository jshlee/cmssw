# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step4 --conditions auto:phase2_realistic -s HARVESTING:@baseValidation+@muonOnlyValidation --era Phase2C2 --scenario pp --filetype DQM --geometry Extended2023D4 --mc --python HARVESTFullGlobal_2023tilted_PU0.py -n -1 --no_exec --filein file:step32_inDQMinDQM__inDQM_inDQM.root --fileout file:step4.root
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('HARVESTING',eras.Phase2C2)

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
#                                      'file:step31_inDQM.root'
#                                      'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_900_pre1_2023D4_PU0/161206_155659/0000/step31_inDQM_1.root',
#                                      'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_900_pre1_2023D4_PU0/161206_155659/0000/step31_inDQM_2.root',
#                                      'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_900_pre1_2023D4_PU0/161206_155659/0000/step31_inDQM_3.root',
#                                      'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_900_pre1_2023D4_PU0/161206_155659/0000/step31_inDQM_4.root',
#                                      'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_900_pre1_2023D4_PU0/161206_155659/0000/step31_inDQM_5.root',
#                                      'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_900_pre1_2023D4_PU0/161206_155659/0000/step31_inDQM_6.root',
#                                      'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_900_pre1_2023D4_PU0/161206_155659/0000/step31_inDQM_7.root',
#                                      'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_900_pre1_2023D4_PU0/161206_155659/0000/step31_inDQM_8.root'
#                                      'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_900_pre1_2023D4_PU0_2/161207_162621/0000/step31_inDQM_1.root',
#                                      'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_900_pre1_2023D4_PU0_2/161207_162621/0000/step31_inDQM_2.root',
#                                      'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_900_pre1_2023D4_PU0_2/161207_162621/0000/step31_inDQM_3.root',
#                                      'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_900_pre1_2023D4_PU0_2/161207_162621/0000/step31_inDQM_4.root',
#                                      'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_900_pre1_2023D4_PU0_2/161207_162621/0000/step31_inDQM_5.root',
#                                      'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_900_pre1_2023D4_PU0_2/161207_162621/0000/step31_inDQM_6.root',
#                                      'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_900_pre1_2023D4_PU0_2/161207_162621/0000/step31_inDQM_7.root',
#                                      'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_900_pre1_2023D4_PU0_2/161207_162621/0000/step31_inDQM_8.root'
#                                      '/store/user/calabria/RelValZMM_14/crab_Val_900_pre1_2023D4_PU0_D4_3/161214_175345/0000/step31_2.root',
#                                      '/store/user/calabria/RelValZMM_14/crab_Val_900_pre1_2023D4_PU0_D4_3/161214_183233/0000/step31_1.root',
#                                      '/store/user/calabria/RelValZMM_14/crab_Val_900_pre1_2023D4_PU0_D4_3/161214_183233/0000/step31_3.root',
#                                      '/store/user/calabria/RelValZMM_14/crab_Val_900_pre1_2023D4_PU0_D4_3/161214_183233/0000/step31_4.root',
#                                      '/store/user/calabria/RelValZMM_14/crab_Val_900_pre1_2023D4_PU0_D4_3/161214_183233/0000/step31_5.root',
#                                      '/store/user/calabria/RelValZMM_14/crab_Val_900_pre1_2023D4_PU0_D4_3/161214_183233/0000/step31_6.root',
#                                      '/store/user/calabria/RelValZMM_14/crab_Val_900_pre1_2023D4_PU0_D4_3/161214_183233/0000/step31_7.root'
                                      'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_900_pre1_2023D4_PU0_D4_4/161215_105633/0000/step31_inDQM_1.root',
                                      'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_900_pre1_2023D4_PU0_D4_4/161215_105633/0000/step31_inDQM_2.root',
                                      'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_900_pre1_2023D4_PU0_D4_4/161215_105633/0000/step31_inDQM_3.root',
                                      'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_900_pre1_2023D4_PU0_D4_4/161215_105633/0000/step31_inDQM_4.root',
                                      'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_900_pre1_2023D4_PU0_D4_4/161215_105633/0000/step31_inDQM_5.root',
                                      'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_900_pre1_2023D4_PU0_D4_4/161215_105633/0000/step31_inDQM_6.root',
                                      'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_900_pre1_2023D4_PU0_D4_4/161215_105633/0000/step31_inDQM_7.root',
                                      'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_900_pre1_2023D4_PU0_D4_4/161215_105633/0000/step31_inDQM_8.root'
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

process.dqmSaver.workflow = "/Global/CMSSW_9_0_0_pre1/RECO_PU0_D4_ZMM"

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
