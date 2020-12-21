#ifndef NovaPrimaryGeneratorAction_h
#define NovaPrimaryGeneratorAction_h 1

#include <G4GeneralParticleSource.hh>
#include "G4VUserPrimaryGeneratorAction.hh"

#include "G4ThreeVector.hh"
#include "G4DataVector.hh"
#include "Randomize.hh"
//#include "CRYSetup.h"
//#include "CRYGenerator.h"
//#include "CRYParticle.h"
//#include "CRYUtils.h"
#include "vector"
#include "RNGWrapper.hh"
#include "NovaPrimaryGeneratorMessenger.hh"
#include "G4ParticleTable.hh"

class G4ParticleGun;
class G4Event;

class NovaPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:

    NovaPrimaryGeneratorAction(const char *inputFilename);
    virtual ~NovaPrimaryGeneratorAction();

  public:
    virtual void GeneratePrimaries(G4Event* anEvent);

  public:
    void InputCRY();
    void UpdateCRY(std::string* MessInput);
    void CRYFromFile(G4String newValue);
    void useCRY(G4bool useCry);

  private:
    //std::vector<CRYParticle*> *cryParticles;
    G4ParticleTable* particleTable;
    G4ParticleGun* particleGun;
    G4GeneralParticleSource* generalParticleSource;

    //CRYGenerator* cryGenerator;
    G4int inputState;
    NovaPrimaryGeneratorMessenger* gunMessenger;
    G4bool fuseCRY;
};

#endif
