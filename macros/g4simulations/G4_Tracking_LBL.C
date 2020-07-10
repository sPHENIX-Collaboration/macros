#ifndef MACRO_G4TRACKINGLBL_C
#define MACRO_G4TRACKINGLBL_C

#include "GlobalVariables.C"

#include "G4_CEmc_EIC.C"
#include "G4_FEMC_EIC.C"
#include "G4_FHCAL.C"
#include "G4_AllSilicon.C"

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
    kalman->set_vertex_xy_resolution(0);// do not smear the vertex used in the built-in DCA calculation
    kalman->set_vertex_z_resolution(0); // do not smear the vertex used in the built-in DCA calculation
    kalman->enable_vertexing(true);     // enable vertex finding and fitting
  }
  else
  {
    // constraint to a primary vertex and use it as part of the fitting level arm
    kalman->set_use_vertex_in_fitting(true);
    kalman->set_vertex_xy_resolution(50e-4);
    kalman->set_vertex_z_resolution(50e-4);
  }

  kalman->set_use_vertex_in_fitting(false);
  kalman->set_sub_top_node_name("TRACKS");
  kalman->set_trackmap_out_name(TRACKING::TrackNodeName);

	char nodename[100];
  //-------------------------
  // Barrel
  //-------------------------
  if (Enable::ALLSILICON)
  {
    for (int i=10; i<16; i++){ // CENTRAL BARREL
      sprintf(nodename,"G4HIT_LBLVTX_CENTRAL_%d", i);
      kalman->add_phg4hits(
	nodename,				// const std::string& phg4hitsNames
	PHG4TrackFastSim::Cylinder,		// const DETECTOR_TYPE phg4dettype
	999.,					// radial-resolution [cm] (this number is not used in cylindrical geometry)
	5.8e-4,					// azimuthal (arc-length) resolution [cm]
	5.8e-4,					// longitudinal (z) resolution [cm]
	1,					// efficiency (fraction)
	0					// hit noise
	);
    }
    for (int i=20; i<25; i++){ // FORWARD DISKS
      sprintf(nodename,"G4HIT_LBLVTX_FORWARD_%d", i);
      kalman->add_phg4hits(
	nodename,                          	// const std::string& phg4hitsNames
	PHG4TrackFastSim::Vertical_Plane,  	// const DETECTOR_TYPE phg4dettype
	5.8e-4,                            	// radial-resolution [cm]
	5.8e-4,                            	// azimuthal (arc-length) resolution [cm]
	999.,                              	// longitudinal (z) resolution [cm] (this number is not used in vertical plane geometry)
	1,                                 	// efficiency (fraction)
	0                                  	// hit noise
	);
    }
    for (int i=30; i<35; i++){ // BACKWARD DISKS
      sprintf(nodename,"G4HIT_LBLVTX_BACKWARD_%d", i);
      kalman->add_phg4hits(
	nodename,                          	// const std::string& phg4hitsNames
	PHG4TrackFastSim::Vertical_Plane,  	// const DETECTOR_TYPE phg4dettype
	5.8e-4,                            	// radial-resolution [cm]
	5.8e-4,                            	// azimuthal (arc-length) resolution [cm]
	999.,                              	// longitudinal (z) resolution [cm] (this number is not used in vertical plane geometry)
	1,                                 	// efficiency (fraction)
	0                                  	// hit noise
	);
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
