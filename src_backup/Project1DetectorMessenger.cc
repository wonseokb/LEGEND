#include "Project1DetectorMessenger.hh"
#include "Project1DetectorConstruction.hh"

#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithoutParameter.hh"


Project1DetectorMessenger::Project1DetectorMessenger(Project1DetectorConstruction* detector):detectorConstruction(detector)
{
  /*	
  detectorDir = new G4UIdirectory("/nova/detector/");
  detectorDir->SetGuidance("Detector geometry control");

  photodetectorType = new G4UIcmdWithAString("/nova/detector/photodetectorType", this);
  photodetectorType->SetGuidance("Photodetector type. Two options are available: pmt and apd.");

  update = new G4UIcommand("/nova/detector/update", this);
  update->SetGuidance("Update the detector geometry with new values.");
  update->SetGuidance("Must be run before beamOn if detector has been updated.");

  defaultValue = new G4UIcommand("/nova/detector/defaultValue", this);
  defaultValue->SetGuidance("Set all detector geometry values to defaults.");

  Material1_LightYield=new G4UIcmdWithADouble("/nova/detector/Material1_Yield", this);
  Material1_LightYield->SetGuidance("Set the liquid scintillator light yield in number of photons per MeV.");

  printSetting = new G4UIcmdWithoutParameter("/nova/printSetting", this);
  printSetting->SetGuidance("Print the detector setting.");

  simulationMode = new G4UIcmdWithAString("/nova/simulationMode", this);
  simulationMode->SetParameterName("simulationMode", false);
  simulationMode->SetGuidance("Simulation mode. Use \"fiber\" to simulate a single fiber. Use \"cell\" to simulate a NOvA cell. Use \"benchtop\" to simulate a benchtop NOvA cell measurement.");
  */
}

Project1DetectorMessenger::~Project1DetectorMessenger()
{
  /*	
  delete photodetectorType;
  delete update;
  delete defaultValue;
  delete Material1_LightYield;
  delete printSetting;
  delete simulationMode;
  */
}

void Project1DetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{ 
  /*
  if (command == photodetectorType) {
    detectorConstruction->setPhotodetectorType(newValue);
  }
  else if (command == update) {
    detectorConstruction->updateDetector();
  }
  else if (command == defaultValue) {
    detectorConstruction->setDefaults();
  }
  else if (command == Material1_LightYield) {
    detectorConstruction->setLiquidScintillatorLightYield(Material1_LightYield->GetNewDoubleValue(newValue));
  }
  else if (command == printSetting) {
    detectorConstruction->printSetting();
  }
  else if (command == simulationMode) {
    detectorConstruction->setSimulationMode(newValue);
  }
  */
}
