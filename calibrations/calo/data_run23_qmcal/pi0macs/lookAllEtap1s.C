{

  TCanvas * c1 = new TCanvas("fits_eta","",1200,900);
  c1->Divide(4,2);
 
  TString fitFnm[8] = 
    {
      "Yfcpi1_fit1_21598.root",  //0
      "Yfcpi1_fit1_21615.root",  //1
      "Yfcpi1_fit1_21796.root", // 2
      "Yfcpi1_fit1_21813.root",  // 3
      "Yfcpi1_fit1_21889.root",  //4
      "Yfcpi1_fit1_22949.root", // 5
      "Yfcpi1_fit1_22979.root", //6
      "Yfcpi1_fit1_21518.root",  //7

    };
  
  

  for (int i = 0; i<8; i++)
    {

      TString ak = "hist_";
      ak += i;

      TFile * mf = new TFile(fitFnm[i].Data());
      TGraph * etagr = (TGraph *) mf->Get("eta_p1");

      TH1F * h1 = new TH1F(ak.Data(),fitFnm[i].Data(),100, -2.5, 98.5);
      h1->SetMinimum(0.090);
      h1->SetMaximum(0.250);
      c1->cd(i+1);
      h1->Draw();

      etagr->Draw("P");


    }



}
