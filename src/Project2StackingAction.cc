#include "Project2StackingAction.hh"
#include "Project2UserEventInformation.hh"
#include "Project2SteppingAction.hh"

#include "G4RunManager.hh"

Project2StackingAction::Project2StackingAction() {}

Project2StackingAction::~Project2StackingAction() {}

G4ClassificationOfNewTrack Project2StackingAction::ClassifyNewTrack(const G4Track* aTrack)
{
  Project2UserEventInformation* eventInformation =
      (Project2UserEventInformation*) G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetUserInformation();
 
  if (aTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()) {
    if(aTrack->GetParentID()>0){
      if(aTrack->GetCreatorProcess()->GetProcessName() == "Scintillation")
        eventInformation->incrementPhotonCountScintillation();
      else if (aTrack->GetCreatorProcess()->GetProcessName()=="Cerenkov") {
        eventInformation->incrementPhotonCountCherenkov();
      }
    }
  }
  return fUrgent;
}

void Project2StackingAction::NewStage() {}

void Project2StackingAction::PrepareNewEvent() {}
