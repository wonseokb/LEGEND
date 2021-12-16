#include "Project3PhysicsList.hh"
#include "Project3GeneralPhysics.hh"
#include "Project3EmPhysics.hh"
#include "Project3MuonPhysics.hh"
#include "G4OpticalPhysics.hh"
#include "G4SystemOfUnits.hh"


Project3PhysicsList::Project3PhysicsList() : G4VModularPhysicsList()
{
  // default cut value  (1.0mm)
  defaultCutValue = 1.0*mm;

  // General Physics
  RegisterPhysics(new Project3GeneralPhysics("general"));

  // EM Physics
  RegisterPhysics(new Project3EmPhysics("standard EM"));

  // Muon Physics
  RegisterPhysics(new Project3MuonPhysics("muon"));

  // Optical Physics
  G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics();
  RegisterPhysics(opticalPhysics);

  opticalPhysics->SetWLSTimeProfile("exponential");

  opticalPhysics->SetScintillationYieldFactor(1.0);
  opticalPhysics->SetScintillationExcitationRatio(0.0);

  opticalPhysics->SetMaxNumPhotonsPerStep(100);
  opticalPhysics->SetMaxBetaChangePerStep(10.0);

  opticalPhysics->SetTrackSecondariesFirst(kCerenkov, true);
  opticalPhysics->SetTrackSecondariesFirst(kScintillation, true);
}

Project3PhysicsList::~Project3PhysicsList() {}

void Project3PhysicsList::SetCuts(){
  SetCutsWithDefault();
}
