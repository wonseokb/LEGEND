#ifndef Project2SteppingAction_H
#define NovaSteppingACtion_H 1

#include "globals.hh"
#include "G4UserSteppingAction.hh"

#include "G4OpBoundaryProcess.hh"

class Project2RecorderBase;
class Project2EventAction;
class Project2TrackingAction;

class Project2SteppingAction : public G4UserSteppingAction
{
  public:
    Project2SteppingAction(Project2RecorderBase*);
    virtual ~Project2SteppingAction();
    virtual void UserSteppingAction(const G4Step*);

    void SetOneStepPrimaries(G4bool b){fOneStepPrimaries=b;}
    G4bool GetOneStepPrimaries(){return fOneStepPrimaries;}
 
  private:
    Project2RecorderBase* fRecorder;
    G4bool fOneStepPrimaries;
};

#endif
