//
// Do a recalibration of the slew from the saved histograms
//
#include <iostream>
#include <fstream>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TString.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TSystem.h>
#include <mbd/MbdCalib.h>
#include <mbd/MbdDefs.h>
#include <mbd/MbdGeomV1.h>
#include "get_runstr.h"

#if defined(__CLING__)
R__LOAD_LIBRARY(libmbd_io.so)
#endif

const int NPOINTS = 16000; // number of points in correction LUT
const int MINADC = 0;       // subtracted adc
const int MAXADC = 15999;

int verbose = 0;

TGraphErrors *g_slew[MbdDefs::MBD_N_PMT];
TF1 *f_slewfit[MbdDefs::MBD_N_PMT];

// find the ridge of the TH2
TGraphErrors *find_th2ridge(const TH2* h2)
{
  int nbinsx = h2->GetNbinsX();
  int nbinsy = h2->GetNbinsY();
  double min_xrange = h2->GetXaxis()->GetBinLowEdge(1);
  double max_xrange = h2->GetXaxis()->GetBinLowEdge(nbinsx+1);
  double min_yrange = h2->GetYaxis()->GetBinLowEdge(1);
  double max_yrange = h2->GetYaxis()->GetBinLowEdge(nbinsy+1);

  TString name;
  TString title;
  name = "aaa";
  title = "aaa";
  //TH1D prof(name,title,nbinsx,min_xrange,max_xrange);
  TGraphErrors *prof = new TGraphErrors();
  prof->SetName(name);
  prof->SetTitle(title);

  TH1 *h_projx = h2->ProjectionX("projx");
  //std::unique_ptr<TH2D> h_projx( h2->ProjectionX("projx") );

  TF1 gaussian("gaussian","gaus",min_yrange,max_yrange);
  gaussian.SetLineColor(4);

  TH1 *h_projy{nullptr};
  double adcmean = 0.;
  double adcnum = 0.;

  for (int ibin=1; ibin<=nbinsx; ibin++)
  {
    name = "hproj_"; name += ibin;
    if ( h_projy==nullptr )
    {
      h_projy = h2->ProjectionY(name,ibin,ibin);
      adcmean = h_projx->GetBinCenter(ibin);
      adcnum = 1.0;
    }
    else
    {
      TH1 *h_projyadd =  h2->ProjectionY(name,ibin,ibin);
      h_projy->Add( h_projyadd );
      delete h_projyadd;

      adcmean += h_projx->GetBinCenter(ibin);
      adcnum += 1.0;
    }


    if ( h_projy->Integral()>2000 || ibin==nbinsx )
    {
      adcmean = adcmean/adcnum;

      h_projy->Draw();

      int maxbin = h_projy->GetMaximumBin();
      double xmax = h_projy->GetBinCenter( maxbin );
      double ymax = h_projy->GetBinContent( maxbin );
      gaussian.SetParameter(1,xmax);
      gaussian.SetParameter(0,ymax);
      gaussian.SetRange(xmax-0.6,xmax+0.6);

      h_projy->Fit("gaussian","RWW");

      double mean = gaussian.GetParameter(1);
      double meanerr = gaussian.GetParError(1);
      if ( meanerr<1.0 )
      {
        int n = prof->GetN();
        prof->SetPoint(n,adcmean,mean);
        prof->SetPointError(n,0,meanerr);
      }

      gPad->Modified();
      gPad->Update();
      /*
         string junk;
         cin >> junk;
         */

      delete h_projy;
      h_projy = nullptr;
    }

  }

  // interpolate last point out to ADC = 16000
  int n = prof->GetN();
  double x1, x2, y1, y2;
  prof->GetPoint(n-2,x1,y1);
  prof->GetPoint(n-1,x2,y2);

  delete h_projx;

  prof->SetBit(TGraph::kIsSortedX);
  return prof;
}

//
// pass: should be the same as cal_mbd pass number
//
//need to fix run number issue...
void recal_mbd_slew(const char *tfname = "calmbdslew_pass1-54321.root", const int pass = 1, const int nevt = 0)
{
  cout << "tfname " << tfname << endl;
  MbdGeom *mbdgeom = new MbdGeomV1();

  // Read in TFile with h_q
  TFile *oldfile = new TFile(tfname,"READ");

  TH2 *h2_slew[MbdDefs::MBD_N_FEECH] = {};

  TString name;
  TString title;
  for (int ipmt=0; ipmt<MbdDefs::MBD_N_PMT; ipmt++)
  {
    int feech = (ipmt / 8) * 16 + ipmt % 8;
    name = "h2_slew"; name += ipmt;
    cout << name << "\t" << feech << endl;
    h2_slew[feech] = (TH2*)oldfile->Get(name);
    if ( h2_slew[feech]==0 ) 
    {
      cout << "ERROR, " << name << " not found in " << tfname << endl;
      return;
    }
  }

  // Create new TFile
  TString dir = "results/";
  dir += get_runnumber(tfname);
  dir += "/";
  name = "mkdir -p "; name += dir;
  gSystem->Exec( name );
  name = dir; 
  name += "recalmbdslew_pass2."; name += pass; name += ".root";
  cout << name << endl;

  TFile *savefile = new TFile(name,"RECREATE");

  TString pdfname = name; pdfname.ReplaceAll(".root",".pdf");

  // Load in calib constants

  TCanvas *ac[100];
  int cvindex = 0;

  // slew curves
  ac[cvindex] = new TCanvas("cal_slew","slew",425*1.5,550*1.5);
  ac[cvindex]->Print( pdfname + "[" );

  for (int ifeech=0; ifeech<MbdDefs::MBD_N_FEECH; ifeech++)
  {
    if ( mbdgeom->get_type(ifeech) == 1 ) continue;  // skip q-channels

    int pmtch = mbdgeom->get_pmt(ifeech);

    /*
    // Use LUT instead of fit
    name = "f_slewfit"; name += pmtch;
    f_slewfit[pmtch] = new TF1(name,"[0]+([1]/x)+[2]*log(x)",20,16000);
    f_slewfit[pmtch]->SetParameters(4.0,1.,1.);
    f_slewfit[pmtch]->SetLineColor(2);
    */

    //h2_slew[ifeech]->RebinX(20);
    //h2_slew[ifeech]->RebinY(10);
    name = "g_slew"; name += pmtch;
    cout << name << endl;
    g_slew[pmtch] = find_th2ridge( h2_slew[ifeech] );
    g_slew[pmtch]->SetName(name);
    g_slew[pmtch]->SetMarkerStyle(20);
    g_slew[pmtch]->SetMarkerSize(0.25);

    ac[cvindex]->cd();
    h2_slew[ifeech]->Draw("colz");
    g_slew[pmtch]->Draw("cp");

    /*
       g_slew[pmtch]->Fit( f_slewfit[pmtch], "R" );
       f_slewfit[pmtch]->DrawCopy("same");
       double par1 = f_slewfit[pmtch]->GetParameter(1);
       double par2 = f_slewfit[pmtch]->GetParameter(2);
       f_slewfit[pmtch]->SetParameter(1,0.);
       f_slewfit[pmtch]->SetLineColor(6);
       f_slewfit[pmtch]->DrawCopy("same");
       f_slewfit[pmtch]->SetParameter(1,par1);
       f_slewfit[pmtch]->SetParameter(2,0.);
       f_slewfit[pmtch]->SetLineColor(4);
       f_slewfit[pmtch]->DrawCopy("same");
    */

    gPad->Modified();
    gPad->Update();

    if ( verbose>10 )
    {
      string junk;
      cout << "? ";
      cin >> junk;
    }

    //name = dir + "/h2_slewfit"; name += pmtch;
    //name += "_pass"; name += pass; name += ".png";
    name = "h2_slewfit"; name += pmtch;
    name += "_pass"; name += pass;
    cout << name << endl;
    ac[cvindex]->Print( pdfname, name );

  }
  ac[cvindex]->Print( pdfname + "]" );

  ++cvindex;


  // Write out slew curves to temp calib file
  TString scorr_fname = dir; scorr_fname += "/pass"; scorr_fname += pass;
  scorr_fname += "_mbd_slewcorr.calib";
  cout << scorr_fname << endl;
  ofstream scorr_file( scorr_fname );
  for (int ifeech=0; ifeech<MbdDefs::MBD_N_FEECH; ifeech++)
  {
    if ( mbdgeom->get_type(ifeech) == 1 ) continue;  // skip q-channels
    int pmtch = mbdgeom->get_pmt(ifeech);

    scorr_file << ifeech << "\t" << NPOINTS << "\t" << MINADC << "\t" << MAXADC << endl;
    int step = (MAXADC-MINADC)/(NPOINTS-1);
    //cout << "STEP " << step << endl;
    for (int iadc=MINADC; iadc<=MAXADC; iadc+=step)
    {
      float slewcorr = g_slew[pmtch]->Eval(iadc);
      scorr_file << slewcorr << " ";
      if ( iadc%10 == 9 ) scorr_file << endl;
    }
  }
  scorr_file.close();

  if ( pass>0 )
  {
    // write out the slew curves
    for (int ipmt=0; ipmt<128; ipmt++)
    {
      g_slew[ipmt]->Write();
    }
  }
  savefile->Write();
  //savefile->Close();
}

