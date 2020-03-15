// $Id: $

/*!
 * \file QA_Draw_Tracking_TruthMatchingOverview.C
 * \brief 
 * \author Jin Huang <jhuang@bnl.gov>
 * \version $Revision:   $
 * \date $Date: $
 */

#include <TFile.h>
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

void QA_Draw_Tracking_TruthMatchingOverview(
    const char *qa_file_name_new =
        "data/G4sPHENIX.root_qa.root",
    const char *qa_file_name_ref =
        "data/G4sPHENIX.root_qa.root")
{
  SetsPhenixStyle();

  TVirtualFitter::SetDefaultFitter("Minuit2");

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
        TString("h_QAG4SimulationTracking_Normalization"), "TH1");
    assert(h_norm);

    Nevent_new = h_norm->GetBinContent(h_norm->GetXaxis()->FindBin("Event"));
  }
  if (qa_file_ref)
  {
    TH1 *h_norm = (TH1 *) qa_file_ref->GetObjectChecked(
        TString("h_QAG4SimulationTracking_Normalization"), "TH1");
    assert(h_norm);

    Nevent_ref = h_norm->GetBinContent(h_norm->GetXaxis()->FindBin("Event"));
  }

  TCanvas *c1 = new TCanvas(TString("QA_Draw_Tracking_TruthMatchingOverview"),
                            TString("QA_Draw_Tracking_TruthMatchingOverview"), 1800, 1000);
  c1->Divide(2, 1);
  int idx = 1;
  TPad *p;


  {

    p = (TPad *) c1->cd(idx++);
    c1->Update();
    p->SetLogx();

    TH1 *h_pass = (TH1 *) qa_file_new->GetObjectChecked(
        "h_QAG4SimulationTracking_nReco_pTGen", "TH1");
    TH1 *h_norm = (TH1 *) qa_file_new->GetObjectChecked(
        "h_QAG4SimulationTracking_nGen_pTGen", "TH1");
    assert(h_norm);
    assert(h_pass);
    TH1 *h_ratio = GetBinominalRatio(h_pass, h_norm);

//    h_ratio->GetXaxis()->SetRangeUser(min_Et, max_Et);
    h_ratio->GetYaxis()->SetTitle("Reco efficiency");
    h_ratio->GetYaxis()->SetRangeUser(-0, 1.2);

    TH1 *h_ratio_ref = NULL;
    if (qa_file_ref)
    {
      TH1 *h_pass = (TH1 *) qa_file_ref->GetObjectChecked(
          "h_QAG4SimulationTracking_nReco_pTGen", "TH1");
      TH1 *h_norm = (TH1 *) qa_file_ref->GetObjectChecked(
          "h_QAG4SimulationTracking_nGen_pTGen", "TH1");
      assert(h_norm);
      assert(h_pass);
      TH1 *h_ratio_ref = GetBinominalRatio(h_pass, h_norm);
    }

    DrawReference(h_ratio, h_ratio_ref, true);

  }
}
