#ifndef MACRO_RECAL_MBD_MIP_C
#define MACRO_RECAL_MBD_MIP_C
//
// Do a recalibration of the mip from the saved histograms
//
#include "get_runstr.h"

#include <mbd/MbdCalib.h>

#include <TCanvas.h>
#include <TF1.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TMath.h>
#include <TPad.h>
#include <TSpectrum.h>
#include <TSystem.h>

#include <fstream>

R__LOAD_LIBRARY(libmbd.so)

//using namespace MBDRUNS;

Double_t qmin = 25.;
Double_t qmax = 1600;
TF1 *bkgf[128]{nullptr};

double minrej = 2000.;
double peak = 2000.;
double maxrej = 5000.;
Double_t powerlaw(Double_t *x, Double_t *par) //NOLINT(readability-non-const-parameter)
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
// [3] = ampl, peak 2
Double_t gaus2(Double_t *x, Double_t *par) //NOLINT(readability-non-const-parameter)
{
  Double_t xx =x[0];
  Double_t f = par[0]*TMath::Gaus(xx,par[1],par[2]) + par[3]*TMath::Gaus(xx,2.0*par[1],sqrt(2)*par[2]); 
  return f;
}

Double_t woodssaxon(Double_t *x, Double_t *par) //NOLINT(readability-non-const-parameter)
{
  Double_t xx =x[0];
  Double_t e = par[0]/(1.0+exp((xx-par[1])/par[2]));
  return e;
}

// Two gaussians + expo
// [0] = ampl, peak 1
// [1] = mean
// [2] = sigma
// [3] = ampl, peak 2
// [4] = ampl, woods-saxon
// [5] = a, woods-saxon 
// [6] = slope, expo
Double_t gaus2ws(Double_t *x, Double_t *par) //NOLINT(readability-non-const-parameter)
{
  Double_t xx =x[0];
  Double_t f = par[0]*TMath::Gaus(xx,par[1],par[2]) + par[3]*TMath::Gaus(xx,2.0*par[1],sqrt(2)*par[2]); 
  //Double_t e = par[4]*TMath::Exp(-par[5]*xx);
  Double_t e = par[4]/(1.0+exp((xx-par[5])/par[6]));
  return f + e;
}


Double_t landau2(Double_t *x, Double_t *par) //NOLINT(readability-non-const-parameter)
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
  Double_t xlow;
  Double_t xupp;
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
// may also want to consider getting threshold from turn-on curves
void FindThreshold(TH1 *h, double& threshold)
{
  threshold = 0.;
  double absolute_min = 10.;
  double absolute_max = 70.;    // max adc where the threshold could be
  int absminbin = h->FindBin( absolute_min );
  int absmaxbin = h->FindBin( absolute_max );
  double absmaxval = h->GetBinContent( h->GetMaximumBin() );
  int maxbin = 0;
  double maxval = 0.;
  double prev_val = 0.;
  int maxjumpbin = 0;
  double maxratio = 0.;

  for (int ibin=absminbin; ibin<=absmaxbin; ibin++)
  {
    double val = h->GetBinContent(ibin);
    if ( val > maxval )
    {
      maxval = val;
      maxbin = ibin;
    }

    if ( val>0. && prev_val>0. )
    {
      double ratio = val/prev_val;
      if ( val>(absmaxval*0.25) )
      {
        //std::cout << ibin << "\t" << ratio << std::endl;
        if ( ratio>maxratio )
        {
          maxratio = ratio;
          maxjumpbin = ibin;
        }
      }
    }

    prev_val = val;
  }

  if ( maxbin==absmaxbin )
  {
    // no exponential before mip peak, use max jump to find threshold
    // (Note: should raise gain for this channel)
    threshold = h->GetBinLowEdge( maxjumpbin );
  }
  else
  {
    // use max bin in threshold range
    threshold = h->GetBinLowEdge( maxbin );
  }
  //std::cout << threshold << std::endl;
}


// xmin and xmax are the min and max range of the peak
// This version uses TSpectrum to find the peaks
/*
void FindPeakRange(TH1 *h, double& xmin, double& peak, double& xmax, double threshold)
{
  int verbose = 1;    // Peak finder
  if ( verbose )
  {
    static TCanvas *ac = new TCanvas("cpeak","peakfinder",800,600);
    ac->cd();
    h->GetXaxis()->SetRangeUser( qmin, qmax );
    h->Draw();
  }

  const Int_t maxPeaks = 1;
  TSpectrum spectrum(maxPeaks);

  // sigma: minimum expected width of a peak in bins (e.g., sigma=2 means width ≈ 4 bins)
  Double_t sigma = 5;

  // Threshold = minimum relative height (0.1 = 10% of max)
  Int_t nPeaks = spectrum.Search(h, sigma, "", 0.1);

  std::cout << "Found " << nPeaks << " peaks:\n";

  // Get peak positions
  Double_t* peaksX = spectrum.GetPositionX();
  for (Int_t i = 0; i < nPeaks; ++i) {
    Double_t x = peaksX[i];
    Double_t y = h->GetBinContent(h->FindBin(x));
    std::cout << "  Peak at x = " << x << ", height = " << y << "\n";
  }

  if ( verbose )
  {
    gPad->Modified();
    gPad->Update();
    std::string junk;
    std::cout << "? ";
    std::cin >> junk;
  }
}
*/

void FindPeakRange(TH1 *h, double& xmin, double& thispeak, double& xmax, double threshold)
{
  int verbose = 1;

  int bin = h->FindBin( threshold );
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

    if ( verbose )
    {
      double x = h->GetBinCenter( ibin );
      std::cout << "bin x y nabove " << ibin << "\t" << x << "\t" << val << "\t" << nabove << std::endl;
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
      thispeak = h->GetBinCenter( ibin-20 );
      break;
    }

    ibin++;
  }

  xmax = thispeak + 4*(thispeak - xmin);

}


// type0: auau200
// type1: pp200
// method0:  TSpectrum bkg + 2 gaus fit
void recal_mbd_mip(const char *tfname = "DST_MBDUNCAL-00020869-0000.root", const int pass = 3, const int type = 0, const int method = 0)
{
  std::cout << "recal_mbd_mip(), type method " << type << " " << method << std::endl;
  std::cout << "tfname " << tfname << std::endl;

  const int NUM_PMT = 128;
  //const int NUM_PMT = 2;
//  const int NUM_ARMS = 2;

  // Create new TFile
  int runnumber = get_runnumber(tfname);
  TString dir = "results/";
  dir += runnumber;
  dir += "/";
  TString name = "mkdir -p "; name += dir;
  gSystem->Exec( name );

  name = dir; name += "calmbdpass2.3_q-"; name += runnumber; name += ".root";

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
    // if ( type == MBDRUNS::AUAU200 )
    // {
    //   //h_q[ipmt]->Rebin(2);
    // }
    // else if ( type == MBDRUNS::PP200 )
    // {
    //   //h_q[ipmt]->Rebin(4);  // b-off
    //   //h_q[ipmt]->Rebin(2);
    // }

    name = "h_tq"; name += ipmt;
    title = "tq"; title += ipmt;
    //h_tq[ipmt] = new TH1F(name,title,7000,-150,31*17.76);
    h_tq[ipmt] = (TH1*)oldfile->Get(name);
  }
  //TH2 *h2_tq = new TH2F("h2_tq","ch vs tq",900,-150,150,NUM_PMT,-0.5,NUM_PMT-0.5);
//  TH2 *h2_tq = (TH2*)oldfile->Get("h2_tq");

  name = dir; 
  name += "recalmbd_pass"; name += pass; name += ".root";
  std::cout << name << std::endl;

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

  std::ofstream cal_mip_file;
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
    std::cout << "setting up for pp200" << std::endl;
    //qmin = 200;
    //qmax = 10000;       // Run24pp boff
    //qmin = 100;
    //qmin = 50;
    qmin = 10;
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

  //TF1 *fws = new TF1("fws",woodssaxon,0,1000,3);
  //fws->SetParameters(-85.27,559.8,99.77);

  // Set up output pdf to save plots
  TString pdfname = dir; pdfname += "calmbdpass2."; pdfname += pass; pdfname += "_mip-"; pdfname += runnumber; pdfname += ".pdf";
  std::cout << pdfname << std::endl;
  ac[cvindex]->Print( pdfname + "[" );

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
      std::cout << "threshold " << ipmt << "\t" << threshold << std::endl;
      h_q[ipmt]->GetXaxis()->SetRangeUser( threshold, qmax );
      h_q[ipmt]->Sumw2();

      double sigma = 20;
      double seedmean = 0;
      TSpectrum s{};
      if ( method==0 )
      {
        h_bkg[ipmt] = s.Background( h_q[ipmt] );
        //h_bkg[ipmt]->Add( fws );

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

        std::cout << "peaks\t" << ipmt << "\t" << nfound << "\t" << best_peak << std::endl;
        */

      }
      else if ( method==1 )
      {
        FindPeakRange( h_bkg[ipmt], minrej, peak, maxrej, threshold );
        std::cout << "peak range\t" << minrej << "\t" << peak << "\t" << maxrej << std::endl;
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

//        double orig_minrej = minrej;
//        double orig_maxrej = maxrej;
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
      std::cout << "SEEDMEAN " << seedmean << std::endl;
      mipfit[ipmt]->SetParameter( 1, seedmean );
      mipfit[ipmt]->SetParameter( 2, 20. );
      */
      // Two gaussian fit
      mipfit[ipmt] = new TF1(name,gaus2,qmin,qmax,4);
      //mipfit[ipmt] = new TF1(name,gaus2expo,qmin,qmax,7);
      //mipfit[ipmt] = new TF1(name,"gaus",qmin,qmax,4);

      //mipfit[ipmt] = new TF1(name,langaufun,qmin,qmax,4);

      seedmean = h_mip[ipmt]->GetBinCenter( h_mip[ipmt]->GetMaximumBin() );
      std::cout << "SEEDMEAN " << seedmean << std::endl;
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
      //mipfit[ipmt]->SetParameter( 4, mipfit[ipmt]->GetParameter(0)*0.01 );
      //mipfit[ipmt]->SetParameter( 5, 200.);
      //mipfit[ipmt]->SetParameter( 6, 20.);
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
        << chi2/ndf << std::endl;
      std::cout << ipmt << "\t" << integ << "\t" << best_peak << "\t" << width << "\t"
        << integerr << "\t" << best_peakerr << "\t" << widtherr << "\t"
        << chi2/ndf << std::endl;

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
         std::cout << "? ";
         cin >> junk;
         */

      //name = dir + "h_qfit"; name += ipmt; name += ".png";
      title = "h_qfit"; title += ipmt;
      //std::cout << pdfname << " " << title << std::endl;
      ac[cvindex]->Print( pdfname, title );
    }

  }

  ac[cvindex]->Print( pdfname + "]" );
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
#endif
