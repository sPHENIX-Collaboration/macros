//
// Do a recalibration from the saved histograms
//
#include <TSpectrum.h>
#include "get_runstr.h"
#include <mbd/MbdCalib.h>


#if defined(__CLING__)
//R__LOAD_LIBRARY(libmbd_io.so)
R__LOAD_LIBRARY(libmbd.so)
#endif

//using namespace MBDRUNS;

Double_t qmin = 25.;
Double_t qmax = 1600;
TF1 *bkgf[128]{nullptr};

double minrej = 2000.;
double peak = 2000.;
double maxrej = 5000.;
Double_t powerlaw(Double_t *x, Double_t *par)
{
  Float_t xx =x[0];
  if ( xx>minrej && xx<maxrej )
  {
    TF1::RejectPoint();
    return 0;
  }
  Double_t f = par[0]*pow((par[1]/(par[1]+xx)),par[2]); 
  return f;
}


// Two gaussians
// [0] = ampl, peak 1
// [1] = mean
// [2] = sigma
// [2] = ampl, peak 2
Double_t gaus2(Double_t *x, Double_t *par)
{
  Double_t xx =x[0];
  Double_t f = par[0]*TMath::Gaus(xx,par[1],par[2]) + par[3]*TMath::Gaus(xx,2.0*par[1],sqrt(2)*par[2]); 
  return f;
}

Double_t landau2(Double_t *x, Double_t *par)
{
  Double_t xx =x[0];
  Double_t f = par[0]*TMath::Landau(xx,par[1],par[2]) + par[3]*TMath::Landau(xx,2.0*par[1],par[4]); 
  return f;
}

Double_t langaufun(Double_t *x, Double_t *par)
{
  //Fit parameters:
  //par[0]=Width (scale) parameter of Landau density
  //par[1]=Most Probable (MP, location) parameter of Landau density
  //par[2]=Total area (integral -inf to inf, normalization constant)
  //par[3]=Width (sigma) of convoluted Gaussian function
  //
  //In the Landau distribution (represented by the CERNLIB approximation),
  //the maximum is located at x=-0.22278298 with the location parameter=0.
  //This shift is corrected within this function, so that the actual
  //maximum is identical to the MP parameter.

  // Numeric constants
  Double_t invsq2pi = 0.3989422804014;   // (2 pi)^(-1/2)
  Double_t mpshift  = -0.22278298;       // Landau maximum location

  // Control constants
  Double_t np = 100.0;      // number of convolution steps
  Double_t sc =   5.0;      // convolution extends to +-sc Gaussian sigmas

  // Variables
  Double_t xx;
  Double_t mpc;
  Double_t fland;
  Double_t sum = 0.0;
  Double_t xlow,xupp;
  Double_t step;
  Double_t i;


  // MP shift correction
  mpc = par[1] - mpshift * par[0];

  // Range of convolution integral
  xlow = x[0] - sc * par[3];
  xupp = x[0] + sc * par[3];

  step = (xupp-xlow) / np;

  // Convolution integral of Landau and Gaussian by sum
  for(i=1.0; i<=np/2; i++) {
    xx = xlow + (i-.5) * step;
    fland = TMath::Landau(xx,mpc,par[0]) / par[0];
    sum += fland * TMath::Gaus(x[0],xx,par[3]);

    xx = xupp - (i-.5) * step;
    fland = TMath::Landau(xx,mpc,par[0]) / par[0];
    sum += fland * TMath::Gaus(x[0],xx,par[3]);
  }

  return (par[2] * step * sum * invsq2pi / par[3]);
}


// find the threshold
void FindThreshold(TH1 *h, double& threshold)
{
  double absolute_min = 10.;
  int bin = h->FindBin( absolute_min );
  int maxbin = h->FindBin( qmax );
  //cout << bin << "\t" << maxbin << endl;

  // now look for peak after first min
  double prev_val = 0.;
  int ibin = bin;
  while ( ibin<=maxbin )
  {
    double val = h->GetBinContent( ibin );
    //cout << val << endl;
    if ( val<=0. )
    {
      prev_val = val;
      ibin++;
      continue;
    }
    else if ( val>0. && prev_val>0. )
    {
      double ratio = val/prev_val;
      //cout << ibin << "\t" << ratio << endl;
      if ( ratio<1.05 )
      {
        threshold = h->GetBinCenter( ibin );
        break;
      }
    }

    prev_val = val;
    ibin++;
  }

}

// xmin and xmax are the min and max range of the peak
void FindPeakRange(TH1 *h, double& xmin, double& peak, double& xmax)
{
  int bin = h->FindBin( qmin );
  int maxbin = h->FindBin( qmax );
  double ymin = 1e12; // the minimum y val
  int nabove = 0;     // num points above the min

  // look for 1st min
  int ibin = bin;
  while ( ibin<=maxbin )
  {
    double val = h->GetBinContent( ibin );
    if ( val < ymin )
    {
      ymin = val;
      nabove = 0; // new min, reset nabove
    }
    else
    {
      nabove++;
    }

    // if we see this many above the min, the signal is rising
    if ( nabove==20 )
    {
      xmin = h->GetBinCenter( ibin-20 );
      break;
    }

    ibin++;
  }

  // now look for peak after first min
  double ymax = 0; // the minimum y val
  int nbelow = 0;     // num points below the max
  while ( ibin<=maxbin )
  {
    double val = h->GetBinContent( ibin );
    if ( val > ymax )
    {
      ymax = val;
      nbelow = 0; // new max, reset nbelow
    }
    else
    {
      nbelow++;
    }

    // if we see this many below the max, the signal is falling
    if ( nbelow==20 )
    {
      peak = h->GetBinCenter( ibin-20 );
      break;
    }

    ibin++;
  }

  xmax = peak + 4*(peak - xmin);

}


// type0: auau200
// type1: pp200
// type
void recal_mbd_mip(const char *tfname = "DST_MBDUNCAL-00020869-0000.root", const int pass = 3, const int nevt = 0, const int type = 0)
{
  cout << "tfname " << tfname << endl;

  const int NUM_PMT = 128;
  //const int NUM_PMT = 12;
  const int NUM_ARMS = 2;

  // Create new TFile
  TString dir = "results/";
  dir += get_runnumber(tfname);
  dir += "/";
  TString name = "mkdir -p "; name += dir;
  gSystem->Exec( name );

  name = dir; name += "calmbdq_pass3.root";

  // Read in TFile with h_q
  TFile *oldfile = new TFile(name,"READ");

  TH1 *h_q[NUM_PMT];
  TH1 *h_tq[NUM_PMT];

  TString title;
  for (int ipmt=0; ipmt<NUM_PMT; ipmt++)
  {
    name = "h_q"; name += ipmt;
    title = "q"; title += ipmt;
    //h_q[ipmt] = new TH1F(name,title,15100/4,-100,15000);
    h_q[ipmt] = (TH1*)oldfile->Get(name);
    if ( type == MBDRUNS::PP200 )
    {
      //h_q[ipmt]->Rebin(4);  // b-off
      h_q[ipmt]->Rebin(2);
    }

    name = "h_tq"; name += ipmt;
    title = "tq"; title += ipmt;
    //h_tq[ipmt] = new TH1F(name,title,7000,-150,31*17.76);
    h_tq[ipmt] = (TH1*)oldfile->Get(name);
  }
  //TH2 *h2_tq = new TH2F("h2_tq","ch vs tq",900,-150,150,NUM_PMT,-0.5,NUM_PMT-0.5);
  TH2 *h2_tq = (TH2*)oldfile->Get("h2_tq");

  name = dir; 
  name += "recalmbd_pass"; name += pass; name += ".root";
  cout << name << endl;

  TFile *savefile = new TFile(name,"RECREATE");

  // Load in calib constants

  TCanvas *ac[100];
  int cvindex = 0;

  //q
  ac[cvindex] = new TCanvas("cal_q","q",425*1.5,550*1.5);
  if ( pass>0 )
  {
    ac[cvindex]->Divide(1,2);
    ac[cvindex]->cd(1);
  }

  ofstream cal_mip_file;
  TString calfname;
  if ( pass==3 ) 
  {
    calfname = dir; calfname += "mbd_qfit.calib";
    cal_mip_file.open( calfname );
    std::cout << "Writing to " << calfname << std::endl;
  }

  // Fit ranges
  // Run23AuAu
  qmin = 25.;
  qmax = 1600;

  if ( type==MBDRUNS::PP200 )
  {
    //qmin = 200;
    //qmax = 10000;       // Run24pp boff
    //qmin = 100;
    qmin = 50;
    qmax = 2000;       // Run24pp bon
  }

  if ( type==MBDRUNS::SIMAUAU200 || type==MBDRUNS::SIMPP200 )
  {
    qmin = 0.25;
    qmax = 6.;
  }

  TF1 *mipfit[128];

  TH1 *h_bkg[NUM_PMT];  // background histogram
  TH1 *h_mip[NUM_PMT];  // mip signal histogram
  TH1 *h_bkgmip[NUM_PMT];  // bkg + fit histogram


  for (int ipmt=0; ipmt<NUM_PMT; ipmt++)
  {

    if (pass>0)
    {
      h_bkg[ipmt] = (TH1*)h_q[ipmt]->Clone();
      name = h_q[ipmt]->GetName(); name.ReplaceAll("q","bkg");
      h_bkg[ipmt]->SetName( name );
      h_bkg[ipmt]->SetTitle( name );
      h_bkg[ipmt]->SetLineColor(2);
      double threshold = qmin + 2.;
      FindThreshold( h_q[ipmt], threshold );
      cout << "threshold " << ipmt << "\t" << threshold << endl;
      h_q[ipmt]->GetXaxis()->SetRangeUser( threshold, qmax );
      h_q[ipmt]->Sumw2();

      double sigma = 20;
      double seedmean = 0;
      TSpectrum s{};
      if (type==MBDRUNS::PP200)
      {
        h_bkg[ipmt] = s.Background( h_q[ipmt] );

        h_mip[ipmt] = (TH1*)h_q[ipmt]->Clone();
        name = h_q[ipmt]->GetName(); name.ReplaceAll("q","mip");
        h_mip[ipmt]->SetName( name );
        h_mip[ipmt]->SetTitle( name );
        h_mip[ipmt]->Add( h_bkg[ipmt], -1.0 );

        /*
           Int_t nfound = s.Search(h_q[ipmt],sigma,"",0.1);
           Double_t *xpeaks = s.GetPositionX();

        //h_bkg[ipmt] = s.Background( h_q[ipmt] );

        double best_peak = xpeaks[0];
        if ( best_peak < 50. )
        {
        best_peak = xpeaks[1];
        }

        cout << "peaks\t" << ipmt << "\t" << nfound << "\t" << best_peak << endl;
        */

      }
      else if (type==MBDRUNS::AUAU200)
      {

        FindPeakRange( h_bkg[ipmt], minrej, peak, maxrej );
        cout << "peak range\t" << minrej << "\t" << peak << "\t" << maxrej << endl;
        sigma = peak-minrej;
        seedmean = peak;

        // Fit background
        name = "bkgf"; name += ipmt;
        bkgf[ipmt] = new TF1(name,powerlaw,qmin,qmax,3);
        bkgf[ipmt]->SetParameter(0,240e-5);
        bkgf[ipmt]->SetParameter(1,1240);
        bkgf[ipmt]->SetParameter(2,2);
        bkgf[ipmt]->SetLineColor(3);
        h_bkg[ipmt]->Draw();
        //h_bkg[ipmt]->Fit(bkgf[ipmt],"MR");
        h_bkg[ipmt]->Fit(bkgf[ipmt],"R");

        // get mip histogram
        h_mip[ipmt] = (TH1*)h_q[ipmt]->Clone();
        name = h_q[ipmt]->GetName(); name.ReplaceAll("q","mip");
        h_mip[ipmt]->SetName( name );
        h_mip[ipmt]->SetTitle( name );

        double orig_minrej = minrej;
        double orig_maxrej = maxrej;
        minrej = 0.;
        maxrej = 0.;
        h_mip[ipmt]->Add( bkgf[ipmt], -1.0 );
      }

      // Fit the mip peak after background subtraction
      name = "mipfit"; name += ipmt;
      /*
      // Laudau fit
      mipfit[ipmt] = new TF1(name,"landau",qmin,qmax);
      mipfit[ipmt]->SetLineColor(4);
      mipfit[ipmt]->SetParameter( 0, 5.0*h_mip[ipmt]->GetMaximum() );
      Double_t seedmean = h_mip[ipmt]->GetBinCenter( h_mip[ipmt]->GetMaximumBin() );
      cout << "SEEDMEAN " << seedmean << endl;
      mipfit[ipmt]->SetParameter( 1, seedmean );
      mipfit[ipmt]->SetParameter( 2, 20. );
      */
      // Two gaussian fit
      mipfit[ipmt] = new TF1(name,gaus2,qmin,qmax,4);

      //mipfit[ipmt] = new TF1(name,langaufun,qmin,qmax,4);

      seedmean = h_mip[ipmt]->GetBinCenter( h_mip[ipmt]->GetMaximumBin() );
      cout << "SEEDMEAN " << seedmean << endl;
      Double_t seedsigma = sigma;
      if ( type==MBDRUNS::SIMAUAU200 )
      {
        seedsigma = 0.2;
      }
      else if ( type==MBDRUNS::PP200 )
      {
        seedsigma = seedmean*(116./719.);  // b-on
        //seedsigma = 50;  // b-on
        //seedsigma = 200;  // b-off
      }

      mipfit[ipmt]->SetLineColor(4);
      // for langaus
      /*
         mipfit[ipmt]->SetParameter( 0, seedsigma );
         mipfit[ipmt]->SetParameter( 1, seedmean );
         mipfit[ipmt]->SetParameter( 2, mipfit[ipmt]->GetParameter(0)*0.1 );
         mipfit[ipmt]->SetParameter( 3, 0.12*seedsigma );
         */
      // for gaus2
      mipfit[ipmt]->SetParameter( 0, 5.0*h_mip[ipmt]->GetMaximum() );
      mipfit[ipmt]->SetParameter( 1, seedmean );
      mipfit[ipmt]->SetParameter( 2, seedsigma );
      mipfit[ipmt]->SetParameter( 3, mipfit[ipmt]->GetParameter(0)*0.1 );
      //mipfit[ipmt]->SetParameter( 4, mipfit[ipmt]->GetParameter(1) );

      h_mip[ipmt]->Fit( mipfit[ipmt], "RM" );

      double integ = mipfit[ipmt]->GetParameter(0);
      double best_peak = mipfit[ipmt]->GetParameter(1);
      double width = mipfit[ipmt]->GetParameter(2);
      double integerr = mipfit[ipmt]->GetParError(0);
      double best_peakerr = mipfit[ipmt]->GetParError(1);
      double widtherr = mipfit[ipmt]->GetParError(2);
      double chi2 = mipfit[ipmt]->GetChisquare();
      double ndf = mipfit[ipmt]->GetNDF();

      cal_mip_file << ipmt << "\t" << integ << "\t" << best_peak << "\t" << width << "\t"
        << integerr << "\t" << best_peakerr << "\t" << widtherr << "\t"
        << chi2/ndf << endl;
      cout << ipmt << "\t" << integ << "\t" << best_peak << "\t" << width << "\t"
        << integerr << "\t" << best_peakerr << "\t" << widtherr << "\t"
        << chi2/ndf << endl;

      // Get full fit
      h_bkgmip[ipmt] = (TH1*)h_bkg[ipmt]->Clone();
      name = h_q[ipmt]->GetName(); name.ReplaceAll("q","bkgmip");
      h_bkgmip[ipmt]->SetName( name );
      h_bkgmip[ipmt]->SetTitle( name );
      h_bkgmip[ipmt]->Add( mipfit[ipmt] );
      h_bkgmip[ipmt]->SetLineColor( 8 );

      /*
         gPad->Modified();
         gPad->Update();
      //mipfit->SetRange( xpeaks[0]-2.5*sigma, 600 );
      mipfit->SetRange( qmin, 600 );
      mipfit->SetParameter( 0, 1000 );
      mipfit->SetParameter( 1, best_peak );
      mipfit->SetParameter( 2, sigma );

      mipfit->SetParameter( 8, f_expo->GetParameter(0) );
      mipfit->SetParameter( 9, f_expo->GetParameter(1) );
      h_q[ipmt]->Fit( mipfit, "R" );
      */


      /*
         int npar = f_bkg->GetNpar();
         for (int ipar=0; ipar<npar; ipar++)
         {
         f_bkg->SetParameter( ipar, mipfit->GetParameter(ipar+3) );
         }
         */

      // Now draw the full dist, plus fit
      ac[cvindex]->cd(1);
      gPad->SetLogy(1);
      h_q[ipmt]->GetXaxis()->SetRangeUser( qmin, qmax );
      //h_q[ipmt]->SetMinimum(10.);
      h_q[ipmt]->Draw();
      h_bkg[ipmt]->Draw("same");
      h_bkgmip[ipmt]->Draw("same");

      gPad->Modified();
      gPad->Update();

      ac[cvindex]->cd(2);
      h_mip[ipmt]->Draw();
      gPad->Modified();
      gPad->Update();

      /*
         string junk;
         cout << "? ";
         cin >> junk;
         */

      name = dir + "h_qfit"; name += ipmt; name += ".png";
      cout << name << endl;
      ac[cvindex]->Print( name );
    }

  }
  ++cvindex;

  if ( pass==3 )
  {
    cal_mip_file.close();

    // Convert to CDB format
    MbdCalib mcal;
    mcal.Download_Gains( calfname.Data() );
    calfname.ReplaceAll(".calib",".root");
    mcal.Write_CDB_Gains( calfname.Data() );
  }

  savefile->Write();
  savefile->Close();
}

