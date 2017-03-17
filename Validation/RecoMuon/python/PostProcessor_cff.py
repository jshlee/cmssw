import FWCore.ParameterSet.Config as cms

postProcessorMuonMultiTrack = cms.EDAnalyzer("DQMGenericClient",
    subDirs = cms.untracked.vstring("Muons/RecoMuonV/MultiTrack/*"),
    efficiency = cms.vstring(
    "efficVtx 'Efficiency vs # vtx' num_assoc(simToReco)_vtx num_simul_vtx",
    "efficVtx_barrel 'Efficiency vs # vtx barrel' num_assoc(simToReco)_vtx_barrel num_simul_vtx_barrel",
    "efficVtx_overlap 'Efficiency vs # vtx overlap' num_assoc(simToReco)_vtx_overlap num_simul_vtx_overlap",
    "efficVtx_endcap 'Efficiency vs # vtx endcap' num_assoc(simToReco)_vtx_endcap num_simul_vtx_endcap",
    "effic 'Efficiency vs #eta' num_assoc(simToReco)_eta num_simul_eta",
    "effic_vs_lxy 'Efficiency vs L_{xy}' num_assoc(simToReco)_lxy num_simul_lxy",
    "effic_vs_lz 'Efficiency vs L_{z}' num_assoc(simToReco)_lz num_simul_lz",
    "effic_vs_lr 'Efficiency vs R' num_assoc(simToReco)_lr num_simul_lr",
    "efficBin1 'Efficiency vs #eta bin1' num_assoc(simToReco)_eta_bin1 num_simul_eta_bin1",
    "efficBin2 'Efficiency vs #eta bin2' num_assoc(simToReco)_eta_bin2 num_simul_eta_bin2",
    "efficBin3 'Efficiency vs #eta bin3' num_assoc(simToReco)_eta_bin3 num_simul_eta_bin3",
    "efficBin4 'Efficiency vs #eta bin4' num_assoc(simToReco)_eta_bin4 num_simul_eta_bin4",
    "efficPt 'Efficiency vs p_{T}' num_assoc(simToReco)_pT num_simul_pT",
    "efficPtBarrel 'Efficiency vs p_{T} Barrel' num_assoc(simToReco)_pT_barrel num_simul_pT_barrel",
    "efficPtOverlap 'Efficiency vs p_{T} Overlap' num_assoc(simToReco)_pT_overlap num_simul_pT_overlap",
    "efficPtEndcap 'Efficiency vs p_{T} Endcap' num_assoc(simToReco)_pT_endcap num_simul_pT_endcap",
    "effic_vs_hit 'Efficiency vs n Hits' num_assoc(simToReco)_hit num_simul_hit",
    "effic_vs_phi 'Efficiency vs #phi' num_assoc(simToReco)_phi num_simul_phi",
    "effic_vs_dxy 'Efficiency vs Dxy' num_assoc(simToReco)_dxy num_simul_dxy",
    "effic_vs_dz 'Efficiency vs Dz' num_assoc(simToReco)_dz num_simul_dz",
                             
    "fakerate 'Fake rate vs #eta' num_assoc(recoToSim)_eta num_reco_eta fake",
    "fakerate_sgn 'Fake rate vs #eta' num_assoc(recoToSim)_eta_SgnMu num_reco_eta fake",
    "fakerate050 'Fake rate vs #eta' num_assoc(recoToSim)_eta_050 num_reco_eta fake",
    "fakerate050Pt10 'Fake rate vs #eta pt10' num_assoc(recoToSim)_eta_pt10 num_reco_eta_pt10 fake",
    "fakerate075 'Fake rate vs #eta' num_assoc(recoToSim)_eta_075 num_reco_eta fake",
    "fakerateBin1 'Fake rate vs #eta bin1' num_assoc(recoToSim)_eta_bin1 num_reco_eta_bin1 fake",
    "fakerateBin2 'Fake rate vs #eta bin2' num_assoc(recoToSim)_eta_bin2 num_reco_eta_bin2 fake",
    "fakerateBin3 'Fake rate vs #eta bin3' num_assoc(recoToSim)_eta_bin3 num_reco_eta_bin3 fake",
    "fakerateBin4 'Fake rate vs #eta bin4' num_assoc(recoToSim)_eta_bin4 num_reco_eta_bin4 fake",
    "fakeratePt 'Fake rate vs p_{T}' num_assoc(recoToSim)_pT num_reco_pT fake",
    "fakeratePt_Sgn 'Fake rate vs p_{T}' num_assoc(recoToSim)_pT_SgnMu num_reco_pT fake",
    "fakeratePt050 'Fake rate vs p_{T}' num_assoc(recoToSim)_pT_050 num_reco_pT fake",
    "fakeratePt075 'Fake rate vs p_{T}' num_assoc(recoToSim)_pT_075 num_reco_pT fake",
    "fakerate_vs_hit 'Fake rate vs hit' num_assoc(recoToSim)_hit num_reco_hit fake",
    "fakerate_vs_phi 'Fake rate vs phi' num_assoc(recoToSim)_phi num_reco_phi fake",
    "fakerate_vs_dxy 'Fake rate vs dxy' num_assoc(recoToSim)_dxy num_reco_dxy fake",
    "fakerate_vs_dz 'Fake rate vs dz' num_assoc(recoToSim)_dz num_reco_dz fake",
                             
    "yield_sgn 'Average bkg yield vs #eta' num_assoc(recoToSim)_eta_SgnMu num_reco_eta yield numEvt",
    "yieldPt_sgn 'Average bkg yield vs p_{T}' num_assoc(recoToSim)_pT_SgnMu num_reco_pT yield numEvt",

    "effic_Q05 'Efficiency vs #eta (Quality>0.5)' num_assoc(simToReco)_eta_Q05 num_simul_eta",
    "effic_Q05Pt10 'Efficiency vs #eta (Quality>0.5, pT > 10 GeV/c)' num_assoc(simToReco)_eta_pt10 num_simul_eta_pt10",
    "effic_Q075 'Efficiency vs #eta (Quality>0.75)' num_assoc(simToReco)_eta_Q075 num_simul_eta",
    "efficPt_Q05 'Efficiency vs p_{T} (Quality>0.5)' num_assoc(simToReco)_pT_Q05 num_simul_pT",
    "efficPt_Q075 'Efficiency vs p_{T} (Quality>0.75)' num_assoc(simToReco)_pT_Q075 num_simul_pT",
    "effic_vs_phi_Q05 'Efficiency vs #phi' num_assoc(simToReco)_phi_Q05 num_simul_phi",
    "effic_vs_phi_Q075 'Efficiency vs #phi' num_assoc(simToReco)_phi_Q075 num_simul_phi",

    "chargeMisIDProb_vs_eta 'Charge Mis-ID vs. #eta' chargeMisID_vs_etaNum chargeMisID_vs_etaDen",
    "chargeMisIDProb_vs_phi 'Charge Mis-ID vs. #phi' chargeMisID_vs_phiNum chargeMisID_vs_phiDen",
    "chargeMisIDProb_vs_pt 'Charge Mis-ID vs. p_{T}' chargeMisID_vs_ptNum chargeMisID_vs_ptDen",

    "chargeMisIDProb_vs_eta_sim 'Charge Mis-ID vs. #eta Sim' chargeMisID_vs_etaNum_sim chargeMisID_vs_etaDen_sim",
    "chargeMisIDProb_vs_phi_sim 'Charge Mis-ID vs. #phi Sim' chargeMisID_vs_phiNum_sim chargeMisID_vs_phiDen_sim",
    "chargeMisIDProb_vs_pt_sim 'Charge Mis-ID vs. p_{T} Sim' chargeMisID_vs_ptNum_sim chargeMisID_vs_ptDen_sim"    ),
    resolutionLimitedFit = cms.untracked.bool(False),
    resolution = cms.vstring("cotThetares_vs_eta '#sigma(cot(#theta)) vs #eta' cotThetares_vs_eta",
                             "cotThetares_vs_pt '#sigma(cot(#theta)) vs p_{T}' cotThetares_vs_pt",
                             "dxypull_vs_eta 'd_{xy} Pull vs #eta' dxypull_vs_eta",
                             "dxyres_vs_eta '#sigma(d_{xy}) vs #eta' dxyres_vs_eta",
                             "dxyres_vs_pt '#sigma(d_{xy}) vs p_{T}' dxyres_vs_pt",
                             "dzpull_vs_eta 'd_{z} Pull vs #eta' dzpull_vs_eta",
                             "dzres_vs_eta '#sigma(d_{z}) vs #eta' dzres_vs_eta",
                             "dzres_vs_pt '#sigma(d_{z}) vs p_{T}' dzres_vs_pt",
                             "etares_vs_eta '#sigma(#eta) vs #eta' etares_vs_eta",
                             "phipull_vs_eta '#phi Pull vs #eta' phipull_vs_eta",
                             "phipull_vs_phi '#phi Pull vs #phi' phipull_vs_phi",
                             "phires_vs_eta '#sigma(#phi) vs #eta' phires_vs_eta",
                             "phires_vs_phi '#sigma(#phi) vs #phi' phires_vs_phi",
                             "phires_vs_pt '#sigma(#phi) vs p_{T}' phires_vs_pt",
                             "ptpull_vs_eta 'p_{T} Pull vs #eta' ptpull_vs_eta",
                             "ptpull_vs_phi 'p_{T} Pull vs #phi' ptpull_vs_phi",
                             "ptres_vs_eta '#sigma(p_{T}) vs #eta' ptres_vs_eta",
                             "ptres_vs_phi '#sigma(p_{T}) vs #phi' ptres_vs_phi",
                             "ptres_vs_pt '#sigma(p_{T}) vs p_{T}' ptres_vs_pt",
                             "invptres_vs_eta '#sigma(1/p_{T}) vs #eta' invptres_vs_eta",
                             "invptres_vs_phi '#sigma(1/p_{T}) vs #phi' invptres_vs_phi",
                             "invptres_vs_pt '#sigma(1/p_{T}) vs p_{T}' invptres_vs_pt",
                             "qOverPtres_vs_eta '#sigma(q/p_{T}) vs #eta' qOverPtres_vs_eta",
                             "qOverPtres_vs_phi '#sigma(q/p_{T}) vs #phi' qOverPtres_vs_phi",
                             "qOverPtres_vs_pt '#sigma(q/p_{T}) vs p_{T}' qOverPtres_vs_pt",
                             "qOverPtresXL_vs_eta '#sigma(q/p_{T}) vs #eta' qOverPtresXL_vs_eta",
                             "qOverPtresXL_vs_phi '#sigma(q/p_{T}) vs #phi' qOverPtresXL_vs_phi",
                             "qOverPtresXL_vs_pt '#sigma(q/p_{T}) vs p_{T}' qOverPtresXL_vs_pt",
                             "ptres_vs_eta_sim '#sigma(p_{T}) vs #eta Sim' ptres_vs_eta_sim",
                             "ptres_vs_phi_sim '#sigma(p_{T}) vs #phi Sim' ptres_vs_phi_sim",
                             "ptres_vs_pt_sim '#sigma(p_{T}) vs p_{T} Sim' ptres_vs_pt_sim",
                             "invptres_vs_eta_sim '#sigma(1/p_{T}) vs #eta Sim' invptres_vs_eta_sim",
                             "invptres_vs_phi_sim '#sigma(1/p_{T}) vs #phi Sim' invptres_vs_phi_sim",
                             "invptres_vs_pt_sim '#sigma(1/p_{T}) vs p_{T} Sim' invptres_vs_pt_sim",
                             "qOverPtres_vs_eta_sim '#sigma(q/p_{T}) vs #eta Sim' qOverPtres_vs_eta_sim",
                             "qOverPtres1_vs_eta_sim '#sigma(q/p_{T}) vs #eta Sim' qOverPtres1_vs_eta_sim",
                             "qOverPtres2_vs_eta_sim '#sigma(q/p_{T}) vs #eta Sim' qOverPtres2_vs_eta_sim",
                             "qOverPtres3_vs_eta_sim '#sigma(q/p_{T}) vs #eta Sim' qOverPtres3_vs_eta_sim",
                             "qOverPtres4_vs_eta_sim '#sigma(q/p_{T}) vs #eta Sim' qOverPtres4_vs_eta_sim",
                             "qOverPtres_vs_phi_sim '#sigma(q/p_{T}) vs #phi Sim' qOverPtres_vs_phi_sim",
                             "qOverPtres_vs_pt_sim '#sigma(q/p_{T}) vs p_{T} Sim' qOverPtres_vs_pt_sim",
                             "qOverPtresB_vs_pt_sim '#sigma(q/p_{T}) vs p_{T} Sim' qOverPtresB_vs_pt_sim",
                             "qOverPtresO_vs_pt_sim '#sigma(q/p_{T}) vs p_{T} Sim' qOverPtresO_vs_pt_sim",
                             "qOverPtresE_vs_pt_sim '#sigma(q/p_{T}) vs p_{T} Sim' qOverPtresE_vs_pt_sim",
                             "qOverPtresXL_vs_eta_sim '#sigma(q/p_{T}) vs #eta Sim' qOverPtresXL_vs_eta_sim",
                             "qOverPtresXL1_vs_eta_sim '#sigma(q/p_{T}) vs #eta Sim' qOverPtresXL1_vs_eta_sim",
                             "qOverPtresXL2_vs_eta_sim '#sigma(q/p_{T}) vs #eta Sim' qOverPtresXL2_vs_eta_sim",
                             "qOverPtresXL3_vs_eta_sim '#sigma(q/p_{T}) vs #eta Sim' qOverPtresXL3_vs_eta_sim",
                             "qOverPtresXL4_vs_eta_sim '#sigma(q/p_{T}) vs #eta Sim' qOverPtresXL4_vs_eta_sim",
                             "qOverPtresXL_vs_phi_sim '#sigma(q/p_{T}) vs #phi Sim' qOverPtresXL_vs_phi_sim",
                             "qOverPtresXL_vs_pt_sim '#sigma(q/p_{T}) vs p_{T} Sim' qOverPtresXL_vs_pt_sim",
                             "qOverPtresXLB_vs_pt_sim '#sigma(q/p_{T}) vs p_{T} Sim' qOverPtresXLB_vs_pt_sim",
                             "qOverPtresXLO_vs_pt_sim '#sigma(q/p_{T}) vs p_{T} Sim' qOverPtresXLO_vs_pt_sim",
                             "qOverPtresXLE_vs_pt_sim '#sigma(q/p_{T}) vs p_{T} Sim' qOverPtresXLE_vs_pt_sim",
                             "thetapull_vs_eta '#theta Pull vs #eta' thetapull_vs_eta",
                             "thetapull_vs_phi '#theta Pull vs #phi' thetapull_vs_phi"),
    outputFileName = cms.untracked.string("")
)


postProcessorMuonMultiTrackComp = cms.EDAnalyzer("DQMGenericClient",
    subDirs = cms.untracked.vstring("Muons/RecoMuonV/MultiTrack/"),
    efficiency = cms.vstring(
    "Eff_GlbTk_Eta_mabh 'Eff_{GLB,TK} vs #eta' extractedGlobalMuons/effic probeTrks/effic",
    "Eff_GlbTk_Pt_mabh 'Eff_{GLB,TK} vs p_{T}' extractedGlobalMuons/efficPt probeTrks/efficPt",
    "Eff_GlbTk_Hit_mabh 'Eff_{GLB,TK} vs n Hits' extractedGlobalMuons/effic_vs_hit probeTrks/effic_vs_hit",
    "Eff_GlbSta_Eta_mabh 'Eff_{GLB,STA} vs #eta' extractedGlobalMuons/effic standAloneMuons_UpdAtVtx/effic",
    "Eff_GlbSta_Pt_mabh 'Eff_{GLB,STA} vs p_{T}' extractedGlobalMuons/efficPt standAloneMuons_UpdAtVtx/efficPt",
    "Eff_GlbSta_Hit_mabh 'Eff_{GLB,STA} vs n Hits' extractedGlobalMuons/effic_vs_hit standAloneMuons_UpdAtVtx/effic_vs_hit",
    ),
    resolution = cms.vstring(""),
    outputFileName = cms.untracked.string("")
)

postProcessorMuonMultiTrackCompFS = cms.EDAnalyzer("DQMGenericClient",
    subDirs = cms.untracked.vstring("Muons/RecoMuonV/MultiTrack/"),
    efficiency = cms.vstring(
    "Eff_GlbTk_Eta_mabh 'Eff_{GLB,TK} vs #eta' extractedGlobalMuons/effic probeTrks/effic",
    "Eff_GlbTk_Pt_mabh 'Eff_{GLB,TK} vs p_{T}' extractedGlobalMuons/efficPt probeTrks/efficPt",
    "Eff_GlbTk_Hit_mabh 'Eff_{GLB,TK} vs n Hits' extractedGlobalMuons/effic_vs_hit probeTrks/effic_vs_hit",
    "Eff_GlbSta_Eta_mabh 'Eff_{GLB,STA} vs #eta' extractedGlobalMuons/effic standAloneMuons_UpdAtVtx/effic",
    "Eff_GlbSta_Pt_mabh 'Eff_{GLB,STA} vs p_{T}' extractedGlobalMuons/efficPt standAloneMuons_UpdAtVtx/efficPt",
    "Eff_GlbSta_Hit_mabh 'Eff_{GLB,STA} vs n Hits' extractedGlobalMuons/effic_vs_hit standAloneMuons_UpdAtVtx/effic_vs_hit",
    ),
    resolution = cms.vstring(""),
    outputFileName = cms.untracked.string("")
)


postProcessorRecoMuon = cms.EDAnalyzer("DQMGenericClient",
    subDirs = cms.untracked.vstring("Muons/RecoMuonV/RecoMuon_MuonAssoc"),
    #efficiencies and fractions
    efficiency = cms.vstring("EffP   'Efficiency vs p'     P   SimP",
                             "EffPt  'Efficiency vs p_{T}' Pt  SimPt",
                             "EffEta 'Efficiency vs #eta'  Eta SimEta",
                             "EffPhi 'Efficiency vs #phi'  Phi SimPhi",
                             "MisQProbPt  'Charge Mis-identification probability vs p_{T}' MisQPt  SimPt",
                             "MisQProbEta 'Charge Mis-identification probability vs #eta'  MisQEta SimEta",
                             #fractions
                             "FractP   'Muontype fraction vs p'     PMuon   PMuonAll",
                             "FractPt  'Muontype fraction  vs p_{T}' PtMuon PtMuonAll",
                             "FractEta 'Muontype fraction vs #eta'  EtaMuon EtaMuonAll",
                             "FractPhi 'Muontype fraction vs #phi'  PhiMuon PhiMuonAll",
                             ),

    resolution = cms.vstring("ErrP_vs_P      '#sigma(p) vs p'           ErrP_vs_P     ",
                             "ErrP_vs_Eta    '#sigma(p) vs #eta'        ErrP_vs_Eta   ",
                             "ErrPt_vs_Pt    '#sigma(p_{T}) vs p_{T}'   ErrPt_vs_Pt   ",
                             "ErrPt_vs_Eta   '#sigma(p_{T}) vs #eta'    ErrPt_vs_Eta  ",
                             "ErrEta_vs_Eta  '#sigma(#eta) vs #eta '    ErrEta_vs_Eta ",
                             "ErrQPt_vs_Pt   '#sigma(q/p_{T}) vs p_{T}' ErrQPt_vs_Pt  ",
                             "ErrQPt_vs_Eta  '#sigma(q/p_{T}) vs #eta'  ErrQPt_vs_Eta ",
                             "PullEta_vs_Pt  'Pull of #eta vs p_{T}'    PullEta_vs_Pt ",
                             "PullEta_vs_Eta 'Pull of #eta vs #eta'     PullEta_vs_Eta",
                             "PullPhi_vs_Eta 'Pull of #phi vs #eta'     PullPhi_vs_Eta",
                             "PullPt_vs_Pt   'Pull of p_{T} vs p_{T}'   PullPt_vs_Pt  ",
                             "PullPt_vs_Eta  'Pull of p_{T} vs #eta'    PullPt_vs_Eta ",
                             ),    
    outputFileName = cms.untracked.string("")
)

# for each type monitored
postProcessorRecoMuon_Glb = postProcessorRecoMuon.clone()
postProcessorRecoMuon_Glb.subDirs = cms.untracked.vstring("Muons/RecoMuonV/RecoMuon_MuonAssoc_Glb")

postProcessorRecoMuon_Trk = postProcessorRecoMuon.clone()
postProcessorRecoMuon_Trk.subDirs = cms.untracked.vstring("Muons/RecoMuonV/RecoMuon_MuonAssoc_Trk")

postProcessorRecoMuon_Sta = postProcessorRecoMuon.clone()
postProcessorRecoMuon_Sta.subDirs = cms.untracked.vstring("Muons/RecoMuonV/RecoMuon_MuonAssoc_Sta")

postProcessorRecoMuon_Tgt = postProcessorRecoMuon.clone()
postProcessorRecoMuon_Tgt.subDirs = cms.untracked.vstring("Muons/RecoMuonV/RecoMuon_MuonAssoc_Tgt")

postProcessorRecoMuon_GlbPF = postProcessorRecoMuon.clone()
postProcessorRecoMuon_GlbPF.subDirs = cms.untracked.vstring("Muons/RecoMuonV/RecoMuon_MuonAssoc_GlbPF")

postProcessorRecoMuon_TrkPF = postProcessorRecoMuon.clone()
postProcessorRecoMuon_TrkPF.subDirs = cms.untracked.vstring("Muons/RecoMuonV/RecoMuon_MuonAssoc_TrkPF")

postProcessorRecoMuon_StaPF = postProcessorRecoMuon.clone()
postProcessorRecoMuon_StaPF.subDirs = cms.untracked.vstring("Muons/RecoMuonV/RecoMuon_MuonAssoc_StaPF")

#not sure about this one, which types are monitored
postProcessorRecoMuonComp = cms.EDAnalyzer(
    "DQMGenericClient",
    subDirs = cms.untracked.vstring("Muons/RecoMuonV/"),
    efficiency = cms.vstring(
    "Eff_GlbSta_Eta 'Eff_{GLB,STA} vs #eta' RecoMuon_MuonAssoc_Glb/EffEta RecoMuon_MuonAssoc_Sta/EffEta",
    "Eff_GlbSta_P   'Eff_{GLB,STA} vs p' RecoMuon_MuonAssoc_Glb/EffP RecoMuon_MuonAssoc_Sta/EffP",
    "Eff_GlbSta_Phi 'Eff_{GLB,STA} vs #phi' RecoMuon_MuonAssoc_Glb/EffPhi RecoMuon_MuonAssoc_Sta/EffPhi",
    "Eff_GlbSta_Pt  'Eff_{GLB,STA} vs p_{T}' RecoMuon_MuonAssoc_Glb/EffPt RecoMuon_MuonAssoc_Sta/EffPt",
    "Eff_TgtGlb_Eta 'Eff_{TGT,GLB} vs #eta' RecoMuon_MuonAssoc_Tgt/EffEta RecoMuon_MuonAssoc_Glb/EffEta",
    "Eff_TgtGlb_P   'Eff_{TGT,GLB} vs p' RecoMuon_MuonAssoc_Tgt/EffP RecoMuon_MuonAssoc_Glb/EffP",
    "Eff_TgtGlb_Phi 'Eff_{TGT,GLB} vs #phi' RecoMuon_MuonAssoc_Tgt/EffPhi RecoMuon_MuonAssoc_Glb/EffPhi",
    "Eff_TgtGlb_Pt  'Eff_{TGT,GLB} vs p_{T}' RecoMuon_MuonAssoc_Tgt/EffPt RecoMuon_MuonAssoc_Glb/EffPt",
    ),
    resolution = cms.vstring(""),
    outputFileName = cms.untracked.string("")
)

postProcessorRecoMuonCompPF = cms.EDAnalyzer(
    "DQMGenericClient",
    subDirs = cms.untracked.vstring("Muons/RecoMuonV/"),
    efficiency = cms.vstring(
    "Eff_GlbPFStaPF_Eta 'Eff_{GLBPF,STAPF} vs #eta' RecoMuon_MuonAssoc_GlbPF/EffEta RecoMuon_MuonAssoc_StaPF/EffEta",
    "Eff_GlbPFStaPF_P   'Eff_{GLBPF,STAPF} vs p' RecoMuon_MuonAssoc_GlbPF/EffP RecoMuon_MuonAssoc_StaPF/EffP",
    "Eff_GlbPFStaPF_Phi 'Eff_{GLBPF,STAPF} vs #phi' RecoMuon_MuonAssoc_GlbPF/EffPhi RecoMuon_MuonAssoc_StaPF/EffPhi",
    "Eff_GlbPFStaPF_Pt  'Eff_{GLBPF,STAPF} vs p_{T}' RecoMuon_MuonAssoc_GlbPF/EffPt RecoMuon_MuonAssoc_StaPF/EffPt",
    ),
    resolution = cms.vstring(""),
    outputFileName = cms.untracked.string("")
)
        

recoMuonPostProcessors = cms.Sequence(postProcessorMuonMultiTrack*postProcessorRecoMuon_Glb*postProcessorRecoMuon_Trk*postProcessorRecoMuon_Sta*postProcessorRecoMuon_Tgt*postProcessorRecoMuon_GlbPF*postProcessorRecoMuon_TrkPF*postProcessorRecoMuon_StaPF*postProcessorMuonMultiTrackComp*postProcessorRecoMuonComp*postProcessorRecoMuonCompPF)
