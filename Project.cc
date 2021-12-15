#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include "Project1DetectorConstruction.hh"
#include "Project3PhysicsList.hh"
#include "Project2PrimaryGeneratorAction.hh"
#include "Project2EventAction.hh"
#include "Project2StackingAction.hh"
#include "Project2SteppingAction.hh"
#include "Project2TrackingAction.hh"
#include "Project2RunAction.hh"
#include "Project2RecorderBase.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"


int main(int argc, char** argv)//to get argc, argv
{
  Project2RecorderBase* recorder = 0;//pointer but there is no =&variable, which adress the pointer points?  
  //recoder is the member function of the class from "Project2RunAction in Project2RunAction.cc" including run,event,track, and step record
  G4RunManager* runManager = new G4RunManager;//mandatory toolkit(1) : runManager
  //pointer runManager for class GRRunManager
  runManager->SetUserInitialization(new Project1DetectorConstruction);//runManager(1)->Initialize(DetectorConstruct) //instance arrow?
  runManager->SetUserInitialization(new Project3PhysicsList);//runManager(1)->Initialize(PhysList)
  runManager->SetUserAction(new Project2PrimaryGeneratorAction(""));//runManager(1)->Action(Primary)
  runManager->SetUserAction(new Project2StackingAction);//runManager(1)->Action(Stacking)
  runManager->SetUserAction(new Project2RunAction(recorder));//runManager(1)->Action(Run)
  runManager->SetUserAction(new Project2EventAction(recorder));//runManager(1)->Action(Event)
  runManager->SetUserAction(new Project2TrackingAction(recorder));//runManager(1)->Action(Tracking)
  runManager->SetUserAction(new Project2SteppingAction(recorder));//runManager(1)->Action(Stepping)
  //access to the member functions in the class G4RunManger via pointer, and execute the function with the variable (new XX)

  G4UImanager* uiManager = G4UImanager::GetUIpointer();//mandatory toolkit(2) : uiManager
  //pointer uiManager for the member function GetUIpointer in class G4UImanager
  G4VisManager* visManager = new G4VisExecutive;//psudo-mandatory toolki(3) : visManager
  //pointer visManager for class G4VisExecutvie
  visManager->Initialize();//visManager(3)->Initialize
  //access to the member functions in the class G4VisExecutive via pointer, and execute the function

  //=======================================================================================================================================================
  
  if (argc == 1) {//when ./nova
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
    //pointer ui for the class G4UIExecutive
    uiManager->ApplyCommand("/control/execute vis.mac");//uiManager(2)->command
    uiManager->ApplyCommand("/control/execute gui.mac");//uiManager(2)->command
    uiManager->ApplyCommand("/geant4e/limits/stepLength 0.001 mm");//edit@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    ui->SessionStart();//ui(2) study
    delete ui;//ui(2)->delete. save memory for dynamic allocated class G4UIExecutive
  }

  else {//when ./nova xxx.mac
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
    G4String filename = argv[1]; //mac
    //G4String randomSeed = argv[2]; //random seed
    //uiManager->ApplyCommand("/random/setSeeds " + randomSeed + " 1");//uiManager(2)->command. ?? Yet, I cannot use it.
    uiManager->ApplyCommand("/control/execute vis.mac");//uiManager(2)->command
    uiManager->ApplyCommand("/control/execute gui.mac");//uiManager(2)->command
    uiManager->ApplyCommand("/control/execute " + filename);//uiManager(2)->command ?? Yet, I cannot use it.
    ui->SessionStart();//ui(2) study
    delete ui;//ui(2)->delete. save memory for dynamic allocated class G4UIExecutive
  }


  //============================================================================================================================================================
  
  delete visManager;//visManager(3)->delete //save memory for dynamic allocated class G4VisExecutive
  delete runManager;//runManager(1)->delete //save memory for dynamic allocated class G4RunManager
  return 0;//reuturn vlaue of this function. when succeed 0, fail 1
}
