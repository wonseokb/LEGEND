#ifndef Project2PrimaryGeneratorMessenger_h
#define Project2PrimaryGeneratorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class Project2PrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithoutParameter;
class G4UIcmdWithABool;


class Project2PrimaryGeneratorMessenger: public G4UImessenger
{
  public:
    Project2PrimaryGeneratorMessenger(Project2PrimaryGeneratorAction*);
    ~Project2PrimaryGeneratorMessenger();
    void SetNewValue(G4UIcommand*, G4String);

  private:
    Project2PrimaryGeneratorAction* primaryGeneratorAction;
    /*
    G4UIdirectory* cryDir;
    G4UIcmdWithAString* fileCmd;
    G4UIcmdWithAString* inputCmd;
    G4UIcmdWithoutParameter* updateCmd;
    G4UIcmdWithABool* useCRYCmd;
    std::string* messengerInput;
    */
};

#endif
