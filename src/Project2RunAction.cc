#include "Project2RunAction.hh"
#include "Project2RecorderBase.hh"


Project2RunAction::Project2RunAction(Project2RecorderBase* r) : recorder(r) {}

Project2RunAction::~Project2RunAction() {}

void Project2RunAction::BeginOfRunAction(const G4Run* aRun){
  if(recorder)
    recorder->RecordBeginOfRun(aRun);

  outputFilename = new TFile("Project.root", "recreate");

  eventTree = new TTree("eventTree", "Event Statistics");
  eventTree->Branch("eventStat",
                  &eventStat.scintillationPhotonCount,
                  "scintillationPhotonCount/I:cherenkovPhotonCount:hitCount:pmtAboveThresholdCount:absorptionCount:boundaryAbsorptionCount:photocathodeAbsorptionCount:outOfWorldCount:unacountedCount:primaryPdg:primaryX/D:primaryY:primaryZ:primaryPX:primaryPY:primaryPZ:energyDepositionPrimary:trackLengthPrimary:energyDeposition:energyDepositionX:energyDepositionY:energyDepositionZ");

  hitTree = new TTree("hitTree", "Hit Statistics");
  hitTree->Branch("hitStat",
                  &hitStat.eventId,
                  "eventId/I:wlsCount:reflectionCount:totalInternalReflectionCount:beginTime/D:beginX:beginY:beginZ:beginE:beginPX:beginPY:beginPZ:beginWavelength:enterTime:enterX:enterY:enterZ:enterE:enterPX:enterPY:enterPZ:enterWavelength:hitTime:hitX:hitY:hitZ:hitE:hitPX:hitPY:hitPZ:hitWavelength:trackLength:hitEnteringAngle:wlsEmittingAngle");//edit

}

void Project2RunAction::EndOfRunAction(const G4Run* aRun)
{
  if(recorder)
    recorder->RecordEndOfRun(aRun);

  eventTree->Write();
  hitTree->Write();
  outputFilename->Close();
}

void Project2RunAction::UpdateEventStatistics(EventStat stat)
{
  eventStat = stat;
  eventTree->Fill();
}

void Project2RunAction::UpdateHitStatistics(HitStat stat)
{
  hitStat = stat;
  hitTree->Fill();
}  
