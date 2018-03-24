/** \file NEWGEMCSCSegmentProducer.cc
 *
 */

#include <RecoLocalMuon/GEMCSCSegment/plugins/NEWGEMCSCSegmentProducer.h>
#include <RecoLocalMuon/GEMCSCSegment/plugins/NEWGEMCSCSegmentBuilder.h>

#include <DataFormats/Common/interface/Handle.h>
#include <FWCore/Framework/interface/ESHandle.h>
#include <FWCore/MessageLogger/interface/MessageLogger.h> 

#include <Geometry/Records/interface/MuonGeometryRecord.h>

#include <DataFormats/GEMRecHit/interface/GEMRecHitCollection.h>
#include <DataFormats/GEMRecHit/interface/GEMCSCSegmentCollection.h>
#include <DataFormats/GEMRecHit/interface/GEMCSCSegment.h>

#include <DataFormats/CSCRecHit/interface/CSCRecHit2DCollection.h>
#include <DataFormats/CSCRecHit/interface/CSCSegmentCollection.h>
#include <DataFormats/CSCRecHit/interface/CSCSegment.h>

NEWGEMCSCSegmentProducer::NEWGEMCSCSegmentProducer(const edm::ParameterSet& pas) : iev(0) {
	
  csc_token = consumes<CSCRecHit2DCollection>( pas.getParameter<edm::InputTag>("inputObjectsCSC"));
  gem_token = consumes<GEMRecHitCollection> ( pas.getParameter<edm::InputTag>("inputObjectsGEM"));
  segmentBuilder_  = new GEMCSCSegmentBuilder(pas); // pass on the parameterset
  
  // register what this produces
  produces<GEMCSCSegmentCollection>();
}


NEWGEMCSCSegmentProducer::~NEWGEMCSCSegmentProducer() {

    LogDebug("GEMCSCSegment") << "deleting GEMCSCSegmentBuilder after " << iev << " events w/ gem and csc data.";
    delete segmentBuilder_;
}


void NEWGEMCSCSegmentProducer::produce(edm::Event& ev, const edm::EventSetup& setup) {
    LogDebug("GEMCSCSegment") << "start producing segments for " << ++iev << "th event w/ gem and csc data";
	
    // find the geometry (& conditions?) for this event & cache it in the builder
    edm::ESHandle<CSCGeometry> cscg;
    setup.get<MuonGeometryRecord>().get(cscg);
    const CSCGeometry* cgeom = &*cscg;
    
    edm::ESHandle<GEMGeometry> gemg;
    setup.get<MuonGeometryRecord>().get(gemg);
    const GEMGeometry* ggeom = &*gemg;
    
    // cache the geometry in the builder
    segmentBuilder_->setGeometry(ggeom,cgeom);

    // fill the map with matches between GEM and CSC chambers
    segmentBuilder_->LinkGEMRollsToCSCChamberIndex(ggeom,cgeom);

    // get the collection of CSCSegment and GEMRecHits
    edm::Handle<CSCRecHit2DCollection> cscRecHits;
    ev.getByToken(csc_token, cscRecHits);
    edm::Handle<GEMRecHitCollection> gemRecHits;
    ev.getByToken(gem_token, gemRecHits);    

    // create empty collection of GEMCSC Segments
    auto oc = std::make_unique<GEMCSCSegmentCollection>();

    // pass the empty collection of GEMCSC Segments and fill it
    segmentBuilder_->build(gemRecHits.product(), cscRecHits.product(), *oc); //@@ FILL oc
    
    // put the filled collection in event
    ev.put(std::move(oc));
}
