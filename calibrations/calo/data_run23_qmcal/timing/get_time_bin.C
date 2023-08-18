//#include "/phenix/scratch/jfrantz/install/sphenix/include/litecaloeval/LiteCaloEval.h"

//R__LOAD_LIBRARY(libLiteCaloEvalTowSlope.so)

void get_time_bin(const char * infile, const int runno){
  
  //  LiteCaloEval modlce;

  //reflce.CaloType(LiteCaloEval::CEMC);
  //  modlce.CaloType(LiteCaloEval::CEMC);

  /// we really dont need to retrieve histos here bc we arent using ref object
  /// reflce.Get_Histos(reffile);

  //  modlce.Get_Histos(infile,modfile);


  TFile * mf = new TFile(infile);
  
  TH2F * corrPati = new TH2F("t0corrPati","",96,0,96,258,0,258);
  TH2F * corrPati2 = new TH2F("t0corrPati2","",96,0,96,258,0,258);
  TH1F * chi2_hist1d_log = new TH1F("t0_hist1d_log","",10000, -5,5); 
  TH1F * chi2_hist1d = new TH1F("t0_hist1d","",340, -1.5,32.5); 
  //  TH2F * corrPati = new TH2F("corrPati","",24,0,24,64,0,64);
  //  TF1 * ffn = 0;
  
  
  for (int jj = 0; jj < 96; jj++) {
    for (int ii = 0; ii < 256; ii++) {
      
	      TString a;
	      a.Form("%d",jj);
	      
	      TString b;
	      b.Form("%d",ii);
	      
	      TString histname = "wvfm_time_eta-bin_" + a + "_phi-bin_" + b;
	      
	      TH1F *tmp = (TH1F *)gDirectory->Get(histname.Data());
	      
	      //	      TF1 *ffn = (TF1 *)tmp->GetFunction("myexpo");
	      
	      float t0 = -1.0;

	      if (tmp->GetEntries() == 0) 
		continue;
	      

	      float t0chk = tmp->GetBinCenter(tmp->GetMaximumBin());

	      if (t0chk < 3.5 ||  t0chk > 12.5)
		continue;
		  
	      float at0 = tmp->GetBinContent(tmp->GetMaximumBin());
	      float at0up = tmp->GetBinContent(tmp->GetMaximumBin()+1);
	      float at0down = tmp->GetBinContent(tmp->GetMaximumBin()-1);
	      float sumt = 0;
	      int cntr = 0; 
	      
	      for (int jt = 1; jt < 32; jt++)
		{
		  if ( fabs( tmp->GetMaximumBin() -  1.0*jt) > 1.1)
		    {
		      sumt += tmp->GetBinContent(jt);
		      if (tmp->GetBinContent(jt) > 0 )
			cntr++;
		    }
		}
	      
	      
	      if (cntr < 1)
		{
		  std::cout << "weird dist " << histname.Data() << std::endl;
		}
	      else if (sumt/cntr/at0 > 0.75) //1st chk: S/B ~1/.75 on max bin 
		{
		  t0 = -0.75; 
		}
	      else  
		{
		  t0 = t0chk;

		  if (sumt/cntr/at0up < 0.75 && sumt/cntr/at0down > 0.75)
		    t0 = t0chk + 0.5;
		  else if (sumt/cntr/at0down < 0.75 && sumt/cntr/at0up > 0.75)
		    t0 = t0chk - 0.5;
		  else if (sumt/cntr/at0down < 0.75 && sumt/cntr/at0up < 0.75)
		    {
		      if (at0down < at0up && sumt/cntr/at0up < 0.7)
			t0 = t0chk + 0.5;
		      else if (at0down > at0up && sumt/cntr/at0down < 0.7) 
			t0 = t0chk - 0.5;
		    }
		}
	      
	      
      if (t0 > 0 ||  t0 < 0)
	{
	  chi2_hist1d->Fill(t0);
	  corrPati->SetBinContent(jj+1,ii+1,t0);
	}
      
      
      
      
      
    } // iphi
    
  } //ieta
  
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
	      /*
      if (ffn)
	{
	  float chi2dof = ffn->GetChisquare();
	  chi2dof /= ffn->GetNDF();
	  chi2_hist1d_log->Fill(log(chi2dof));
	  chi2_hist1d->Fill(chi2dof);
	  corrPati->SetBinContent(jj+1,ii+1,chi2dof);
	}

//	above was in inside above loop
	      */

  TString ofnm = "timecutOhioU_";
  ofnm +=  runno;
  ofnm += ".root";
  
  TFile mfo(ofnm, "RECREATE");
  chi2_hist1d->Write();
  corrPati->Write();

  chi2_hist1d_log->Draw();
  TCanvas * c3 = new TCanvas();
  
  chi2_hist1d->Draw();
  TCanvas * c2 = new TCanvas();
  corrPati->Draw("colz");

  mfo.Close();

}
