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
//
/// \file EventAction.cc
/// \brief Implementation of the EventAction class

#include "EventAction.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4ios.hh"
#include "SensitiveDetectorHit.hh"
#include "HistoManager.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction()
: G4UserEventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event*)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* event){
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    G4VHitsCollection* hc = event->GetHCofThisEvent()->GetHC(0); //there is only one hit collection defined, thus the 0


    if(hc){
      G4int eventID = event->GetEventID();
      G4int nHits = hc->GetSize(); //number of hits stored in event
      G4double totalEdep = 0;
      G4double EdepNI = 0;
      G4double trackLength = 0;
      G4ThreeVector pos;
      G4ThreeVector pos_start;
      G4ThreeVector pos_last;
      G4double kinE;
      G4double deltaE;

      for(G4int i=0; i<nHits; i++){
        auto hit = static_cast<SensitiveDetectorHit*>(hc->GetHit(i));
        deltaE = hit->GetEdep()*1000; //go to keV
        totalEdep += deltaE; 
        EdepNI += hit->GetEdepNI()*1000;
        trackLength += hit->GetStepLength();
        pos = hit->GetPos();

        analysisManager->FillNtupleDColumn(1, 0, pos[0]);
        analysisManager->FillNtupleDColumn(1, 1, pos[1]);
        analysisManager->FillNtupleDColumn(1, 2, pos[2]);
        analysisManager->FillNtupleDColumn(1, 3, deltaE);
        analysisManager->AddNtupleRow(1);

        if (i==0){
          pos_start = hit->GetPos();
          kinE = hit->GetKinE()*1000; 
        }
        if (i==(nHits-1)){
          pos_last = hit->GetPos();
        }

      }
      
      if(totalEdep>0){
        analysisManager->FillH1(4, totalEdep); 
        analysisManager->FillH1(5, pos_start[0]);
        analysisManager->FillH1(6, pos_start[1]);
        analysisManager->FillH1(7, pos_start[2]);
        analysisManager->FillH1(8, trackLength);

        analysisManager->FillNtupleIColumn(0, eventID);
        analysisManager->FillNtupleDColumn(1, pos_start[0]);
        analysisManager->FillNtupleDColumn(2, pos_start[1]);
        analysisManager->FillNtupleDColumn(3, pos_start[2]);
        analysisManager->FillNtupleDColumn(4, totalEdep);
        analysisManager->FillNtupleDColumn(5, trackLength); 
        analysisManager->FillNtupleDColumn(6, EdepNI);
        analysisManager->FillNtupleDColumn(7, kinE); 
        analysisManager->FillNtupleDColumn(8, pos_last[0]);
        analysisManager->FillNtupleDColumn(9, pos_last[1]);
        analysisManager->FillNtupleDColumn(10, pos_last[2]);
        analysisManager->AddNtupleRow();

        //G4cout << "total deposited energy: " << totalEdep*1000 << " keV" << G4endl;
      }
    }

}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
