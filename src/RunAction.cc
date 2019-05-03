#include "RunAction.hh"
#include "HistoManager.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"


RunAction::RunAction() :
    G4UserRunAction(), fHistoManager(0)
{
  // set printing event number per each event
  G4RunManager::GetRunManager()->SetPrintProgress(10000);

  // Book predefined histograms
  fHistoManager = new HistoManager();

  //   Creating detector hit ntuple
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  G4cout << "Using " << analysisManager->GetType() << G4endl;

  // Create directories
  analysisManager->SetHistoDirectoryName("Histograms");
  analysisManager->SetNtupleDirectoryName("EventData");

  analysisManager->SetNtupleMerging(true);
  //analysisManager->SetFirstNtupleId(1);
  // Detector hit tuple (every sensor interaction creates a hit)
  analysisManager->CreateNtuple("DetectorHits", "HitCollection");  // ID 0
  analysisManager->CreateNtupleIColumn("Event");
  analysisManager->CreateNtupleDColumn("HitX");
  analysisManager->CreateNtupleDColumn("HitY");
  analysisManager->CreateNtupleDColumn("HitZ");
  analysisManager->CreateNtupleDColumn("TotalEnergyDeposit");
  analysisManager->CreateNtupleDColumn("TrackLength");
  analysisManager->CreateNtupleDColumn("EnergyDepositNI"); //non-ionizing energy deposit
  analysisManager->CreateNtupleDColumn("KinE"); //kinetic energy of particle
  analysisManager->CreateNtupleDColumn("HitX_last");
  analysisManager->CreateNtupleDColumn("HitY_last");
  analysisManager->CreateNtupleDColumn("HitZ_last");
  analysisManager->FinishNtuple();


  analysisManager->CreateNtuple("Tracking", "TrackingCollection");  // ID 0
  analysisManager->CreateNtupleDColumn(1, "x");
  analysisManager->CreateNtupleDColumn(1, "y");
  analysisManager->CreateNtupleDColumn(1, "z");
  analysisManager->CreateNtupleDColumn(1, "edelta");
  analysisManager->FinishNtuple(1);


}

RunAction::~RunAction()
{
  delete fHistoManager;
}

void RunAction::BeginOfRunAction(const G4Run* /*run*/)
{
  //inform the runManager to save random number seed
  //G4RunManager::GetRunManager()->SetRandomNumberStore(true);

  //histograms
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  if (analysisManager->IsActive()) {
    analysisManager->OpenFile();
  }
}

void RunAction::EndOfRunAction(const G4Run* /*run*/)
{
  // print histogram statistics
  //fHistoManager->printStats();

  // save histograms
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  if (analysisManager->IsActive()) {
    analysisManager->Write();
    analysisManager->CloseFile();
  }
}

