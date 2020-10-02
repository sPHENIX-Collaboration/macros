// $Id: $

/*!
 * \file QA_Draw_Utility.C
 * \brief
 * \author Jin Huang <jhuang@bnl.gov>
 * \version $Revision:   $
 * \date $Date: $
 */

#ifndef QA_Draw_Utility_C
#define QA_Draw_Utility_C

#include <TCanvas.h>
#include <TClass.h>
#include <TDirectory.h>
#include <TFile.h>
#include <TSystem.h>

#include <TGraphErrors.h>
#include <TH1F.h>
#include <TList.h>
#include <TObject.h>
#include <TString.h>
#include <TStyle.h>
#include <TTree.h>

#include <TLegend.h>
#include <TLegendEntry.h>
#include <TLine.h>
#include <TPad.h>

#include <cassert>
#include <cmath>

#include <iostream>

using namespace std;

//! Service function to SaveCanvas()
void SavePad(TPad *p)
{
  if (!p)
    return;

  TList *l = p->GetListOfPrimitives();
  //  l->Print();

  TIter next(l);
  TObject *obj = NULL;
  while ((obj = next()))
  {
    if (obj->IsA()->GetBaseClassOffset(TClass::GetClass("TPad")) >= 0)
    {
      if ((TPad *) obj != p)
        SavePad((TPad *) obj);
    }
    else if (obj->IsA()->GetBaseClassOffset(TClass::GetClass("TH1")) >= 0)
    {
      cout << "Save TH1 " << obj->GetName() << endl;
      obj->Clone()->Write(obj->GetName(), TObject::kOverwrite);
    }
    else if (obj->IsA()->GetBaseClassOffset(TClass::GetClass("TF1")) >= 0)
    {
      cout << "Save TF1 " << obj->GetName() << endl;
      obj->Clone()->Write(obj->GetName(), TObject::kOverwrite);
    }
    else if (obj->IsA()->GetBaseClassOffset(TClass::GetClass("TGraph")) >= 0)
    {
      cout << "Save TGraph " << obj->GetName() << endl;
      obj->Clone()->Write(obj->GetName(), TObject::kOverwrite);
    }
  }
}

//! Save canvas to multiple formats
/*!
 *  @param[in] c    pointer to the canvas
 *  @param[in] name Base of the file name. The default is the name of the cavas
 *  @param[in] bEPS true = save .eps and .pdf format too.
 */
void SaveCanvas(TCanvas *c, TString name = "", Bool_t bEPS = kTRUE)
{
  if (name.Length() == 0)
    name = c->GetName();

  c->Print(name + ".png");

  TDirectory *oldd = gDirectory;

  TString rootfilename;

  c->Print(rootfilename = name + ".root");

  TFile f(rootfilename, "update");

  SavePad(c);

  f.Close();

  oldd->cd();

  if (bEPS)
  {
    //      c->Print(name + ".pdf");

    float x = 20;
    float y = 20;
    gStyle->GetPaperSize(x, y);

    gStyle->SetPaperSize(c->GetWindowWidth() / 72 * 2.54,
                         c->GetWindowHeight() / 72 * 2.54);
    //      c->Print(name + ".eps");
    c->Print(name + ".svg");
    gSystem->Exec("rsvg-convert -f pdf -o " + name + ".pdf " + name + ".svg");
    gSystem->Exec("rm -fv " + name + ".svg");

    gStyle->SetPaperSize(x, y);
  }
  //      c->Print(name+".C");
}

//! Draw 1D histogram along with its reference as shade
//! @param[in] draw_href_error whether to draw error band for reference plot. Otherwise, it is a filled histogram (default)
double DrawReference(TH1 *hnew, TH1 *href, bool draw_href_error = false)
{
  hnew->SetLineColor(kBlue + 3);
  hnew->SetMarkerColor(kBlue + 3);
  //  hnew->SetLineWidth(2);
  hnew->SetMarkerStyle(kFullCircle);
  //  hnew->SetMarkerSize(1);

  if (href)
  {
    if (draw_href_error)
    {
      href->SetLineColor(kGreen + 1);
      href->SetFillColor(kGreen + 1);
      href->SetLineStyle(kSolid);
      href->SetMarkerColor(kGreen + 1);
      //      href->SetLineWidth(2);
      href->SetMarkerStyle(kDot);
      href->SetMarkerSize(0);
    }
    else
    {
      href->SetLineColor(kGreen + 1);
      href->SetFillColor(kGreen + 1);
      href->SetLineStyle(0);
      href->SetMarkerColor(kGreen + 1);
      href->SetLineWidth(0);
      href->SetMarkerStyle(kDot);
      href->SetMarkerSize(0);
    }
  }

  hnew->Draw();  // set scale

  double ks_test = numeric_limits<double>::signaling_NaN();

  if (href)
  {
    if (draw_href_error)
    {
      href->DrawClone("E2 same");
      href->SetFillStyle(0);
      href->SetLineWidth(8);
      href->DrawClone("HIST same ][");
    }
    else
      href->Draw("HIST same");
    hnew->Draw("same");  // over lay data points

    ks_test = hnew->KolmogorovTest(href, "D");
  }

  // ---------------------------------
  // now, make summary header
  // ---------------------------------
  if (href)
  {
    gPad->SetTopMargin(.14);
    TLegend *legend = new TLegend(0, .93, 0, 1, hnew->GetTitle(), "NB NDC");
    legend->Draw();
    legend = new TLegend(0, .86, .3, .93, NULL, "NB NDC");
    legend->AddEntry(href, Form("Reference"), "f");
    legend->Draw();
    legend = new TLegend(0.3, .86, 1, .93, NULL, "NB NDC");
    TLegendEntry *le = legend->AddEntry(hnew, Form("New: KS-Test P=%.3f", ks_test), "lpe");
    if (ks_test>=1)
      le->SetTextColor(kBlue+1);
    else if (ks_test>=.2)
      le->SetTextColor(kGreen+2);
    else if (ks_test>=.05)
      le->SetTextColor(kYellow+1);
    else
      le->SetTextColor(kRed+1);
    legend->Draw();
  }
  else
  {
    gPad->SetTopMargin(.07);
    TLegend *legend = new TLegend(0, .93, 0, 1, hnew->GetTitle(), "NB NDC");
    legend->Draw();
  }

  return ks_test;
}

//! Draw 1D TGraph along with its reference as shade
//! @param[in] draw_href_error whether to draw error band for reference plot. Otherwise, it is a filled histogram (default)
void DrawReference(TGraph *hnew, TGraph *href, bool draw_href_error = true)
{
  hnew->SetLineColor(kBlue + 3);
  hnew->SetMarkerColor(kBlue + 3);
  hnew->SetLineWidth(2);
  hnew->SetMarkerStyle(kFullCircle);
  hnew->SetMarkerSize(1);

  if (href)
  {
    if (draw_href_error)
    {
      href->SetLineColor(kGreen + 1);
      href->SetFillColor(kGreen + 1);
      href->SetFillStyle(0);
      href->SetLineStyle(kSolid);
      href->SetMarkerColor(kGreen + 1);
      href->SetLineWidth(4);
      href->SetMarkerStyle(kDot);
      href->SetMarkerSize(0);
    }
    else
    {
      href->SetLineColor(kGreen + 1);
      href->SetFillColor(kGreen + 1);
      href->SetLineStyle(0);
      href->SetMarkerColor(kGreen + 1);
      href->SetLineWidth(0);
      href->SetMarkerStyle(kDot);
      href->SetMarkerSize(0);
    }
  }

  if (href)
  {
    if (draw_href_error)
    {
      href->DrawClone("E2");
    }
    else
      href->Draw("HIST same");
    hnew->Draw("p e");  // over lay data points
  }
}

//! Fit for resolution of TH2F
TGraphErrors *
FitResolution(const TH2F *h2, const bool normalize_mean = true)
{
  TProfile *p2 = h2->ProfileX();

  int n = 0;
  double x[1000];
  double ex[1000];
  double y[1000];
  double ey[1000];

  for (int i = 1; i <= h2->GetNbinsX(); i++)
  {
    TH1D *h1 = h2->ProjectionY(Form("htmp_%d", rand()), i, i);

    if (h1->GetSum() < 10)
      continue;

    TF1 fgaus("fgaus", "gaus", -p2->GetBinError(i) * 4,
              p2->GetBinError(i) * 4);

    TF1 f2(Form("dgaus"), "gaus + [3]*exp(-0.5*((x-[1])/[4])**2) + [5]",
           -p2->GetBinError(i) * 4, p2->GetBinError(i) * 4);

    fgaus.SetParameter(1, p2->GetBinContent(i));
    fgaus.SetParameter(2, p2->GetBinError(i));

    h1->Fit(&fgaus, "MQ0");

    x[n] = p2->GetBinCenter(i);
    ex[n] = (p2->GetBinCenter(2) - p2->GetBinCenter(1)) / 2;

    const double norm = normalize_mean ? fgaus.GetParameter(1) : 1;

    y[n] = fgaus.GetParameter(2) / norm;
    ey[n] = fgaus.GetParError(2) / norm;

    n++;
    delete h1;
  }

  TGraphErrors *ge = new TGraphErrors(n, x, y, 0, ey);
  ge->SetName(TString(h2->GetName()) + "_FitResolution");

  ge->SetLineColor(kBlue + 3);
  ge->SetMarkerColor(kBlue + 3);
  ge->SetLineWidth(2);
  ge->SetMarkerStyle(kFullCircle);
  ge->SetMarkerSize(1);
  return ge;
}

//! Fit for profile along the Y direction of TH2F
TGraphErrors *
FitProfile(const TH2F *h2)
{
  TProfile *p2 = h2->ProfileX();

  int n = 0;
  double x[1000];
  double ex[1000];
  double y[1000];
  double ey[1000];

  for (int i = 1; i <= h2->GetNbinsX(); i++)
  {
    TH1D *h1 = h2->ProjectionY(Form("htmp_%d", rand()), i, i);

    if (h1->GetSum() < 10)
      continue;

    TF1 fgaus("fgaus", "gaus", -p2->GetBinError(i) * 4,
              p2->GetBinError(i) * 4);

    TF1 f2(Form("dgaus"), "gaus + [3]*exp(-0.5*((x-[1])/[4])**2) + [5]",
           -p2->GetBinError(i) * 4, p2->GetBinError(i) * 4);

    fgaus.SetParameter(1, p2->GetBinContent(i));
    fgaus.SetParameter(2, p2->GetBinError(i));

    h1->Fit(&fgaus, "MQ0");

    //      f2.SetParameters(fgaus.GetParameter(0) / 2, fgaus.GetParameter(1),
    //          fgaus.GetParameter(2), fgaus.GetParameter(0) / 2,
    //          fgaus.GetParameter(2) / 4, 0);
    //
    //      h1->Fit(&f2, "MQ0");

    //      new TCanvas;
    //      h1->Draw();
    //      fgaus.Draw("same");
    //      break;

    x[n] = p2->GetBinCenter(i);
    ex[n] = (p2->GetBinCenter(2) - p2->GetBinCenter(1)) / 2;
    y[n] = fgaus.GetParameter(1);
    ey[n] = fgaus.GetParameter(2);

    //      p2->SetBinContent(i, fgaus.GetParameter(1));
    //      p2->SetBinError(i, fgaus.GetParameter(2));

    n++;
    delete h1;
  }

  TGraphErrors *ge = new TGraphErrors(n, x, y, ex, ey);
  ge->SetName(TString(h2->GetName()) + "_FitProfile");
  ge->SetLineColor(kBlue + 3);
  ge->SetMarkerColor(kBlue + 3);
  ge->SetLineWidth(2);
  ge->SetMarkerStyle(kFullCircle);
  ge->SetMarkerSize(1);
  return ge;
}

//!ratio between two histograms with binominal error based on Wilson score interval. Assuming each histogram is count.
TH1 *GetBinominalRatio(TH1 *h_pass, TH1 *h_n_trial, bool process_zero_bins = false)
{
  assert(h_pass);
  assert(h_n_trial);

  assert(h_pass->GetNbinsX() == h_n_trial->GetNbinsX());
  assert(h_pass->GetNbinsY() == h_n_trial->GetNbinsY());
  assert(h_pass->GetNbinsZ() == h_n_trial->GetNbinsZ());

  TH1 *h_ratio = (TH1 *) h_pass->Clone(TString(h_pass->GetName()) + "_Ratio");
  assert(h_ratio);
  h_ratio->Divide(h_n_trial);  // a rough estimation first, also taking care of the overflow bins and zero bins

  for (int x = 1; x <= h_n_trial->GetNbinsX(); ++x)
    for (int y = 1; y <= h_n_trial->GetNbinsY(); ++y)
      for (int z = 1; z <= h_n_trial->GetNbinsZ(); ++z)
      {
        const double n_trial = h_n_trial->GetBinContent(x, y, z);

        if (n_trial > 0)
        {
          const double p = h_pass->GetBinContent(x, y, z) / n_trial;

          // Wilson score interval
          h_ratio->SetBinContent(x, y, z,  //
                                 (p + 1 / (2 * n_trial)) / (1 + 1 / n_trial));
          h_ratio->SetBinError(x, y,
                               z,  //
                               TMath::Sqrt(
                                   1. / n_trial * p * (1 - p) + 1. / (4 * n_trial * n_trial)) /
                                   (1 + 1 / n_trial));
        }
        else if (process_zero_bins)
        {
          h_ratio->SetBinContent(x, y, z, 0.5);
          h_ratio->SetBinError(x, y, z, 0.5);
        }
      }

  return h_ratio;
}

//! Put input file name on the canvas
//! \param[in] c1 canvas pointer
//! \param[in] bottom height fraction height of the canvas to be used for this label
//! \param[in] new_file_name new file name being inspected
//! \param[in] ref_file_name reference file name
void PutInputFileName(TCanvas *c1, const double height, const char *new_file_name, const char *ref_file_name)
{
  c1->cd();
  TPad *pad = new TPad("PutInputFileName", "PutInputFileName", 0, 0, 1, height, 18);
  pad->Draw();
  pad->cd();

  if (new_file_name)
  {
    TText *t = new TText(0.05, 0.75, TString("New QA File:          ") + TString(new_file_name));
    t->SetTextAlign(12);
    t->SetTextColor(kBlue + 3);
    t->SetTextSize(.4);
    t->Draw();
  }
  if (ref_file_name)
  {
    TText *t = new TText(0.05, 0.25, TString("Reference QA File: ") + TString(ref_file_name));
    t->SetTextAlign(12);
    t->SetTextColor(kGreen + 1);
    t->SetTextSize(.4);
    t->Draw();
  }
}

#endif
