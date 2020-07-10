// $Id: $

/*!
 * \file QA_Draw_DCArPhi.C
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

void QA_Draw_DCArPhi(
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

  TH2 *h_new = (TH2 *) qa_file_new->GetObjectChecked(
      prefix + TString("DCArPhi_pT"), "TH2");
  assert(h_new);

  //  h_new->Rebin(1, 2);
  h_new->Sumw2();
  //  h_new->Scale(1. / Nevent_new);

  TH2 *h_ref = NULL;
  if (qa_file_ref)
  {
    h_ref = (TH2 *) qa_file_ref->GetObjectChecked(
        prefix + TString("DCArPhi_pT"), "TH2");
    assert(h_ref);

    //    h_ref->Rebin(1, 2);
    h_ref->Sumw2();
    h_ref->Scale(Nevent_new / Nevent_ref);
  }

  TCanvas *c1 = new TCanvas(TString("QA_Draw_Tracking_DCArPhi") + TString("_") + hist_name_prefix,
                            TString("QA_Draw_Tracking_DCArPhi") + TString("_") + hist_name_prefix,
                            1800, 1000);
  c1->Divide(4, 2);
  int idx = 1;
  TPad *p;

  vector<pair<double, double>> gpt_ranges{
      {0, 0.5},
      {0.5, 1},
      {1, 1.5},
      {1.5, 2},
      {2, 4},
      {4, 8},
      {8, 16},
      {16, 40}};
  for (auto pt_range : gpt_ranges)
  {
    cout << __PRETTY_FUNCTION__ << " process " << pt_range.first << " - " << pt_range.second << " GeV/c";

    p = (TPad *) c1->cd(idx++);
    c1->Update();
    p->SetLogy();

    const double epsilon = 1e-6;
    const int bin_start = h_new->GetXaxis()->FindBin(pt_range.first + epsilon);
    const int bin_end = h_new->GetXaxis()->FindBin(pt_range.second - epsilon);

    TH1 *h_proj_new = h_new->ProjectionY(
        TString::Format(
            "%s_New_ProjX_%d_%d",
            h_new->GetName(), bin_start, bin_end),
        bin_start, bin_end);
    if (pt_range.first < 2.0)
    {
      h_proj_new->GetXaxis()->SetRangeUser(-.05,.05);
      h_proj_new->Rebin(5);
    }
    else
    {
      h_proj_new->GetXaxis()->SetRangeUser(-.01,.01);
    }
    h_proj_new->SetTitle(TString(hist_name_prefix) + TString::Format(
                                                         ": %.1f - %.1f GeV/c", pt_range.first, pt_range.second));
    h_proj_new->GetXaxis()->SetTitle(TString::Format(
        "DCA (r #phi) [cm]"));
    h_proj_new->GetXaxis()->SetNdivisions(5,5);
    
    TH1 *h_proj_ref = nullptr;
    if (h_ref)
    {
      h_proj_ref =
          h_ref->ProjectionY(
              TString::Format(
                  "%s_Ref_ProjX_%d_%d",
                  h_new->GetName(), bin_start, bin_end),
              bin_start, bin_end);
      if (pt_range.first < 2.0)
      {
	//h_proj_ref->GetXaxis()->SetRangeUser(-.05,.05);
	h_proj_ref->Rebin(5);
      }
    }
    
    DrawReference(h_proj_new, h_proj_ref);
  }

  SaveCanvas(c1, TString(qa_file_name_new) + TString("_") + TString(c1->GetName()), true);
}
