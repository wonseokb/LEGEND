#include <G4TrajectoryContainer.hh>
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4AttDefStore.hh"
#include "G4UIcommand.hh"
#include "G4UnitsTable.hh"
#include "Project2Trajectory.hh"
#include "Project2TrajectoryPoint.hh"
#include "G4ParticleTable.hh"
#ifdef G4ATTDEBUG
#include "G4AttCheck.hh"
#endif


G4Allocator<Project2Trajectory> myTrajectoryAllocator;

Project2Trajectory::Project2Trajectory()
    : trajectoryPointContainer(0), trackId(0), parentID(0), pdgCharge(0.0), pdgEncoding(0),
      particleName(""), initialMomentum(G4ThreeVector()) {}

Project2Trajectory::Project2Trajectory(const G4Track* aTrack)
{
  G4ParticleDefinition * particleDefinition = aTrack->GetDefinition();
  particleName = particleDefinition->GetParticleName();
  pdgCharge = particleDefinition->GetPDGCharge();
  pdgEncoding = particleDefinition->GetPDGEncoding();
  trackId = aTrack->GetTrackID();
  parentID = aTrack->GetParentID();
  initialMomentum = aTrack->GetMomentum();
  trajectoryPointContainer = new TrajectoryPointContainer();
  trajectoryPointContainer->push_back(new Project2TrajectoryPoint(aTrack));
}

Project2Trajectory::Project2Trajectory(Project2Trajectory &right) : G4VTrajectory()
{
  particleName = right.particleName;
  pdgCharge = right.pdgCharge;
  pdgEncoding = right.pdgEncoding;
  trackId = right.trackId;
  parentID = right.parentID;
  initialMomentum = right.initialMomentum;
  trajectoryPointContainer = new TrajectoryPointContainer();

  for(size_t i=0;i<right.trajectoryPointContainer->size();++i) {
    Project2TrajectoryPoint* rightPoint = (Project2TrajectoryPoint*)((*(right.trajectoryPointContainer))[i]);
    trajectoryPointContainer->push_back(new Project2TrajectoryPoint(*rightPoint));
  }
}

Project2Trajectory::~Project2Trajectory()
{
  for (size_t i=0;i<trajectoryPointContainer->size();++i) {
    delete  (*trajectoryPointContainer)[i];
  }
  trajectoryPointContainer->clear();

  delete trajectoryPointContainer;
}

void Project2Trajectory::ShowTrajectory(std::ostream& os) const
{
  G4VTrajectory::ShowTrajectory(os);
}

void Project2Trajectory::DrawTrajectory() const
{
  G4VTrajectory::DrawTrajectory();
}

void Project2Trajectory::AppendStep(const G4Step* aStep)
{
  trajectoryPointContainer->push_back(new Project2TrajectoryPoint(aStep));
}

G4ParticleDefinition* Project2Trajectory::GetParticleDefinition()
{
  return (G4ParticleTable::GetParticleTable()->FindParticle(particleName));
}

void Project2Trajectory::MergeTrajectory(G4VTrajectory* secondTrajectory)
{
  if (!secondTrajectory)
    return;

  Project2Trajectory* second = (Project2Trajectory*) secondTrajectory;
  G4int pointEntryCount = second->GetPointEntries();
  for(G4int i=1; i<pointEntryCount; ++i) {
    trajectoryPointContainer->push_back((*(second->trajectoryPointContainer))[i]);
  }
  delete (*second->trajectoryPointContainer)[0];
  second->trajectoryPointContainer->clear();
}

const std::map<G4String,G4AttDef>* Project2Trajectory::GetAttDefs() const
{
  G4bool isNew;
  std::map<G4String,G4AttDef>* store = G4AttDefStore::GetInstance("Trajectory", isNew);

  if (isNew) {
    G4String ID("ID");
    (*store)[ID] = G4AttDef(ID, "Track ID", "Bookkeeping", "", "G4int");

    G4String PID("PID");
    (*store)[PID] = G4AttDef(PID, "Parent ID", "Bookkeeping", "", "G4int");

    G4String PN("PN");
    (*store)[PN] = G4AttDef(PN, "Particle Name", "Physics", "", "G4String");

    G4String Ch("Ch");
    (*store)[Ch] = G4AttDef(Ch, "Charge", "Physics", "e+", "G4double");

    G4String PDG("PDG");
    (*store)[PDG] = G4AttDef(PDG, "PDG Encoding", "Physics", "", "G4int");

    G4String IMom("IMom");
    (*store)[IMom] = G4AttDef(IMom,
                              "Momentum of track at start of trajectory",
                              "Physics", "G4BestUnit", "G4ThreeVector");

    G4String IMag("IMag");
    (*store)[IMag] = G4AttDef(IMag,
                              "Magnitude of momentum of track at start of trajectory",
                              "Physics", "G4BestUnit", "G4double");

    G4String NTP("NTP");
    (*store)[NTP] = G4AttDef(NTP, "No. of points", "Bookkeeping", "", "G4int");

    G4String Status("Status");
    (*store)[Status] = G4AttDef(Status, "Track status", "Bookkeeping", "", "int");
  }
  return store;
}

std::vector<G4AttValue>* Project2Trajectory::CreateAttValues() const
{
  std::vector<G4AttValue>* values = new std::vector<G4AttValue>;
  values->push_back(G4AttValue("ID", G4UIcommand::ConvertToString(trackId), ""));
  values->push_back(G4AttValue("PID", G4UIcommand::ConvertToString(parentID), ""));
  values->push_back(G4AttValue("PN", particleName, ""));
  values->push_back(G4AttValue("Ch", G4UIcommand::ConvertToString(pdgCharge), ""));
  values->push_back(G4AttValue("PDG", G4UIcommand::ConvertToString(pdgEncoding), ""));
  values->push_back(G4AttValue("IMom", G4BestUnit(initialMomentum,"Energy"), ""));
  values->push_back(G4AttValue("IMag", G4BestUnit(initialMomentum.mag(),"Energy"), ""));
  values->push_back(G4AttValue("NTP", G4UIcommand::ConvertToString(GetPointEntries()), ""));

#ifdef G4ATTDEBUG
  G4cout << G4AttCheck(values,GetAttDefs());
#endif
  return values;
}

Project2Trajectory* Project2Trajectory::getParentTrajectory(G4TrajectoryContainer* trajectoryContainer)
{
  for (size_t i = 0; i < trajectoryContainer->size(); i++) {
    Project2Trajectory* trajectory = (Project2Trajectory*) (*trajectoryContainer)[i];
    if (trajectory->GetTrackID() == this->GetParentID()) {
      return trajectory;
    }
  }
  return 0;
}
