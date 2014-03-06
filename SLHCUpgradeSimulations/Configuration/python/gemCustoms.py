import FWCore.ParameterSet.Config as cms

def customise(process):
    if hasattr(process,'digitisation_step'):
        process=customise_Digi(process)
    if hasattr(process,'L1simulation_step'):
       process=customise_L1Emulator(process)
    if hasattr(process,'DigiToRaw'):
        process=customise_DigiToRaw(process)
    if hasattr(process,'RawToDigi'):
        process=customise_RawToDigi(process)
    if hasattr(process,'reconstruction'):
        process=customise_Reco(process)
    if hasattr(process,'dqmoffline_step'):
        process=customise_DQM(process)
    if hasattr(process,'dqmHarvesting'):
        process=customise_harvesting(process)
    if hasattr(process,'validation_step'):
        process=customise_Validation(process)

    return process

def customise_Digi(process):
    process.RandomNumberGeneratorService.simMuonGEMDigis = cms.PSet(
        initialSeed = cms.untracked.uint32(1234567),
        engineName = cms.untracked.string('HepJamesRandom')
    )

    process.mix.mixObjects.mixSH.crossingFrames.append('MuonGEMHits')
    process.mix.mixObjects.mixSH.input.append(cms.InputTag("g4SimHits","MuonGEMHits"))
    process.mix.mixObjects.mixSH.subdets.append('MuonGEMHits')

    process.load('SimMuon.GEMDigitizer.muonGEMDigis_cfi')
    process.load('SimMuon.GEMDigitizer.muonGEMCSCPadDigis_cfi')
    process.muonDigi += process.simMuonGEMDigis
    process.muonDigi += process.simMuonGEMCSCPadDigis

    process=outputCustoms(process)
    return process

def customise_L1Emulator(process, ptdphi):
    process.simCscTriggerPrimitiveDigis.gemPadProducer =  cms.untracked.InputTag("simMuonGEMCSCPadDigis","")
    process.simCscTriggerPrimitiveDigis.clctSLHC.clctPidThreshPretrig = 2
    process.simCscTriggerPrimitiveDigis.clctParam07.clctPidThreshPretrig = 2

    dphi_lct_pad98 = {
        'pt0'  : { 'odd' :  2.00000000 , 'even' :  2.00000000 },
        'pt05' : { 'odd' :  0.02203510 , 'even' :  0.00930056 },
        'pt06' : { 'odd' :  0.01825790 , 'even' :  0.00790009 },
        'pt10' : { 'odd' :  0.01066000 , 'even' :  0.00483286 },
        'pt15' : { 'odd' :  0.00722795 , 'even' :  0.00363230 },
        'pt20' : { 'odd' :  0.00562598 , 'even' :  0.00304879 },
        'pt30' : { 'odd' :  0.00416544 , 'even' :  0.00253782 },
        'pt40' : { 'odd' :  0.00342827 , 'even' :  0.00230833 }
    }

    tmb = process.simCscTriggerPrimitiveDigis.tmbSLHC
    tmb.runGEMCSCILT = cms.untracked.bool(True)
    tmb.printAvailablePads = cms.untracked.bool(False)
    tmb.dropLowQualityCLCTsNoGEMs_ME1a = cms.untracked.bool(False)
    tmb.dropLowQualityCLCTsNoGEMs_ME1b = cms.untracked.bool(False)
    tmb.buildLCTfromALCTandGEM_ME1a = cms.untracked.bool(False)
    tmb.buildLCTfromALCTandGEM_ME1b = cms.untracked.bool(False) 
    tmb.doLCTGhostBustingWithGEMs = cms.untracked.bool(False)
    tmb.useOldLCTDataFormatALCTGEM = cms.untracked.bool(True)
    tmb.gemMatchDeltaEta = cms.untracked.double(0.08)
    tmb.gemMatchDeltaBX = cms.untracked.int32(1)
    tmb.maxDeltaBXInCoPad = cms.untracked.int32(1)
    tmb.maxDeltaRollInCoPad = cms.untracked.int32(0)
    tmb.maxDeltaPadInCoPad = cms.untracked.int32(0)
    tmb.maxDeltaBXPad = cms.untracked.int32(0)
    tmb.maxDeltaRollPad = cms.untracked.int32(0)
    tmb.maxDeltaPadPad = cms.untracked.int32(0)
    tmb.maxDeltaBXCoPad = cms.untracked.int32(0)
    tmb.maxDeltaRollCoPad = cms.untracked.int32(0)
    tmb.maxDeltaPadCoPad = cms.untracked.int32(0)
    tmb.gemMatchDeltaPhiOdd = cms.untracked.double(dphi_lct_pad98[ptdphi]['odd'])
    tmb.gemMatchDeltaPhiEven = cms.untracked.double(dphi_lct_pad98[ptdphi]['even'])
    print tmb.gemMatchDeltaPhiOdd, tmb.gemMatchDeltaPhiEven
    if ptdphi == 'pt0':
        tmb.gemClearNomatchLCTs = cms.untracked.bool(False)
        
    return process

def customise_DigiToRaw(process):
    return process

def customise_RawToDigi(process):
    return process

def customise_Reco(process):
    process.load('RecoLocalMuon.GEMRecHit.gemRecHits_cfi')
    process.gemRecHits.gemDigiLabel = cms.InputTag("simMuonGEMDigis")
    process.muonlocalreco += process.gemRecHits
    process=outputCustoms(process)
    process.standAloneMuons.STATrajBuilderParameters.EnableGEMMeasurement = cms.bool(True)
    process.standAloneMuons.STATrajBuilderParameters.BWFilterParameters.EnableGEMMeasurement = cms.bool(True)
    return process

def customise_DQM(process):
    return process

def customise_harvesting(process):
    return (process)

def customise_Validation(process):
    return process

def outputCustoms(process):
    alist=['AODSIM','RECOSIM','FEVTSIM','FEVTDEBUG','FEVTDEBUGHLT','RECODEBUG','RAWRECOSIMHLT','RAWRECODEBUGHLT']
    for a in alist:
        b=a+'output'
        if hasattr(process,b):
            getattr(process,b).outputCommands.append('keep *_simMuonGEMDigis_*_*')
            getattr(process,b).outputCommands.append('keep *_simMuonGEMCSCPadDigis_*_*')
            getattr(process,b).outputCommands.append('keep *_gemRecHits_*_*')

    return process
