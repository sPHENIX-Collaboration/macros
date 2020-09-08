// $Id: $

/*!
 * \file QA_Draw_VertexResolution.C
 * \brief 
 * \author Thomas Marshall and Jin Huang <rosstom@ucla.edu> & <jhuang@bnl.gov>
 * \version $Revision:   $
 * \date $Date: $
 */

#include <TFile.h>
#include <TFunction.h>
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

void QA_Draw_VertexResolution(
    const char *hist_name_prefix = "QAG4SimulationVertex",
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

  // X-direction

  TH2 *h_new = (TH2 *) qa_file_new->GetObjectChecked(
      prefix + TString("vxRes_gvz"), "TH2");
  assert(h_new);

  // h_new->Rebin2D(1, 5);
  h_new->Sumw2();
  // h_new->GetXaxis()->SetRangeUser(-15,15);
  //  h_new->Scale(1. / Nevent_new);

  TH2 *h_ref = NULL;
  if (qa_file_ref)
  {
    h_ref = (TH2 *) qa_file_ref->GetObjectChecked(
        prefix + TString("vxRes_gvz"), "TH2");
    assert(h_ref);

    // h_ref->Rebin2D(1, 5);
    h_ref->Sumw2();
    // h_ref->Scale(Nevent_new / Nevent_ref);
  }

  TCanvas *c1 = new TCanvas(TString("QA_Draw_Vertex_Resolution_x") + TString("_") + hist_name_prefix,
                            TString("QA_Draw_Vertex_Resolution_x") + TString("_") + hist_name_prefix,
                            1800, 1000);
  c1->Divide(2,1);
  int idx = 1;
  TPad *p;

  vector<pair<double, double>> gvz_ranges{
      {-10.0, 10.0}};
  TF1 *f1 = nullptr;
  TF1 *fit = nullptr;
  Double_t sigma = 0;
  Double_t sigma_unc = 0;
  char resstr[500];
  TLatex *res = nullptr;
  for (auto gvz_range : gvz_ranges)
  {
    cout << __PRETTY_FUNCTION__ << " process " << gvz_range.first << " - " << gvz_range.second << " cm";

    p = (TPad *) c1->cd(idx++);
    c1->Update();
    // p->SetLogy();

    const double epsilon = 1e-6;
    const int bin_start = h_new->GetXaxis()->FindBin(gvz_range.first + epsilon);
    const int bin_end = h_new->GetXaxis()->FindBin(gvz_range.second - epsilon);

    TH1 *h_proj_new = h_new->ProjectionY(
        TString::Format(
            "%s_New_ProjX_%d_%d",
            h_new->GetName(), bin_start, bin_end));
	// bin_start, bin_end);

    h_proj_new->SetTitle(TString(hist_name_prefix) + TString::Format(
                                                         ": %.1f - %.1f cm - gvz", gvz_range.first, gvz_range.second));
    h_proj_new->GetXaxis()->SetTitle(TString::Format(
        "Vertex Resolution (x) [cm]"));
    h_proj_new->GetXaxis()->SetNdivisions(5,5);
    h_proj_new->GetXaxis()->SetRangeUser(-0.002,0.002);

    f1 = new TF1("f1","gaus",-.002,.002);
    h_proj_new->Fit(f1);
    fit = h_proj_new->GetFunction("f1");
    sigma = fit->GetParameter(2);
    sigma_unc = fit->GetParError(2);


    TH1 *h_proj_ref = nullptr;
    if (h_ref)
    {
      h_proj_ref =
          h_ref->ProjectionY(
              TString::Format(
                  "%s_Ref_ProjX_%d_%d",
                  h_new->GetName(), bin_start, bin_end));
              // bin_start, bin_end);
      h_proj_ref->GetXaxis()->SetRangeUser(-10,10);
    }
    
    DrawReference(h_proj_new, h_proj_ref);
    sprintf(resstr,"#sigma = %.5f #pm %.5f cm", sigma, sigma_unc);
    res = new TLatex(0.325,0.825,resstr);
    res->SetNDC();
    res->SetTextSize(0.05);
    res->SetTextAlign(13);
    res->Draw();
  }
  p = (TPad *) c1->cd(idx++);
  c1->Update();
  gPad->SetLeftMargin(.2);
  h_new->GetYaxis()->SetTitleOffset(2);
  h_new->Draw("colz");
  

  SaveCanvas(c1, TString(qa_file_name_new) + TString("_") + TString(c1->GetName()), true);

  // Y-direction

  TH2 *h_new2 = (TH2 *) qa_file_new->GetObjectChecked(
      prefix + TString("vyRes_gvz"), "TH2");
  assert(h_new2);

  //  h_new->Rebin(1, 2);
  h_new2->Sumw2();
  //  h_new->Scale(1. / Nevent_new);

  TH2 *h_ref2 = NULL;
  if (qa_file_ref)
  {
    h_ref2 = (TH2 *) qa_file_ref->GetObjectChecked(
        prefix + TString("vyRes_gvz"), "TH2");
    assert(h_ref2);

    // h_ref->Rebin(1, 2);
    h_ref2->Sumw2();
    // h_ref->Scale(Nevent_new / Nevent_ref);
  }

  TCanvas *c2 = new TCanvas(TString("QA_Draw_Vertex_Resolution_y") + TString("_") + hist_name_prefix,
                            TString("QA_Draw_Vertex_Resolution_y") + TString("_") + hist_name_prefix,
                            1800, 1000);
  c2->Divide(2,1);
  int idx2 = 1;
  TPad *p2;

  vector<pair<double, double>> gvz_ranges2{
      {-10.0, 10.0}};
  TF1 *f2 = nullptr;
  TF1 *fit2 = nullptr;
  Double_t sigma2 = 0;
  Double_t sigma_unc2 = 0;
  char resstr2[500];
  TLatex *res2 = nullptr;
  for (auto gvz_range : gvz_ranges2)
  {
    cout << __PRETTY_FUNCTION__ << " process " << gvz_range.first << " - " << gvz_range.second << " cm";

    p2 = (TPad *) c2->cd(idx2++);
    c2->Update();
    // p->SetLogy();

    const double epsilon = 1e-6;
    const int bin_start = h_new2->GetXaxis()->FindBin(gvz_range.first + epsilon);
    const int bin_end = h_new2->GetXaxis()->FindBin(gvz_range.second - epsilon);

    TH1 *h_proj_new2 = h_new2->ProjectionY(
        TString::Format(
            "%s_New_ProjX_%d_%d",
            h_new2->GetName(), bin_start, bin_end),
        bin_start, bin_end);

    h_proj_new2->SetTitle(TString(hist_name_prefix) + TString::Format(
                                                         ": %.1f - %.1f cm - gvz", gvz_range.first, gvz_range.second));
    h_proj_new2->GetXaxis()->SetTitle(TString::Format(
        "Vertex Resolution (y) [cm]"));
    h_proj_new2->GetXaxis()->SetNdivisions(5,5);
    h_proj_new2->GetXaxis()->SetRangeUser(-0.002,0.002);
    
    f2 = new TF1("f2","gaus",-.002,.002);
    h_proj_new2->Fit(f2);
    fit2 = h_proj_new2->GetFunction("f2");
    sigma2 = fit2->GetParameter(2);
    sigma_unc2 = fit2->GetParError(2);

    TH1 *h_proj_ref2 = nullptr;
    if (h_ref2)
    {
      h_proj_ref2 =
          h_ref2->ProjectionY(
              TString::Format(
                  "%s_Ref_ProjX_%d_%d",
                  h_new2->GetName(), bin_start, bin_end),
              bin_start, bin_end);
    }
    
    DrawReference(h_proj_new2, h_proj_ref2);
    sprintf(resstr2,"#sigma = %.5f #pm %.5f cm", sigma2, sigma_unc2);
    res2 = new TLatex(0.325,0.825,resstr2);
    res2->SetNDC();
    res2->SetTextSize(0.05);
    res2->SetTextAlign(13);
    res2->Draw();
  }
  p2 = (TPad *) c2->cd(idx2++);
  c2->Update();
  gPad->SetLeftMargin(.2);
  h_new2->GetYaxis()->SetTitleOffset(2);
  h_new2->Draw("colz");

  SaveCanvas(c2, TString(qa_file_name_new) + TString("_") + TString(c2->GetName()), true);

  // Z-direction

  TH2 *h_new3 = (TH2 *) qa_file_new->GetObjectChecked(
      prefix + TString("vzRes_gvz"), "TH2");
  assert(h_new3);

  //  h_new->Rebin(1, 2);
  h_new3->Sumw2();
  //  h_new->Scale(1. / Nevent_new);

  TH2 *h_ref3 = NULL;
  if (qa_file_ref)
  {
    h_ref3 = (TH2 *) qa_file_ref->GetObjectChecked(
        prefix + TString("vzRes_gvz"), "TH2");
    assert(h_ref3);

    // h_ref->Rebin(1, 2);
    h_ref3->Sumw2();
    // h_ref->Scale(Nevent_new / Nevent_ref);
  }

  TCanvas *c3 = new TCanvas(TString("QA_Draw_Vertex_Resolution_z") + TString("_") + hist_name_prefix,
                            TString("QA_Draw_Vertex_Resolution_z") + TString("_") + hist_name_prefix,
                            1800, 1000);
  c3->Divide(2,1);
  int idx3 = 1;
  TPad *p3;

  vector<pair<double, double>> gvz_ranges3{
      {-10.0, 10.0}};
  TF1 *f3 = nullptr;
  TF1 *fit3 = nullptr;
  Double_t sigma3 = 0;
  Double_t sigma_unc3 = 0;
  char resstr3[500];
  TLatex *res3 = nullptr;
  for (auto gvz_range : gvz_ranges3)
  {
    cout << __PRETTY_FUNCTION__ << " process " << gvz_range.first << " - " << gvz_range.second << " cm";

    p3 = (TPad *) c3->cd(idx3++);
    c3->Update();
    // p->SetLogy();

    const double epsilon = 1e-6;
    const int bin_start = h_new3->GetXaxis()->FindBin(gvz_range.first + epsilon);
    const int bin_end = h_new3->GetXaxis()->FindBin(gvz_range.second - epsilon);

    TH1 *h_proj_new3 = h_new3->ProjectionY(
        TString::Format(
            "%s_New_ProjX_%d_%d",
            h_new3->GetName(), bin_start, bin_end),
        bin_start, bin_end);

    h_proj_new3->SetTitle(TString(hist_name_prefix) + TString::Format(
                                                         ": %.1f - %.1f cm -gvz", gvz_range.first, gvz_range.second));
    h_proj_new3->GetXaxis()->SetTitle(TString::Format(
        "Vertex Resolution (z) [cm]"));
    h_proj_new3->GetXaxis()->SetNdivisions(5,5);
    h_proj_new3->GetXaxis()->SetRangeUser(-0.002,0.002);
    
    f3 = new TF1("f3","gaus",-.002,.002);
    h_proj_new3->Fit(f3);
    fit3 = h_proj_new3->GetFunction("f3");
    sigma3 = fit3->GetParameter(2);
    sigma_unc3 = fit3->GetParError(2);

    TH1 *h_proj_ref3 = nullptr;
    if (h_ref3)
    {
      h_proj_ref3 =
          h_ref3->ProjectionY(
              TString::Format(
                  "%s_Ref_ProjX_%d_%d",
                  h_new3->GetName(), bin_start, bin_end),
              bin_start, bin_end);
    }
    
    DrawReference(h_proj_new3, h_proj_ref3);
    sprintf(resstr3,"#sigma = %.5f #pm %.5f cm", sigma3, sigma_unc3);
    res3 = new TLatex(0.325,0.825,resstr3);
    res3->SetNDC();
    res3->SetTextSize(0.05);
    res3->SetTextAlign(13);
    res3->Draw();
  }
  p3 = (TPad *) c3->cd(idx3++);
  c3->Update();
  gPad->SetLeftMargin(.2);
  h_new3->GetYaxis()->SetTitleOffset(2);
  h_new3->Draw("colz");

  SaveCanvas(c3, TString(qa_file_name_new) + TString("_") + TString(c3->GetName()), true);
}

