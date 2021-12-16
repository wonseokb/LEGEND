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
#include "G4Ellipsoid.hh"
#include "G4OpticalSurface.hh"//Surface header file3
#include "G4VisAttributes.hh"
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
  experimentalHallX = (60)*cm;
  experimentalHallY = (60)*cm;
  experimentalHallZ = (60)*cm;
  
  //For Visual
  /* 
  TPBthickness = (0.3/2.)*cm;
  TPBwidth = (2/2.)*cm;
  TPBlength = (10./2.)*cm;
  
  EJ280Thickness = (1./2.)*cm;
  EJ280Width = (2/2)*cm;
  EJ280Length = (10./2.)*cm;
  
  Material1_LightYield = 0;
  pmtThickness = (0.5/2.) * cm;
  */

  //For Experiment
  TPBthickness = (3./2.)*um;
  TPBwidth = (5.14*2/2.)*cm;
  TPBlength = (100./2.)*cm;
  
  EJ280Thickness = (3./2.)*mm;
  EJ280Width = (5.14*2/2.)*cm;
  EJ280Length = (100./2.)*cm;
  
  Material1_LightYield = 0;
  pmtThickness = (1./2.) * um;
  //photodetectorType = "hitCounter";
  photodetectorType = "pmt";

  //simulationMode = "flat_plate_onlyTPB"; 
  //simulationMode = "flat_plate_onlyEJ280";
  simulationMode = "flat_plate";
  //simulationMode = "Assembly";
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
  else if (simulationMode == "Assembly")
    return makeMode4();
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

//======================================================================================================================================================== 1.Mode decision & data file
//======================================================================================================================================================== Constructor & Destructor for the function we need

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

  G4double refractionIndex = 1.58;//constant
  std::vector<G4double> refractionIndices(emissionEnergies.size(), refractionIndex);

  //Wavelength shift material
  Material1_Mpt->AddProperty("RINDEX", &emissionEnergies[0], &refractionIndices[0], (G4int) emissionEnergies.size());//1
  Material1_Mpt->AddConstProperty("WLSTIMECONSTANT", 9.2 * ns);//2 
  Material1_Mpt->AddProperty("WLSABSLENGTH", &wlsAbsorptionEnergies[0], &wlsAbsorptions[0], (G4int) wlsAbsorptionEnergies.size());//3
  Material1_Mpt->AddProperty("ABSLENGTH", &absorptionEnergies[0], &absorptions[0], (G4int) absorptionEnergies.size());//4
  Material1_Mpt->AddProperty("WLSCOMPONENT", &wlsEmissionEnergies[0], &wlsEmissions[0], (G4int) wlsEmissionEnergies.size());//5 
  Material1_Mpt->AddConstProperty("WLSMEANNUMBERPHOTONS", 0.86);//6

  //Scintillator
  /*
  Material1_Mpt->AddConstProperty("SCINTILLATIONYIELD", Material1_LightYield);
  Material1_Mpt->AddConstProperty("CONSTANTQUANTUMYIELD", 0.9);
  Material1_Mpt->AddConstProperty("RESOLUTIONSCALE", 1.0);
  Material1_Mpt->AddProperty("FASTCOMPONENT", &emissionEnergies[0], &fastEmissions[0], (G4int) emissionEnergies.size());
  Material1_Mpt->AddProperty("SLOWCOMPONENT", &emissionEnergies[0], &slowEmissions[0], (G4int) emissionEnergies.size()); 
  Material1_Mpt->AddConstProperty("FASTTIMECONSTANT", 2.1 * ns);//catch!
  Material1_Mpt->AddConstProperty("SLOWTIMECONSTANT", 10.0 * ns);//catch!
  Material1_Mpt->AddConstProperty("YIELDRATIO", 1.0);//catch!
  // set Birks constant to the value measured by Dubna ***
  // https://nova-docdb.fnal.gov/cgi-bin/private/ShowDocumentdocid=34223 ***
  Material1->GetIonisation()->SetBirksConstant(0.134 * mm / MeV);//catch! ***
  */

  Material1->SetMaterialPropertiesTable(Material1_Mpt);//check
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

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
  readCsvFile(MATERIAL2_WLS_EMISSION_FILENAME, wlsEmissionEnergies, wlsEmissions, 1.0);

  std::vector<G4double> refractionIndexEnergies;
  std::vector<G4double> refractionIndices;
  readCsvFile(MATERIAL2_REFRACTION_INDEX_FILENAME, refractionIndexEnergies, refractionIndices, 1.0);//check
  
  Material2_Mpt = new G4MaterialPropertiesTable();  

  //Wavelength shift material 
  Material2_Mpt->AddProperty("RINDEX", &refractionIndexEnergies[0], &refractionIndices[0], (G4int) refractionIndexEnergies.size());//1
  Material2_Mpt->AddConstProperty("WLSTIMECONSTANT", 0.01 * ns);//2
  Material2_Mpt->AddProperty("WLSABSLENGTH", &wlsAbsorptionEnergies[0],  &wlsAbsorptions[0],  (G4int) wlsAbsorptionEnergies.size());//3
  Material2_Mpt->AddProperty("ABSLENGTH", &absorptionEnergies[0], &absorptions[0], (G4int) absorptionEnergies.size());//4
  Material2_Mpt->AddProperty("WLSCOMPONENT", &wlsEmissionEnergies[0], &wlsEmissions[0], (G4int) wlsEmissionEnergies.size());//5 
  Material2_Mpt->AddConstProperty("WLSMEANNUMBERPHOTONS", 1.0); //6 1.0 or 1.2

  Material2->SetMaterialPropertiesTable(Material2_Mpt);
  
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------

void Project1DetectorConstruction::setMaterial3Property()//LAr
{ 
  std::vector<G4double> refractionIndexEnergies;
  std::vector<G4double> refractionIndices;
  readCsvFile(MATERIAL3_REFRACTION_INDEX_FILENAME, refractionIndexEnergies, refractionIndices, 1.0);//check

  Material3_Mpt = new G4MaterialPropertiesTable();
  Material3_Mpt->AddProperty("RINDEX", &refractionIndexEnergies[0], &refractionIndices[0], (G4int) refractionIndexEnergies.size());//1
  Material3->SetMaterialPropertiesTable(Material3_Mpt);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

void Project1DetectorConstruction::setGalacticProperty()//Galactic = vacuum
{
  const G4int energyCount = 2;
  G4double energies[energyCount] = {convertWavelengthToEnergy(200.0), convertWavelengthToEnergy(700.0)};
  G4double refractionIndices[energyCount] = {1.0, 1.0};
  G4MaterialPropertiesTable* galacticMpt = new G4MaterialPropertiesTable();
  galacticMpt->AddProperty("RINDEX", energies, refractionIndices, energyCount);
  galactic->SetMaterialPropertiesTable(galacticMpt);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

void Project1DetectorConstruction::defineMaterials()//Give elements to Material
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
  Material3->AddElement(C, 1); /////***Need to change???????????????????????????????????????????????????????????????????????????????? 
  //Ar
     
  Material4 = new G4Material("Material4",7.9* g / cm3,8);
  Material4->AddElement(G4NistManager::Instance()->FindOrBuildElement("Fe"), 0.70105);
  Material4->AddElement(G4NistManager::Instance()->FindOrBuildElement("Cr"), 0.18);
  Material4->AddElement(G4NistManager::Instance()->FindOrBuildElement("Ni"), 0.09);
  Material4->AddElement(G4NistManager::Instance()->FindOrBuildElement("Mn"), 0.02);
  Material4->AddElement(G4NistManager::Instance()->FindOrBuildElement("C"), 0.0007);
  Material4->AddElement(G4NistManager::Instance()->FindOrBuildElement("S"), 0.0003);
  Material4->AddElement(G4NistManager::Instance()->FindOrBuildElement("Si"), 0.0075);
  Material4->AddElement(G4NistManager::Instance()->FindOrBuildElement("P"), 0.00045);
  // Steel - Firas
    
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

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

G4VPhysicalVolume* Project1DetectorConstruction::makeMode1()//For Only TPB TEST
{
  
  //rotating-------------------------------------------------------------------------------------------------------------------------------------
  G4RotationMatrix* RotationMatrix = new G4RotationMatrix();
  RotationMatrix->rotateX(CLHEP::pi / 2.0 * rad);
  
  // 1.Experimental Hall--------------------------------------------------------------------------------------------------------------------------
  experimentalHallSolid = new G4Box("experimentalHallSolid", experimentalHallX, experimentalHallY, experimentalHallZ);
  experimentalHallLogicalVolume = new G4LogicalVolume(experimentalHallSolid, Material3, "experimentalHallLogicalVolume", 0, 0, 0);
  experimentalHallPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(), experimentalHallLogicalVolume, "experimentalHallPhysicalVolume", 0, false, 0);
  
  // 2.TPB----------------------------------------------------------------------------------------------------------------------------------------
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
  G4SDManager* sdManager = G4SDManager::GetSDMpointer();

  // 3.Pmt-----------------------------------------------------------------------------------------------------------------------------------------
  G4LogicalVolume* pmtLogicalVolume1 = makePmtLogicalVolume_plate1();   
  new G4PVPlacement(0,
                    G4ThreeVector(0.0, 0.0, 0.0),
                    pmtLogicalVolume1,
                    "pmt1",
                    experimentalHallLogicalVolume,
                    false,
                    0);
  
  Project1PmtSd* pmtSd = (Project1PmtSd*) sdManager->FindSensitiveDetector(PMT_SENSITIVE_DETECTOR_NAME, false);
  if (!pmtSd) {
    pmtSd = new Project1PmtSd(PMT_SENSITIVE_DETECTOR_NAME);
    pmtSd->initPmts(1);
    pmtSd->setPmtPosition(0, 0.0, 0.0, 0.0);
    sdManager->AddNewDetector(pmtSd);
  }
  pmtLogicalVolume1->SetSensitiveDetector(pmtSd);

  //color------------------------------------------------------------------------------------------------------------------------------------------
  //For color //(1.0,1.0,1.0))-white, (0.0,0.0,1.0))-blue, (1.0,0.0,0.0))-red, (0.0,1.0,0.0))-green, (1.0,0.0,1.0))-magenta
  G4VisAttributes* VisAttributes_magenta = new G4VisAttributes(G4Colour(1.0,0.0,1.0));//magenta
  G4VisAttributes* VisAttributes_red = new G4VisAttributes(G4Colour(1.0,0.0,0.0));//red
  G4VisAttributes* VisAttributes_blue = new G4VisAttributes(G4Colour(0.0,0.0,1.0));//blue
  TPBPlateLogicalVolume->SetVisAttributes(VisAttributes_magenta);

  //sensitive detector
  pmtLogicalVolume1->SetVisAttributes(VisAttributes_blue);

  return experimentalHallPhysicalVolume;  
}

//=================================================================================================================================================

G4VPhysicalVolume* Project1DetectorConstruction::makeMode2()//Fully Understand //For Only EJ280
{

  //rotating--------------------------------------------------------------------------------------------------------------------------------------	
  G4RotationMatrix* RotationMatrix = new G4RotationMatrix();
  RotationMatrix->rotateX(CLHEP::pi / 2.0 * rad);
  G4RotationMatrix* RotationMatrix2 = new G4RotationMatrix();
  RotationMatrix2->rotateX(CLHEP::pi / -2.0 * rad);
  
  // 1.Experimental Hall---------------------------------------------------------------------------------------------------------------------------
  experimentalHallSolid = new G4Box("experimentalHallSolid", experimentalHallX, experimentalHallY, experimentalHallZ);
  experimentalHallLogicalVolume = new G4LogicalVolume(experimentalHallSolid, Material3, "experimentalHallLogicalVolume", 0, 0, 0);
  experimentalHallPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(), experimentalHallLogicalVolume, "experimentalHallPhysicalVolume", 0, false, 0);
    
  // 3.EJ280---------------------------------------------------------------------------------------------------------------------------------------
  G4Box* EJ280Plate = new G4Box("EJ280Plate", EJ280Width, EJ280Thickness, EJ280Length);
  G4LogicalVolume* EJ280PlateLogicalVolume = new G4LogicalVolume(EJ280Plate, Material1, "EJ280PlateLogicalVolume", 0, 0, 0);

  new G4PVPlacement(0,
                    G4ThreeVector(0.0, 0.0, 0.0),
                    EJ280PlateLogicalVolume,
                    "EJ280Plate",
                    experimentalHallLogicalVolume,
                    false, 0);
  
  G4SDManager* sdManager = G4SDManager::GetSDMpointer();

  G4VSensitiveDetector* Material1_Sd = (Project1Material1Sd*) sdManager->FindSensitiveDetector(MATERIAL1_SENSITIVE_DETECTOR_NAME, false);
  if (!Material1_Sd) {
    Material1_Sd = new Project1Material1Sd(MATERIAL1_SENSITIVE_DETECTOR_NAME);
    sdManager->AddNewDetector(Material1_Sd);
  }

  EJ280PlateLogicalVolume->SetSensitiveDetector(Material1_Sd);

  // 4.Pmt------------------------------------------------------------------------------------------------------------------------------------------  
  G4LogicalVolume* pmtLogicalVolume2 = makePmtLogicalVolume_plate2();
  new G4PVPlacement(0,
                    G4ThreeVector(0.0, 0.0, TPBlength+pmtThickness),
                    pmtLogicalVolume2,
                    "pmt2",
                    experimentalHallLogicalVolume,
                    false,
                    0);

  new G4PVPlacement(0,
                    G4ThreeVector(0.0, 0.0, -TPBlength-pmtThickness),
                    pmtLogicalVolume2,
                    "pmt3",
                    experimentalHallLogicalVolume,
                    false,
                    0);

  Project1PmtSd* pmtSd = (Project1PmtSd*) sdManager->FindSensitiveDetector(PMT_SENSITIVE_DETECTOR_NAME, false);
  if (!pmtSd) {
    pmtSd = new Project1PmtSd(PMT_SENSITIVE_DETECTOR_NAME);
    pmtSd->initPmts(1);
    pmtSd->setPmtPosition(0, 0.0, 0.0, 0.0);
    sdManager->AddNewDetector(pmtSd);
  }

  pmtLogicalVolume2->SetSensitiveDetector(pmtSd); 

  //color--------------------------------------------------------------------------------------------------------------------------------------------
  //For color //(1.0,1.0,1.0))-white, (0.0,0.0,1.0))-blue, (1.0,0.0,0.0))-red, (0.0,1.0,0.0))-green, (1.0,0.0,1.0))-magenta
  G4VisAttributes* VisAttributes_green = new G4VisAttributes(G4Colour(0.0,1.0,0.0));//green
  G4VisAttributes* VisAttributes_blue = new G4VisAttributes(G4Colour(0.0,0.0,1.0));//blue
  EJ280PlateLogicalVolume->SetVisAttributes(VisAttributes_green);
  
  //Sensitive Detector
  pmtLogicalVolume2->SetVisAttributes(VisAttributes_blue);

  return experimentalHallPhysicalVolume;  

}

//===================================================================================================================================================

G4VPhysicalVolume* Project1DetectorConstruction::makeMode3()//Fully Understand //For TPB+EJ280
{
  //rotating-----------------------------------------------------------------------------------------------------------------------------------------	
  G4RotationMatrix* RotationMatrix = new G4RotationMatrix();
  RotationMatrix->rotateX(CLHEP::pi / 2.0 * rad);
  G4RotationMatrix* RotationMatrix2 = new G4RotationMatrix();
  RotationMatrix2->rotateX(CLHEP::pi / -2.0 * rad);
  
  // 1.Experimental Hall-----------------------------------------------------------------------------------------------------------------------------
  experimentalHallSolid = new G4Box("experimentalHallSolid", experimentalHallX, experimentalHallY, experimentalHallZ);
  experimentalHallLogicalVolume = new G4LogicalVolume(experimentalHallSolid, Material3, "experimentalHallLogicalVolume", 0, 0, 0);
  experimentalHallPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(), experimentalHallLogicalVolume, "experimentalHallPhysicalVolume", 0, false, 0);
  
  // 2.TPB-------------------------------------------------------------------------------------------------------------------------------------------
  G4Box* TPBPlate = new G4Box("TPB", TPBwidth, TPBthickness, TPBlength);
  G4LogicalVolume* TPBPlateLogicalVolume = new G4LogicalVolume(TPBPlate, Material2, "TPBPlateLogicalVolume", 0, 0, 0);
  G4Box* TPBPlate2 = new G4Box("TPB", TPBthickness, EJ280Thickness, TPBlength);
  G4LogicalVolume* TPBPlateLogicalVolume2 = new G4LogicalVolume(TPBPlate2, Material2, "TPBPlateLogicalVolume2", 0, 0, 0);

  //TPB Surface: no need to use because it is dielectric
  /*
  G4OpticalSurface* opticalSurface_TPB1 = new G4OpticalSurface("opticalSurface_TPB1");
  opticalSurface_TPB1->SetModel(glisur);
  opticalSurface_TPB1->SetFinish(ground);
  opticalSurface_TPB1->SetType(dielectric_LUT);
  new G4LogicalSkinSurface("TPB1LogicalSkinSurface", TPBPlateLogicalVolume, opticalSurface_TPB1);
  G4OpticalSurface* opticalSurface_TPB2 = new G4OpticalSurface("opticalSurface_TPB2");
  opticalSurface_TPB2->SetModel(glisur);
  opticalSurface_TPB2->SetFinish(ground);
  opticalSurface_TPB2->SetType(dielectric_LUT);
  new G4LogicalSkinSurface("TPB1LogicalSkinSurface", TPBPlateLogicalVolume, opticalSurface_TPB2);
  */


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
  
  // 3.EJ280-------------------------------------------------------------------------------------------------------------------------------------------
  G4Box* EJ280Plate = new G4Box("EJ280Plate", EJ280Width, EJ280Thickness, EJ280Length);
  G4LogicalVolume* EJ280PlateLogicalVolume = new G4LogicalVolume(EJ280Plate, Material1, "EJ280PlateLogicalVolume", 0, 0, 0);

  //EJ280 Surface: no need to use because it is dielectric
  /*
  G4OpticalSurface* opticalSurface_EJ280 = new G4OpticalSurface("opticalSurface_EJ280");
  opticalSurface_EJ280->SetModel(glisur);
  opticalSurface_EJ280->SetFinish(polished);
  opticalSurface_EJ280->SetType(dielectric_LUT);
  new G4LogicalSkinSurface("EJ280LogicalSkinSurface", EJ280PlateLogicalVolume, opticalSurface_EJ280);
  */

  new G4PVPlacement(0,
                    G4ThreeVector(0.0, EJ280Thickness+pmtThickness, 0.0),
                    EJ280PlateLogicalVolume,
                    "EJ280Plate",
                    experimentalHallLogicalVolume,
                    false, 0);
  
  G4SDManager* sdManager = G4SDManager::GetSDMpointer();

  G4VSensitiveDetector* Material1_Sd = (Project1Material1Sd*) sdManager->FindSensitiveDetector(MATERIAL1_SENSITIVE_DETECTOR_NAME, false);
  if (!Material1_Sd) {
    Material1_Sd = new Project1Material1Sd(MATERIAL1_SENSITIVE_DETECTOR_NAME);
    sdManager->AddNewDetector(Material1_Sd);
  }

  EJ280PlateLogicalVolume->SetSensitiveDetector(Material1_Sd);

  // 4.Pmt---------------------------------------------------------------------------------------------------------------------------------------------  
  G4LogicalVolume* pmtLogicalVolume2 = makePmtLogicalVolume_plate2();
  
  //simple 2 hitCounter
  /*
  new G4PVPlacement(0,
                    G4ThreeVector(0.0, pmtThickness+EJ280Thickness, TPBlength+pmtThickness),
                    pmtLogicalVolume2,
                    "pmt2",
                    experimentalHallLogicalVolume,
                    false,
                    0);

  new G4PVPlacement(0,
                    G4ThreeVector(0.0, pmtThickness+EJ280Thickness, -TPBlength-pmtThickness),
                    pmtLogicalVolume2,
                    "pmt3",
                    experimentalHallLogicalVolume,
                    false,
                    0);
  */
  
  //2x2 hitCounter or SiPM
  new G4PVPlacement(0,
                    G4ThreeVector(-2.57*cm, pmtThickness+EJ280Thickness, TPBlength+pmtThickness),
                    pmtLogicalVolume2,
                    "pmt2",
                    experimentalHallLogicalVolume,
                    false,
                    0);

  new G4PVPlacement(0,
                    G4ThreeVector(2.57*cm, pmtThickness+EJ280Thickness, TPBlength+pmtThickness),
                    pmtLogicalVolume2,
                    "pmt2",
                    experimentalHallLogicalVolume,
                    false,
                    0);

  new G4PVPlacement(0,
                    G4ThreeVector(-2.57*cm, pmtThickness+EJ280Thickness, -TPBlength-pmtThickness),
                    pmtLogicalVolume2,
                    "pmt3",
                    experimentalHallLogicalVolume,
                    false,
                    0);

  new G4PVPlacement(0,
                    G4ThreeVector(2.57*cm, pmtThickness+EJ280Thickness, -TPBlength-pmtThickness),
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

  //color-------------------------------------------------------------------------------------------------------------------------------------------------
  //For color //(1.0,1.0,1.0))-white, (0.0,0.0,1.0))-blue, (1.0,0.0,0.0))-red, (0.0,1.0,0.0))-green, (1.0,0.0,1.0))-magenta
  G4VisAttributes* VisAttributes_magenta = new G4VisAttributes(G4Colour(1.0,0.0,1.0));//magenta
  G4VisAttributes* VisAttributes_green = new G4VisAttributes(G4Colour(0.0,1.0,0.0));//red
  G4VisAttributes* VisAttributes_blue = new G4VisAttributes(G4Colour(0.0,0.0,1.0));//blue
  TPBPlateLogicalVolume->SetVisAttributes(VisAttributes_magenta);
  TPBPlateLogicalVolume2->SetVisAttributes(VisAttributes_magenta);
  EJ280PlateLogicalVolume->SetVisAttributes(VisAttributes_green);
  pmtLogicalVolume2->SetVisAttributes(VisAttributes_blue);

  return experimentalHallPhysicalVolume;  
}

//=======================================================================================================================================================================

G4VPhysicalVolume* Project1DetectorConstruction::makeMode4()//Fully Understand //For TPB+EJ280
{

  //larger hall needed--------------------------------------------------------------------------------------------------------------------------------------	
  experimentalHallX = (1000)*cm;
  experimentalHallY = (1000)*cm;
  experimentalHallZ = (1000)*cm;

  // 1.Experimental Hall
  experimentalHallSolid = new G4Box("experimentalHallSolid", experimentalHallX, experimentalHallY, experimentalHallZ);
  experimentalHallLogicalVolume = new G4LogicalVolume(experimentalHallSolid, Material3, "experimentalHallLogicalVolume", 0, 0, 0);
  experimentalHallPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(), experimentalHallLogicalVolume, "experimentalHallPhysicalVolume", 0, false, 0);

  //opticalUnit(EJ280+TPB+pmt)---------------------------------------------------------------------------------------------------------------------------------------------
  G4LogicalVolume* OpticalUnitLogicalVolume =  makeOpticalUnit();	

  //rotating for opticalUnit and LAr tank
  G4RotationMatrix* RotationMatrix_90 = new G4RotationMatrix();
  RotationMatrix_90->rotateZ(CLHEP::pi / 2.0 * rad);
  G4RotationMatrix* RotationMatrix_45 = new G4RotationMatrix();
  RotationMatrix_45->rotateZ(CLHEP::pi / 4.0 * rad);
  G4RotationMatrix* RotationMatrix_135 = new G4RotationMatrix();
  RotationMatrix_135->rotateZ(CLHEP::pi / -4.0 * rad);
  G4RotationMatrix* RotationMatrix_180 = new G4RotationMatrix();
  RotationMatrix_180->rotateZ(CLHEP::pi* rad); // defined so i can flip the dome! - Firas
  G4RotationMatrix* RotationMatrix_360 = new G4RotationMatrix();
  RotationMatrix_360->rotateZ(CLHEP::pi * 2 * rad); // defined so i can flip the dome! - Firas //???

  //Diameter 32cm
  G4double position_32_1 = 16*cm+EJ280Thickness+TPBthickness;
  G4double position_32_2 = (position_32_1*std::sin(CLHEP::pi / 4.0 * rad)); //11.314*cm;
    
    
  //LAr and Steel tank(Firas)----------------------------------------------------------------------------------------------------------------------------------
  
    G4Tubs* SteelCylinder = new G4Tubs("Steel Cylinder",700/2*cm,720/2*cm,400/2*cm,0*deg, 360*deg);
    G4LogicalVolume* SteelCylinderLogic = new G4LogicalVolume(SteelCylinder,Material4,"SteelCylinderLogic",0,0,0);

    new G4PVPlacement(0,
                      G4ThreeVector(0.0, 0.0, 0.0),
                      SteelCylinderLogic,
                      "Cylinder",
                      experimentalHallLogicalVolume,
                      false, 0);

    G4Tubs* ArgonCylinder = new G4Tubs("Argon Cylinder",0*cm,700/2*cm,400/2*cm,0*deg,360*deg);
    G4LogicalVolume* ArgonCylinderLogic = new G4LogicalVolume(ArgonCylinder,Material3,"ArgonFilledCylinder",0,0,0);
    new G4PVPlacement(0,
                      G4ThreeVector(0.0, 0.0, 0.0),
                      ArgonCylinderLogic,
                      "ArgonCylinderTank",
                      experimentalHallLogicalVolume,
                      false, 0);
    
    G4Ellipsoid* ArgonDome1 = new G4Ellipsoid("ArgonDome1",700/2*cm,700/2*cm,150*cm,0,700*5);
    G4LogicalVolume* ArgonDomeLogic = new G4LogicalVolume(ArgonDome1,Material3,"+ArDome",0,0,0);
    new G4PVPlacement(0,
                      G4ThreeVector(0.0, 0.0, 400/2*cm),
                      ArgonDomeLogic,
                      "ArgonDome1",
                      experimentalHallLogicalVolume,
                      false, 0);
    

    G4Ellipsoid* ArgonDome2 = new G4Ellipsoid("ArgonDome2",700/2*cm,700/2*cm,150*cm,-700*5,0);
    G4LogicalVolume* ArgonDomeLogic2 = new G4LogicalVolume(ArgonDome2,Material3,"-ArDome",0,0,0);
    new G4PVPlacement(0,
                      G4ThreeVector(0.0, 0.0, -400/2*cm),
                      ArgonDomeLogic2,
                      "ArgonDome2",
                      experimentalHallLogicalVolume,
                      false, 0);
    
    G4Ellipsoid* SteelDome1 = new G4Ellipsoid("SteelDome1",720/2*cm,720/2*cm,(150+5)*cm,0*cm,700*5);
    G4SubtractionSolid* SteelDomeA = new G4SubtractionSolid("SteelDomeA",SteelDome1,ArgonDome1,0,G4ThreeVector(0,0,0*cm));
    G4LogicalVolume* SteelDomeLogic = new G4LogicalVolume(SteelDomeA,Material4,"SteelDomeLogic1",0,0,0);
    new G4PVPlacement(0,
                      G4ThreeVector(0.0, 0.0, 400/2*cm),
                      SteelDomeLogic,
                      "Dome1",
                      experimentalHallLogicalVolume,
                      false, 0);
    
    G4Ellipsoid* SteelDome2 = new G4Ellipsoid("SteelDome2",720/2*cm,720/2*cm,(150+5)*cm,-700*5,0);
    G4SubtractionSolid* SteelDomeB = new G4SubtractionSolid("SteelDomeB",SteelDome2,ArgonDome2,0,G4ThreeVector(0,0,0*cm));
    G4LogicalVolume* SteelDomeLogic2 = new G4LogicalVolume(SteelDomeB,Material4,"SteelDomeLogic2",0,0,0);
    new G4PVPlacement(0,
                      G4ThreeVector(0.0, 0.0, -400/2*cm),
                      SteelDomeLogic2,
                      "Dome2",
                      experimentalHallLogicalVolume,
                      false, 0);

    G4VisAttributes* VisAttributes_blue = new G4VisAttributes(G4Colour(0.0,0.0,1.0));//blue
    G4VisAttributes* VisAttributes_grey = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5));//grey
    SteelCylinderLogic->SetVisAttributes(VisAttributes_grey);
    SteelDomeLogic->SetVisAttributes(VisAttributes_grey);
    SteelDomeLogic2->SetVisAttributes(VisAttributes_grey);
    ArgonCylinderLogic->SetVisAttributes(VisAttributes_blue);
    ArgonDomeLogic->SetVisAttributes(VisAttributes_blue);
    ArgonDomeLogic2->SetVisAttributes(VisAttributes_blue);
    
  //opticalUnit-------------------------------------------------------------------------------------------------------------------------------------------------------
    
  new G4PVPlacement(0,
                    G4ThreeVector(0.0 - 0.35*cm, position_32_1, 0.0),
                    OpticalUnitLogicalVolume,
                    "OpticalUnitLogicalVolume",
                    experimentalHallLogicalVolume,
                    false,
                    0);

  new G4PVPlacement(0,
                    G4ThreeVector(0.0 + 0.35*cm, -position_32_1, 0.0),
                    OpticalUnitLogicalVolume,
                    "OpticalUnitLogicalVolume",
                    experimentalHallLogicalVolume,
                    false,
                    0);

  new G4PVPlacement(RotationMatrix_90,
                    G4ThreeVector(position_32_1, 0.0 + 0.35*cm, 0.0),
                    OpticalUnitLogicalVolume,
                    "OpticalUnitLogicalVolume",
                    experimentalHallLogicalVolume,
                    false,
                    0);

  new G4PVPlacement(RotationMatrix_90,
                    G4ThreeVector(-position_32_1, 0.0 - 0.35*cm, 0.0),
                    OpticalUnitLogicalVolume,
                    "OpticalUnitLogicalVolume",
                    experimentalHallLogicalVolume,
                    false,
                    0);

  new G4PVPlacement(RotationMatrix_45,
                    G4ThreeVector(position_32_2-(0.35*std::sin(CLHEP::pi / 4.0 * rad))*cm, position_32_2+(0.35*std::sin(CLHEP::pi / 4.0 * rad))*cm, 0.0),
                    OpticalUnitLogicalVolume,
                    "OpticalUnitLogicalVolume",
                    experimentalHallLogicalVolume,
                    false,
                    0);

  new G4PVPlacement(RotationMatrix_135,
                    G4ThreeVector(-position_32_2-(0.35*std::sin(CLHEP::pi / 4.0 * rad))*cm, position_32_2-(0.35*std::sin(CLHEP::pi / 4.0 * rad))*cm, 0.0),
                    OpticalUnitLogicalVolume,
                    "OpticalUnitLogicalVolume",
                    experimentalHallLogicalVolume,
                    false,
                    0);

  new G4PVPlacement(RotationMatrix_45,
                    G4ThreeVector(-position_32_2+(0.35*std::sin(CLHEP::pi / 4.0 * rad))*cm, -position_32_2-(0.35*std::sin(CLHEP::pi / 4.0 * rad))*cm, 0.0),
                    OpticalUnitLogicalVolume,
                    "OpticalUnitLogicalVolume",
                    experimentalHallLogicalVolume,
                    false,
                    0);

  new G4PVPlacement(RotationMatrix_135,
                    G4ThreeVector(position_32_2+(0.35*std::sin(CLHEP::pi / 4.0 * rad))*cm, -position_32_2+(0.35*std::sin(CLHEP::pi / 4.0 * rad))*cm, 0.0),
                    OpticalUnitLogicalVolume,
                    "OpticalUnitLogicalVolume",
                    experimentalHallLogicalVolume,
                    false,
                    0);

  return experimentalHallPhysicalVolume;
}

//=================================================================================================================================== 3. Geometry for different modes
//===================================================================================================================================================================

G4LogicalVolume* Project1DetectorConstruction::makePmtLogicalVolume_plate1()//pmt1 only used on Mode 1 and 2
{
  //Geometery-------------------------------------------------------------------------------------------------------------------------------------- 
  G4Box* photocathodeSolid = new G4Box("pmtSolid", EJ280Width, pmtThickness, EJ280Length);
  G4LogicalVolume* photocathodeLogicalVolume = new G4LogicalVolume(photocathodeSolid, galactic, "photocathodeLogicalVolume");

  //QE---------------------------------------------------------------------------------------------------------------------------------------------
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

  //Surface----------------------------------------------------------------------------------------------------------------------------------------
  G4OpticalSurface* opticalSurface_photocathode = new G4OpticalSurface("opticalSurface_photocathode");
  G4MaterialPropertiesTable* opticalSurfaceMpt = new G4MaterialPropertiesTable();
  opticalSurfaceMpt->AddProperty("EFFICIENCY", &energies[0], &quantumEfficiencies[0], (G4int) energies.size());//QE
  opticalSurfaceMpt->AddProperty("REFLECTIVITY", &energies[0], &reflectivities[0], (G4int) energies.size());//Reflection
  opticalSurfaceMpt->AddProperty("RINDEX", &energies[0], &refractionIndices[0], (G4int) energies.size());//Refraction
  opticalSurface_photocathode->SetModel(glisur);
  opticalSurface_photocathode->SetFinish(polished);
  opticalSurface_photocathode->SetType(dielectric_metal);
  opticalSurface_photocathode->SetMaterialPropertiesTable(opticalSurfaceMpt);
  new G4LogicalSkinSurface("pmtLogicalSkinSurface", photocathodeLogicalVolume, opticalSurface_photocathode);

  return photocathodeLogicalVolume;
}

G4LogicalVolume* Project1DetectorConstruction::makePmtLogicalVolume_plate2()//pmt2 used on Mode 3,4 and others.
{
  //Geometry---------------------------------------------------------------------------------------------------------------------------------------

  //Simple hitCounter    	
  G4Box* photocathodeSolid = new G4Box("pmtSolid", EJ280Width/2., EJ280Thickness, pmtThickness);
  G4LogicalVolume* photocathodeLogicalVolume = new G4LogicalVolume(photocathodeSolid, galactic, "photocathodeLogicalVolume");

  //SiPM
  G4Box* longArray = new G4Box("long array", EJ280Width/2 * mm, EJ280Thickness, pmtThickness);
  G4LogicalVolume* longLogic = new G4LogicalVolume(longArray,galactic,"arrayLogic");
  G4Box* psaSIPM = new G4Box("photo sensitive area SIPM", 0.15 * cm, EJ280Thickness, pmtThickness);
  G4LogicalVolume* psaLogic = new G4LogicalVolume(psaSIPM,galactic,"psaLogic");
  const G4double arrayPitch = 51.4 * mm;
  const G4double pixelPitch = 3.2 * mm; 
  
  //QE---------------------------------------------------------------------------------------------------------------------------------------------
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

  //Surface----------------------------------------------------------------------------------------------------------------------------------------
  G4OpticalSurface* opticalSurface = new G4OpticalSurface("opticalSurface");
  G4MaterialPropertiesTable* opticalSurfaceMpt = new G4MaterialPropertiesTable();
  opticalSurfaceMpt->AddProperty("EFFICIENCY", &energies[0], &quantumEfficiencies[0], (G4int) energies.size());//QE
  opticalSurfaceMpt->AddProperty("REFLECTIVITY", &energies[0], &reflectivities[0], (G4int) energies.size());//Reflection
  opticalSurfaceMpt->AddProperty("RINDEX", &energies[0], &refractionIndices[0], (G4int) energies.size());//Refraction
  opticalSurface->SetModel(glisur);
  opticalSurface->SetFinish(polished);
  opticalSurface->SetType(dielectric_metal);
  opticalSurface->SetMaterialPropertiesTable(opticalSurfaceMpt);
  
  //Simple hitCounter
  /*
  new G4LogicalSkinSurface("pmtLogicalSkinSurface", photocathodeLogicalVolume, opticalSurface);
  return photocathodeLogicalVolume;
  */

  //SiPM
  new G4LogicalSkinSurface("pmtLogicalSkinSurface", psaLogic, opticalSurface);
  for(G4int I = 0 ; I < 16; I++){
                new G4PVPlacement(0,G4ThreeVector(-arrayPitch/2 + 1.7 + I * pixelPitch,0,0), // place photo sensitive areas into circuit board
                      psaLogic, "arraySIPM", longLogic, false, I);
  }
  return longLogic;
 
}

//========================================================================================================================================================4. PMT(SiPM)

G4LogicalVolume* Project1DetectorConstruction::makeOpticalUnit()
{
  OpticalUnitSolidX = TPBwidth;
  OpticalUnitSolidY = EJ280Thickness+2*TPBthickness;
  OpticalUnitSolidZ = (TPBlength + 2*pmtThickness); 

  G4RotationMatrix* RotationMatrix = new G4RotationMatrix();
  RotationMatrix->rotateX(CLHEP::pi / 2.0 * rad);
  G4RotationMatrix* RotationMatrix2 = new G4RotationMatrix();
  RotationMatrix2->rotateX(CLHEP::pi / -2.0 * rad);

  // 1.Experimental Hall-----------------------------------------------------------------------------------------------------------------------------------
  OpticalUnitSolid = new G4Box("OpticalUnitSolid", OpticalUnitSolidX, OpticalUnitSolidY, OpticalUnitSolidZ);
  OpticalUnitLogicalVolume = new G4LogicalVolume(OpticalUnitSolid, Material3, "OpticalUnitLogicalVolume", 0, 0, 0);
  OpticalUnitPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(), OpticalUnitLogicalVolume, "OpticalUnitPhysicalVolume", 0, false, 0);
  
  // 2.TPB-------------------------------------------------------------------------------------------------------------------------------------------------
  G4Box* TPBPlate = new G4Box("TPB", TPBwidth, TPBthickness, TPBlength);
  G4LogicalVolume* TPBPlateLogicalVolume = new G4LogicalVolume(TPBPlate, Material2, "TPBPlateLogicalVolume", 0, 0, 0);
  G4Box* TPBPlate2 = new G4Box("TPB", TPBthickness, EJ280Thickness, TPBlength);
  G4LogicalVolume* TPBPlateLogicalVolume2 = new G4LogicalVolume(TPBPlate2, Material2, "TPBPlateLogicalVolume2", 0, 0, 0);
    
    
    
  //Top
  new G4PVPlacement(0,
                    G4ThreeVector(0.0, TPBthickness+EJ280Thickness, 0.0),
                    TPBPlateLogicalVolume,
                    "TPBPlate",
                    OpticalUnitLogicalVolume,
                    false, 0);
  //Bottom
  new G4PVPlacement(0,
                    G4ThreeVector(0.0, -TPBthickness-EJ280Thickness, 0.0),
                    TPBPlateLogicalVolume,
                    "TPBPlate",
                    OpticalUnitLogicalVolume,
                    false, 0);
  //Left
  new G4PVPlacement(0,
                    G4ThreeVector(TPBwidth+TPBthickness, 0.0, 0.0),
                    TPBPlateLogicalVolume2,
                    "TPBPlate2",
                    OpticalUnitLogicalVolume,
                    false, 0);
  //Right
  new G4PVPlacement(0,
                    G4ThreeVector(-TPBwidth-TPBthickness, 0.0, 0.0),
                    TPBPlateLogicalVolume2,
                    "TPBPlate2",
                    OpticalUnitLogicalVolume,
                    false, 0);
  
  G4VisAttributes* plateVisAttributes = new G4VisAttributes();
  plateVisAttributes->SetVisibility(true);
  plateVisAttributes->SetForceSolid(true);
  
  // 3.EJ280----------------------------------------------------------------------------------------------------------------------------------------------------
  G4Box* EJ280Plate = new G4Box("EJ280Plate", EJ280Width, EJ280Thickness, EJ280Length);
  G4LogicalVolume* EJ280PlateLogicalVolume = new G4LogicalVolume(EJ280Plate, Material1, "EJ280PlateLogicalVolume", 0, 0, 0);
 
  new G4PVPlacement(0,
                    G4ThreeVector(0.0, 0.0, 0.0),
                    EJ280PlateLogicalVolume,
                    "EJ280Plate",
                    OpticalUnitLogicalVolume,
                    false, 0);

  G4SDManager* sdManager = G4SDManager::GetSDMpointer();

  G4VSensitiveDetector* Material1_Sd = (Project1Material1Sd*) sdManager->FindSensitiveDetector(MATERIAL1_SENSITIVE_DETECTOR_NAME, false);//***
  if (!Material1_Sd) {
    Material1_Sd = new Project1Material1Sd(MATERIAL1_SENSITIVE_DETECTOR_NAME);
    sdManager->AddNewDetector(Material1_Sd);
  }

  EJ280PlateLogicalVolume->SetSensitiveDetector(Material1_Sd);

  // 4.Pmt------------------------------------------------------------------------------------------------------------------------------------------------------  
  G4LogicalVolume* pmtLogicalVolume2 = makePmtLogicalVolume_plate2();
  new G4PVPlacement(0,
                    G4ThreeVector(0.0, 0.0, TPBlength+pmtThickness),
                    pmtLogicalVolume2,
                    "pmt2",
                    OpticalUnitLogicalVolume,
                    false,
                    0);

  new G4PVPlacement(0,
                    G4ThreeVector(0.0, 0.0, -TPBlength-pmtThickness),
                    pmtLogicalVolume2,
                    "pmt3",
                    OpticalUnitLogicalVolume,
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

  //color--------------------------------------------------------------------------------------------------------------------------------------------------------
  //For color //(1.0,1.0,1.0))-white, (0.0,0.0,1.0))-blue, (1.0,0.0,0.0))-red, (0.0,1.0,0.0))-green, (1.0,0.0,1.0))-magenta
  G4VisAttributes* VisAttributes_magenta = new G4VisAttributes(G4Colour(1.0,0.0,1.0));//magenta
  G4VisAttributes* VisAttributes_green = new G4VisAttributes(G4Colour(0.0,1.0,0.0));//green
  G4VisAttributes* VisAttributes_blue = new G4VisAttributes(G4Colour(0.0,0.0,1.0));//blue
  G4VisAttributes* VisAttributes_red = new G4VisAttributes(G4Colour(1.0,0.0,0.0));//blue
  TPBPlateLogicalVolume->SetVisAttributes(VisAttributes_magenta);
  TPBPlateLogicalVolume2->SetVisAttributes(VisAttributes_magenta);
  EJ280PlateLogicalVolume->SetVisAttributes(VisAttributes_green);
  pmtLogicalVolume2->SetVisAttributes(VisAttributes_blue);
  OpticalUnitLogicalVolume->SetVisAttributes(VisAttributes_red);

  return OpticalUnitLogicalVolume;
}

//===================================================================================================================================================5. Optical Unit
