#ifndef Project3PhysicsList_h
#define Project3PhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"


class Project3PhysicsList: public G4VModularPhysicsList
{
  public:
    Project3PhysicsList();
    virtual ~Project3PhysicsList();

  public:
    virtual void SetCuts();
};

#endif
