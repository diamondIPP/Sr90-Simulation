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
/// \file PrimaryGeneratorAction.cc
/// \brief Implementation of the PrimaryGeneratorAction class
//
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#include "PrimaryGeneratorAction.hh"
#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4Geantino.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "HistoManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction(): G4VUserPrimaryGeneratorAction(),
  gps(0){

  G4int noParticles = 1;
  gps = new G4GeneralParticleSource();
  gps->SetNumberOfParticles(noParticles); 
  G4ParticleDefinition* particleDefinition = G4ParticleTable::GetParticleTable()->FindParticle("e-");
  gps->SetParticleDefinition(particleDefinition);      
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete gps;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent){
    
  //create vertex
  //G4double u, v;
 // G4double x0;
  //G4double y0 = 5.68;
  //G4double z0;

  //all this does is selecting a random point on the circle where the Sr90 is
  //u = (1.57/2.) * (1.57/2.) * G4UniformRand();
  //v = 2 * M_PI * G4UniformRand();
  //x0 = std::sqrt(u) * std::cos(v);
  //z0 = std::sqrt(u) * std::sin(v);
  //G4cout << "Set Position: " << x0 << ", " << y0 << ", " << z0 << G4endl;
  //gps->SetParticlePosition(G4ThreeVector(x0,y0,z0));

  gps->GeneratePrimaryVertex(anEvent);

  // store primary energy distribution and position
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillH1(0, gps->GetParticleEnergy());
  G4ThreeVector pos = gps->GetParticlePosition();

  //G4cout << "Position: " << pos[0] << ", " << pos[1] << ", " << pos[2] << G4endl;
  //G4cout << "Beta energy: " << gps->GetParticleEnergy()/CLHEP::keV << " keV" << G4endl;
  analysisManager->FillH1(1, pos[0]);   // x0-pos hist
  analysisManager->FillH1(2, pos[1]);   // y0-pos hist
  analysisManager->FillH1(3, pos[2]);   // z0-pos hist

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
