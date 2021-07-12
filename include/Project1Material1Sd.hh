#ifndef LXeScintSD_h
#define LXeScintSD_h 1

#include "Project1Material1Hit.hh"
#include "G4VSensitiveDetector.hh"

class G4Step;
class G4HCofThisEvent;

class Project1Material1Sd : public G4VSensitiveDetector
{
  public:

    Project1Material1Sd(G4String name);
    virtual ~Project1Material1Sd();

    virtual void Initialize(G4HCofThisEvent* );
    virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* );
    virtual void EndOfEvent(G4HCofThisEvent* );
    virtual void clear();
    virtual void DrawAll();
    virtual void PrintAll();
 
  private:

    NovaScintHitsCollection* fScintCollection;
 
};

#endif
