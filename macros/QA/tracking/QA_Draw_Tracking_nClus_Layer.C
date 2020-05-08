// $Id: $

/*!
 * \file QA_Draw_Tracking_nClus_Layer.C
 * \brief
 * \author Hugo Pereira Da Costa <hugo.pereira-da-costa@cea.fr>
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

namespace  {

  // Normalization
  double Nevent_new = 1;
  double Nevent_ref = 1;

  void GetNormalization( TFile* qa_file_new, TFile* qa_file_ref, const TString& prefix, const TString& tag )
  {
    if (qa_file_new)
    {
      TH1 *h_norm = (TH1 *) qa_file_new->GetObjectChecked( prefix + TString("Normalization"), "TH1");
      assert(h_norm);
      Nevent_new = h_norm->GetBinContent(h_norm->GetXaxis()->FindBin( tag ));
    }

    if (qa_file_ref)
    {
      TH1 *h_norm = (TH1 *) qa_file_ref->GetObjectChecked( prefix + TString("Normalization"), "TH1");
      assert(h_norm);
      Nevent_ref = h_norm->GetBinContent(h_norm->GetXaxis()->FindBin( tag ));
    }
  }

  void Draw( TFile* qa_file_new, TFile* qa_file_ref, const TString& prefix, const TString& tag )
  {

    auto h_new = static_cast<TH1*>( qa_file_new->GetObjectChecked( prefix + tag, "TH1" ) );
    assert(h_new);
    h_new->Sumw2();
    h_new->Scale(1./Nevent_new);

    TH1 *h_ref = nullptr;
    if (qa_file_ref)
    {
      h_ref = static_cast<TH1*>( qa_file_ref->GetObjectChecked( prefix + tag, "TH1") );
      assert(h_ref);
      h_ref->Sumw2();
      h_ref->Scale(1.0/Nevent_ref);
    }

    DrawReference(h_new, h_ref);
    HorizontalLine( gPad, 1 )->Draw();
  }

}

void QA_Draw_Tracking_nClus_Layer(
    const char *hist_name_prefix = "QAG4SimulationTracking",
    const char *qa_file_name_new = "data/G4sPHENIX.root_qa.root",
    const char *qa_file_name_ref = "data/G4sPHENIX.root_qa.root")
{
  SetsPhenixStyle();

  TString prefix = TString("h_") + hist_name_prefix + TString("_");

  TFile *qa_file_new = new TFile(qa_file_name_new);
  assert(qa_file_new->IsOpen());

  TFile *qa_file_ref = nullptr;
  if (qa_file_name_ref)
  {
    qa_file_ref = new TFile(qa_file_name_ref);
    assert(qa_file_ref->IsOpen());
  }

  auto c1 = new TCanvas(TString("QA_Draw_Tracking_nClus_Layer") + TString("_") + hist_name_prefix,
    TString("QA_Draw_Tracking_nClus_Layer") + TString("_") + hist_name_prefix,
    1800, 1000);

  c1->Divide( 2, 1 );
  c1->cd(1);
  GetNormalization( qa_file_new, qa_file_ref, prefix, "Truth Track" );
  Draw( qa_file_new, qa_file_ref, prefix, "nClus_layerGen" );

  c1->cd(2);
  GetNormalization( qa_file_new, qa_file_ref, prefix, "Reco Track" );
  Draw( qa_file_new, qa_file_ref, prefix, "nClus_layer" );

  SaveCanvas(c1, TString(qa_file_name_new) + TString("_") + TString(c1->GetName()), true);
}
