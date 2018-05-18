#include <vector>

#include "G4_GEM_EIC.C"
#include "G4_TPC_EIC.C"

void SvtxInit(int verbosity = 0)
{

}

double Svtx(PHG4Reco* g4Reco, double radius,
            const int absorberactive = 0,
            int verbosity = 0)
{
  /* Place electron-going side tracking detectors */
  EGEMSetup(g4Reco);

  /* Place hadron-going side tracking detectors */
  FGEMSetup(g4Reco);

  /* Place central tracking detectors */
  TPC(g4Reco, radius);

  return;
}

void Svtx_Reco(int verbosity = 0)
{
  //---------------
  // Load libraries
  //---------------

  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4hough.so");

  //---------------
  // Fun4All server
  //---------------

  Fun4AllServer* se = Fun4AllServer::instance();

  //---------------------
  // Kalman Filter
  //---------------------

  PHG4TrackFastSim* kalman = new PHG4TrackFastSim("PHG4TrackFastSim");
  kalman->Verbosity(10);

  kalman->set_use_vertex_in_fitting(true);
  kalman->set_vertex_xy_resolution(50E-4);
  kalman->set_vertex_z_resolution(50E-4);

  kalman->set_detector_type(PHG4TrackFastSim::Vertical_Plane); // Vertical_Plane, Cylinder
  kalman->set_phi_resolution(50E-4);
  kalman->set_r_resolution(1.);

  kalman->set_pat_rec_hit_finding_eff(1.);
  kalman->set_pat_rec_noise_prob(0.);

  std::string phg4hits_names[] = {"G4HIT_SVTX","G4HIT_EGEM_0","G4HIT_EGEM_1","G4HIT_EGEM_2","G4HIT_EGEM_3","G4HIT_FGEM_0","G4HIT_FGEM_1","G4HIT_FGEM_2","G4HIT_FGEM_3","G4HIT_FGEM_4"};
  kalman->set_phg4hits_names(phg4hits_names, 10);
  kalman->set_sub_top_node_name("SVTX");
  kalman->set_trackmap_out_name("SvtxTrackMap");

  kalman->set_fit_alg_name("KalmanFitterRefTrack");//
  kalman->set_primary_assumption_pid(13);
  kalman->set_do_evt_display(false);

  se->registerSubsystem(kalman);

  return;
}


void Svtx_Cells(int verbosity = 0)
{
}


void G4_Svtx_Reco()
{
  cout << "\033[31;1m"
       << "Warning: G4_Svtx_Reco() was moved to G4_Svtx.C and renamed to Svtx_Reco(), please update macros"
       << "\033[0m" << endl;
  Svtx_Reco();

  return;
}

void Svtx_Eval(std::string outputfile, int verbosity = 0)
{
  //---------------
  // Load libraries
  //---------------

  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4detectors.so");
  gSystem->Load("libg4hough.so");
  gSystem->Load("libg4eval.so");

  //---------------
  // Fun4All server
  //---------------

  Fun4AllServer* se = Fun4AllServer::instance();

  //----------------
  // SVTX evaluation
  //----------------

  SvtxEvaluator* eval;
  eval = new SvtxEvaluator("SVTXEVALUATOR", outputfile.c_str());
  eval->do_cluster_eval(false);
  eval->do_g4hit_eval(false);
  eval->do_hit_eval(false);  // enable to see the hits that includes the chamber physics...
  eval->do_gpoint_eval(false);
  eval->scan_for_embedded(false);  // take all tracks if false - take only embedded tracks if true
  eval->Verbosity(verbosity);
  se->registerSubsystem(eval);

  // MomentumEvaluator* eval = new MomentumEvaluator(outputfile.c_str(),0.2,0.4,Max_si_layer,2,Max_si_layer-4,10.,80.);
  // se->registerSubsystem( eval );

  return;
}
