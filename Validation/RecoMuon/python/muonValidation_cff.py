import FWCore.ParameterSet.Config as cms

from Validation.RecoMuon.selectors_cff import *
from Validation.RecoMuon.associators_cff import *
# Configurations for MuonTrackValidators
import Validation.RecoMuon.MuonTrackValidator_cfi
from SimTracker.TrackAssociation.LhcParametersDefinerForTP_cfi import *
from SimTracker.TrackAssociation.CosmicParametersDefinerForTP_cfi import *
from SimTracker.TrackAssociatorProducers.trackAssociatorByChi2_cfi import *

trackAssociatorByChi2.chi2cut = cms.double(500.0)
trackAssociatorByPull = trackAssociatorByChi2.clone(chi2cut = 50.0, onlyDiagonal = True)

bestMuonLooseTrackVTrackAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
bestMuonLooseTrackVTrackAssoc.associatormap = 'tpToLooseMuonAssociation'
bestMuonLooseTrackVTrackAssoc.associators = ('MuonAssociationByHits',)
bestMuonLooseTrackVTrackAssoc.label = ('bestMuonLoose',)
bestMuonLooseTrackVTrackAssoc.usetracker = True
bestMuonLooseTrackVTrackAssoc.usemuon = True

bestMuonLooseTrackVTrackAssoc5 = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
bestMuonLooseTrackVTrackAssoc5.associatormap = 'tpToLoose5MuonAssociation'
bestMuonLooseTrackVTrackAssoc5.associators = ('MuonAssociationByHits',)
bestMuonLooseTrackVTrackAssoc5.label = ('bestMuonLoose5',)
bestMuonLooseTrackVTrackAssoc5.ptMinTP = 5.0
bestMuonLooseTrackVTrackAssoc5.usetracker = True
bestMuonLooseTrackVTrackAssoc5.usemuon = True

#-------------------------------------------------------------------------------------------------------------------

staUpdMuonTrackVMuonAssoc2StTime = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
staUpdMuonTrackVMuonAssoc2StTime.associatormap = 'tpToStaUpdMuonAssociation2StTime'
staUpdMuonTrackVMuonAssoc2StTime.associators = ('MuonAssociationByHits',)
staUpdMuonTrackVMuonAssoc2StTime.label = ('extractedSTAMuons2StatTiming',)
staUpdMuonTrackVMuonAssoc2StTime.usetracker = False
staUpdMuonTrackVMuonAssoc2StTime.usemuon = True

# about STA

staUpdMuonTrackVSelMuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
staUpdMuonTrackVSelMuonAssoc.associatormap = 'tpToStaUpdSelMuonAssociation'
staUpdMuonTrackVSelMuonAssoc.associators = ('MuonAssociationByHits',)
staUpdMuonTrackVSelMuonAssoc.label = ('standAloneMuons:UpdatedAtVtx',)
staUpdMuonTrackVSelMuonAssoc.usetracker = False
staUpdMuonTrackVSelMuonAssoc.usemuon = True

staUpdMuonTrackVSelMuonAssoc2StTime = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
staUpdMuonTrackVSelMuonAssoc2StTime.associatormap = 'tpToStaUpdSelMuonAssociation2StTime'
staUpdMuonTrackVSelMuonAssoc2StTime.associators = ('MuonAssociationByHits',)
staUpdMuonTrackVSelMuonAssoc2StTime.label = ('extractedSTAMuons2StatTiming',)
staUpdMuonTrackVSelMuonAssoc2StTime.usetracker = False
staUpdMuonTrackVSelMuonAssoc2StTime.usemuon = True

staUpdMuonTrackVSel2MuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
staUpdMuonTrackVSel2MuonAssoc.associatormap = 'tpToStaUpdSel2MuonAssociation'
staUpdMuonTrackVSel2MuonAssoc.associators = ('MuonAssociationByHits',)
staUpdMuonTrackVSel2MuonAssoc.label = ('standAloneMuons:UpdatedAtVtx',)
staUpdMuonTrackVSel2MuonAssoc.usetracker = False
staUpdMuonTrackVSel2MuonAssoc.usemuon = True

staUpdMuonTrackVSel2MuonAssoc2StTime = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
staUpdMuonTrackVSel2MuonAssoc2StTime.associatormap = 'tpToStaUpdSel2MuonAssociation2StTime'
staUpdMuonTrackVSel2MuonAssoc2StTime.associators = ('MuonAssociationByHits',)
staUpdMuonTrackVSel2MuonAssoc2StTime.label = ('extractedSTAMuons2StatTiming',)
staUpdMuonTrackVSel2MuonAssoc2StTime.usetracker = False
staUpdMuonTrackVSel2MuonAssoc2StTime.usemuon = True
#staUpdMuonTrackVSel2MuonAssoc2StTime.ptMinTP = 5.0

staUpdMuonTrackVSel2MuonAssoc2StTime05 = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
staUpdMuonTrackVSel2MuonAssoc2StTime05.associatormap = 'tpToStaUpdSel2MuonAssociation2StTime'
staUpdMuonTrackVSel2MuonAssoc2StTime05.associators = ('MuonAssociationByHits',)
staUpdMuonTrackVSel2MuonAssoc2StTime05.label = ('extractedSTAMuons2StatTiming',)
staUpdMuonTrackVSel2MuonAssoc2StTime05.usetracker = False
staUpdMuonTrackVSel2MuonAssoc2StTime05.usemuon = True
staUpdMuonTrackVSel2MuonAssoc2StTime05.ptMinTP = 5.0
staUpdMuonTrackVSel2MuonAssoc2StTime05.dirName = 'Muons/RecoMuonV/MultiTrack/Cut5/'

# about GLB
glbMuonTrackVSelMuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
glbMuonTrackVSelMuonAssoc.associatormap = 'tpToGlbSelMuonAssociation'
glbMuonTrackVSelMuonAssoc.associators = ('MuonAssociationByHits',)
glbMuonTrackVSelMuonAssoc.label = ('extractedGlobalMuons',)
glbMuonTrackVSelMuonAssoc.usetracker = True
glbMuonTrackVSelMuonAssoc.usemuon = True

glbMuonTrackVSel2MuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
glbMuonTrackVSel2MuonAssoc.associatormap = 'tpToGlbSel2MuonAssociation'
glbMuonTrackVSel2MuonAssoc.associators = ('MuonAssociationByHits',)
glbMuonTrackVSel2MuonAssoc.label = ('extractedGlobalMuons',)
glbMuonTrackVSel2MuonAssoc.usetracker = True
glbMuonTrackVSel2MuonAssoc.usemuon = True

glbMuonTrackVSel3MuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
glbMuonTrackVSel3MuonAssoc.associatormap = 'tpToGlbSel3MuonAssociation'
glbMuonTrackVSel3MuonAssoc.associators = ('MuonAssociationByHits',)
glbMuonTrackVSel3MuonAssoc.label = ('extractedGlobalMuons',)
glbMuonTrackVSel3MuonAssoc.usetracker = True
glbMuonTrackVSel3MuonAssoc.usemuon = True

glbMuonTrackVSel4MuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
glbMuonTrackVSel4MuonAssoc.associatormap = 'tpToGlbSel4MuonAssociation'
glbMuonTrackVSel4MuonAssoc.associators = ('MuonAssociationByHits',)
glbMuonTrackVSel4MuonAssoc.label = ('extractedGlobalMuons',)
glbMuonTrackVSel4MuonAssoc.usetracker = True
glbMuonTrackVSel4MuonAssoc.usemuon = True

#-------------------------------------------------------------------------------------------------------------------

looseMuonTrackVSelMuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
looseMuonTrackVSelMuonAssoc.associatormap = 'tpToLooseSelMuonAssociation'
looseMuonTrackVSelMuonAssoc.associators = ('MuonAssociationByHits',)
looseMuonTrackVSelMuonAssoc.label = ('bestMuonLoose',)
looseMuonTrackVSelMuonAssoc.usetracker = True
looseMuonTrackVSelMuonAssoc.usemuon = True

looseMuonTrackVSel2MuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
looseMuonTrackVSel2MuonAssoc.associatormap = 'tpToLooseSel2MuonAssociation'
looseMuonTrackVSel2MuonAssoc.associators = ('MuonAssociationByHits',)
looseMuonTrackVSel2MuonAssoc.label = ('bestMuonLoose',)
looseMuonTrackVSel2MuonAssoc.usetracker = True
looseMuonTrackVSel2MuonAssoc.usemuon = True

looseMuonTrackVSel0MuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
looseMuonTrackVSel0MuonAssoc.associatormap = 'tpToLooseSel0MuonAssociation'
looseMuonTrackVSel0MuonAssoc.associators = ('MuonAssociationByHits',)
looseMuonTrackVSel0MuonAssoc.label = ('bestMuonLoose',)
looseMuonTrackVSel0MuonAssoc.usetracker = True
looseMuonTrackVSel0MuonAssoc.usemuon = True
looseMuonTrackVSel0MuonAssoc.useMCTruth = False

looseMuonTrackVSelUncMuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
looseMuonTrackVSelUncMuonAssoc.associatormap = 'tpToLooseSelUncMuonAssociation'
looseMuonTrackVSelUncMuonAssoc.associators = ('MuonAssociationByHits',)
looseMuonTrackVSelUncMuonAssoc.label = ('bestMuonLoose',)
looseMuonTrackVSelUncMuonAssoc.usetracker = False
looseMuonTrackVSelUncMuonAssoc.usemuon = True
looseMuonTrackVSelUncMuonAssoc.useMCTruth = False

looseMuonTrackVSel05SimMuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
looseMuonTrackVSel05SimMuonAssoc.associatormap = 'tpToLooseSel0MuonAssociation'
looseMuonTrackVSel05SimMuonAssoc.associators = ('MuonAssociationByHits',)
looseMuonTrackVSel05SimMuonAssoc.label = ('bestMuonLoose',)
looseMuonTrackVSel05SimMuonAssoc.ptMinTP = 5.0
looseMuonTrackVSel05SimMuonAssoc.usetracker = True
looseMuonTrackVSel05SimMuonAssoc.usemuon = True
looseMuonTrackVSel05SimMuonAssoc.useMCTruth = False
looseMuonTrackVSel05SimMuonAssoc.dirName = 'Muons/RecoMuonV/MultiTrack/Cut5/'

looseMuonTrackVSel05MuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
looseMuonTrackVSel05MuonAssoc.associatormap = 'tpToLooseSel05MuonAssociation'
looseMuonTrackVSel05MuonAssoc.associators = ('MuonAssociationByHits',)
looseMuonTrackVSel05MuonAssoc.label = ('bestMuonLoose5',)
looseMuonTrackVSel05MuonAssoc.ptMinTP = 5.0
looseMuonTrackVSel05MuonAssoc.usetracker = True
looseMuonTrackVSel05MuonAssoc.usemuon = True
looseMuonTrackVSel05MuonAssoc.useMCTruth = False

#--------------------------------------------------------------------------------------------------------------------

looseMuonTrackVSel3MuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
looseMuonTrackVSel3MuonAssoc.associatormap = 'tpToLooseSel3MuonAssociation'
looseMuonTrackVSel3MuonAssoc.associators = ('MuonAssociationByHits',)
looseMuonTrackVSel3MuonAssoc.label = ('bestMuonLoose',)
looseMuonTrackVSel3MuonAssoc.usetracker = True
looseMuonTrackVSel3MuonAssoc.usemuon = True
looseMuonTrackVSel3MuonAssoc.useMCTruth = False

looseMuonTrackVSel35SimMuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
looseMuonTrackVSel35SimMuonAssoc.associatormap = 'tpToLooseSel3MuonAssociation'
looseMuonTrackVSel35SimMuonAssoc.associators = ('MuonAssociationByHits',)
looseMuonTrackVSel35SimMuonAssoc.label = ('bestMuonLoose',)
looseMuonTrackVSel35SimMuonAssoc.ptMinTP = 5.0
looseMuonTrackVSel35SimMuonAssoc.usetracker = True
looseMuonTrackVSel35SimMuonAssoc.usemuon = True
looseMuonTrackVSel35SimMuonAssoc.useMCTruth = False
looseMuonTrackVSel35SimMuonAssoc.dirName = 'Muons/RecoMuonV/MultiTrack/Cut5/'

looseMuonTrackVSel35MuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
looseMuonTrackVSel35MuonAssoc.associatormap = 'tpToLooseSel35MuonAssociation'
looseMuonTrackVSel35MuonAssoc.associators = ('MuonAssociationByHits',)
looseMuonTrackVSel35MuonAssoc.label = ('bestMuonLoose5',)
looseMuonTrackVSel35MuonAssoc.ptMinTP = 5.0
looseMuonTrackVSel35MuonAssoc.usetracker = True
looseMuonTrackVSel35MuonAssoc.usemuon = True
looseMuonTrackVSel35MuonAssoc.useMCTruth = False

looseMuonTrackVSel4MuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
looseMuonTrackVSel4MuonAssoc.associatormap = 'tpToLooseSel4MuonAssociation'
looseMuonTrackVSel4MuonAssoc.associators = ('MuonAssociationByHits',)
looseMuonTrackVSel4MuonAssoc.label = ('bestMuonLoose',)
looseMuonTrackVSel4MuonAssoc.usetracker = True
looseMuonTrackVSel4MuonAssoc.usemuon = True

#-------------------------------------------------------------------------------------------------------------------

looseModMuonTrackVSel0MuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
looseModMuonTrackVSel0MuonAssoc.associatormap = 'tpToLooseModSel0MuonAssociation'
looseModMuonTrackVSel0MuonAssoc.associators = ('MuonAssociationByHits',)
looseModMuonTrackVSel0MuonAssoc.label = ('bestMuonLooseMod',)
looseModMuonTrackVSel0MuonAssoc.usetracker = True
looseModMuonTrackVSel0MuonAssoc.usemuon = True
looseModMuonTrackVSel0MuonAssoc.useMCTruth = False

looseModMuonTrackVSelUncMuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
looseModMuonTrackVSelUncMuonAssoc.associatormap = 'tpToLooseModSelUncMuonAssociation'
looseModMuonTrackVSelUncMuonAssoc.associators = ('MuonAssociationByHits',)
looseModMuonTrackVSelUncMuonAssoc.label = ('bestMuonLooseMod',)
looseModMuonTrackVSelUncMuonAssoc.usetracker = False
looseModMuonTrackVSelUncMuonAssoc.usemuon = True
looseModMuonTrackVSelUncMuonAssoc.useMCTruth = False

looseModMuonTrackVSel05SimMuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
looseModMuonTrackVSel05SimMuonAssoc.associatormap = 'tpToLooseModSel0MuonAssociation'
looseModMuonTrackVSel05SimMuonAssoc.associators = ('MuonAssociationByHits',)
looseModMuonTrackVSel05SimMuonAssoc.label = ('bestMuonLooseMod',)
looseModMuonTrackVSel05SimMuonAssoc.ptMinTP = 5.0
looseModMuonTrackVSel05SimMuonAssoc.usetracker = True
looseModMuonTrackVSel05SimMuonAssoc.usemuon = True
looseModMuonTrackVSel05SimMuonAssoc.useMCTruth = False
looseModMuonTrackVSel05SimMuonAssoc.dirName = 'Muons/RecoMuonV/MultiTrack/Cut5/'

looseModMuonTrackVSel05MuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
looseModMuonTrackVSel05MuonAssoc.associatormap = 'tpToLooseModSel05MuonAssociation'
looseModMuonTrackVSel05MuonAssoc.associators = ('MuonAssociationByHits',)
looseModMuonTrackVSel05MuonAssoc.label = ('bestMuonLooseMod5',)
looseModMuonTrackVSel05MuonAssoc.ptMinTP = 5.0
looseModMuonTrackVSel05MuonAssoc.usetracker = True
looseModMuonTrackVSel05MuonAssoc.usemuon = True
looseModMuonTrackVSel05MuonAssoc.useMCTruth = False

#-------------------------------------------------------------------------------------------------------------------

looseMuonTrackVChi2MuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
looseMuonTrackVChi2MuonAssoc.associatormap = ''
looseMuonTrackVChi2MuonAssoc.associators =  ['trackAssociatorByPull']
looseMuonTrackVChi2MuonAssoc.label = ('bestMuonLoose',)
looseMuonTrackVChi2MuonAssoc.UseAssociators = True

looseMuonTrackVChi25SimMuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
looseMuonTrackVChi25SimMuonAssoc.associatormap = ''
looseMuonTrackVChi25SimMuonAssoc.associators =  ['trackAssociatorByPull']
looseMuonTrackVChi25SimMuonAssoc.label = ('bestMuonLoose',)
looseMuonTrackVChi25SimMuonAssoc.ptMinTP = 5.0
looseMuonTrackVChi25SimMuonAssoc.dirName = 'Muons/RecoMuonV/MultiTrack/Cut5/'
looseMuonTrackVChi25SimMuonAssoc.UseAssociators = True

tightMuonTrackVChi2MuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
tightMuonTrackVChi2MuonAssoc.associatormap = ''
tightMuonTrackVChi2MuonAssoc.associators =  ['trackAssociatorByPull']
tightMuonTrackVChi2MuonAssoc.label = ('bestMuonTight',)
tightMuonTrackVChi2MuonAssoc.UseAssociators = True

tightMuonTrackVChi25SimMuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
tightMuonTrackVChi25SimMuonAssoc.associatormap = ''
tightMuonTrackVChi25SimMuonAssoc.associators =  ['trackAssociatorByPull']
tightMuonTrackVChi25SimMuonAssoc.label = ('bestMuonTight',)
tightMuonTrackVChi25SimMuonAssoc.ptMinTP = 5.0
tightMuonTrackVChi25SimMuonAssoc.dirName = 'Muons/RecoMuonV/MultiTrack/Cut5/'
tightMuonTrackVChi25SimMuonAssoc.UseAssociators = True

#-------------------------------------------------------------------------------------------------------------------

tightMuonTrackVSelMuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
tightMuonTrackVSelMuonAssoc.associatormap = 'tpToTightSelMuonAssociation'
tightMuonTrackVSelMuonAssoc.associators = ('MuonAssociationByHits',)
tightMuonTrackVSelMuonAssoc.label = ('bestMuonTight',)
tightMuonTrackVSelMuonAssoc.usetracker = True
tightMuonTrackVSelMuonAssoc.usemuon = True

tightMuonTrackVSel2MuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
tightMuonTrackVSel2MuonAssoc.associatormap = 'tpToTightSel2MuonAssociation'
tightMuonTrackVSel2MuonAssoc.associators = ('MuonAssociationByHits',)
tightMuonTrackVSel2MuonAssoc.label = ('bestMuonTight',)
tightMuonTrackVSel2MuonAssoc.usetracker = True
tightMuonTrackVSel2MuonAssoc.usemuon = True

tightMuonTrackVSel0MuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
tightMuonTrackVSel0MuonAssoc.associatormap = 'tpToTightSel0MuonAssociation'
tightMuonTrackVSel0MuonAssoc.associators = ('MuonAssociationByHits',)
tightMuonTrackVSel0MuonAssoc.label = ('bestMuonTight',)
tightMuonTrackVSel0MuonAssoc.usetracker = True
tightMuonTrackVSel0MuonAssoc.usemuon = True
tightMuonTrackVSel0MuonAssoc.useMCTruth = False

tightMuonTrackVSelUncMuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
tightMuonTrackVSelUncMuonAssoc.associatormap = 'tpToTightSelUncMuonAssociation'
tightMuonTrackVSelUncMuonAssoc.associators = ('MuonAssociationByHits',)
tightMuonTrackVSelUncMuonAssoc.label = ('bestMuonTight',)
tightMuonTrackVSelUncMuonAssoc.usetracker = False
tightMuonTrackVSelUncMuonAssoc.usemuon = True
tightMuonTrackVSelUncMuonAssoc.useMCTruth = False

tightMuonTrackVSel05SimMuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
tightMuonTrackVSel05SimMuonAssoc.associatormap = 'tpToTightSel0MuonAssociation'
tightMuonTrackVSel05SimMuonAssoc.associators = ('MuonAssociationByHits',)
tightMuonTrackVSel05SimMuonAssoc.label = ('bestMuonTight',)
tightMuonTrackVSel05SimMuonAssoc.ptMinTP = 5.0
tightMuonTrackVSel05SimMuonAssoc.usetracker = True
tightMuonTrackVSel05SimMuonAssoc.usemuon = True
tightMuonTrackVSel05SimMuonAssoc.useMCTruth = False
tightMuonTrackVSel05SimMuonAssoc.dirName = 'Muons/RecoMuonV/MultiTrack/Cut5/'

tightMuonTrackVSel05MuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
tightMuonTrackVSel05MuonAssoc.associatormap = 'tpToTightSel05MuonAssociation'
tightMuonTrackVSel05MuonAssoc.associators = ('MuonAssociationByHits',)
tightMuonTrackVSel05MuonAssoc.label = ('bestMuonTight5',)
tightMuonTrackVSel05MuonAssoc.ptMinTP = 5.0
tightMuonTrackVSel05MuonAssoc.usetracker = True
tightMuonTrackVSel05MuonAssoc.usemuon = True
tightMuonTrackVSel05MuonAssoc.useMCTruth = False

tightMuonTrackVSel3MuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
tightMuonTrackVSel3MuonAssoc.associatormap = 'tpToTightSel3MuonAssociation'
tightMuonTrackVSel3MuonAssoc.associators = ('MuonAssociationByHits',)
tightMuonTrackVSel3MuonAssoc.label = ('bestMuonTight',)
tightMuonTrackVSel3MuonAssoc.usetracker = True
tightMuonTrackVSel3MuonAssoc.usemuon = True
tightMuonTrackVSel3MuonAssoc.useMCTruth = False

tightMuonTrackVSel35SimMuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
tightMuonTrackVSel35SimMuonAssoc.associatormap = 'tpToTightSel3MuonAssociation'
tightMuonTrackVSel35SimMuonAssoc.associators = ('MuonAssociationByHits',)
tightMuonTrackVSel35SimMuonAssoc.label = ('bestMuonTight',)
tightMuonTrackVSel35SimMuonAssoc.ptMinTP = 5.0
tightMuonTrackVSel35SimMuonAssoc.usetracker = True
tightMuonTrackVSel35SimMuonAssoc.usemuon = True
tightMuonTrackVSel35SimMuonAssoc.useMCTruth = False
tightMuonTrackVSel35SimMuonAssoc.dirName = 'Muons/RecoMuonV/MultiTrack/Cut5/'

tightMuonTrackVSel35MuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
tightMuonTrackVSel35MuonAssoc.associatormap = 'tpToTightSel35MuonAssociation'
tightMuonTrackVSel35MuonAssoc.associators = ('MuonAssociationByHits',)
tightMuonTrackVSel35MuonAssoc.label = ('bestMuonTight5',)
tightMuonTrackVSel35MuonAssoc.ptMinTP = 5.0
tightMuonTrackVSel35MuonAssoc.usetracker = True
tightMuonTrackVSel35MuonAssoc.usemuon = True
tightMuonTrackVSel35MuonAssoc.useMCTruth = False

tightMuonTrackVSel4MuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
tightMuonTrackVSel4MuonAssoc.associatormap = 'tpToTightSel4MuonAssociation'
tightMuonTrackVSel4MuonAssoc.associators = ('MuonAssociationByHits',)
tightMuonTrackVSel4MuonAssoc.label = ('bestMuonTight',)
tightMuonTrackVSel4MuonAssoc.usetracker = True
tightMuonTrackVSel4MuonAssoc.usemuon = True

#-------------------------------------------------------------------------------------------------------------------

tightModMuonTrackVSel0MuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
tightModMuonTrackVSel0MuonAssoc.associatormap = 'tpToTightModSel0MuonAssociation'
tightModMuonTrackVSel0MuonAssoc.associators = ('MuonAssociationByHits',)
tightModMuonTrackVSel0MuonAssoc.label = ('bestMuonTightMod',)
tightModMuonTrackVSel0MuonAssoc.usetracker = True
tightModMuonTrackVSel0MuonAssoc.usemuon = True
tightModMuonTrackVSel0MuonAssoc.useMCTruth = False

tightModMuonTrackVSelUncMuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
tightModMuonTrackVSelUncMuonAssoc.associatormap = 'tpToTightModSelUncMuonAssociation'
tightModMuonTrackVSelUncMuonAssoc.associators = ('MuonAssociationByHits',)
tightModMuonTrackVSelUncMuonAssoc.label = ('bestMuonTightMod',)
tightModMuonTrackVSelUncMuonAssoc.usetracker = False
tightModMuonTrackVSelUncMuonAssoc.usemuon = True
tightModMuonTrackVSelUncMuonAssoc.useMCTruth = False

tightModMuonTrackVSel05SimMuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
tightModMuonTrackVSel05SimMuonAssoc.associatormap = 'tpToTightModSel0MuonAssociation'
tightModMuonTrackVSel05SimMuonAssoc.associators = ('MuonAssociationByHits',)
tightModMuonTrackVSel05SimMuonAssoc.label = ('bestMuonTightMod',)
tightModMuonTrackVSel05SimMuonAssoc.ptMinTP = 5.0
tightModMuonTrackVSel05SimMuonAssoc.usetracker = True
tightModMuonTrackVSel05SimMuonAssoc.usemuon = True
tightModMuonTrackVSel05SimMuonAssoc.useMCTruth = False
tightModMuonTrackVSel05SimMuonAssoc.dirName = 'Muons/RecoMuonV/MultiTrack/Cut5/'

tightModMuonTrackVSel05MuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
tightModMuonTrackVSel05MuonAssoc.associatormap = 'tpToTightModSel05MuonAssociation'
tightModMuonTrackVSel05MuonAssoc.associators = ('MuonAssociationByHits',)
tightModMuonTrackVSel05MuonAssoc.label = ('bestMuonTightMod5',)
tightModMuonTrackVSel05MuonAssoc.ptMinTP = 5.0
tightModMuonTrackVSel05MuonAssoc.usetracker = True
tightModMuonTrackVSel05MuonAssoc.usemuon = True
tightModMuonTrackVSel05MuonAssoc.useMCTruth = False

#-----------------------------------------------------------------------------------------------------------------------

trkMuonTrackVTrackAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
trkMuonTrackVTrackAssoc.associatormap = 'tpToTkmuTrackAssociation'
trkMuonTrackVTrackAssoc.associators = ('trackAssociatorByHits',)
#trkMuonTrackVTrackAssoc.label = ('generalTracks',)
trkMuonTrackVTrackAssoc.label = ('probeTracks',)
trkMuonTrackVTrackAssoc.usetracker = True
trkMuonTrackVTrackAssoc.usemuon = False

trkCosmicMuonTrackVTrackAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
trkCosmicMuonTrackVTrackAssoc.associatormap = 'tpToTkCosmicTrackAssociation'
trkCosmicMuonTrackVTrackAssoc.associators = ('trackAssociatorByHits',)
trkCosmicMuonTrackVTrackAssoc.label = ('ctfWithMaterialTracksP5LHCNavigation',)
trkCosmicMuonTrackVTrackAssoc.usetracker = True
trkCosmicMuonTrackVTrackAssoc.usemuon = False

staMuonTrackVTrackAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
staMuonTrackVTrackAssoc.associatormap = 'tpToStaTrackAssociation'
staMuonTrackVTrackAssoc.associators = ('trackAssociatorByDeltaR',)
staMuonTrackVTrackAssoc.label = ('standAloneMuons',)
staMuonTrackVTrackAssoc.usetracker = False
staMuonTrackVTrackAssoc.usemuon = True

staUpdMuonTrackVTrackAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
staUpdMuonTrackVTrackAssoc.associatormap = 'tpToStaUpdTrackAssociation'
staUpdMuonTrackVTrackAssoc.associators = ('trackAssociatorByDeltaR',)
staUpdMuonTrackVTrackAssoc.label = ('standAloneMuons:UpdatedAtVtx',)
staUpdMuonTrackVTrackAssoc.usetracker = False
staUpdMuonTrackVTrackAssoc.usemuon = True

glbMuonTrackVTrackAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
glbMuonTrackVTrackAssoc.associatormap = 'tpToGlbTrackAssociation'
glbMuonTrackVTrackAssoc.associators = ('trackAssociatorByDeltaR',)
glbMuonTrackVTrackAssoc.label = ('globalMuons',)
glbMuonTrackVTrackAssoc.usetracker = True
glbMuonTrackVTrackAssoc.usemuon = True

tevMuonFirstTrackVTrackAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
tevMuonFirstTrackVTrackAssoc.associatormap = 'tpToTevFirstTrackAssociation'
tevMuonFirstTrackVTrackAssoc.associators = ('trackAssociatorByDeltaR',)
tevMuonFirstTrackVTrackAssoc.label = ('tevMuons:firstHit',)
tevMuonFirstTrackVTrackAssoc.usetracker = True
tevMuonFirstTrackVTrackAssoc.usemuon = True

tevMuonPickyTrackVTrackAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
tevMuonPickyTrackVTrackAssoc.associatormap = 'tpToTevPickyTrackAssociation'
tevMuonPickyTrackVTrackAssoc.associators = ('trackAssociatorByDeltaR',)
tevMuonPickyTrackVTrackAssoc.label = ('tevMuons:picky',)
tevMuonPickyTrackVTrackAssoc.usetracker = True
tevMuonPickyTrackVTrackAssoc.usemuon = True

tevMuonDytTrackVTrackAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
tevMuonDytTrackVTrackAssoc.associatormap = 'tpToTevDytTrackAssociation'
tevMuonDytTrackVTrackAssoc.associators = ('trackAssociatorByDeltaR',)
tevMuonDytTrackVTrackAssoc.label = ('tevMuons:dyt',)
tevMuonDytTrackVTrackAssoc.usetracker = True
tevMuonDytTrackVTrackAssoc.usemuon = True

staCosmicMuonTrackVTrackAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
staCosmicMuonTrackVTrackAssoc.associatormap = 'tpToStaCosmicTrackAssociation'
staCosmicMuonTrackVTrackAssoc.associators = ('trackAssociatorByDeltaR',)
staCosmicMuonTrackVTrackAssoc.label = ('cosmicMuons',)
staCosmicMuonTrackVTrackAssoc.usetracker = False
staCosmicMuonTrackVTrackAssoc.usemuon = True

glbCosmicMuonTrackVTrackAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
glbCosmicMuonTrackVTrackAssoc.associatormap = 'tpToGlbCosmicTrackAssociation'
glbCosmicMuonTrackVTrackAssoc.associators = ('trackAssociatorByDeltaR',)
glbCosmicMuonTrackVTrackAssoc.label = ('globalCosmicMuons',)
glbCosmicMuonTrackVTrackAssoc.usetracker = True
glbCosmicMuonTrackVTrackAssoc.usemuon = True

trkProbeTrackVMuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
#trkMuonTrackVMuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
trkProbeTrackVMuonAssoc.associatormap = 'tpToTkMuonAssociation' 
trkProbeTrackVMuonAssoc.associators = ('MuonAssociationByHits',)
##trkMuonTrackVMuonAssoc.label = ('generalTracks',)
trkProbeTrackVMuonAssoc.label = ('probeTracks',)
trkProbeTrackVMuonAssoc.usetracker = True
trkProbeTrackVMuonAssoc.usemuon = False

staSeedTrackVMuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
staSeedTrackVMuonAssoc.associatormap = 'tpToStaSeedAssociation'
staSeedTrackVMuonAssoc.associators = ('MuonAssociationByHits',)
staSeedTrackVMuonAssoc.label = ('seedsOfSTAmuons',)
staSeedTrackVMuonAssoc.usetracker = False
staSeedTrackVMuonAssoc.usemuon = True

staMuonTrackVMuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
staMuonTrackVMuonAssoc.associatormap = 'tpToStaMuonAssociation'
staMuonTrackVMuonAssoc.associators = ('MuonAssociationByHits',)
staMuonTrackVMuonAssoc.label = ('standAloneMuons',)
staMuonTrackVMuonAssoc.usetracker = False
staMuonTrackVMuonAssoc.usemuon = True

staUpdMuonTrackVMuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
staUpdMuonTrackVMuonAssoc.associatormap = 'tpToStaUpdMuonAssociation'
staUpdMuonTrackVMuonAssoc.associators = ('MuonAssociationByHits',)
staUpdMuonTrackVMuonAssoc.label = ('standAloneMuons:UpdatedAtVtx',)
staUpdMuonTrackVMuonAssoc.usetracker = False
staUpdMuonTrackVMuonAssoc.usemuon = True

glbMuonTrackVMuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
glbMuonTrackVMuonAssoc.associatormap = 'tpToGlbMuonAssociation'
glbMuonTrackVMuonAssoc.associators = ('MuonAssociationByHits',)
glbMuonTrackVMuonAssoc.label = ('extractedGlobalMuons',)
glbMuonTrackVMuonAssoc.usetracker = True
glbMuonTrackVMuonAssoc.usemuon = True

staRefitMuonTrackVMuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
staRefitMuonTrackVMuonAssoc.associatormap = 'tpToStaRefitMuonAssociation'
staRefitMuonTrackVMuonAssoc.associators = ('MuonAssociationByHits',)
staRefitMuonTrackVMuonAssoc.label = ('refittedStandAloneMuons',)
staRefitMuonTrackVMuonAssoc.usetracker = False
staRefitMuonTrackVMuonAssoc.usemuon = True

staRefitUpdMuonTrackVMuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
staRefitUpdMuonTrackVMuonAssoc.associatormap = 'tpToStaRefitUpdMuonAssociation'
staRefitUpdMuonTrackVMuonAssoc.associators = ('MuonAssociationByHits',)
staRefitUpdMuonTrackVMuonAssoc.label = ('refittedStandAloneMuons:UpdatedAtVtx',)
staRefitUpdMuonTrackVMuonAssoc.usetracker = False
staRefitUpdMuonTrackVMuonAssoc.usemuon = True

displacedTrackVMuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
displacedTrackVMuonAssoc.associatormap = 'tpToDisplacedTrkMuonAssociation'
displacedTrackVMuonAssoc.associators = ('MuonAssociationByHits',)
displacedTrackVMuonAssoc.label = ('displacedTracks',)
displacedTrackVMuonAssoc.usetracker = True
displacedTrackVMuonAssoc.usemuon = False
displacedTrackVMuonAssoc.tipTP = cms.double(85.)
displacedTrackVMuonAssoc.lipTP = cms.double(210.)

displacedStaSeedTrackVMuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
displacedStaSeedTrackVMuonAssoc.associatormap = 'tpToDisplacedStaSeedAssociation'
displacedStaSeedTrackVMuonAssoc.associators = ('MuonAssociationByHits',)
displacedStaSeedTrackVMuonAssoc.label = ('seedsOfDisplacedSTAmuons',)
displacedStaSeedTrackVMuonAssoc.usetracker = False
displacedStaSeedTrackVMuonAssoc.usemuon = True
displacedStaSeedTrackVMuonAssoc.tipTP = cms.double(85.)
displacedStaSeedTrackVMuonAssoc.lipTP = cms.double(210.)

#displacedStaMuonTrackVMuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
#displacedStaMuonTrackVMuonAssoc.associatormap = 'tpToDisplacedStaMuonAssociation'
#displacedStaMuonTrackVMuonAssoc.associators = ('MuonAssociationByHits',)
#displacedStaMuonTrackVMuonAssoc.label = ('displacedStandAloneMuons',)
#displacedStaMuonTrackVMuonAssoc.usetracker = False
#displacedStaMuonTrackVMuonAssoc.usemuon = True
#displacedStaMuonTrackVMuonAssoc.tipTP = cms.double(85.)
#displacedStaMuonTrackVMuonAssoc.lipTP = cms.double(210.)

displacedStaMuonTrackVMuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
displacedStaMuonTrackVMuonAssoc.associatormap = 'tpToDisplacedStaMuonAssociation'
displacedStaMuonTrackVMuonAssoc.associators = ('MuonAssociationByHits',)
displacedStaMuonTrackVMuonAssoc.label = ('displacedStandAloneMuons',)
displacedStaMuonTrackVMuonAssoc.usetracker = False
displacedStaMuonTrackVMuonAssoc.usemuon = True
displacedStaMuonTrackVMuonAssoc.tipTP = 300000.
displacedStaMuonTrackVMuonAssoc.lipTP = 300000.
displacedStaMuonTrackVMuonAssoc.vertexSrc = ""
displacedStaMuonTrackVMuonAssoc.prodRho = 350.
displacedStaMuonTrackVMuonAssoc.prodZ = 500.

displacedGlbMuonTrackVMuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
displacedGlbMuonTrackVMuonAssoc.associatormap = 'tpToDisplacedGlbMuonAssociation'
displacedGlbMuonTrackVMuonAssoc.associators = ('MuonAssociationByHits',)
displacedGlbMuonTrackVMuonAssoc.label = ('displacedGlobalMuons',)
displacedGlbMuonTrackVMuonAssoc.usetracker = True
displacedGlbMuonTrackVMuonAssoc.usemuon = True
displacedGlbMuonTrackVMuonAssoc.tipTP = cms.double(85.)
displacedGlbMuonTrackVMuonAssoc.lipTP = cms.double(210.)

tevMuonFirstTrackVMuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
tevMuonFirstTrackVMuonAssoc.associatormap = 'tpToTevFirstMuonAssociation'
tevMuonFirstTrackVMuonAssoc.associators = ('MuonAssociationByHits',)
tevMuonFirstTrackVMuonAssoc.label = ('tevMuons:firstHit',)
tevMuonFirstTrackVMuonAssoc.usetracker = True
tevMuonFirstTrackVMuonAssoc.usemuon = True

tevMuonPickyTrackVMuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
tevMuonPickyTrackVMuonAssoc.associatormap = 'tpToTevPickyMuonAssociation'
tevMuonPickyTrackVMuonAssoc.associators = ('MuonAssociationByHits',)
tevMuonPickyTrackVMuonAssoc.label = ('tevMuons:picky',)
tevMuonPickyTrackVMuonAssoc.usetracker = True
tevMuonPickyTrackVMuonAssoc.usemuon = True

tevMuonDytTrackVMuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
tevMuonDytTrackVMuonAssoc.associatormap = 'tpToTevDytMuonAssociation'
tevMuonDytTrackVMuonAssoc.associators = ('MuonAssociationByHits',)
tevMuonDytTrackVMuonAssoc.label = ('tevMuons:dyt',)
tevMuonDytTrackVMuonAssoc.usetracker = True
tevMuonDytTrackVMuonAssoc.usemuon = True

gemMuonTrackVMuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
gemMuonTrackVMuonAssoc.associatormap = 'tpToGEMMuonMuonAssociation'
gemMuonTrackVMuonAssoc.associators = ('MuonAssociationByHits',)
gemMuonTrackVMuonAssoc.label = ('extractGemMuons',)
gemMuonTrackVMuonAssoc.minRapidityTP = 1.6
gemMuonTrackVMuonAssoc.maxRapidityTP = 2.4
gemMuonTrackVMuonAssoc.usetracker = True
gemMuonTrackVMuonAssoc.usemuon = False
gemMuonTrackVMuonAssoc.min = 1.6
gemMuonTrackVMuonAssoc.max = 2.4
gemMuonTrackVMuonAssoc.nint = 8
gemMuonTrackVMuonAssoc.minRes = 2.0
gemMuonTrackVMuonAssoc.maxRes = 2.8
gemMuonTrackVMuonAssoc.nintRes = 8
gemMuonTrackVMuonAssoc.useFabsEta = True
gemMuonTrackVMuonAssoc.useAbsEta = True

me0MuonTrackVMuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
me0MuonTrackVMuonAssoc.associatormap = 'tpToME0MuonMuonAssociation'
me0MuonTrackVMuonAssoc.associators = ('MuonAssociationByHits',)
me0MuonTrackVMuonAssoc.label = ('extractMe0Muons',)
me0MuonTrackVMuonAssoc.minRapidityTP = 2.0
me0MuonTrackVMuonAssoc.maxRapidityTP = 2.8
me0MuonTrackVMuonAssoc.usetracker = True
me0MuonTrackVMuonAssoc.usemuon = False
me0MuonTrackVMuonAssoc.min = 2.0
me0MuonTrackVMuonAssoc.max = 2.8
me0MuonTrackVMuonAssoc.nint = 8
me0MuonTrackVMuonAssoc.minRes = 2.0
me0MuonTrackVMuonAssoc.maxRes = 2.8
me0MuonTrackVMuonAssoc.nintRes = 8
me0MuonTrackVMuonAssoc.useFabsEta = True
me0MuonTrackVMuonAssoc.useAbsEta = True

# cosmics 2-leg reco
trkCosmicMuonTrackVSelMuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
trkCosmicMuonTrackVSelMuonAssoc.associatormap = 'tpToTkCosmicSelMuonAssociation'
trkCosmicMuonTrackVSelMuonAssoc.associators = ('MuonAssociationByHits',)
trkCosmicMuonTrackVSelMuonAssoc.label = ('ctfWithMaterialTracksP5LHCNavigation',)
trkCosmicMuonTrackVSelMuonAssoc.usetracker = True
trkCosmicMuonTrackVSelMuonAssoc.usemuon = False
trkCosmicMuonTrackVSelMuonAssoc.parametersDefiner = cms.string('CosmicParametersDefinerForTP')
trkCosmicMuonTrackVSelMuonAssoc.ptMinTP = cms.double(1.)
trkCosmicMuonTrackVSelMuonAssoc.tipTP = cms.double(80.)
trkCosmicMuonTrackVSelMuonAssoc.lipTP = cms.double(212.)
trkCosmicMuonTrackVSelMuonAssoc.BiDirectional_RecoToSim_association = False

staCosmicMuonTrackVSelMuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
staCosmicMuonTrackVSelMuonAssoc.associatormap = 'tpToStaCosmicSelMuonAssociation'
staCosmicMuonTrackVSelMuonAssoc.associators = ('MuonAssociationByHits',)
staCosmicMuonTrackVSelMuonAssoc.label = ('cosmicMuons',)
staCosmicMuonTrackVSelMuonAssoc.usetracker = False
staCosmicMuonTrackVSelMuonAssoc.usemuon = True
staCosmicMuonTrackVSelMuonAssoc.parametersDefiner = cms.string('CosmicParametersDefinerForTP')
staCosmicMuonTrackVSelMuonAssoc.ptMinTP = cms.double(1.)
staCosmicMuonTrackVSelMuonAssoc.tipTP = cms.double(80.)
staCosmicMuonTrackVSelMuonAssoc.lipTP = cms.double(212.)
staCosmicMuonTrackVSelMuonAssoc.BiDirectional_RecoToSim_association = False

glbCosmicMuonTrackVSelMuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
glbCosmicMuonTrackVSelMuonAssoc.associatormap = 'tpToGlbCosmicSelMuonAssociation'
glbCosmicMuonTrackVSelMuonAssoc.associators = ('MuonAssociationByHits',)
glbCosmicMuonTrackVSelMuonAssoc.label = ('globalCosmicMuons',)
glbCosmicMuonTrackVSelMuonAssoc.usetracker = True
glbCosmicMuonTrackVSelMuonAssoc.usemuon = True
glbCosmicMuonTrackVSelMuonAssoc.parametersDefiner = cms.string('CosmicParametersDefinerForTP')
glbCosmicMuonTrackVSelMuonAssoc.ptMinTP = cms.double(1.)
glbCosmicMuonTrackVSelMuonAssoc.tipTP = cms.double(80.)
glbCosmicMuonTrackVSelMuonAssoc.lipTP = cms.double(212.)
glbCosmicMuonTrackVSelMuonAssoc.BiDirectional_RecoToSim_association = False

# cosmics 1-leg reco
trkCosmic1LegMuonTrackVSelMuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
trkCosmic1LegMuonTrackVSelMuonAssoc.associatormap = 'tpToTkCosmic1LegSelMuonAssociation'
trkCosmic1LegMuonTrackVSelMuonAssoc.associators = ('MuonAssociationByHits',)
trkCosmic1LegMuonTrackVSelMuonAssoc.label = ('ctfWithMaterialTracksP5',)
trkCosmic1LegMuonTrackVSelMuonAssoc.usetracker = True
trkCosmic1LegMuonTrackVSelMuonAssoc.usemuon = False
trkCosmic1LegMuonTrackVSelMuonAssoc.parametersDefiner = cms.string('CosmicParametersDefinerForTP')
trkCosmic1LegMuonTrackVSelMuonAssoc.ptMinTP = cms.double(1.)
trkCosmic1LegMuonTrackVSelMuonAssoc.tipTP = cms.double(80.)
trkCosmic1LegMuonTrackVSelMuonAssoc.lipTP = cms.double(212.)
trkCosmic1LegMuonTrackVSelMuonAssoc.BiDirectional_RecoToSim_association = False

staCosmic1LegMuonTrackVSelMuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
staCosmic1LegMuonTrackVSelMuonAssoc.associatormap = 'tpToStaCosmic1LegSelMuonAssociation'
staCosmic1LegMuonTrackVSelMuonAssoc.associators = ('MuonAssociationByHits',)
staCosmic1LegMuonTrackVSelMuonAssoc.label = ('cosmicMuons1Leg',)
staCosmic1LegMuonTrackVSelMuonAssoc.usetracker = False
staCosmic1LegMuonTrackVSelMuonAssoc.usemuon = True
staCosmic1LegMuonTrackVSelMuonAssoc.parametersDefiner = cms.string('CosmicParametersDefinerForTP')
staCosmic1LegMuonTrackVSelMuonAssoc.ptMinTP = cms.double(1.)
staCosmic1LegMuonTrackVSelMuonAssoc.tipTP = cms.double(80.)
staCosmic1LegMuonTrackVSelMuonAssoc.lipTP = cms.double(212.)
staCosmic1LegMuonTrackVSelMuonAssoc.BiDirectional_RecoToSim_association = False

glbCosmic1LegMuonTrackVSelMuonAssoc = Validation.RecoMuon.MuonTrackValidator_cfi.muonTrackValidator.clone()
glbCosmic1LegMuonTrackVSelMuonAssoc.associatormap = 'tpToGlbCosmic1LegSelMuonAssociation'
glbCosmic1LegMuonTrackVSelMuonAssoc.associators = ('MuonAssociationByHits',)
glbCosmic1LegMuonTrackVSelMuonAssoc.label = ('globalCosmicMuons1Leg',)
glbCosmic1LegMuonTrackVSelMuonAssoc.usetracker = True
glbCosmic1LegMuonTrackVSelMuonAssoc.usemuon = True
glbCosmic1LegMuonTrackVSelMuonAssoc.parametersDefiner = cms.string('CosmicParametersDefinerForTP')
glbCosmic1LegMuonTrackVSelMuonAssoc.ptMinTP = cms.double(1.)
glbCosmic1LegMuonTrackVSelMuonAssoc.tipTP = cms.double(80.)
glbCosmic1LegMuonTrackVSelMuonAssoc.lipTP = cms.double(212.)
glbCosmic1LegMuonTrackVSelMuonAssoc.BiDirectional_RecoToSim_association = False

# Configurations for RecoMuonValidators
from RecoMuon.TrackingTools.MuonServiceProxy_cff import *
from Validation.RecoMuon.RecoMuonValidator_cfi import *

#import SimGeneral.MixingModule.mixNoPU_cfi
from SimMuon.MCTruth.muonAssociatorByHitsNoSimHitsHelper_cfi import *
from SimMuon.MCTruth.MuonAssociatorByHits_cfi import muonAssociatorByHitsCommonParameters

#tracker
muonAssociatorByHitsNoSimHitsHelperTrk = SimMuon.MCTruth.muonAssociatorByHitsNoSimHitsHelper_cfi.muonAssociatorByHitsNoSimHitsHelper.clone()
muonAssociatorByHitsNoSimHitsHelperTrk.UseTracker = True
muonAssociatorByHitsNoSimHitsHelperTrk.UseMuon  = False
recoMuonVMuAssoc_trk = Validation.RecoMuon.RecoMuonValidator_cfi.recoMuonValidator.clone()
recoMuonVMuAssoc_trk.subDir = 'Muons/RecoMuonV/RecoMuon_MuonAssoc_Trk'
recoMuonVMuAssoc_trk.simLabel = 'mix:MergedTrackTruth'
recoMuonVMuAssoc_trk.muAssocLabel = 'muonAssociatorByHitsNoSimHitsHelperTrk'
recoMuonVMuAssoc_trk.trackType = 'inner'
recoMuonVMuAssoc_trk.selection = "isTrackerMuon"

#tracker and PF
muonAssociatorByHitsNoSimHitsHelperTrkPF = SimMuon.MCTruth.muonAssociatorByHitsNoSimHitsHelper_cfi.muonAssociatorByHitsNoSimHitsHelper.clone()
muonAssociatorByHitsNoSimHitsHelperTrkPF.UseTracker = True
muonAssociatorByHitsNoSimHitsHelperTrkPF.UseMuon  = False
recoMuonVMuAssoc_trkPF = Validation.RecoMuon.RecoMuonValidator_cfi.recoMuonValidator.clone()
recoMuonVMuAssoc_trkPF.subDir = 'Muons/RecoMuonV/RecoMuon_MuonAssoc_TrkPF'
recoMuonVMuAssoc_trkPF.usePFMuon = True
recoMuonVMuAssoc_trkPF.simLabel = 'mix:MergedTrackTruth'
recoMuonVMuAssoc_trkPF.muAssocLabel = 'muonAssociatorByHitsNoSimHitsHelperTrkPF'
recoMuonVMuAssoc_trkPF.trackType = 'inner'
recoMuonVMuAssoc_trkPF.selection = "isTrackerMuon & isPFMuon"

#standalone
muonAssociatorByHitsNoSimHitsHelperStandalone = SimMuon.MCTruth.muonAssociatorByHitsNoSimHitsHelper_cfi.muonAssociatorByHitsNoSimHitsHelper.clone()
muonAssociatorByHitsNoSimHitsHelperStandalone.UseTracker = False
muonAssociatorByHitsNoSimHitsHelperStandalone.UseMuon  = True
recoMuonVMuAssoc_sta = Validation.RecoMuon.RecoMuonValidator_cfi.recoMuonValidator.clone()
recoMuonVMuAssoc_sta.subDir = 'Muons/RecoMuonV/RecoMuon_MuonAssoc_Sta'
recoMuonVMuAssoc_sta.simLabel = 'mix:MergedTrackTruth'
recoMuonVMuAssoc_sta.muAssocLabel = 'muonAssociatorByHitsNoSimHitsHelperStandalone'
recoMuonVMuAssoc_sta.trackType = 'outer'
recoMuonVMuAssoc_sta.selection = "isStandAloneMuon"

#seed of StandAlone
muonAssociatorByHitsNoSimHitsHelperSeedStandalone = SimMuon.MCTruth.muonAssociatorByHitsNoSimHitsHelper_cfi.muonAssociatorByHitsNoSimHitsHelper.clone()
muonAssociatorByHitsNoSimHitsHelperSeedStandalone.UseTracker = False
muonAssociatorByHitsNoSimHitsHelperSeedStandalone.UseMuon  = True
recoMuonVMuAssoc_seedSta = Validation.RecoMuon.RecoMuonValidator_cfi.recoMuonValidator.clone()
recoMuonVMuAssoc_seedSta.subDir = 'Muons/RecoMuonV/RecoMuon_MuonAssoc_SeedSta'
recoMuonVMuAssoc_seedSta.simLabel = 'mix:MergedTrackTruth'
recoMuonVMuAssoc_seedSta.muAssocLabel = 'muonAssociatorByHitsNoSimHitsHelperStandalone'
recoMuonVMuAssoc_seedSta.trackType = 'outer'
recoMuonVMuAssoc_seedSta.selection = ""

#standalone and PF
muonAssociatorByHitsNoSimHitsHelperStandalonePF = SimMuon.MCTruth.muonAssociatorByHitsNoSimHitsHelper_cfi.muonAssociatorByHitsNoSimHitsHelper.clone()
muonAssociatorByHitsNoSimHitsHelperStandalonePF.UseTracker = False
muonAssociatorByHitsNoSimHitsHelperStandalonePF.UseMuon  = True
recoMuonVMuAssoc_staPF = Validation.RecoMuon.RecoMuonValidator_cfi.recoMuonValidator.clone()
recoMuonVMuAssoc_staPF.subDir = 'Muons/RecoMuonV/RecoMuon_MuonAssoc_StaPF'
recoMuonVMuAssoc_staPF.usePFMuon = True
recoMuonVMuAssoc_staPF.simLabel = 'mix:MergedTrackTruth'
recoMuonVMuAssoc_staPF.muAssocLabel = 'muonAssociatorByHitsNoSimHitsHelperStandalonePF'
recoMuonVMuAssoc_staPF.trackType = 'outer'
recoMuonVMuAssoc_staPF.selection = "isStandAloneMuon & isPFMuon"

#global
muonAssociatorByHitsNoSimHitsHelperGlobal = SimMuon.MCTruth.muonAssociatorByHitsNoSimHitsHelper_cfi.muonAssociatorByHitsNoSimHitsHelper.clone()
muonAssociatorByHitsNoSimHitsHelperGlobal.UseTracker = True
muonAssociatorByHitsNoSimHitsHelperGlobal.UseMuon  = True
recoMuonVMuAssoc_glb = Validation.RecoMuon.RecoMuonValidator_cfi.recoMuonValidator.clone()
recoMuonVMuAssoc_glb.subDir = 'Muons/RecoMuonV/RecoMuon_MuonAssoc_Glb'
recoMuonVMuAssoc_glb.simLabel = 'mix:MergedTrackTruth'
recoMuonVMuAssoc_glb.muAssocLabel = 'muonAssociatorByHitsNoSimHitsHelperGlobal'
recoMuonVMuAssoc_glb.trackType = 'global'
recoMuonVMuAssoc_glb.selection = "isGlobalMuon"

#global and PF
muonAssociatorByHitsNoSimHitsHelperGlobalPF = SimMuon.MCTruth.muonAssociatorByHitsNoSimHitsHelper_cfi.muonAssociatorByHitsNoSimHitsHelper.clone()
muonAssociatorByHitsNoSimHitsHelperGlobalPF.UseTracker = True
muonAssociatorByHitsNoSimHitsHelperGlobalPF.UseMuon  = True
recoMuonVMuAssoc_glbPF = Validation.RecoMuon.RecoMuonValidator_cfi.recoMuonValidator.clone()
recoMuonVMuAssoc_glbPF.subDir = 'Muons/RecoMuonV/RecoMuon_MuonAssoc_GlbPF'
recoMuonVMuAssoc_glbPF.usePFMuon = True
recoMuonVMuAssoc_glbPF.simLabel = 'mix:MergedTrackTruth'
recoMuonVMuAssoc_glbPF.muAssocLabel = 'muonAssociatorByHitsNoSimHitsHelperGlobalPF'
recoMuonVMuAssoc_glbPF.trackType = 'global'
recoMuonVMuAssoc_glbPF.selection = "isGlobalMuon & isPFMuon"

#tight
muonAssociatorByHitsNoSimHitsHelperTight = SimMuon.MCTruth.muonAssociatorByHitsNoSimHitsHelper_cfi.muonAssociatorByHitsNoSimHitsHelper.clone()
muonAssociatorByHitsNoSimHitsHelperTight.UseTracker = True
muonAssociatorByHitsNoSimHitsHelperTight.UseMuon  = True
recoMuonVMuAssoc_tgt = Validation.RecoMuon.RecoMuonValidator_cfi.recoMuonValidator.clone()
recoMuonVMuAssoc_tgt.subDir = 'Muons/RecoMuonV/RecoMuon_MuonAssoc_Tgt'
recoMuonVMuAssoc_tgt.simLabel = 'mix:MergedTrackTruth'
recoMuonVMuAssoc_tgt.muAssocLabel = 'muonAssociatorByHitsNoSimHitsHelperTight'
recoMuonVMuAssoc_tgt.trackType = 'global'
recoMuonVMuAssoc_tgt.selection = 'isGlobalMuon'
recoMuonVMuAssoc_tgt.wantTightMuon = True
recoMuonVMuAssoc_tgt.beamSpot = 'offlineBeamSpot'
recoMuonVMuAssoc_tgt.primaryVertex = 'offlinePrimaryVertices'

# Muon validation sequences

muonValidation_seq = cms.Sequence(
#   trkProbeTrackVMuonAssoc+trkMuonTrackVTrackAssoc
#   + staSeedTrackVMuonAssoc
#	+ staMuonTrackVMuonAssoc + 
	staUpdMuonTrackVMuonAssoc
#	+staUpdMuonTrackVMuonAssoc2St+
#   +staUpdMuonTrackVMuonAssoc2StTime
	+ glbMuonTrackVMuonAssoc
	#+ trkProbeTrackVSelMuonAssoc
	#+ staUpdMuonTrackVSelMuonAssoc+staUpdMuonTrackVSel2MuonAssoc
	#+ staUpdMuonTrackVSelMuonAssoc2StTime
 	+staUpdMuonTrackVSel2MuonAssoc2StTime
    +staUpdMuonTrackVSel2MuonAssoc2StTime05
	#+ glbMuonTrackVSelMuonAssoc+glbMuonTrackVSel2MuonAssoc+glbMuonTrackVSel3MuonAssoc+glbMuonTrackVSel4MuonAssoc
	#+ looseMuonTrackVSelMuonAssoc+looseMuonTrackVSel2MuonAssoc
	#+ looseMuonTrackVSel3MuonAssoc 
	+ looseMuonTrackVSel0MuonAssoc
    #+ looseMuonTrackVSelUncMuonAssoc
	#+looseMuonTrackVSel4MuonAssoc
	#+ looseMuonTrackVSel35MuonAssoc
# 	+ looseMuonTrackVSel05MuonAssoc
	#+ looseMuonTrackVSel35SimMuonAssoc
	+ looseMuonTrackVSel05SimMuonAssoc
	#+ tightMuonTrackVSelMuonAssoc+tightMuonTrackVSel2MuonAssoc
	#+ tightMuonTrackVSel3MuonAssoc 
	+ tightMuonTrackVSel0MuonAssoc
    #+ tightMuonTrackVSelUncMuonAssoc
	#+tightMuonTrackVSel4MuonAssoc
	#+ tightMuonTrackVSel35MuonAssoc 
#	+ tightMuonTrackVSel05MuonAssoc
	#+ tightMuonTrackVSel35SimMuonAssoc
	+ tightMuonTrackVSel05SimMuonAssoc
	+ looseModMuonTrackVSel0MuonAssoc
    #+ looseModMuonTrackVSelUncMuonAssoc
# 	+ looseModMuonTrackVSel05MuonAssoc
	+ looseModMuonTrackVSel05SimMuonAssoc
	+ tightModMuonTrackVSel0MuonAssoc
    #+ tightModMuonTrackVSelUncMuonAssoc
# 	+ tightModMuonTrackVSel05MuonAssoc
   + tightModMuonTrackVSel05SimMuonAssoc
#	+ recoMuonVMuAssoc_trk+recoMuonVMuAssoc_sta+recoMuonVMuAssoc_glb+recoMuonVMuAssoc_tgt
	#+ bestMuonLooseTrackVTrackAssoc #+ bestMuonLooseTrackVTrackAssoc5
#	+ bestMuonLoose2TrackVTrackAssoc + bestMuonLoose2TrackVTrackAssoc5
	#+ bestMuonTightTrackVTrackAssoc #+ bestMuonTightTrackVTrackAssoc5
#    +looseMuonTrackVChi2MuonAssoc
#    +looseMuonTrackVChi25SimMuonAssoc
#    +tightMuonTrackVChi2MuonAssoc
#    +looseMuonTrackVChi25SimMuonAssoc
)

muonValidation_reduced_seq = cms.Sequence(
    probeTracks_seq + tpToTkMuonAssociation + trkProbeTrackVMuonAssoc
#    +trackAssociatorByHits + tpToTkmuTrackAssociation + trkMuonTrackVTrackAssoc
#    +seedsOfSTAmuons_seq + tpToStaSeedAssociation + staSeedTrackVMuonAssoc
#    +tpToStaMuonAssociation + staMuonTrackVMuonAssoc
    +tpToStaUpdMuonAssociation + staUpdMuonTrackVMuonAssoc
    +extractedMuonTracks_seq + tpToGlbMuonAssociation + glbMuonTrackVMuonAssoc
#    +muonAssociatorByHitsNoSimHitsHelperTrk +recoMuonVMuAssoc_trk
#    +muonAssociatorByHitsNoSimHitsHelperStandalone +recoMuonVMuAssoc_sta
#    +muonAssociatorByHitsNoSimHitsHelperGlobal +recoMuonVMuAssoc_glb
#    +muonAssociatorByHitsNoSimHitsHelperTight +recoMuonVMuAssoc_tgt
)

muonValidationTEV_seq = cms.Sequence(
    tpToTevFirstMuonAssociation + tevMuonFirstTrackVMuonAssoc
    +tpToTevPickyMuonAssociation + tevMuonPickyTrackVMuonAssoc
    +tpToTevDytMuonAssociation + tevMuonDytTrackVMuonAssoc
)

muonValidationRefit_seq = cms.Sequence(
    tpToStaRefitMuonAssociation + staRefitMuonTrackVMuonAssoc
    +tpToStaRefitUpdMuonAssociation + staRefitUpdMuonTrackVMuonAssoc
)

muonValidationDisplaced_seq = cms.Sequence(
    seedsOfDisplacedSTAmuons_seq + tpToDisplacedStaSeedAssociation + displacedStaSeedTrackVMuonAssoc
    +tpToDisplacedStaMuonAssociation + displacedStaMuonTrackVMuonAssoc
#    +tpToDisplacedTrkMuonAssociation + displacedTrackVMuonAssoc
    +tpToDisplacedGlbMuonAssociation + displacedGlbMuonTrackVMuonAssoc
)

muonValidationCosmic_seq = cms.Sequence(
    tpToTkCosmicSelMuonAssociation + trkCosmicMuonTrackVSelMuonAssoc
    +tpToTkCosmic1LegSelMuonAssociation + trkCosmic1LegMuonTrackVSelMuonAssoc
    +tpToStaCosmicSelMuonAssociation + staCosmicMuonTrackVSelMuonAssoc
    +tpToStaCosmic1LegSelMuonAssociation + staCosmic1LegMuonTrackVSelMuonAssoc
    +tpToGlbCosmicSelMuonAssociation + glbCosmicMuonTrackVSelMuonAssoc
    +tpToGlbCosmic1LegSelMuonAssociation + glbCosmic1LegMuonTrackVSelMuonAssoc
)

# The full offline muon validation sequence
recoMuonValidation = cms.Sequence(
    muonAssociation_seq * muonValidation_seq
    #+ muonValidationTEV_seq + muonValidationRefit_seq
    + muonValidationDisplaced_seq
)

# no displaces in fastsim
from Configuration.Eras.Modifier_fastSim_cff import fastSim
if fastSim.isChosen():
    recoMuonValidation = cms.Sequence(muonValidation_seq + muonValidationTEV_seq + muonValidationRefit_seq)

# sequence for cosmic muons
recoCosmicMuonValidation = cms.Sequence(
    muonValidationCosmic_seq
)

gemMuonValidation = cms.Sequence(extractGemMuonsTracks_seq + tpToGEMMuonMuonAssociation + gemMuonTrackVMuonAssoc)
me0MuonValidation = cms.Sequence(extractMe0MuonsTracks_seq + tpToME0MuonMuonAssociation + me0MuonTrackVMuonAssoc)

_run3_muonValidation = muonValidation_seq.copy() #For full validation
#_run3_muonValidation = muonValidation_reduced_seq.copy()
_run3_muonValidation += gemMuonValidation

_phase2_muonValidation = _run3_muonValidation.copy()
_phase2_muonValidation += me0MuonValidation

from Configuration.Eras.Modifier_run3_GEM_cff import run3_GEM
run3_GEM.toReplaceWith( muonValidation_seq, _run3_muonValidation ) #For full validation
#run3_GEM.toReplaceWith( recoMuonValidation, _run3_muonValidation )
from Configuration.Eras.Modifier_phase2_muon_cff import phase2_muon
phase2_muon.toReplaceWith( muonValidation_seq, _phase2_muonValidation ) #For full validation
#phase2_muon.toReplaceWith( recoMuonValidation, _phase2_muonValidation )
