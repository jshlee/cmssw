# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step3 --conditions auto:phase2_realistic -n -1 --era Phase2C2 --eventcontent DQM --runUnscheduled -s VALIDATION:@baseValidation+@muonOnlyValidation --datatier DQMIO --geometry Extended2023D13 --filein file:step3.root --fileout file:step31.root --python Val_2023D13_2.py --no_exec
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

######
#process.load('FWCore.MessageService.MessageLogger_cfi')

#process.MessageLogger.categories = cms.untracked.vstring('MuonAssociatorEDProducer', 'MuonTrackProducer', 'MuonAssociatorByHits', 'DTHitAssociator', 'RPCHitAssociator', 'MuonTruth', 'MuonTrackValidator', 'FwkJob', 'FwkReport', 'FwkSummary', 'Root_NoDictionary')

process.MessageLogger.categories = cms.untracked.vstring('MuonAssociatorEDProducer','MuonToTrackingParticleAssociatorEDProducer',
'MuonAssociatorByHits',
'MuonToTrackingParticleAssociatorByHits','MuonAssociatorByHitsHelper','MuonToTrackingParticleAssociatorByHitsImpl',
'TrackerMuonHitExtractor','MuonTrackProducer','SeedToTrackProducer',
'DTHitAssociator', 'RPCHitAssociator', 'MuonTruth', 'CSCHitAssociator',
'MuonTrackValidator','NewMuonTrackValidator',
'CosmicParametersDefinerForTP','CosmicTrackingParticleSelector',
'MuonMCClassifier', 'MyAnal',
'FwkJob','FwkReport','FwkSummary','Root_NoDictionary')

#process.MessageLogger.debugModules = cms.untracked.vstring('tpToGlbMuonAssociation','glbMuonTrackVMuonAssoc','glbMuonTrackVSelMuonAssoc','glbMuonTrackVSelWithMismatchMuonAssoc')
#process.MessageLogger.debugModules = cms.untracked.vstring('trkCosmicMuonTrackVMuonAssoc',
# 'staCosmicMuonTrackVMuonAssoc','glbCosmicMuonTrackVMuonAssoc')

# comment the modules which you want debug from !
#process.MessageLogger.suppressInfo = cms.untracked.vstring(
##'muonAssociatorByHitsNoSimHitsHelper',
#'classByHitsTM',
#'classByHitsTMLSAT',
#'classByHitsGlb',
#'classByHitsSta',
#'classByHitsGlbTk'
##'classByHitsRecoMu',
#)

process.MessageLogger.cerr = cms.untracked.PSet(
    noTimeStamps = cms.untracked.bool(True),

    threshold = cms.untracked.string('WARNING'),

    MuonAssociatorEDProducer = cms.untracked.PSet(
        limit = cms.untracked.int32(0)
    ),
    MuonToTrackingParticleAssociatorEDProducer = cms.untracked.PSet(
        limit = cms.untracked.int32(0)
    ),
    MuonAssociatorByHits = cms.untracked.PSet(
        limit = cms.untracked.int32(0)
    ),
    MuonToTrackingParticleAssociatorByHits = cms.untracked.PSet(
        limit = cms.untracked.int32(0)
    ),
    MuonToTrackingParticleAssociatorByHitsImpl = cms.untracked.PSet(
        limit = cms.untracked.int32(0)
    ),
    MuonAssociatorByHitsHelper = cms.untracked.PSet(
        limit = cms.untracked.int32(0)
    ),
    TrackerMuonHitExtractor = cms.untracked.PSet(
        limit = cms.untracked.int32(0)
    ),
    MuonTrackProducer = cms.untracked.PSet(
        limit = cms.untracked.int32(0)
    ),
    SeedToTrackProducer = cms.untracked.PSet(
        limit = cms.untracked.int32(0)
    ), 
    DTHitAssociator = cms.untracked.PSet(
        limit = cms.untracked.int32(0)
    ),
    CSCHitAssociator = cms.untracked.PSet(
        limit = cms.untracked.int32(0)
    ),
    RPCHitAssociator = cms.untracked.PSet(
        limit = cms.untracked.int32(0)
    ),
    MuonTruth = cms.untracked.PSet(
        limit = cms.untracked.int32(0)
    ),
    MuonTrackValidator = cms.untracked.PSet(
        limit = cms.untracked.int32(0)
    ),
    MuonMCClassifier = cms.untracked.PSet(
        limit = cms.untracked.int32(0)
    ),
    MyAnal = cms.untracked.PSet(
        limit = cms.untracked.int32(0)
    )
)

process.MessageLogger.cout = cms.untracked.PSet(
    noTimeStamps = cms.untracked.bool(True),

#    threshold = cms.untracked.string('DEBUG'),
    threshold = cms.untracked.string('INFO'),

    default = cms.untracked.PSet(
#        limit = cms.untracked.int32(10000000)
        limit = cms.untracked.int32(0)
    ),
    MuonAssociatorEDProducer = cms.untracked.PSet(
        limit = cms.untracked.int32(10000000)
#        limit = cms.untracked.int32(0)
    ),
    MuonToTrackingParticleAssociatorEDProducer = cms.untracked.PSet(
        limit = cms.untracked.int32(10000000)
#        limit = cms.untracked.int32(0)
    ),
    MuonAssociatorByHits = cms.untracked.PSet(
        limit = cms.untracked.int32(10000000)
#        limit = cms.untracked.int32(0)
    ),
    MuonToTrackingParticleAssociatorByHits = cms.untracked.PSet(
        limit = cms.untracked.int32(10000000)
#        limit = cms.untracked.int32(0)
    ),
    MuonToTrackingParticleAssociatorByHitsImpl = cms.untracked.PSet(
        limit = cms.untracked.int32(10000000)
#        limit = cms.untracked.int32(0)
    ),
    MuonAssociatorByHitsHelper = cms.untracked.PSet(
        limit = cms.untracked.int32(10000000)
#        limit = cms.untracked.int32(0)
    ),
    TrackerMuonHitExtractor = cms.untracked.PSet(
        limit = cms.untracked.int32(10000000)
#        limit = cms.untracked.int32(0)
    ),
    MuonMCClassifier = cms.untracked.PSet(
#        limit = cms.untracked.int32(10000000)
        limit = cms.untracked.int32(0)
    ),
    MyAnal = cms.untracked.PSet(
#        limit = cms.untracked.int32(10000000)
        limit = cms.untracked.int32(0)
    ),
    MuonTrackProducer = cms.untracked.PSet(
#        limit = cms.untracked.int32(10000000)
        limit = cms.untracked.int32(0)
    ),
    SeedToTrackProducer = cms.untracked.PSet(
#        limit = cms.untracked.int32(10000000)
        limit = cms.untracked.int32(0)
    ), 
    DTHitAssociator = cms.untracked.PSet(
#        limit = cms.untracked.int32(10000000)
        limit = cms.untracked.int32(0)
    ),
    CSCHitAssociator = cms.untracked.PSet(
#        limit = cms.untracked.int32(10000000)
        limit = cms.untracked.int32(0)
    ),
    RPCHitAssociator = cms.untracked.PSet(
#        limit = cms.untracked.int32(10000000)
        limit = cms.untracked.int32(0)
    ),
    MuonTruth = cms.untracked.PSet(
#        limit = cms.untracked.int32(10000000)
        limit = cms.untracked.int32(0)
    ),
    MuonTrackValidator = cms.untracked.PSet(
        limit = cms.untracked.int32(10000000)
#        limit = cms.untracked.int32(0)
    ),
    NewMuonTrackValidator = cms.untracked.PSet(
        limit = cms.untracked.int32(10000000)
#        limit = cms.untracked.int32(0)
    ),
    CosmicTrackingParticleSelector = cms.untracked.PSet(
#        limit = cms.untracked.int32(10000000)
        limit = cms.untracked.int32(0)
    ),
    CosmicParametersDefinerForTP = cms.untracked.PSet(
#        limit = cms.untracked.int32(10000000)
        limit = cms.untracked.int32(0)
    ),
    FwkReport = cms.untracked.PSet(
        reportEvery = cms.untracked.int32(1),
        limit = cms.untracked.int32(10000000)
    ),
    FwkSummary = cms.untracked.PSet(
        reportEvery = cms.untracked.int32(1),
        limit = cms.untracked.int32(10000000)
    ),
    FwkJob = cms.untracked.PSet(
        limit = cms.untracked.int32(0)
    ),
    Root_NoDictionary = cms.untracked.PSet(
        limit = cms.untracked.int32(0)
    )
)

#process.MessageLogger.statistics = cms.untracked.vstring('cout')

#process.Tracer = cms.Service("Tracer")



process.MessageLogger.suppressInfo = cms.untracked.vstring(

#    'tpToStaUpdMuonAssociation',
#    'tpToGlbMuonAssociation',
#    'tpToInTimeGlbMuonAssociation',
##    +tpToME0MuonMuonAssociation
##    +tpToME0LooseMuonMuonAssociation
##    +tpToME0TightMuonMuonAssociation
##    +tpToGEMMuonMuonAssociation
#    'tpToStaUpdSel2MuonAssociation2StTime',
##    +tpToLooseSel0MuonAssociation
##    +tpToLooseModSel0MuonAssociation
#    'tpToLooseModExtSel0MuonAssociation',
#    'tpToLooseModExtSel05MuonAssociation',
##    +tpToTightSel0MuonAssociation
##    +tpToTightModSel0MuonAssociation
#    'tpToTightModExtSel0MuonAssociation',
#    'tpToTightModExtSel05MuonAssociation',
#    'tpToTightClassicSel0MuonAssociation',
#    'tpToTightClassicNoIPzSel0MuonAssociation',
##    +tpToTightModExtSimSel0MuonAssociation
#    #STAMuon updated at the vtx
#	'staUpdMuonTrackVMuonAssoc',
#    #GLB muon
#	'glbMuonTrackVMuonAssoc',
#    'glbMuonInTimeTrackVMuonAssoc',
#    #STA muon update at the vtx + timing
# 	'staUpdMuonTrackVSel2MuonAssoc2StTime',
#    #STA muon update at the vtx + timing with sim pT > 5 GeV
#    #+ staUpdMuonTrackVSel2MuonAssoc2StTime05
#    #Loose ID
#    #+ looseMuonTrackVSel0MuonAssoc
#    #Loose ID with sim pT > 5 GeV
#    #+ looseMuonTrackVSel05SimMuonAssoc
#    #Tight ID
#    #+ tightMuonTrackVSel0MuonAssoc
#    #Tight ID with sim pT > 5 GeV
#    #+ tightMuonTrackVSel05SimMuonAssoc
#    #Loose ID with PFMuon selections by hand
#    #+ looseModMuonTrackVSel0MuonAssoc
#    #Loose ID with PFMuon selections by hand with ME0Muon
#    'looseModExtMuonTrackVSel0MuonAssoc',
#    #Loose ID with PFMuon selections by hand and with pT > 5 GeV
#    #+ looseModMuonTrackVSel05SimMuonAssoc
#    #Loose ID with PFMuon selections by hand and with pT > 5 GeV with ME0Muon
#    'looseModExtMuonTrackVSel05SimMuonAssoc',
#    'looseModExtMuonTrackVSel05MuonAssoc',
#    #Tight ID with PFMuon selections by hand
#    #+ tightModMuonTrackVSel0MuonAssoc
#    #Tight ID with PFMuon selections by hand with ME0Muon
#    'tightModExtMuonTrackVSel0MuonAssoc',
#    'tightClassicMuonTrackVSel0MuonAssoc',
#    'tightClassicNoIPzMuonTrackVSel0MuonAssoc',
#    #+ tightModExtSimMuonTrackVSel0MuonAssoc
#    #Tight ID with PFMuon selections by hand and with sim pT > 5 GeV
#    #+ tightModMuonTrackVSel05SimMuonAssoc
#    #Tight ID with PFMuon selections by hand and with sim pT > 5 GeV and with ME0Muon
#    'tightModExtMuonTrackVSel05SimMuonAssoc',
#    'tightModExtMuonTrackVSel05MuonAssoc',
#    'tightClassicMuonTrackVSel05SimMuonAssoc',
#    'tightClassicNoIPzMuonTrackVSel05SimMuonAssoc',
#    #+ tightModExtSimMuonTrackVSel05SimMuonAssoc
#    'tpToME0TightMuonMuonAssociation',
#    'me0TightMuonTrackVMuonAssoc',
#    'tpToGEMMuonMuonAssociation',
#    'gemMuonTrackVMuonAssoc'

)


############################################################

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(500)
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
                            
'/store/relval/CMSSW_9_1_1/RelValZMM_14/GEN-SIM-RECO/91X_upgrade2023_realistic_v1_D17-v1/10000/14A91939-1D3F-E711-A09A-0025905A610A.root',
'/store/relval/CMSSW_9_1_1/RelValZMM_14/GEN-SIM-RECO/91X_upgrade2023_realistic_v1_D17-v1/10000/1AF43477-223F-E711-89EA-0025905A610C.root',
'/store/relval/CMSSW_9_1_1/RelValZMM_14/GEN-SIM-RECO/91X_upgrade2023_realistic_v1_D17-v1/10000/4C4B7246-1E3F-E711-949B-0025905A60DA.root',
'/store/relval/CMSSW_9_1_1/RelValZMM_14/GEN-SIM-RECO/91X_upgrade2023_realistic_v1_D17-v1/10000/722FED43-1E3F-E711-81B6-0CC47A78A3EC.root',
'/store/relval/CMSSW_9_1_1/RelValZMM_14/GEN-SIM-RECO/91X_upgrade2023_realistic_v1_D17-v1/10000/7671FDAA-1F3F-E711-B7DE-0025905B860C.root',
'/store/relval/CMSSW_9_1_1/RelValZMM_14/GEN-SIM-RECO/91X_upgrade2023_realistic_v1_D17-v1/10000/8AFE90A8-1F3F-E711-869F-0CC47A7C34E6.root',
'/store/relval/CMSSW_9_1_1/RelValZMM_14/GEN-SIM-RECO/91X_upgrade2023_realistic_v1_D17-v1/10000/96206C74-223F-E711-9EBC-0CC47A7C3458.root',
'/store/relval/CMSSW_9_1_1/RelValZMM_14/GEN-SIM-RECO/91X_upgrade2023_realistic_v1_D17-v1/10000/AC5F5FF7-1A3F-E711-9F95-0CC47A4D75EC.root',
'/store/relval/CMSSW_9_1_1/RelValZMM_14/GEN-SIM-RECO/91X_upgrade2023_realistic_v1_D17-v1/10000/ACDADD48-203F-E711-ABC1-0CC47A4D75EC.root',
'/store/relval/CMSSW_9_1_1/RelValZMM_14/GEN-SIM-RECO/91X_upgrade2023_realistic_v1_D17-v1/10000/B09C3A31-1F3F-E711-B70F-0CC47A78A3D8.root',
'/store/relval/CMSSW_9_1_1/RelValZMM_14/GEN-SIM-RECO/91X_upgrade2023_realistic_v1_D17-v1/10000/DCC47538-1F3F-E711-888A-0CC47A78A33E.root',
'/store/relval/CMSSW_9_1_1/RelValZMM_14/GEN-SIM-RECO/91X_upgrade2023_realistic_v1_D17-v1/10000/DEAB0FD1-1C3F-E711-83A4-0025905A607A.root',
                            
                            ),
    secondaryFileNames = cms.untracked.vstring(

                            
                            )
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

#process.muonValidation_seq = cms.Sequence()
#process.muonValidationDisplaced_seq = cms.Sequence(
##    process.tpToDisplacedStaMuonAssociation +
##    process.displacedStaMuonTrackVMuonAssoc
#    process.selectedVertices*
#    process.bestMuonLooseMod *
#    process.tpToLooseModSel0MuonAssociation *
#    process.looseModMuonTrackVSel0MuonAssoc
#)
#process.recoMuonValidation = cms.Sequence(
#   process.muonValidation_seq +
#   process.muonValidationDisplaced_seq
#)
#process.gemMuonValidation = cms.Sequence()
#process.me0MuonValidation = cms.Sequence()


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
