import FWCore.ParameterSet.Config as cms
from DQMServices.Core.DQMEDAnalyzer import DQMEDAnalyzer

gemSimHitValidation = DQMEDAnalyzer('GEMHitsValidation',
    folder=cms.string("MuonGEMHitsV/GEMHitsTask"),
    verboseSimHit = cms.untracked.int32(1),
    simInputLabel = cms.InputTag('g4SimHits',"MuonGEMHits"),
    # st1, st2 of xbin, st1, st2 of ybin
    nBinGlobalZR = cms.untracked.vdouble(200,200,150,250), 
    # st1 xmin xmax, ymin, ymax, <--> st2 xmin xmax, ymin ymax
    RangeGlobalZR = cms.untracked.vdouble(564, 574, 110, 290,
                                          792, 802, 120, 390),

    nBinGlobalXY = cms.untracked.int32(720),
    detailPlot = cms.bool(True), 
    # station1 min / station1 max / station2 min / station2 max                     
    TOFRange = cms.untracked.vdouble(18, 22, 26, 30),
)


gemSimValidation = cms.Sequence(gemSimHitValidation)
