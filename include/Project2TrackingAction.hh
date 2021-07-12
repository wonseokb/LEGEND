#ifndef LXeTrackingAction_h
#define LXeTrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "globals.hh"

class Project2RecorderBase;

class Project2TrackingAction : public G4UserTrackingAction {

  public:
    Project2TrackingAction(Project2RecorderBase*);
    virtual ~Project2TrackingAction() {};

    virtual void PreUserTrackingAction(const G4Track*);
    virtual void PostUserTrackingAction(const G4Track*);

  private:
    Project2RecorderBase* fRecorder;
};

#endif
