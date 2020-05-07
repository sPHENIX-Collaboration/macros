/*!
 * \file QA_Draw_MVtx.C
 * \brief
 * \author Hugo Pereira Da Costa <hugo.pereira-da-costa@cea.fr>
 * \version $Revision:   $
 * \date $Date: $
 */

#include <TFile.h>
#include <TH1.h>
#include <TLine.h>
#include <TString.h>

//some common style files
#include "../../sPHENIXStyle/sPhenixStyle.C"
#include "QA_Draw_Utility.C"

// assume MVTX layers are 0, 1 and 2
static constexpr int first_layer_mvtx = 0;
static constexpr int nlayers_mvtx = 3;

namespace
{

  TCanvas* Draw( TFile* qa_file_new, TFile* qa_file_ref, const TString& hist_name_prefix, const TString& tag )
  {

    const TString prefix = TString("h_") + hist_name_prefix + TString("_");

    auto cv = new TCanvas(
      TString("QA_Draw_Mvtx_") + tag + TString("_") + hist_name_prefix,
      TString("QA_Draw_Mvtx_") + tag + TString("_") + hist_name_prefix,
      1800, 1000);

    cv->Divide( nlayers_mvtx, 1 );
    for( int ilayer = 0; ilayer < nlayers_mvtx; ++ilayer )
    {

      const int layer = ilayer + first_layer_mvtx;

      // get histograms
      auto hnew = static_cast<TH1*>( qa_file_new->GetObjectChecked( Form( "%s%s_%i", prefix.Data(), tag.Data(), layer ), "TH1" ) );
      hnew->Scale( 1./hnew->GetEntries() );
      hnew->SetMinimum(0);

      // reference
      auto href = qa_file_ref ? static_cast<TH1*>( qa_file_ref->GetObjectChecked( Form( "%s%s_%i", prefix.Data(), tag.Data(), layer ), "TH1" ) ) : nullptr;
      if( href )
      {
        href->Scale( 1./href->GetEntries() );
        href->SetMinimum(0);
      }

      // draw
      cv->cd( ilayer+1 );
      DrawReference(hnew, href);

      auto line = VerticalLine( gPad, 0 );
      line->Draw();
    }

    return cv;

  }

}

void QA_Draw_Mvtx(
    const char *hist_name_prefix = "QAG4SimulationMvtx",
    const char *qa_file_name_new =  "data/G4sPHENIX.root_qa.root",
    const char *qa_file_name_ref =  "data/G4sPHENIX.root_qa.root"
    )
{

  SetsPhenixStyle();

  auto qa_file_new = new TFile(qa_file_name_new);
  assert(qa_file_new->IsOpen());

  TFile* qa_file_ref = nullptr;
  if (qa_file_name_ref)
  {
    qa_file_ref = new TFile(qa_file_name_ref);
    assert(qa_file_ref->IsOpen());
  }
  
  std::vector<TCanvas*> cvlist;

  cvlist.push_back( Draw( qa_file_new, qa_file_ref, hist_name_prefix, "drphi" ) );
  cvlist.push_back( Draw( qa_file_new, qa_file_ref, hist_name_prefix, "rphi_error" ) );
  cvlist.push_back( Draw( qa_file_new, qa_file_ref, hist_name_prefix, "phi_pulls" ) );

  cvlist.push_back( Draw( qa_file_new, qa_file_ref, hist_name_prefix, "dz" ) );
  cvlist.push_back( Draw( qa_file_new, qa_file_ref, hist_name_prefix, "z_error" ) );
  cvlist.push_back( Draw( qa_file_new, qa_file_ref, hist_name_prefix, "z_pulls" ) );

  for( const auto& cv:cvlist )
  { SaveCanvas(cv, TString(qa_file_name_new) + TString("_") + TString(cv->GetName()), true); }

}
