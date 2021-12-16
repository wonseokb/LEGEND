1. G4_setup&command.sh 
   1) G4 environment set up to run a simulation.
   2) useful commands for simulation
   
2. [input]
   1) data files connected to /include/DetectorConstruction.hh
  
3. [include]
   1) (almost) only DetectorConstruction.hh is needed to edited
   
4. [build]
   1) only project, vis.mac, gui.mac is essential
   2) run without macro file "./Project"
   3) run with macro file "./Project xxx.mac"
   4) when I try "cmake .." on this directory, it makes "CMakeCache.txt" "cmake_install.cmake" "Makefile" [CMakeFils] [input]
      here, we need 
   5) there are some more macro files
   
5.  [src]StackingAction.cc
   1) DetectorConstruction.cc
   2) EventAction.cc -> data to root file and text file.
   3) RunAction.cc -> header of root file data.
   4) StackingAction.cc -> incrementPhotonCountScintillation/Cherenkov. Track information
   5) SteppingAction.cc -> Step information
   
6.  [src_backup]
   1) old src files
