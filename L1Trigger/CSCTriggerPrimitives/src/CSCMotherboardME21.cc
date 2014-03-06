#include <L1Trigger/CSCTriggerPrimitives/src/CSCMotherboardME21.h>
#include <FWCore/MessageLogger/interface/MessageLogger.h>
#include <DataFormats/MuonDetId/interface/CSCTriggerNumbering.h>

CSCMotherboardME21::CSCMotherboardME21(unsigned endcap, unsigned station,
                               unsigned sector, unsigned subsector,
                               unsigned chamber,
                               const edm::ParameterSet& conf) :
  CSCMotherboard(endcap, station, sector, subsector, chamber, conf)
{
  // Normal constructor.  -JM
  // Pass ALCT, CLCT, and common parameters on to ALCT and CLCT processors.
  static bool config_dumped = false;

  // Some configuration parameters and some details of the emulator
  // algorithms depend on whether we want to emulate the trigger logic
  // used in TB/MTCC or its idealized version (the latter was used in MC
  // studies since early ORCA days until (and including) CMSSW_2_1_X).
  edm::ParameterSet commonParams =
    conf.getParameter<edm::ParameterSet>("commonParam");
  isMTCC = commonParams.getParameter<bool>("isMTCC");

  // Switch for a new (2007) version of the TMB firmware.
  isTMB07 = commonParams.getParameter<bool>("isTMB07");

  // is it (non-upgrade algorithm) run along with upgrade one?
  isSLHC = commonParams.getUntrackedParameter<bool>("isSLHC");

  // Choose the appropriate set of configuration parameters depending on
  // isTMB07 and isMTCC flags.
  // Starting with CMSSW_3_1_X, these settings are overwritten by the
  // ones delivered by the EventSetup mechanism.
  edm::ParameterSet alctParams, clctParams;
  if (isTMB07) {
    alctParams = conf.getParameter<edm::ParameterSet>("alctParam07");
    clctParams = conf.getParameter<edm::ParameterSet>("clctParam07");
  }
  else if (isMTCC) {
    alctParams = conf.getParameter<edm::ParameterSet>("alctParamMTCC");
    clctParams = conf.getParameter<edm::ParameterSet>("clctParamMTCC");
  }
  else {
    alctParams = conf.getParameter<edm::ParameterSet>("alctParamOldMC");
    clctParams = conf.getParameter<edm::ParameterSet>("clctParamOldMC");
  }

  // Motherboard parameters:
  edm::ParameterSet tmbParams  =  conf.getParameter<edm::ParameterSet>("tmbParam");

  if (isSLHC && theStation == 1 &&
      CSCTriggerNumbering::ringFromTriggerLabels(theStation, theTrigChamber) == 1 ) {
    alctParams = conf.getParameter<edm::ParameterSet>("alctSLHC");
    clctParams = conf.getParameter<edm::ParameterSet>("clctSLHC");
    tmbParams  =  conf.getParameter<edm::ParameterSet>("tmbSLHC");
  }

  mpc_block_me1a    = tmbParams.getParameter<unsigned int>("mpcBlockMe1a");
  alct_trig_enable  = tmbParams.getParameter<unsigned int>("alctTrigEnable");
  clct_trig_enable  = tmbParams.getParameter<unsigned int>("clctTrigEnable");
  match_trig_enable = tmbParams.getParameter<unsigned int>("matchTrigEnable");
  match_trig_window_size =
    tmbParams.getParameter<unsigned int>("matchTrigWindowSize");
  tmb_l1a_window_size = // Common to CLCT and TMB
    tmbParams.getParameter<unsigned int>("tmbL1aWindowSize");

  // configuration handle for number of early time bins
  early_tbins = tmbParams.getUntrackedParameter<int>("tmbEarlyTbins",4);

  // whether to not reuse ALCTs that were used by previous matching CLCTs
  drop_used_alcts = tmbParams.getUntrackedParameter<bool>("tmbDropUsedAlcts",true);

  // whether to readout only the earliest two LCTs in readout window
  readout_earliest_2 = tmbParams.getUntrackedParameter<bool>("tmbReadoutEarliest2",false);

  infoV = tmbParams.getUntrackedParameter<int>("verbosity", 0);

  alct = new CSCAnodeLCTProcessor(endcap, station, sector, subsector, chamber, alctParams, commonParams);
  clct = new CSCCathodeLCTProcessor(endcap, station, sector, subsector, chamber, clctParams, commonParams, tmbParams);

  //if (theStation==1 && CSCTriggerNumbering::ringFromTriggerLabels(theStation, theTrigChamber)==2) infoV = 3;

  // Check and print configuration parameters.
  checkConfigParameters();
  if (infoV > 0 && !config_dumped) {
    dumpConfigParams();
    config_dumped = true;
  }

  // test to make sure that what goes into a correlated LCT is also what
  // comes back out.
  // testLCT();
}

CSCMotherboardME21::~CSCMotherboardME21() {
  if (alct) delete alct;
  if (clct) delete clct;
}


void
CSCMotherboardME21::run(const CSCWireDigiCollection* wiredc,
                    const CSCComparatorDigiCollection* compdc,
                    const GEMCSCPadDigiCollection* gemPads) {
  clear();
  if (alct && clct) {
    {
      std::vector<CSCALCTDigi> alctV = alct->run(wiredc); // run anodeLCT
    }
    {
      std::vector<CSCCLCTDigi> clctV = clct->run(compdc); // run cathodeLCT
    }

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
  else {
    if (infoV >= 0) edm::LogError("L1CSCTPEmulatorSetupError")
      << "+++ run() called for non-existing ALCT/CLCT processor! +++ \n";
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
