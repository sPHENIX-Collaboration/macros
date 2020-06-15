#pragma once

#include "GlobalVariables.C"

#include "G4_CEmc_EIC.C"
#include "G4_FEMC_EIC.C"
#include "G4_FHCAL.C"
#include "G4_GEM_EIC.C"
#include "G4_Mvtx_EIC.C"
#include "G4_TPC_EIC.C"

#include <g4trackfastsim/PHG4TrackFastSim.h>

#include <trackreco/PHRaveVertexing.h>

#include <g4trackfastsim/PHG4TrackFastSimEval.h>

#include <fun4all/Fun4AllServer.h>

#include <vector>

class SubsysReco;
R__LOAD_LIBRARY(libtrack_reco.so)
R__LOAD_LIBRARY(libg4trackfastsim.so)

namespace Enable
{
  bool TRACKING = false;
}

void TrackingInit() {}

void Tracking_Reco(int verbosity = 0, bool displaced_vertex = false)
{
  //---------------
  // Fun4All server
  //---------------

  Fun4AllServer *se = Fun4AllServer::instance();

  PHG4TrackFastSim *kalman = new PHG4TrackFastSim("PHG4TrackFastSim");
  //  kalman->Verbosity();
  //  kalman->Smearing(false);
  if (displaced_vertex)
  {
    //use very loose vertex constraint (1cm in sigma) to allow reco of displaced vertex
    kalman->set_use_vertex_in_fitting(true);
    kalman->set_vertex_xy_resolution(1);
    kalman->set_vertex_z_resolution(1);
    kalman->enable_vertexing(true);
  }
  else
  {
    // constraint to a primary vertex and use it as part of the fitting level arm
    kalman->set_use_vertex_in_fitting(true);
    kalman->set_vertex_xy_resolution(50e-4);
    kalman->set_vertex_z_resolution(50e-4);
  }

  kalman->set_sub_top_node_name("SVTX");
  kalman->set_trackmap_out_name("SvtxTrackMap");

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
  //
  // TPC
  if (Enable::TPC)
  {
    kalman->add_phg4hits(
        "G4HIT_SVTX",                //      const std::string& phg4hitsNames,
        PHG4TrackFastSim::Cylinder,  //      const DETECTOR_TYPE phg4dettype,
        1,                           //      const float radres,
        200e-4,                      //      const float phires,
        500e-4,                      //      const float lonres,
        1,                           //      const float eff,
        0                            //      const float noise
    );
  }
  if (Enable::EGEM)
  {
    // GEM0, 70um azimuthal resolution, 1cm radial strips
    kalman->add_phg4hits(
        "G4HIT_EGEM_0",                    //      const std::string& phg4hitsNames,
        PHG4TrackFastSim::Vertical_Plane,  //      const DETECTOR_TYPE phg4dettype,
        1. / sqrt(12.),                    //      const float radres,
        70e-4,                             //      const float phires,
        100e-4,                            //      const float lonres,
        1,                                 //      const float eff,
        0                                  //      const float noise
    );
    // GEM1, 70um azimuthal resolution, 1cm radial strips
    kalman->add_phg4hits(
        "G4HIT_EGEM_1",                    //      const std::string& phg4hitsNames,
        PHG4TrackFastSim::Vertical_Plane,  //      const DETECTOR_TYPE phg4dettype,
        1. / sqrt(12.),                    //      const float radres,
        70e-4,                             //      const float phires,
        100e-4,                            //      const float lonres,
        1,                                 //      const float eff,
        0                                  //      const float noise
    );
    // GEM2, 70um azimuthal resolution, 1cm radial strips
    kalman->add_phg4hits(
        "G4HIT_EGEM_2",                    //      const std::string& phg4hitsNames,
        PHG4TrackFastSim::Vertical_Plane,  //      const DETECTOR_TYPE phg4dettype,
        1. / sqrt(12.),                    //      const float radres,
        70e-4,                             //      const float phires,
        100e-4,                            //      const float lonres,
        1,                                 //      const float eff,
        0                                  //      const float noise
    );
    // GEM3, 70um azimuthal resolution, 1cm radial strips
    kalman->add_phg4hits(
        "G4HIT_EGEM_3",                    //      const std::string& phg4hitsNames,
        PHG4TrackFastSim::Vertical_Plane,  //      const DETECTOR_TYPE phg4dettype,
        1. / sqrt(12.),                    //      const float radres,
        70e-4,                             //      const float phires,
        100e-4,                            //      const float lonres,
        1,                                 //      const float eff,
        0                                  //      const float noise
    );
  }

  if (Enable::FGEM)
  {
    bool do_FST = true;
    bool do_FGEM = true;

    if (do_FST)
    {
      // LANL FST:   We could put the hit resolution at 5 micron with the 30 micron pixel pitch.
      for (int i = 0; i < 6; i++)
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

    if (do_FGEM)
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
  }

  // Saved track states (projections)
  if (Enable::FEMC)
  {
    //    kalman->add_state_name("FEMC");
  }
  if (Enable::FHCAL)
  {
    //    kalman->add_state_name("FHCAL");
  }

  if (Enable::CEMC)
  {
    //    kalman->add_state_name("CEMC");
  }
  se->registerSubsystem(kalman);

  return;
}

void Tracking_Eval(std::string outputfile, int verbosity = 0)
{
  //---------------
  // Fun4All server
  //---------------

  Fun4AllServer *se = Fun4AllServer::instance();

  //----------------
  // Fast Tracking evaluation
  //----------------

  PHG4TrackFastSimEval *fast_sim_eval = new PHG4TrackFastSimEval("FastTrackingEval");
  fast_sim_eval->set_filename(outputfile.c_str());
  se->registerSubsystem(fast_sim_eval);
}
