#ifndef RecoLocalMuon_GEMCSCSegment_NEWGEMCSCSegmentAlgorithm_h
#define RecoLocalMuon_GEMCSCSegment_NEWGEMCSCSegmentAlgorithm_h

/** \class GEMCSCSegmentAlgo
 *
 * An abstract base class for algorithmic classes used to 
 * build segments combining CSC and GEM information.
 *
 */

#include <DataFormats/CSCRecHit/interface/CSCRecHit2DCollection.h>
#include <DataFormats/CSCRecHit/interface/CSCSegment.h>
#include <Geometry/CSCGeometry/interface/CSCChamber.h>

#include <DataFormats/GEMRecHit/interface/GEMRecHitCollection.h>
#include <DataFormats/GEMRecHit/interface/GEMCSCSegment.h>
#include <Geometry/GEMGeometry/interface/GEMEtaPartition.h>

#include <FWCore/Framework/interface/Frameworkfwd.h>
#include <vector>
#include <map>

class NEWGEMCSCSegmentAlgorithm {

    public:

    /// Constructor
    explicit NEWGEMCSCSegmentAlgorithm(const edm::ParameterSet&) {};
    /// Destructor
    virtual ~NEWGEMCSCSegmentAlgorithm() {};
    /// Run the algorithm = build segments 
    virtual std::vector<GEMCSCSegment> run(const std::map<uint32_t, const CSCLayer*>& csclayermap, const std::map<uint32_t,
					   const GEMEtaPartition*>& gemrollmap,
					   const std::vector<const CSCRecHit2D*>& cscsegments,
					   const std::vector<const GEMRecHit*>& gemrechits) = 0;
    
    private:
};

#endif
