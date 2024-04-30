//
// make the timecorr calib files
// input is the mbdtimecalib.C output
//
#include <iostream>
#include <MbdCalib.h>
#include <MbdDefs.h>
#include <MbdGeomV1.h>
#include "get_runstr.h"
//#include <GausProc.h>
#include <vector>
#include <fstream>

#if defined(__CLING__)
R__LOAD_LIBRARY(libmbd_io.so)
//R__LOAD_LIBRARY(libGausProc.so)
#endif

const int NPOINTS = 1000; // number of points in correction LUT
Int_t _feech[MbdDefs::MBD_N_FEECH];
Int_t _npts[MbdDefs::MBD_N_FEECH];
Double_t _mintdc[MbdDefs::MBD_N_FEECH];    // subtracted tdc
Double_t _maxtdc[MbdDefs::MBD_N_FEECH];
Double_t _mintime[MbdDefs::MBD_N_FEECH];    // min time in range
Double_t _maxtime[MbdDefs::MBD_N_FEECH];    // max time in range
Double_t _timecorr[MbdDefs::MBD_N_FEECH][NPOINTS];

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
  _feech[ifeech] = ifeech;
  _npts[ifeech] = NPOINTS;
  _mintdc[ifeech] = 0.;
  _maxtdc[ifeech] = 16000.;

  // set up GPR to do interpolation
  // convert tp data to vectors
  vector<Double_t> vx( x, x + n );
  vector<Double_t> vy( y, y + n );
  vector<Double_t> vey( ey, ey + n );
  //std::fill(vey.begin(), vey.end(), 0.003);     // temporary

  for (int i=100; i<110; i++)
  {
    cout << "vvv " << i << "\t" << n << "\t" << vx[i] << "\t" << vy[i] << "\t" << vey[i] << endl;
  }

  unsigned int nPredictions = NPOINTS;
  Double_t xmin = _mintdc[ifeech];
  Double_t xmax = _maxtdc[ifeech];
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

  cout << "gpopt pars " << gpopt.getPar(0) << " " << gpopt.getPar(1) << endl;  
  
  GausProc gp_result(vx, vy, vey, xmin, xmax, nPredictions, outfile);
  gp_result.SetPar(0,gpopt.getPar(0));
  gp_result.SetPar(1,gpopt.getPar(1));
  gp_result.process();
  gp_result.Write(-1);
 
  // Get the resulting histogram
  TFile fin(outfile,"READ");
  TH1F *ho = (TH1F*)fin.Get("ho");
 
  int nbinsx = ho->GetNbinsX();
  _feech[ifeech] = ifeech;
  _npts[ifeech] = NPOINTS;
  _mintdc[ifeech] = 0.;
  _maxtdc[ifeech] = 16000.;
  cout << _feech[ifeech] << "\t" << _npts[ifeech] << "\t" << _mintdc[ifeech] << "\t" << _maxtdc[ifeech] << endl;
  cout << nbinsx << endl;
  for (int ibin=1; ibin<=5; ibin++)
  {
    cout << ibin << " ";
    cout << ho->GetBinCenter(ibin) << " ";
    cout << ho->GetBinContent(ibin) << " ";
    //if ( ibin%10 == 0 ) cout << endl;
    cout << endl;
  }
  for (int ibin=nbinsx-5; ibin<=nbinsx; ibin++)
  {
    cout << ibin << " ";
    cout << ho->GetBinCenter(ibin) << " ";
    cout << ho->GetBinContent(ibin) << " ";
    //if ( ibin%10 == 0 ) cout << endl;
    cout << endl;
  }

  fin.Close();

}
*/

// use poly fit to interpolate
void interpolate_timecorr( const int ifeech, TGraphErrors *g, TF1 *f )
{
  Int_t n = g->GetN();
  Double_t *x = g->GetX();
  Double_t *y = g->GetY();

  g->Fit(f,"R");

  g->Draw("ap");
  gPad->Modified();
  gPad->Update();

  float mintime = TMath::MinElement( n, y );
  float maxtime = TMath::MaxElement( n, y );
  float mintdc = TMath::MinElement( n, x );
  float maxtdc = TMath::MaxElement( n, x );
  cout << ifeech << "\t" << mintime << "\t" << maxtime << "\t" << maxtime - mintime << endl;
  cout << "tdc\t" << mintdc << "\t" << maxtdc << endl;

  // parameters for calibrations output
  _feech[ifeech] = ifeech;
  _npts[ifeech] = NPOINTS;
  _mintdc[ifeech] = mintdc;
  _maxtdc[ifeech] = maxtdc;
  _mintime[ifeech] = mintime;
  _maxtime[ifeech] = maxtime;

  //cout << _feech[ifeech] << "\t" << _npts[ifeech] << "\t" << _mintdc[ifeech] << "\t" << _maxtdc[ifeech] << endl;
 
  //double dstep = (_maxtdc[ifeech] - _mintdc[ifeech])/(NPOINTS-1);
  //double tdc = _mintdc[ifeech];
  double dstep = 14985./(NPOINTS-1);
  double tdc = 0;
  for (int istep=0; istep<NPOINTS; istep++)
  {
    _timecorr[ifeech][istep] = f->Eval(tdc) - mintime;

    if ( istep==0 || istep==NPOINTS-1 )
    {
      cout << "ch " <<  ifeech << "\t" << istep << "\t" << tdc << "\t" << _timecorr[ifeech][istep] << endl;
    }

    tdc += dstep;
  }

}


void make_timecorr(const char *rootfname = "calib_seb18-00029705-0000_mbdtimecalib.root")
{
  MbdGeom *mbdgeom = new MbdGeomV1();
  //MbdCalib *mcal = new MbdCalib();

  //== Create output directory (should already exist)
  TString dir = "results/";
  dir += get_runstr(rootfname);
  dir.ReplaceAll("_mbdtimecalib","");
  dir += "/";
  //TString name = "mkdir -p " + dir;
  TString name = "echo " + dir;
  gSystem->Exec( name );
  cout << name << endl;

  TF1 *fpoly = new TF1("fpoly","pol3",0,15000);
  fpoly->SetParameters(28.,-0.0018,0.,0.);
  fpoly->SetLineColor(4);
  TGraphErrors *g_delaytime[MbdDefs::MBD_N_FEECH]; // delay curves, corrected for time
  TFile *tfile = new TFile(rootfname,"READ");
  for (int ifeech=0; ifeech<MbdDefs::MBD_N_FEECH; ifeech++)
  {
    if ( mbdgeom->get_type(ifeech) == 1 ) continue;  // skip q-channels

    name = "g_delaytime"; name += ifeech;
    g_delaytime[ifeech] = (TGraphErrors*)tfile->Get(name);

    // do a prefit since for some reason the first fit fails
    if ( ifeech==0 )
    {
      g_delaytime[ifeech]->Fit(fpoly,"R");
    }

    interpolate_timecorr( ifeech, g_delaytime[ifeech], fpoly );

    //if ( ifeech>4 ) break;
  }

  // write out mbd_tpscan.txt file
  TString tscan_fname = dir; tscan_fname += "/mbd_tpscan.txt";
  cout << tscan_fname << endl;
  ofstream tscan_file( tscan_fname );
  for (int ifeech=0; ifeech<MbdDefs::MBD_N_FEECH; ifeech++)
  {
    if ( mbdgeom->get_type(ifeech) == 1 ) continue;  // skip q-channels

    tscan_file << _feech[ifeech] << "\t" << _mintdc[ifeech] << "\t" << _maxtdc[ifeech] << "\t"
      << _maxtdc[ifeech] - _mintdc[ifeech] << "\t"
      << _mintime[ifeech] << "\t" << _maxtime[ifeech] << "\t" << _maxtime[ifeech] - _mintime[ifeech] << endl;
  }
  tscan_file.close();


  // write out mbd_timecorr.calib file
  TString tcorr_fname = dir; tcorr_fname += "/mbd_timecorr.calib";
  cout << tcorr_fname << endl;
  ofstream tcorr_file( tcorr_fname );
  for (int ifeech=0; ifeech<MbdDefs::MBD_N_FEECH; ifeech++)
  {
    if ( mbdgeom->get_type(ifeech) == 1 ) continue;  // skip q-channels

    //tcorr_file << _feech[ifeech] << "\t" << _npts[ifeech] << "\t" << _mintdc[ifeech] << "\t" << _maxtdc[ifeech] << endl;
    tcorr_file << _feech[ifeech] << "\t" << _npts[ifeech] << "\t" << 0 << "\t" << 14985 << endl;
    for (int ipt=0; ipt<NPOINTS; ipt++)
    {
      tcorr_file << _timecorr[ifeech][ipt] << " ";
      if ( ipt%10 == 9 ) tcorr_file << endl;
    }
  }
  tcorr_file.close();
}

