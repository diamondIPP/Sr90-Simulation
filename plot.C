{

  //ePrimE, ePrimX, ePrimY, ePrimZ, EdepDia, DHitX, DHitY, DHitZ
  //Event, HitX, HitY, HitZ, TotalEnergyDeposit, TrackLength
  //gROOT->Reset();  
  // Draw histos filled by Geant4 simulation 

  TFile f("output.root");  
  TDirectory *dir = f.GetDirectory("EventData");
  dir.cd();
  TTree *tree = (TTree*) dir->Get("DetectorHits");
  cout << tree->GetEntries() << " Events in Data" << endl;

  //Draw total deposited energy
  TCanvas* c1 = new TCanvas("c1", "  ");
  c1->cd();
  TH1F *EdepDia = new TH1F("EdepDia", "Energy Deposit of Betas", 100, 0, 2000);
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


  //get detailed tracking information
  TTree *tree = (TTree*) dir->Get("Tracking");
  TCanvas* c4 = new TCanvas("c4", "  ");
  c4->cd();
  c4->SetLogy();
  c4->SetLogz();
  TH2F *edepZ = new TH2F("edepZ", "", 150,-0.02,0.555, 1600,0.3,40);
  edepZ->GetXaxis()->SetTitle("Depth [mm]");
  edepZ->GetYaxis()->SetTitle("Ionizing Energy Deposit [keV]");
  edepZ->SetStats(false);
  tree->Draw("edelta:(-1.0*(y+30.268))>>edepZ", "", "colz");


  TCanvas* c5 = new TCanvas("c5", "  ");
  c5->cd();
  TH1D *proj = edepZ->ProjectionX("proj");
  proj->GetXaxis()->SetTitle("Depth [mm]");
  proj->GetYaxis()->SetTitle("Ionizing Energy Deposit [keV]");
  proj->SetStats(false);
  proj->Draw();


  TCanvas* c6 = new TCanvas("c6", "  ");
  c6->cd();
  TH1D *proj2 = edepZ->ProjectionX("proj2");
  proj2->GetXaxis()->SetTitle("Depth [mm]");
  proj2->GetYaxis()->SetTitle("Number of generated eh-pairs");
  proj2->Scale(1000.0/13.0);
  proj2->SetStats(false);
  proj2->Draw();



  /*
  double edeposited = 0;
  for (int i=0; i<tree->GetEntries(); i++){
    tree->GetEntry(i);
    edeposited += tree.GetLeaf("TotalEnergyDeposit").GetValue();
  }
  cout << "Deposited Energy: " << edeposited << endl;
  */

}  
