/*
 *  \author Julia Yarba
 */

#include <ostream>

#include "IOMC/ParticleGuns/interface/FlatRandomPtAndD0GunProducer.h"

#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "CLHEP/Random/RandFlat.h"

using namespace edm;
using namespace std;

FlatRandomPtAndD0GunProducer::FlatRandomPtAndD0GunProducer(const ParameterSet& pset) :
BaseFlatGunProducer(pset)
{
    
    
    ParameterSet defpset ;
    ParameterSet pgun_params =
    pset.getParameter<ParameterSet>("PGunParameters") ;
    
    fMinPt = pgun_params.getParameter<double>("MinPt");
    fMaxPt = pgun_params.getParameter<double>("MaxPt");
    LMin_ = pgun_params.getParameter<double>("LMin");
    LMax_ = pgun_params.getParameter<double>("LMax");
    drMax_ = pgun_params.getParameter<double>("drMax");
    
    produces<HepMCProduct>("unsmeared");
    produces<GenEventInfoProduct>();
}

FlatRandomPtAndD0GunProducer::~FlatRandomPtAndD0GunProducer()
{
    // no need to cleanup GenEvent memory - done in HepMCProduct
}

void FlatRandomPtAndD0GunProducer::produce(Event &e, const EventSetup& es)
{
    edm::Service<edm::RandomNumberGenerator> rng;
    CLHEP::HepRandomEngine* engine = &rng->getEngine(e.streamID());
    
    if ( fVerbosity > 0 )
    {
        cout << " FlatRandomPtAndD0GunProducer : Begin New Event Generation" << endl ;
    }
    // event loop (well, another step in it...)
    
    // no need to clean up GenEvent memory - done in HepMCProduct
    //
    
    // here re-create fEvt (memory)
    //
    fEvt = new HepMC::GenEvent() ;
    
    // now actualy, cook up the event from PDGTable and gun parameters
    //
    // 1st, primary vertex
    //
    double phi_vtx    = CLHEP::RandFlat::shoot(engine, fMinPhi, fMaxPhi);
    double eta_vtx    = CLHEP::RandFlat::shoot(engine, fMinEta, fMaxEta);
    double theta_vtx  = 2.*atan(exp(-eta_vtx));
    HepMC::GenVertex* Vtx = 0;
    if( LMin_ == LMax_ ) {
        double LMin_x = LMin_*sin(theta_vtx)*cos(phi_vtx);
        double LMin_y = LMin_*sin(theta_vtx)*sin(phi_vtx);
        double LMin_z = LMin_*cos(theta_vtx);
        Vtx = new HepMC::GenVertex(HepMC::FourVector(LMin_x,LMin_y,LMin_z));
    }
    else {
        double len = CLHEP::RandFlat::shoot(engine, LMin_, LMax_)*10;
        double len_x = len*sin(theta_vtx)*cos(phi_vtx);
        double len_y = len*sin(theta_vtx)*sin(phi_vtx);
        double len_z = len*cos(theta_vtx);
        //std::cout<<len<<" "<<len_x<<" "<<len_y<<" "<<len_z<<std::endl;
        Vtx = new HepMC::GenVertex(HepMC::FourVector(len_x,len_y,len_z));
    }
    
    // loop over particles
    //
    int barcode = 1 ;
    for (unsigned int ip=0; ip<fPartIDs.size(); ++ip)
    {
        
        double pt     = 0;
        double eta    = 0;
        double phi    = 0;
        double dr     = 999;
        
        do{
            
            pt     = CLHEP::RandFlat::shoot(engine, fMinPt, fMaxPt) ;
            eta    = CLHEP::RandFlat::shoot(engine, fMinEta, fMaxEta) ;
            phi    = CLHEP::RandFlat::shoot(engine, fMinPhi, fMaxPhi) ;
            dr     = deltaR(eta, phi, eta_vtx, phi_vtx);
            
        }while(dr > drMax_);
        
        int PartID = fPartIDs[ip] ;
        const HepPDT::ParticleData*
        PData = fPDGTable->particle(HepPDT::ParticleID(abs(PartID))) ;
        double mass   = PData->mass().value() ;
        double theta  = 2.*atan(exp(-eta)) ;
        double mom    = pt/sin(theta) ;
        double px     = pt*cos(phi) ;
        double py     = pt*sin(phi) ;
        double pz     = mom*cos(theta) ;
        double energy2= mom*mom + mass*mass ;
        double energy = sqrt(energy2) ;
        HepMC::FourVector p(px,py,pz,energy) ;
        HepMC::GenParticle* Part =
        new HepMC::GenParticle(p,PartID,1);
        Part->suggest_barcode( barcode ) ;
        barcode++ ;
        Vtx->add_particle_out(Part);
        
        if ( fAddAntiParticle )
        {
            HepMC::FourVector ap(-px,-py,-pz,energy) ;
            int APartID = -PartID ;
            if ( PartID == 22 || PartID == 23 )
            {
                APartID = PartID ;
            }
            HepMC::GenParticle* APart =
            new HepMC::GenParticle(ap,APartID,1);
            APart->suggest_barcode( barcode ) ;
            barcode++ ;
            if( dr < drMax_ ) Vtx->add_particle_out(APart) ;
        }
        
    }
    
    fEvt->add_vertex(Vtx) ;
    fEvt->set_event_number(e.id().event()) ;
    fEvt->set_signal_process_id(20) ;
    
    if ( fVerbosity > 0 )
    {
        fEvt->print() ;
    }
    
    unique_ptr<HepMCProduct> BProduct(new HepMCProduct()) ;
    BProduct->addHepMCData( fEvt );
    e.put(std::move(BProduct), "unsmeared");
    
    unique_ptr<GenEventInfoProduct> genEventInfo(new GenEventInfoProduct(fEvt));
    e.put(std::move(genEventInfo));
    
    if ( fVerbosity > 0 )
    {
        // for testing purpose only
        // fEvt->print() ; // prints empty info after it's made into edm::Event
        cout << " FlatRandomPtAndD0GunProducer : Event Generation Done " << endl;
    }
}
//#include "FWCore/Framework/interface/MakerMacros.h"
//DEFINE_FWK_MODULE(FlatRandomPtAndD0GunProducer);
