// $Id: $

/*!
 * \file QA_Draw_Tracking_TruthMatching_NumOfClusters.C
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

void QA_Draw_Tracking_TruthMatching_NumOfClusters(
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

  //MVTX, INTT, TPC
  vector<TString> detectors{"MVTX", "INTT", "TPC"};
  vector<int> eff_ncluster_cuts{2, 2, 40};
  vector<double> ncluster_spectrum_pt_cuts{2, 2, 2};
  vector<TH2 *> h_pass_detectors(3, nullptr);
  static const int nrebin = 5;

  h_pass_detectors[0] = (TH2 *) qa_file_new->GetObjectChecked(
      prefix + "nMVTX_nReco_pTGen", "TH1");
  h_pass_detectors[1] = (TH2 *) qa_file_new->GetObjectChecked(
      prefix + "nINTT_nReco_pTGen", "TH1");
  h_pass_detectors[2] = (TH2 *) qa_file_new->GetObjectChecked(
      prefix + "nTPC_nReco_pTGen", "TH1");

  TH1 *h_norm = (TH1 *) qa_file_new->GetObjectChecked(
      prefix + "nGen_pTGen", "TH1");
  assert(h_norm);
  h_norm->Rebin(nrebin);

  vector<TH2 *> h_pass_detectors_ref(3, nullptr);
  TH1 *h_norm_ref = nullptr;
  if (qa_file_ref)
  {
    h_pass_detectors_ref[0] = (TH2 *) qa_file_ref->GetObjectChecked(
        prefix + "nMVTX_nReco_pTGen", "TH1");
    h_pass_detectors_ref[1] = (TH2 *) qa_file_ref->GetObjectChecked(
        prefix + "nINTT_nReco_pTGen", "TH1");
    h_pass_detectors_ref[2] = (TH2 *) qa_file_ref->GetObjectChecked(
        prefix + "nTPC_nReco_pTGen", "TH1");

    h_norm_ref = (TH1 *) qa_file_ref->GetObjectChecked(
        prefix + "nGen_pTGen", "TH1");
    h_norm_ref->Rebin(nrebin);
  }

  TCanvas *c1 = new TCanvas(TString("QA_Draw_Tracking_TruthMatching_NumOfClusters") + TString("_") + hist_name_prefix,
                            TString("QA_Draw_Tracking_TruthMatching_NumOfClusters") + TString("_") + hist_name_prefix,
                            1800, 1000);
  c1->Divide(3, 2);
  TPad *p;

  for (int i = 0; i < 3; ++i)
  {
    TString detector = detectors[i];
    TH2 *h_pass_detector = h_pass_detectors[i];
    TH2 *h_pass_detector_ref = h_pass_detectors_ref[i];
    assert(h_pass_detector);

    {
      p = (TPad *) c1->cd(i + 1);
      c1->Update();
      p->SetLogy();

      const int bin_start = h_pass_detector->GetXaxis()->FindBin(ncluster_spectrum_pt_cuts[i]);

      TH1 *h_pass_detector_ncluster = h_pass_detector->ProjectionY(
          TString(h_pass_detector->GetName()) + "_nCluster_new",
          bin_start);
      TH1 *h_pass_detector_ncluster_ref = nullptr;
      if (h_pass_detector_ref)
      {
        h_pass_detector_ncluster_ref = h_pass_detector_ref->ProjectionY(
            TString(h_pass_detector_ref->GetName()) + "_nCluster_ref",
            bin_start);
      }

      h_pass_detector_ncluster->SetTitle(TString(hist_name_prefix) + ": " + detector + Form(" n_{Cluster} | p_{T} #geq %.1fGeV/c", ncluster_spectrum_pt_cuts[i]));
      h_pass_detector_ncluster->SetYTitle("# of reconstructed track");
      DrawReference(h_pass_detector_ncluster, h_pass_detector_ncluster_ref, false);
    }

    {
      p = (TPad *) c1->cd(i + 3 + 1);
      c1->Update();
      p->SetLogx();
      p->SetGridy();

      const int bin_start = h_pass_detector->GetYaxis()->FindBin(eff_ncluster_cuts[i]);
      TH1 *h_pass = h_pass_detector->ProjectionX(
          TString(h_pass_detector->GetName()) + "_nReco_new",
          bin_start);

      assert(h_pass);
      h_pass->Rebin(nrebin);

      TH1 *h_ratio = GetBinominalRatio(h_pass, h_norm);
      h_ratio->GetYaxis()->SetTitle("Reco efficiency | " + detector + Form(" n_{Cluster} #geq %d", eff_ncluster_cuts[i]));
      h_ratio->GetYaxis()->SetRangeUser(-0, 1.);
      //
      TH1 *h_ratio_ref = NULL;
      if (h_pass_detector_ref)
      {
        TH1 *h_pass = h_pass_detector_ref->ProjectionX(
            TString(h_pass_detector->GetName()) + "_nReco_ref",
            bin_start);

        assert(h_pass);
        h_pass->Rebin(nrebin);

        h_ratio_ref = GetBinominalRatio(h_pass, h_norm);
      }
      //
      h_ratio->SetTitle("Tracking efficiency | " + detector + Form(" n_{Cluster} #geq %d", eff_ncluster_cuts[i]));
      DrawReference(h_ratio, h_ratio_ref, false);
    }
  }

  SaveCanvas(c1, TString(qa_file_name_new) + TString("_") + TString(c1->GetName()), true);
}
