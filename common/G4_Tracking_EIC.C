#ifndef MACRO_G4TRACKINGEIC_C
#define MACRO_G4TRACKINGEIC_C

#include <GlobalVariables.C>

#include <G4_CEmc_EIC.C>
#include <G4_FEMC_EIC.C>
#include <G4_FHCAL.C>
#include <G4_GEM_EIC.C>
#include <G4_Mvtx_EIC.C>
#include <G4_TPC_EIC.C>

#include <g4trackfastsim/PHG4TrackFastSim.h>

#include <trackreco/PHRaveVertexing.h>

#include <g4trackfastsim/PHG4TrackFastSimEval.h>

#include <fun4all/Fun4AllServer.h>

#include <vector>

R__LOAD_LIBRARY(libtrack_reco.so)
R__LOAD_LIBRARY(libg4trackfastsim.so)

namespace Enable
{
  bool TRACKING = false;
  bool TRACKING_EVAL = false;
  int TRACKING_VERBOSITY = 0;
}  // namespace Enable

namespace G4TRACKING
{
  bool DISPLACED_VERTEX = false;
  bool PROJECTION_CEMC = false;
  bool PROJECTION_FEMC = false;
  bool PROJECTION_FHCAL = false;
}  // namespace G4TRACKING

//-----------------------------------------------------------------------------//
void TrackingInit()
{
  TRACKING::TrackNodeName = "TrackMap";
}
//-----------------------------------------------------------------------------//
void Tracking_Reco()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::TRACKING_VERBOSITY);
  //---------------
  // Fun4All server
  //---------------

  Fun4AllServer *se = Fun4AllServer::instance();

  PHG4TrackFastSim *kalman = new PHG4TrackFastSim("PHG4TrackFastSim");
  //  kalman->Verbosity();
  //  kalman->Smearing(false);
  if (G4TRACKING::DISPLACED_VERTEX)
  {
    // do not use truth vertex in the track fitting,
    // which would lead to worse momentum resolution for prompt tracks
    // but this allows displaced track analysis including DCA and vertex finding
    kalman->set_use_vertex_in_fitting(false);
    kalman->set_vertex_xy_resolution(0);  // do not smear the vertex used in the built-in DCA calculation
    kalman->set_vertex_z_resolution(0);   // do not smear the vertex used in the built-in DCA calculation
    kalman->enable_vertexing(true);       // enable vertex finding and fitting
  }
  else
  {
    // constraint to a primary vertex and use it as part of the fitting level arm
    kalman->set_use_vertex_in_fitting(true);
    kalman->set_vertex_xy_resolution(50e-4);
    kalman->set_vertex_z_resolution(50e-4);
  }

  kalman->set_sub_top_node_name("TRACKS");
  kalman->set_trackmap_out_name(TRACKING::TrackNodeName);

  //-------------------------
  // Barrel
  //-------------------------
  if (Enable::BARREL)
  {
    kalman->add_phg4hits("G4HIT_BARREL",              //      const std::string& phg4hitsNames,
                         PHG4TrackFastSim::Cylinder,  //      const DETECTOR_TYPE phg4dettype,
                         5e-4,                        //      const float radres,
                         5e-4,                        //      const float phires,
                         5e-4,                        //      const float lonres,
                         1,                           //      const float eff,
                         0);                          //      const float noise
  }
  //-------------------------
  // MVTX
  //-------------------------
  if (Enable::MVTX)
  {
    //   MAPS
    kalman->add_phg4hits(
        "G4HIT_MVTX",                //      const std::string& phg4hitsNames,
        PHG4TrackFastSim::Cylinder,  //      const DETECTOR_TYPE phg4dettype,
        5e-4,                        //      const float radres,
        5e-4,                        //      const float phires,
        5e-4,                        //      const float lonres,
        1,                           //      const float eff,
        0                            //      const float noise
    );
  }
  //-------------------------
  // TPC
  //-------------------------
  if (Enable::TPC)
  {
    kalman->add_phg4hits(
        "G4HIT_TPC",                 //      const std::string& phg4hitsNames,
        PHG4TrackFastSim::Cylinder,  //      const DETECTOR_TYPE phg4dettype,
        1,                           //      const float radres,
        200e-4,                      //      const float phires,
        500e-4,                      //      const float lonres,
        1,                           //      const float eff,
        0                            //      const float noise
    );
  }
  //-------------------------
  // EGEM
  //-------------------------
  if (Enable::EGEM)
  {
    // GEM, 70um azimuthal resolution, 1cm radial strips
    for (int i = 0; i < 4; i++)
    {
      kalman->add_phg4hits(
          Form("G4HIT_EGEM_%d", i),          //      const std::string& phg4hitsNames,
          PHG4TrackFastSim::Vertical_Plane,  //      const DETECTOR_TYPE phg4dettype,
          1. / sqrt(12.),                    //      const float radres,
          70e-4,                             //      const float phires,
          100e-4,                            //      const float lonres,
          1,                                 //      const float eff,
          0                                  //      const float noise
      );
    }
  }
  //-------------------------
  // FGEM
  //-------------------------
  if (Enable::FGEM)
  {
    // GEM2, 70um azimuthal resolution, 1cm radial strips
    for (int i = 2; i < 5; i++)
    {
      kalman->add_phg4hits(Form("G4HIT_FGEM_%d", i),          //      const std::string& phg4hitsNames,
                           PHG4TrackFastSim::Vertical_Plane,  //      const DETECTOR_TYPE phg4dettype,
                           1. / sqrt(12.),                    //      const float radres,
                           70e-4,                             //      const float phires,
                           100e-4,                            //      const float lonres,
                           1,                                 //      const float eff,
                           0);                                //      const float noise
    }
  }
  //-------------------------
  // FST
  //-------------------------
  if (Enable::FST)
  {
    for (int i = 0; i < 5; i++)
    {
      kalman->add_phg4hits(Form("G4HIT_FST_%d", i),           //      const std::string& phg4hitsNames,
                           PHG4TrackFastSim::Vertical_Plane,  //      const DETECTOR_TYPE phg4dettype,
                           5e-4,                              //      const float radres,
                           5e-4,                              //      const float phires,
                           50e-4 / sqrt(12.),                 //      const float lonres,
                           1,                                 //      const float eff,
                           0);                                //      const float noise
    }
  }
  //-------------------------
  // FEMC
  //-------------------------
  // Saved track states (projections)
  if (Enable::FEMC && G4TRACKING::PROJECTION_FEMC)
  {
    kalman->add_state_name("FEMC");
  }

  //-------------------------
  // FHCAL
  //-------------------------
  if (Enable::FHCAL && G4TRACKING::PROJECTION_FHCAL)
  {
    kalman->add_state_name("FHCAL");
  }
  //-------------------------
  // CEMC
  //-------------------------

  if (Enable::CEMC && G4TRACKING::PROJECTION_CEMC)
  {
    kalman->add_state_name("CEMC");
  }
  se->registerSubsystem(kalman);

  return;
}

//-----------------------------------------------------------------------------//

void Tracking_Eval(const std::string &outputfile)
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::TRACKING_VERBOSITY);
  //---------------
  // Fun4All server
  //---------------

  Fun4AllServer *se = Fun4AllServer::instance();

  //----------------
  // Fast Tracking evaluation
  //----------------

  PHG4TrackFastSimEval *fast_sim_eval = new PHG4TrackFastSimEval("FastTrackingEval");
  fast_sim_eval->set_trackmapname(TRACKING::TrackNodeName);
  fast_sim_eval->set_filename(outputfile);
  se->registerSubsystem(fast_sim_eval);
}
#endif
