#ifndef RecoLocalMuon_GEMCSCSegment_NEWGEMCSCSegmentProducer_h
#define RecoLocalMuon_GEMCSCSegment_NEWGEMCSCSegmentProducer_h

/** \class NEWGEMCSCSegmentProducer 
 * Produces a collection of GEM-CSCSegments
 *
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/CSCRecHit/interface/CSCSegmentCollection.h"
#include "DataFormats/GEMRecHit/interface/GEMRecHitCollection.h"

class NEWGEMCSCSegmentBuilder; 

class NEWGEMCSCSegmentProducer : public edm::stream::EDProducer<> {
public:
    /// Constructor
    explicit NEWGEMCSCSegmentProducer(const edm::ParameterSet&);
    /// Destructor
    ~NEWGEMCSCSegmentProducer() override;
    /// Produce the GEM-CSCSegment collection
    void produce(edm::Event&, const edm::EventSetup&) override;

private:
    int iev; // events through
    NEWGEMCSCSegmentBuilder* segmentBuilder_;
    edm::EDGetTokenT<CSCRecHit2DCollection> csc_token;
    edm::EDGetTokenT<GEMRecHitCollection>  gem_token;
};

#endif
