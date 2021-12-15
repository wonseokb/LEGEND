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

G4double experimentalHallX = 1000*cm;
G4double experimentalHallY = 1000*cm;
G4double experimentalHallZ = 1000*cm;
G4double TPBthickness = 3*um;//half
G4double EJ280Thickness = 1.0*mm;//half
G4double EJ280Length = 50*cm;//half
G4double EJ280Radius = 5*cm;//not half
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

//======================================================================================================================================================== 1. Constructor & Destructor

void Project1DetectorConstruction::setMaterial1Property()//***
{
  Material1_Mpt = new G4MaterialPropertiesTable();

  std::vector<G4double> emissionEnergies;//No use
  std::vector<G4double> fastEmissions;//No use
  readCsvFile(MATERIAL1_EMISSION_FILENAME, emissionEnergies, fastEmissions, 1.0);//No use
  std::vector<G4double> slowEmissions(emissionEnergies.size(), 0.0);//No use 
  
  std::vector<G4double> absorptionEnergies;//No use
  std::vector<G4double> absorptions;//No use
  readCsvFile(MATERIAL1_ABSORPTION_LENGTH_FILENAME, absorptionEnergies, absorptions, 1.8 * m);//No use

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

void Project1DetectorConstruction::setMaterial2Property()//***
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
  Material2->SetMaterialPropertiesTable(Material2_Mpt);
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
  
  Material1 = new G4Material("Material1", 0.862*g/cm3, 2, kStateLiquid, 273.15*kelvin, 1.0*atmosphere);
  Material1->AddElement(H, 10);
  Material1->AddElement(C, 9);
  //CH2CH(C6H4CH3)

  Material2 = new G4Material("Material2", 1.079*g/cm3, 2, kStateSolid, 273.15*kelvin, 1.0*atmosphere);
  Material2->AddElement(H, 22);
  Material2->AddElement(C, 28);
  //C28H22
}

void Project1DetectorConstruction::setMaterialProperties()//Make Material Property (Fully Understand)
{
  setMaterial1Property();
  setMaterial2Property();
  setGalacticProperty();
}

G4VPhysicalVolume* Project1DetectorConstruction::Construct()//Make Detector Phys Volume (Fully Understand)
{
  setMaterialProperties();
  return makeDetectorPhysicalVolume();
}

//================================================================================================================================================= 2. MaterialProperties

G4VPhysicalVolume* Project1DetectorConstruction::makeDetectorPhysicalVolume()//Fully Understand
{
  if (simulationMode == "fiber")
    return makeMode1();
  else if (simulationMode == "cell")
    return makeMode2();
  else
    throw "The selected simulation mode does not exist.";
}


G4VPhysicalVolume* Project1DetectorConstruction::makeMode1()//Fully Understand //For Only TPB TEST
{
  return 0;  
}

//=======================================================================================================================================================================

G4VPhysicalVolume* Project1DetectorConstruction::makeMode2()//***
{
  //3.1 Experimental Hall (Fully Understand)
  experimentalHallSolid = new G4Box("experimentalHallSolid", experimentalHallX, experimentalHallY, experimentalHallZ);
  experimentalHallLogicalVolume = new G4LogicalVolume(experimentalHallSolid, galactic, "experimentalHallLogicalVolume", 0, 0, 0);
  experimentalHallPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(), experimentalHallLogicalVolume, "experimentalHallPhysicalVolume", 0, false, 0);

  //-----------------------------------------------------------------------------------------------------------------------------------------
  
  //3.2 TPB+EJ280 (Fully Understand)
  G4Tubs* TPBCylinder = new G4Tubs("TPBCylinder", EJ280Radius , EJ280Radius + TPBthickness, EJ280Length, 0.0, CLHEP::twopi * rad);
  G4LogicalVolume* TPBCylinderLogicalVolume = new G4LogicalVolume(TPBCylinder, Material2, "TPBCylinderLogicalVolume");
  
  G4Tubs* EJ280Cylinder = new G4Tubs("EJ280Cylinder", EJ280Radius-EJ280Thickness, EJ280Radius, EJ280Length, 0.0, CLHEP::twopi * rad);
  G4LogicalVolume* EJ280CylinderLogicalVolume = new G4LogicalVolume(EJ280Cylinder, Material1, "EJ280CylinderLogicalVolume");

  G4Tubs* TPBCylinder2 = new G4Tubs("TPBCylinder2", EJ280Radius-EJ280Thickness-TPBthickness, EJ280Radius-EJ280Thickness, EJ280Length, 0.0, CLHEP::twopi * rad);
  G4LogicalVolume* TPBCylinderLogicalVolume2 = new G4LogicalVolume(TPBCylinder2, Material2, "TPBCylinderLogicalVolume2"); 
   
  //new G4PVPlacement(0, G4ThreeVector(), TPBCylinderLogicalVolume, "TPBCylinder", experimentalHallLogicalVolume, false, 0);
  new G4PVPlacement(0, G4ThreeVector(), EJ280CylinderLogicalVolume, "EJ280Cylinder", experimentalHallLogicalVolume, false, 0);
  //new G4PVPlacement(0, G4ThreeVector(), TPBCylinderLogicalVolume2, "TPBCylinder2", experimentalHallLogicalVolume, false, 0);
  
  G4VisAttributes* visAttributes = new G4VisAttributes();//***
  visAttributes->SetVisibility(true);//***
  visAttributes->SetForceSolid(true);//***
   
  G4SDManager* sdManager = G4SDManager::GetSDMpointer();

  G4VSensitiveDetector* Material1_Sd = (Project1Material1Sd*) sdManager->FindSensitiveDetector(MATERIAL1_SENSITIVE_DETECTOR_NAME, false);//***
  if (!Material1_Sd) {
    Material1_Sd = new Project1Material1Sd(MATERIAL1_SENSITIVE_DETECTOR_NAME);
    sdManager->AddNewDetector(Material1_Sd);
  }
  
  EJ280CylinderLogicalVolume->SetSensitiveDetector(Material1_Sd);
  
  //-------------------------------------------------------------------------------------------------------------------------------------------  

  //3.3 Pmt (Fully Understand)

  G4LogicalVolume* pmtLogicalVolume = makePmtLogicalVolume();
   
  new G4PVPlacement(0, G4ThreeVector(), pmtLogicalVolume, "pmt4", experimentalHallLogicalVolume, false, 0); 
  /*
  new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, EJ280Length+(pmtThickness)), pmtLogicalVolume, "pmt1", experimentalHallLogicalVolume, false, 0);
  new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, -EJ280Length-(pmtThickness)), pmtLogicalVolume, "pmt2", experimentalHallLogicalVolume, false, 0);
  */
  Project1PmtSd* pmtSd = (Project1PmtSd*) sdManager->FindSensitiveDetector(PMT_SENSITIVE_DETECTOR_NAME, false);//***
  if (!pmtSd) {
    pmtSd = new Project1PmtSd(PMT_SENSITIVE_DETECTOR_NAME);
    pmtSd->initPmts(1);
    pmtSd->setPmtPosition(0, 0.0, 0.0, 0.0);
    sdManager->AddNewDetector(pmtSd);
  }
  pmtLogicalVolume->SetSensitiveDetector(pmtSd);

  //-------------------------------------------------------------------------------------------------------------------------------------------
  
  return experimentalHallPhysicalVolume;
}

//======================================================================================================================================================== 3. Volume

G4LogicalVolume* Project1DetectorConstruction::makePmtLogicalVolume()//***
{ 
  //4.1 Geometery (Fully Understand)
  G4Tubs* photocathodeSolid = new G4Tubs("photocathodeSolid", EJ280Radius-EJ280Thickness-pmtThickness, EJ280Radius-EJ280Thickness, EJ280Length, 0.0, CLHEP::twopi * rad);
  G4LogicalVolume* photocathodeLogicalVolume = new G4LogicalVolume(photocathodeSolid, galactic, "photocathodeLogicalVolume");

  //---------------------------------------------------------------------------------------------------------------------------------------------------

  //4.2 QE (Fully Understand)
  G4String quantumEfficiencyFilename;
  if (photodetectorType == "apd") {
    quantumEfficiencyFilename = APD_QUANTUM_EFFICIENCY_FILENAME;
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


//========================================================================================================================================================= 4. Pmt

void Project1DetectorConstruction::setDefaults()//Fully Understand
{
  Material1_LightYield = 0;
  pmtThickness = 1 * um;
  photodetectorType = "apd";
  simulationMode = "cell";
  isUpdated = true;
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

//========================================================================================================================================================= 5. Sub_functions
