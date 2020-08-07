// $Id: $

/*!
 * \file QA_Draw_DCA_Resolution.C
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

void QA_Draw_DCA_Resolution(
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
    cout << "Open new QA file " << qa_file_new->GetName() << endl;

    TH1 *h_norm = (TH1 *) qa_file_new->GetObjectChecked(
        prefix + TString("Normalization"), "TH1");
    assert(h_norm);

    Nevent_new = h_norm->GetBinContent(h_norm->GetXaxis()->FindBin("Event"));
  }
  if (qa_file_ref)
  {
    cout << "Open ref QA file " << qa_file_ref->GetName() << endl;
    TH1 *h_norm = (TH1 *) qa_file_ref->GetObjectChecked(
        prefix + TString("Normalization"), "TH1");
    assert(h_norm);

    Nevent_ref = h_norm->GetBinContent(h_norm->GetXaxis()->FindBin("Event"));
  }

  TCanvas *c1 = new TCanvas(TString("QA_Draw_Tracking_DCA_Resolution") + TString("_") + hist_name_prefix,
                            TString("QA_Draw_Tracking_DCA_Resolution") + TString("_") + hist_name_prefix,
                            1800, 1000);
  c1->Divide(2, 1);
  int idx = 1;
  TPad *p;

  {
    p = (TPad *) c1->cd(idx++);
    c1->Update();
    p->SetLogx();
    TH1 *frame = p->DrawFrame(0.1, -0.01, 50, 0.01,
                              ";Truth p_{T} [GeV/c];<DCA (r #phi)> #pm #sigma(DCA (r #phi)) [cm]");
    gPad->SetLeftMargin(.2);
    frame->GetYaxis()->SetTitleOffset(2);
    TLine *l = new TLine(0.1, 0, 50, 0);
    l->SetLineColor(kGray);
    l->Draw();

    TH2 *h_QAG4SimulationTracking_DCArPhi = (TH2 *) qa_file_new->GetObjectChecked(
        prefix + "DCArPhi_pT_cuts", "TH2");
    assert(h_QAG4SimulationTracking_DCArPhi);
    
    h_QAG4SimulationTracking_DCArPhi->Rebin2D(20, 1);

    // h_QAG4SimulationTracking_DCArPhi->Draw("colz");
    TGraphErrors *ge_QAG4SimulationTracking_DCArPhi = FitProfile(h_QAG4SimulationTracking_DCArPhi);
    ge_QAG4SimulationTracking_DCArPhi->Draw("pe");

    TGraphErrors *h_ratio_ref = NULL;
    if (qa_file_ref)
    {
      TH2 *h_QAG4SimulationTracking_DCArPhi = (TH2 *) qa_file_ref->GetObjectChecked(
          prefix + "DCArPhi_pT_cuts", "TH2");
      assert(h_QAG4SimulationTracking_DCArPhi);
      
      h_QAG4SimulationTracking_DCArPhi->Rebin2D(20, 1);

      h_ratio_ref = FitProfile(h_QAG4SimulationTracking_DCArPhi);
      ge_QAG4SimulationTracking_DCArPhi->Draw("pe");
    }

    ge_QAG4SimulationTracking_DCArPhi->SetTitle("DCA (r #phi, #geq 2MVTX, #geq 1INTT, #geq 20TPC) [cm]"); 
    DrawReference(ge_QAG4SimulationTracking_DCArPhi, h_ratio_ref, true);
  }

  {
    p = (TPad *) c1->cd(idx++);
    c1->Update();
    p->SetLogx();
    TH1 *frame = p->DrawFrame(0.1, -0.01, 50, 0.01,
                              "DCA (Z) [cm];Truth p_{T} [GeV/c];<DCA (Z)> #pm #sigma(DCA (Z)) [cm]");
    // gPad->SetLeftMargin(.2);
    gPad->SetTopMargin(-1);
    frame->GetYaxis()->SetTitleOffset(1.7);
    //TLine *l = new TLine(0.1, 0, 50, 0);
    //l->SetLineColor(kGray);
    //l->Draw();
    HorizontalLine( gPad, 1 )->Draw();

    TH2 *h_QAG4SimulationTracking_DCAZ = (TH2 *) qa_file_new->GetObjectChecked(
        prefix + "DCAZ_pT_cuts", "TH2");
    assert(h_QAG4SimulationTracking_DCAZ);
 
    h_QAG4SimulationTracking_DCAZ->Rebin2D(40, 1);

    TGraphErrors *ge_QAG4SimulationTracking_DCAZ = FitProfile(h_QAG4SimulationTracking_DCAZ);
    ge_QAG4SimulationTracking_DCAZ->Draw("pe");
    ge_QAG4SimulationTracking_DCAZ->SetTitle("DCA (Z) [cm]");

    TGraphErrors *h_ratio_ref = NULL;
    if (qa_file_ref)
    {
      TH2 *h_QAG4SimulationTracking_DCAZ = (TH2 *) qa_file_ref->GetObjectChecked(
          prefix + "DCAZ_pT_cuts", "TH2");
      assert(h_QAG4SimulationTracking_DCAZ);
      
      h_QAG4SimulationTracking_DCAZ->Rebin2D(40, 1);

      h_ratio_ref = FitProfile(h_QAG4SimulationTracking_DCAZ);
      ge_QAG4SimulationTracking_DCAZ->Draw("pe");
    }

    DrawReference(ge_QAG4SimulationTracking_DCAZ, h_ratio_ref, true);
  }

  SaveCanvas(c1, TString(qa_file_name_new) + TString("_") + TString(c1->GetName()), true);
}
