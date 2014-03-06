#include <L1Trigger/CSCTriggerPrimitives/src/CSCMotherboardME21.h>
#include <FWCore/MessageLogger/interface/MessageLogger.h>
#include <DataFormats/MuonDetId/interface/CSCTriggerNumbering.h>

CSCMotherboardME21::CSCMotherboardME21(unsigned endcap, unsigned station,
                               unsigned sector, unsigned subsector,
                               unsigned chamber,
                               const edm::ParameterSet& conf) :
  CSCMotherboard(endcap, station, sector, subsector, chamber, conf)
{
  edm::ParameterSet commonParams = conf.getParameter<edm::ParameterSet>("commonParam");
  
  if (!isSLHC) edm::LogError("L1CSCTPEmulatorConfigError")
    << "+++ Upgrade CSCMotherboardME21 constructed while isSLHC is not set! +++\n";
  
  edm::ParameterSet alctParams = conf.getParameter<edm::ParameterSet>("alctSLHC");
  edm::ParameterSet clctParams = conf.getParameter<edm::ParameterSet>("clctSLHC");
  edm::ParameterSet tmbParams = conf.getParameter<edm::ParameterSet>("tmbSLHC");

  // whether to not reuse CLCTs that were used by previous matching ALCTs
  // in ALCT-to-CLCT algorithm
  drop_used_clcts = tmbParams.getUntrackedParameter<bool>("tmbDropUsedClcts",true);

  //----------------------------------------------------------------------------------------//

  //       G E M  -  C S C   I N T E G R A T E D   L O C A L   A L G O R I T H M

  //----------------------------------------------------------------------------------------//

  runUpgradeME21_ = tmbParams.getUntrackedParameter<bool>("runUpgradeME21",false);
}

CSCMotherboardME21::~CSCMotherboardME21() {
  if (alct) delete alct;
  if (clct) delete clct;
}


void
CSCMotherboardME21::run(const CSCWireDigiCollection* wiredc,
                    const CSCComparatorDigiCollection* compdc,
                    const GEMCSCPadDigiCollection* gemPads) 
{
  clear();

  if (!( alct and clct and runUpgradeME21_))
  {
    if (infoV >= 0) edm::LogError("L1CSCTPEmulatorSetupError")
      << "+++ run() called for non-existing ALCT/CLCT processor! +++ \n";
    return;
  }

  std::vector<CSCALCTDigi> alctV = alct->run(wiredc); // run anodeLCT
  std::vector<CSCCLCTDigi> clctV = clct->run(compdc); // run cathodeLCT

  int used_alct_mask[20];
  for (int a=0;a<20;++a) used_alct_mask[a]=0;
  
  int bx_alct_matched = 0; // bx of last matched ALCT
  for (int bx_clct = 0; bx_clct < CSCCathodeLCTProcessor::MAX_CLCT_BINS;
       bx_clct++) {
    // There should be at least one valid ALCT or CLCT for a
    // correlated LCT to be formed.  Decision on whether to reject
    // non-complete LCTs (and if yes of which type) is made further
    // upstream.
    if (clct->bestCLCT[bx_clct].isValid()) {
      // Look for ALCTs within the match-time window.  The window is
      // centered at the CLCT bx; therefore, we make an assumption
      // that anode and cathode hits are perfectly synchronized.  This
      // is always true for MC, but only an approximation when the
      // data is analyzed (which works fairly good as long as wide
      // windows are used).  To get rid of this assumption, one would
      // need to access "full BX" words, which are not readily
      // available.
      bool is_matched = false;
      int bx_alct_start = bx_clct - match_trig_window_size/2;
      int bx_alct_stop  = bx_clct + match_trig_window_size/2;
      // Empirical correction to match 2009 collision data (firmware change?)
      // (but don't do it for SLHC case, assume it would not be there)
      if (!isSLHC) bx_alct_stop += match_trig_window_size%2;
      
      for (int bx_alct = bx_alct_start; bx_alct <= bx_alct_stop; bx_alct++) {
        if (bx_alct < 0 || bx_alct >= CSCAnodeLCTProcessor::MAX_ALCT_BINS)
          continue;
        // default: do not reuse ALCTs that were used with previous CLCTs
        if (drop_used_alcts && used_alct_mask[bx_alct]) continue;
        if (alct->bestALCT[bx_alct].isValid()) {
          if (infoV > 1) LogTrace("CSCMotherboardME21")
            << "Successful ALCT-CLCT match: bx_clct = " << bx_clct
            << "; match window: [" << bx_alct_start << "; " << bx_alct_stop
            << "]; bx_alct = " << bx_alct;
          correlateLCTs(alct->bestALCT[bx_alct], alct->secondALCT[bx_alct],
                        clct->bestCLCT[bx_clct], clct->secondCLCT[bx_clct]);
          used_alct_mask[bx_alct] += 1;
          is_matched = true;
          bx_alct_matched = bx_alct;
          break;
        }
      }
      // No ALCT within the match time interval found: report CLCT-only LCT
      // (use dummy ALCTs).
      if (!is_matched) {
        if (infoV > 1) LogTrace("CSCMotherboardME21")
          << "Unsuccessful ALCT-CLCT match (CLCT only): bx_clct = "
          << bx_clct << "; match window: [" << bx_alct_start
          << "; " << bx_alct_stop << "]";
        correlateLCTs(alct->bestALCT[bx_clct], alct->secondALCT[bx_clct],
                      clct->bestCLCT[bx_clct], clct->secondCLCT[bx_clct]);
      }
    }
    // No valid CLCTs; attempt to make ALCT-only LCT.  Use only ALCTs
    // which have zeroth chance to be matched at later cathode times.
    // (I am not entirely sure this perfectly matches the firmware logic.)
    // Use dummy CLCTs.
    else {
      int bx_alct = bx_clct - match_trig_window_size/2;
      if (bx_alct >= 0 && bx_alct > bx_alct_matched) {
        if (alct->bestALCT[bx_alct].isValid()) {
          if (infoV > 1) LogTrace("CSCMotherboardME21")
            << "Unsuccessful ALCT-CLCT match (ALCT only): bx_alct = "
            << bx_alct;
          correlateLCTs(alct->bestALCT[bx_alct], alct->secondALCT[bx_alct],
                        clct->bestCLCT[bx_clct], clct->secondCLCT[bx_clct]);
          }
      }
    }
  }
  
  if (infoV > 0) {
    for (int bx = 0; bx < MAX_LCT_BINS; bx++) {
      if (firstLCT[bx].isValid())
        LogDebug("CSCMotherboardME21") << firstLCT[bx];
      if (secondLCT[bx].isValid())
        LogDebug("CSCMotherboardME21") << secondLCT[bx];
    }
  }
}
/*
  
void CSCMotherboardME21::correlateLCTs(CSCALCTDigi bestALCT,
CSCALCTDigi secondALCT,
                                   CSCCLCTDigi bestCLCT,
                                   CSCCLCTDigi secondCLCT) {

  bool anodeBestValid     = bestALCT.isValid();
  bool anodeSecondValid   = secondALCT.isValid();
  bool cathodeBestValid   = bestCLCT.isValid();
  bool cathodeSecondValid = secondCLCT.isValid();

  if (anodeBestValid && !anodeSecondValid)     secondALCT = bestALCT;
  if (!anodeBestValid && anodeSecondValid)     bestALCT   = secondALCT;
  if (cathodeBestValid && !cathodeSecondValid) secondCLCT = bestCLCT;
  if (!cathodeBestValid && cathodeSecondValid) bestCLCT   = secondCLCT;

  // ALCT-CLCT matching conditions are defined by "trig_enable" configuration
  // parameters.
  if ((alct_trig_enable  && bestALCT.isValid()) ||
      (clct_trig_enable  && bestCLCT.isValid()) ||
      (match_trig_enable && bestALCT.isValid() && bestCLCT.isValid())) {
    CSCCorrelatedLCTDigi lct = constructLCTs(bestALCT, bestCLCT);
    int bx = lct.getBX();
    if (bx >= 0 && bx < MAX_LCT_BINS) {
      firstLCT[bx] = lct;
      firstLCT[bx].setTrknmb(1);
    }
    else {
      if (infoV > 0) edm::LogWarning("L1CSCTPEmulatorOutOfTimeLCT")
        << "+++ Bx of first LCT candidate, " << bx
        << ", is not within the allowed range, [0-" << MAX_LCT_BINS-1
        << "); skipping it... +++\n";
    }
  }

  if (((secondALCT != bestALCT) || (secondCLCT != bestCLCT)) &&
      ((alct_trig_enable  && secondALCT.isValid()) ||
       (clct_trig_enable  && secondCLCT.isValid()) ||
       (match_trig_enable && secondALCT.isValid() && secondCLCT.isValid()))) {
    CSCCorrelatedLCTDigi lct = constructLCTs(secondALCT, secondCLCT);
    int bx = lct.getBX();
    if (bx >= 0 && bx < MAX_LCT_BINS) {
      secondLCT[bx] = lct;
      secondLCT[bx].setTrknmb(2);
    }
    else {
      if (infoV > 0) edm::LogWarning("L1CSCTPEmulatorOutOfTimeLCT")
        << "+++ Bx of second LCT candidate, " << bx
        << ", is not within the allowed range, [0-" << MAX_LCT_BINS-1
        << "); skipping it... +++\n";
    }
  }
}

// This method calculates all the TMB words and then passes them to the
// constructor of correlated LCTs.
CSCCorrelatedLCTDigi CSCMotherboardME21::constructLCTs(const CSCALCTDigi& aLCT,
                                                   const CSCCLCTDigi& cLCT) {
  // CLCT pattern number
  unsigned int pattern = encodePattern(cLCT.getPattern(), cLCT.getStripType());

  // LCT quality number
  unsigned int quality = findQuality(aLCT, cLCT);

  // Bunch crossing: get it from cathode LCT if anode LCT is not there.
  int bx = aLCT.isValid() ? aLCT.getBX() : cLCT.getBX();

  // construct correlated LCT; temporarily assign track number of 0.
  int trknmb = 0;
  CSCCorrelatedLCTDigi thisLCT(trknmb, 1, quality, aLCT.getKeyWG(),
                               cLCT.getKeyStrip(), pattern, cLCT.getBend(),
                               bx, 0, 0, 0, theTrigChamber);
  return thisLCT;
}

// CLCT pattern number: encodes the pattern number itself and
// whether the pattern consists of half-strips or di-strips.
unsigned int CSCMotherboardME21::encodePattern(const int ptn,
                                           const int stripType) {
  const int kPatternBitWidth = 4;
  unsigned int pattern;

  if (!isTMB07) {
    // Cathode pattern number is a kPatternBitWidth-1 bit word.
    pattern = (abs(ptn) & ((1<<(kPatternBitWidth-1))-1));

    // The pattern has the MSB (4th bit in the default version) set if it
    // consists of half-strips.
    if (stripType) {
      pattern = pattern | (1<<(kPatternBitWidth-1));
    }
  }
  else {
    // In the TMB07 firmware, LCT pattern is just a 4-bit CLCT pattern.
    pattern = (abs(ptn) & ((1<<kPatternBitWidth)-1));
  }

  return pattern;
}
*/


void CSCMotherboardME21::buildCoincidencePads(const GEMCSCPadDigiCollection* out_pads, GEMCSCPadDigiCollection& out_co_pads)
{
  // build coincidences
  for (auto det_range = out_pads->begin(); det_range != out_pads->end(); ++det_range) {
    const GEMDetId& id = (*det_range).first;
    
    // all coincidences detIDs will have layer=1
    if (id.layer() != 1) continue;
    
    // find the corresponding id with layer=2
    GEMDetId co_id(id.region(), id.ring(), id.station(), 2, id.chamber(), id.roll());
    
    auto co_pads_range = out_pads->get(co_id);
    // empty range = no possible coincidence pads
    if (co_pads_range.first == co_pads_range.second) continue;
      
    // now let's correlate the pads in two layers of this partition
    const auto& pads_range = (*det_range).second;
    for (auto p = pads_range.first; p != pads_range.second; ++p) {
      for (auto co_p = co_pads_range.first; co_p != co_pads_range.second; ++co_p) {
        // check the match in pad
        if (std::abs(p->pad() - co_p->pad()) > maxDeltaPadInCoPad_) continue;
        // check the match in BX
        if (std::abs(p->bx() - co_p->bx()) > maxDeltaBXInCoPad_ ) continue;
        
        // always use layer1 pad's BX as a copad's BX
        GEMCSCPadDigi co_pad_digi(p->pad(), p->bx());
        out_co_pads.insertDigi(id, co_pad_digi);
      }
    }
  }
}
