// $Id: $

/*!
 * \file QA_Draw_nVertex.C
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

// some common style files
#include "../../sPHENIXStyle/sPhenixStyle.C"
#include "QA_Draw_Utility.C"
using namespace std;

void QA_Draw_nVertex(
    const char *hist_name_prefix = "QAG4SimulationVertex",
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

  TCanvas *c1 = new TCanvas(TString("QA_Draw_Vertex_nVertex") +
                                TString("_") + hist_name_prefix,
                            TString("QA_Draw_Vertex_nVertex") +
                                TString("_") + hist_name_prefix,
                            1800, 1000);
  c1->Divide(2, 2);
  int idx = 1;
  TPad *p;

  {
    static const int nrebin = 1;

    p = (TPad *)c1->cd(idx++);
    c1->Update();
    // p->SetLogx();
    p->SetGridy();

    TH1 *h_pass =
        (TH1 *)qa_file_new->GetObjectChecked(prefix + "gntracks", "TH1");
    assert(h_pass);

    h_pass->Rebin(nrebin);

    //    h_ratio->GetXaxis()->SetRangeUser(min_Et, max_Et);
    h_pass->GetYaxis()->SetTitle("Counts");
    // h_pass->GetYaxis()->SetRangeUser(-0, 1.);

    TH1 *h_ref = NULL;
    if (qa_file_ref) {
      TH1 *h_ref =
          (TH1 *)qa_file_ref->GetObjectChecked(prefix + "gntracks", "TH1");
      assert(h_pass);

      h_ref->Rebin(nrebin);
    }

    h_pass->SetTitle(TString(hist_name_prefix) + ": gntracks");

    DrawReference(h_pass, h_ref, false);
  }

  {
    static const int nrebin = 1;
    
    p = (TPad *)c1->cd(idx++);
    c1->Update();
    // p->SetLogx();
    p->SetGridy();

    TH1 *h_pass =
        (TH1 *)qa_file_new->GetObjectChecked(prefix + "gntracksmaps", "TH1");
    assert(h_pass);

    h_pass->Rebin(nrebin);

    //    h_ratio->GetXaxis()->SetRangeUser(min_Et, max_Et);
    h_pass->GetYaxis()->SetTitle("Counts");
    // h_pass->GetYaxis()->SetRangeUser(-0, 1.);

    TH1 *h_ref = NULL;
    if (qa_file_ref) {
      TH1 *h_ref =
          (TH1 *)qa_file_ref->GetObjectChecked(prefix + "gntracksmaps", "TH1");
      assert(h_pass);

      h_ref->Rebin(nrebin);
    }

    h_pass->SetTitle(TString(hist_name_prefix) + ": gntracksmaps");

    DrawReference(h_pass, h_ref, false);
  }

  {
    static const int nrebin = 1;
    
    p = (TPad *)c1->cd(idx++);
    c1->Update();
    // p->SetLogx();
    p->SetGridy();

    TH1 *h_pass =
        (TH1 *)qa_file_new->GetObjectChecked(prefix + "ntracks", "TH1");
    assert(h_pass);

    h_pass->Rebin(nrebin);

    //    h_ratio->GetXaxis()->SetRangeUser(min_Et, max_Et);
    h_pass->GetYaxis()->SetTitle("Counts");
    // h_pass->GetYaxis()->SetRangeUser(-0, 1.);

    TH1 *h_ref = NULL;
    if (qa_file_ref) {
      TH1 *h_ref =
          (TH1 *)qa_file_ref->GetObjectChecked(prefix + "ntracks", "TH1");
      assert(h_pass);

      h_ref->Rebin(nrebin);
    }

    h_pass->SetTitle(TString(hist_name_prefix) + ": ntracks");

    DrawReference(h_pass, h_ref, false);
  }

  {
    static const int nrebin = 1;
    
    p = (TPad *)c1->cd(idx++);
    c1->Update();
    // p->SetLogx();
    p->SetGridy();

    TH1 *h_pass =
        (TH1 *)qa_file_new->GetObjectChecked(prefix + "ntracks_cuts", "TH1");
    assert(h_pass);

    h_pass->Rebin(nrebin);

    //    h_ratio->GetXaxis()->SetRangeUser(min_Et, max_Et);
    h_pass->GetYaxis()->SetTitle("Counts");
    // h_pass->GetYaxis()->SetRangeUser(-0, 1.);

    TH1 *h_ref = NULL;
    if (qa_file_ref) {
      TH1 *h_ref =
          (TH1 *)qa_file_ref->GetObjectChecked(prefix + "ntracks_cuts", "TH1");
      assert(h_pass);

      h_ref->Rebin(nrebin);
    }

    h_pass->SetTitle(TString(hist_name_prefix) + ": ntracks (#geq 2 MVTX)");

    DrawReference(h_pass, h_ref, false);
  }

  SaveCanvas(c1,
             TString(qa_file_name_new) + TString("_") + TString(c1->GetName()),
             true);
}
