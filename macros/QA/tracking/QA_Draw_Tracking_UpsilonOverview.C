// $Id: $

/*!
 * \file QA_Draw_Tracking_UpsilonOverview.C
 * \brief 
 * \author Jin Huang <jhuang@bnl.gov>
 * \version $Revision:   $
 * \date $Date: $
 */

#include <TFile.h>
#include <TH2.h>
#include <TLine.h>
#include <TString.h>
#include <TTree.h>
#include <TVirtualFitter.h>
#include <cassert>
#include <cmath>

#include <TF1.h>
#include <TMath.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TGraph.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TROOT.h>
#include <TLatex.h>

//some common style files
#include "../../sPHENIXStyle/sPhenixStyle.C"
#include "QA_Draw_Utility.C"
using namespace std;

Double_t CBcalc(Double_t *xx, Double_t *par)
{
  // Crystal Ball fit to one state
  double f;
  double x = xx[0];

  // The four parameters (alpha, n, x_mean, sigma) plus normalization (N) are:
   
  double alpha = par[0];
  double n = par[1];
  double x_mean = par[2];
  double sigma = par[3];
  double N = par[4];

  // we need:

  double A = pow( (n/TMath::Abs(alpha)),n) * exp(-pow(alpha,2)/2.0);
  double B = n/TMath::Abs(alpha) - TMath::Abs(alpha);

  // The Crystal Ball function is:
   
  if( (x-x_mean)/sigma > -alpha)
  {
    f = N * exp( -pow(x-x_mean,2) / (2.0*pow(sigma,2)));
  }
  else
  {
    f = N * A * pow(B - (x-x_mean)/sigma, -n);
  }

  return f;
}



void QA_Draw_Tracking_UpsilonOverview(
    const char *hist_name_prefix = "QAG4SimulationUpsilon",
    const char *qa_file_name_new =
        "data/G4sPHENIX.root_qa.root",
    const char *qa_file_name_ref =
        "data/G4sPHENIX.root_qa.root")
{
  SetsPhenixStyle();

  TVirtualFitter::SetDefaultFitter("Minuit2");

  TString prefix = TString("h_") + hist_name_prefix + TString("_");

  TFile *qa_file_new = new TFile(qa_file_name_new);
  assert(qa_file_new->IsOpen());

  TFile *qa_file_ref = NULL;
  if (qa_file_name_ref)
  {
    qa_file_ref = new TFile(qa_file_name_ref);
    assert(qa_file_ref->IsOpen());
  }

  // obtain normalization
  double Nevent_new = 1;
  double Nevent_ref = 1;

  if (qa_file_new)
  {
    TH1 *h_norm = (TH1 *) qa_file_new->GetObjectChecked(
        prefix + TString("Normalization"), "TH1");
    assert(h_norm);

    Nevent_new = h_norm->GetBinContent(h_norm->GetXaxis()->FindBin("Event"));
  }
  if (qa_file_ref)
  {
    TH1 *h_norm = (TH1 *) qa_file_ref->GetObjectChecked(
        prefix + TString("Normalization"), "TH1");
    assert(h_norm);

    Nevent_ref = h_norm->GetBinContent(h_norm->GetXaxis()->FindBin("Event"));
  }


  TCanvas *c1 = new TCanvas(TString("QA_Draw_Tracking_UpsilonOverview") + TString("_") + hist_name_prefix,
                            TString("QA_Draw_Tracking_UpsilonOverview") + TString("_") + hist_name_prefix,
                            1800, 1000);
  c1->Divide(2, 1);
  int idx = 1;
  TPad *p;

  {
    p = (TPad *) c1->cd(idx++);
    c1->Update();
    p->SetLogy();

    TH2 *h_new = (TH2 *) qa_file_new->GetObjectChecked(
        prefix + TString("pTRecoGenRatio_pTGen"), "TH2");
    assert(h_new);

    //  h_new->Rebin(1, 2);
    h_new->Sumw2();
    //  h_new->Scale(1. / Nevent_new);

    TH2 *h_ref = NULL;
    if (qa_file_ref)
    {
      h_ref = (TH2 *) qa_file_ref->GetObjectChecked(
          prefix + TString("pTRecoGenRatio_pTGen"), "TH2");
      assert(h_ref);

      //    h_ref->Rebin(1, 2);
      h_ref->Sumw2();
      h_ref->Scale(Nevent_new / Nevent_ref);
    }

    TH1 *h_proj_new = h_new->ProjectionY(
        TString::Format(
            "%s_New_ProjX",
            h_new->GetName()));

    h_proj_new->GetXaxis()->SetRangeUser(0, 1.3);
    h_proj_new->SetTitle(TString(hist_name_prefix) + TString::Format(
                                                         ": Electron lineshape"));
    h_proj_new->GetXaxis()->SetTitle(TString::Format(
        "Reco p_{T}/Truth p_{T}"));
    
    TF1 *f_eLineshape = new TF1("f_eLineshape", CBcalc, 7, 11, 5);
    f_eLineshape->SetParameter(0, 1.0);
    f_eLineshape->SetParameter(1, 1.0);
    f_eLineshape->SetParameter(2, 0.95);
    f_eLineshape->SetParameter(3, 0.08);
    f_eLineshape->SetParameter(4, 20.0);

    f_eLineshape->SetParNames("alpha1S","n1S","m1S","sigma1S","N1S");
    f_eLineshape->SetLineColor(kRed);
    f_eLineshape->SetLineWidth(3);
    f_eLineshape->SetLineStyle(kSolid);
    f_eLineshape->SetNpx(1000);

    h_proj_new->Fit(f_eLineshape);

    TH1 *h_proj_ref = nullptr;
    if (h_ref)
    {
      h_proj_ref =
          h_ref->ProjectionY(
              TString::Format(
                  "%s_Ref_ProjX",
                  h_new->GetName()));
    }
    TF1 *f_eLineshape_ref = new TF1("f_eLineshape_ref", CBcalc, 7, 11, 5);
    f_eLineshape_ref->SetParameter(0, 1.0);
    f_eLineshape_ref->SetParameter(1, 1.0);
    f_eLineshape_ref->SetParameter(2, 0.95);
    f_eLineshape_ref->SetParameter(3, 0.08);
    f_eLineshape_ref->SetParameter(4, 20.0);

    f_eLineshape_ref->SetParNames("alpha1S","n1S","m1S","sigma1S","N1S");
    f_eLineshape_ref->SetLineColor(kRed);
    f_eLineshape_ref->SetLineWidth(3);
    f_eLineshape_ref->SetLineStyle(kSolid);

    h_proj_ref->Fit(f_eLineshape_ref);


    DrawReference(h_proj_new, h_proj_ref);
    f_eLineshape->Draw("same");

    char resstr_1[500];
    sprintf(resstr_1,"#sigma_{dp/p} = %.2f #pm %.2f %%", f_eLineshape->GetParameter(3)*100, f_eLineshape->GetParError(3)*100);
    TLatex *res_1 = new TLatex(0.2,0.75,resstr_1);
    res_1->SetNDC();
    res_1->SetTextSize(0.05);
    res_1->SetTextAlign(13);
    res_1->Draw();

    char resstr_2[500];
    sprintf(resstr_2,"#sigma_{dp/p,ref} = %.2f #pm %.2f %%", f_eLineshape_ref->GetParameter(3)*100, f_eLineshape_ref->GetParError(3)*100);
    TLatex *res_2 = new TLatex(0.2,0.7,resstr_2);
    res_2->SetNDC();
    res_2->SetTextSize(0.05);
    res_2->SetTextAlign(13);
    res_2->Draw();
  }

  {
    p = (TPad *) c1->cd(idx++);
    c1->Update();
//    p->SetLogy();

    TH1 *h_new = (TH1 *) qa_file_new->GetObjectChecked(
        prefix + TString("nReco_Pair_InvMassReco"), "TH1");
    assert(h_new);

    //    h_new->Rebin(2);
    h_new->Sumw2();
    //  h_new->Scale(1. / Nevent_new);

    TF1 *f1S = new TF1("f1S", CBcalc, 7, 11, 5);
    f1S->SetParameter(0, 1.0);
    f1S->SetParameter(1, 1.0);
    f1S->SetParameter(2, 9.46);
    f1S->SetParameter(3, 0.08);
    f1S->SetParameter(4, 50.0);

    f1S->SetParNames("alpha1S","n1S","m1S","sigma1S","N1S");
    f1S->SetLineColor(kRed);
    f1S->SetLineWidth(3);
    f1S->SetLineStyle(kSolid);
    f1S->SetNpx(1000);

    h_new->Fit(f1S);

    TH1 *h_ref = NULL;
    if (qa_file_ref)
    {
      h_ref = (TH1 *) qa_file_ref->GetObjectChecked(
          prefix + TString("nReco_Pair_InvMassReco"), "TH1");
      assert(h_ref);

      //      h_ref->Rebin(2);
      h_ref->Sumw2();
      //      h_ref->Scale(Nevent_new / Nevent_ref);
    }

    h_new->SetTitle(TString(hist_name_prefix) + TString::Format(
                                                    ": #Upsilon #rightarrow e^{+}e^{-} lineshape"));
    h_new->GetXaxis()->SetRangeUser(7, 10);

    TF1 *f1S_ref = new TF1("f1S_ref", CBcalc, 7, 11, 5);
    f1S_ref->SetParameter(0, 1.0);
    f1S_ref->SetParameter(1, 1.0);
    f1S_ref->SetParameter(2, 9.46);
    f1S_ref->SetParameter(3, 0.08);
    f1S_ref->SetParameter(4, 50.0);

    f1S_ref->SetParNames("alpha1S","n1S","m1S","sigma1S","N1S");
    f1S_ref->SetLineColor(kRed);
    f1S_ref->SetLineWidth(3);
    f1S_ref->SetLineStyle(kSolid);

    h_ref->Fit(f1S_ref);

    DrawReference(h_new, h_ref, false);
    f1S->Draw("same");

    // cout << "f1S pars " <<  f1S->GetParameter(3) << "   " << f1S->GetParError(3) << endl;

    char resstr_3[500];
    sprintf(resstr_3,"#sigma_{1S} = %.1f #pm %.1f MeV", f1S->GetParameter(3)*1000, f1S->GetParError(3)*1000);
    TLatex *res_3 = new TLatex(0.2,0.75,resstr_3);
    res_3->SetNDC();
    res_3->SetTextSize(0.05);
    res_3->SetTextAlign(13);
    res_3->Draw();

    char resstr_4[500];
    sprintf(resstr_4,"#sigma_{1S,ref} = %.1f #pm %.1f MeV", f1S_ref->GetParameter(3)*1000, f1S_ref->GetParError(3)*1000);
    TLatex *res_4 = new TLatex(0.2,0.7,resstr_4);
    res_4->SetNDC();
    res_4->SetTextSize(0.05);
    res_4->SetTextAlign(13);
    res_4->Draw();
  }

  SaveCanvas(c1, TString(qa_file_name_new) + TString("_") + TString(c1->GetName()), true);
}

/* 
void CBfitter_ups1s()
{
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  gStyle->SetOptTitle(0);

  TFile *file1S, *file2S, *file3S;
  TH1 *recomass1S, *recomass2S, *recomass3S;

  bool draw_gauss = false;

  // input root file containing "recomass" invariant mass histogram                                                                                                                                    
 
  file1S = new TFile("root_files/ntp_quarkonium_out.root");

  if(!file1S)
    {
      cout << "Failed to open file  " << endl;
      exit(1);
    }

  recomass1S = (TH1 *)file1S->Get("recomass0");

  TH1 *recomass = (TH1*)recomass1S->Clone("recomass1");
  if(!recomass)
    {
      cout << "Failed to get recomass histogram  " << endl;
      exit(1);
    }

  recomass->GetXaxis()->SetTitle("invariant mass (GeV/c^{2})");
  recomass->Sumw2();

  TCanvas *cups = new TCanvas("cups","cups",5,5,800,800);
  recomass->SetTitle("Y(1S,2S,3S) #rightarrow e^{+}e^{-}");
  recomass->SetMarkerStyle(20);
  recomass->SetMarkerSize(1);
  recomass->SetLineStyle(kSolid);
  recomass->SetLineWidth(2);
  recomass->DrawCopy("p");

  TF1 *f1S = new TF1("f1S",CBcalc,7,11,5);
  f1S->SetParameter(0, 1.0);     // alpha                                                                                                                                                             
   
  f1S->SetParameter(1, 1.0);      // n                                                                                                                                                               
   
  f1S->SetParameter(2, 9.46);      // xmean                                                                                                                                                           
   
  f1S->SetParameter(3, 0.08);     // sigma                                                                                                                                                            
   
  f1S->SetParameter(4, 2000.0);    // N                                                                                                                                                                  

  //f1S->SetParameter(4, 50.0);    // N                                                                                                                                                                  

  f1S->SetParNames("alpha1S","n1S","m1S","sigma1S","N1S");
  f1S->SetLineColor(kBlue);
  f1S->SetLineWidth(3);
  f1S->SetLineStyle(kDashed);

  recomass->Fit(f1S);
  f1S->Draw("same");
  cout << "f1S pars " <<  f1S->GetParameter(3) << "   " << f1S->GetParError(3) << endl;

  char resstr[500];
  sprintf(resstr,"#sigma_{1S} = %.1f #pm %.1f MeV", f1S->GetParameter(3)*1000, f1S->GetParError(3)*1000);
  TLatex *res = new TLatex(0.13,0.55,resstr);
  res->SetNDC();
  res->SetTextSize(0.05);
  res->Draw();


  double binw = recomass->GetBinWidth(1);
  double renorm = 1.0/binw;   // (1 / (bin_width of data in GeV) )                                                                                                                                    
   
  cout << "renorm = " << renorm << endl;

  cout << "Area of f1S is " << renorm * f1S->Integral(7,11) << endl;

  // Extract ratio of yield in central gaussian to total                                                                                                                                              
   

  TF1 *fgauss = new TF1("fgauss","gaus(0)",7,11);
  fgauss->SetParameter(0, f1S->GetParameter(4));
  fgauss->SetParameter(1, f1S->GetParameter(2));
  fgauss->SetParameter(2, f1S->GetParameter(3));
  fgauss->SetLineColor(kRed);
  if(draw_gauss) fgauss->Draw("same");

  // calculate fraction of yield in gaussian                                                                                                                                                          
   
  double area_fgauss =  fgauss->Integral(7,11) * renorm;
  double area_f1S = f1S->Integral(7,11) * renorm;
  double fraction = area_fgauss / area_f1S;


  cout << "Parameters of fgauss = " << fgauss->GetParameter(0) << "  " << fgauss->GetParameter(1) << "  " << fgauss->GetParameter(2) << " Area of fgauss is " << renorm * fgauss->Integral(7,11) << " fraction in fgauss " << area_fgauss / area_f1S << endl;

  char labfrac[500];
  sprintf(labfrac, "Gauss fraction %.2f", fraction);
  TLatex *lab = new TLatex(0.13,0.75,labfrac);
  lab->SetNDC();
  lab->SetTextSize(0.05);
  if(draw_gauss)  lab->Draw();

}
*/
