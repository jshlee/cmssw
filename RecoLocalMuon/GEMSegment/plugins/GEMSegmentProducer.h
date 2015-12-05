#ifndef GEMRecHit_GEMSegmentProducer_h
#define GEMRecHit_GEMSegmentProducer_h

/** \class GEMSegmentProducer derived by CSCSegmentProducer 
 * Produces a collection of GEMSegment's in endcap muon GEMs. 
 *
 * \author Piet Verwilligen
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include <DataFormats/GEMRecHit/interface/GEMRecHitCollection.h>

class GEMSegmentBuilder; 

class GEMSegmentProducer : public edm::EDProducer {
public:
    /// Constructor
    explicit GEMSegmentProducer(const edm::ParameterSet&);
    /// Destructor
    ~GEMSegmentProducer();
    /// Produce the GEMSegment collection
    virtual void produce(edm::Event&, const edm::EventSetup&);

private:
    int iev; // events through
    edm::EDGetTokenT<GEMRecHitCollection> theGEMRecHitToken;
    std::unique_ptr<GEMSegmentBuilder> segmentBuilder_;
};

#endif
