//
// Do a recalibration of the slew from the saved histograms
//
#include "get_runstr.h"

#include <mbd/MbdCalib.h>
#include <mbd/MbdDefs.h>
#include <mbd/MbdGeomV2.h>

#include <TCanvas.h>
#include <TF1.h>
#include <TFile.h>
#include <TGraphErrors.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TString.h>
#include <TSystem.h>

#include <fstream>
#include <iostream>

R__LOAD_LIBRARY(libmbd_io.so)

const int NPOINTS = 16000;  // number of points in correction LUT
const int MINADC = 0;       // subtracted adc
const int MAXADC = 15999;

int verbose = 0;

TGraphErrors *g_slew[MbdDefs::MBD_N_PMT];     // slew curve
TGraphErrors *g_trms[MbdDefs::MBD_N_PMT];     // sgl ch time resol vs ADC
//TF1 *f_slewfit[MbdDefs::MBD_N_PMT];

// find the ridge of the TH2, return ridge and rms
void find_th2ridge(const TH2 *h2, TGraphErrors*& gridge, TGraphErrors*& grms)
{
  int nbinsx = h2->GetNbinsX();
  int nbinsy = h2->GetNbinsY();
  // double min_xrange = h2->GetXaxis()->GetBinLowEdge(1);
  // double max_xrange = h2->GetXaxis()->GetBinLowEdge(nbinsx+1);
  double min_yrange = h2->GetYaxis()->GetBinLowEdge(1);
  double max_yrange = h2->GetYaxis()->GetBinLowEdge(nbinsy + 1);

  TString name;
  TString title;
  name = "gridge";
  title = "ridge";
  // TH1D gridge(name,title,nbinsx,min_xrange,max_xrange);
  gridge = new TGraphErrors();
  gridge->SetName(name);
  gridge->SetTitle(title);

  name = "grms";
  title = "rms of ridge";
  grms = new TGraphErrors();
  grms->SetName(name);
  grms->SetTitle(title);

  TH1 *h_projx = h2->ProjectionX("projx");
  // std::unique_ptr<TH2D> h_projx( h2->ProjectionX("projx") );

  TF1 gaussian("gaussian", "gaus", min_yrange, max_yrange);
  gaussian.SetLineColor(4);

  TH1 *h_projy{nullptr};
  double adcmean = 0.;
  double adcnum = 0.;

  for (int ibin = 1; ibin <= nbinsx; ibin++)
  {
    name = "hproj_";
    name += ibin;
    if (h_projy == nullptr)
    {
      h_projy = h2->ProjectionY(name, ibin, ibin);
      adcmean = h_projx->GetBinCenter(ibin);
      adcnum = 1.0;
    }
    else
    {
      TH1 *h_projyadd = h2->ProjectionY(name, ibin, ibin);
      h_projy->Add(h_projyadd);
      delete h_projyadd;

      adcmean += h_projx->GetBinCenter(ibin);
      adcnum += 1.0;
    }

    if (h_projy->Integral() > 2000 || ibin == nbinsx)
    {
      adcmean = adcmean / adcnum;

      h_projy->Draw();

      int maxbin = h_projy->GetMaximumBin();
      double xmax = h_projy->GetBinCenter(maxbin);
      double ymax = h_projy->GetBinContent(maxbin);
      gaussian.SetParameter(1, xmax);
      gaussian.SetParameter(0, ymax);
      gaussian.SetRange(xmax - 0.6, xmax + 0.6);

      h_projy->Fit("gaussian", "RWW");

      double mean = gaussian.GetParameter(1);
      double meanerr = gaussian.GetParError(1);
      double rms = gaussian.GetParameter(2);
      double rmserr = gaussian.GetParError(2);
      if (meanerr < 1.0)
      {
        int n = gridge->GetN();
        gridge->SetPoint(n, adcmean, mean);
        gridge->SetPointError(n, 0, meanerr);
      }

      if (rmserr < 0.01)
      {
        int n = grms->GetN();
        grms->SetPoint(n, adcmean, rms);
        grms->SetPointError(n, 0, rmserr);
      }

      gPad->Modified();
      gPad->Update();
      /*
         std::string junk;
         std::cin >> junk;
         */

      delete h_projy;
      h_projy = nullptr;
    }
  }

  // interpolate last point out to ADC = 16000
  int n = gridge->GetN();
  double x1;
  double x2;
  double y1;
  double y2;
  gridge->GetPoint(n - 2, x1, y1);
  gridge->GetPoint(n - 1, x2, y2);

  delete h_projx;

  gridge->SetBit(TGraph::kIsSortedX);
  grms->SetBit(TGraph::kIsSortedX);

}

//
// pass: should be the same as cal_mbd pass number
//
// need to fix run number issue...
//void recal_mbd_slew(const char *tfname = "calmbdslew_pass1-54321.root", const int pass = 1, const int /*nevt*/ = 0)
void recal_mbd_slew(const char *tfname = "calmbdslew_pass1-54321.root", const int pass = 1)
{
  std::cout << "tfname " << tfname << std::endl;
  MbdGeom *mbdgeom = new MbdGeomV2();

  // Read in TFile with h_q
  TFile *oldfile = new TFile(tfname, "READ");

  TH2 *h2_slew[MbdDefs::MBD_N_FEECH] = {};

  TString name;
  TString title;
  for (int ipmt = 0; ipmt < MbdDefs::MBD_N_PMT; ipmt++)
  {
    //int feech = (ipmt / 8) * 16 + ipmt % 8;
    int feech = mbdgeom->get_feech(ipmt,0);   // feech for time ch
    name = "h2_slew";
    name += ipmt;
    std::cout << name << "\t" << feech << std::endl;
    h2_slew[feech] = (TH2 *) oldfile->Get(name);
    if (h2_slew[feech] == nullptr)
    {
      std::cout << "ERROR, " << name << " not found in " << tfname << std::endl;
      return;
    }
  }

  // Create new TFile
  TString dir = "results/";
  dir += get_runnumber(tfname);
  dir += "/";
  name = "mkdir -p ";
  name += dir;
  gSystem->Exec(name);
  name = dir;
  name += "recalmbdslew_pass2.";
  name += pass;
  name += ".root";
  std::cout << name << std::endl;

  TFile *savefile = new TFile(name, "RECREATE");

  TString pdfname = name;
  pdfname.ReplaceAll(".root", ".pdf");

  // Load in calib constants

  TCanvas *ac[100];
  int cvindex = 0;

  // slew curves
  ac[cvindex] = new TCanvas("cal_slew", "slew", 425 * 1.5, 550 * 1.5);
  ac[cvindex]->Print(pdfname + "[");

  for (int ifeech = 0; ifeech < MbdDefs::MBD_N_FEECH; ifeech++)
  {
    if (mbdgeom->get_type(ifeech) == 1)
    {
      continue;  // skip q-channels
    }

    int pmtch = mbdgeom->get_pmt(ifeech);

    /*
    // Use LUT instead of fit
    name = "f_slewfit"; name += pmtch;
    f_slewfit[pmtch] = new TF1(name,"[0]+([1]/x)+[2]*log(x)",20,16000);
    f_slewfit[pmtch]->SetParameters(4.0,1.,1.);
    f_slewfit[pmtch]->SetLineColor(2);
    */

    // h2_slew[ifeech]->RebinX(20);
    // h2_slew[ifeech]->RebinY(10);
    name = "g_slew"; name += pmtch;
    std::cout << name << std::endl;
    find_th2ridge(h2_slew[ifeech],g_slew[pmtch],g_trms[pmtch]);
    g_slew[pmtch]->SetName(name);
    g_slew[pmtch]->SetMarkerStyle(20);
    g_slew[pmtch]->SetMarkerSize(0.25);

    name = "g_trms"; name += pmtch;
    g_trms[pmtch]->SetName(name);
    g_trms[pmtch]->SetMarkerStyle(20);
    g_trms[pmtch]->SetMarkerSize(0.25);

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

    if (verbose > 10)
    {
      std::string junk;
      std::cout << "? ";
      std::cin >> junk;
    }

    // name = dir + "/h2_slewfit"; name += pmtch;
    // name += "_pass"; name += pass; name += ".png";
    name = "h2_slewfit";
    name += pmtch;
    name += "_pass";
    name += pass;
    std::cout << name << std::endl;
    ac[cvindex]->Print(pdfname, name);
  }
  ac[cvindex]->Print(pdfname + "]");
  ++cvindex;

  // trms curves
  pdfname = name;
  pdfname.ReplaceAll("slew", "trms");
  ac[cvindex] = new TCanvas("cal_trms", "trms", 425 * 1.5, 550 * 1.5);
  ac[cvindex]->Print(pdfname + "[");

  for (int ipmtch = 0; ipmtch < MbdDefs::MBD_N_PMT; ipmtch++)
  {
    ac[cvindex]->cd();
    g_trms[ipmtch]->Draw("cp");

    gPad->Modified();
    gPad->Update();

    if (verbose > 10)
    {
      std::string junk;
      std::cout << ipmtch << " ? ";
      std::cin >> junk;
    }

    name = "trms";
    name += ipmtch;
    name += "_pass";
    name += pass;
    std::cout << name << std::endl;
    ac[cvindex]->Print(pdfname, name);
  }
  ac[cvindex]->Print(pdfname + "]");
  cvindex++;

  // Write out slew curves to temp calib file
  TString scorr_fname = dir;
  scorr_fname += "/pass";
  scorr_fname += pass;
  scorr_fname += "_mbd_slewcorr.calib";
  std::cout << scorr_fname << std::endl;
  std::ofstream scorr_file(scorr_fname);
  for (int ifeech = 0; ifeech < MbdDefs::MBD_N_FEECH; ifeech++)
  {
    if (mbdgeom->get_type(ifeech) == 1)
    {
      continue;  // skip q-channels
    }
    int pmtch = mbdgeom->get_pmt(ifeech);

    scorr_file << ifeech << "\t" << NPOINTS << "\t" << MINADC << "\t" << MAXADC << std::endl;
    int step = (MAXADC - MINADC) / (NPOINTS - 1);
    // std::cout << "STEP " << step << std::endl;
    for (int iadc = MINADC; iadc <= MAXADC; iadc += step)
    {
      float slewcorr = g_slew[pmtch]->Eval(iadc);
      scorr_file << slewcorr << " ";
      if (iadc % 10 == 9)
      {
        scorr_file << std::endl;
      }
    }
  }
  scorr_file.close();

  // Write out time rms to temp calib file
  TString trms_fname = dir;
  trms_fname += "/pass";
  trms_fname += pass;
  trms_fname += "_mbd_timerms.calib";
  std::cout << trms_fname << std::endl;
  std::ofstream trms_file(trms_fname);
  for (int ifeech = 0; ifeech < MbdDefs::MBD_N_FEECH; ifeech++)
  {
    if (mbdgeom->get_type(ifeech) == 1)
    {
      continue;  // skip q-channels
    }
    int pmtch = mbdgeom->get_pmt(ifeech);

    trms_file << ifeech << "\t" << NPOINTS << "\t" << MINADC << "\t" << MAXADC << std::endl;
    int step = (MAXADC - MINADC) / (NPOINTS - 1);
    // std::cout << "STEP " << step << std::endl;
    for (int iadc = MINADC; iadc <= MAXADC; iadc += step)
    {
      float trms = g_trms[pmtch]->Eval(iadc);
      trms_file << trms << " ";
      if (iadc % 10 == 9)
      {
        trms_file << std::endl;
      }
    }
  }
  trms_file.close();

  if (pass > 0)
  {
    // write out the slew curves
    for (auto &ipmt : g_slew)
    {
      ipmt->Write();
    }
    // write out the trms curves
    for (auto &ipmt : g_trms)
    {
      ipmt->Write();
    }
  }
  savefile->Write();
  // savefile->Close();
}
