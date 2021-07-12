#ifndef Project2TrajectoryPoint_h
#define Project2TrajectoryPoint_h 1

#include "globals.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4TrajectoryPoint.hh"
#include "G4StepStatus.hh"


class G4Track;
class G4Step;
class G4VProcess;

class Project2TrajectoryPoint : public G4TrajectoryPoint
{
  public:
    Project2TrajectoryPoint();
    Project2TrajectoryPoint(const G4Track* aTrack);
    Project2TrajectoryPoint(const G4Step* aStep);
    Project2TrajectoryPoint(const Project2TrajectoryPoint &right);
    virtual ~Project2TrajectoryPoint();

    inline void *operator new(size_t);
    inline void operator delete(void *aTrajectoryPoint);
    inline int operator==(const Project2TrajectoryPoint& right) const { return (this==&right); };

    inline G4double GetTime() const { return fTime; };
    inline const G4ThreeVector GetMomentum() const { return fMomentum; };
    inline G4StepStatus GetStepStatus() const { return fStepStatus; };
    inline G4String GetVolumeName() const { return fVolumeName; };

    virtual const std::map<G4String,G4AttDef>* GetAttDefs() const;
    virtual std::vector<G4AttValue>* CreateAttValues() const;

  private:
    G4double fTime;
    G4ThreeVector fMomentum;
    G4StepStatus fStepStatus;
    G4String fVolumeName;
};

extern G4DLLEXPORT G4Allocator<Project2TrajectoryPoint> trajectoryPointAllocator;

inline void* Project2TrajectoryPoint::operator new(size_t)
{
  void *aTrajectoryPoint = (void*) trajectoryPointAllocator.MallocSingle();
  return aTrajectoryPoint;
}

inline void Project2TrajectoryPoint::operator delete(void *aTrajectoryPoint)
{
  trajectoryPointAllocator.FreeSingle((Project2TrajectoryPoint*) aTrajectoryPoint);
}

#endif
