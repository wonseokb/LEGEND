#include <TString.h>
#include <G4PVPlacement.hh>
#include <G4SubtractionSolid.hh>
#include "Project1DetectorConstruction.hh"//User header file1
#include "Project1PmtSd.hh"//User header file2
#include "Project1Material1Sd.hh"//User header file3
#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4LogicalBorderSurface.hh"//Surface header file1
#include "G4LogicalSkinSurface.hh"//Surface header file2
#include "G4SolidStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4GeometryManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Torus.hh"
#include "G4OpticalSurface.hh"//Surface header file3

//======================================================================================================================================================== 0. header files

Project1DetectorConstruction::Project1DetectorConstruction()// Constructor (Fully Understand)
{
  nistManager = G4NistManager::Instance();//G4NistManager(class)'s member function -> nistManager
  defineMaterials();//use member function to access member variables

  isUpdated = false;//for safety set false
  setDefaults();//set some values for simulation
}

Project1DetectorConstruction::~Project1DetectorConstruction()// Destructor (Fully Understand)
{}

void Project1DetectorConstruction::setDefaults()//Fully Understand
{ 
  experimentalHallX = (200./2.)*cm;
  experimentalHallY = (200./2.)*cm;
  experimentalHallZ = (200./2.)*cm;
  
  TPBthickness = (3/2.)*um; //0.1
  TPBwidth = (4.55635/2.)*cm; //(1./2.)*cm;   //  ((4.56+0.24)/2.)*cm
  TPBlength = (100./2.)*cm; //(10./2.)*cm;
  
  EJ280Thickness = (3./2.)*mm;
  EJ280Width = (4.55635/2)*cm; //(1./2.)*cm;
  EJ280Length = (100./2.)*cm; //(10./2.)*cm;
  
  Material1_LightYield = 0;
  pmtThickness = (1./2.) * um;
  photodetectorType = "hitCounter";
   
  //simulationMode = "flat_plate_onlyTPB"; 
  simulationMode = "flat_plate_onlyEJ280";
  //simulationMode = "flat_plate";
  isUpdated = true;
}

G4VPhysicalVolume* Project1DetectorConstruction::makeDetectorPhysicalVolume()//Fully Understand
{
  if (simulationMode == "flat_plate_onlyTPB")
    return makeMode1();
  else if (simulationMode == "flat_plate_onlyEJ280")
    return makeMode2();
  else if (simulationMode == "flat_plate")
    return makeMode3();
  else
    throw "The selected simulation mode does not exist.";
}

void Project1DetectorConstruction::updateDetector()//Update Detector (Just Use)
{
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();
  G4LogicalSkinSurface::CleanSurfaceTable();
  G4LogicalBorderSurface::CleanSurfaceTable();
  G4SurfaceProperty::CleanSurfacePropertyTable();
  setMaterialProperties();
  G4RunManager::GetRunManager()->DefineWorldVolume(makeDetectorPhysicalVolume());
  G4RunManager::GetRunManager()->GeometryHasBeenModified();

  isUpdated = false;
}

void Project1DetectorConstruction::readCsvFile(G4String filename, std::vector<G4double>& energies, std::vector<G4double>& values, double valueUnit)//Fully Understand
{
  G4String wavelengthString;
  G4String valueString;
  std::ifstream fCsv(getFilePath(filename));
  G4cout<<"filename : "<<filename<<"###################################################################################################"<<G4endl;
  if(fCsv.is_open()){
    while (std::getline(fCsv, wavelengthString, ',')) {
      std::getline(fCsv, valueString);
      energies.push_back(convertWavelengthToEnergy(std::stof(wavelengthString)));
      values.push_back(std::stof(valueString) * valueUnit);
      G4cout<<"wavelength(nm) : "<<wavelengthString<<" valueString : "<<valueString<<G4endl;
    }
  }
}

//======================================================================================================================================================== 1. Constructor & Destructor
//========================================================================================================================================================

void Project1DetectorConstruction::setMaterial1Property()//EJ280
{
  Material1_Mpt = new G4MaterialPropertiesTable();

  std::vector<G4double> emissionEnergies;//No use
  std::vector<G4double> fastEmissions;//No use
  readCsvFile(MATERIAL1_EMISSION_FILENAME, emissionEnergies, fastEmissions, 1.0);//No use
  std::vector<G4double> slowEmissions(emissionEnergies.size(), 0.0);//No use 
  
  std::vector<G4double> absorptionEnergies;//No use
  std::vector<G4double> absorptions;//No use
  readCsvFile(MATERIAL1_ABSORPTION_LENGTH_FILENAME, absorptionEnergies, absorptions, 1.0 * m);//No use

  std::vector<G4double> wlsAbsorptionEnergies;
  std::vector<G4double> wlsAbsorptions;
  readCsvFile(MATERIAL1_WLS_ABSORPTION_LENGTH_FILENAME, wlsAbsorptionEnergies, wlsAbsorptions, 1.0 * m);

  std::vector<G4double> wlsEmissionEnergies;
  std::vector<G4double> wlsEmissions;
  readCsvFile(MATERIAL1_WLS_EMISSION_FILENAME, wlsEmissionEnergies, wlsEmissions, 1.0);//***

  G4double refractionIndex = 1.58;//constant ***
  std::vector<G4double> refractionIndices(emissionEnergies.size(), refractionIndex);//***

  Material1_Mpt->AddProperty("RINDEX", &emissionEnergies[0], &refractionIndices[0], (G4int) emissionEnergies.size());//1
  Material1_Mpt->AddConstProperty("WLSTIMECONSTANT", 8.5 * ns);//2 
  Material1_Mpt->AddProperty("WLSABSLENGTH", &wlsAbsorptionEnergies[0], &wlsAbsorptions[0], (G4int) wlsAbsorptionEnergies.size());//3
  //Material1_Mpt->AddProperty("ABSLENGTH", &absorptionEnergies[0], &absorptions[0], (G4int) absorptionEnergies.size());//4 ***
  Material1_Mpt->AddProperty("WLSCOMPONENT", &wlsEmissionEnergies[0], &wlsEmissions[0], (G4int) wlsEmissionEnergies.size());//5
  //Material1_Mpt->AddProperty("FASTCOMPONENT", &emissionEnergies[0], &fastEmissions[0], (G4int) emissionEnergies.size());
  //Material1_Mpt->AddProperty("SLOWCOMPONENT", &emissionEnergies[0], &slowEmissions[0], (G4int) emissionEnergies.size()); 
 
  //Scintillator
  /*
  Material1_Mpt->AddConstProperty("SCINTILLATIONYIELD", Material1_LightYield);
  Material1_Mpt->AddConstProperty("CONSTANTQUANTUMYIELD", 0.9);
  Material1_Mpt->AddConstProperty("RESOLUTIONSCALE", 1.0);
  Material1_Mpt->AddConstProperty("FASTTIMECONSTANT", 2.1 * ns);//catch!
  Material1_Mpt->AddConstProperty("SLOWTIMECONSTANT", 10.0 * ns);//catch!
  Material1_Mpt->AddConstProperty("YIELDRATIO", 1.0);//catch!
  // set Birks constant to the value measured by Dubna ***
  // https://nova-docdb.fnal.gov/cgi-bin/private/ShowDocumentdocid=34223 ***
  Material1->GetIonisation()->SetBirksConstant(0.134 * mm / MeV);//catch! ***
  */
  Material1->SetMaterialPropertiesTable(Material1_Mpt);//check
}

void Project1DetectorConstruction::setMaterial2Property()//TPB
{
  std::vector<G4double> absorptionEnergies;
  std::vector<G4double> absorptions;
  readCsvFile(MATERIAL2_ABSORPTION_LENGTH_FILENAME, absorptionEnergies, absorptions, m);//check

  std::vector<G4double> wlsAbsorptionEnergies;
  std::vector<G4double> wlsAbsorptions;
  readCsvFile(MATERIAL2_WLS_ABSORPTION_LENGTH_FILENAME, wlsAbsorptionEnergies, wlsAbsorptions, m);//check

  std::vector<G4double> wlsEmissionEnergies;
  std::vector<G4double> wlsEmissions;
  readCsvFile(MATERIAL2_WLS_EMISSION_FILENAME, wlsEmissionEnergies, wlsEmissions, 1.0);//***

  std::vector<G4double> refractionIndexEnergies;
  std::vector<G4double> refractionIndices;//***
  readCsvFile(MATERIAL2_REFRACTION_INDEX_FILENAME, refractionIndexEnergies, refractionIndices, 1.0);//check
  
  Material2_Mpt = new G4MaterialPropertiesTable();  
 
  Material2_Mpt->AddProperty("RINDEX", &refractionIndexEnergies[0], &refractionIndices[0], (G4int) refractionIndexEnergies.size());//1
  Material2_Mpt->AddConstProperty("WLSTIMECONSTANT", 0.01 * ns);//2
  Material2_Mpt->AddProperty("WLSABSLENGTH", &wlsAbsorptionEnergies[0],  &wlsAbsorptions[0],  (G4int) wlsAbsorptionEnergies.size());//3
  //Material2_Mpt->AddProperty("ABSLENGTH", &absorptionEnergies[0], &absorptions[0], (G4int) absorptionEnergies.size());//4 ***
  Material2_Mpt->AddProperty("WLSCOMPONENT", &wlsEmissionEnergies[0], &wlsEmissions[0], (G4int) wlsEmissionEnergies.size());//5 
  //Material2_Mpt->AddConstProperty("YIELDRATIO", 1.2);//doesn't work
  //Material2_Mpt->AddConstProperty("SCINTILLATIONYIELD", Material2_LightYield);
  //Material2_LightYield = 1.2;
  Material2->SetMaterialPropertiesTable(Material2_Mpt);
  
}

void Project1DetectorConstruction::setMaterial3Property()//LAr
{ 
  G4cout<<"setMaterial3Property#######################################################################################################"<<G4endl;
  std::vector<G4double> refractionIndexEnergies;
  std::vector<G4double> refractionIndices;//***
  readCsvFile(MATERIAL3_REFRACTION_INDEX_FILENAME, refractionIndexEnergies, refractionIndices, 1.0);//check

  Material3_Mpt = new G4MaterialPropertiesTable();
  Material3_Mpt->AddProperty("RINDEX", &refractionIndexEnergies[0], &refractionIndices[0], (G4int) refractionIndexEnergies.size());//1
  Material3->SetMaterialPropertiesTable(Material3_Mpt);
}


void Project1DetectorConstruction::setGalacticProperty()//Fully Understand
{
  const G4int energyCount = 2;
  G4double energies[energyCount] = {convertWavelengthToEnergy(200.0), convertWavelengthToEnergy(700.0)};
  G4double refractionIndices[energyCount] = {1.0, 1.0};
  G4MaterialPropertiesTable* galacticMpt = new G4MaterialPropertiesTable();
  galacticMpt->AddProperty("RINDEX", energies, refractionIndices, energyCount);
  galactic->SetMaterialPropertiesTable(galacticMpt);
}

void Project1DetectorConstruction::defineMaterials()//Fully Understand
{
  H = nistManager->FindOrBuildElement("H");
  C = nistManager->FindOrBuildElement("C");
  galactic = nistManager->FindOrBuildMaterial("G4_Galactic");
  
  Material1 = new G4Material("Material1", 0.862*g/cm3, 2, kStateLiquid, 80*kelvin, 23.8161*atmosphere);
  Material1->AddElement(H, 10);
  Material1->AddElement(C, 9);
  //CH2CH(C6H4CH3)

  Material2 = new G4Material("Material2", 1.079*g/cm3, 2, kStateSolid, 80*kelvin, 23.8161*atmosphere);
  Material2->AddElement(H, 22);
  Material2->AddElement(C, 28);
  //C28H22

 
  Material3 = new G4Material("Material3", 1.3954*g/cm3, 1, kStateLiquid,80*kelvin, 23.8161*atmosphere);//DUNE 77K–89K
  Material3->AddElement(C, 1); /////Need to change 
  //Ar 
}

void Project1DetectorConstruction::setMaterialProperties()//Make Material Property (Fully Understand)
{
  setMaterial1Property();
  setMaterial2Property();
  setMaterial3Property();
  setGalacticProperty();
}

G4VPhysicalVolume* Project1DetectorConstruction::Construct()//Make Detector Phys Volume (Fully Understand)
{
  setMaterialProperties();
  return makeDetectorPhysicalVolume();
}

//================================================================================================================================================= 2. MaterialProperties
//=================================================================================================================================================

G4VPhysicalVolume* Project1DetectorConstruction::makeMode1()//Fully Understand //For Only TPB TEST
{
  
  G4RotationMatrix* RotationMatrix = new G4RotationMatrix();
  RotationMatrix->rotateX(CLHEP::pi / 2.0 * rad);
  
  // 1.Experimental Hall
  experimentalHallSolid = new G4Box("experimentalHallSolid", experimentalHallX, experimentalHallY, experimentalHallZ);
  experimentalHallLogicalVolume = new G4LogicalVolume(experimentalHallSolid, Material3, "experimentalHallLogicalVolume", 0, 0, 0);
  experimentalHallPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(), experimentalHallLogicalVolume, "experimentalHallPhysicalVolume", 0, false, 0);
  
  // 2.TPB
  G4Box* TPBPlate = new G4Box("TPB", TPBwidth, TPBthickness ,TPBlength);
  G4LogicalVolume* TPBPlateLogicalVolume = new G4LogicalVolume(TPBPlate, Material2, "TPBPlateLogicalVolume", 0, 0, 0);

  new G4PVPlacement(0,
                    G4ThreeVector(0.0, TPBthickness+pmtThickness, 0.0),
                    TPBPlateLogicalVolume,
                    "TPBPlate",
                    experimentalHallLogicalVolume,
                    false, 0);
  
  G4VisAttributes* plateVisAttributes = new G4VisAttributes();
  plateVisAttributes->SetVisibility(true);
  plateVisAttributes->SetForceSolid(true);

  //Check
  G4SDManager* sdManager = G4SDManager::GetSDMpointer();
 
  // 3.Pmt
  G4LogicalVolume* pmtLogicalVolume1 = makePmtLogicalVolume_plate1();   
  new G4PVPlacement(0,
                    G4ThreeVector(0.0, 0.0, 0.0),
                    pmtLogicalVolume1,
                    "pmt1",
                    experimentalHallLogicalVolume,
                    false,
                    0);
  
  Project1PmtSd* pmtSd = (Project1PmtSd*) sdManager->FindSensitiveDetector(PMT_SENSITIVE_DETECTOR_NAME, false);//***
  if (!pmtSd) {
    pmtSd = new Project1PmtSd(PMT_SENSITIVE_DETECTOR_NAME);
    pmtSd->initPmts(1);
    pmtSd->setPmtPosition(0, 0.0, 0.0, 0.0);
    sdManager->AddNewDetector(pmtSd);
  }
  pmtLogicalVolume1->SetSensitiveDetector(pmtSd);
  
  return experimentalHallPhysicalVolume;  
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------

G4VPhysicalVolume* Project1DetectorConstruction::makeMode2()//Fully Understand //For Only EJ280
{

  G4RotationMatrix* RotationMatrix = new G4RotationMatrix();
  RotationMatrix->rotateX(CLHEP::pi / 2.0 * rad);
  G4RotationMatrix* RotationMatrix2 = new G4RotationMatrix();
  RotationMatrix2->rotateX(CLHEP::pi / -2.0 * rad);
  
  // 1.Experimental Hall
  experimentalHallSolid = new G4Box("experimentalHallSolid", experimentalHallX, experimentalHallY, experimentalHallZ);
  experimentalHallLogicalVolume = new G4LogicalVolume(experimentalHallSolid, Material3, "experimentalHallLogicalVolume", 0, 0, 0);
  experimentalHallPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(), experimentalHallLogicalVolume, "experimentalHallPhysicalVolume", 0, false, 0);
    
  // 3.EJ280
  G4Box* EJ280Plate = new G4Box("EJ280Plate", EJ280Width, EJ280Thickness, EJ280Length);
  G4LogicalVolume* EJ280PlateLogicalVolume = new G4LogicalVolume(EJ280Plate, Material1, "EJ280PlateLogicalVolume", 0, 0, 0);

  new G4PVPlacement(0,
                    G4ThreeVector(0.0, EJ280Thickness+pmtThickness, 0.0),
                    EJ280PlateLogicalVolume,
                    "EJ280Plate",
                    experimentalHallLogicalVolume,
                    false, 0);
  
  G4SDManager* sdManager = G4SDManager::GetSDMpointer();

  G4VSensitiveDetector* Material1_Sd = (Project1Material1Sd*) sdManager->FindSensitiveDetector(MATERIAL1_SENSITIVE_DETECTOR_NAME, false);//***
  if (!Material1_Sd) {
    Material1_Sd = new Project1Material1Sd(MATERIAL1_SENSITIVE_DETECTOR_NAME);
    sdManager->AddNewDetector(Material1_Sd);
  }

  EJ280PlateLogicalVolume->SetSensitiveDetector(Material1_Sd);

  // 3.Pmt
  G4LogicalVolume* pmtLogicalVolume1 = makePmtLogicalVolume_plate1();   
  new G4PVPlacement(0,
                    G4ThreeVector(0.0, 0.0, 0.0),
                    pmtLogicalVolume1,
                    "pmt1",
                    experimentalHallLogicalVolume,
                    false,
                    0);
  
  Project1PmtSd* pmtSd = (Project1PmtSd*) sdManager->FindSensitiveDetector(PMT_SENSITIVE_DETECTOR_NAME, false);//***
  if (!pmtSd) {
    pmtSd = new Project1PmtSd(PMT_SENSITIVE_DETECTOR_NAME);
    pmtSd->initPmts(1);
    pmtSd->setPmtPosition(0, 0.0, 0.0, 0.0);
    sdManager->AddNewDetector(pmtSd);
  }
  pmtLogicalVolume1->SetSensitiveDetector(pmtSd);
  
  return experimentalHallPhysicalVolume;  

}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------

G4VPhysicalVolume* Project1DetectorConstruction::makeMode3()//Fully Understand //For TPB+EJ280
{
  	
  G4RotationMatrix* RotationMatrix = new G4RotationMatrix();
  RotationMatrix->rotateX(CLHEP::pi / 2.0 * rad);
  G4RotationMatrix* RotationMatrix2 = new G4RotationMatrix();
  RotationMatrix2->rotateX(CLHEP::pi / -2.0 * rad);
  
  // 1.Experimental Hall
  experimentalHallSolid = new G4Box("experimentalHallSolid", experimentalHallX, experimentalHallY, experimentalHallZ);
  experimentalHallLogicalVolume = new G4LogicalVolume(experimentalHallSolid, Material3, "experimentalHallLogicalVolume", 0, 0, 0);
  experimentalHallPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(), experimentalHallLogicalVolume, "experimentalHallPhysicalVolume", 0, false, 0);
  
  // 2.TPB
  G4Box* TPBPlate = new G4Box("TPB", TPBwidth, TPBthickness, TPBlength);
  G4LogicalVolume* TPBPlateLogicalVolume = new G4LogicalVolume(TPBPlate, Material2, "TPBPlateLogicalVolume", 0, 0, 0);
  G4Box* TPBPlate2 = new G4Box("TPB", TPBthickness, EJ280Thickness, TPBlength);
  G4LogicalVolume* TPBPlateLogicalVolume2 = new G4LogicalVolume(TPBPlate2, Material2, "TPBPlateLogicalVolume2", 0, 0, 0);

  //Top
  new G4PVPlacement(0,
                    G4ThreeVector(0.0, TPBthickness+2*EJ280Thickness+pmtThickness, 0.0),
                    TPBPlateLogicalVolume,
                    "TPBPlate",
                    experimentalHallLogicalVolume,
                    false, 0);
  //Bottom
  new G4PVPlacement(0,
                    G4ThreeVector(0.0, -TPBthickness+pmtThickness, 0.0),
                    TPBPlateLogicalVolume,
                    "TPBPlate",
                    experimentalHallLogicalVolume,
                    false, 0);
  //Left
  new G4PVPlacement(0,
                    G4ThreeVector(TPBwidth+TPBthickness, EJ280Thickness+pmtThickness , 0.0),
                    TPBPlateLogicalVolume2,
                    "TPBPlate2",
                    experimentalHallLogicalVolume,
                    false, 0);
  //Right
  new G4PVPlacement(0,
                    G4ThreeVector(-TPBwidth-TPBthickness, EJ280Thickness+pmtThickness , 0.0),
                    TPBPlateLogicalVolume2,
                    "TPBPlate2",
                    experimentalHallLogicalVolume,
                    false, 0);
  
  G4VisAttributes* plateVisAttributes = new G4VisAttributes();
  plateVisAttributes->SetVisibility(true);
  plateVisAttributes->SetForceSolid(true);
  
  // 3.EJ280
  G4Box* EJ280Plate = new G4Box("EJ280Plate", EJ280Width, EJ280Thickness, EJ280Length);
  G4LogicalVolume* EJ280PlateLogicalVolume = new G4LogicalVolume(EJ280Plate, Material1, "EJ280PlateLogicalVolume", 0, 0, 0);

  new G4PVPlacement(0,
                    G4ThreeVector(0.0, EJ280Thickness+pmtThickness, 0.0),
                    EJ280PlateLogicalVolume,
                    "EJ280Plate",
                    experimentalHallLogicalVolume,
                    false, 0);
  
  G4SDManager* sdManager = G4SDManager::GetSDMpointer();

  G4VSensitiveDetector* Material1_Sd = (Project1Material1Sd*) sdManager->FindSensitiveDetector(MATERIAL1_SENSITIVE_DETECTOR_NAME, false);//***
  if (!Material1_Sd) {
    Material1_Sd = new Project1Material1Sd(MATERIAL1_SENSITIVE_DETECTOR_NAME);
    sdManager->AddNewDetector(Material1_Sd);
  }

  EJ280PlateLogicalVolume->SetSensitiveDetector(Material1_Sd);

  // 4.Pmt  
  G4LogicalVolume* pmtLogicalVolume2 = makePmtLogicalVolume_plate2();
  new G4PVPlacement(RotationMatrix,
                    G4ThreeVector(0.0, pmtThickness+EJ280Thickness, TPBlength+pmtThickness),
                    pmtLogicalVolume2,
                    "pmt2",
                    experimentalHallLogicalVolume,
                    false,
                    0);

  new G4PVPlacement(RotationMatrix2,
                    G4ThreeVector(0.0, pmtThickness+EJ280Thickness, -TPBlength-pmtThickness),
                    pmtLogicalVolume2,
                    "pmt3",
                    experimentalHallLogicalVolume,
                    false,
                    0);

  Project1PmtSd* pmtSd = (Project1PmtSd*) sdManager->FindSensitiveDetector(PMT_SENSITIVE_DETECTOR_NAME, false);//***
  if (!pmtSd) {
    pmtSd = new Project1PmtSd(PMT_SENSITIVE_DETECTOR_NAME);
    pmtSd->initPmts(1);
    pmtSd->setPmtPosition(0, 0.0, 0.0, 0.0);
    sdManager->AddNewDetector(pmtSd);
  }

  pmtLogicalVolume2->SetSensitiveDetector(pmtSd);
  return experimentalHallPhysicalVolume;  
}

//=======================================================================================================================================================================

G4LogicalVolume* Project1DetectorConstruction::makePmtLogicalVolume_plate1()//*** check!!!
{
  //4.1 Geometery (Fully Understand) 
  G4Box* photocathodeSolid = new G4Box("pmtSolid", EJ280Width, pmtThickness, EJ280Length);
  G4LogicalVolume* photocathodeLogicalVolume = new G4LogicalVolume(photocathodeSolid, galactic, "photocathodeLogicalVolume");
  
  //---------------------------------------------------------------------------------------------------------------------------------------------------

  //4.2 QE (Fully Understand)
  G4String quantumEfficiencyFilename;
  if (photodetectorType == "hitCounter") {
    quantumEfficiencyFilename = QUANTUM_EFFICIENCY_FILENAME;
  }
  else if (photodetectorType == "pmt") {
    quantumEfficiencyFilename = PMT_QUANTUM_EFFICIENCY_FILENAME;
  }
  else
    throw "The selected photodetector type does not exist.";

  std::vector<G4double> energies;
  std::vector<G4double> quantumEfficiencies;
  readCsvFile(quantumEfficiencyFilename, energies, quantumEfficiencies, 1.0);
  std::vector<G4double> refractionIndices(energies.size(), 1.0);
  std::vector<G4double> reflectivities(energies.size(), 0.0);

  //---------------------------------------------------------------------------------------------------------------------------------------------------

  //4.3 Surface
  G4OpticalSurface* opticalSurface = new G4OpticalSurface("opticalSurface");// **************
  G4MaterialPropertiesTable* opticalSurfaceMpt = new G4MaterialPropertiesTable();
  opticalSurfaceMpt->AddProperty("EFFICIENCY", &energies[0], &quantumEfficiencies[0], (G4int) energies.size());//QE
  opticalSurfaceMpt->AddProperty("REFLECTIVITY", &energies[0], &reflectivities[0], (G4int) energies.size());//Reflection
  opticalSurfaceMpt->AddProperty("RINDEX", &energies[0], &refractionIndices[0], (G4int) energies.size());//Refraction
  opticalSurface->SetModel(glisur);// ************
  opticalSurface->SetFinish(polished);// ************
  opticalSurface->SetType(dielectric_metal);// ***************
  opticalSurface->SetMaterialPropertiesTable(opticalSurfaceMpt);
  new G4LogicalSkinSurface("pmtLogicalSkinSurface", photocathodeLogicalVolume, opticalSurface);

  return photocathodeLogicalVolume;
}

G4LogicalVolume* Project1DetectorConstruction::makePmtLogicalVolume_plate2()//*** check!!!
{
  //4.1 Geometery (Fully Understand) 
  G4Box* photocathodeSolid = new G4Box("pmtSolid", EJ280Width, EJ280Thickness, pmtThickness);
  G4LogicalVolume* photocathodeLogicalVolume = new G4LogicalVolume(photocathodeSolid, galactic, "photocathodeLogicalVolume");

  //---------------------------------------------------------------------------------------------------------------------------------------------------

  //4.2 QE (Fully Understand)
  G4String quantumEfficiencyFilename;
  if (photodetectorType == "hitCounter") {
    quantumEfficiencyFilename = QUANTUM_EFFICIENCY_FILENAME;
  }
  else if (photodetectorType == "pmt") {
    quantumEfficiencyFilename = PMT_QUANTUM_EFFICIENCY_FILENAME;
  }
  else
    throw "The selected photodetector type does not exist.";

  std::vector<G4double> energies;
  std::vector<G4double> quantumEfficiencies;
  readCsvFile(quantumEfficiencyFilename, energies, quantumEfficiencies, 1.0);
  std::vector<G4double> refractionIndices(energies.size(), 1.0);
  std::vector<G4double> reflectivities(energies.size(), 0.0);

  //---------------------------------------------------------------------------------------------------------------------------------------------------

  //4.3 Surface
  G4OpticalSurface* opticalSurface = new G4OpticalSurface("opticalSurface");// **************
  G4MaterialPropertiesTable* opticalSurfaceMpt = new G4MaterialPropertiesTable();
  opticalSurfaceMpt->AddProperty("EFFICIENCY", &energies[0], &quantumEfficiencies[0], (G4int) energies.size());//QE
  opticalSurfaceMpt->AddProperty("REFLECTIVITY", &energies[0], &reflectivities[0], (G4int) energies.size());//Reflection
  opticalSurfaceMpt->AddProperty("RINDEX", &energies[0], &refractionIndices[0], (G4int) energies.size());//Refraction
  opticalSurface->SetModel(glisur);// ************
  opticalSurface->SetFinish(polished);// ************
  opticalSurface->SetType(dielectric_metal);// ***************
  opticalSurface->SetMaterialPropertiesTable(opticalSurfaceMpt);
  new G4LogicalSkinSurface("pmtLogicalSkinSurface", photocathodeLogicalVolume, opticalSurface);

  return photocathodeLogicalVolume;
}

