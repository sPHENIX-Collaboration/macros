void sumEtaHists(const char * fitfile){

  TFile * cal_output = new TFile(fitfile);
  TH1F * eta_hist[96];

  TCanvas * c1[4];
  TString c1nm[4] = {"Sumeta","2nd_4th","3rd_4th","4th_4th"};

  int cntr = 0; 
  TH1F * h1st = 0;
  for (int i = 0; i< 1; i++)
    {
      

      c1[i] = new TCanvas(c1nm[i].Data(),c1nm[i].Data(),900,700);
      //      c1[i]->Divide(8,3);

	  //	  gPad->SetLogy(1);
	  
      for (int jj = 13; jj < 95; jj++)
	{ 
				       
	  //	  c1[i]->cd(jj+1);	  
	  int ieta = jj;

	  std::cout << "summing " << ieta << std::endl;
	  if (jj == 40)
	    continue;
	  else if (jj == 38)
	    continue;
	  else if (jj == 31)
	    continue;
	  else if (jj >8 && jj < 26 && jj !=  20 && jj != 13 && jj != 15 && jj != 12)
	    continue;
	  else if (jj == 31)
	    continue;
	  else if (jj == 69)
	    continue;
	  else if (jj > 37 && jj < 41)
	    continue;
	  else if (jj > 73 && jj < 87 && jj != 80 && jj !=  82 && jj != 84)
	    continue;
	  else if (jj == 89)
	    continue;
	  else if (jj > 92)
	    continue;

	  std::cout << "yes " << ieta << std::endl;


	  // getting eta towers
	  TString a;
	  a.Form("%d", ieta);
	  TString b = "eta_" + a;
	  TH1F *heta_temp = (TH1F *) cal_output->Get(b.Data());
	  eta_hist[ieta] = heta_temp;
	  if (cntr == 0)
	    {
	      h1st = heta_temp;
	      h1st->SetName("SumEtaHist");
	    }
	  else 
	    h1st->Add(heta_temp);
	  //	  eta_hist[ieta]->Draw();
	  
	  cntr++;
	  
	}
    }

  h1st->Draw();

}


