#include "Project1Material1Hit.hh"
#include "G4VVisManager.hh"

G4Allocator<Project1Material1Hit> NovaScintHitAllocator;

Project1Material1Hit::Project1Material1Hit() : fEdep(0.), fPos(0.), fPhysVol(0) {}

Project1Material1Hit::Project1Material1Hit(G4VPhysicalVolume* pVol) : fPhysVol(pVol) {}

Project1Material1Hit::~Project1Material1Hit() {}

Project1Material1Hit::Project1Material1Hit(const Project1Material1Hit &right) : G4VHit()
{
  fEdep = right.fEdep;
  fPos = right.fPos;
  fPhysVol = right.fPhysVol;
}

const Project1Material1Hit& Project1Material1Hit::operator=(const Project1Material1Hit &right)
{
  fEdep = right.fEdep;
  fPos = right.fPos;
  fPhysVol = right.fPhysVol;
  return *this;
}

G4int Project1Material1Hit::operator==(const Project1Material1Hit&) const
{
  return false;
}

void Project1Material1Hit::Draw() {}

void Project1Material1Hit::Print() {}
