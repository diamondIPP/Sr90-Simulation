{

  //ePrimE, ePrimX, ePrimY, ePrimZ, EdepDia, DHitX, DHitY, DHitZ
  gROOT->Reset();  
  // Draw histos filled by Geant4 simulation 

  TFile f("output21M.root");  
  
  TCanvas* c1 = new TCanvas("c1", "  ");
  c1->cd();
  TH1 *ePrimE;
  f->GetObject("Histograms/ePrimE", ePrimE);
  ePrimE->Draw();

  TCanvas* c2 = new TCanvas("c2", "  ");
  c2->cd();
  TH1 *EdepDia;
  f->GetObject("Histograms/EdepDia", EdepDia);
  EdepDia->SetLineColor(kRed); 
  EdepDia->SetLineWidth(2);
  EdepDia->Draw("HIST");

  int xmin = 0;
  int xmax = 30;
  TAxis *axis = EdepDia->GetXaxis();
  int bmin = axis->FindBin(xmin);
  int bmax = axis->FindBin(xmax);
  double integral = EdepDia->Integral(bmin,bmax);
  integral -= EdepDia->GetBinContent(bmin)*(xmin-axis->GetBinLowEdge(bmin))/axis->GetBinWidth(bmin);
  integral -= EdepDia->GetBinContent(bmax)*(axis->GetBinUpEdge((bmax)-xmax))/axis->GetBinWidth(bmax);
  cout << "Integral (sum of deposited energy): " << integral << "keV" << endl; 

}  
