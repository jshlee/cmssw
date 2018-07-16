import FWCore.ParameterSet.Config as cms


from DQMServices.Core.DQMEDAnalyzer import DQMEDAnalyzer
gemRecHitsValidation = DQMEDAnalyzer('GEMRecHitsValidation',
    verboseSimHit = cms.untracked.int32(1),
    simInputLabel = cms.InputTag('g4SimHits',"MuonGEMHits"),
    recHitsInputLabel = cms.InputTag('gemRecHits'),
    # st1, st2 of xbin, st1, st2 of ybin
    nBinGlobalZR = cms.untracked.vdouble(200,200,150,250), 
    # st1 xmin xmax, st2 xmin xmax, st1 ymin ymax, st2 ymin ymax
    RangeGlobalZR = cms.untracked.vdouble(564,574,792,802,110,290,120,390), 
    nBinGlobalXY = cms.untracked.int32(720), 
    detailPlot = cms.bool(True),
    folder = cms.string("MuonGEMRecHitsV/GEMRecHitsTask"),
)



gemLocalRecoValidation = cms.Sequence(gemRecHitsValidation)
