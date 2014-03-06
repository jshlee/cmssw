#ifndef CSCTriggerPrimitives_CSCMotherboardME21_h
#define CSCTriggerPrimitives_CSCMotherboardME21_h

/** \class CSCMotherboardME11
 *
 * Extended CSCMotherboardME21 for ME21 TMB upgrade
 *
 * \author Sven Dildick March 2014
 *
 * Based on CSCMotherboardME21 code
 *
 */

#include <L1Trigger/CSCTriggerPrimitives/src/CSCMotherboard.h>
#include <DataFormats/GEMDigi/interface/GEMCSCPadDigiCollection.h>

class CSCGeometry;
class CSCChamber;
class GEMGeometry;
class GEMSuperChamber;

class CSCMotherboardME21 : public CSCMotherboard
{
 public:
  /** Normal constructor. */
  CSCMotherboardME21(unsigned endcap, unsigned station, unsigned sector, 
		 unsigned subsector, unsigned chamber,
		 const edm::ParameterSet& conf);

  /** Default destructor. */
  ~CSCMotherboardME21();

  /** Run function for normal usage.  Runs cathode and anode LCT processors,
      takes results and correlates into CorrelatedLCT. */
  void run(const CSCWireDigiCollection* wiredc, 
           const CSCComparatorDigiCollection* compdc, 
           const GEMCSCPadDigiCollection* gemPads);

  /// set CSC and GEM geometries for the matching needs
  void setCSCGeometry(const CSCGeometry *g) { csc_g = g; }
  void setGEMGeometry(const GEMGeometry *g) { gem_g = g; }

 private: 

  const CSCGeometry* csc_g;
  const GEMGeometry* gem_g;

/*   void correlateLCTs(CSCALCTDigi bestALCT, CSCALCTDigi secondALCT, */
/*                      CSCCLCTDigi bestCLCT, CSCCLCTDigi secondCLCT); */

/*   CSCCorrelatedLCTDigi constructLCTs(const CSCALCTDigi& aLCT, */
/*                                      const CSCCLCTDigi& cLCT); */

/*   unsigned int encodePattern(const int ptn, const int highPt); */

/*   unsigned int findQuality(const CSCALCTDigi& aLCT, const CSCCLCTDigi& cLCT); */
};
#endif
