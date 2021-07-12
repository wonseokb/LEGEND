#ifndef Project1DetectorConstruction_H   //Matbe it only needed for header file.
#define Project1DetectorConstruction_H 1

#include <G4NistManager.hh>
#include "G4SystemOfUnits.hh"
#include <G4UnionSolid.hh>
#include "G4Material.hh"
#include "Project1DetectorMessenger.hh"
#include "G4VisAttributes.hh"
#include "G4RotationMatrix.hh"
#include "G4VUserDetectorConstruction.hh"


class G4LogicalVolume;//void class. it is not from header file. G4 original class
class G4VPhysicalVolume;//void class. it is not from header file. G4 original class
class G4Box;//void class. it is not from header file. G4 original class
class G4Tubs;//void class. it is not from header file. G4 original class
class Project1PmtSd;//void class.
class Project1Material1Sd;//void class.


class Project1DetectorConstruction : public G4VUserDetectorConstruction //Class
{
  public:
    Project1DetectorConstruction();//constructor
    virtual ~Project1DetectorConstruction();//Destructor. what is vritual???
    virtual G4VPhysicalVolume* Construct();//pointer type variable? pointer type function?

    //12 set-functions : set the values of variables (Width, Height, Length, Thinkness, Radius...) -> set-functinos
    void setSimulationMode(G4String a) {simulationMode = a; isUpdated = true;}
    void setPhotodetectorType(G4String a) {photodetectorType = a; isUpdated = true;}
    void setLiquidScintillatorLightYield(G4double a) {Material1_LightYield = a; isUpdated = true;};
    void setDefaults();
 
    void updateDetector();//used as "detectorConstruction->updateDetector();"
    //void printSetting();//used as "printSetting = new G4UIcmdWithoutParameter("/nova/printSetting", this);"
    void readCsvFile(G4String filename, std::vector<G4double>& energies, std::vector<G4double>& values, double valueUnit);//read a csv file.
    //what is difference of vector<G4double>& and double and G4double?
    G4double convertWavelengthToEnergy(G4double wavelength) { return PLANK_CONSTANT / wavelength * eV;  }// Energy = (hc)/lamda = 1239.84193/wavelength

  private:
    
    //fixed variables
    G4double PLANK_CONSTANT = 1239.84193;
    //G4double MATERIAL2_REFRACTION_INDEX = 1.59;
        
    //fixed files
    G4String INPUT_DIR = "input";
    //====================================================================================================
    G4String MATERIAL1_EMISSION_FILENAME =
    "6_EJ280_WLSemission_460-600nm.txt";
    G4String MATERIAL1_ABSORPTION_LENGTH_FILENAME =
    //"5_EJ280_WLSabsorptionLength_300-480nm.txt";//doesn't matter
    //"5_EJ280_WLSabsorptionLength_350-480nm.txt";//doesn't matter
    "5_EJ280_WLSabsorptionLength_constant.txt";
    G4String MATERIAL1_WLS_ABSORPTION_LENGTH_FILENAME = 
    //"5_EJ280_WLSabsorptionLength_300-480nm.txt";//doesn't matter
    //"5_EJ280_WLSabsorptionLength_350-480nm.txt";//doesn't matter
    "5_EJ280_WLSabsorptionLength_constant.txt";
    G4String MATERIAL1_WLS_EMISSION_FILENAME = 
    "6_EJ280_WLSemission_460-600nm.txt"; 
    //====================================================================================================
    G4String MATERIAL2_ABSORPTION_LENGTH_FILENAME =
    //"3_TPB_WLSabsorptionLegnth_GERDA_50-250.txt";//not bad
    //"3_TPB_WLSabsorptionLegnth_GERDA_50-250&275-415nm.txt";
    "3_TPB_WLSabsorptionLegnth_GERDA_50-415nm.txt"; //good
    //"3_TPB_WLSabsorptionLength_Springer_275-415nm.txt"; //bad
    G4String MATERIAL2_WLS_ABSORPTION_LENGTH_FILENAME =
    //"3_TPB_WLSabsorptionLegnth_GERDA_50-250.txt";//not bad
    //"3_TPB_WLSabsorptionLegnth_GERDA_50-250&275-415nm.txt";
    "3_TPB_WLSabsorptionLegnth_GERDA_50-415nm.txt"; //good
    //"3_TPB_WLSabsorptionLength_Springer_275-415nm.txt"; //bad
    G4String MATERIAL2_WLS_EMISSION_FILENAME = 
    "4_TPB_WLSemission_GERDA_350-600nm.txt";//good
    //"4_TPB_WLSemission_Springer_350-650nm.txt";
    G4String MATERIAL2_REFRACTION_INDEX_FILENAME = 
    "2_TPB_refraction_index_constant.txt";//not bad
    //====================================================================================================
    G4String MATERIAL3_REFRACTION_INDEX_FILENAME = 
    //"1_LAr_refraction_index_110-500nm.txt";
    "1_LAr_refraction_index_110-700nm.txt";//good
    //====================================================================================================
    G4String QUANTUM_EFFICIENCY_FILENAME = "7_QuantumEfficiency.txt"; 
    G4String APD_QUANTUM_EFFICIENCY_FILENAME = "apd_qe.dat";
    G4String PMT_QUANTUM_EFFICIENCY_FILENAME = "pmt_qe.dat";
    G4String MATERIAL1_SENSITIVE_DETECTOR_NAME = "/NovaDet/liquidScintillatorSd";
    G4String PMT_SENSITIVE_DETECTOR_NAME = "/NovaDet/pmtSd";

    //get-function
    G4String getFilePath(G4String filename) {return INPUT_DIR + "/" + filename;}
    //set-functions //Set Material Properties
    void defineMaterials();
    void setMaterialProperties();
    void setGlassProperty();
    void setMaterial1Property();
    void setMaterial2Property();
    void setMaterial3Property();
    void setMaterial4Property();
    void TEST();
    void setGalacticProperty();
    void setPvcSurfaceProperty(G4LogicalVolume* pvcLogicalVolume, G4bool turnOffReflectivity = false);

    //Union Solid : Solid + Solid (pointer type)
    
    //Logical Volume (pointer type)
    G4LogicalVolume* makePmtLogicalVolume_cylinder();
    G4LogicalVolume* makePmtLogicalVolume_plate1();
    G4LogicalVolume* makePmtLogicalVolume_plate2();
    G4LogicalVolume* makePmtLogicalVolume1();
    G4LogicalVolume* makePmtLogicalVolume2();

    //Physical Volume (pointer type)
    G4VPhysicalVolume* makeDetectorPhysicalVolume();
    G4VPhysicalVolume* makeMode1();
    G4VPhysicalVolume* makeMode2();
    G4VPhysicalVolume* makeMode3();
    G4VPhysicalVolume* makeMode4();
    
    G4bool isUpdated;//isUpdated = true/false, It used to be a variable of functions. So, it can be a "switch" to open/close the update
    G4Box* experimentalHallSolid;//Box  (pointer type)
    G4LogicalVolume* experimentalHallLogicalVolume;//Logical Volume  (pointer type)
    G4VPhysicalVolume* experimentalHallPhysicalVolume;//Physical Volume  (pointer type)
    G4NistManager* nistManager;//It used as "nistManager = G4NistManager::Instance();" , "H = nistManager->FindOrBuildElement("H");"

    //edit
    G4Box* TPBSolid;//Box  (pointer type)
    G4Box* TPBSolid2;
    G4Box* TPBSolid3;
    G4LogicalVolume* TPBLogicalVolume;//Logical Volume  (pointer type)
    G4LogicalVolume* TPBLogicalVolume2;
    G4LogicalVolume* TPBLogicalVolume3;
    G4VPhysicalVolume* TPBPhysicalVolume;//Physical Volume  (pointer type)


    //Element (pointer type)
    G4Element* H;
    G4Element* C;
    //G4Element* O;
    //G4Element* Ti;

    //Material (pointer type)
    //G4Material* tiO2;
    //G4Material* Material5;//polystyrene
    //G4Material* pvc;
    G4Material* Material1;//LiquidScintillator->EJ280
    G4Material* Material2;//fiberCore->TPB
    G4Material* Material3;//pmma->LAr
    //G4Material* Material4;//fluorinatedPolymer
    G4Material* glass;
    //G4Material* aluminum;
    G4Material* galactic;

    // for Mpt(the Volume which has physical constant ex. Refractive Index, WLS time constant. 
    // #include "G4MaterialPropertiesTable.hh" (pointer type)
    G4MaterialPropertiesTable* Material1_Mpt;//EJ280
    G4MaterialPropertiesTable* Material2_Mpt;//TPB
    G4MaterialPropertiesTable* Material3_Mpt;//LAr
    //G4MaterialPropertiesTable* Material4_Mpt;//fluorinatedPolymer

    //Declare variables
    G4double Material1_LightYield;
    G4double Material2_LightYield;
    G4double pmtThickness;
    G4String photodetectorType;
    G4String simulationMode;
    G4double experimentalHallX;
    G4double experimentalHallY;
    G4double experimentalHallZ;
    G4double TPBthickness;
    G4double TPBwidth;
    G4double TPBlength;
    G4double EJ280Thickness;
    G4double EJ280Width;
    G4double EJ280Length;
    G4double EJ280Radius;
};

#endif
