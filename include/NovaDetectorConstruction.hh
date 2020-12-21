#ifndef NovaDetectorConstruction_H   //Matbe it only needed for header file.
#define NovaDetectorConstruction_H 1

#include <G4NistManager.hh>
#include "G4SystemOfUnits.hh"
#include <G4UnionSolid.hh>
#include "G4Material.hh"
#include "NovaDetectorMessenger.hh"
#include "G4VisAttributes.hh"
#include "G4RotationMatrix.hh"
#include "G4VUserDetectorConstruction.hh"


class G4LogicalVolume;//void class. it is not from header file. G4 original class
class G4VPhysicalVolume;//void class. it is not from header file. G4 original class
class G4Box;//void class. it is not from header file. G4 original class
class G4Tubs;//void class. it is not from header file. G4 original class
class NovaPmtSd;//void class. it is not from header file. Nova Class
class NovaLiquidScintillatorSd;//void class. it is not from header file. Nova class
class G4Sphere;//void class. it is not from header file. G4 original class

class NovaDetectorConstruction : public G4VUserDetectorConstruction //Class NovaDC inherited from G4UserDC
{
  public:
    NovaDetectorConstruction();//constructor
    virtual ~NovaDetectorConstruction();//Destructor. what is vritual???
    virtual G4VPhysicalVolume* Construct();//pointer type variable? pointer type function?

    //12 set-functions : set the values of variables (Width, Height, Length, Thinkness, Radius...)
    void setStraightWidth(G4double a) {straightWidth = a; isUpdated = true;}
    void setStraightHeight(G4double a) {straightHeight = a; isUpdated = true;}
    void setDetectorLength(G4double a) {detectorLength = a; isUpdated = true;}
    void setSnoutLength(G4double a) {snoutLength = a; isUpdated = true;}
    void setPvcThickness(G4double a) {pvcThickness = a; isUpdated = true;}
    void setInnerCellCornerRadius(G4double a) {innerCellCornerRadius = a; isUpdated = true;}
    void setFiberRadius(G4double a) {fiberRadius = a; isUpdated = true;}
    void setFiberCurveRadius(G4double a) {fiberCurveRadius = a; isUpdated = true;}
    void setSimulationMode(G4String a) {simulationMode = a; isUpdated = true;}
    void setPhotodetectorType(G4String a) {photodetectorType = a; isUpdated = true;}
    void setLiquidScintillatorLightYield(G4double a) {liquidScintillatorLightYield = a; isUpdated = true;};
    void setDefaults();

    //5 get-functions : get the values of variables (Width, Height, Length, Thinkness, Radius...) the type of return value is G4double
    G4double getOuterCellCornerRadius() {return innerCellCornerRadius + pvcThickness;}
    G4double getCellHeight() {return straightHeight + 2.0 * getOuterCellCornerRadius();}//6.6cm (cell=5.94cm, pvc 0.66cm)
    G4double getCellWidth() { return straightWidth + 2.0 * getOuterCellCornerRadius();}
    G4double getFiberCoreRadius() {return fiberRadius * (1.0 - 2.0 * fiberCladdingFraction);};
    G4double getFiberInnerCladdingRadius() {return fiberRadius * (1.0 - fiberCladdingFraction);};

    void updateDetector();//used as "detectorConstruction->updateDetector();"
    void printSetting();//used as "printSetting = new G4UIcmdWithoutParameter("/nova/printSetting", this);"
    void readCsvFile(G4String filename, std::vector<G4double>& energies, std::vector<G4double>& values, double valueUnit);//read a csv file.
    //what is difference of vector<G4double>& and double and G4double?
    G4double convertWavelengthToEnergy(G4double wavelength) { return PLANK_CONSTANT / wavelength * eV;  }// Energy = (hc)/lamda = 1239.84193/wavelength

  private:
    
    //fixed variables
    G4double PLANK_CONSTANT = 1239.84193;
    G4double FIBER_CORE_REFRACTION_INDEX = 1.59;
    G4double FIBER_FIRST_CLADDING_REFRACTION_INDEX = 1.49;
    G4double FIBER_SECOND_CLADDING_REFRACTION_INDEX = 1.42;
    
    //fixed files
    G4String INPUT_DIR = "input";
    G4String PVC_REFLECTIVITY_FILENAME = "avg_ext.txt";
    //====================================================================================================
    G4String LIQUID_SCINTILLATOR_EMISSION_FILENAME = "ppo_emission.txt";
    G4String LIQUID_SCINTILLATOR_ABSORPTION_LENGTH_FILENAME = "PSTBulkAbsorb.cfg";
    G4String LIQUID_SCINTILLATOR_WLS_ABSORPTION_LENGTH_FILENAME = "UPS923.cfg";
    G4String LIQUID_SCINTILLATOR_WLS_EMISSION_FILENAME = "bisMSB_emission.txt";
    //====================================================================================================
    G4String FIBER_CORE_ABSORPTION_LENGTH_FILENAME = "fiberPSTabsorb.dat";
    G4String FIBER_CORE_WLS_ABSORPTION_LENGTH_FILENAME = "y11_abs_length.dat";
    //G4String FIBER_CORE_WLS_ABSORPTION_LENGTH_FILENAME = "TPB_att_length.dat";
    G4String FIBER_CORE_WLS_EMISSION_FILENAME = "k27_emission.csv";
    //G4String FIBER_CORE_WLS_EMISSION_FILENAME = "TPB_emission.csv";
    G4String FIBER_CORE_REFRACTION_INDEX_FILENAME = "refraction_index_polystyrene_k27.csv";
    //====================================================================================================	
    G4String FIBER_CLADDING_ABSORPTION_LENGTH_FILENAME = "PMMABulkAbsorb.dat";
    G4String FIBER_FIRST_CLADDING_REFRACTION_INDEX_FILENAME = "refraction_index_pmma.csv";
    G4String FIBER_SECOND_CLADDING_REFRACTION_INDEX_FILENAME = "refraction_index_fluorinated_polymer.csv";
    //====================================================================================================
    G4String APD_QUANTUM_EFFICIENCY_FILENAME = "apd_qe_1.dat";
    G4String PMT_QUANTUM_EFFICIENCY_FILENAME = "qe_file_2.dat";
    G4String LIQUID_SCINTILLATOR_SENSITIVE_DETECTOR_NAME = "/NovaDet/liquidScintillatorSd";
    G4String PMT_SENSITIVE_DETECTOR_NAME = "/NovaDet/pmtSd";

    //get-function
    G4String getFilePath(G4String filename) {return INPUT_DIR + "/" + filename;}
    //set-functions
    void defineMaterials();
    void setMaterialProperties();
    void setGlassProperty();
    void setLiquidScintillatorProperty();
    void setFiberCoreProperty();
    void setPmmaProperty();
    void setFluorinatedPolymerProperty();
    void setGalacticProperty();
    void setPvcSurfaceProperty(G4LogicalVolume* pvcLogicalVolume, G4bool turnOffReflectivity = false);

    //Union Solid : Solid + Solid (pointer type)
    G4UnionSolid* makeCellSolid(G4double deltaSize, G4double length);
    G4UnionSolid* makePvcCellSolid();
    G4UnionSolid* makeLiquidScintillatorSolid();
    G4UnionSolid* makeFiberLoopSolid(G4double radius, G4double curveRadius, G4double straightLength);
   
    //Logical Volume (pointer type)
    G4LogicalVolume* makeWlsFiberLoopLogicalVolume(G4double straightLength);
    G4LogicalVolume* makeWlsFiberStraightLogicalVolume(G4double straightLength);
    G4LogicalVolume* makePmtLogicalVolume();
    
    //Physical Volume (pointer type)
    G4VPhysicalVolume* makeDetectorPhysicalVolume();
    G4VPhysicalVolume* makeNovaCellPhysicalVolume();
    G4VPhysicalVolume* makeSingleWlsFiberPhysicalVolume();
    G4VPhysicalVolume* makeBenchtopNovaCellPhysicalVolume();
    G4VPhysicalVolume* makeElectronGunPhysicalVolume();
    
    G4bool isUpdated;//isUpdated = true/false, It used to be a variable of functions. So, it can be a "switch" to open/close the update
    G4Box* experimentalHallSolid;//Box  (pointer type)
    G4LogicalVolume* experimentalHallLogicalVolume;//Logical Volume  (pointer type)
    G4VPhysicalVolume* experimentalHallPhysicalVolume;//Physical Volume  (pointer type)
    G4NistManager* nistManager;//It used as "nistManager = G4NistManager::Instance();" , "H = nistManager->FindOrBuildElement("H");"

    //edit
    G4Box* TPBSolid;//Box  (pointer type)
    G4LogicalVolume* TPBLogicalVolume;//Logical Volume  (pointer type)
    G4VPhysicalVolume* TPBPhysicalVolume;//Physical Volume  (pointer type)


    //Element (pointer type)
    G4Element* H;
    G4Element* C;
    G4Element* O;
    G4Element* Ti;

    //Material (pointer type)
    G4Material* tiO2;
    G4Material* polystyrene;
    G4Material* pvc;
    G4Material* liquidScintillator;
    G4Material* fiberCore;
    G4Material* pmma;
    G4Material* fluorinatedPolymer;
    G4Material* glass;
    G4Material* aluminum;
    G4Material* galactic;

    // for Mpt(the Volume which has physical constant ex. Refractive Index, WLS time constant. 
    // #include "G4MaterialPropertiesTable.hh" (pointer type)
    G4MaterialPropertiesTable* liquidScintillatorMpt;
    G4MaterialPropertiesTable* fiberCoreMpt;
    G4MaterialPropertiesTable* pmmaMpt;
    G4MaterialPropertiesTable* fluorinatedPolymerMpt;

    //Declare variables
    G4double liquidScintillatorLightYield;
    G4double straightWidth;
    G4double straightHeight;
    G4double detectorLength;
    G4double snoutLength;
    G4double fiberCurveToEndPlateDistance;
    G4double pvcThickness;
    G4double innerCellCornerRadius;
    G4double fiberRadius;
    G4double fiberCladdingFraction;
    G4double fiberCurveRadius;
    G4double pmtThickness;
    G4String photodetectorType;
    G4String simulationMode;
};

#endif
