#include "Project1PmtHit.hh"
#include "G4VVisManager.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

G4Allocator<Project1PmtHit> Project1PmtHitAllocator;

Project1PmtHit::Project1PmtHit()
    : pmtNumber(-1),photons(0),physicalVolume(0),drawIt(false) {}

Project1PmtHit::~Project1PmtHit() {}

Project1PmtHit::Project1PmtHit(const Project1PmtHit &right) : G4VHit()
{
  pmtNumber = right.pmtNumber;
  photons = right.photons;
  physicalVolume = right.physicalVolume;
  drawIt = right.drawIt;
}

const Project1PmtHit& Project1PmtHit::operator=(const Project1PmtHit &right){
  pmtNumber = right.pmtNumber;
  photons = right.photons;
  physicalVolume = right.physicalVolume;
  drawIt = right.drawIt;
  return *this;
}

G4int Project1PmtHit::operator==(const Project1PmtHit &right) const{
  return (pmtNumber==right.pmtNumber);
}

void Project1PmtHit::Draw(){
  if(drawIt && physicalVolume){
    G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
    if (pVVisManager) {
      G4VisAttributes attributes(G4Colour(1.0, 0.0, 0.0));
      attributes.SetForceSolid(true);
      G4RotationMatrix rot;
      if (physicalVolume->GetRotation())
        rot = *(physicalVolume->GetRotation());
      G4Transform3D trans(rot, physicalVolume->GetTranslation());
    }
  }
}

void Project1PmtHit::Print() {
  G4cout << "hits" << G4endl;
}
