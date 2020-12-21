#include "NovaSteppingAction.hh"
#include "NovaEventAction.hh"
#include "NovaTrackingAction.hh"
#include "NovaPmtSd.hh"
#include "NovaUserTrackInformation.hh"
#include "NovaUserEventInformation.hh"
#include "NovaRecorderBase.hh"

#include "G4SteppingManager.hh"
#include "G4SDManager.hh"
#include "G4EventManager.hh"

NovaSteppingAction::NovaSteppingAction(NovaRecorderBase* r)
    : fRecorder(r),fOneStepPrimaries(false) {}

NovaSteppingAction::~NovaSteppingAction() {}

void NovaSteppingAction::UserSteppingAction(const G4Step* theStep)
{
  G4Track* theTrack = theStep->GetTrack();

  NovaUserTrackInformation* trackInformation = (NovaUserTrackInformation*) theTrack->GetUserInformation();
  NovaUserEventInformation* eventInformation
      = (NovaUserEventInformation*)G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetUserInformation();

  G4StepPoint* preStepPoint = theStep->GetPreStepPoint();
  G4StepPoint* postStepPoint = theStep->GetPostStepPoint();
  G4VPhysicalVolume* prePhysicalVolume = preStepPoint->GetPhysicalVolume();

  if (prePhysicalVolume->GetName() == "liquidScintillator" && theTrack->GetParentID() == 0) {
    eventInformation->addEnergyDepositionPrimary(theStep->GetTotalEnergyDeposit());
    eventInformation->addTrackLengthPrimary(theStep->GetStepLength());
  }

  static G4OpBoundaryProcess* boundary = 0;
  if (!boundary) {
    G4ProcessManager* processManager = theStep->GetTrack()->GetDefinition()->GetProcessManager();
    G4ProcessVector* processes = processManager->GetProcessList();
    for (G4int i = 0; i < processManager->GetProcessListLength(); i++) {
      if ((*processes)[i]->GetProcessName() == "OpBoundary") {
        boundary = (G4OpBoundaryProcess*)(*processes)[i];
        break;
      }
    }
  }

  G4ParticleDefinition* particleType = theTrack->GetDefinition();
  if (particleType == G4OpticalPhoton::OpticalPhotonDefinition()) {
    if (postStepPoint->GetProcessDefinedStep()->GetProcessName() == "OpAbsorption") {
      trackInformation->setTrackStatusFlag(absorbed);
      eventInformation->incrementAbsorption();//incrementAbsorption
    }

    if (postStepPoint->GetStepStatus() == fWorldBoundary) {
      eventInformation->incrementOutOfWorldCount();//incrementOutOfWorldCount
    }

    if (postStepPoint->GetStepStatus() == fGeomBoundary) {
      switch (boundary->GetStatus()) {//boundary
        case Absorption ://Absorption
          trackInformation->setTrackStatusFlag(boundaryAbsorbed);
          eventInformation->incrementBoundaryAbsorption();//incrementBoundaryAbsorption
          if (postStepPoint->GetPhysicalVolume()->GetName() == "photocathode") {
            eventInformation->incrementPhotocathodeAbsorptionCount();//incrementPhotocathodeAbsorptionCount
          }
          break;
        case Detection : {//Detection
          G4SDManager *sdManager = G4SDManager::GetSDMpointer();
          NovaPmtSd *pmtSd = (NovaPmtSd *) sdManager->FindSensitiveDetector("/NovaDet/pmtSd");
          if (pmtSd)
            pmtSd->ProcessHits_constStep(theStep, 0);//incrementPhotonCount
          trackInformation->setTrackStatusFlag(hitPmt);
          break;
        }
        case TotalInternalReflection ://TotalInternalReflection
          if(prePhysicalVolume->GetName() == "core")
            trackInformation->incrementTotalInternalReflectionCount();//incrementTotalInternalReflectionCount
          break;
        case LambertianReflection :
          trackInformation->incrementReflectionCount();//incrementReflectionCount
          break;
        case BackScattering ://BackScattering
        case FresnelReflection ://FresnelReflection
        case LobeReflection ://LobeReflection
        case SpikeReflection ://SpikeReflection
        default :
          break;
      }
    }
  }

  if(fRecorder)
    fRecorder->RecordStep(theStep);
}
