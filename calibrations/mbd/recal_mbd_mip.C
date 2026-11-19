#ifndef MACRO_RECAL_MBD_MIP_C
#define MACRO_RECAL_MBD_MIP_C
//
// Do a recalibration of the mip from the saved histograms
//
#include "get_runstr.h"

#include <mbd/MbdCalib.h>

#include <fun4all/Fun4AllUtils.h>

#include <TCanvas.h>
#include <TF1.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TMath.h>
#include <TPad.h>
#include <TPaveStats.h>
#include <TSpectrum.h>
#include <TSystem.h>
#include <TStyle.h>

#include <fstream>

R__LOAD_LIBRARY(libmbd.so)

int verbose{0};

Double_t qmin = 25.;
Double_t qmax = 4000;
TF1 *bkgf[128]{nullptr};
TF1 *mipfit[128]{nullptr};
TF1 *totfit[128]{nullptr};

const int NUM_PMT = 128;
//const int NUM_PMT = 2;
//const int NUM_ARMS = 2;

TH1 *h_q[NUM_PMT];       // orig histograms
TH1 *h_tq[NUM_PMT];
TH1 *h_bkg[NUM_PMT];     // background histogram
TH1 *h_mip[NUM_PMT];     // mip signal histogram
TH1 *h_bkgmip[NUM_PMT];  // bkg + fit histogram


double minrej = 2000.;
double peak = 2000.;
double maxrej = 5000.;
double n_at_peak{0};

const int NPAR_BKGF = 9;
double seed_threshold[128]{0};
double seed_qmin[128]{0};
double seed_minrej[128]{0};
double seed_natpeak[128]{0};
double seed_maxrej[128]{0};
double seed_qmax[128]{0};
double seed_par[128][NPAR_BKGF]{{0}};

// Power law + exponential
// [0] = plaw ampl
// [1] = plaw parameter
// [2] = plaw exponent
// [3] = expo ampl
// [4] = expo exponent
Double_t expopowerlaw(Double_t *x, Double_t *par) //NOLINT(readability-non-const-parameter)
{
  Float_t xx =x[0];

  if ( xx>minrej && xx<maxrej )
  {
    TF1::RejectPoint();
    //return std::numeric_limits<double>::quiet_NaN();
    return 100;
  }

  Double_t f = par[0]*pow((par[1]/(par[1]+xx)),par[2]) + par[3]*exp(-1.0*par[4]*xx);

  return f;
}

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

// Simple skewed gaussian + 2nd gaussian peak
// [0] = ampl, peak 1
// [1] = mean
// [2] = sigma
// [3] = skewness
// [4] = ampl, peak 2
Double_t skgaus2(Double_t *x, Double_t *par) //NOLINT(readability-non-const-parameter)
{
  Double_t xx =x[0];

  Double_t denom = par[2]+par[3]*(xx-par[1]);
  if (std::abs(denom) < 1e-12) denom = 1e-12;  // prevent division by zero
  Double_t f = par[0]*exp(-0.5*pow((xx-par[1])/denom,2.0))
    + par[4]*TMath::Gaus(xx,2.0*par[1],sqrt(2)*par[2]); 

  return f;
}

// Power law + exponential + gaus2
// [0] = plaw ampl
// [1] = plaw parameter
// [2] = plaw exponent
// [3] = expo ampl
// [4] = expo exponent
// [5] = gaus ampl, peak 1
// [6] = gaus mean
// [7] = gaus sigma
// [8] = gaus ampl, peak 2
Double_t expopowerlawskgaus2(Double_t *x, Double_t *par) //NOLINT(readability-non-const-parameter)
{
  Double_t f = expopowerlaw(x,par) + skgaus2(x,&par[5]);
  return f;
}

// Two skewed gaussians
// [0] = ampl, peak 1
// [1] = xi (almost mean)
// [2] = omega (almost sigma)
// [3] = alpha (skewness)
// [4] = ampl, peak 2
Double_t skewedgaus2(Double_t *x, Double_t *par) // NOLINT(readability-non-const-parameter)
{
  double xx = x[0];
  double xi = par[1];
  double omega = par[2];
  double alpha = par[3];
  double arg = (xx - xi) / omega;
  double smallphi = TMath::Gaus(arg, 0.0, 1.0, true);
  double bigphi = 0.5 * (1 + std::erf(alpha * arg/std::sqrt(2)));
  double f = par[0] * (2./omega) * smallphi * bigphi;
  f += par[4]*TMath::Gaus(xx,2.0*par[1],sqrt(2)*par[2]);

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

/*
// Not used
Double_t langaufun(Double_t *x, Double_t *par) // NOLINT(readability-non-const-parameter)
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


  // MP shift correction
  mpc = par[1] - mpshift * par[0];

  // Range of convolution integral
  xlow = x[0] - sc * par[3];
  xupp = x[0] + sc * par[3];

  step = (xupp-xlow) / np;

  // Convolution integral of Landau and Gaussian by sum
  for(double i=1.0; i<=np/2.0; i+=1.0)
  {
    xx = xlow + (i-0.5) * step;
    fland = TMath::Landau(xx,mpc,par[0]) / par[0];
    sum += fland * TMath::Gaus(x[0],xx,par[3]);

    xx = xupp - (i-.5) * step;
    fland = TMath::Landau(xx,mpc,par[0]) / par[0];
    sum += fland * TMath::Gaus(x[0],xx,par[3]);
  }

  return (par[2] * step * sum * invsq2pi / par[3]);
}
*/

// Read in the seeds
void ReadSeeds(const std::string& sfname = "mipseeds.txt")
{
  std::ifstream seedsfile( sfname );
  if (!seedsfile.is_open())
  {
    std::cout << "ERROR: Could not open seed file " << sfname << std::endl;
    return;
  }
  int pmt;
  for ( int ipmt=0; ipmt<128; ipmt++ )
  {
    seedsfile >> pmt;
    if ( pmt != ipmt )
    {
      std::cout << "ERROR, seedsfile is bad" << ipmt << "\t" << pmt << std::endl;
    }
    //seedsfile >> seed_threshold[pmt] >> seed_minrej[pmt] >> seed_natpeak[pmt] >> seed_maxrej[pmt] >> seed_qmax[pmt];
    for (int ipar=0; ipar<NPAR_BKGF; ipar++)
    {
      seedsfile >> seed_par[pmt][ipar];
    }
  }
}

// find the threshold
// may also want to consider getting threshold from turn-on curves
void FindThreshold(TH1 *horig, double& threshold, const int runtype = 0)
{
  TH1 *h = (TH1*)horig->Clone("hnew");
  h->Smooth();
  threshold = 0.;
  double absolute_min = 10.;
  double absolute_max = 75.;    // max adc where the threshold could be (auau)
  if ( runtype==MBDRUNTYPE::PP200 )
  {
    absolute_max = 100.;
  }
  int absminbin = h->FindBin( absolute_min );
  int absmaxbin = h->FindBin( absolute_max );
  double absmaxval = h->GetBinContent( h->GetMaximumBin() );
  double maxval = 0.;     // max value found
  double prev_val = 0.;
  int maxbin = 0;         // bin for max value
  double maxratio = 0.;   // max ratio of bin/prev_bin
  int maxjumpbin = 0;     // where the max jump was

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
    // (Note: should raise gain for this channel if possible)
    double adc = h->GetBinCenter( maxjumpbin );
    double threshold_bin = h->FindBin( adc + 15. );
    threshold = h->GetBinLowEdge( threshold_bin );
  }
  else
  {
    // use max bin in threshold range
    threshold = h->GetBinLowEdge( maxbin );
  }

  delete h;

  //std::cout << threshold << std::endl;
}

void FindPeakRange(TH1 *horig, double& xmin, double& thispeak, double& xmax, const double threshold)
{
  TH1 *h = (TH1*)horig->Clone("hnew");
  Double_t integ = h->Integral();
  if ( integ<12000. )
  {
    h->Rebin(4);
  }
  else if ( integ<24000. )
  {
    h->Rebin(2);
  }
  h->Smooth();
  
  // zero up to threshold
  int threshbin = h->FindBin(threshold);
  for (int ibin=1; ibin<threshbin; ibin++)
  {
    h->SetBinContent(ibin,0.);
  }

  // find mip peak
  Double_t binwid = h->GetBinWidth(1);
  Double_t ymax{0.};
  Int_t peakbin{0};

  Double_t temp_threshold = threshold;
  thispeak = temp_threshold;

  while ( std::abs(thispeak-temp_threshold) < (5.0*binwid) )
  {
    h->SetBinContent( peakbin, 0. );
    ymax = h->GetMaximum();
    peakbin = h->GetMaximumBin();
    thispeak = h->GetBinCenter( peakbin );

    if ( std::abs(thispeak-temp_threshold) < (5.0*binwid) )
    {
      temp_threshold = thispeak;
    }
    
    if ( verbose>=5 )
    {
      std::cout << "peakfind thresh x_at_peak peakval\t" << temp_threshold << "\t" << thispeak << "\t" << ymax << "\t" << peakbin << std::endl;
    }

    /*
    h->GetXaxis()->SetRangeUser(62,300);
    h->Draw();
    gPad->SetGridx(1);
    gPad->SetGridy(1);
    gPad->Modified();
    gPad->Update();
    std::string junk;
    cin >> junk;
    */
  }

  delete h;

  // find xmin (min between threshold and peak)
  h = (TH1*)horig->Clone("hnew");
  h->Smooth();
  int nbinsx = h->GetNbinsX();
  for (int ibin=1; ibin<=nbinsx; ibin++)
  {
    if ( ibin<threshbin || ibin>peakbin )
    {
      h->SetBinContent( ibin, 1e12 );
    }
  }

  xmin = h->GetBinLowEdge( h->GetMinimumBin() );
  if ( xmin==threshold )
  {
    xmin += binwid;     // no bkg b4 mip situation
  }

  Double_t peakmindiff = thispeak - xmin;
  xmax = 2*thispeak + 2.0*peakmindiff;  // 1.0 for pp

  delete h;
}


void recal_mbd_mip(const std::string &tfname = "DST_MBDUNCAL-00020869-0000.root", const int pass = 3)
{
  std::cout << "tfname " << tfname << std::endl;
  gStyle->SetOptFit(1111);
  gStyle->SetOptStat(111111);

  // set up run-dependent settings
  // type0: auau200
  // type1: pp200
  // method0:  TSpectrum bkg + 2 gaus fit
  // method1:  expopowerlaw + 2 skgaus fit
//  int runnumber = get_runnumber(tfname);
  std::pair<int, int> runseg = Fun4AllUtils::GetRunSegment(tfname);
  int runnumber = runseg.first;
  int type = get_runtype( runnumber );
  int method = 1;
  if ( type == MBDRUNTYPE::AUAU200 )
  {
    method = 0;
  }
  else if ( type == MBDRUNTYPE::PP200 )
  {
    method = 1;

    // Read in Seeds File
    ReadSeeds();
  }
  std::cout << "recal_mbd_mip(), type method " << type << " " << method << std::endl;

  // make results directory
  TString dir = "results/";
  dir += runnumber;
  dir += "/";
  TString name = "mkdir -p ";
  name += dir;
  gSystem->Exec( name );

  // Read in TFile with h_q
  name = dir;
  name += "calmbdpass2.3_q-";
  name += runnumber;
  name += ".root";
  TFile *oldfile = TFile::Open(name,"READ");

  if ( (oldfile == nullptr) || oldfile->IsZombie() )
  {
    std::cout << "Error: bad tfile " << name << std::endl;
    return;
  }

  TString title;
  for (int ipmt=0; ipmt<NUM_PMT; ipmt++)
  {
    name = "h_q"; name += ipmt;
    title = "q"; title += ipmt;
    oldfile->GetObject(name, h_q[ipmt]);
    h_q[ipmt]->SetMarkerSize(0.6);

    /*
    if ( type == MBDRUNTYPE::AUAU200 )
    {
      //h_q[ipmt]->Rebin(2);
    }
    else if ( type == MBDRUNTYPE::PP200 )
    {
      //h_q[ipmt]->Rebin(4);  // b-off
      //h_q[ipmt]->Rebin(2);
    }
    */

    name = "h_tq"; name += ipmt;
    title = "tq"; title += ipmt;
    oldfile->GetObject(name,h_tq[ipmt]);
  }

  // Create new TFile
  name = dir; 
  name += "recalmbd_pass";
  name += pass;
  name += ".root";
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
    calfname = dir;
    calfname += "mbd_qfit.calib";
    cal_mip_file.open( calfname );
    std::cout << "Writing to " << calfname << std::endl;
  }

  // Fit ranges
  // Run23AuAu
  qmin = 25.;
  //qmax = 1600;
  qmax = 2000;

  if ( type==MBDRUNTYPE::PP200 )
  {
    std::cout << "setting up for pp200" << std::endl;
    //qmin = 200;
    //qmax = 10000;       // Run24pp boff
    //qmin = 100;
    //qmin = 50;
    //qmin = 10;
    //qmax = 2000;       // Run24pp bon
    qmin = 20;
    qmax = 4000;       // Run25pp bon
  }
  else if ( type==MBDRUNTYPE::SIMAUAU200 || type==MBDRUNTYPE::SIMPP200 )
  {
    qmin = 0.25;
    qmax = 6.;
  }

  // Set up output pdf to save plots
  TString pdfname = dir; pdfname += "calmbdpass2."; pdfname += pass; pdfname += "_mip-"; pdfname += runnumber; pdfname += ".pdf";
  std::cout << pdfname << std::endl;
  ac[cvindex]->Print( pdfname + "[" );

  // output bkgfit save file
  TString savefitsname = dir;
  savefitsname += "savefitspass2."; savefitsname += pass; savefitsname += "_mip-";
  savefitsname += runnumber; savefitsname += ".txt";
  std::ofstream savefits(savefitsname);

  for (int ipmt=0; ipmt<NUM_PMT; ipmt++)
  {
    ac[cvindex]->cd(1);
    h_q[ipmt]->Draw();

    if ( verbose>9 && ipmt!=0 ) continue;  // check sgl channel
    if (pass>0)
    {
      double threshold{0.};
      FindThreshold( h_q[ipmt], threshold, type );
      std::cout << "threshold " << ipmt << "\t" << threshold << std::endl;
      h_q[ipmt]->GetXaxis()->SetRangeUser( threshold, qmax );
      h_q[ipmt]->Sumw2();

      FindPeakRange( h_q[ipmt], minrej, peak, maxrej, threshold );  // works only in pp for now
      qmin = threshold;
      n_at_peak = h_q[ipmt]->GetBinContent( h_q[ipmt]->FindBin( peak ) );

      std::cout << "peak range\t" << minrej << "\t" << peak << "\t" << maxrej << "\t" << qmin << "\t" << qmax << std::endl;
      savefits << ipmt << "\t" << threshold << "\t" << minrej << "\t" << n_at_peak << "\t" << maxrej << "\t" << qmax;

      double seedmean = peak;
      double seedsigma = 20.;

      if ( method==0 )
      {
        TSpectrum s{};
        h_bkg[ipmt] = s.Background( h_q[ipmt] );
        h_bkg[ipmt]->SetLineColor(2);

        h_mip[ipmt] = (TH1*)h_q[ipmt]->Clone();
        name = h_q[ipmt]->GetName(); name.ReplaceAll("q","mip");
        h_mip[ipmt]->SetName( name );
        h_mip[ipmt]->SetTitle( name );
        h_mip[ipmt]->SetLineColor( 4 );
        h_mip[ipmt]->Add( h_bkg[ipmt], -1.0 );

        seedmean = h_mip[ipmt]->GetBinCenter( h_mip[ipmt]->GetMaximumBin() );
        seedsigma = 0.2*seedmean;
      }
      else if ( method==1 )
      {
        h_bkg[ipmt] = (TH1*)h_q[ipmt]->Clone();
        name = h_q[ipmt]->GetName(); name.ReplaceAll("q","bkg");
        h_bkg[ipmt]->SetName( name );
        h_bkg[ipmt]->SetTitle( name );
        h_bkg[ipmt]->SetLineColor(2);

        // Fit background
        name = "bkgf"; name += ipmt;
        bkgf[ipmt] = new TF1(name,expopowerlaw,qmin,qmax,5);
        bkgf[ipmt]->SetNpx(1000);
        bkgf[ipmt]->SetParameter(0,seed_par[ipmt][0]*n_at_peak);
        bkgf[ipmt]->SetParameter(1,seed_par[ipmt][1]);
        bkgf[ipmt]->SetParameter(2,seed_par[ipmt][2]);
        bkgf[ipmt]->SetParameter(3,seed_par[ipmt][3]*n_at_peak);
        bkgf[ipmt]->SetParameter(4,seed_par[ipmt][4]);
        bkgf[ipmt]->SetLineColor(3);
        h_bkg[ipmt]->GetXaxis()->SetRangeUser(threshold,qmax);
        h_bkg[ipmt]->Draw();
        //h_bkg[ipmt]->Fit(bkgf[ipmt],"MR");
        h_bkg[ipmt]->Fit(bkgf[ipmt],"R");

        gPad->SetLogy(1);
        gPad->Modified();
        gPad->Update();

        if (verbose ) // after bkg fit
        {
          std::string junk2;
          std::cin >> junk2;
        }

        // get mip histogram
        h_mip[ipmt] = (TH1*)h_q[ipmt]->Clone();
        name = h_q[ipmt]->GetName(); name.ReplaceAll("q","mip");
        h_mip[ipmt]->SetName( name );
        h_mip[ipmt]->SetTitle( name );


        // double orig_minrej = minrej;
        // double orig_maxrej = maxrej;
        minrej = 0.;
        maxrej = 0.;
        h_mip[ipmt]->Add( bkgf[ipmt], -1.0 );
        h_bkg[ipmt]->Reset();
        h_bkg[ipmt]->Add(bkgf[ipmt]);
        h_q[ipmt]->Draw();
        h_bkg[ipmt]->Draw("same");
      }

      ac[cvindex]->cd(2);
     
      // Fit the mip peak after background subtraction
      name = "mipfit"; name += ipmt;

      // Two gaussian fit
      //mipfit[ipmt] = new TF1(name,gaus2,qmin,qmax,4);
      mipfit[ipmt] = new TF1(name,skgaus2,qmin,qmax,5);
      mipfit[ipmt]->SetNpx(1000);
      mipfit[ipmt]->SetLineColor(4);
      mipfit[ipmt]->SetParNames("ampl","mean","sigma","skew","ampl2");

      /*
      if ( type==MBDRUNTYPE::SIMAUAU200 )
      {
        seedsigma = 0.2;
      }
      else if ( type==MBDRUNTYPE::PP200 )
      {
        seedsigma = seedmean*(116./719.);  // b-on
        //seedsigma = 50;  // b-on
        //seedsigma = 200;  // b-off
      }
      */

      // for gaus2
      double seed_ampl = 5.0*h_mip[ipmt]->GetMaximum();
      mipfit[ipmt]->SetParameter( 0, seed_ampl );
      mipfit[ipmt]->SetParameter( 1, seedmean );
      mipfit[ipmt]->SetParameter( 2, seedsigma );
      mipfit[ipmt]->SetParameter( 3, 0.1 );
      if ( type==MBDRUNTYPE::PP200 )
      {
        mipfit[ipmt]->SetParameter( 4, seed_ampl*0.1 );
        mipfit[ipmt]->FixParameter(4,0.);
      }
      else if ( type==MBDRUNTYPE::AUAU200 )
      {
        mipfit[ipmt]->SetParameter( 4, seed_ampl*0.2 );
      }

      std::cout << "MIPFIT SEEDS " << seed_ampl << "\t" << seedmean << "\t" << seedsigma << std::endl;

      //h_mip[ipmt]->Fit( mipfit[ipmt], "RM" );
      h_mip[ipmt]->Fit( mipfit[ipmt], "R" );
      gPad->Modified();
      gPad->Update();

      if ( verbose ) // after mip fit
      {
        std::string junk2;
        std::cin >> junk2;
      }

      // Now do total re-fit (after we have gotten seeds for mip and background)
      if ( method==1 )
      {
        ac[cvindex]->cd(1);
        name = "totfit"; name += ipmt;
        minrej = 0.;
        maxrej = 0.;
        //totfit[ipmt] = new TF1(name,expopowerlawgaus2,qmin,qmax,9);
        totfit[ipmt] = new TF1(name,expopowerlawskgaus2,qmin,qmax,10);
        totfit[ipmt]->SetNpx(1000);
        totfit[ipmt]->SetLineColor(4);
        totfit[ipmt]->SetParNames("plaw_ampl", "plaw_alpha", "plaw_power", "expo_ampl", "expo_power",
            "gaus_ampl", "gaus_mean", "gaus_sigma", "gaus_skew", "gaus2_ampl");
        for (int ipar=0; ipar<5; ipar++)
        {
          totfit[ipmt]->SetParameter(ipar,bkgf[ipmt]->GetParameter(ipar));
        }
        for (int ipar=5; ipar<totfit[ipmt]->GetNumberFreeParameters(); ipar++)
        {
          totfit[ipmt]->SetParameter(ipar,mipfit[ipmt]->GetParameter(ipar-5));
        }

        totfit[ipmt]->SetParameter(7,std::abs(totfit[ipmt]->GetParameter(7)));
        //totfit[ipmt]->SetParLimits(9,0.,1e12);
        totfit[ipmt]->FixParameter(9,0.);

        h_q[ipmt]->Fit( totfit[ipmt], "RM" );

        double redchi2 = totfit[ipmt]->GetChisquare()/totfit[ipmt]->GetNDF();
        if ( redchi2>5.0 )
        {
          totfit[ipmt]->SetParameter(0,seed_par[ipmt][0]*n_at_peak);
          totfit[ipmt]->SetParameter(1,seed_par[ipmt][1]);
          totfit[ipmt]->SetParameter(2,seed_par[ipmt][2]);
          totfit[ipmt]->SetParameter(3,seed_par[ipmt][3]*n_at_peak);
          totfit[ipmt]->SetParameter(4,seed_par[ipmt][4]);
          totfit[ipmt]->SetParameter(5,seed_ampl );
          totfit[ipmt]->SetParameter(6,seedmean );
          totfit[ipmt]->SetParameter(7,seedsigma );
          totfit[ipmt]->SetParameter(8,seed_par[ipmt][8]);
          h_q[ipmt]->Fit( totfit[ipmt], "RM" );
        }
        gPad->Modified();
        gPad->Update();
        if (verbose)
        {
          std::string junk;
          std::cout << "? ";
          std::cin >> junk;
        }

        for (int ipar=0; ipar<5; ipar++)
        {
          bkgf[ipmt]->SetParameter( ipar, totfit[ipmt]->GetParameter( ipar ));
        }
        for (int ipar=5; ipar<totfit[ipmt]->GetNumberFreeParameters(); ipar++)
        {
          mipfit[ipmt]->SetParameter( ipar-5, totfit[ipmt]->GetParameter( ipar ));
          mipfit[ipmt]->SetParError( ipar-5, totfit[ipmt]->GetParError( ipar ));
        }

        // get h_mip again
        h_mip[ipmt]->Reset();
        //h_mip[ipmt]->Clear();
        h_mip[ipmt]->ResetStats();
        h_mip[ipmt]->Add( h_q[ipmt] );
        h_mip[ipmt]->Add( bkgf[ipmt], -1.0 );

        //h_mip[ipmt]->Draw();
        h_mip[ipmt]->Fit(mipfit[ipmt],"R");
        //mipfit[ipmt]->Draw("same");
      }

      double integ = mipfit[ipmt]->GetParameter(0);
      double best_peak = mipfit[ipmt]->GetParameter(1);
      double width = mipfit[ipmt]->GetParameter(2);
      double integerr = mipfit[ipmt]->GetParError(0);
      double best_peakerr = mipfit[ipmt]->GetParError(1);
      double widtherr = mipfit[ipmt]->GetParError(2);
      double chi2 = mipfit[ipmt]->GetChisquare();
      double ndf = mipfit[ipmt]->GetNDF();
      if ( method==1 )
      {
        double ORIG_chi2 = chi2;
        chi2 = h_mip[ipmt]->Chisquare(mipfit[ipmt],"R"); // from re-fit
        std::cout << "CHI2COMPARE " << ipmt << "\t" << chi2 << "\t" << ORIG_chi2 << "\t" << ORIG_chi2-chi2 << std::endl;
        mipfit[ipmt]->SetChisquare( chi2 );
      }

      cal_mip_file << ipmt << "\t" << integ << "\t" << best_peak << "\t" << std::abs(width) << "\t"
        << integerr << "\t" << best_peakerr << "\t" << widtherr << "\t"
        << chi2/ndf << std::endl;
      std::cout << ipmt << "\t" << integ << "\t" << best_peak << "\t" << width << "\t"
        << integerr << "\t" << best_peakerr << "\t" << widtherr << "\t"
        << chi2/ndf << std::endl;

      if ( method==0 )
      {
        for (int ipar=0; ipar<mipfit[ipmt]->GetNumberFreeParameters(); ipar++)
        {
          if ( ipar==0 || ipar==4 ) // scale all the amplitude parameters
          {
            savefits << "\t" << mipfit[ipmt]->GetParameter(ipar)/n_at_peak;
          }
          else
          {
            savefits << "\t" << mipfit[ipmt]->GetParameter(ipar);
          }
        }
        savefits << std::endl;
      }
      else if ( method==1 )
      {
        for (int ipar=0; ipar<totfit[ipmt]->GetNumberFreeParameters(); ipar++)
        {
          if ( ipar==0 || ipar==3 || ipar==5 || ipar==9 ) // scale all the amplitude parameters
          {
            savefits << "\t" << totfit[ipmt]->GetParameter(ipar)/n_at_peak;
          }
          else
          {
            savefits << "\t" << totfit[ipmt]->GetParameter(ipar);
          }
        }
        savefits << std::endl;

        // Get full fit
        h_bkg[ipmt]->Reset();
        h_bkg[ipmt]->Add(bkgf[ipmt]);
      }

      h_bkgmip[ipmt] = (TH1*)h_q[ipmt]->Clone();
      name = h_q[ipmt]->GetName(); name.ReplaceAll("q","bkgmip");
      h_bkgmip[ipmt]->SetName( name );
      h_bkgmip[ipmt]->SetTitle( name );
      h_bkgmip[ipmt]->Reset();
      h_bkgmip[ipmt]->Add(h_bkg[ipmt]);
      //h_bkgmip[ipmt]->Sumw2();
      h_bkgmip[ipmt]->Add( mipfit[ipmt] );
      h_bkgmip[ipmt]->SetLineColor( kCyan );

      // Now draw the full dist, plus fit
      ac[cvindex]->cd(1);
      gPad->SetLogy(1);
      h_q[ipmt]->GetXaxis()->SetRangeUser( qmin, qmax );
      //h_q[ipmt]->SetMinimum(10.);
      h_q[ipmt]->Draw();
      h_bkg[ipmt]->Draw("histsame");
      h_bkgmip[ipmt]->Draw("histsame");
      TPaveStats *st = (TPaveStats*)h_q[ipmt]->FindObject("stats");
      if ( st )
      {
        st->SetX1NDC(0.6); // X start position (0 to 1)
        st->SetY1NDC(0.4); // Y start position (0 to 1)
        st->SetX2NDC(0.99); // X end position
        st->SetY2NDC(0.99); // Y end position
      }

      gPad->Modified();
      gPad->Update();

      ac[cvindex]->cd(2);
      h_mip[ipmt]->Draw();
      if ( type==MBDRUNTYPE::PP200 )
      {
        h_mip[ipmt]->GetXaxis()->SetRangeUser(qmin,1200);
        if ( mipfit[ipmt]->GetParameter(1)>600. )
        {
          h_mip[ipmt]->GetXaxis()->SetRangeUser(qmin,1400);
        }
      }
      else if ( type==MBDRUNTYPE::AUAU200 )
      {
        h_mip[ipmt]->GetXaxis()->SetRangeUser(qmin,1000);
      }
      gPad->SetGridy(1);
      gPad->Modified();
      gPad->Update();

      if (verbose)
      {
        std::string junk;
        std::cout << "? ";
        std::cin >> junk;
      }

      title = "h_qfit"; title += ipmt;
      //std::cout << pdfname << " " << title << std::endl;
      ac[cvindex]->Print( pdfname, title );
    }


  }

  //savefits.write();
  savefits.close();

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

  for (auto & hist: h_q)
  {
    hist->Write();
  }
  savefile->Write();
  savefile->Close();
}
#endif
