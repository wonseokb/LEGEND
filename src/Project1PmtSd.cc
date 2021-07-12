#include "Project1PmtSd.hh"
#include "Project1DetectorConstruction.hh"

#include "G4ParticleTypes.hh"


Project1PmtSd::Project1PmtSd(G4String name)
  : G4VSensitiveDetector(name),
    pmtHitsCollection(0),
    pmtPositionXs(0),
    pmtPositionYs(0),
    pmtPositionZs(0)
{
  collectionName.insert("pmtHitCollection");
}

Project1PmtSd::~Project1PmtSd() {}

void Project1PmtSd::Initialize(G4HCofThisEvent* hitsCE)
{
  pmtHitsCollection = new Project1PmtHitsCollection(SensitiveDetectorName,collectionName[0]);
  static G4int hitCID = -1;
  if(hitCID<0){
    hitCID = GetCollectionID(0);
  }
  hitsCE->AddHitsCollection( hitCID, pmtHitsCollection );
}

G4bool Project1PmtSd::ProcessHits(G4Step* ,G4TouchableHistory* )
{
  return false;
}

G4bool Project1PmtSd::ProcessHits_constStep(const G4Step* aStep, G4TouchableHistory*)
{
  if(aStep->GetTrack()->GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition())
    return false;

  Project1PmtHit* hit = NULL;
  G4int pmtNumber = aStep->GetPostStepPoint()->GetTouchable()->GetReplicaNumber(1);
  G4VPhysicalVolume* physicalVolume = aStep->GetPostStepPoint()->GetTouchable()->GetVolume(1);

  G4int n = pmtHitsCollection->entries();
  for (G4int i = 0; i < n; i++) {
    if ((*pmtHitsCollection)[i]->getPmtNumber() == pmtNumber) {
      hit = (*pmtHitsCollection)[i];
      break;
    }
  }
 
  if (hit == NULL) {
    hit = new Project1PmtHit();
    hit->SetPmtNumber(pmtNumber);
    hit->setPmtPhysicalVolume(physicalVolume);
    pmtHitsCollection->insert(hit);
    hit->setPmtPosition((*pmtPositionXs)[pmtNumber], (*pmtPositionYs)[pmtNumber], (*pmtPositionZs)[pmtNumber]);
  }

  hit->incrementPhotonCount();
  return true;
}

void Project1PmtSd::EndOfEvent(G4HCofThisEvent*) {}

void Project1PmtSd::clear() {}

void Project1PmtSd::DrawAll() {}

void Project1PmtSd::PrintAll() {}
