#include <Project2UserTrackInformation.hh>
#include <G4OpticalPhoton.hh>
#include "Project2EventAction.hh"
#include "Project1Material1Hit.hh"
#include "Project1PmtHit.hh"
#include "Project2UserEventInformation.hh"
#include "Project2RecorderBase.hh"
#include "G4EventManager.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4VVisManager.hh"
#include "G4SystemOfUnits.hh"
#include "Project2RunAction.hh"
#include "Project2TrajectoryPoint.hh"

//#include <iostream>//edit Aug31,2020
//using namespace std;//edit 
#include "G4UnitsTable.hh"
#include "G4SteppingManager.hh"
#include "G4UnitsTable.hh"

//edit
/*
#include "G4DynamicParticle.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4ProcessManager.hh"
#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4OpticalPhoton.hh"
#include "G4Material.hh"
#include "G4Trajectory.hh"
*/
#include "Project1PmtSd.hh"
#include "Project1DetectorConstruction.hh"
#include "G4ParticleTypes.hh"

std::ofstream myfile_HitData1("DetectedPhotons.txt", std::ios_base::app);
G4double DetectedPhotonCount = 0;

Project2EventAction::Project2EventAction(Project2RecorderBase* r)
    : recorder(r),saveThreshold(0),scintCollectionId(-1),
      pmtCollectionId(-1),verbose(0),pmtThreshold(1) {}

Project2EventAction::~Project2EventAction(){}

void Project2EventAction::BeginOfEventAction(const G4Event* anEvent)
{
  G4EventManager::GetEventManager()->SetUserInformation(new Project2UserEventInformation);

  G4SDManager* sdManager = G4SDManager::GetSDMpointer();
  if (scintCollectionId < 0)
    scintCollectionId = sdManager->GetCollectionID("Material1_Collection");
  if (pmtCollectionId < 0)
    pmtCollectionId = sdManager->GetCollectionID("pmtHitCollection");
  if (recorder)
    recorder->RecordBeginOfEvent(anEvent);
}

void Project2EventAction::EndOfEventAction(const G4Event* anEvent)
{
  Project2UserEventInformation* eventInformation = (Project2UserEventInformation*) anEvent->GetUserInformation();
  G4TrajectoryContainer* trajectoryContainer = anEvent->GetTrajectoryContainer();
  G4int nTrajectories = 0;
  if (trajectoryContainer)
    nTrajectories = trajectoryContainer->entries();

  NovaScintHitsCollection* scintHitsCollection = 0;
  Project1PmtHitsCollection* pmtHitsCollection = 0;
  G4HCofThisEvent* hcOfThisEvent = anEvent->GetHCofThisEvent();

  if (hcOfThisEvent) {
    if (scintCollectionId >= 0)
      scintHitsCollection = (NovaScintHitsCollection*)(hcOfThisEvent->GetHC(scintCollectionId));
    if (pmtCollectionId >= 0)
      pmtHitsCollection = (Project1PmtHitsCollection*)(hcOfThisEvent->GetHC(pmtCollectionId));
  }

  if (scintHitsCollection) {
    int nHit = scintHitsCollection->entries();
    G4ThreeVector energyWeightedPosition(0.0);
    G4double energyDepositionMax = 0.0;

    for(int i = 0; i < nHit; i++) {
      G4double energyDeposition = (*scintHitsCollection)[i]->GetEdep();
      eventInformation->addEnergyDeposition(energyDeposition);
      energyWeightedPosition += (*scintHitsCollection)[i]->GetPos() * energyDeposition;
      if (energyDeposition > energyDepositionMax) {
        energyDepositionMax = energyDeposition;
        G4ThreeVector positionMax = (*scintHitsCollection)[i]->GetPos();
        eventInformation->setPositionMax(positionMax, energyDepositionMax);
      }
    }

    if (eventInformation->getEnergyDeposition() == 0.0) {
      if(verbose > 0)
        G4cout << "No hits in the scintillator this event." << G4endl;
    }
    else {
      energyWeightedPosition /= eventInformation->getEnergyDeposition();
      eventInformation->setEnergyWeightedPosition(energyWeightedPosition);
    }
  }

  if(pmtHitsCollection){
    G4int pmtHitCount = pmtHitsCollection->entries();
    for (G4int i = 0; i < pmtHitCount; i++) {
      eventInformation->incrementHitCount((*pmtHitsCollection)[i]->getPhotonCount());
      if ((*pmtHitsCollection)[i]->getPhotonCount() >= pmtThreshold) {
        eventInformation->incrementPmtCountAboveThreshold();
      }
      else
        (*pmtHitsCollection)[i]->setDrawIt(false);
    }
    pmtHitsCollection->DrawAllHits();
  }

  Project2RunAction* runAction = (Project2RunAction*) G4RunManager::GetRunManager()->GetUserRunAction();

  eventStat.scintillationPhotonCount = eventInformation->getScintillationPhotonCount();
  eventStat.cherenkovPhotonCount = eventInformation->getCherenkovPhotonCount();
  eventStat.hitCount = eventInformation->getHitCount();
  eventStat.pmtAboveThresholdCount = eventInformation->getPmtAboveThresholdCount();
  eventStat.absorptionCount = eventInformation->getAbsorptionCount();
  eventStat.boundaryAbsorptionCount = eventInformation->getBoundaryAbsorptionCount();
  eventStat.photocathodeAbsorptionCount = eventInformation->getPhotocathodeAbsorptionCount();
  eventStat.outOfWorldCount = eventInformation->getOutOfWorldCount();
  eventStat.unacountedCount = eventInformation->getCherenkovPhotonCount()
                            + eventInformation->getScintillationPhotonCount()
                            - eventInformation->getAbsorptionCount()
                            - eventInformation->getHitCount()
                            - eventInformation->getBoundaryAbsorptionCount()
                            - eventInformation->getOutOfWorldCount();
  eventStat.energyDeposition  = eventInformation->getEnergyDeposition() / MeV;
  eventStat.trackLengthPrimary = eventInformation->getTrackLengthPrimary() / cm;
  eventStat.energyDepositionPrimary = eventInformation->getEnergyDepositionPrimary() / MeV;
  eventStat.energyDepositionX = eventInformation->getEnergyWeightedPosition().getX();
  eventStat.energyDepositionY = eventInformation->getEnergyWeightedPosition().getY();
  eventStat.energyDepositionZ = eventInformation->getEnergyWeightedPosition().getZ();
  hitStat.eventId = anEvent->GetEventID();

  for (G4int i = 0; i < nTrajectories; i++) {
    Project2Trajectory* trajectory = (Project2Trajectory*) (*trajectoryContainer)[i];

    if (trajectory->GetParentID() == 0) {
      Project2TrajectoryPoint* trajectoryPoint = (Project2TrajectoryPoint*) trajectory->GetPoint(0);
      G4ThreeVector position = trajectoryPoint->GetPosition();
      eventStat.primaryPdg = trajectory->GetPDGEncoding();
      eventStat.primaryPX = trajectory->GetInitialMomentum().getX();
      eventStat.primaryPY = trajectory->GetInitialMomentum().getY();
      eventStat.primaryPZ = trajectory->GetInitialMomentum().getZ();
      eventStat.primaryX = position.getX();
      eventStat.primaryY = position.getY();
      eventStat.primaryZ = position.getZ();
    }

    if(trajectory->GetStatus() == hitPmt){
      Project2TrajectoryPoint* lastTrajectoryPoint = (Project2TrajectoryPoint*)trajectory->GetPoint(trajectory->GetPointEntries() - 1);
      hitStat.hitTime = lastTrajectoryPoint->GetTime();
      hitStat.hitX = lastTrajectoryPoint->GetPosition().getX();
      hitStat.hitY = lastTrajectoryPoint->GetPosition().getY();
      hitStat.hitZ = lastTrajectoryPoint->GetPosition().getZ();
      hitStat.hitE = lastTrajectoryPoint->GetMomentum().getR() / eV;
      hitStat.hitPX = lastTrajectoryPoint->GetMomentum().getX() / eV;
      hitStat.hitPY = lastTrajectoryPoint->GetMomentum().getY() / eV;
      hitStat.hitPZ = lastTrajectoryPoint->GetMomentum().getZ() / eV;
      //if( lastTrajectoryPoint->GetMomentum().getR() / eV < 2.95){
      hitStat.hitWavelength = 1239.84 / (lastTrajectoryPoint->GetMomentum().getR() / eV);
      	DetectedPhotonCount++;
      	myfile_HitData1 << DetectedPhotonCount << std::endl;

      //hitStat.enteringAngle = atan(sqrt(hitPX*hitPX + hitPY*hitPY)/hitPZ);//edit
      //}

      hitStat.wlsCount = 0;
      hitStat.trackLength = trajectory->getTrackLength();
      hitStat.reflectionCount = trajectory->getReflectionCount();
      hitStat.totalInternalReflectionCount = trajectory->getTotalInternalReflectionCount();
      incrementWlsCount(trajectory);

      Project2Trajectory* parentTrajectory = trajectory->getParentTrajectory(trajectoryContainer);
      while (parentTrajectory != 0 && parentTrajectory->GetParticleDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()) {
        hitStat.trackLength += parentTrajectory->getTrackLength();
        hitStat.reflectionCount += parentTrajectory->getReflectionCount();
        hitStat.totalInternalReflectionCount += parentTrajectory->getTotalInternalReflectionCount();
        incrementWlsCount(parentTrajectory);
        parentTrajectory = parentTrajectory->getParentTrajectory(trajectoryContainer);
      }

      //hitStat.enteringAngle = atan(sqrt(hitStat.hitPX*hitStat.hitPX + hitStat.hitPY*hitStat.hitPY)/hitStat.hitPZ);//edit

      runAction->UpdateHitStatistics(hitStat);
    }
  }

  if (anEvent->GetEventID() % 100 == 0) {
    std::cout << "anEvent->GetEventID() = " << anEvent->GetEventID() << std::endl;
  }
  // printEventInfo();

  runAction->UpdateEventStatistics(eventStat);
  if (saveThreshold && eventInformation->getPhotonCount() <= saveThreshold)
    G4RunManager::GetRunManager()->rndmSaveThisEvent();

  if(recorder)
    recorder->RecordEndOfEvent(anEvent);
}

void Project2EventAction::printEventInfo()
{
  G4cout << "eventStat.scintillationPhotonCount = " << eventStat.scintillationPhotonCount << G4endl;
  G4cout << "eventStat.cherenkovPhotonCount = " << eventStat.cherenkovPhotonCount << G4endl;
  G4cout << "eventStat.hitCount = " << eventStat.hitCount << G4endl << G4endl;
}

void Project2EventAction::SetSaveThreshold(G4int save)
{
  saveThreshold = save;
  G4RunManager::GetRunManager()->SetRandomNumberStore(true);
  G4RunManager::GetRunManager()->SetRandomNumberStoreDir("random/");
}

void Project2EventAction::incrementWlsCount(Project2Trajectory* trajectory)
{
  if(trajectory->GetProcessName() == "OpWLS"){
    hitStat.wlsCount++;
    hitStat.hitEnteringAngle = atan(sqrt(hitStat.hitPX*hitStat.hitPX + hitStat.hitPY*hitStat.hitPY)/hitStat.hitPZ)*57.2958;//edit
    //57.2958 = 180 / pi
    //CLHEP::Hep3Vector MomDir = aStep->GetTrack()->GetMomentumDirection();
    /*
    hitStat.beginPX = trajectory->GetMomentum().getX() / eV;//edit, problematic
    hitStat.beginPY = trajectory->GetMomentum().getY() / eV;//edit, problematic
    hitStat.beginPZ = trajectory->GetMomentum().getZ() / eV;//edit, problematic
    */
    hitStat.wlsEmittingPX = trajectory->GetInitialMomentum().getX();
    hitStat.wlsEmittingPY = trajectory->GetInitialMomentum().getY();
    hitStat.wlsEmittingPZ = trajectory->GetInitialMomentum().getZ();
    hitStat.wlsEmittingAngle = atan(sqrt(hitStat.wlsEmittingPX*hitStat.wlsEmittingPX + hitStat.wlsEmittingPY*hitStat.wlsEmittingPY)/hitStat.wlsEmittingPZ)*57.2958;//edit

    //std::cout << "Project2EventAction.cc works " << std::endl; //This works
    }
}
