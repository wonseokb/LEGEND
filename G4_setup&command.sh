# To Run Geant4 Simulation on my Linux machine
export G4NEUTRONHPDATA=/usr/local/share/Geant4-10.5.1/data/G4NDL4.5; export G4LEDATA=/usr/local/share/Geant4-10.5.1/data/G4EMLOW7.7; export G4LEVELGAMMADATA=/usr/local/share/Geant4-10.5.1/data/PhotonEvaporation5.3; export G4RADIOACTIVEDATA=/usr/local/share/Geant4-10.5.1/data/RadioactiveDecay5.3; export G4PARTICLEXSDATA=/usr/local/share/Geant4-10.5.1/data/G4PARTICLEXS1.1; export G4PIIDATA=/usr/local/share/Geant4-10.5.1/data/G4PII1.3; export G4REALSURFACEDATA=/usr/local/share/Geant4-10.5.1/data/RealSurface2.1.1; export G4SAIDXSDATA=/usr/local/share/Geant4-10.5.1/data/G4SAIDDATA2.0; export G4ABLADATA=/usr/local/share/Geant4-10.5.1/data/G4ABLA3.1; export G4INCLDATA=/usr/local/share/Geant4-10.5.1/data/G4INCL1.0; export G4ENSDFSTATEDATA=/usr/local/share/Geant4-10.5.1/data/G4ENSDFSTATE2.2
=========================================================================================================
# Refresh Viewrer
/vis/viewer/refresh
# Verbose
/tracking/verbose 2
# View
/vis/open OGL 1200x1000-0+0
/vis/viewer/zoom 10
/vis/viewer/pan -50 0 cm (Move the viewer)
/vis/viewer/pan 0 0.4 cm
/vis/viewer/set/viewpointThetaPhi 90. 0.
/vis/viewer/set/viewpointThetaPhi 0. 90.
/vis/viewer/set/viewpointThetaPhi 9. 90.
/vis/viewer/set/viewpointThetaPhi 45. 30.
# Axis
/vis/scene/add/axes 0 0 0 5 cm
=========================================================================================================
#1. Test plate model on the center
/gps/particle opticalphoton
/gps/ene/mono 9.6875 eV (128nm)
/gps/direction 0 -1 0
/gps/pos/centre 0. 1. 0. cm
=========================================================================================================
#2. Shoot the beam isotropically
/gps/particle opticalphoton
/gps/ene/mono 9.6875 eV (128nm)
/gps/pos/centre 0. 0. 0. m
/gps/ang/type iso
/gps/ang/rot1 1 0 0
/gps/ang/rot2 0 0 1
=========================================================================================================
#3. Shoot the beam in a random plane
/gps/particle opticalphoton
/gps/ene/mono 9.6875 eV
/gps/pos/type Plane
/gps/pos/shape Rectangle
/gps/pos/halfx 5.14 cm
/gps/pos/halfy 0.15 cm
=========================================================================================================
#4. Electron Gun
/vis/viewer/set/viewpointThetaPhi 80. 30.
/gps/particle e-
/gps/ene/mono 0.6 MeV
/gps/ene/mono 2.826 MeV (Sr90 Q value)
/gps/direction 0 0 1
/gps/pos/centre 0. 0. -15. cm
/run/beamOn 10000
=========================================================================================================
#5. beam angle
/gps/direction 0 -1 0.0875 (5 degrees)
/gps/direction 0 -1 0.1763 (10 degrees)
/gps/direction 0 -1 0.2679 (15 degrees)
/gps/direction 0 -1 0.3640 (20 degrees)
/gps/direction 0 -1 0.4663 (25 degrees)
/gps/direction 0 -1 0.5774 (30 degrees)
/gps/direction 0 -1 0.7002 (35 degrees)
/gps/direction 0 -1 0.8391 (40 degrees)
/gps/direction 0 -1 1.0000 (45 degrees)
/gps/direction 0 -1 1.1918 (50 degrees)
/gps/direction 0 -1 1.4281 (55 degrees)
/gps/direction 0 -1 1.7321 (60 degrees)
/gps/direction 0 -1 1.8040 (61 degrees)
/gps/direction 0 -1 1.8897 (62 degrees)
/gps/direction 0 -1 1.9626 (63 degrees)
/gps/direction 0 -1 2.0503 (64 degrees)
/gps/direction 0 -1 2.1445 (65 degrees)
/gps/direction 0 -1 2.2460 (66 degrees)
/gps/direction 0 -1 2.3559 (67 degrees)
/gps/direction 0 -1 2.4751 (68 degrees)
/gps/direction 0 -1 2.6051 (69 degrees)
/gps/direction 0 -1 2.7475 (70 degrees)
/gps/direction 0 -1 2.9042 (71 degrees)
/gps/direction 0 -1 3.0777 (72 degrees)
/gps/direction 0 -1 3.2709 (73 degrees)
/gps/direction 0 -1 3.4874 (74 degrees)
/gps/direction 0 -1 3.7322 (75 degrees)
/gps/direction 0 -1 4.0108 (76 degrees)
/gps/direction 0 -1 4.3314 (77 degrees)
/gps/direction 0 -1 4.7046 (78 degrees)
/gps/direction 0 -1 5.1446 (79 degrees)
/gps/direction 0 -1 5.6713 (80 degrees)
/gps/direction 0 -1 6.3138 (81 degrees)
/gps/direction 0 -1 7.1154 (82 degress)
/gps/direction 0 -1 8.1443 (83 degress)  
/gps/direction 0 -1 9.5146 (84 degrees)
/gps/direction 0 -1 11.430 (85 degrees)
/gps/direction 0 -1 14.301 (86 degrees)
/gps/direction 0 -1 19.081 (87 degress)
/gps/direction 0 -1 28.636 (88 degress)  
/gps/direction 0 -1 57.290 (89 degrees)
==========================================================================================================
#6. Nova Simulation "Cell Mode"
Fiber - magenta
LiquidScintillator - blue
pvc - green
pmt - red
FrontEnd - gray
BackEnd - gray
Glass(pmt) - Red
PhotoCathode - Yellow
/gps/particle e-
/gps/ene/mono 2 MeV
/gps/direction 0 -1 0
/gps/pos/centre 0. 15. 0. cm
/gps/direction 0 0 -1
/gps/pos/centre 0. 0. 150. cm
/vis/viewer/set/viewpointThetaPhi 20. 40.
==========================================================================================================
#7. Nova Simulation "Fiber Mode"
outer cladding - magenta
inner cladding - red
core - blue
/gps/particle opticalphoton
/gps/ene/mono 2.95 MeV
/gps/direction 0 -1 0
/gps/pos/centre 0. 15. 0. cm
==========================================================================================================
#8. Nova Simulation "CellWater Mode"
/gps/particle e-
/gps/ene/mono 10 MeV
/gps/direction 0 0 1
/gps/pos/centre 0. 0. -2.3 cm  -2.38
==========================================================================================================
#9. OpNovice
OpNovice: Chrenkov + Scintillation + Mie scattering in water box
OpNovice2: OpNovice + data collecting
