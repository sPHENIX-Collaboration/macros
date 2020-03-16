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

//some common style files
#include "../../sPHENIXStyle/sPhenixStyle.C"
#include "QA_Draw_Utility.C"
using namespace std;

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

    Nevent_new = h_norm->GetBinContent(h_norm->GetXaxis()->FindBin("Truth Track"));
  }
  if (qa_file_ref)
  {
    TH1 *h_norm = (TH1 *) qa_file_ref->GetObjectChecked(
        prefix + TString("Normalization"), "TH1");
    assert(h_norm);

    Nevent_ref = h_norm->GetBinContent(h_norm->GetXaxis()->FindBin("Truth Track"));
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

    TH1 *h_proj_ref = nullptr;
    if (h_ref)
      h_proj_ref =
          h_new->ProjectionY(
              TString::Format(
                  "%s_Ref_ProjX",
                  h_new->GetName()));

    DrawReference(h_proj_new, h_proj_ref);
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
    DrawReference(h_new, h_ref, false);
  }

  SaveCanvas(c1, TString(qa_file_name_new) + TString("_") + TString(c1->GetName()), true);
}
