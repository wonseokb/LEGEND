#include "Project2TrackingAction.hh"
#include "Project2UserTrackInformation.hh"
#include "Project2RecorderBase.hh"
#include "G4TrackingManager.hh"
#include "Project2Trajectory.hh"


Project2TrackingAction::Project2TrackingAction(Project2RecorderBase* r)
  : fRecorder(r) {}

void Project2TrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
  fpTrackingManager->SetStoreTrajectory(true);
  fpTrackingManager->SetTrajectory(new Project2Trajectory(aTrack));
  fpTrackingManager->SetUserTrackInformation(new Project2UserTrackInformation);
}

void Project2TrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{
  Project2Trajectory* trajectory=(Project2Trajectory*)fpTrackingManager->GimmeTrajectory();
  Project2UserTrackInformation* trackInformation =(Project2UserTrackInformation*) aTrack->GetUserInformation();

  trajectory->SetStatus(trackInformation->getTrackStatus());
  trajectory->setTrackLength(aTrack->GetTrackLength());
  if(aTrack->GetParentID()>0)
    trajectory->SetProcessName(aTrack->GetCreatorProcess()->GetProcessName());
  trajectory->setReflectionCount(trackInformation->getReflectionCount());
  trajectory->setTotalInternalReflectionCount(trackInformation->getTotalInternalReflectionCount());

  if(fRecorder)
    fRecorder->RecordTrack(aTrack);
}
