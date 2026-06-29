//
// make the timecorr calib files
// input is the mbdtimecalib.C output
//
#include "get_runstr.h"

#include <mbd/MbdCalib.h>
#include <mbd/MbdDefs.h>
#include <mbd/MbdGeomV1.h>

#include <TF1.h>
#include <TFile.h>
#include <TGraphErrors.h>
#include <TPad.h>
#include <TSystem.h>

#include <iostream>
// #include <GausProc.h>
#include <fstream>
#include <vector>

R__LOAD_LIBRARY(libmbd_io.so)

const int NPOINTS = 1000;  // number of points in correction LUT
Int_t feech[MbdDefs::MBD_N_FEECH];
Int_t npts[MbdDefs::MBD_N_FEECH];
Double_t mintdc[MbdDefs::MBD_N_FEECH];  // subtracted tdc
Double_t maxtdc[MbdDefs::MBD_N_FEECH];
Double_t mintime[MbdDefs::MBD_N_FEECH];  // min time in range
Double_t maxtime[MbdDefs::MBD_N_FEECH];  // max time in range
Double_t timecorr[MbdDefs::MBD_N_FEECH][NPOINTS];

/*
void gpr_interpolate_timecorr( const int ifeech, TGraphErrors *g )
{
  Int_t n = g->GetN();
  Double_t *x = g->GetX();
  Double_t *y = g->GetY();
  Double_t *ey = g->GetEY();
  g->Print("ALL");
  g->SetMarkerStyle(20);
  g->SetMarkerSize(0);
  g->Draw("ap");

  // parameters for calibrations output
  feech[ifeech] = ifeech;
  npts[ifeech] = NPOINTS;
  mintdc[ifeech] = 0.;
  maxtdc[ifeech] = 16000.;

  // set up GPR to do interpolation
  // convert tp data to std::vectors
  std::vector<Double_t> vx( x, x + n );
  std::vector<Double_t> vy( y, y + n );
  std::vector<Double_t> vey( ey, ey + n );
  //std::fill(vey.begin(), vey.end(), 0.003);     // temporary

  for (int i=100; i<110; i++)
  {
    std::cout << "vvv " << i << "\t" << n << "\t" << vx[i] << "\t" << vy[i] << "\t" << vey[i] << std::endl;
  }

  unsigned int nPredictions = NPOINTS;
  Double_t xmin = mintdc[ifeech];
  Double_t xmax = maxtdc[ifeech];
  const char *outfile = "null.root";
  gSystem->Exec(Form("rm -f %s",outfile));

  GausProc gp(vx, vy, vey, xmin, xmax, nPredictions, outfile);
  int verbosity = 1;
  gp.SetVerbosity(verbosity);
  gp.SetKernel(GausProc::RBF);

  const int npar = 2;
  const int nparfixed = 0;
  // 2,10 = par[0],par[1], which are the start points for the minimizer
  GPOptimizer gpopt(&gp,2,10);
  gpopt.GPoptimize(npar,nparfixed);

  std::cout << "gpopt pars " << gpopt.getPar(0) << " " << gpopt.getPar(1) << std::endl;

  GausProc gp_result(vx, vy, vey, xmin, xmax, nPredictions, outfile);
  gp_result.SetPar(0,gpopt.getPar(0));
  gp_result.SetPar(1,gpopt.getPar(1));
  gp_result.process();
  gp_result.Write(-1);

  // Get the resulting histogram
  TFile fin(outfile,"READ");
  TH1F *ho = (TH1F*)fin.Get("ho");

  int nbinsx = ho->GetNbinsX();
  feech[ifeech] = ifeech;
  npts[ifeech] = NPOINTS;
  mintdc[ifeech] = 0.;
  maxtdc[ifeech] = 16000.;
  std::cout << feech[ifeech] << "\t" << npts[ifeech] << "\t" << mintdc[ifeech] << "\t" << maxtdc[ifeech] << std::endl;
  std::cout << nbinsx << std::endl;
  for (int ibin=1; ibin<=5; ibin++)
  {
    std::cout << ibin << " ";
    std::cout << ho->GetBinCenter(ibin) << " ";
    std::cout << ho->GetBinContent(ibin) << " ";
    //if ( ibin%10 == 0 ) std::cout << std::endl;
    std::cout << std::endl;
  }
  for (int ibin=nbinsx-5; ibin<=nbinsx; ibin++)
  {
    std::cout << ibin << " ";
    std::cout << ho->GetBinCenter(ibin) << " ";
    std::cout << ho->GetBinContent(ibin) << " ";
    //if ( ibin%10 == 0 ) std::cout << std::endl;
    std::cout << std::endl;
  }

  fin.Close();

}
*/

// use poly fit to interpolate
// now using spline fit
void interpolate_timecorr(const int ifeech, TGraphErrors *g, TF1 *f)
{
  Int_t n = g->GetN();
  Double_t *x = g->GetX();
  Double_t *y = g->GetY();

  g->Fit(f, "R");

  g->Draw("ap");
  gPad->Modified();
  gPad->Update();

  float mintimelocal = TMath::MinElement(n, y);
  float maxtimelocal = TMath::MaxElement(n, y);
  float mintdclocal = TMath::MinElement(n, x);
  float maxtdclocal = TMath::MaxElement(n, x);
  std::cout << ifeech << "\t" << mintimelocal << "\t" << maxtimelocal << "\t" << maxtimelocal - mintimelocal << std::endl;
  std::cout << "tdc\t" << mintdclocal << "\t" << maxtdclocal << std::endl;

  // parameters for calibrations output
  feech[ifeech] = ifeech;
  npts[ifeech] = NPOINTS;
  mintdc[ifeech] = mintdclocal;
  maxtdc[ifeech] = maxtdclocal;
  mintime[ifeech] = mintimelocal;
  maxtime[ifeech] = maxtimelocal;

  // std::cout << feech[ifeech] << "\t" << npts[ifeech] << "\t" << mintdc[ifeech] << "\t" << maxtdc[ifeech] << std::endl;

  // double dstep = (maxtdc[ifeech] - mintdc[ifeech])/(NPOINTS-1);
  // double tdc = mintdc[ifeech];
  double dstep = 14985. / (NPOINTS - 1);
  double tdc = 0;
  for (int istep = 0; istep < NPOINTS; istep++)
  {
    // timecorr[ifeech][istep] = f->Eval(tdc) - mintimelocal;
    timecorr[ifeech][istep] = g->Eval(tdc) - mintimelocal;

    if (istep == 0 || istep == NPOINTS - 1)
    {
      std::cout << "ch " << ifeech << "\t" << istep << "\t" << tdc << "\t" << timecorr[ifeech][istep] << std::endl;
    }

    tdc += dstep;
  }
}

void make_timecorr(const char *rootfname = "calib_seb18-00029705-0000_mbdtimecalib.root")
{
  MbdGeom *mbdgeom = new MbdGeomV1();
  // MbdCalib *mcal = new MbdCalib();

  //== Create output directory (should already exist)
  TString dir = "results/";
  dir += get_runstr(rootfname);
  dir.ReplaceAll("_mbdtimecalib", "");
  // dir.ReplaceAll("_mbdtrigtimecalib","");
  dir += "/";
  // TString name = "mkdir -p " + dir;
  TString name = "echo " + dir;
  gSystem->Exec(name);
  std::cout << name << std::endl;

  // TF1 *fpoly = new TF1("fpoly","pol3",0,15000);
  // fpoly->SetParameters(28.,-0.0018,0.,0.);
  TF1 *fpoly = new TF1("fpoly", "pol4", 0, 15000);
  fpoly->SetParameters(28., -0.0018, -1e-7, 1.3e-11, -5e-16);
  fpoly->SetLineColor(4);
  TGraphErrors *g_delaytime[MbdDefs::MBD_N_FEECH];  // delay curves, corrected for time
  TFile *tfile = new TFile(rootfname, "READ");
  for (int ifeech = 0; ifeech < MbdDefs::MBD_N_FEECH; ifeech++)
  {
    if (mbdgeom->get_type(ifeech) == 1)
    {
      continue;  // skip q-channels
    }

    name = "g_delaytime";
    name += ifeech;
    g_delaytime[ifeech] = (TGraphErrors *) tfile->Get(name);
    std::cout << name << std::endl;

    // do a prefit since for some reason the first fit fails
    if (ifeech == 0)
    {
      g_delaytime[ifeech]->Fit(fpoly, "R");
    }

    // if ( ifeech!=118)
    //{
    interpolate_timecorr(ifeech, g_delaytime[ifeech], fpoly);
    //}

    // if ( ifeech>4 ) break;
  }

  // write out mbd_tpscan.txt file
  TString tscan_fname = dir;
  tscan_fname += "/mbd_tpscan.txt";
  std::cout << tscan_fname << std::endl;
  std::ofstream tscan_file(tscan_fname);
  for (int ifeech = 0; ifeech < MbdDefs::MBD_N_FEECH; ifeech++)
  {
    if (mbdgeom->get_type(ifeech) == 1)
    {
      continue;  // skip q-channels
    }

    tscan_file << feech[ifeech] << "\t" << mintdc[ifeech] << "\t" << maxtdc[ifeech] << "\t"
               << maxtdc[ifeech] - mintdc[ifeech] << "\t"
               << mintime[ifeech] << "\t" << maxtime[ifeech] << "\t" << maxtime[ifeech] - mintime[ifeech] << std::endl;
  }
  tscan_file.close();

  // write out mbd_timecorr.calib file
  TString tcorr_fname = dir;
  tcorr_fname += "/mbd_timecorr.calib";
  // TString tcorr_fname = dir; tcorr_fname += "/mbd_trigtimecorr.calib";
  std::cout << "tcorr_fname " << tcorr_fname << std::endl;
  std::ofstream tcorr_file(tcorr_fname);
  for (int ifeech = 0; ifeech < MbdDefs::MBD_N_FEECH; ifeech++)
  {
    if (mbdgeom->get_type(ifeech) == 1)
    {
      continue;  // skip q-channels
    }

    // tcorr_file << feech[ifeech] << "\t" << npts[ifeech] << "\t" << mintdc[ifeech] << "\t" << maxtdc[ifeech] << std::endl;
    tcorr_file << feech[ifeech] << "\t" << npts[ifeech] << "\t" << 0 << "\t" << 14985 << std::endl;
    for (int ipt = 0; ipt < NPOINTS; ipt++)
    {
      tcorr_file << timecorr[ifeech][ipt] << " ";
      if (ipt % 10 == 9)
      {
        tcorr_file << std::endl;
      }
    }
  }
  tcorr_file.close();
}
