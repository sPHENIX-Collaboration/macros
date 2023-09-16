#ifndef MACRO_TRKREVAL_C
#define MACRO_TRKREVAL_C

#include <GlobalVariables.C>
#include <G4_TrkrVariables.C>
#include <Trkr_TruthTables.C>
#include <g4eval/SvtxEvaluator.h>
#include <g4eval/TruthRecoTrackMatching.h>
#include <g4eval/TrkrClusterIsMatcher.h>
#include <g4eval/FillClusMatchTree.h>

R__LOAD_LIBRARY(libg4eval.so)

void Tracking_Eval(const std::string& outputfile)
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::TRACKING_VERBOSITY);

  //---------------
  // Fun4All server
  //---------------

  Fun4AllServer* se = Fun4AllServer::instance();
  build_truthreco_tables(); 

  //----------------
  // Tracking evaluation
  //----------------
  SvtxEvaluator* eval;
  eval = new SvtxEvaluator("SVTXEVALUATOR", outputfile, "SvtxTrackMap",
                           G4MVTX::n_maps_layer,
                           G4INTT::n_intt_layer,
                           G4TPC::n_gas_layer,
                           G4MICROMEGAS::n_micromegas_layer);
  eval->do_cluster_eval(true);
  eval->do_g4hit_eval(false);
  eval->do_hit_eval(false);  // enable to see the hits that includes the chamber physics...
  eval->do_gpoint_eval(true);
  eval->do_vtx_eval_light(true);
  eval->do_eval_light(true);
  eval->do_track_eval(true);
  eval->do_gtrack_eval(true);
  eval->do_track_match(true);
  eval->set_use_initial_vertex(G4TRACKING::g4eval_use_initial_vertex);
  bool embed_scan = true;
  if(TRACKING::pp_mode) embed_scan = false;
  eval->scan_for_embedded(embed_scan);   // take all tracks if false - take only embedded tracks if true
  eval->scan_for_primaries(embed_scan);  // defaults to only thrown particles for ntp_gtrack
  std::cout << "SvtxEvaluator: pp_mode set to " << TRACKING::pp_mode << " and scan_for_embedded set to " << embed_scan << std::endl;
  eval->Verbosity(verbosity);
 
  se->registerSubsystem(eval);

  return;
}

void Track_Matching(const std::string& ttreefilename) {
  TrkrClusterIsMatcher* ismatcher = new TrkrClusterIsMatcher();
  // These are the default values -- uncomment and change as desired
  //  ismatcher->single_pixel_phi_MVTX = false ; // default to pitch*max(N_pixels_M,N_pixels_T)*tol_MVTX
  //  ismatcher->single_pixel_phi_INTT = false ; // ... same as for MVTX
  //  ismatcher->single_bin_phi_TPC    = true  ;  // default to pitch*tol_phi_TPC
  //
  //  ismatcher->single_pixel_z_MVTX = false ; // default to pitch*max(N_pixels_M,N_pixels_T)*tol_z_MVTX
  //  ismatcher->single_pixel_z_INTT = false ; // ... same as for MVTX
  //  ismatcher->single_bin_t_TPC    = true  ;  // default to pitch*tol_t_TPC
  //
  //  ismatcher-> tol_phi_MVTX = 0.5;
  //  ismatcher-> tol_phi_INTT = 0.5;
  //  ismatcher-> tol_phi_TPC  = 1.0;

  //  ismatcher-> tol_z_MVTX  = 0.5;
  //  ismatcher-> tol_t_TPC   = 1.0;
  auto trackmatcher = new TruthRecoTrackMatching(ismatcher);
  trackmatcher->set_min_cl_match     (5);    // minimum number of matched clusters to make a matched track
  trackmatcher->set_min_cl_ratio     (0.1);  // at least 10% of truth clusters must be matched
  trackmatcher->set_cutoff_deta      (0.3);  // won't compare tracks with |Δeta|>0.3 away
  trackmatcher->set_cutoff_dphi      (0.3);  // won't compare tracks with |Δphi|>0.3 away
  trackmatcher->set_smallsearch_deta (0.05); // will first compare tracks within this |Δphi|
  trackmatcher->set_smallsearch_dphi (0.05); // will first compare tracks within this |Δeta|
  trackmatcher->set_max_nreco_per_truth (4); // maximum reco tracks matched for any truth track
  trackmatcher->set_max_ntruth_per_reco (4); // maximum truth tracks matched for any reco track
  int verbosity = std::max(Enable::VERBOSITY, Enable::TRACKING_VERBOSITY);

  Fun4AllServer *se = Fun4AllServer::instance();
  trackmatcher->Verbosity(verbosity);
  se->registerSubsystem(trackmatcher);

  if (Enable::TRACK_MATCHING_TREE) {
    auto treefiller = new FillClusMatchTree(ismatcher, ttreefilename);//, true, true, true, outputFile);
    treefiller->Verbosity(verbosity);
    if (Enable::TRACK_MATCHING_TREE_CLUSTERS) {
      treefiller->m_fill_clusters     = true;
      treefiller->m_fill_SvUnmatched  = true;
    } else {
      treefiller->m_fill_clusters     = false;
      treefiller->m_fill_SvUnmatched  = false;
    }
    treefiller->m_fill_clusverbose = false;
    se->registerSubsystem(treefiller);
  }
}

#endif
