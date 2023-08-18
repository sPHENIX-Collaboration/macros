void mergetsfits(const char * outfile)
{
  
  TFile * mf =  new TFile(outfile,"RECREATE");

  int min_bin = 9;

  TH2F * corrPatty = 0;


  for (int n = min_bin; n < 95; n++)
    {
      TString modfile = "tsGainFit"; 
      //      modfile += 
      modfile += n;
      modfile += ".root";

      std::cout << modfile.Data() << std::endl;
      
      TFile * tf = new TFile(modfile.Data());
      
      TString b = "eta_";
      b += n;
      
      TH1F * hh = (TH1F * ) tf->Get(b.Data());
      hh->SetDirectory(mf);
      
      if (n == 30)
	{
	  b = "eta_96";
	  hh = (TH1F * ) tf->Get(b.Data());
	  hh->SetDirectory(mf);
	}
      
      int max_iphi = 256;
      /// start of phi loop
      for (int j = 0; j < max_iphi; j++)
	{
	  /// create string to hold name of individual tower
	  std::string hist_name_p = "emc_ieta" + std::to_string(n) + "_phi" + std::to_string(j);
	  
	  hh = (TH1F * ) tf->Get(hist_name_p.c_str());
	  hh->SetDirectory(mf);
	  
	}

      if (n ==  min_bin)
	{
	  corrPatty = (TH2F *) tf->Get("corrPat");
	  corrPatty->SetDirectory(mf);
	}
      else
	{
	  TH2F * h2temp = (TH2F *) tf->Get("corrPat");
	  corrPatty->Add(h2temp);
	}
      
    }

  std::cout << "gathered all hists now gathering chi2" << std::endl;
  
  mf->cd();

  
  TH2F * corrPati = new TH2F("chi2corrPati","",96,0,96,258,0,258);
  TH1F * chi2_hist1d_log = new TH1F("chi2_hist1d_log","",10000, -5,5); 
  TH1F * chi2_hist1d = new TH1F("chi2_hist1d","",10000, 1e-1,1e3); 
  //  TH2F * corrPati = new TH2F("corrPati","",24,0,24,64,0,64);
  //  TF1 * ffn = 0;


  for (int jj = 0; jj < 96; jj++) {

    if (jj%10 == 0)
      std::cout << " eta  " << jj << std::endl;

    for (int ii = 0; ii < 256; ii++) {

	      TString a;
	      a.Form("%d",jj);
	 
	      TString b;
	      b.Form("%d",ii);

	      TString histname = "emc_ieta" + a + "_phi" + b;

	      TH1F *tmp = (TH1F *)gDirectory->Get(histname.Data());

	      TF1 *ffn = 0;  

	      if (tmp)
		ffn = (TF1 *)tmp->GetFunction("myexpo");


	      /*
	      //fill 1d histo
	      chi->Fill(fn->GetChisquare()/fn->GetNDF());

	      //fill 2d histo
	      chiPD->SetMinimum(0.5);
	      chiPD->SetMaximum(1.0);
	      chiPD->SetBinContent(i+1,j+1,fn->GetChisquare()/fn->GetNDF());
	      */


	      /*

      if (modlce.cemc_hist_eta_phi[ii][jj] == NULL) continue;
      ffn = 0;
      cout << "i,j,hist" << ii << " " << jj << " " << modlce.cemc_hist_eta_phi[ii][jj] << endl;
      TH1 * h1 = modlce.cemc_hist_eta_phi[ii][jj]; 
      ffn =   h1->GetFunction("myexpo");
      cout << "called " << ffn  << endl;
      
	      */
      if (ffn)
	{
	  float chi2dof = ffn->GetChisquare();
	  chi2dof /= ffn->GetNDF();
	  chi2_hist1d_log->Fill(log(chi2dof));
	  chi2_hist1d->Fill(chi2dof);
	  corrPati->SetBinContent(jj+1,ii+1,chi2dof);
	}
      

    }
  }


  std::cout << "now writing out file ... " << std::endl;

  /*
  chi2_hist1d_log->Draw();
  TCanvas * c3 = new TCanvas();
  
  chi2_hist1d->Draw();
  TCanvas * c2 = new TCanvas();
  corrPati->Draw("colz");
  corrPati->Write();

  */

  mf->Write();
  mf->Close();



  
}
