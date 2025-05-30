#include <filesystem>

#include <TFile.h>
#include <TSpectrum.h>
#include <TH2.h>
#include <TH1.h>
#include <TF1.h>

#include "make_cdbtree.C"

TSpectrum *tspec{nullptr};
TF1 *FitHitTime{nullptr};

void FitGausToPeak(TH1 *h1)
{
  float rangemin;
  float rangemax;
  int npeak = tspec->Search(h1, 5, "goff",0.2);  // finds the highest peak, draws marker


  //std::cout << "NPEAKS " << npeak << std::endl;
  if (npeak < 3)                                       
  {
    h1->Fit(FitHitTime,"QN0L");
    rangemin = FitHitTime->GetParameter(1) - 0.6*FitHitTime->GetParameter(2);
    rangemax = FitHitTime->GetParameter(1) + 0.6*FitHitTime->GetParameter(2);
  }
  else
  {
    double *peakpos = tspec->GetPositionX();
    float centerpeak = peakpos[0];
    float sidepeak[2];
    if (peakpos[2] > peakpos[1])
    {
      sidepeak[0] = peakpos[1];
      sidepeak[1] = peakpos[2];
    }
    else
    {
      sidepeak[1] = peakpos[1];
      sidepeak[0] = peakpos[2];
    }
    rangemin = centerpeak - (centerpeak - sidepeak[0]) / 2.;
    rangemax = centerpeak + (sidepeak[1] - centerpeak) / 2.;
  }

  FitHitTime->SetRange(rangemin, rangemax);
  h1->Fit("FitHitTime", "QRL");

}

void calib_t0mean(const char *fname = "results/48700/calmbdpass2.3_q_48700.root")
{
  double gmean[2];
  double gmeanerr[2];
  double gsigma[2];
  double gsigmaerr[2];
  double hmean[2];
  double hmeanerr[2];
  double hrms[2];
  double hrmserr[2];

  TFile *tfile = new TFile(fname,"READ");
  TH2 *h2_tt = (TH2F*)tfile->Get("h2_tt");

  TH1 *h_ttarm[2]{nullptr};
  h_ttarm[0] = h2_tt->ProjectionX("h_ttarm0",1,64);
  h_ttarm[1] = h2_tt->ProjectionX("h_ttarm1",65,128);

  // Get Histogram results
  for (int iarm=0; iarm<2; iarm++)
  {
    //h_ttarm[iarm]->ResetStats();

    hmean[iarm] = h_ttarm[iarm]->GetMean();
    hmeanerr[iarm] = h_ttarm[iarm]->GetMeanError();
    hrms[iarm] = h_ttarm[iarm]->GetRMS();
    hrms[iarm] = h_ttarm[iarm]->GetRMSError();
  }

  // Now do gaus fit to central peak
  tspec = new TSpectrum(5);  // 5 peaks is enough - we have 4
  FitHitTime = new TF1("FitHitTime","gaus",-5,5);

  TCanvas *ac = new TCanvas("ac","hittime",1200,600);
  ac->Divide(2,1);
  for (int iarm=0; iarm<2; iarm++)
  {
    ac->cd(iarm+1);
    FitGausToPeak( h_ttarm[iarm] );
    h_ttarm[iarm]->Draw();
    gmean[iarm] = FitHitTime->GetParameter(1);
    gmeanerr[iarm] = FitHitTime->GetParError(1);
    gsigma[iarm] = FitHitTime->GetParameter(2);
    gsigmaerr[iarm] = FitHitTime->GetParError(2);
  }

  TString pdfname = fname;
  pdfname.ReplaceAll("_q-","_t0mean-");
  pdfname.ReplaceAll(".root",".pdf");
  ac->Print( pdfname );

  // Get Mean of two peaks
  float t0corr_mean = (gmean[0]+gmean[1])/2.0;
  float t0corr_meanerr = sqrt(gmeanerr[0]*gmeanerr[0]+gmeanerr[1]*gmeanerr[1])/2.0;

  // Save results to calib files
  filesystem::path dir(fname);
  //cout << dir.parent_path() << endl;
  std::string calibfname = dir.parent_path();
  calibfname += "/mbd_t0corr.calib";
  cout << "Saving calibs to " << calibfname << endl;

  ofstream calibsavefile( calibfname );
  calibsavefile << t0corr_mean << "\t" << t0corr_meanerr << endl;
  calibsavefile << gmean[0] << "\t" << gmeanerr[0] << "\t" << gsigma[0] << "\t" << gsigmaerr[0]
        << "\t" << gmean[1] << "\t" << gmeanerr[1] << "\t" << gsigma[1] << "\t" << gsigmaerr[1] << endl;
  calibsavefile << hmean[0] << "\t" << hmeanerr[0] << "\t" << hrms[0] << "\t" << hrmserr[0]
        << "\t" << hmean[1] << "\t" << hmeanerr[1] << "\t" << hrms[1] << "\t" << hrmserr[1] << endl;
  calibsavefile.close();
  make_cdbtree( calibfname.c_str() );
}
