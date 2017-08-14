#ifndef FlatRandomPtAndD0GunProducer_H
#define FlatRandomPtAndD0GunProducer_H

/** \class FlatRandomPtAndD0GunProducer
 *
 * Generates single particle gun in HepMC format
 * The d0 is taken by convention equal to dx.
 ***************************************/

#include "IOMC/ParticleGuns/interface/BaseFlatGunProducer.h"

namespace edm
{
    
    class FlatRandomPtAndD0GunProducer : public BaseFlatGunProducer
    {
        
    public:
        FlatRandomPtAndD0GunProducer(const ParameterSet & pset);
        virtual ~FlatRandomPtAndD0GunProducer();
        
        virtual void produce(Event & e, const EventSetup& es) override;
        
    private:
        
        // data members
        
        double            fMinPt   ;
        double            fMaxPt   ;
        double            LMin_   ;
        double            LMax_   ;
        double            drMax_   ;

        
    };
} 

#endif