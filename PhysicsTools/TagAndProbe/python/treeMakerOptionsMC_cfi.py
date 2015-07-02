import FWCore.ParameterSet.Config as cms

options = dict()

options['MC_FLAG']                 = cms.bool(True)
options['HLTProcessName']          = "HLT"
options['INPUT_FILE_NAME']         = "file:/cms/data/xrd/store/mc/RunIISpring15DR74/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/AODSIM/Asympt50ns_MCRUN2_74_V9A-v2/00000/0033A97B-8707-E511-9D3B-008CFA1980B8.root",
"file:/cms/data/xrd/store/mc/RunIISpring15DR74/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/AODSIM/Asympt50ns_MCRUN2_74_V9A-v2/00000/003826EE-8807-E511-9628-008CFA06470C.root"
options['OUTPUT_FILE_NAME']        = "TnPTree.root"
options['ELECTRON_COLL']           = "slimmedElectrons"
options['ELECTRON_CUTS']           = "(abs(superCluster.eta)<2.5) && (ecalEnergy*sin(superClusterPosition.theta)>10.0)"
options['ELECTRON_TAG_CUTS']       = "(abs(superCluster.eta)<=2.5) && !(1.4442<=abs(superCluster.eta)<=1.566) && pt >= 25.0"
options['SUPERCLUSTER_COLL']       = "reducedEgamma:reducedSuperClusters"
options['SUPERCLUSTER_CUTS']       = "abs(eta)<2.5 && !(1.4442< abs(eta) <1.566) && et>10.0"
options['TnPPATHS']                = cms.vstring("HLT_Ele20WP60_Ele8_Mass55_v*", "HLT_Ele25WP60_SC4_Mass55_v*")
options['TnPHLTTagFilters']        = cms.vstring("hltEle20WP60Ele8TrackIsoFilter", "hltEle25WP60SC4TrackIsoFilter")
options['TnPHLTProbeFilters']      = cms.vstring("hltEle20WP60Ele8PixelMatchUnseededFilter", "hltEle25WP60SC4EtUnseededFilter")
options['HLTPathToMeasure']        = cms.vstring("HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v15")
options['HLTFILTERTOMEASURE']      = cms.vstring("hltEle17TightIdLooseIsoEle8TightIdLooseIsoTrackIsoFilter")
options['GLOBALTAG']               = 'MCRUN2_74_V9A::All'
options['EVENTSToPROCESS']         = cms.untracked.VEventRange()
options['MAXEVENTS']               = cms.untracked.int32(-1) 
options['useAOD']                  = cms.bool(False)
options['DOTRIGGER']               = cms.bool(True)
options['DORECO']                  = cms.bool(True)
options['DOID']                    = cms.bool(True)
options['OUTPUTEDMFILENAME']       = 'edmFile.root'
options['DEBUG']                   = cms.bool(False)
