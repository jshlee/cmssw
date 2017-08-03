# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step4 --conditions auto:phase2_realistic -s HARVESTING:@baseValidation+@muonOnlyValidation --era Phase2C1 --scenario pp --filetype DQM --geometry Extended2023D3 --mc --python HARVESTFullGlobal_2023D3_PU0.py -n -1 --no_exec --filein file:step32_inDQM.root', --fileout file:step4.root',
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('HARVESTING',eras.Phase2C2)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.Geometry.GeometryExtended2023D17Reco_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.DQMSaverAtRunEnd_cff')
process.load('Configuration.StandardSequences.Harvesting_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# Input source
readFiles = cms.untracked.vstring()
process.source = cms.Source ("DQMRootSource",fileNames = readFiles)
readFiles.extend((
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_1.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_10.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_100.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_101.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_102.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_103.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_104.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_105.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_106.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_107.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_108.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_109.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_11.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_110.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_111.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_112.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_113.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_114.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_115.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_116.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_117.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_118.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_119.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_12.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_120.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_121.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_122.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_123.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_124.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_125.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_126.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_127.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_128.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_129.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_13.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_130.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_131.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_132.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_133.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_134.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_135.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_136.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_137.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_138.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_139.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_14.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_140.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_141.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_142.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_143.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_144.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_145.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_146.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_147.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_148.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_149.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_15.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_150.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_151.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_152.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_153.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_154.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_155.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_156.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_157.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_158.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_159.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_16.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_160.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_161.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_162.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_163.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_164.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_165.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_166.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_167.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_168.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_169.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_17.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_170.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_171.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_172.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_173.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_174.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_175.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_176.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_177.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_178.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_179.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_18.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_180.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_181.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_182.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_183.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_184.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_185.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_186.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_187.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_188.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_189.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_19.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_190.root',
#                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_191.root',
#                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_192.root',
#                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_193.root',
#                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_194.root',
#                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_195.root',
#                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_196.root',
#                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_197.root',
#                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_198.root',
#                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_199.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_2.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_20.root',
#                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_200.root',
#                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_201.root',
#                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_202.root',
#                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_203.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_21.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_22.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_23.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_24.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_25.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_26.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_27.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_28.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_29.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_3.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_30.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_31.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_32.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_33.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_34.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_35.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_36.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_37.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_38.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_39.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_4.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_40.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_41.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_42.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_43.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_44.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_45.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_46.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_47.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_48.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_49.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_5.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_50.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_51.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_52.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_53.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_54.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_55.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_56.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_57.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_58.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_59.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_6.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_60.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_61.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_62.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_63.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_64.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_65.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_66.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_67.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_68.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_69.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_7.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_71.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_72.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_73.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_74.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_75.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_76.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_77.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_78.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_79.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_8.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_80.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_81.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_82.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_83.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_84.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_85.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_86.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_87.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_88.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_89.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_9.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_90.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_91.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_92.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_93.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_94.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_95.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_96.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_97.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_98.root',
                  'file:/lustre/cms/store/user/calabria/RelValZMM_14/crab_Val_911_PU140_D17_8/170719_064627/0000/step31_99.root',
                   ))

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

process.dqmSaver.workflow = "/Global/CMSSW_9_1_1/RECO_PU140_D17_ZMM"

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
