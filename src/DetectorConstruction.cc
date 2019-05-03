//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class
//
// $Id: DetectorConstruction.cc 98257 2016-07-04 17:39:46Z gcosmo $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorConstruction.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4SubtractionSolid.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "SensitiveDetector.hh"
#include "G4SDManager.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction():G4VUserDetectorConstruction(){
    fWorldSize = 20*cm;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction(){}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct(){

    int n_collimators = 1;

    G4Material* Air = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");

    //World
    G4Box* solidWorld = new G4Box("World",fWorldSize/2,fWorldSize/2,fWorldSize/2);
    G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld,    //its solid
                                                      Air,                    //its material
                                                      "World");               //its name
    G4VPhysicalVolume* physiWorld = new G4PVPlacement(0,                      //no rotation
                                                      G4ThreeVector(),        //at (0,0,0)
                                                      logicWorld,             //its logical volume
                                                      "World",                //its name
                                                      0,                      //its mother  volume
                                                      false,                  //no boolean operation
                                                      0);                     //copy number

    logicWorld -> SetVisAttributes(G4VisAttributes::Invisible);
    //Sr-90 source definition
    G4double a, z;
    G4double density;
    G4int    ncomponents;
    G4bool checkOverlaps = true;

    //define brass
    G4Material* Cu = new G4Material("Copper" , z=29., a = 63.550*g/mole, density = 8.96*g/cm3);
    G4Material* Zn = new G4Material("Zinc"   , z=30., a = 65.409*g/mole, density = 7.14*g/cm3);
    G4Material* Brass= new G4Material("Brass", density= 8.5*g/cm3, ncomponents=2);
    Brass->AddMaterial(Cu  , 70*perCent);
    Brass->AddMaterial(Zn  , 30*perCent);


    //build Sr-90 geometry
    //*************************************************************
    G4double container_x = 14;
    G4double container_y = 50;
    G4double container_z = 14;

    G4Box *containerBox = new G4Box("Container Box",container_x/2.,container_y/2.,container_z/2.);
    G4RotationMatrix* rotationMatrix = new G4RotationMatrix();
    rotationMatrix->rotateX(90.*deg);


    //big hole (diam: 6.7, depth: 4mm)
    G4VSolid* hole1 =
            new G4Tubs("hole1",
                       0.,                           // inner radius
                       6.7/2.,                       // outer radius
                       4,                            // height
                       0.*deg,                       // rotation
                       360.*deg);
    G4ThreeVector translation1(0, -container_y/2.,      0);
    G4SubtractionSolid *boxplushole1 = new G4SubtractionSolid("Box+Hole1", containerBox, hole1, rotationMatrix, translation1);

    //small hole where source sits (diam: 1.55, depth: 1.68mm)
    G4VSolid* hole2 =
            new G4Tubs("hole2",
                       0.,                           // inner radius
                       1.57/2.,                       // outer radius
                       1.68,                            // height
                       0.*deg,                       // rotation
                       360.*deg);
    G4ThreeVector translation2(0, -container_y/2. + 4 , 0);
    G4SubtractionSolid *boxplushole2 = new G4SubtractionSolid("Box+Hole2",boxplushole1, hole2, rotationMatrix, translation2);
    //*************************************************************


    //give Sr-90 source a material and place it
    G4LogicalVolume* logicContainer = new G4LogicalVolume(boxplushole2,Brass,"Container");
    new G4PVPlacement(0,
                      G4ThreeVector(0, container_y/2., 0),
                      logicContainer,
                      "Container",
                      logicWorld,
                      false,
                      0,
                      checkOverlaps);

    //create collimator and place it
    if(n_collimators >= 1) {
        G4Box *collimator1Box =
                new G4Box("Collimator 1 Box",                //name
                          container_x/2.,
                          20./2.,
                          container_z/2.);

        G4VSolid* holecol =
                new G4Tubs("hole1",
                           0.,                           // inner radius
                           6.7/2.,                       // outer radius
                           20.,                            // height
                           0.*deg,                       // rotation
                           360.*deg);

        G4SubtractionSolid *collimator1plushole = new G4SubtractionSolid("Collimator 1 + Hole1",
                                                                         collimator1Box,
                                                                         holecol,
                                                                         rotationMatrix,
                                                                         G4ThreeVector(0,0,0));

        G4LogicalVolume* logicCollimator1 =
                new G4LogicalVolume(collimator1plushole,
                                    Brass,
                                    "Collimator 1");
        logicCollimator1->SetVisAttributes(G4VisAttributes(G4Colour(0.99,0.78,0.05)));

        new G4PVPlacement(0,
                          G4ThreeVector(0, - 20./2., 0),
                          logicCollimator1,
                          "Collimator 1",
                          logicWorld,
                          false,
                          0,
                          checkOverlaps);
    } // end if(n_collimators >= 1)

    //for second collimator
    if(n_collimators >= 2) {
        G4Box *collimator2Box =
                new G4Box("Collimator 2 Box",                //name
                          container_x/2.,
                          20./2.,
                          container_z/2.);

        G4VSolid* holecol2 =
                new G4Tubs("hole2",
                           0.,                           // inner radius
                           2./2.,                       // outer radius
                           20./2.,                  // height
                           0.*deg,                       // rotation
                           360.*deg);
        G4SubtractionSolid *collimator2plushole = new G4SubtractionSolid("Collimator 1 + Hole1",
                                                                         collimator2Box,
                                                                         holecol2,
                                                                         rotationMatrix,
                                                                         G4ThreeVector(0,0,0));

        G4LogicalVolume* logicCollimator2 =
                new G4LogicalVolume(collimator2plushole,
                                    Brass,
                                    "Collimator 2");
        logicCollimator2->SetVisAttributes(G4VisAttributes(G4Colour(0.99,0.78,0.05)));

        new G4PVPlacement(0,
                          G4ThreeVector(0, - 20 - 20./2., 0),
                          logicCollimator2,
                          "Collimator 2",
                          logicWorld,
                          false,
                          0,
                          checkOverlaps);
    } //end if(n_collimators >=2)

    /*

    //Sr-90 radioactive material volume
    G4VSolid* sr90_solid =
            new G4Tubs("sr90_solid",
                       0.,                           // inner radius
                       1.57/2.,                       // outer radius
                       0.05,                            // height
                       90.*deg,                       // rotation
                       360.*deg);
    
    

    G4Material* Sr = new G4Material("Stronitum", z=38., a = 87.62*g/mole, density = 2.63*g/cm3);
    G4LogicalVolume* sr90_logic =
                new G4LogicalVolume(sr90_solid,
                                    Sr,
                                    "sr90_logic");
    sr90_logic->SetVisAttributes(G4VisAttributes(G4Colour(1,0,0)));

    G4ThreeVector positionSr(0, 4+1.63 , 0);
    new G4PVPlacement(rotationMatrix,
                  positionSr,
                  sr90_logic,
                  "sr90_placed",
                  logicWorld,
                  false,
                  0,
                  checkOverlaps);

    */

    // Here comes the definition of the diamond detector
    
    //Define diamond
    G4double A = 12.01 * g/mole;
    G4int Z = 6;
    G4Material* diamond_material = new G4Material("diamond_material", Z, A, 3.515*g/cm3);

    //define geometry
    G4double dia_x = 4.5;
    G4double dia_y = 4.5;
    G4double dia_z = 0.54;

    //define distance quellen-unterseite bis diamant oberlfaeche (positive number)
    G4double distance = 10;

    G4VSolid* diamond_solid = new G4Box("diamond_solid", dia_x/2., dia_y/2., dia_z/2.);
    diamond_volume = new G4LogicalVolume(diamond_solid,diamond_material,"diamond_volume");
    diamond_volume->SetVisAttributes(G4VisAttributes(G4Colour(0.0,1.0,1.0)));

    G4double diamond_offset = -n_collimators*20. - dia_z - distance;
    new G4PVPlacement(rotationMatrix,
                      G4ThreeVector(0, diamond_offset, 0),
                      diamond_volume,
                      "diamond",
                      logicWorld,
                      false,
                      0, //this is the copy number (unique per detector)
                      checkOverlaps);


    //define electrode pad
    G4double Aau = 196.97 * g/mole;
    G4int Zau = 79;
    G4Material* gold = new G4Material("gold", Zau, Aau, 19.32*g/cm3);

    G4double diael_x = 4.0;
    G4double diael_y = 4.0;
    G4double diael_z = 0.0001; //100nm

    G4VSolid* diamond_electrode = new G4Box("diamond_electrode", diael_x/2., diael_y/2., diael_z/2.);
    G4LogicalVolume* diamond_electrode_volume = new G4LogicalVolume(diamond_electrode,gold,"diamond_electrode_volume");
    diamond_electrode_volume->SetVisAttributes(G4VisAttributes(G4Colour(1.0,0.0,0.0)));

    G4double diamond_electrode_offset_top = -n_collimators*20. - dia_z - distance + dia_z/2.0 + diael_z/2.0;
    new G4PVPlacement(rotationMatrix,
                      G4ThreeVector(0, diamond_electrode_offset_top, 0),
                      diamond_electrode_volume,
                      "diamond_electrode_top",
                      logicWorld,
                      false,
                      0, //this is the copy number (unique per detector)
                      checkOverlaps);

    G4double diamond_electrode_offset_bottom = -n_collimators*20. - dia_z - distance - dia_z/2.0 - diael_z/2.0;
    new G4PVPlacement(rotationMatrix,
                      G4ThreeVector(0, diamond_electrode_offset_bottom, 0),
                      diamond_electrode_volume,
                      "diamond_electrode_bottom",
                      logicWorld,
                      false,
                      0, //this is the copy number (unique per detector)
                      checkOverlaps);


    return physiWorld;
}


  void DetectorConstruction::ConstructSDandField()
  {
    G4String detector_name = "diamond_plate";
    SensitiveDetector* dia = new SensitiveDetector(detector_name, "DiamondHitsCollection");
    G4SDManager::GetSDMpointer()->AddNewDetector(dia);
    SetSensitiveDetector(diamond_volume, dia);
  }



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
