//
// make plots of the ped calibs to check them
//
//#include "get_runstr.h"
#include <TFile.h>
#include <TTree.h>
#include <TGraphErrors.h>
#include <TString.h>
#include <TCanvas.h>
#include <TPad.h>

#include <iostream>
#include <fstream>

const int NFEECH = 256;
const int MAXRUNS = 10000;

Double_t mean[NFEECH][MAXRUNS];
Double_t meanerr[NFEECH][MAXRUNS];
Double_t sigma[NFEECH][MAXRUNS];
Double_t sigmaerr[NFEECH][MAXRUNS];
Double_t runs[MAXRUNS];

TGraphErrors *g_mean[NFEECH];
TGraphErrors *g_sigma[NFEECH];

void plot_ped(const char *fname = "results/ped.list")
{
  // Make a TFile for easier analysis
  TString name;
  //TFile *savefile = new TFile("peds.root");


  Float_t temp_mean;
  Float_t temp_meanerr;
  Float_t temp_sigma;
  Float_t temp_sigmaerr;
  Int_t   temp_feech;

  TString rootfname;
  TString runtext;
  TFile *cdbfile[MAXRUNS];

  std::ifstream inflist;
  inflist.open( fname );

  int nruns = 0;

  while  ( inflist >> rootfname )
  {
    cdbfile[nruns] = new TFile(rootfname,"READ");

    // get run number
    runtext = rootfname;
    runtext.ReplaceAll("/mbd_ped.root","");
    runtext.ReplaceAll("results/","");
    runs[nruns] = runtext.Atof();
    std::cout << rootfname << ", run " << runs[nruns] << std::endl;

    TTree *pedtree = (TTree*)cdbfile[nruns]->Get("Multiple");
    pedtree->SetBranchAddress("Fpedmean",&temp_mean);
    pedtree->SetBranchAddress("Fpedmeanerr",&temp_meanerr);
    pedtree->SetBranchAddress("Fpedsigma",&temp_sigma);
    pedtree->SetBranchAddress("Fpedsigmaerr",&temp_sigmaerr);
    pedtree->SetBranchAddress("IID",&temp_feech);

    Stat_t nentries = pedtree->GetEntries();
    //std::cout << nentries << std::endl;
    for (Stat_t ientry=0; ientry<nentries; ientry++)
    {
      pedtree->GetEntry( ientry );

      //if (nruns==0 ) std::cout << temp_feech << std::endl;
      if ( temp_feech<0 || temp_feech>255 )
      {
        std::cout << "ERROR feech " << temp_feech << std::endl;
        continue;
      }
      mean[temp_feech][nruns] = temp_mean;
      meanerr[temp_feech][nruns] = temp_meanerr;
      sigma[temp_feech][nruns] = temp_sigma;
      sigmaerr[temp_feech][nruns] = temp_sigmaerr;
    }

    //delete cdbfile[nruns];
    nruns++;
  }

  std::cout << "Processed " << nruns << " runs" << NFEECH << std::endl;

  // Make the plots
  for (int ifeech=0; ifeech<NFEECH; ifeech++)
  {
    g_mean[ifeech] = new TGraphErrors(nruns,runs,mean[ifeech],nullptr,meanerr[ifeech]);
    name = "g_mean"; name += ifeech;
    g_mean[ifeech]->SetName( name );
    g_mean[ifeech]->SetTitle( name );
    //g_mean[ifeech]->SetMarkerStyle( 20 );

    g_sigma[ifeech] = new TGraphErrors(nruns,runs,sigma[ifeech],nullptr,sigmaerr[ifeech]);
    name = "g_sigma"; name += ifeech;
    g_sigma[ifeech]->SetName( name );
    g_sigma[ifeech]->SetTitle( name );
  }

  TCanvas *ac[100];
  ac[0] = new TCanvas("cped","ped",1200,900);
  ac[0]->Divide(1,2);

  ac[0]->Print("ped.pdf[");
  for (int ifeech=0; ifeech<NFEECH; ifeech++)
  {
    std::cout << "feech " << ifeech << std::endl;
    ac[0]->cd(1);
    g_mean[ifeech]->Draw("alp");
    gPad->Modified();
    gPad->Update();

    ac[0]->cd(2);
    g_sigma[ifeech]->Draw("alp");
    gPad->Modified();
    gPad->Update();

    name = "ped, ch"; name += ifeech;
    ac[0]->Print("ped.pdf",name);
    //sleep(1);
  }
  ac[0]->Print("ped.pdf]");

}
