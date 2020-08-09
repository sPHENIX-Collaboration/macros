// $Id: $

/*!
 * \file QA_Draw_Tracking_RecoTruthMatching.C
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

// some common style files
#include "../../sPHENIXStyle/sPhenixStyle.C"
#include "QA_Draw_Utility.C"
using namespace std;

void QA_Draw_Tracking_RecoTruthMatching(
    const char *hist_name_prefix = "QAG4SimulationTracking",
    const char *qa_file_name_new = "data/G4sPHENIX.root_qa.root",
    const char *qa_file_name_ref = "data/G4sPHENIX.root_qa.root") {
  SetsPhenixStyle();

  TVirtualFitter::SetDefaultFitter("Minuit2");

  TString prefix = TString("h_") + hist_name_prefix + TString("_");

  TFile *qa_file_new = new TFile(qa_file_name_new);
  assert(qa_file_new->IsOpen());

  TFile *qa_file_ref = NULL;
  if (qa_file_name_ref) {
    qa_file_ref = new TFile(qa_file_name_ref);
    assert(qa_file_ref->IsOpen());
  }

  // obtain normalization
  double Nevent_new = 1;
  double Nevent_ref = 1;

  if (qa_file_new) {
    cout << "Open new QA file " << qa_file_new->GetName() << endl;

    TH1 *h_norm = (TH1 *)qa_file_new->GetObjectChecked(
        prefix + TString("Normalization"), "TH1");
    assert(h_norm);

    Nevent_new = h_norm->GetBinContent(h_norm->GetXaxis()->FindBin("Event"));
  }
  if (qa_file_ref) {
    cout << "Open ref QA file " << qa_file_ref->GetName() << endl;
    TH1 *h_norm = (TH1 *)qa_file_ref->GetObjectChecked(
        prefix + TString("Normalization"), "TH1");
    assert(h_norm);

    Nevent_ref = h_norm->GetBinContent(h_norm->GetXaxis()->FindBin("Event"));
  }

  TCanvas *c1 = new TCanvas(TString("QA_Draw_Tracking_RecoTruthMatching") +
                                TString("_") + hist_name_prefix,
                            TString("QA_Draw_Tracking_RecoTruthMatching") +
                                TString("_") + hist_name_prefix,
                            1800, 1000);
  c1->Divide(2, 1);
  int idx = 1;
  TPad *p;

  {
    static const int nrebin = 5;

    p = (TPad *)c1->cd(idx++);
    c1->Update();
    p->SetLogx();
    p->SetGridy();

    TH1 *h_pass =
        (TH1 *)qa_file_new->GetObjectChecked(prefix + "nGen_pTReco", "TH1");
    TH1 *h_norm =
        (TH1 *)qa_file_new->GetObjectChecked(prefix + "nReco_pTReco", "TH1");
    assert(h_norm);
    assert(h_pass);

    h_norm->Rebin(nrebin);
    h_pass->Rebin(nrebin);

    TH1 *h_ratio = GetBinominalRatio(h_pass, h_norm);

    //    h_ratio->GetXaxis()->SetRangeUser(min_Et, max_Et);
    h_ratio->GetYaxis()->SetTitle("Tracking Purity");
    h_ratio->GetYaxis()->SetRangeUser(-0, 1.1);

    TH1 *h_ratio_ref = NULL;
    if (qa_file_ref) {
      TH1 *h_pass =
          (TH1 *)qa_file_ref->GetObjectChecked(prefix + "nGen_pTReco", "TH1");
      TH1 *h_norm =
          (TH1 *)qa_file_ref->GetObjectChecked(prefix + "nReco_pTReco", "TH1");
      assert(h_norm);
      assert(h_pass);
      h_norm->Rebin(nrebin);
      h_pass->Rebin(nrebin);
      h_ratio_ref = GetBinominalRatio(h_pass, h_norm);
    }

    h_ratio->SetTitle("Tracking Purity (matched truth-reco pairs)");

    DrawReference(h_ratio, h_ratio_ref, false);
  }

  {
    p = (TPad *)c1->cd(idx++);
    c1->Update();
    //    p->SetLogx();
    TH1 *frame = p->DrawFrame(0, .9, 50, 1.1,
                              "Mean and sigma p_{Tmatched}/p_{Treco};Reco p_{T} [GeV/c];<p_{T,matched}/p_{T,reco}> #pm #sigma(p_{T,matched}/p_{T,reco})");
    // gPad->SetLeftMargin(.2);
    gPad->SetTopMargin(-1);
    frame->GetYaxis()->SetTitleOffset(1.7);
    // TLine *l = new TLine(0, 1, 50, 1);
    // l->SetLineColor(kGray);
    // l->Draw();
    HorizontalLine( gPad, 1 )->Draw();

    TH2 *h_QAG4SimulationTracking_pTRecoTruthMatchedRatio_pTReco =
        (TH2 *)qa_file_new->GetObjectChecked(
            prefix + "pTRecoTruthMatchedRatio_pTReco", "TH2");
    assert(h_QAG4SimulationTracking_pTRecoTruthMatchedRatio_pTReco);

    h_QAG4SimulationTracking_pTRecoTruthMatchedRatio_pTReco->Rebin2D(16, 1);

    TGraphErrors *ge_QAG4SimulationTracking_pTRecoTruthMatchedRatio_pTReco =
        FitProfile(h_QAG4SimulationTracking_pTRecoTruthMatchedRatio_pTReco);
    ge_QAG4SimulationTracking_pTRecoTruthMatchedRatio_pTReco->Draw("pe");
    ge_QAG4SimulationTracking_pTRecoTruthMatchedRatio_pTReco->SetTitle(
       "Mean and sigma p_{Tmatched}/p_{Treco}");

    TGraphErrors *h_ratio_ref = NULL;
    if (qa_file_ref) {
      TH2 *h_QAG4SimulationTracking_pTRecoTruthMatchedRatio_pTReco =
          (TH2 *)qa_file_ref->GetObjectChecked(
              prefix + "pTRecoTruthMatchedRatio_pTReco", "TH2");
      assert(h_QAG4SimulationTracking_pTRecoTruthMatchedRatio_pTReco);

      h_QAG4SimulationTracking_pTRecoTruthMatchedRatio_pTReco->Rebin2D(16, 1);

      h_ratio_ref =
          FitProfile(h_QAG4SimulationTracking_pTRecoTruthMatchedRatio_pTReco);
      ge_QAG4SimulationTracking_pTRecoTruthMatchedRatio_pTReco->Draw("pe");
    }

    DrawReference(ge_QAG4SimulationTracking_pTRecoTruthMatchedRatio_pTReco,
                  h_ratio_ref, true);
  }

  SaveCanvas(c1,
             TString(qa_file_name_new) + TString("_") + TString(c1->GetName()),
             true);


  TCanvas *c2 = new TCanvas(TString("QA_Draw_Tracking_RecoTruthMatching_cuts") +
                                TString("_") + hist_name_prefix,
                            TString("QA_Draw_Tracking_RecoTruthMatching_cuts") +
                                TString("_") + hist_name_prefix,
                            1800, 1000);
  c2->Divide(2, 1);
  int idx2 = 1;
  TPad *p2;

  {
    static const int nrebin2 = 5;

    p2 = (TPad *)c2->cd(idx2++);
    c2->Update();
    p2->SetLogx();
    p2->SetGridy();

    TH1 *h_pass2 =
        (TH1 *)qa_file_new->GetObjectChecked(prefix + "nGen_pTReco_cuts", "TH1");
    TH1 *h_norm2 =
        (TH1 *)qa_file_new->GetObjectChecked(prefix + "nReco_pTReco_cuts", "TH1");
    assert(h_norm2);
    assert(h_pass2);

    h_norm2->Rebin(nrebin2);
    h_pass2->Rebin(nrebin2);

    TH1 *h_ratio2 = GetBinominalRatio(h_pass2, h_norm2);

    //    h_ratio->GetXaxis()->SetRangeUser(min_Et, max_Et);
    h_ratio2->GetYaxis()->SetTitle("Tracking Purity");
    h_ratio2->GetYaxis()->SetRangeUser(-0, 1.);

    TH1 *h_ratio_ref2 = NULL;
    if (qa_file_ref) {
      TH1 *h_pass2 =
          (TH1 *)qa_file_ref->GetObjectChecked(prefix + "nGen_pTReco_cuts", "TH1");
      TH1 *h_norm2 =
          (TH1 *)qa_file_ref->GetObjectChecked(prefix + "nReco_pTReco_cuts", "TH1");
      assert(h_norm2);
      assert(h_pass2);
      h_norm2->Rebin(nrebin2);
      h_pass2->Rebin(nrebin2);
      h_ratio_ref2 = GetBinominalRatio(h_pass2, h_norm2);
    }

    h_ratio2->SetTitle("Tracking Purity (#geq 2MVTX, #geq 1INTT, #geq 20 TPC)");

    DrawReference(h_ratio2, h_ratio_ref2, false);
  }

  {
    p2 = (TPad *)c2->cd(idx2++);
    c2->Update();
    //    p->SetLogx();
    TH1 *frame2 = p2->DrawFrame(0, .9, 50, 1.1,
                              "Mean and sigma, p_{T,matched}/p_{T,reco};Reco p_{T} [GeV/c];<p_{T,matched}/p_{T,reco}> #pm #sigma(p_{T,matched}/p_{T,reco})");
    // gPad->SetLeftMargin(.2);
    gPad->SetTopMargin(-1);
    frame2->GetYaxis()->SetTitleOffset(1.7);
    //TLine *l2 = new TLine(0, 1, 50, 1);
    //l2->SetLineColor(kGray);
    //l2->Draw();
    HorizontalLine( gPad, 1 )->Draw();
    

    TH2 *h_QAG4SimulationTracking_pTRecoTruthMatchedRatio_pTReco_cuts =
        (TH2 *)qa_file_new->GetObjectChecked(
            prefix + "pTRecoTruthMatchedRatio_pTReco_cuts", "TH2");
    assert(h_QAG4SimulationTracking_pTRecoTruthMatchedRatio_pTReco_cuts);

    h_QAG4SimulationTracking_pTRecoTruthMatchedRatio_pTReco_cuts->Rebin2D(16, 1);

    TGraphErrors *ge_QAG4SimulationTracking_pTRecoTruthMatchedRatio_pTReco_cuts =
        FitProfile(h_QAG4SimulationTracking_pTRecoTruthMatchedRatio_pTReco_cuts);
    ge_QAG4SimulationTracking_pTRecoTruthMatchedRatio_pTReco_cuts->Draw("pe");
     ge_QAG4SimulationTracking_pTRecoTruthMatchedRatio_pTReco_cuts->SetTitle(
        "Mean and sigma, p_{T,matched}/p_{T,reco}");

    TGraphErrors *h_ratio_ref2 = NULL;
    if (qa_file_ref) {
      TH2 *h_QAG4SimulationTracking_pTRecoTruthMatchedRatio_pTReco_cuts =
          (TH2 *)qa_file_ref->GetObjectChecked(
              prefix + "pTRecoTruthMatchedRatio_pTReco_cuts", "TH2");
      assert(h_QAG4SimulationTracking_pTRecoTruthMatchedRatio_pTReco_cuts);

      h_QAG4SimulationTracking_pTRecoTruthMatchedRatio_pTReco_cuts->Rebin2D(16, 1);

      h_ratio_ref2 =
          FitProfile(h_QAG4SimulationTracking_pTRecoTruthMatchedRatio_pTReco_cuts);
      ge_QAG4SimulationTracking_pTRecoTruthMatchedRatio_pTReco_cuts->Draw("pe");
    }

    DrawReference(ge_QAG4SimulationTracking_pTRecoTruthMatchedRatio_pTReco_cuts,
                  h_ratio_ref2, true);
  }

  SaveCanvas(c2,
             TString(qa_file_name_new) + TString("_") + TString(c2->GetName()),
             true);
}
