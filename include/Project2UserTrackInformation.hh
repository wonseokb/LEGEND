#ifndef Project2UserTrackInformation_h
#define Project2UserTrackInformation_h 1

#include "G4VUserTrackInformation.hh"
#include "globals.hh"


enum Project2TrackStatus
{
  hitPmt=1,
  absorbed=2,
  boundaryAbsorbed=4,
};

class Project2UserTrackInformation : public G4VUserTrackInformation
{
  public:
    Project2UserTrackInformation();
    virtual ~Project2UserTrackInformation();
    void setTrackStatusFlag(int s){trackStatus=s;}
    void incrementReflectionCount(){reflectionCount++;}
    void incrementTotalInternalReflectionCount(){totalInternalReflectionCount++;}
    int getTrackStatus()const {return trackStatus;}
    G4int getReflectionCount()const {return reflectionCount;}
    G4int getTotalInternalReflectionCount()const {return totalInternalReflectionCount;}

  private:
    int trackStatus;
    G4int reflectionCount;
    G4int totalInternalReflectionCount;
    G4bool forceDraw;
};

#endif
