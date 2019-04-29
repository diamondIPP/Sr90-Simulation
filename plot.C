{

  //ePrimE, ePrimX, ePrimY, ePrimZ, EdepDia, DHitX, DHitY, DHitZ
  //Event, HitX, HitY, HitZ, TotalEnergyDeposit, TrackLength
  //gROOT->Reset();  
  // Draw histos filled by Geant4 simulation 

  TFile f("data.root");  
  TDirectory *dir = f.GetDirectory("EventData");
  dir.cd();
  TTree *tree = (TTree*) dir->Get("DetectorHits");
  cout << tree->GetEntries() << " Events in Data" << endl;

  //Draw total deposited energy
  TCanvas* c1 = new TCanvas("c1", "  ");
  c1->cd();
  TH1F *EdepDia = new TH1F("EdepDia", "Energy Deposit", 100, 0, 2000);
  EdepDia->GetXaxis()->SetTitle("E [keV]");
  EdepDia->SetLineColor(kRed); 
  EdepDia->SetLineWidth(2);
  tree->Draw("TotalEnergyDeposit>>EdepDia");

  //Calculate the integral of the deposited charge
  TH1F *integral = new TH1F("integral", "integral", 1, 0, 2);
  tree->Draw("1>>integral", "TotalEnergyDeposit", "goff");
  double edep = integral->GetBinContent(1);
  cout << "Integral (sum of deposited energy): " << edep << "keV" << endl; 
  cout << "Number of eh-pairs: " << edep*1000/13 << endl;

  //Draw the hitmap
  TCanvas* c2 = new TCanvas("c2", "  ");
  c2->cd();
  TH2F *hitmap = new TH2F("hitmap", "Hit Map", 120,-3,3, 120,-3,3);
  tree->Draw("HitX:HitZ>>hitmap", "", "colz");


  //Draw energy deposit vs track length
  TCanvas* c3 = new TCanvas("c3", "  ");
  c3->cd();
  TH2F *eVsl = new TH2F("eVsl", "Energy Deposit over Track Length", 100,0,3, 200,0,2000);
  eVsl->GetXaxis()->SetTitle("Track Length [mm]");
  eVsl->GetYaxis()->SetTitle("Total Energy Deposit [keV]");
  tree->Draw("TotalEnergyDeposit:TrackLength>>eVsl", "", "colz");


  /*
  double edeposited = 0;
  for (int i=0; i<tree->GetEntries(); i++){
    tree->GetEntry(i);
    edeposited += tree.GetLeaf("TotalEnergyDeposit").GetValue();
  }
  cout << "Deposited Energy: " << edeposited << endl;
  */

}  
