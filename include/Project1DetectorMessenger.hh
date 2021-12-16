#ifndef Project1DetectorMessenger_h
#define Project1DetectorMessenger_h 1

#include <G4UIcmdWithAString.hh>
#include "G4UImessenger.hh"
#include "globals.hh"

class Project1DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithAnInteger;
class G4UIcommand;
class G4UIcmdWithABool;
class G4UIcmdWithADouble;
class G4UIcmdWithoutParameter;

class Project1DetectorMessenger: public G4UImessenger
{
  public:
    Project1DetectorMessenger(Project1DetectorConstruction*);
    virtual ~Project1DetectorMessenger();
    virtual void SetNewValue(G4UIcommand*, G4String);

  private:
     
    Project1DetectorConstruction* detectorConstruction;
    /*
    G4UIdirectory* detectorDir;
    G4UIcmdWithAString* photodetectorType;
    G4UIcommand* update;
    G4UIcommand* defaultValue;
    G4UIcmdWithADouble* Material1_LightYield;
    G4UIcmdWithoutParameter* printSetting;
    G4UIcmdWithAString* simulationMode;
    */
};

#endif
