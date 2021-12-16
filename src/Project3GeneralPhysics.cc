#include "Project3GeneralPhysics.hh"
#include "G4ProcessManager.hh"
#include "G4ChargedGeantino.hh"
#include "G4Geantino.hh"


Project3GeneralPhysics::Project3GeneralPhysics(const G4String& name)
    : G4VPhysicsConstructor(name) {}

Project3GeneralPhysics::~Project3GeneralPhysics() {
  fDecayProcess = NULL;
}

void Project3GeneralPhysics::ConstructParticle()
{
  G4Geantino::GeantinoDefinition();
  G4ChargedGeantino::ChargedGeantinoDefinition();
}

void Project3GeneralPhysics::ConstructProcess()
{
  fDecayProcess = new G4Decay();

  auto particleIterator = GetParticleIterator();
  particleIterator->reset();
  while( (*particleIterator)() ){
    G4ParticleDefinition* particle = particleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    if (fDecayProcess->IsApplicable(*particle)) {
      pmanager ->AddProcess(fDecayProcess);
      pmanager ->SetProcessOrdering(fDecayProcess, idxPostStep);
      pmanager ->SetProcessOrdering(fDecayProcess, idxAtRest);
    }
  }
}
