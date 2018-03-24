#ifndef RecoLocalMuon_GEMCSCSegment_NEWGEMCSCSegAlgoRU_h
#define RecoLocalMuon_GEMCSCSegment_NEWGEMCSCSegAlgoRU_h

/**
 * \class NEWGEMCSCSegAlgoRU
 *
 */

#include <RecoLocalMuon/GEMCSCSegment/plugins/NEWGEMCSCSegmentAlgorithm.h>
#include <DataFormats/GEMRecHit/interface/GEMRecHit.h>
#include <DataFormats/CSCRecHit/interface/CSCRecHit2D.h>

#include <deque>
#include <vector>


class GEMCSCSegFit;

class NEWGEMCSCSegAlgoRU : public NEWGEMCSCSegmentAlgorithm {


public:

  /// Constructor
  explicit NEWGEMCSCSegAlgoRU(const edm::ParameterSet& ps);

  /// Destructor
  ~NEWGEMCSCSegAlgoRU() override;

  /**
   * Build segments for all desired groups of hits
   */
  std::vector<GEMCSCSegment> run( const std::map<uint32_t, const CSCLayer*>& csclayermap, const std::map<uint32_t,
				  const GEMEtaPartition*>& gemrollmap,
				  const std::vector<const CSCRecHit2D*>& cscsegments,
				  const std::vector<const GEMRecHit*>& gemrechits) override;
private:

  /// Utility functions 
  /**
   * Search for GEMHits inside a Box around the position extrapolated from the CSC segment.
   */
  std::vector<const TrackingRecHit*> chainHitsToSegm(const CSCSegment* cscsegment, const std::vector<const GEMRecHit*>& gemrechits);

    
  /**
   * Build the GEMCSCSegment.
   */
  std::vector<GEMCSCSegment> buildSegments(const CSCSegment* cscsegment, const std::vector<const TrackingRecHit*>& rechits);


  /// Configuration parameters
  bool         debug;
  unsigned int minHitsPerSegment;
  bool         preClustering;
  double       dXclusBoxMax;
  double       dYclusBoxMax;
  bool         preClustering_useChaining;
  double       dPhiChainBoxMax;
  double       dThetaChainBoxMax;
  double       dRChainBoxMax;
  int          maxRecHitsInCluster;
  
  /// Member variables
  const std::string myName; // name of the algorithm, here: NEWGEMCSCSegAlgoRU

  std::map<uint32_t, const CSCLayer*>        theCSCLayers_;
  std::map<uint32_t, const GEMEtaPartition*> theGEMEtaParts_;
  GEMCSCSegFit*                              sfit_;
};

#endif
