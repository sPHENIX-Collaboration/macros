#ifndef MACRO_TRACKINGVARIABLES_C
#define MACRO_TRACKINGVARIABLES_C

#include <g4mvtx/PHG4MvtxDefs.h>
#include <g4intt/PHG4InttDefs.h>
//#include <g4tpc/PHG4TpcDefs.h>
//#include <g4micromegas/PHG4MicromegasDefs.h>

namespace ACTSGEOM
{

  unsigned int mvtxMisalignment = 1;
  unsigned int inttMisalignment = 1;
  unsigned int tpcMisalignment = 1;
  unsigned int tpotMisalignment = 1;

}  // namespace ACTSGEOM

namespace Enable
{
  bool TRACKING_TRACK = false;
  bool TRACKING_EVAL = false;
  int TRACKING_VERBOSITY = 0;
  bool TRACKING_QA = false;
}  // namespace Enable


namespace G4TRACKING
{
  // Space Charge calibration flag
  bool SC_CALIBMODE = false;                                            // this is anded with G4TPC::ENABLE_DISTORTIONS in TrackingInit()
  bool SC_USE_MICROMEGAS = true;
  bool SC_SAVEHISTOGRAMS = false;
  double SC_COLLISIONRATE = 50e3;                                      // leave at 50 KHz for now, scaling of distortion map not implemented yet
  std::string SC_ROOTOUTPUT_FILENAME = "TpcSpaceChargeMatrices.root";  // space charge calibration output file
  std::string SC_HISTOGRAMOUTPUT_FILENAME = "TpcResiduals.root"; // space charge calibration output file

  // Vertexing
  bool g4eval_use_initial_vertex = true;  // if true, g4eval uses initial vertices in SvtxVertexMap, not final vertices in SvtxVertexMapRefit
  
  // Truth seeding options for diagnostics (can use any or all)
  bool use_truth_silicon_seeding = false;     // if true runs truth silicon seeding instead of acts silicon seeding
  bool use_truth_tpc_seeding = false;         // if true runs truth silicon seeding instead of reco TPC seeding
  bool use_truth_si_matching = false;         // if true, associates silicon clusters using best truth track match to TPC seed tracks - for diagnostics only
                                              // Full truth track seeding
  bool use_full_truth_track_seeding = false;  // makes track seeds using truth info, used for both Acts and Genfit
  bool use_truth_vertexing = false;           // if true runs truth vertexing, if false runs PHSimpleVertexFinder

  // Runs a converter from TrackSeed object to SvtxTrack object to enable
  // use of the various evaluation tools already available
  bool convert_seeds_to_svtxtracks = false;

  // Flag to run commissioning seeding workflow with tuned parameters for
  // misaligned + distorted tracks
  bool use_alignment = false;
  std::string magfield = string(getenv("CALIBRATIONROOT")) + string("/Field/Map/sphenix3dtrackingmapxyz_gap_rebuild.root");
  double magfield_rescale = 1.;

}  // namespace G4TRACKING

namespace Enable
{
  bool MVTX = false;
  bool MVTX_OVERLAPCHECK = false;
  bool MVTX_CELL = false;
  bool MVTX_CLUSTER = false;
  bool MVTX_QA = false;
  bool MVTX_ABSORBER = false;
  int MVTX_VERBOSITY = 0;

}  // namespace Enable

namespace G4MVTX
{
  int n_maps_layer = 3;        // must be 0-3, setting it to zero removes Mvtx completely, n < 3 gives the first n layers
  double radius_offset = 0.7;  // clearance around radius
}  // namespace G4MVTX

namespace Enable
{
  bool TPC = false;
  bool TPC_ABSORBER = false;
  bool TPC_OVERLAPCHECK = false;
  bool TPC_CELL = false;
  bool TPC_CLUSTER = false;
  bool TPC_QA = false;

  bool TPC_ENDCAP = true;

  int TPC_VERBOSITY = 0;
}  // namespace Enable


namespace G4TPC
{
  int n_tpc_layer_inner = 16;
  int tpc_layer_rphi_count_inner = 1152;
  int n_tpc_layer_mid = 16;
  int n_tpc_layer_outer = 16;
  int n_gas_layer = n_tpc_layer_inner + n_tpc_layer_mid + n_tpc_layer_outer;
  double tpc_outer_radius = 77. + 2.;

  // drift velocity is set here for all relevant modules
  double tpc_drift_velocity_sim= 8.0 / 1000.0;  // cm/ns   // this is the Ne version of the gas
//  double tpc_drift_velocity_reco now set in GlobalVariables.C
//  double tpc_drift_velocity_reco= 8.0 / 1000.0;  // cm/ns   // this is the Ne version of the gas

  // use simple clusterizer
  bool USE_SIMPLE_CLUSTERIZER = false;

  // distortions
  bool ENABLE_STATIC_DISTORTIONS = false;
  auto static_distortion_filename = std::string(getenv("CALIBRATIONROOT")) + "/distortion_maps/static_only.distortion_map.hist.root";

  bool ENABLE_TIME_ORDERED_DISTORTIONS = false;
  std::string time_ordered_distortion_filename = std::string(getenv("CALIBRATIONROOT")) + "/distortion_maps/TimeOrderedDistortions.root";

  // distortion corrections
  bool ENABLE_CORRECTIONS = false;
  auto correction_filename = std::string(getenv("CALIBRATIONROOT")) + "/distortion_maps/static_only_inverted_10-new.root";

  // enable central membrane g4hits generation
  bool ENABLE_CENTRAL_MEMBRANE_HITS = false;
  
  // enable direct laser g4hits generation
  bool ENABLE_DIRECT_LASER_HITS = false;

  // save histograms
  bool DIRECT_LASER_SAVEHISTOGRAMS = false;

  // do cluster <-> hit association
  bool DO_HIT_ASSOCIATION = true;
  
  // space charge calibration output file
  std::string DIRECT_LASER_ROOTOUTPUT_FILENAME = "TpcSpaceChargeMatrices.root";
  std::string DIRECT_LASER_HISTOGRAMOUTPUT_FILENAME = "TpcDirectLaserReconstruction.root"; 
  
}  // namespace G4TPC


namespace Enable
{
  bool INTT = false;
  bool INTT_ABSORBER = false;
  bool INTT_OVERLAPCHECK = false;
  bool INTT_CELL = false;
  bool INTT_CLUSTER = false;
  bool INTT_QA = false;
  bool INTT_SUPPORT = false;
  int INTT_VERBOSITY = 0;
}  // namespace Enable


namespace G4INTT
{
  int n_intt_layer = 4;           // must be 4 or 0, setting to zero removes INTT completely
  double intt_radius_max = 140.;  // including stagger radius (mm)
  int laddertype[4] = {PHG4InttDefs::SEGMENTATION_PHI,
                       PHG4InttDefs::SEGMENTATION_PHI,
                       PHG4InttDefs::SEGMENTATION_PHI,
                       PHG4InttDefs::SEGMENTATION_PHI};
  int nladder[4] = {12, 12, 16, 16};
  double sensor_radius[4] = {7.188 - 36e-4, 7.732 - 36e-4, 9.680 - 36e-4, 10.262 - 36e-4};

  double offsetphi[4] = {0.0, 0.5 * 360.0 / nladder[1], 0.0, 0.5 * 360.0 / nladder[3]};

  enum enu_InttDeadMapType  // Dead map options for INTT
  {
    kInttNoDeadMap = 0,  // All channel in Intt is alive
    kInttDeadMap = 1,    // with dead channel
  };
  //enu_InttDeadMapType InttDeadMapOption = kInttNoDeadMap;  // Choose Intt deadmap here
  enu_InttDeadMapType InttDeadMapOption = kInttDeadMap;  // Choose Intt deadmap here

}  // namespace G4INTT

void InttInit()
{
  BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, 20.);  // estimated from display, can be made smaller but good enough
  BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z, 410. / 2.);
  BlackHoleGeometry::min_z = std::min(BlackHoleGeometry::min_z, -410. / 2.);
  // the mvtx is not called if disabled but the default number of layers is set to 3, so we need to set it
  // to zero
  if (!Enable::MVTX)
  {
    G4MVTX::n_maps_layer = 0;
  }
}

namespace Enable
{
  bool MICROMEGAS_CELL = false;
  bool MICROMEGAS_CLUSTER = false;
  bool MICROMEGAS_QA = false;
}  // namespace Enable

#endif
