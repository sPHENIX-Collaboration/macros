void viewEtaFits(char * fitfile){

  TFile * cal_output = new TFile(fitfile);
  TH1F * eta_hist[96];


  TCanvas * c1[4];
  TString c1nm[4] = {"1st_4th","2nd_4th","3rd_4th","4th_4th"};
  for (int i = 0; i< 4; i++)
    {
      

      c1[i] = new TCanvas(c1nm[i].Data(),c1nm[i].Data(),1200,800);
      c1[i]->Divide(8,3);

	  //	  gPad->SetLogy(1);
	  
      for (int jj = 0; jj < 24; jj++)
	{ 
				       
	  c1[i]->cd(jj+1);	  
	  int ieta = i*24+jj;

	  // getting eta towers
	  TString a;
	  a.Form("%d", ieta);
	  TString b = "eta_" + a;
	  TH1F *heta_temp = (TH1F *)cal_output->Get(b.Data());
	  eta_hist[ieta] = heta_temp;
	  if (eta_hist[ieta])
	    {
	      eta_hist[ieta]->GetXaxis()->SetRangeUser(0.01,0.5);
	      eta_hist[ieta]->Draw();
	    }
	}
    }
  
  TCanvas * c2kj = new TCanvas("p1canv","p1 canv",500,600);
  TGraph * tgr1 = (TGraph *) cal_output->Get("eta_p1");
  tgr1->Draw("AP");
  
}
