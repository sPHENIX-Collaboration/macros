#ifndef MACRO_TRKREVAL_C
#define MACRO_TRKREVAL_C

#include <GlobalVariables.C>
#include <G4_TrkrVariables.C>
#include <Trkr_TruthTables.C>
#include <g4eval/SvtxEvaluator.h>

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
  eval->set_use_initial_vertex(G4TRACKING::g4eval_use_initial_vertex);
  bool embed_scan = true;
  if(TRACKING::pp_mode) embed_scan = false;
  eval->scan_for_embedded(embed_scan);   // take all tracks if false - take only embedded tracks if true
  eval->scan_for_primaries(embed_scan);  // defaults to only thrown particles for ntp_gtrack
  std::cout << "SvtxEvaluator: pp_mode set to " << TRACKING::pp_mode << " and scan_for_embedded set to " << embed_scan << std::endl;
  eval->Verbosity(verbosity);
  eval->set_cluster_version(G4TRACKING::cluster_version);

  se->registerSubsystem(eval);

  return;
}


#endif
