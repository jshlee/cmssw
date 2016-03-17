import FWCore.ParameterSet.Config as cms

process = cms.Process("TrackerGEMEfficiency")
process.load("FWCore.MessageService.MessageLogger_cfi")

#process.load('Configuration.Geometry.GeometryExtended2015MuonGEMDevReco_cff')
process.load('Configuration.Geometry.GeometryExtended2023HGCalMuonReco_cff')
#process.load('Configuration.Geometry.GeometryExtended2023SHCalNoTaperReco_cff')

# CSCGeometry depends on alignment ==> necessary to provide GlobalPositionRecord
process.load("Alignment.CommonAlignmentProducer.FakeAlignmentSource_cfi")
process.load("Geometry.CSCGeometry.cscGeometry_cfi")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

#import sys

#infile = sys.argv[2]
#outroot = sys.argv[3]

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        #open('Sh_trackerGEM_path.txt').readlines()
        #open('HGC_trackerGEM_path.txt').readlines()
        #'file:/cms/home/jskim/cmssw/CMSSW_6_2_0_SLHC27_gem_into_tracker/src/work/condor/MuonGun_RECO_gem_into_tracker/out_reco_0.root'
        open('filelist_MuonGun.txt').readlines()
        #'file:/cms/home/jskim/cmssw/CMSSW_6_2_0_SLHC27_gem_into_tracker/src/work/out_reco.root'
    ),
    duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
    skipBadFiles = cms.untracked.bool(True),
)

process.trackergem = cms.EDAnalyzer('TrackerGEMEfficiencyAnalyzer',
                              # ----------------------------------------------------------------------
                              #RootFileName = cms.untracked.string("Sh_efficiency_0p1_matching.root"),
                              #RootFileName = cms.untracked.string("HGC_efficiency_0p1_matching.root"),
                              RootFileName = cms.untracked.string('eff.root'),
                              # ----------------------------------------------------------------------
                              printSegmntInfo = cms.untracked.bool(False),
                              # ----------------------------------------------------------------------

)

process.p = cms.Path(process.trackergem)
