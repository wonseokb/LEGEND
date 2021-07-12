#include "Project1Material1Sd.hh"
#include "G4VProcess.hh"

Project1Material1Sd::Project1Material1Sd(G4String name)
    : G4VSensitiveDetector(name)
{
  fScintCollection = NULL;
  collectionName.insert("Material1_Collection");
}

Project1Material1Sd::~Project1Material1Sd() {}

void Project1Material1Sd::Initialize(G4HCofThisEvent* hitsCE){
  fScintCollection = new NovaScintHitsCollection(SensitiveDetectorName, collectionName[0]);
  static G4int hitsCID = -1;
  if(hitsCID<0){
    hitsCID = GetCollectionID(0);
  }
  hitsCE->AddHitsCollection( hitsCID, fScintCollection );
}

G4bool Project1Material1Sd::ProcessHits(G4Step* aStep,G4TouchableHistory*){
  G4double totalEnergyDeposit = aStep->GetTotalEnergyDeposit();
  if(totalEnergyDeposit==0.0)
    return false;

  G4StepPoint* thePrePoint = aStep->GetPreStepPoint();
  G4TouchableHistory* theTouchable = (G4TouchableHistory*)(aStep->GetPreStepPoint()->GetTouchable());
  G4VPhysicalVolume* thePrePV = theTouchable->GetVolume();
  G4StepPoint* thePostPoint = aStep->GetPostStepPoint();

  G4ThreeVector position = (thePrePoint->GetPosition() + thePostPoint->GetPosition()) / 2.0;
  Project1Material1Hit* Material1_Hit = new Project1Material1Hit(thePrePV);
  Material1_Hit->SetEdep(totalEnergyDeposit);
  Material1_Hit->SetPos(position);

  fScintCollection->insert(Material1_Hit);

  return true;
}

void Project1Material1Sd::EndOfEvent(G4HCofThisEvent* ) {}

void Project1Material1Sd::clear() {}

void Project1Material1Sd::DrawAll() {}

void Project1Material1Sd::PrintAll() {}
