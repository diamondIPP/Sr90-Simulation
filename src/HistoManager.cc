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
/// \file HistoManager.cc
/// \brief Implementation of the HistoManager class
//
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "HistoManager.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::HistoManager() : fFileName("output")
{

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  G4cout << "HistoManager: " << analysisManager->GetType() << G4endl;
  Book();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::~HistoManager()
{
  delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Book()
{
  // Create or get analysis manager
  // The choice of analysis technology is done via selection of a namespace
  // in HistoManager.hh
  G4cout << "Booking histograms.." << G4endl;
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetFileName(fFileName);
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetActivation(true);
  
  // Define histograms start values
  const G4int kMaxHisto = 10;
  const G4String id[] = {"ePrimE","ePrimX","ePrimY","ePrimZ","EdepDia","DHitX","DHitY","DHitZ","trackLen","9"};
  const G4String title[] = 
          { "Primary energy distribution e- (MeV)",     //0
            "Primary x-position distribution e- (mm)",  //1
            "Primary y-position distribution e- (mm)",  //2
            "Primary z-position distribution e- (mm)",  //3
            "Deposited energy in diamond",              //4
            "x-hit position diamond (mm)",              //5
            "y-hit position diamond (mm)",              //6
            "z-hit position diamond (mm)",              //7
            "Track Length (mm)",                        //8
            "dummy",  //
            "dummy",  //
          };

  dataType.push_back("Energy"); //0
  dataType.push_back("Length"); //1
  dataType.push_back("Length"); //2
  dataType.push_back("Length"); //3
  dataType.push_back("Energy"); //4
  dataType.push_back("Length"); //5
  dataType.push_back("Energy"); //6
  dataType.push_back("Length"); //7
  dataType.push_back("Length"); //8
  dataType.push_back("Length"); //9

  // Default values (to be reset via /analysis/h1/set command)               
  G4int nbins = 100;
  G4double vmin = 0.;
  G4double vmax = 100.;

  // Create all histograms as inactivated 
  // as we have not yet set nbins, vmin, vmax
  for (G4int k=0; k<kMaxHisto; k++) {
    G4cout << k << "\t" << title[k] << G4endl;
    G4int ih = analysisManager->CreateH1(id[k], title[k], nbins, vmin, vmax);
    analysisManager->SetH1Activation(ih, false);
  }



}

void HistoManager::printStats()
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  for (unsigned int i = 0; i < dataType.size(); ++i) {
    if (analysisManager->GetH1(i, false, true) != 0) {
      G4cout << i << "\t"
          << analysisManager->GetH1Title(i) <<", 1 dim, "<< analysisManager->GetH1(i)->entries()<< " entries:\n\t\t mean = "
          << analysisManager->GetH1(i)->mean() << " "<< analysisManager->GetH1XAxisTitle(i)<<" rms = "
          << analysisManager->GetH1(i)->rms() << " " << analysisManager->GetH1XAxisTitle(i)<<G4endl;
    } else if (analysisManager->GetH2(i, false, true) != 0) {
      G4cout << i << "\t" << analysisManager->GetH2Title(i) << ", 2 dim, " << analysisManager->GetH2(i)->entries() << G4endl;
    }
    else
    G4cout << "Hist with index "<< i << " does not exist." <<G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
