#ifndef NovaSteppingAction_H
#define NovaSteppingACtion_H 1

#include "globals.hh"
#include "G4UserSteppingAction.hh"

#include "G4OpBoundaryProcess.hh"

class NovaRecorderBase;
class NovaEventAction;
class NovaTrackingAction;

class NovaSteppingAction : public G4UserSteppingAction
{
  public:
    NovaSteppingAction(NovaRecorderBase*);
    virtual ~NovaSteppingAction();
    virtual void UserSteppingAction(const G4Step*);

    void SetOneStepPrimaries(G4bool b){fOneStepPrimaries=b;}
    G4bool GetOneStepPrimaries(){return fOneStepPrimaries;}
 
  private:
    NovaRecorderBase* fRecorder;
    G4bool fOneStepPrimaries;
};

#endif
