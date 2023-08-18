//#include "/phenix/scratch/jfrantz/install/sphenix/include/litecaloeval/LiteCaloEval.h"

//R__LOAD_LIBRARY(libLiteCaloEvalTowSlope.so)

void get_chi2_map2(char * infile, char * modfile = " "){
  
  //  LiteCaloEval modlce;

  //reflce.CaloType(LiteCaloEval::CEMC);
  //  modlce.CaloType(LiteCaloEval::CEMC);

  /// we really dont need to retrieve histos here bc we arent using ref object
  /// reflce.Get_Histos(reffile);

  //  modlce.Get_Histos(infile,modfile);


  //  TFile * mf = new TFile(infile,"UPDATE");

  TFile * mf = new TFile(infile);

  TH2F * corrPati = new TH2F("chi2corrPati","",96,0,96,258,0,258);
  TH1F * chi2_hist1d_log = new TH1F("chi2_hist1d_log","",100000, 0,100000); 
  TH1F * chi2_hist1d = new TH1F("chi2_hist1d","",5000, 1e-1,5e1); 
  //  TH2F * corrPati = new TH2F("corrPati","",24,0,24,64,0,64);
  //  TF1 * ffn = 0;


  for (int jj = 0; jj < 96; jj++) {
    for (int ii = 0; ii < 256; ii++) {

	      TString a;
	      a.Form("%d",jj);
	 
	      TString b;
	      b.Form("%d",ii);

	      TString histname = "emc_ieta" + a + "_phi" + b;

	      TH1F *tmp = (TH1F *)gDirectory->Get(histname.Data());

	      if (!tmp) continue;
	      
	      tmp->Rebin(20);

	      TF1 *ffn = (TF1 *)tmp->GetFunction("myexpo");

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
	      
	      float hiEint =  0;
	      
	      for (int ibx = tmp->FindBin(6.0); ibx < tmp->GetNbinsX()+1; ibx++)
		{
		  hiEint += tmp->GetBinContent(ibx);
		}
	      
	      chi2_hist1d_log->Fill(hiEint);
	      

      if (ffn)
	{
	  float chi2dof = ffn->GetChisquare();
	  chi2dof /= ffn->GetNDF();
	  chi2_hist1d->Fill(chi2dof);
	  corrPati->SetBinContent(jj+1,ii+1,chi2dof);
	}


    }
  }

  chi2_hist1d_log->Draw();
  TCanvas * c3 = new TCanvas();
  
  chi2_hist1d->Draw();
  TCanvas * c2 = new TCanvas();
  corrPati->Draw("colz");
  //  corrPati->Write();
  //mf->Close();

}
