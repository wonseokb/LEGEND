#ifndef LXeStackingAction_H
#define LXeStackingAction_H 1

#include "globals.hh"
#include "G4UserStackingAction.hh"

class Project2StackingAction : public G4UserStackingAction
{
  public:
    Project2StackingAction();
    virtual ~Project2StackingAction();
 
    virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);
    virtual void NewStage();
    virtual void PrepareNewEvent();
 };

#endif
