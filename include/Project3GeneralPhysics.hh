#ifndef Project3GeneralPhysics_h
#define Project3GeneralPhysics_h 1

#include "globals.hh"
#include "G4ios.hh"
#include "G4VPhysicsConstructor.hh"
#include "G4Decay.hh"


class Project3GeneralPhysics : public G4VPhysicsConstructor
{
  public:
    Project3GeneralPhysics(const G4String& name = "general");
    virtual ~Project3GeneralPhysics();
    virtual void ConstructParticle();
    virtual void ConstructProcess();

  protected:
    G4Decay* fDecayProcess;
};

#endif
