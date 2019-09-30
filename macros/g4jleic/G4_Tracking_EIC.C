#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6, 00, 0)
#include <fun4all/Fun4AllServer.h>
#include <g4eval/SvtxEvaluator.h>
#include <g4trackfastsim/PHG4TrackFastSim.h>
#include <g4trackfastsim/PHG4TrackFastSimEval.h>
class SubsysReco;
R__LOAD_LIBRARY(libg4eval.so)
R__LOAD_LIBRARY(libg4trackfastsim.so)
#endif


void Tracking_Reco(int verbosity = 0)
{
  //---------------
  // Load libraries
  //---------------

  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4trackfastsim.so");

  //---------------
  // Fun4All server
  //---------------

  Fun4AllServer *se = Fun4AllServer::instance();

  PHG4TrackFastSim *kalman = new PHG4TrackFastSim("PHG4TrackFastSim");
  kalman->Verbosity(verbosity);

  kalman->set_use_vertex_in_fitting(false);
//  kalman->set_vertex_xy_resolution(50E-4);
//  kalman->set_vertex_z_resolution(50E-4);

  kalman->set_sub_top_node_name("TRACKS");
  kalman->set_trackmap_out_name("KalFitTrackMap");

  //   VTX
  kalman->add_phg4hits(
      "G4HIT_JLVTX",                //      const std::string& phg4hitsNames,
      PHG4TrackFastSim::Cylinder,  //      const DETECTOR_TYPE phg4dettype,
      20e-4,                        //      const float radres,
      20e-4,                        //      const float phires,
      20e-4,                        //      const float lonres,
      1,                           //      const float eff,
      0                            //      const float noise
  );

  //   CTD
  kalman->add_phg4hits(
      "G4HIT_JLCTD",                //      const std::string& phg4hitsNames,
      PHG4TrackFastSim::Cylinder,  //      const DETECTOR_TYPE phg4dettype,
      5e-2,                        //      const float radres,
      5e-2,                        //      const float phires,
      5e-2,                        //      const float lonres,
      1,                           //      const float eff,
      0                            //      const float noise
  );
//
  // GEM0, 70um azimuthal resolution, 1cm radial strips
  kalman->add_phg4hits(
      "G4HIT_GEMHADRON",                    //      const std::string& phg4hitsNames,
      PHG4TrackFastSim::Vertical_Plane,  //      const DETECTOR_TYPE phg4dettype,
      1. / sqrt(12.),                    //      const float radres,
      70e-4,                             //      const float phires,
      100e-4,                            //      const float lonres,
      1,                                 //      const float eff,
      0                                  //      const float noise
  );
  // GEM1, 70um azimuthal resolution, 1cm radial strips
  kalman->add_phg4hits(
      "G4HIT_GEMHADRON",                    //      const std::string& phg4hitsNames,
      PHG4TrackFastSim::Vertical_Plane,  //      const DETECTOR_TYPE phg4dettype,
      1. / sqrt(12.),                    //      const float radres,
      70e-4,                             //      const float phires,
      100e-4,                            //      const float lonres,
      1,                                 //      const float eff,
      0                                  //      const float noise
  );

  // Saved track states (projections)
//  kalman->add_state_name("JLDIRC");
//  kalman->add_state_name("FHCAL");

  se->registerSubsystem(kalman);

  return;
}

void Tracking_Eval(std::string outputfile, int verbosity = 0)
{
  //---------------
  // Load libraries
  //---------------

  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4detectors.so");
  gSystem->Load("libg4trackfastsim.so");
  gSystem->Load("libg4eval.so");

  //---------------
  // Fun4All server
  //---------------

  Fun4AllServer *se = Fun4AllServer::instance();

  //----------------
  // SVTX evaluation
  //----------------

  //  SvtxEvaluator* eval;
  //  eval = new SvtxEvaluator("SVTXEVALUATOR", outputfile.c_str());
  //  eval->do_cluster_eval(false);
  //  eval->do_g4hit_eval(false);
  //  eval->do_hit_eval(false);  // enable to see the hits that includes the chamber physics...
  //  eval->do_gpoint_eval(false);
  //  eval->scan_for_embedded(false);  // take all tracks if false - take only embedded tracks if true
  //  eval->Verbosity(verbosity);
  //  se->registerSubsystem(eval);

  // MomentumEvaluator* eval = new MomentumEvaluator(outputfile.c_str(),0.2,0.4,Max_si_layer,2,Max_si_layer-4,10.,80.);
  // se->registerSubsystem( eval );

  PHG4TrackFastSimEval *fast_sim_eval = new PHG4TrackFastSimEval("FastTrackingEval",outputfile,"KalFitTrackMap");
  fast_sim_eval->set_filename(outputfile.c_str());
  se->registerSubsystem(fast_sim_eval);
}
