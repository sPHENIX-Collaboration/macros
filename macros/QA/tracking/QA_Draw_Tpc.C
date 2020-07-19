/*!
 * \file QA_Draw_Tpc.C
 * \brief
 * \author Tony Frawley <afrawley@fsu.edu>
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

static constexpr int nregions_tpc = 3;

namespace
{

  TCanvas* Draw( TFile* qa_file_new, TFile* qa_file_ref, const TString& hist_name_prefix, const TString& tag )
  {
    
    const TString prefix = TString("h_") + hist_name_prefix + TString("_");
    
    auto cv = new TCanvas(
			  TString("QA_Draw_Tpc_") + tag + TString("_") + hist_name_prefix,
			  TString("QA_Draw_Tpc_") + tag + TString("_") + hist_name_prefix,
      1800, 1000);

    cv->Divide( nregions_tpc, 1 );
    for( int region = 0; region < nregions_tpc; ++region )
    {

      // get histograms
      auto hnew = static_cast<TH1*>( qa_file_new->GetObjectChecked( Form( "%s%s_%i", prefix.Data(), tag.Data(), region ), "TH1" ) );
      hnew->Scale( 1./hnew->GetEntries() );
      hnew->SetMinimum(0);

      // reference
      auto href = qa_file_ref ? static_cast<TH1*>( qa_file_ref->GetObjectChecked( Form( "%s%s_%i", prefix.Data(), tag.Data(), region ), "TH1" ) ) : nullptr;
      if( href )
      {
        href->Scale( 1./href->GetEntries() );
        href->SetMinimum(0);
      }

      // draw
      cv->cd( region+1 );
      DrawReference(hnew, href);

      auto line = VerticalLine( gPad, 0 );
      line->Draw();
    }

    return cv;

  }

  TCanvas* Draw_eff( TFile* qa_file_new, TFile* qa_file_ref, const TString& hist_name_prefix, const TString& tag )
  {
    
    const TString prefix = TString("h_") + hist_name_prefix + TString("_");
    
    auto cv = new TCanvas(
			  TString("QA_Draw_Tpc_") + tag + TString("_") + hist_name_prefix,
			  TString("QA_Draw_Tpc_") + tag + TString("_") + hist_name_prefix,
			  1800, 1000);

    // get histograms
    auto hnew0 = static_cast<TH1*>( qa_file_new->GetObjectChecked( Form( "%s%s_0", prefix.Data(), tag.Data()), "TH1" ) );
    auto hnew1 = static_cast<TH1*>( qa_file_new->GetObjectChecked( Form( "%s%s_1", prefix.Data(), tag.Data()), "TH1" ) );
    
    hnew1->Divide(hnew1, hnew0, 1, 1, "B");
    hnew1->SetMinimum(0);
    
    // reference
    auto href0 = qa_file_ref ? static_cast<TH1*>( qa_file_ref->GetObjectChecked( Form( "%s%s_0", prefix.Data(), tag.Data()), "TH1" ) ) : nullptr;
    auto href1 = qa_file_ref ? static_cast<TH1*>( qa_file_ref->GetObjectChecked( Form( "%s%s_1", prefix.Data(), tag.Data()), "TH1" ) ) : nullptr;
    if( href0 )
      {
	href1->Divide(href1, href0, 1, 1, "B");
	href1->SetMinimum(0);
      }
    
    // draw
    DrawReference(hnew1, href1);
        
    auto line = HorizontalLine( gPad, 1 );
    line->Draw();
    
    
    return cv;
    
  }
  
}

void QA_Draw_Tpc(
		 const char *hist_name_prefix = "QAG4SimulationTpc",
		 const char *qa_file_name_new =  "data/G4sPHENIX.root_qa.root",
		 const char *qa_file_name_ref =  "data/200evts_G4sPHENIX.root_qa.root"
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

  // cluster efficiency
  cvlist.push_back( Draw_eff( qa_file_new, qa_file_ref, hist_name_prefix, "efficiency" ) );

  // phi residuals, errors and pulls
  cvlist.push_back( Draw( qa_file_new, qa_file_ref, hist_name_prefix, "drphi" ) );
  cvlist.push_back( Draw( qa_file_new, qa_file_ref, hist_name_prefix, "rphi_error" ) );
  cvlist.push_back( Draw( qa_file_new, qa_file_ref, hist_name_prefix, "phi_pulls" ) );

  // z residuals, errors and pulls
  cvlist.push_back( Draw( qa_file_new, qa_file_ref, hist_name_prefix, "dz" ) );
  cvlist.push_back( Draw( qa_file_new, qa_file_ref, hist_name_prefix, "z_error" ) );
  cvlist.push_back( Draw( qa_file_new, qa_file_ref, hist_name_prefix, "z_pulls" ) );

  // cluster sizes
  cvlist.push_back( Draw( qa_file_new, qa_file_ref, hist_name_prefix, "clus_size" ) );
  cvlist.push_back( Draw( qa_file_new, qa_file_ref, hist_name_prefix, "clus_size_phi" ) );
  cvlist.push_back( Draw( qa_file_new, qa_file_ref, hist_name_prefix, "clus_size_z" ) );

  for( const auto& cv:cvlist )
  { SaveCanvas(cv, TString(qa_file_name_new) + TString("_") + TString(cv->GetName()), true); }

}
