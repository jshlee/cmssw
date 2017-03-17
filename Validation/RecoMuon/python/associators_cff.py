import FWCore.ParameterSet.Config as cms

#Track selector
from Validation.RecoMuon.selectors_cff import *

#TrackAssociation
from SimTracker.TrackAssociatorProducers.trackAssociatorByChi2_cfi import *
import SimTracker.TrackAssociatorProducers.quickTrackAssociatorByHits_cfi
import SimTracker.TrackAssociatorProducers.trackAssociatorByPosition_cfi

import SimTracker.TrackAssociatorProducers.quickTrackAssociatorByHits_cfi

#TrackAssociatorByHits = SimTracker.TrackAssociatorProducers.quickTrackAssociatorByHits_cfi.quickTrackAssociatorByHits.clone( ComponentName = 'TrackAssociatorByHits' )
trackAssociatorByHits = SimTracker.TrackAssociatorProducers.quickTrackAssociatorByHits_cfi.quickTrackAssociatorByHits.clone()


onlineTrackAssociatorByHits = SimTracker.TrackAssociatorProducers.quickTrackAssociatorByHits_cfi.quickTrackAssociatorByHits.clone()
onlineTrackAssociatorByHits.UseGrouped = cms.bool(False)
onlineTrackAssociatorByHits.UseSplitting = cms.bool(False)
onlineTrackAssociatorByHits.ThreeHitTracksAreSpecial = False

trackAssociatorByPosDeltaR = SimTracker.TrackAssociatorProducers.trackAssociatorByPosition_cfi.trackAssociatorByPosition.clone()
trackAssociatorByPosDeltaR.method = cms.string('momdr')
trackAssociatorByPosDeltaR.QCut = cms.double(0.5)
trackAssociatorByPosDeltaR.ConsiderAllSimHits = cms.bool(True)

#
# Configuration for Muon track extractor
#

selectedVertices = cms.EDFilter("VertexSelector",
    src = cms.InputTag('offlinePrimaryVertices'),
    cut = cms.string("isValid & ndof >= 4 & chi2 > 0 & tracksSize > 0 & abs(z) < 24 & abs(position.Rho) < 2."),
    filter = cms.bool(False)
)

selectedFirstPrimaryVertex = cms.EDFilter("PATSingleVertexSelector",
    mode = cms.string('firstVertex'),
    vertices = cms.InputTag('selectedVertices'),
    filter = cms.bool(False)
)

muonPt5 = cms.EDFilter("MuonSelector",
    src = cms.InputTag("muons"),
    cut = cms.string("pt > 5."),
    filter = cms.bool(False)
)

muon2StatTiming = cms.EDFilter("MuonSelector",
    src = cms.InputTag("muons"),
    cut = cms.string("isStandAloneMuon && outerTrack.hitPattern.muonStationsWithValidHits > 1 && abs(time.timeAtIpInOut) < (12.5 + abs(time.timeAtIpInOutErr))"),
    filter = cms.bool(False)
)

import PhysicsTools.RecoAlgos.recoTrackSelector_cfi
staMuonsPt5 = PhysicsTools.RecoAlgos.recoTrackSelector_cfi.recoTrackSelector.clone()
staMuonsPt5.ptMin = cms.double(5.0)
staMuonsPt5.quality = cms.vstring('')
staMuonsPt5.minHit = cms.int32(0)
staMuonsPt5.src = cms.InputTag("standAloneMuons:UpdatedAtVtx")

bestMuonLoose = cms.EDProducer("MuonTrackProducer",
   muonsTag = cms.InputTag("muons"),
   inputDTRecSegment4DCollection = cms.InputTag("dt4DSegments"),
   inputCSCSegmentCollection = cms.InputTag("cscSegments"),
   vtxTag = cms.InputTag("selectedVertices"),
   #selectionTags = cms.vstring('AllGlobalMuons'),
   selectionTags = cms.vstring('All'),
   trackType = cms.string('globalTrackLoose')
)

bestMuonLoose5 = cms.EDProducer("MuonTrackProducer",
   muonsTag = cms.InputTag("muonPt5"),
   inputDTRecSegment4DCollection = cms.InputTag("dt4DSegments"),
   inputCSCSegmentCollection = cms.InputTag("cscSegments"),
   vtxTag = cms.InputTag("selectedVertices"),
   #selectionTags = cms.vstring('AllGlobalMuons'),
   selectionTags = cms.vstring('All'),
   trackType = cms.string('globalTrackLoose')
)

bestMuonTight = cms.EDProducer("MuonTrackProducer",
   muonsTag = cms.InputTag("muons"),
   inputDTRecSegment4DCollection = cms.InputTag("dt4DSegments"),
   inputCSCSegmentCollection = cms.InputTag("cscSegments"),
   vtxTag = cms.InputTag("selectedVertices"),
   #selectionTags = cms.vstring('AllGlobalMuons'),
   selectionTags = cms.vstring('All'),
   trackType = cms.string('globalTrackTight')
)

bestMuonTight5 = cms.EDProducer("MuonTrackProducer",
   muonsTag = cms.InputTag("muonPt5"),
   inputDTRecSegment4DCollection = cms.InputTag("dt4DSegments"),
   inputCSCSegmentCollection = cms.InputTag("cscSegments"),
   vtxTag = cms.InputTag("selectedVertices"),
   #selectionTags = cms.vstring('AllGlobalMuons'),
   selectionTags = cms.vstring('All'),
   trackType = cms.string('globalTrackTight')
)

#-----------------------------------------------------------------------------------------------------------------------

bestMuonLooseMod = cms.EDProducer("MuonTrackProducer",
   muonsTag = cms.InputTag("muons"),
   inputDTRecSegment4DCollection = cms.InputTag("dt4DSegments"),
   inputCSCSegmentCollection = cms.InputTag("cscSegments"),
   vtxTag = cms.InputTag("selectedVertices"),
   selectionTags = cms.vstring('All'),
   trackType = cms.string('globalTrackLooseMod')
)

bestMuonLooseMod5 = cms.EDProducer("MuonTrackProducer",
   muonsTag = cms.InputTag("muonPt5"),
   inputDTRecSegment4DCollection = cms.InputTag("dt4DSegments"),
   inputCSCSegmentCollection = cms.InputTag("cscSegments"),
   vtxTag = cms.InputTag("selectedVertices"),
   selectionTags = cms.vstring('All'),
   trackType = cms.string('globalTrackLooseMod')
)


bestMuonTightMod = cms.EDProducer("MuonTrackProducer",
   muonsTag = cms.InputTag("muons"),
   inputDTRecSegment4DCollection = cms.InputTag("dt4DSegments"),
   inputCSCSegmentCollection = cms.InputTag("cscSegments"),
   vtxTag = cms.InputTag("selectedVertices"),
   selectionTags = cms.vstring('All'),
   trackType = cms.string('globalTrackTightMod')
)

bestMuonTightMod5 = cms.EDProducer("MuonTrackProducer",
   muonsTag = cms.InputTag("muonPt5"),
   inputDTRecSegment4DCollection = cms.InputTag("dt4DSegments"),
   inputCSCSegmentCollection = cms.InputTag("cscSegments"),
   vtxTag = cms.InputTag("selectedVertices"),
   selectionTags = cms.vstring('All'),
   trackType = cms.string('globalTrackTightMod')
)

#-----------------------------------------------------------------------------------------------------------------------

me0MuonInd = cms.EDProducer("ME0MuonTrackCollProducer",
    me0MuonTag = cms.InputTag("me0SegmentMatching"),
    selectionTags = cms.vstring('All'),
)

muonColl_seq = cms.Sequence(
    #staMuonsPt5 *
    muon2StatTiming *
    muonPt5
)

bestMuon_seq = cms.Sequence(
    bestMuonLoose * bestMuonLoose5
    *bestMuonTight * bestMuonTight5
    *bestMuonLooseMod * bestMuonLooseMod5
    *bestMuonTightMod * bestMuonTightMod5
    #* me0MuonInd
)

import SimMuon.MCTruth.MuonTrackProducer_cfi
extractedGlobalMuons = SimMuon.MCTruth.MuonTrackProducer_cfi.muonTrackProducer.clone()
extractedGlobalMuons.selectionTags = ('AllGlobalMuons',)
extractedGlobalMuons.trackType = "globalTrack"
extractedSTAMuons2StatTiming = SimMuon.MCTruth.MuonTrackProducer_cfi.muonTrackProducer.clone()
extractedSTAMuons2StatTiming.selectionTags = ('AllStandAloneMuons',)
extractedSTAMuons2StatTiming.trackType = "outerTrack"
extractedSTAMuons2StatTiming.muonsTag = "muon2StatTiming"
extractedMuonTracks_seq = cms.Sequence( extractedGlobalMuons * extractedSTAMuons2StatTiming)

extractGemMuons = SimMuon.MCTruth.MuonTrackProducer_cfi.muonTrackProducer.clone()
extractGemMuons.selectionTags = ('All',)
extractGemMuons.trackType = "gemMuonTrack"
extractGemMuonsTracks_seq = cms.Sequence( extractGemMuons )

extractMe0Muons = SimMuon.MCTruth.MuonTrackProducer_cfi.muonTrackProducer.clone()
extractMe0Muons.selectionTags = cms.vstring('All',)
extractMe0Muons.trackType = "me0MuonTrack"
extractMe0MuonsTracks_seq = cms.Sequence( extractMe0Muons )

#
# Configuration for Seed track extractor
#

import SimMuon.MCTruth.SeedToTrackProducer_cfi
seedsOfSTAmuons = SimMuon.MCTruth.SeedToTrackProducer_cfi.SeedToTrackProducer.clone()
seedsOfSTAmuons.L2seedsCollection = cms.InputTag("ancientMuonSeed")
seedsOfSTAmuons_seq = cms.Sequence( seedsOfSTAmuons )

seedsOfDisplacedSTAmuons = SimMuon.MCTruth.SeedToTrackProducer_cfi.SeedToTrackProducer.clone()
seedsOfDisplacedSTAmuons.L2seedsCollection = cms.InputTag("displacedMuonSeeds")
seedsOfDisplacedSTAmuons_seq = cms.Sequence( seedsOfDisplacedSTAmuons )

# select probe tracks
import PhysicsTools.RecoAlgos.recoTrackSelector_cfi
probeTracks = PhysicsTools.RecoAlgos.recoTrackSelector_cfi.recoTrackSelector.clone()
probeTracks.quality = cms.vstring('highPurity')
probeTracks.tip = cms.double(3.5)
probeTracks.lip = cms.double(30.)
probeTracks.ptMin = cms.double(4.0)
probeTracks.minRapidity = cms.double(-2.4)
probeTracks.maxRapidity = cms.double(2.4)
probeTracks_seq = cms.Sequence( probeTracks )

#
# Associators for Full Sim + Reco:
#

tpToTkmuTrackAssociation = cms.EDProducer('TrackAssociatorEDProducer',
    associator = cms.InputTag('trackAssociatorByHits'),
    label_tp = cms.InputTag('mix', 'MergedTrackTruth'),
#    label_tr = cms.InputTag('generalTracks')
    label_tr = cms.InputTag('probeTracks')
)

tpToStaTrackAssociation = cms.EDProducer('TrackAssociatorEDProducer',
    associator = cms.InputTag('trackAssociatorByDeltaR'),
    label_tp = cms.InputTag('mix', 'MergedTrackTruth'),
    label_tr = cms.InputTag('standAloneMuons','')
)

tpToStaUpdTrackAssociation = cms.EDProducer('TrackAssociatorEDProducer',
    associator = cms.InputTag('trackAssociatorByDeltaR'),
    label_tp = cms.InputTag('mix', 'MergedTrackTruth'),
    label_tr = cms.InputTag('standAloneMuons','UpdatedAtVtx')
)

tpToGlbTrackAssociation = cms.EDProducer('TrackAssociatorEDProducer',
    associator = cms.InputTag('trackAssociatorByDeltaR'),
    label_tp = cms.InputTag('mix', 'MergedTrackTruth'),
    label_tr = cms.InputTag('extractedGlobalMuons')
)

tpToTevFirstTrackAssociation = cms.EDProducer('TrackAssociatorEDProducer',
    associator = cms.InputTag('trackAssociatorByDeltaR'),
    label_tp = cms.InputTag('mix', 'MergedTrackTruth'),
    label_tr = cms.InputTag('tevMuons','firstHit')
)

tpToTevPickyTrackAssociation = cms.EDProducer('TrackAssociatorEDProducer',
    associator = cms.InputTag('trackAssociatorByDeltaR'),
    label_tp = cms.InputTag('mix', 'MergedTrackTruth'),
    label_tr = cms.InputTag('tevMuons','picky')
)
tpToTevDytTrackAssociation = cms.EDProducer('TrackAssociatorEDProducer',
    associator = cms.InputTag('trackAssociatorByDeltaR'),
    label_tp = cms.InputTag('mix', 'MergedTrackTruth'),
    label_tr = cms.InputTag('tevMuons','dyt')
)

tpToL2TrackAssociation = cms.EDProducer('TrackAssociatorEDProducer',
    ignoremissingtrackcollection=cms.untracked.bool(True),
    associator = cms.InputTag('trackAssociatorByDeltaR'),
    label_tp = cms.InputTag('mix', 'MergedTrackTruth'),
    label_tr = cms.InputTag('hltL2Muons','')
)

tpToL2UpdTrackAssociation = cms.EDProducer('TrackAssociatorEDProducer',
    ignoremissingtrackcollection=cms.untracked.bool(True),
    associator = cms.InputTag('trackAssociatorByDeltaR'),
    label_tp = cms.InputTag('mix', 'MergedTrackTruth'),
    label_tr = cms.InputTag('hltL2Muons','UpdatedAtVtx')
)

tpToL3TrackAssociation = cms.EDProducer("TrackAssociatorEDProducer",
    ignoremissingtrackcollection=cms.untracked.bool(True),
    associator = cms.InputTag('trackAssociatorByDeltaR'),
    label_tp = cms.InputTag('mix', 'MergedTrackTruth'),
    label_tr = cms.InputTag('hltL3Muons')
)

tpToL3TkTrackTrackAssociation = cms.EDProducer("TrackAssociatorEDProducer",
    ignoremissingtrackcollection=cms.untracked.bool(True),
    associator = cms.string('onlineTrackAssociatorByHits'),
    label_tp = cms.InputTag('mix','MergedTrackTruth'),
    label_tr = cms.InputTag('hltL3TkTracksFromL2','')
)

tpToL3L2TrackTrackAssociation = cms.EDProducer("TrackAssociatorEDProducer",
    ignoremissingtrackcollection=cms.untracked.bool(True),
    associator = cms.string('onlineTrackAssociatorByHits'),
    label_tp = cms.InputTag('mix','MergedTrackTruth'),
    label_tr = cms.InputTag('hltL3Muons:L2Seeded')
)



#MuonAssociation
import SimMuon.MCTruth.MuonAssociatorByHits_cfi

tpToTkMuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToStaSeedAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToStaMuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToStaUpdMuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToGlbMuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToStaRefitMuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToStaRefitUpdMuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToDisplacedTrkMuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToDisplacedStaSeedAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToDisplacedStaMuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToDisplacedGlbMuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToTevFirstMuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToTevPickyMuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToTevDytMuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToL3TkMuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToL2MuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToL2UpdMuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToL3MuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToME0MuonMuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToGEMMuonMuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()

tpToStaUpdMuonAssociation2StTime = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToLooseMuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToLoose5MuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToTightMuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToTight5MuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()

tpToTkSelMuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToStaUpdSelMuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToStaUpdSel2MuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()

tpToStaUpdSelMuonAssociation2StTime = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToStaUpdSel2MuonAssociation2StTime = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()

tpToStaUpd10SelMuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToStaUpd20SelMuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()

tpToGlbSelMuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToGlbSel2MuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToGlbSel3MuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToGlbSel4MuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()

tpToLooseSelMuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToLooseSel2MuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToLooseSel3MuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToLooseSel35MuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToLooseSel0MuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToLooseSelUncMuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToLooseSel05MuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToLooseSel4MuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToLooseSel45MuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()

tpToTightSelMuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToTightSel2MuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToTightSel3MuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToTightSel35MuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToTightSel0MuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToTightSelUncMuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToTightSel05MuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToTightSel4MuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToTightSel45MuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()

tpToLooseModSel0MuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToLooseModSelUncMuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToLooseModSel05MuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()

tpToTightModSel0MuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToTightModSelUncMuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToTightModSel05MuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()

tpToStaRefitMuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()

tpToTkMuonAssociation.tpTag = 'mix:MergedTrackTruth'
#tpToTkMuonAssociation.tracksTag = 'generalTracks'
tpToTkMuonAssociation.tracksTag = 'probeTracks'
tpToTkMuonAssociation.UseTracker = True
tpToTkMuonAssociation.UseMuon = False

tpToStaSeedAssociation.tpTag = 'mix:MergedTrackTruth'
tpToStaSeedAssociation.tracksTag = 'seedsOfSTAmuons'
tpToStaSeedAssociation.UseTracker = False
tpToStaSeedAssociation.UseMuon = True

tpToStaMuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToStaMuonAssociation.tracksTag = 'standAloneMuons'
tpToStaMuonAssociation.UseTracker = False
tpToStaMuonAssociation.UseMuon = True

tpToStaUpdMuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToStaUpdMuonAssociation.tracksTag = 'standAloneMuons:UpdatedAtVtx'
tpToStaUpdMuonAssociation.UseTracker = False
tpToStaUpdMuonAssociation.UseMuon = True

tpToStaUpdMuonAssociation2StTime.tpTag = 'mix:MergedTrackTruth'
tpToStaUpdMuonAssociation2StTime.tracksTag = 'extractedSTAMuons2StatTiming'
tpToStaUpdMuonAssociation2StTime.UseTracker = False
tpToStaUpdMuonAssociation2StTime.UseMuon = True

tpToGlbMuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToGlbMuonAssociation.tracksTag = 'extractedGlobalMuons'
tpToGlbMuonAssociation.UseTracker = True
tpToGlbMuonAssociation.UseMuon = True

tpToLooseMuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToLooseMuonAssociation.tracksTag = 'bestMuonLoose'
tpToLooseMuonAssociation.UseTracker = True
tpToLooseMuonAssociation.UseMuon = True

tpToLoose5MuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToLoose5MuonAssociation.tracksTag = 'bestMuonLoose5'
tpToLoose5MuonAssociation.UseTracker = True
tpToLoose5MuonAssociation.UseMuon = True

tpToTightMuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToTightMuonAssociation.tracksTag = 'bestMuonTight'
tpToTightMuonAssociation.UseTracker = True
tpToTightMuonAssociation.UseMuon = True

tpToTight5MuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToTight5MuonAssociation.tracksTag = 'bestMuonTight5'
tpToTight5MuonAssociation.UseTracker = True
tpToTight5MuonAssociation.UseMuon = True

#######################################################################

tpToStaRefitMuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToStaRefitMuonAssociation.tracksTag = 'refittedStandAloneMuons'
tpToStaRefitMuonAssociation.UseTracker = False
tpToStaRefitMuonAssociation.UseMuon = True

tpToStaRefitUpdMuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToStaRefitUpdMuonAssociation.tracksTag = 'refittedStandAloneMuons:UpdatedAtVtx'
tpToStaRefitUpdMuonAssociation.UseTracker = False
tpToStaRefitUpdMuonAssociation.UseMuon = True

tpToDisplacedTrkMuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToDisplacedTrkMuonAssociation.tracksTag = 'displacedTracks'
tpToDisplacedTrkMuonAssociation.UseTracker = True
tpToDisplacedTrkMuonAssociation.UseMuon = False

tpToDisplacedStaSeedAssociation.tpTag = 'mix:MergedTrackTruth'
tpToDisplacedStaSeedAssociation.tracksTag = 'seedsOfDisplacedSTAmuons'
tpToDisplacedStaSeedAssociation.UseTracker = False
tpToDisplacedStaSeedAssociation.UseMuon = True

tpToDisplacedStaMuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToDisplacedStaMuonAssociation.tracksTag = 'displacedStandAloneMuons'
tpToDisplacedStaMuonAssociation.UseTracker = False
tpToDisplacedStaMuonAssociation.UseMuon = True

tpToDisplacedGlbMuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToDisplacedGlbMuonAssociation.tracksTag = 'displacedGlobalMuons'
tpToDisplacedGlbMuonAssociation.UseTracker = True
tpToDisplacedGlbMuonAssociation.UseMuon = True

tpToTevFirstMuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToTevFirstMuonAssociation.tracksTag = 'tevMuons:firstHit'
tpToTevFirstMuonAssociation.UseTracker = True
tpToTevFirstMuonAssociation.UseMuon = True

tpToTevPickyMuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToTevPickyMuonAssociation.tracksTag = 'tevMuons:picky'
tpToTevPickyMuonAssociation.UseTracker = True
tpToTevPickyMuonAssociation.UseMuon = True

tpToTevDytMuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToTevDytMuonAssociation.tracksTag = 'tevMuons:dyt'
tpToTevDytMuonAssociation.UseTracker = True
tpToTevDytMuonAssociation.UseMuon = True

tpToL3TkMuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToL3TkMuonAssociation.tracksTag = 'hltL3TkTracksFromL2'
tpToL3TkMuonAssociation.DTrechitTag = 'hltDt1DRecHits'
tpToL3TkMuonAssociation.UseTracker = True
tpToL3TkMuonAssociation.UseMuon = False
tpToL3TkMuonAssociation.ignoreMissingTrackCollection = True
tpToL3TkMuonAssociation.UseSplitting = False
tpToL3TkMuonAssociation.UseGrouped = False

tpToL2MuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToL2MuonAssociation.tracksTag = 'hltL2Muons'
tpToL2MuonAssociation.DTrechitTag = 'hltDt1DRecHits'
tpToL2MuonAssociation.UseTracker = False
tpToL2MuonAssociation.UseMuon = True
tpToL2MuonAssociation.ignoreMissingTrackCollection = True

tpToL2UpdMuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToL2UpdMuonAssociation.tracksTag = 'hltL2Muons:UpdatedAtVtx'
tpToL2UpdMuonAssociation.DTrechitTag = 'hltDt1DRecHits'
tpToL2UpdMuonAssociation.UseTracker = False
tpToL2UpdMuonAssociation.UseMuon = True
tpToL2UpdMuonAssociation.ignoreMissingTrackCollection = True

tpToL3MuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToL3MuonAssociation.tracksTag = 'hltL3Muons'
tpToL3MuonAssociation.DTrechitTag = 'hltDt1DRecHits'
tpToL3MuonAssociation.UseTracker = True
tpToL3MuonAssociation.UseMuon = True
tpToL3MuonAssociation.ignoreMissingTrackCollection = True
tpToL3MuonAssociation.UseSplitting = False
tpToL3MuonAssociation.UseGrouped = False

tpToME0MuonMuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToME0MuonMuonAssociation.tracksTag = 'extractMe0Muons'
tpToME0MuonMuonAssociation.UseTracker = True
tpToME0MuonMuonAssociation.UseMuon = False

tpToGEMMuonMuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToGEMMuonMuonAssociation.tracksTag = 'extractGemMuons'
tpToGEMMuonMuonAssociation.UseTracker = True
tpToGEMMuonMuonAssociation.UseMuon = False

# few more association modules usable for the Upgrade TP/SD/TDR studies
# about STA

tpToTkSelMuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToTkSelMuonAssociation.tracksTag = 'probeTracks'
tpToTkSelMuonAssociation.UseTracker = True
tpToTkSelMuonAssociation.UseMuon = False
tpToTkSelMuonAssociation.EfficiencyCut_track = cms.double(0.5)
tpToTkSelMuonAssociation.PurityCut_track = cms.double(0.75)

tpToStaUpdSelMuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToStaUpdSelMuonAssociation.tracksTag = 'standAloneMuons:UpdatedAtVtx' 
tpToStaUpdSelMuonAssociation.UseTracker = False
tpToStaUpdSelMuonAssociation.UseMuon = True
tpToStaUpdSelMuonAssociation.includeZeroHitMuons = False

tpToStaUpdSelMuonAssociation2StTime.tpTag = 'mix:MergedTrackTruth'
tpToStaUpdSelMuonAssociation2StTime.tracksTag = 'extractedSTAMuons2StatTiming' 
tpToStaUpdSelMuonAssociation2StTime.UseTracker = False
tpToStaUpdSelMuonAssociation2StTime.UseMuon = True
tpToStaUpdSelMuonAssociation2StTime.includeZeroHitMuons = False

tpToStaUpdSel2MuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToStaUpdSel2MuonAssociation.tracksTag = 'standAloneMuons:UpdatedAtVtx' 
tpToStaUpdSel2MuonAssociation.UseTracker = False
tpToStaUpdSel2MuonAssociation.UseMuon = True
tpToStaUpdSel2MuonAssociation.includeZeroHitMuons = False
tpToStaUpdSel2MuonAssociation.PurityCut_muon = cms.double(0.75)

tpToStaUpdSel2MuonAssociation2StTime.tpTag = 'mix:MergedTrackTruth'
tpToStaUpdSel2MuonAssociation2StTime.tracksTag = 'extractedSTAMuons2StatTiming' 
tpToStaUpdSel2MuonAssociation2StTime.UseTracker = False
tpToStaUpdSel2MuonAssociation2StTime.UseMuon = True
tpToStaUpdSel2MuonAssociation2StTime.includeZeroHitMuons = False
tpToStaUpdSel2MuonAssociation2StTime.PurityCut_muon = cms.double(0.75)

# about global

tpToGlbSelMuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToGlbSelMuonAssociation.tracksTag = 'extractedGlobalMuons' 
tpToGlbSelMuonAssociation.UseTracker = True
tpToGlbSelMuonAssociation.UseMuon = True
tpToGlbSelMuonAssociation.EfficiencyCut_track = cms.double(0.5)
tpToGlbSelMuonAssociation.PurityCut_track = cms.double(0.75)
tpToGlbSelMuonAssociation.acceptOneStubMatchings = False
tpToGlbSelMuonAssociation.includeZeroHitMuons = False

tpToGlbSel2MuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToGlbSel2MuonAssociation.tracksTag = 'extractedGlobalMuons' 
tpToGlbSel2MuonAssociation.UseTracker = True
tpToGlbSel2MuonAssociation.UseMuon = True
tpToGlbSel2MuonAssociation.EfficiencyCut_track = cms.double(0.5)
tpToGlbSel2MuonAssociation.PurityCut_track = cms.double(0.75)
#tpToGlbSel2MuonAssociation.acceptOneStubMatchings = False
tpToGlbSel2MuonAssociation.includeZeroHitMuons = False

tpToGlbSel3MuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToGlbSel3MuonAssociation.tracksTag = 'extractedGlobalMuons' 
tpToGlbSel3MuonAssociation.UseTracker = True
tpToGlbSel3MuonAssociation.UseMuon = True
tpToGlbSel3MuonAssociation.EfficiencyCut_track = cms.double(0.5)
tpToGlbSel3MuonAssociation.PurityCut_track = cms.double(0.75)
tpToGlbSel3MuonAssociation.PurityCut_muon = cms.double(0.75)
tpToGlbSel3MuonAssociation.acceptOneStubMatchings = False
tpToGlbSel3MuonAssociation.includeZeroHitMuons = False

tpToGlbSel4MuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToGlbSel4MuonAssociation.tracksTag = 'extractedGlobalMuons' 
tpToGlbSel4MuonAssociation.UseTracker = True
tpToGlbSel4MuonAssociation.UseMuon = True
tpToGlbSel4MuonAssociation.EfficiencyCut_track = cms.double(0.5)
tpToGlbSel4MuonAssociation.PurityCut_track = cms.double(0.75)
tpToGlbSel4MuonAssociation.PurityCut_muon = cms.double(0.75)
#tpToGlbSel4MuonAssociation.acceptOneStubMatchings = False
tpToGlbSel4MuonAssociation.includeZeroHitMuons = False

#####################################################################################################

tpToLooseSelMuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToLooseSelMuonAssociation.tracksTag = 'bestMuonLoose' 
tpToLooseSelMuonAssociation.UseTracker = True
tpToLooseSelMuonAssociation.UseMuon = True
tpToLooseSelMuonAssociation.EfficiencyCut_track = cms.double(0.5)
tpToLooseSelMuonAssociation.PurityCut_track = cms.double(0.75)
tpToLooseSelMuonAssociation.acceptOneStubMatchings = False
tpToLooseSelMuonAssociation.includeZeroHitMuons = False

tpToLooseSel0MuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToLooseSel0MuonAssociation.tracksTag = 'bestMuonLoose' 
tpToLooseSel0MuonAssociation.UseTracker = True
tpToLooseSel0MuonAssociation.UseMuon = True
#tpToLooseSel0MuonAssociation.EfficiencyCut_track = cms.double(0.5)
#tpToLooseSel0MuonAssociation.PurityCut_track = cms.double(0.75)
#tpToLooseSel0MuonAssociation.EfficiencyCut_muon = cms.double(0.5)
#tpToLooseSel0MuonAssociation.PurityCut_muon = cms.double(0.75)
tpToLooseSel0MuonAssociation.acceptOneStubMatchings = False
#tpToLooseSel0MuonAssociation.includeZeroHitMuons = False

tpToLooseSelUncMuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToLooseSelUncMuonAssociation.tracksTag = 'bestMuonLoose' 
tpToLooseSelUncMuonAssociation.UseTracker = False
tpToLooseSelUncMuonAssociation.UseMuon = True
#tpToLooseSelUncMuonAssociation.EfficiencyCut_track = cms.double(0.5)
#tpToLooseSelUncMuonAssociation.PurityCut_track = cms.double(0.75)
#tpToLooseSelUncMuonAssociation.EfficiencyCut_muon = cms.double(0.5)
#tpToLooseSelUncMuonAssociation.PurityCut_muon = cms.double(0.75)
tpToLooseSelUncMuonAssociation.acceptOneStubMatchings = False
#tpToLooseSelUncMuonAssociation.includeZeroHitMuons = False

tpToLooseSel05MuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToLooseSel05MuonAssociation.tracksTag = 'bestMuonLoose5' 
tpToLooseSel05MuonAssociation.UseTracker = True
tpToLooseSel05MuonAssociation.UseMuon = True
#tpToLooseSel05MuonAssociation.EfficiencyCut_track = cms.double(0.5)
#tpToLooseSel05MuonAssociation.PurityCut_track = cms.double(0.75)
#tpToLooseSel05MuonAssociation.EfficiencyCut_muon = cms.double(0.5)
#tpToLooseSel05MuonAssociation.PurityCut_muon = cms.double(0.75)
tpToLooseSel05MuonAssociation.acceptOneStubMatchings = False
#tpToLooseSel05MuonAssociation.includeZeroHitMuons = False

tpToLooseSel3MuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToLooseSel3MuonAssociation.tracksTag = 'bestMuonLoose' 
tpToLooseSel3MuonAssociation.UseTracker = True
tpToLooseSel3MuonAssociation.UseMuon = True
tpToLooseSel3MuonAssociation.EfficiencyCut_track = cms.double(0.5)
tpToLooseSel3MuonAssociation.PurityCut_track = cms.double(0.75)
tpToLooseSel3MuonAssociation.EfficiencyCut_muon = cms.double(0.5)
tpToLooseSel3MuonAssociation.PurityCut_muon = cms.double(0.75)
tpToLooseSel3MuonAssociation.acceptOneStubMatchings = False
tpToLooseSel3MuonAssociation.includeZeroHitMuons = False

tpToLooseSel35MuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToLooseSel35MuonAssociation.tracksTag = 'bestMuonLoose5' 
tpToLooseSel35MuonAssociation.UseTracker = True
tpToLooseSel35MuonAssociation.UseMuon = True
tpToLooseSel35MuonAssociation.EfficiencyCut_track = cms.double(0.5)
tpToLooseSel35MuonAssociation.PurityCut_track = cms.double(0.75)
tpToLooseSel35MuonAssociation.EfficiencyCut_muon = cms.double(0.5)
tpToLooseSel35MuonAssociation.PurityCut_muon = cms.double(0.75)
tpToLooseSel35MuonAssociation.acceptOneStubMatchings = False
tpToLooseSel35MuonAssociation.includeZeroHitMuons = False

tpToLooseSel2MuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToLooseSel2MuonAssociation.tracksTag = 'bestMuonLoose' 
tpToLooseSel2MuonAssociation.UseTracker = True
tpToLooseSel2MuonAssociation.UseMuon = True
tpToLooseSel2MuonAssociation.EfficiencyCut_track = cms.double(0.5)
tpToLooseSel2MuonAssociation.PurityCut_track = cms.double(0.75)
#tpToLooseSel2MuonAssociation.acceptOneStubMatchings = False
tpToLooseSel2MuonAssociation.includeZeroHitMuons = False

tpToLooseSel4MuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToLooseSel4MuonAssociation.tracksTag = 'bestMuonLoose' 
tpToLooseSel4MuonAssociation.UseTracker = True
tpToLooseSel4MuonAssociation.UseMuon = True
#tpToLooseSel4MuonAssociation.EfficiencyCut_track = cms.double(0.5)
#tpToLooseSel4MuonAssociation.PurityCut_track = cms.double(0.75)
#tpToLooseSel4MuonAssociation.EfficiencyCut_muon = cms.double(0.5)
#tpToLooseSel4MuonAssociation.PurityCut_muon = cms.double(0.75)
#tpToLooseSel4MuonAssociation.acceptOneStubMatchings = False
#tpToLooseSel4MuonAssociation.includeZeroHitMuons = False

tpToLooseSel45MuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToLooseSel45MuonAssociation.tracksTag = 'bestMuonLoose5' 
tpToLooseSel45MuonAssociation.UseTracker = True
tpToLooseSel45MuonAssociation.UseMuon = True
#tpToLooseSel45MuonAssociation.EfficiencyCut_track = cms.double(0.5)
#tpToLooseSel45MuonAssociation.PurityCut_track = cms.double(0.75)
#tpToLooseSel45MuonAssociation.EfficiencyCut_muon = cms.double(0.5)
#tpToLooseSel45MuonAssociation.PurityCut_muon = cms.double(0.75)
#tpToLooseSel45MuonAssociation.acceptOneStubMatchings = False
#tpToLooseSel45MuonAssociation.includeZeroHitMuons = False

#####################################################################################################

tpToTightSelMuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToTightSelMuonAssociation.tracksTag = 'bestMuonTight' 
tpToTightSelMuonAssociation.UseTracker = True
tpToTightSelMuonAssociation.UseMuon = True
tpToTightSelMuonAssociation.EfficiencyCut_track = cms.double(0.5)
tpToTightSelMuonAssociation.PurityCut_track = cms.double(0.75)
tpToTightSelMuonAssociation.acceptOneStubMatchings = False
tpToTightSelMuonAssociation.includeZeroHitMuons = False

tpToTightSel0MuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToTightSel0MuonAssociation.tracksTag = 'bestMuonTight' 
tpToTightSel0MuonAssociation.UseTracker = True
tpToTightSel0MuonAssociation.UseMuon = True
#tpToTightSel0MuonAssociation.EfficiencyCut_track = cms.double(0.5)
#tpToTightSel0MuonAssociation.PurityCut_track = cms.double(0.75)
#tpToTightSel0MuonAssociation.EfficiencyCut_muon = cms.double(0.5)
#tpToTightSel0MuonAssociation.PurityCut_muon = cms.double(0.75)
tpToTightSel0MuonAssociation.acceptOneStubMatchings = False
#tpToTightSel0MuonAssociation.includeZeroHitMuons = False

tpToTightSelUncMuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToTightSelUncMuonAssociation.tracksTag = 'bestMuonTight' 
tpToTightSelUncMuonAssociation.UseTracker = False
tpToTightSelUncMuonAssociation.UseMuon = True
#tpToTightSelUncMuonAssociation.EfficiencyCut_track = cms.double(0.5)
#tpToTightSelUncMuonAssociation.PurityCut_track = cms.double(0.75)
#tpToTightSelUncMuonAssociation.EfficiencyCut_muon = cms.double(0.5)
#tpToTightSelUncMuonAssociation.PurityCut_muon = cms.double(0.75)
tpToTightSelUncMuonAssociation.acceptOneStubMatchings = False
#tpToTightSelUncMuonAssociation.includeZeroHitMuons = False

tpToTightSel05MuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToTightSel05MuonAssociation.tracksTag = 'bestMuonTight5' 
tpToTightSel05MuonAssociation.UseTracker = True
tpToTightSel05MuonAssociation.UseMuon = True
#tpToTightSel05MuonAssociation.EfficiencyCut_track = cms.double(0.5)
#tpToTightSel05MuonAssociation.PurityCut_track = cms.double(0.75)
#tpToTightSel05MuonAssociation.EfficiencyCut_muon = cms.double(0.5)
#tpToTightSel05MuonAssociation.PurityCut_muon = cms.double(0.75)
tpToTightSel05MuonAssociation.acceptOneStubMatchings = False
#tpToTightSel05MuonAssociation.includeZeroHitMuons = False

tpToTightSel3MuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToTightSel3MuonAssociation.tracksTag = 'bestMuonTight' 
tpToTightSel3MuonAssociation.UseTracker = True
tpToTightSel3MuonAssociation.UseMuon = True
tpToTightSel3MuonAssociation.EfficiencyCut_track = cms.double(0.5)
tpToTightSel3MuonAssociation.PurityCut_track = cms.double(0.75)
#tpToTightSel3MuonAssociation.EfficiencyCut_muon = cms.double(0.5)
tpToTightSel3MuonAssociation.PurityCut_muon = cms.double(0.75)
tpToTightSel3MuonAssociation.acceptOneStubMatchings = False
tpToTightSel3MuonAssociation.includeZeroHitMuons = False

tpToTightSel35MuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToTightSel35MuonAssociation.tracksTag = 'bestMuonTight5' 
tpToTightSel35MuonAssociation.UseTracker = True
tpToTightSel35MuonAssociation.UseMuon = True
tpToTightSel35MuonAssociation.EfficiencyCut_track = cms.double(0.5)
tpToTightSel35MuonAssociation.PurityCut_track = cms.double(0.75)
#tpToTightSel35MuonAssociation.EfficiencyCut_muon = cms.double(0.5)
tpToTightSel35MuonAssociation.PurityCut_muon = cms.double(0.75)
tpToTightSel35MuonAssociation.acceptOneStubMatchings = False
tpToTightSel35MuonAssociation.includeZeroHitMuons = False

tpToTightSel2MuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToTightSel2MuonAssociation.tracksTag = 'bestMuonTight' 
tpToTightSel2MuonAssociation.UseTracker = True
tpToTightSel2MuonAssociation.UseMuon = True
tpToTightSel2MuonAssociation.EfficiencyCut_track = cms.double(0.5)
tpToTightSel2MuonAssociation.PurityCut_track = cms.double(0.75)
#tpToTightSel2MuonAssociation.acceptOneStubMatchings = False
tpToTightSel2MuonAssociation.includeZeroHitMuons = False

tpToTightSel4MuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToTightSel4MuonAssociation.tracksTag = 'bestMuonTight' 
tpToTightSel4MuonAssociation.UseTracker = True
tpToTightSel4MuonAssociation.UseMuon = True
#tpToTightSel4MuonAssociation.EfficiencyCut_track = cms.double(0.5)
#tpToTightSel4MuonAssociation.PurityCut_track = cms.double(0.75)
#tpToTightSel4MuonAssociation.EfficiencyCut_muon = cms.double(0.5)
#tpToTightSel4MuonAssociation.PurityCut_muon = cms.double(0.75)
#tpToTightSel4MuonAssociation.acceptOneStubMatchings = False
#tpToTightSel4MuonAssociation.includeZeroHitMuons = False

tpToTightSel45MuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToTightSel45MuonAssociation.tracksTag = 'bestMuonTight5' 
tpToTightSel45MuonAssociation.UseTracker = True
tpToTightSel45MuonAssociation.UseMuon = True
#tpToTightSel45MuonAssociation.EfficiencyCut_track = cms.double(0.5)
#tpToTightSel45MuonAssociation.PurityCut_track = cms.double(0.75)
#tpToTightSel45MuonAssociation.EfficiencyCut_muon = cms.double(0.5)
#tpToTightSel45MuonAssociation.PurityCut_muon = cms.double(0.75)
#tpToTightSel45MuonAssociation.acceptOneStubMatchings = False
#tpToTightSel45MuonAssociation.includeZeroHitMuons = False

#####################################################################################################

tpToLooseModSel0MuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToLooseModSel0MuonAssociation.tracksTag = 'bestMuonLooseMod'
tpToLooseModSel0MuonAssociation.UseTracker = True
tpToLooseModSel0MuonAssociation.UseMuon = True
#tpToLooseModSel0MuonAssociation.EfficiencyCut_track = cms.double(0.5)
#tpToLooseModSel0MuonAssociation.PurityCut_track = cms.double(0.75)
#tpToLooseModSel0MuonAssociation.EfficiencyCut_muon = cms.double(0.5)
#tpToLooseModSel0MuonAssociation.PurityCut_muon = cms.double(0.75)
tpToLooseModSel0MuonAssociation.acceptOneStubMatchings = False
#tpToLooseModSel0MuonAssociation.includeZeroHitMuons = False

tpToLooseModSelUncMuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToLooseModSelUncMuonAssociation.tracksTag = 'bestMuonLooseMod'
tpToLooseModSelUncMuonAssociation.UseTracker = False
tpToLooseModSelUncMuonAssociation.UseMuon = True
#tpToLooseModSelUncMuonAssociation.EfficiencyCut_track = cms.double(0.5)
#tpToLooseModSelUncMuonAssociation.PurityCut_track = cms.double(0.75)
#tpToLooseModSelUncMuonAssociation.EfficiencyCut_muon = cms.double(0.5)
#tpToLooseModSelUncMuonAssociation.PurityCut_muon = cms.double(0.75)
tpToLooseModSelUncMuonAssociation.acceptOneStubMatchings = False
#tpToLooseModSelUncMuonAssociation.includeZeroHitMuons = False

tpToLooseModSel05MuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToLooseModSel05MuonAssociation.tracksTag = 'bestMuonLooseMod5'
tpToLooseModSel05MuonAssociation.UseTracker = True
tpToLooseModSel05MuonAssociation.UseMuon = True
#tpToLooseModSel05MuonAssociation.EfficiencyCut_track = cms.double(0.5)
#tpToLooseModSel05MuonAssociation.PurityCut_track = cms.double(0.75)
#tpToLooseModSel05MuonAssociation.EfficiencyCut_muon = cms.double(0.5)
#tpToLooseModSel05MuonAssociation.PurityCut_muon = cms.double(0.75)
tpToLooseModSel05MuonAssociation.acceptOneStubMatchings = False
#tpToLooseModSel05MuonAssociation.includeZeroHitMuons = False

#####################################################################################################

tpToTightModSel0MuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToTightModSel0MuonAssociation.tracksTag = 'bestMuonTightMod'
tpToTightModSel0MuonAssociation.UseTracker = True
tpToTightModSel0MuonAssociation.UseMuon = True
#tpToTightModSel0MuonAssociation.EfficiencyCut_track = cms.double(0.5)
#tpToTightModSel0MuonAssociation.PurityCut_track = cms.double(0.75)
#tpToTightModSel0MuonAssociation.EfficiencyCut_muon = cms.double(0.5)
#tpToTightModSel0MuonAssociation.PurityCut_muon = cms.double(0.75)
tpToTightModSel0MuonAssociation.acceptOneStubMatchings = False
#tpToTightModSel0MuonAssociation.includeZeroHitMuons = False

tpToTightModSelUncMuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToTightModSelUncMuonAssociation.tracksTag = 'bestMuonTightMod'
tpToTightModSelUncMuonAssociation.UseTracker = False
tpToTightModSelUncMuonAssociation.UseMuon = True
#tpToTightModSelUncMuonAssociation.EfficiencyCut_track = cms.double(0.5)
#tpToTightModSelUncMuonAssociation.PurityCut_track = cms.double(0.75)
#tpToTightModSelUncMuonAssociation.EfficiencyCut_muon = cms.double(0.5)
#tpToTightModSelUncMuonAssociation.PurityCut_muon = cms.double(0.75)
tpToTightModSelUncMuonAssociation.acceptOneStubMatchings = False
#tpToTightModSelUncMuonAssociation.includeZeroHitMuons = False

tpToTightModSel05MuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToTightModSel05MuonAssociation.tracksTag = 'bestMuonTightMod5'
tpToTightModSel05MuonAssociation.UseTracker = True
tpToTightModSel05MuonAssociation.UseMuon = True
#tpToTightModSel05MuonAssociation.EfficiencyCut_track = cms.double(0.5)
#tpToTightModSel05MuonAssociation.PurityCut_track = cms.double(0.75)
#tpToTightModSel05MuonAssociation.EfficiencyCut_muon = cms.double(0.5)
#tpToTightModSel05MuonAssociation.PurityCut_muon = cms.double(0.75)
tpToTightModSel05MuonAssociation.acceptOneStubMatchings = False
#tpToTightModSel05MuonAssociation.includeZeroHitMuons = False

#####################################################################################################

#
# Associators for cosmics:
#

tpToTkCosmicTrackAssociation = cms.EDProducer('TrackAssociatorEDProducer',
    associator = cms.InputTag('trackAssociatorByHits'),
    label_tp = cms.InputTag('mix', 'MergedTrackTruth'),
    label_tr = cms.InputTag('ctfWithMaterialTracksP5LHCNavigation')
)

tpToStaCosmicTrackAssociation = cms.EDProducer('TrackAssociatorEDProducer',
    associator = cms.InputTag('trackAssociatorByDeltaR'),
    label_tp = cms.InputTag('mix', 'MergedTrackTruth'),
    label_tr = cms.InputTag('cosmicMuons')
)

tpToGlbCosmicTrackAssociation = cms.EDProducer('TrackAssociatorEDProducer',
    associator = cms.InputTag('trackAssociatorByDeltaR'),
    label_tp = cms.InputTag('mix', 'MergedTrackTruth'),
    label_tr = cms.InputTag('globalCosmicMuons')
)

# 2-legs cosmics reco: simhits can be twice the reconstructed ones in any single leg
# (Quality cut have to be set at 0.25, purity cuts can stay at 0.75)
# T.B.D. upper and lower leg should be analyzed separately 
tpToTkCosmicSelMuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToStaCosmicSelMuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToGlbCosmicSelMuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
# 1-leg cosmics reco
tpToTkCosmic1LegSelMuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToStaCosmic1LegSelMuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()
tpToGlbCosmic1LegSelMuonAssociation = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.clone()

tpToTkCosmicSelMuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToTkCosmicSelMuonAssociation.tracksTag = 'ctfWithMaterialTracksP5LHCNavigation'
tpToTkCosmicSelMuonAssociation.UseTracker = True
tpToTkCosmicSelMuonAssociation.UseMuon = False
tpToTkCosmicSelMuonAssociation.EfficiencyCut_track = cms.double(0.25)
tpToTkCosmicSelMuonAssociation.PurityCut_track = cms.double(0.75)

tpToStaCosmicSelMuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToStaCosmicSelMuonAssociation.tracksTag = 'cosmicMuons'
tpToStaCosmicSelMuonAssociation.UseTracker = False
tpToStaCosmicSelMuonAssociation.UseMuon = True
tpToStaCosmicSelMuonAssociation.includeZeroHitMuons = False
tpToStaCosmicSelMuonAssociation.EfficiencyCut_muon = cms.double(0.25)
tpToStaCosmicSelMuonAssociation.PurityCut_muon = cms.double(0.75)

tpToGlbCosmicSelMuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToGlbCosmicSelMuonAssociation.tracksTag = 'globalCosmicMuons'
tpToGlbCosmicSelMuonAssociation.UseTracker = True
tpToGlbCosmicSelMuonAssociation.UseMuon = True
tpToGlbCosmicSelMuonAssociation.EfficiencyCut_track = cms.double(0.25)
tpToGlbCosmicSelMuonAssociation.PurityCut_track = cms.double(0.75)
tpToGlbCosmicSelMuonAssociation.EfficiencyCut_muon = cms.double(0.25)
tpToGlbCosmicSelMuonAssociation.PurityCut_muon = cms.double(0.75)
tpToGlbCosmicSelMuonAssociation.acceptOneStubMatchings = False
tpToGlbCosmicSelMuonAssociation.includeZeroHitMuons = False

tpToTkCosmic1LegSelMuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToTkCosmic1LegSelMuonAssociation.tracksTag = 'ctfWithMaterialTracksP5'
tpToTkCosmic1LegSelMuonAssociation.UseTracker = True
tpToTkCosmic1LegSelMuonAssociation.UseMuon = False
tpToTkCosmic1LegSelMuonAssociation.EfficiencyCut_track = cms.double(0.5)
tpToTkCosmic1LegSelMuonAssociation.PurityCut_track = cms.double(0.75)

tpToStaCosmic1LegSelMuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToStaCosmic1LegSelMuonAssociation.tracksTag = 'cosmicMuons1Leg'
tpToStaCosmic1LegSelMuonAssociation.UseTracker = False
tpToStaCosmic1LegSelMuonAssociation.UseMuon = True
tpToStaCosmic1LegSelMuonAssociation.includeZeroHitMuons = False
tpToStaCosmic1LegSelMuonAssociation.EfficiencyCut_muon = cms.double(0.5)
tpToStaCosmic1LegSelMuonAssociation.PurityCut_muon = cms.double(0.75)

tpToGlbCosmic1LegSelMuonAssociation.tpTag = 'mix:MergedTrackTruth'
tpToGlbCosmic1LegSelMuonAssociation.tracksTag = 'globalCosmicMuons1Leg'
tpToGlbCosmic1LegSelMuonAssociation.UseTracker = True
tpToGlbCosmic1LegSelMuonAssociation.UseMuon = True
tpToGlbCosmic1LegSelMuonAssociation.EfficiencyCut_track = cms.double(0.5)
tpToGlbCosmic1LegSelMuonAssociation.PurityCut_track = cms.double(0.75)
tpToGlbCosmic1LegSelMuonAssociation.EfficiencyCut_muon = cms.double(0.5)
tpToGlbCosmic1LegSelMuonAssociation.PurityCut_muon = cms.double(0.75)
tpToGlbCosmic1LegSelMuonAssociation.acceptOneStubMatchings = False
tpToGlbCosmic1LegSelMuonAssociation.includeZeroHitMuons = False

#
# The full-sim association sequences
#

muonAssociation_seq = cms.Sequence(
    selectedVertices
    + muonColl_seq
    + extractedMuonTracks_seq
    + bestMuon_seq
#    +seedsOfSTAmuons_seq
    #+probeTracks_seq
    #+(tpToTkMuonAssociation+tpToTkmuTrackAssociation)
#    +(tpToStaSeedAssociation+tpToStaMuonAssociation
    +tpToStaUpdMuonAssociation
    +tpToGlbMuonAssociation
    +tpToME0MuonMuonAssociation
    +tpToGEMMuonMuonAssociation
    #)
    #+(
	#tpToStaMuonAssociation+
	#tpToStaUpdMuonAssociation2StTime
	#+tpToGlbMuonAssociation
    #+tpToLooseMuonAssociation#+tpToLoose5MuonAssociation
#    +tpToLoose2MuonAssociation+tpToLoose52MuonAssociation
    #+tpToTightMuonAssociation#+tpToTight5MuonAssociation
    #)
#   +(tpToStaTrackAssociation+tpToStaUpdTrackAssociation+tpToGlbTrackAssociation)
#    
# few more association modules usable for the Upgrade TP studies 
    +(
      #tpToTkSelMuonAssociation+
      #tpToStaUpdSelMuonAssociation+tpToStaUpdSel2MuonAssociation
      tpToStaUpdSel2MuonAssociation2StTime
      #+tpToGlbSelMuonAssociation+tpToGlbSel2MuonAssociation+tpToGlbSel3MuonAssociation+tpToGlbSel4MuonAssociation
     )
    +(
      #tpToLooseSelMuonAssociation+tpToLooseSel2MuonAssociation+
      #tpToLooseSel3MuonAssociation
      #+tpToLooseSel35MuonAssociation
      tpToLooseSel0MuonAssociation
      #+tpToLooseSelUncMuonAssociation
      +tpToLooseSel05MuonAssociation
      +tpToLooseModSel0MuonAssociation
      #+tpToLooseModSelUncMuonAssociation
      #+tpToLooseModSel05MuonAssociation
     )
    +(
      #tpToTightSelMuonAssociation+tpToTightSel2MuonAssociation+
      #tpToTightSel3MuonAssociation
      #+tpToTightSel35MuonAssociation
      tpToTightSel0MuonAssociation
      #+tpToTightSelUncMuonAssociation
      +tpToTightSel05MuonAssociation
      +tpToTightModSel0MuonAssociation
      #+tpToTightModSelUncMuonAssociation
      #+tpToTightModSel05MuonAssociation
    )
)

muonAssociationTEV_seq = cms.Sequence(
    tpToTevFirstMuonAssociation+tpToTevPickyMuonAssociation+tpToTevDytMuonAssociation
)

muonAssociationDisplaced_seq = cms.Sequence(
    seedsOfDisplacedSTAmuons_seq+tpToDisplacedStaSeedAssociation+tpToDisplacedStaMuonAssociation
    +tpToDisplacedTrkMuonAssociation+tpToDisplacedGlbMuonAssociation
)

muonAssociationRefit_seq = cms.Sequence(tpToStaRefitMuonAssociation+tpToStaRefitUpdMuonAssociation)

muonAssociationCosmic_seq = cms.Sequence(
    tpToTkCosmicSelMuonAssociation+ tpToTkCosmic1LegSelMuonAssociation
    +tpToStaCosmicSelMuonAssociation+tpToStaCosmic1LegSelMuonAssociation
    +tpToGlbCosmicSelMuonAssociation+tpToGlbCosmic1LegSelMuonAssociation
)

muonAssociationHLT_seq = cms.Sequence(
    tpToL2MuonAssociation+tpToL2UpdMuonAssociation+tpToL3TkMuonAssociation+tpToL3MuonAssociation
)


# fastsim has no hlt specific dt hit collection
from Configuration.Eras.Modifier_fastSim_cff import fastSim
if fastSim.isChosen():
    _DTrechitTag = SimMuon.MCTruth.MuonAssociatorByHits_cfi.muonAssociatorByHits.DTrechitTag
    tpToL3TkMuonAssociation.DTrechitTag = _DTrechitTag
    tpToL2MuonAssociation.DTrechitTag = _DTrechitTag
    tpToL2UpdMuonAssociation.DTrechitTag = _DTrechitTag
    tpToL3MuonAssociation.DTrechitTag = _DTrechitTag

