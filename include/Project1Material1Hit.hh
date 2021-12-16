#ifndef LXeScintHit_h
#define LXeScintHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4VPhysicalVolume.hh"


class Project1Material1Hit : public G4VHit
{
  public:
    Project1Material1Hit();
    Project1Material1Hit(G4VPhysicalVolume* pVol);
    virtual ~Project1Material1Hit();
    Project1Material1Hit(const Project1Material1Hit &right);
    const Project1Material1Hit& operator=(const Project1Material1Hit &right);
    G4int operator==(const Project1Material1Hit &right) const;

    inline void *operator new(size_t);
    inline void operator delete(void *aHit);
 
    virtual void Draw();
    virtual void Print();

    inline void SetEdep(G4double de) { fEdep = de; }
    inline void AddEdep(G4double de) { fEdep += de; }
    inline G4double GetEdep() { return fEdep; }

    inline void SetPos(G4ThreeVector xyz) { fPos = xyz; }
    inline G4ThreeVector GetPos() { return fPos; }

    inline const G4VPhysicalVolume * GetPhysV() { return fPhysVol; }

  private:
    G4double fEdep;
    G4ThreeVector fPos;
    const G4VPhysicalVolume* fPhysVol;
};

typedef G4THitsCollection<Project1Material1Hit> NovaScintHitsCollection;

extern G4Allocator<Project1Material1Hit> NovaScintHitAllocator;

inline void* Project1Material1Hit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) NovaScintHitAllocator.MallocSingle();
  return aHit;
}

inline void Project1Material1Hit::operator delete(void *aHit)
{
  NovaScintHitAllocator.FreeSingle((Project1Material1Hit*) aHit);
}

#endif
