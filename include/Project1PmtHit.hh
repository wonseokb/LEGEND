#ifndef Project1PmtHit_h
#define Project1PmtHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4VPhysicalVolume.hh"

class G4VTouchable;

class Project1PmtHit : public G4VHit
{
  public:
    Project1PmtHit();
    virtual ~Project1PmtHit();
    Project1PmtHit(const Project1PmtHit &right);

    const Project1PmtHit& operator=(const Project1PmtHit &right);
    G4int operator==(const Project1PmtHit &right) const;

    inline void *operator new(size_t);
    inline void operator delete(void *aHit);
 
    virtual void Draw();
    virtual void Print();

    inline void setDrawIt(G4bool b){drawIt=b;}
    inline G4bool getDrawIt(){return drawIt;}

    inline void incrementPhotonCount(){photons++;}
    inline G4int getPhotonCount(){return photons;}

    inline void SetPmtNumber(G4int n) { pmtNumber = n; }
    inline G4int getPmtNumber() { return pmtNumber; }

    inline void setPmtPhysicalVolume(G4VPhysicalVolume *physVol){this->physicalVolume = physVol;}
    inline G4VPhysicalVolume* getPmtPhysicalVolume() {return physicalVolume;}
    inline void setPmtPosition(G4double x, G4double y, G4double z) {position = G4ThreeVector(x, y, z);}
    inline G4ThreeVector getPmtPosition() {return position;}

  private:
    G4int pmtNumber;
    G4int photons;
    G4ThreeVector position;
    G4VPhysicalVolume* physicalVolume;
    G4bool drawIt;
};

typedef G4THitsCollection<Project1PmtHit> Project1PmtHitsCollection;

extern G4Allocator<Project1PmtHit> Project1PmtHitAllocator;

inline void* Project1PmtHit::operator new(size_t){
  void *aHit;
  aHit = (void *) Project1PmtHitAllocator.MallocSingle();
  return aHit;
}

inline void Project1PmtHit::operator delete(void *aHit){
  Project1PmtHitAllocator.FreeSingle((Project1PmtHit*) aHit);
}

#endif
