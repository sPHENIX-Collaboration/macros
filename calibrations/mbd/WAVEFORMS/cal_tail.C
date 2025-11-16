//
// To calibrate the tails from events in previous crossings,
//
//    1. first create the mdbsigXX.txt files (uncomment pileupfile in MbdSig.cc)
//
//    2. get_tails() : extracts the mean of the tails from all time and charge ch's from all events
//                     can be used to play with functional fits
//                     (once fit fcn determined, not necessary and can skip to 2)
//
//    3. calc_residuals() : fit the waveforms from events where there was no event in crossing,
//                          but there was an event in prev. crossing
//                          writes out fit parameters to tfit and qfit tree
//                          makes plots of residuals to fits
//                          only the charge fit makes sense here, since the time can be done by simple
//                          correlation
//
//    4. fit_time_pileupcorr() : find slope of s8 vs s0 in time channels, write out mbd_pileup.calib file
//                               this is already done at end of calc_residuals
//
#include "find_th2ridge.C"

#include <TGraphErrors.h>
#include <TF1.h>
#include <TFile.h>
#include <TPad.h>
#include <TTree.h>

#include <fstream>
#include <iostream>
#include <set>
#include <string>

const int NFEECH = 256;
//const int NFEECH = 10;
const int NSAMPLES = 16;
 
TH2 *h2_tail[NFEECH];
TH2 *h2_residuals[NFEECH];
TH2 *h2_s8s0[NFEECH]{nullptr};
TGraphErrors *g_tail[NFEECH];
TF1 *fit[NFEECH] = {nullptr};
TF1 *fit_s8s0[NFEECH] = {nullptr};

// Tree variables
TTree *tfit_tree{nullptr};
Short_t tch{-1};
Float_t tchi2ndf{-1.};
Float_t tfitpar[10]{0.};
TTree *qfit_tree{nullptr};
Short_t qch{-1};
Float_t qchi2ndf{-1.};
Float_t qfitpar[10]{0.};

Double_t powerlaw(Double_t *x, Double_t *par) // NOLINT(readability-non-const-parameter)
{
  Float_t xx =x[0];

  Double_t f = par[0]*pow((par[1]/(par[1]+xx)),par[2]); 

  return f;
}

//
// Do test fits of the g_tail, which is the mean tail shape from all events
// Used to develop the fit function
//
void fit_test(const int feech = 8, const std::string &fname = "mbdsig_tails.root")
{
  TString name;

  static TFile *tfile{nullptr};

  if ( tfile == nullptr )
  {
    std::cout << "Opening " << fname << std::endl;
    tfile = new TFile(fname.c_str(),"READ");
  }

  name = "g_tail"; name += feech;
  TGraphErrors *g = (TGraphErrors*)tfile->Get( name );

  TF1 *fit2mean{nullptr};

  if ( ((feech/8)%2)==0 ) // time channel
  {
    //fit2mean = new TF1("fit2mean","gaus+[3]*x+[4]",0,16);
    fit2mean = new TF1("fit2mean","gaus+pol2(3)",0,16);
    fit2mean->SetParameters(131.5,-18,5.87,-0.002,-0.002/16.,1e-3);
    //fit2mean->SetParameters(49.18,-14.544,5.2014,0.0139,-0.00147,4.064e-5);

    g->Draw("ap");
    g->Fit(fit2mean,"R");

    TF1 *gaussian = new TF1("gaussian","gaus",0,16);
    gaussian->SetParameters(fit2mean->GetParameter(0),fit2mean->GetParameter(1),fit2mean->GetParameter(2));
    gaussian->SetLineColor(4);
    gaussian->Draw("same");
  }
  else
  {
    //fit2mean = new TF1("fit2mean","gaus+pol2(3)",0,16);
    //fit2mean->SetParameters(131.5,-18,5.87,-0.002,-0.002/16.,1e-3);

    fit2mean = new TF1("fit2mean","expo + pol2(2)",0,16);
    fit2mean->SetParameters(1,-1/16.,1e-2,1e-3,1e-4);

    g->Draw("ap");
    g->Fit(fit2mean,"R");
  }

  gPad->SetLogy(1);
}

void fit_tail(TGraph *g, const int feech)
{
  int verbose = 0;

  TString name;
  if ( fit[feech] == nullptr )
  {
    if ( ((feech/8)%2)==0 ) // time channel
    {
      name = "fit"; name += feech;
      fit[feech] = new TF1(name,"gaus+pol2(3)",-0.1,16);
      fit[feech]->SetLineColor(2);
      fit[feech]->SetParameters(5.5*g->GetPointY(0),-4.8,2.6,-0.002,-0.002/16.,1e-3);
      //fit[feech]->SetParameters(49.18,-14.544,5.2014,0.0139,-0.00147,4.064e-5);
    }
    else
    {
      name = "fit"; name += feech;
      //fit[feech] = new TF1(name,"expo+pol2(2)",0,16);
      //fit[feech]->SetParameters(1,-1/16.,1e-2,1e-3,1e-4);
      
      //fit[feech] = new TF1(name,"expo",0,4);
      //fit[feech]->SetParameters(log(g->GetPointY(0)),-1/16);

      fit[feech] = new TF1(name,"gaus",-0.1,4.1);
      fit[feech]->SetParameters(1.43*g->GetPointY(0),-3.2,3.8);
      
      fit[feech]->SetLineColor(2);
    }
  }

  if ( ((feech/8)%2)==0 ) // time channel
  {
    if ( verbose )
    {
      g->Draw("ap");
      fit[feech]->SetRange(-0.1,16);
      g->Fit(fit[feech],"R");
      gPad->SetLogy(1);
      Double_t chi2 = fit[feech]->GetChisquare();
      Double_t ndf = fit[feech]->GetNDF();
      std::cout << "chi2 ndf " << chi2 << "\t" << ndf << std::endl;
    }
    else
    {
      fit[feech]->SetRange(-0.1,16);
      g->Fit(fit[feech],"RNQ");

      // save to tree
      tch = feech;
      for (int ipar=0; ipar<6; ipar++)
      {
        tfitpar[ipar] = static_cast<Float_t>( fit[feech]->GetParameter(ipar) );
      }

      // copy samp 0 and samp 8 values. samp 8 should be timing maxsamp
      tfitpar[7] = static_cast<Float_t>( g->GetPointY(0) );
      tfitpar[8] = static_cast<Float_t>( g->GetPointY(1) );
      tfitpar[9] = static_cast<Float_t>( g->GetPointY(8) );

      h2_s8s0[feech]->Fill( g->GetPointY(0), g->GetPointY(8) );

      Double_t chi2 = fit[feech]->GetChisquare();
      Double_t ndf = fit[feech]->GetNDF();
      tchi2ndf = chi2/ndf;
      tfit_tree->Fill();
    }

    if ( verbose )
    {
      /*
      TF1 *gaussian = new TF1("gaussian","gaus",0,16);
      gaussian->SetParameters(fit[feech]->GetParameter(0),fit[feech]->GetParameter(1),fit[feech]->GetParameter(2));
      gaussian->SetLineColor(4);
      gaussian->Draw("same");
      */
    }

  }
  else    // charge channel
  {
    if ( verbose )
    {
      g->Draw("ap");
      //gPad->SetLogy(1);
      fit[feech]->SetRange(-0.1,4.1);
      //fit[feech]->SetParameters(1.43*g->GetPointY(0),-3.2,3.8);
      fit[feech]->SetParameters(200.*g->GetPointY(0),-32.,9.8);
      g->Fit(fit[feech],"R");

      Double_t chi2 = fit[feech]->GetChisquare();
      Double_t ndf = fit[feech]->GetNDF();
      std::cout << "chi2 ndf " << chi2 << "\t" << ndf << std::endl;

    }
    else
    {
      fit[feech]->SetRange(-0.1,4.1);
      //fit[feech]->SetParameters(1.43*g->GetPointY(0),-3.2,3.8);
      fit[feech]->SetParameters(200.*g->GetPointY(0),-32.,9.8);
      g->Fit(fit[feech],"RNQ");

      // save to tree
      qch = feech;
      for (int ipar=0; ipar<3; ipar++)
      {
        qfitpar[ipar] = static_cast<Float_t>( fit[feech]->GetParameter(ipar) );
      }
      Double_t chi2 = fit[feech]->GetChisquare();
      Double_t ndf = fit[feech]->GetNDF();
      qchi2ndf = chi2/ndf;

      qfit_tree->Fill();

    }

    /*
    TF1 *expon = new TF1("expon","expo",0,16);
    expon->SetParameters(fit[feech]->GetParameter(0),fit[feech]->GetParameter(1));
    expon->SetLineColor(4);
    expon->Draw("same");
    */

  }

  if ( verbose )
  {
    fit[feech]->SetRange(0,16);
    fit[feech]->Draw("same");
  
    gPad->Modified();
    gPad->Update();
    std::string junk;
    std::cin >> junk;
  }
}


//
// get the events with only tails in them to use for fitting
//
void get_tails()
{
  std::ifstream infile;

  TString name;

  name = "mbdsig_tails.root";
  TFile *savefile = new TFile(name,"RECREATE");

  for (int ifeech=0; ifeech<NFEECH; ifeech++)
  {
    name = "h2_tail"; name += ifeech;
    h2_tail[ifeech]  = new TH2F(name,name,NSAMPLES,-0.5,NSAMPLES-0.5,2200,-0.1,1.1);
  }

  // set up the sample number array
  double xsamp[NSAMPLES];
  double yval[NSAMPLES];
  for (int isamp=0; isamp<NSAMPLES; isamp++)
  {
    xsamp[isamp] = isamp;
  }


  std::string junk1;
  std::string junk2;
  int ch;
  double mean;
  double pedsigma = 4.0;

  for (int ifeech=0; ifeech<NFEECH; ifeech++)
  {
    name = "mbdsig"; name += ifeech; name += ".txt";
    infile.open( name.Data() );
    int evt = 1;
    while ( infile >> junk1 >> ch >> junk2 >> mean )
    {
      if ( evt%1000 == 1 )
      {
        std::cout << "evt " << evt << std::endl;
      }

      for (double & isamp : yval)
      {
        infile >> isamp;
      }

      // find those tail only events with high enough amplitude
      if ( (fabs(yval[12]-mean) < pedsigma*3) && ((yval[0]-mean) > 200) )
      {
        // fill histogram
        for (int isamp=0; isamp<NSAMPLES; isamp++)
        {
          h2_tail[ifeech]->Fill( isamp, (yval[isamp]-mean)/(yval[0]-mean) );
        }
      }

      /*
      if ( fabs(yval[12]-mean) < pedsigma*3 )
      {
        g_subpulse->Draw("ap");
        gPad->Modified();
        gPad->Update();
        std::cin >> junk2;
      }
      */

      evt++;
    }

    infile.close();
  }

  for (int ifeech=0; ifeech<NFEECH; ifeech++)
  {
    g_tail[ifeech] = find_th2ridge(h2_tail[ifeech]);
    //g_tail[ifeech]->InsertPointBefore(0,0,1);
    g_tail[ifeech]->SetPointError(0,0,g_tail[ifeech]->GetErrorY(1));
    name = "g_tail"; name += ifeech;
    g_tail[ifeech]->SetName(name);

    h2_tail[ifeech]->Draw("colz");
    g_tail[ifeech]->Draw("cp");

    name = "tail_"; name += ifeech; name += ".png";
    gPad->Print( name );

    gPad->SetLogz(1);
    gPad->Modified();
    gPad->Update();
    /*
    std::string junk;
    std::cin >> junk;
    */

    g_tail[ifeech]->Write();
  }

  savefile->Write();
}

//
// fits s8 vs s0 in time channels to get time pileup correction
// then writes out mbd_pileup.calib file
//
void fit_time_pileupcorr()
{ 
  std::ofstream calibfile("mbd_pileup.calib");

  TString name;

  for (int ifeech=0; ifeech<NFEECH; ifeech++)
  {
    if ( ((ifeech/8)%2) == 0 )  // time ch
    {
      name = "f_s8s0_"; name += ifeech;
      fit_s8s0[ifeech] = new TF1(name,"[0]*x",0,4500);
      fit_s8s0[ifeech]->SetParameters(0.01);

      h2_s8s0[ifeech]->Fit(fit_s8s0[ifeech],"R");

      double p0 = fit_s8s0[ifeech]->GetParameter(0);
      double p0err = fit_s8s0[ifeech]->GetParError(0);
      double chi2ndf = fit_s8s0[ifeech]->GetChisquare()/fit_s8s0[ifeech]->GetNDF();

      calibfile << ifeech
                << "\t" << p0 << "\t" << p0err
                << "\t" << 0. << "\t" << 0.
                << "\t" << 0. << "\t" << 0.
                << "\t" << chi2ndf << std::endl;
    }
    else  // charge ch
    {
      calibfile << ifeech
                << "\t" << 200. << "\t" << 0.
                << "\t" << -32. << "\t" << 0.
                << "\t" << 9.8 << "\t" << 0.
                << "\t" << 1. << std::endl;
    }
  }

  calibfile.close();
}


//
// get the residuals event by event
//
void calc_residuals()
{
  std::ifstream infile;

  TString name;

  name = "mbdsig_tailresiduals.root";
  TFile *savefile = new TFile(name,"RECREATE");

  for (int ifeech=0; ifeech<NFEECH; ifeech++)
  {
    name = "h2_residuals"; name += ifeech;
    h2_residuals[ifeech]  = new TH2F(name,name,NSAMPLES,-0.5,NSAMPLES-0.5,200,-100,100);
    h2_residuals[ifeech]->SetXTitle("sample");

    name = "h2_s8s0_"; name += ifeech;
    h2_s8s0[ifeech]  = new TH2F(name,name,900,-0.5,4500-0.5,100,-20,120);
    h2_s8s0[ifeech]->SetXTitle("s0");
    h2_s8s0[ifeech]->SetYTitle("s8");
  }

  tfit_tree = new TTree("tfit","time ch fits");
  tfit_tree->Branch("ch",&tch,"ch/S");
  tfit_tree->Branch("chi2ndf",&tchi2ndf,"chi2ndf/F");
  tfit_tree->Branch("ampl",&tfitpar[0],"ampl/F");
  tfit_tree->Branch("mean",&tfitpar[1],"mean/F");
  tfit_tree->Branch("sig",&tfitpar[2],"sig/F");
  tfit_tree->Branch("p0",&tfitpar[3],"p0/F");
  tfit_tree->Branch("p1",&tfitpar[4],"p1/F");
  tfit_tree->Branch("p2",&tfitpar[5],"p2/F");
  tfit_tree->Branch("s0",&tfitpar[7],"s0/F");   // samp0
  tfit_tree->Branch("s1",&tfitpar[8],"s1/F");   // samp1
  tfit_tree->Branch("s8",&tfitpar[9],"s8/F");   // samp8 (used for timing)

  qfit_tree = new TTree("qfit","charge ch fits");
  qfit_tree->Branch("ch",&qch,"ch/S");
  qfit_tree->Branch("chi2ndf",&qchi2ndf,"chi2ndf/F");
  qfit_tree->Branch("ampl",&qfitpar[0],"ampl/F");
  qfit_tree->Branch("mean",&qfitpar[1],"mean/F");
  qfit_tree->Branch("sig",&qfitpar[2],"sig/F");

  TGraphErrors *g_subpulse = new TGraphErrors(NSAMPLES);

  // set up the sample number array
  double xsamp[NSAMPLES];
  double yval[NSAMPLES];
  for (int isamp=0; isamp<NSAMPLES; isamp++)
  {
    xsamp[isamp] = isamp;
  }

  std::string junk1;
  std::string junk2;
  int ch;
  double mean;
  double pedsigma = 4.0;

  for (int ifeech=0; ifeech<NFEECH; ifeech++)
  {
    name = "mbdsig"; name += ifeech; name += ".txt";
    std::cout << "Processing " << name << std::endl;
    infile.open( name.Data() );
    int evt = 1;
    while ( infile >> junk1 >> ch >> junk2 >> mean )
    {
      if ( evt%1000 == 1 )
      {
        std::cout << "evt " << evt << "\tch " << ifeech << std::endl;
      }

      for (int isamp=0; isamp<NSAMPLES; isamp++)
      {
        infile >> yval[isamp];
        g_subpulse->SetPoint( isamp, isamp, yval[isamp] - mean);
        g_subpulse->SetPointError( isamp, 0., 5.);
      }

      // find those tail only events with high enough amplitude
      if ( (fabs(yval[12]-mean) < pedsigma*3) && ((yval[0]-mean) > 200) )
      {
        fit_tail( g_subpulse, ifeech );

        // fill histogram
        for (int isamp=0; isamp<NSAMPLES; isamp++)
        {
          h2_residuals[ifeech]->Fill( isamp, (yval[isamp]-mean) - fit[ifeech]->Eval(isamp) );
          /*
          if ( fabs((yval[isamp]-mean) - fit[ifeech]->Eval(isamp)) > 100 )
          {
            std::cout << "xxx " << isamp << "\t" << yval[isamp]-mean << "\t" << fit[ifeech]->Eval(isamp) << std::endl;
            isbad = 1;
          }
          */
        }
      }

      evt++;
    }

    infile.close();
  }

  fit_time_pileupcorr();

  savefile->Write();
}


