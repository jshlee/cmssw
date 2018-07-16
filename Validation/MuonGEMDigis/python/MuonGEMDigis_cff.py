import FWCore.ParameterSet.Config as cms

from DQMServices.Core.DQMEDAnalyzer import DQMEDAnalyzer

gemStripValidation = DQMEDAnalyzer('GEMStripDigiValidation',
  outputFile = cms.string(''),
  stripLabel= cms.InputTag('muonGEMDigis'),
  simInputLabel = cms.InputTag('g4SimHits',"MuonGEMHits"),
  # st1, st2 of xbin, st1, st2 of ybin
  nBinGlobalZR = cms.untracked.vdouble(200,200,150,250), 
  # st1 xmin xmax, st2 xmin xmax, st1 ymin ymax, st2 ymin ymax
  RangeGlobalZR = cms.untracked.vdouble(564,574,792,802,110,290,120,390), 
  nBinGlobalXY = cms.untracked.int32(360),
  detailPlot = cms.bool(True), 
  nStripsGE11 =  cms.untracked.int32(384),
  nStripsGE21 =  cms.untracked.int32(768),
  folder = cms.string("MuonGEMDigisV/GEMDigisTask/Strip"),
)


gemPadValidation = DQMEDAnalyzer('GEMPadDigiValidation',
  outputFile = cms.string(''),
  PadLabel = cms.InputTag('simMuonGEMPadDigis'),
  simInputLabel = cms.InputTag('g4SimHits',"MuonGEMHits"),
  nBinGlobalZR = cms.untracked.vdouble(200,200,150,250), 
  RangeGlobalZR = cms.untracked.vdouble(564,574,792,802,110,290,120,390), 
  nBinGlobalXY = cms.untracked.int32(360), 
  detailPlot = cms.bool(True), 
  folder = cms.string("MuonGEMDigisV/GEMDigisTask/Pad"),
)


gemCoPadValidation = DQMEDAnalyzer('GEMCoPadDigiValidation',
  outputFile = cms.string(''),
  CopadLabel = cms.InputTag('simCscTriggerPrimitiveDigis') ,
  simInputLabel = cms.InputTag('g4SimHits',"MuonGEMHits"),
  nBinGlobalZR = cms.untracked.vdouble(200,200,150,250), 
  RangeGlobalZR = cms.untracked.vdouble(564,574,792,802,110,290,120,390), 
  nBinGlobalXY = cms.untracked.int32(360), 
  detailPlot = cms.bool(True), 
  minBXGEM = cms.int32(-1),
  maxBXGEM = cms.int32(1),
  folder = cms.string("MuonGEMDigisV/GEMDigisTask/CoPad"),
)


from Configuration.Eras.Modifier_phase2_muon_cff import phase2_muon
from Configuration.ProcessModifiers.premix_stage2_cff import premix_stage2
(premix_stage2 & phase2_muon).toModify(gemStripValidation, stripLabel = "mixData")
#(premix_stage2 & phase2_muon).toModify(gemDigiTrackValidation, gemDigiInput = "mixData")

gemGeometryChecker = DQMEDAnalyzer('GEMCheckGeometry',
  detailPlot = cms.bool(False), 
)

gemDigiValidation = cms.Sequence(
    gemStripValidation + gemPadValidation + gemCoPadValidation + gemGeometryChecker)
 
from Configuration.Eras.Modifier_run2_GEM_2017_cff import run2_GEM_2017
run2_GEM_2017.toReplaceWith(gemDigiValidation, gemDigiValidation.copyAndExclude([gemPadValidation,gemCoPadValidation]))
