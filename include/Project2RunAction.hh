#include "G4UserRunAction.hh"
#include "Project2EventAction.hh"

#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"

#ifndef Project2RunAction_h
#define Project2RunAction_h 1

class Project2RecorderBase;

class Project2RunAction : public G4UserRunAction
{
  public:
    Project2RunAction(Project2RecorderBase*);
    virtual ~Project2RunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

    virtual void UpdateEventStatistics(EventStat stat);
    virtual void UpdateHitStatistics(HitStat stat);

  private:
    Project2RecorderBase* recorder;
    TFile* outputFilename;
    EventStat  eventStat;
    TTree*   eventTree;

    HitStat  hitStat;
    TTree*   hitTree;
};

#endif
