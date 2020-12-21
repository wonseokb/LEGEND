#include "NovaDetectorMessenger.hh"
#include "NovaDetectorConstruction.hh"

#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithoutParameter.hh"


NovaDetectorMessenger::NovaDetectorMessenger(NovaDetectorConstruction* detector)
    : detectorConstruction(detector)
{
  detectorDir = new G4UIdirectory("/nova/detector/");
  detectorDir->SetGuidance("Detector geometry control");

  straightHeight = new G4UIcmdWithADoubleAndUnit("/nova/detector/straightHeight", this);
  straightHeight->SetGuidance("Set the rectangle height.");
  straightHeight->SetParameterName("straightHeight", false);
  straightHeight->SetDefaultUnit("mm");

  straightWidth = new G4UIcmdWithADoubleAndUnit("/nova/detector/straightWidth", this);
  straightWidth->SetGuidance("Set the rectangle width.");
  straightWidth->SetParameterName("straightWidth", false);
  straightWidth->SetDefaultUnit("mm");

  innerCellCornerRadius = new G4UIcmdWithADoubleAndUnit("/nova/detector/innerCellCornerRadius", this);
  innerCellCornerRadius->SetGuidance("Set the inner cell corner radius.");
  innerCellCornerRadius->SetParameterName("innerCellCornerRadius", false);
  innerCellCornerRadius->SetDefaultUnit("mm");

  pvcThickness = new G4UIcmdWithADoubleAndUnit("/nova/detector/pvcThickness", this);
  pvcThickness->SetGuidance("Set the thickness of the PVC.");
  pvcThickness->SetParameterName("pvcThickness", false);
  pvcThickness->SetDefaultUnit("mm");

  fiberRadius = new G4UIcmdWithADoubleAndUnit("/nova/detector/fiberRadius", this);
  fiberRadius->SetGuidance("Set the fiber radius.");
  fiberRadius->SetParameterName("fiberRadius", false);
  fiberRadius->SetDefaultUnit("mm");

  fiberCurveRadius = new G4UIcmdWithADoubleAndUnit("/nova/detector/fiberCurveRadius", this);
  fiberCurveRadius->SetGuidance("Set the curve radius for the fiber loop.");
  fiberCurveRadius->SetParameterName("fiberCurveRadius", false);
  fiberCurveRadius->SetDefaultUnit("mm");

  detectorLength = new G4UIcmdWithADoubleAndUnit("/nova/detector/detectorLength", this);
  detectorLength->SetGuidance("Set detector length.");
  detectorLength->SetParameterName("detectorLength", false);
  detectorLength->SetDefaultUnit("cm");

  snoutLength = new G4UIcmdWithADoubleAndUnit("/nova/detector/snoutLength", this);
  snoutLength->SetGuidance("Set snout length. This is the length of the part of the fiber that is outside the liquid scintillator.");
  snoutLength->SetParameterName("snoutLength", false);
  snoutLength->SetDefaultUnit("mm");

  photodetectorType = new G4UIcmdWithAString("/nova/detector/photodetectorType", this);
  photodetectorType->SetGuidance("Photodetector type. Two options are available: pmt and apd.");

  update = new G4UIcommand("/nova/detector/update", this);
  update->SetGuidance("Update the detector geometry with new values.");
  update->SetGuidance("Must be run before beamOn if detector has been updated.");

  defaultValue = new G4UIcommand("/nova/detector/defaultValue", this);
  defaultValue->SetGuidance("Set all detector geometry values to defaults.");

  liquidScintillatorLightYield=new G4UIcmdWithADouble("/nova/detector/liquidScintillatorLightYield", this);
  liquidScintillatorLightYield->SetGuidance("Set the liquid scintillator light yield in number of photons per MeV.");

  printSetting = new G4UIcmdWithoutParameter("/nova/printSetting", this);
  printSetting->SetGuidance("Print the detector setting.");

  simulationMode = new G4UIcmdWithAString("/nova/simulationMode", this);
  simulationMode->SetParameterName("simulationMode", false);
  simulationMode->SetGuidance("Simulation mode. Use \"fiber\" to simulate a single fiber. Use \"cell\" to simulate a NOvA cell. Use \"benchtop\" to simulate a benchtop NOvA cell measurement.");
}

NovaDetectorMessenger::~NovaDetectorMessenger()
{
  delete straightWidth;
  delete straightHeight;
  delete detectorLength;
  delete snoutLength;
  delete pvcThickness;
  delete innerCellCornerRadius;
  delete fiberRadius;
  delete photodetectorType;
  delete update;
  delete defaultValue;
  delete liquidScintillatorLightYield;
  delete printSetting;
  delete simulationMode;
}

void NovaDetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if (command == straightWidth) {
    detectorConstruction->setStraightWidth(straightWidth->GetNewDoubleValue(newValue));
  }
  else if (command == straightHeight) {
    detectorConstruction->setStraightHeight(straightHeight->GetNewDoubleValue(newValue));
  }
  else if (command == detectorLength) {
    detectorConstruction->setDetectorLength(detectorLength->GetNewDoubleValue(newValue));
  }
  else if (command == snoutLength) {
    detectorConstruction->setSnoutLength(snoutLength->GetNewDoubleValue(newValue));
  }
  else if (command == pvcThickness) {
    detectorConstruction->setPvcThickness(pvcThickness->GetNewDoubleValue(newValue));
  }
  else if (command == innerCellCornerRadius) {
    detectorConstruction->setInnerCellCornerRadius(innerCellCornerRadius->GetNewDoubleValue(newValue));
  }
  else if (command == fiberRadius) {
    detectorConstruction->setFiberRadius(fiberRadius->GetNewDoubleValue(newValue));
  }
  else if (command == fiberCurveRadius) {
    detectorConstruction->setFiberCurveRadius(fiberCurveRadius->GetNewDoubleValue(newValue));
  }
  else if (command == photodetectorType) {
    detectorConstruction->setPhotodetectorType(newValue);
  }
  else if (command == update) {
    detectorConstruction->updateDetector();
  }
  else if (command == defaultValue) {
    detectorConstruction->setDefaults();
  }
  else if (command == liquidScintillatorLightYield) {
    detectorConstruction->setLiquidScintillatorLightYield(liquidScintillatorLightYield->GetNewDoubleValue(newValue));
  }
  else if (command == printSetting) {
    detectorConstruction->printSetting();
  }
  else if (command == simulationMode) {
    detectorConstruction->setSimulationMode(newValue);
  }
}
