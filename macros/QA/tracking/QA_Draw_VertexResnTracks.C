// $Id: $

/*!
 * \file QA_Draw_VertexResnTracks.C
 * \brief 
 * \author Thomas Marshall and Jin Huang <rosstom@ucla.edu> & <jhuang@bnl.gov>
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

//some common style files
#include "../../sPHENIXStyle/sPhenixStyle.C"
#include "QA_Draw_Utility.C"
using namespace std;

void QA_Draw_VertexResnTracks(
    const char *hist_name_prefix = "QAG4SimulationTracking",
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
  /*
  if (qa_file_new)
  {
    TH1 *h_norm = (TH1 *) qa_file_new->GetObjectChecked(
        prefix + TString("Normalization"), "TH1");
    assert(h_norm);

    Nevent_new = h_norm->GetBinContent(h_norm->GetXaxis()->FindBin("Truth Track"));
  }
  if (qa_file_ref)
  {
    TH1 *h_norm = (TH1 *) qa_file_ref->GetObjectChecked(
        prefix + TString("Normalization"), "TH1");
    assert(h_norm);

    Nevent_ref = h_norm->GetBinContent(h_norm->GetXaxis()->FindBin("Truth Track"));
  }
  */
  TH2 *h_new = (TH2 *) qa_file_new->GetObjectChecked(
      prefix + TString("vxRes_ntracks"), "TH2");
  assert(h_new);

  //  h_new->Rebin(1, 2);
  h_new->Sumw2();
  //  h_new->Scale(1. / Nevent_new);

  TH2 *h_ref = NULL;
  if (qa_file_ref)
  {
    h_ref = (TH2 *) qa_file_ref->GetObjectChecked(
        prefix + TString("vxRes_ntracks"), "TH2");
    assert(h_ref);

    //    h_ref->Rebin(1, 2);
    h_ref->Sumw2();
    // h_ref->Scale(Nevent_new / Nevent_ref);
  }

  TCanvas *c1 = new TCanvas(TString("QA_Draw_Tracking_vxRes_ntracks") + TString("_") + hist_name_prefix,
                            TString("QA_Draw_Tracking_vxRes_ntracks") + TString("_") + hist_name_prefix,
                            1800, 1000);
  c1->Divide(4, 2);
  int idx = 1;
  TPad *p;

  vector<pair<double, double>> ntracks_ranges{
      {0, 5},
      {5, 10},
      {10, 15},
      {15, 20},
      {20, 25},
      {25, 30},
      {30, 35},
      {35, 40}};
  TF1 *f1 = nullptr;
  TF1 *fit = nullptr;
  Double_t sigma = 0;
  Double_t sigma_unc = 0;
  char resstr[500];
  TLatex *res = nullptr;
  for (auto ntracks_range : ntracks_ranges)
  {
    cout << __PRETTY_FUNCTION__ << " process " << ntracks_range.first << " - " << ntracks_range.second;

    p = (TPad *) c1->cd(idx++);
    c1->Update();
    p->SetLogy();

    const double epsilon = 1e-6;
    const int bin_start = h_new->GetXaxis()->FindBin(ntracks_range.first + epsilon);
    const int bin_end = h_new->GetXaxis()->FindBin(ntracks_range.second - epsilon);

    TH1 *h_proj_new = h_new->ProjectionY(
        TString::Format(
            "%s_New_ProjX_%d_%d",
            h_new->GetName(), bin_start, bin_end),
        bin_start, bin_end);
    /*if (ntracks_range.first < 2.0)
    {
      h_proj_new->GetXaxis()->SetRangeUser(-.05,.05);
      h_proj_new->Rebin(5);
    }
    else
    {
      h_proj_new->GetXaxis()->SetRangeUser(-.01,.01);
    }*/
    h_proj_new->Rebin(5);
    h_proj_new->SetTitle(TString(hist_name_prefix) + TString::Format(
                                                         ": %.1f - %.1f tracks", ntracks_range.first, ntracks_range.second));
    h_proj_new->GetXaxis()->SetNdivisions(5,5);
    f1 = new TF1("f1","gaus",-.005,.005);
    h_proj_new->Fit(f1);
    fit = h_proj_new->GetFunction("f1");
    sigma = fit->GetParameter(2);
    sigma_unc = fit->GetParError(2);
    h_proj_new->GetXaxis()->SetTitle(TString::Format("Vertex Res (x) [cm]"));

    TH1 *h_proj_ref = nullptr;
    if (h_ref)
    {
      h_proj_ref =
          h_ref->ProjectionY(
              TString::Format(
                  "%s_Ref_ProjX_%d_%d",
                  h_new->GetName(), bin_start, bin_end),
              bin_start, bin_end);
      //if (ntracks_range.first < 2.0)
      //{
	//h_proj_ref->GetXaxis()->SetRangeUser(-.05,.05);
	//h_proj_ref->Rebin(5);
      //}
      h_proj_ref->Rebin(5);
    }
    
    DrawReference(h_proj_new, h_proj_ref);

    sprintf(resstr,"#sigma = %.5f #pm %.5f cm", sigma, sigma_unc);
    res = new TLatex(0.325,0.825,resstr);
    res->SetNDC();
    res->SetTextSize(0.05);
    res->SetTextAlign(13);
    res->Draw();
  }
  /*
  p = (TPad *) c1->cd(idx++);
  c1->Update();
  TPaveText *pt = new TPaveText(.05,.1,.95,.8);
  pt->AddText("No cuts");
  pt->Draw();
  */
  SaveCanvas(c1, TString(qa_file_name_new) + TString("_") + TString(c1->GetName()), true);


  TH2 *h_new2 = (TH2 *) qa_file_new->GetObjectChecked(
      prefix + TString("vyRes_ntracks"), "TH2");
  assert(h_new2);

  //  h_new->Rebin(1, 2);
  h_new2->Sumw2();
  //  h_new->Scale(1. / Nevent_new);

  TH2 *h_ref2 = NULL;
  if (qa_file_ref)
  {
    h_ref2 = (TH2 *) qa_file_ref->GetObjectChecked(
        prefix + TString("vyRes_ntracks"), "TH2");
    assert(h_ref2);

    //    h_ref->Rebin(1, 2);
    h_ref2->Sumw2();
    //h_ref->Scale(Nevent_new / Nevent_ref);
  }

  TCanvas *c2 = new TCanvas(TString("QA_Draw_Tracking_vyRes_ntracks") + TString("_") + hist_name_prefix,
                            TString("QA_Draw_Tracking_vyRes_ntracks") + TString("_") + hist_name_prefix,
                            1800, 1000);
  c2->Divide(4, 2);
  int idx2 = 1;
  TPad *p2;

  vector<pair<double, double>> ntracks_ranges2{
      {0, 5},
      {5, 10},
      {10, 15},
      {15, 20},
      {20, 25},
      {25, 30},
      {30, 35},
      {35, 40}};
  TF1 *f2 = nullptr;
  TF1 *fit2 = nullptr;
  Double_t sigma2 = 0;
  Double_t sigma_unc2 = 0;
  char resstr2[500];
  TLatex *res2 = nullptr;
  for (auto ntracks_range2 : ntracks_ranges2)
  {
    cout << __PRETTY_FUNCTION__ << " process " << ntracks_range2.first << " - " << ntracks_range2.second;

    p2 = (TPad *) c2->cd(idx2++);
    c2->Update();
    p2->SetLogy();

    const double epsilon = 1e-6;
    const int bin_start = h_new2->GetXaxis()->FindBin(ntracks_range2.first + epsilon);
    const int bin_end = h_new2->GetXaxis()->FindBin(ntracks_range2.second - epsilon);

    TH1 *h_proj_new = h_new2->ProjectionY(
        TString::Format(
            "%s_New_ProjX_%d_%d",
            h_new2->GetName(), bin_start, bin_end),
        bin_start, bin_end);
    /*if (ntracks_range.first < 2.0)
    {
      h_proj_new->GetXaxis()->SetRangeUser(-.05,.05);
      h_proj_new->Rebin(5);
    }
    else
    {
      h_proj_new->GetXaxis()->SetRangeUser(-.01,.01);
    }*/
    h_proj_new->Rebin(5);
    h_proj_new->SetTitle(TString(hist_name_prefix) + TString::Format(
                                                         ": %.1f - %.1f tracks", ntracks_range2.first, ntracks_range2.second));
    h_proj_new->GetXaxis()->SetNdivisions(5,5);
    f2 = new TF1("f2","gaus",-.005,.005);
    h_proj_new->Fit(f2);
    fit2 = h_proj_new->GetFunction("f2");
    sigma2 = fit2->GetParameter(2);
    sigma_unc2 = fit2->GetParError(2);
    h_proj_new->GetXaxis()->SetTitle(TString::Format("Vertex Res (y) [cm]"));

    TH1 *h_proj_ref = nullptr;
    if (h_ref)
    {
      h_proj_ref =
          h_ref2->ProjectionY(
              TString::Format(
                  "%s_Ref_ProjX_%d_%d",
                  h_new2->GetName(), bin_start, bin_end),
              bin_start, bin_end);
      //if (ntracks_range2.first < 2.0)
      //{
	//h_proj_ref->GetXaxis()->SetRangeUser(-.05,.05);
	//h_proj_ref->Rebin(5);
      //}
      h_proj_ref->Rebin(5);
    }
    
    DrawReference(h_proj_new, h_proj_ref);

    sprintf(resstr2,"#sigma = %.5f #pm %.5f cm", sigma2, sigma_unc2);
    res2 = new TLatex(0.325,0.825,resstr2);
    res2->SetNDC();
    res2->SetTextSize(0.05);
    res2->SetTextAlign(13);
    res2->Draw();
  }

  SaveCanvas(c2, TString(qa_file_name_new) + TString("_") + TString(c2->GetName()), true);

  TH2 *h_new3 = (TH2 *) qa_file_new->GetObjectChecked(
      prefix + TString("vzRes_ntracks"), "TH2");
  assert(h_new3);

  //  h_new->Rebin(1, 2);
  h_new3->Sumw2();
  //  h_new->Scale(1. / Nevent_new);

  TH2 *h_ref3 = NULL;
  if (qa_file_ref)
  {
    h_ref3 = (TH2 *) qa_file_ref->GetObjectChecked(
        prefix + TString("vzRes_ntracks"), "TH2");
    assert(h_ref3);

    //    h_ref->Rebin(1, 2);
    h_ref3->Sumw2();
    //h_ref->Scale(Nevent_new / Nevent_ref);
  }

  TCanvas *c3 = new TCanvas(TString("QA_Draw_Tracking_vzRes_ntracks") + TString("_") + hist_name_prefix,
                            TString("QA_Draw_Tracking_vzRes_ntracks") + TString("_") + hist_name_prefix,
                            1800, 1000);
  c3->Divide(4, 2);
  int idx3 = 1;
  TPad *p3;

  vector<pair<double, double>> ntracks_ranges3{
      {0, 5},
      {5, 10},
      {10, 15},
      {15, 20},
      {20, 25},
      {25, 30},
      {30, 35},
      {35, 40}};
  TF1 *f3 = nullptr;
  TF1 *fit3 = nullptr;
  Double_t sigma3 = 0;
  Double_t sigma_unc3 = 0;
  char resstr3[500];
  TLatex *res3 = nullptr;
  for (auto ntracks_range3 : ntracks_ranges3)
  {
    cout << __PRETTY_FUNCTION__ << " process " << ntracks_range3.first << " - " << ntracks_range3.second;

    p3 = (TPad *) c3->cd(idx3++);
    c3->Update();
    p3->SetLogy();

    const double epsilon = 1e-6;
    const int bin_start = h_new3->GetXaxis()->FindBin(ntracks_range3.first + epsilon);
    const int bin_end = h_new3->GetXaxis()->FindBin(ntracks_range3.second - epsilon);

    TH1 *h_proj_new = h_new3->ProjectionY(
        TString::Format(
            "%s_New_ProjX_%d_%d",
            h_new3->GetName(), bin_start, bin_end),
        bin_start, bin_end);
    /*if (ntracks_range.first < 2.0)
    {
      h_proj_new->GetXaxis()->SetRangeUser(-.05,.05);
      h_proj_new->Rebin(5);
    }
    else
    {
      h_proj_new->GetXaxis()->SetRangeUser(-.01,.01);
    }*/
    h_proj_new->Rebin(5);
    h_proj_new->SetTitle(TString(hist_name_prefix) + TString::Format(
                                                         ": %.1f - %.1f tracks", ntracks_range3.first, ntracks_range3.second));
    h_proj_new->GetXaxis()->SetNdivisions(5,5);
    f3 = new TF1("f3","gaus",-.005,.005);
    h_proj_new->Fit(f3);
    fit3 = h_proj_new->GetFunction("f3");
    sigma3 = fit3->GetParameter(2);
    sigma_unc3 = fit3->GetParError(2);
    h_proj_new->GetXaxis()->SetTitle(TString::Format("Vertex Res (z) [cm]"));

    TH1 *h_proj_ref = nullptr;
    if (h_ref)
    {
      h_proj_ref =
          h_ref3->ProjectionY(
              TString::Format(
                  "%s_Ref_ProjX_%d_%d",
                  h_new3->GetName(), bin_start, bin_end),
              bin_start, bin_end);
      //if (ntracks_range2.first < 2.0)
      //{
	//h_proj_ref->GetXaxis()->SetRangeUser(-.05,.05);
	//h_proj_ref->Rebin(5);
      //}
      h_proj_ref->Rebin(5);
    }
    
    DrawReference(h_proj_new, h_proj_ref);
    sprintf(resstr3,"#sigma = %.5f #pm %.5f cm", sigma3, sigma_unc3);
    res3 = new TLatex(0.325,0.825,resstr);
    res3->SetNDC();
    res3->SetTextSize(0.05);
    res3->SetTextAlign(13);
    res3->Draw();
  }

  SaveCanvas(c3, TString(qa_file_name_new) + TString("_") + TString(c3->GetName()), true);
}
