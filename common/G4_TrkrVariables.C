#ifndef MACRO_G4TRKRVARIABLES_C
#define MACRO_G4TRKRVARIABLES_C

#include <GlobalVariables.C>

#include <g4intt/PHG4InttDefs.h>
#include <g4mvtx/PHG4MvtxDefs.h>

namespace Enable
{
  bool MVTX = false;
  bool MVTX_OVERLAPCHECK = false;

  bool MVTX_CELL = false;
  bool MVTX_CLUSTER = false;
  bool MVTX_QA = false;
  bool MVTX_SUPPORT = false;

  int MVTX_VERBOSITY = 0;

  bool INTT = false;
  bool INTT_ABSORBER = false;
  bool INTT_OVERLAPCHECK = false;
  bool INTT_CELL = false;
  bool INTT_CLUSTER = false;
  bool INTT_QA = false;
  bool INTT_SUPPORT = false;
  bool INTT_USEG4SURVEYGEOM = true;
  int INTT_VERBOSITY = 0;

  bool TPC = false;
  bool TPC_ABSORBER = false;
  bool TPC_OVERLAPCHECK = false;
  bool TPC_CELL = false;
  bool TPC_CLUSTER = false;
  bool TPC_QA = false;

  bool TPC_ENDCAP = true;

  int TPC_VERBOSITY = 0;

  bool MICROMEGAS_OVERLAPCHECK = false;
  bool MICROMEGAS_CELL = false;
  bool MICROMEGAS_CLUSTER = false;
  bool MICROMEGAS_QA = false;
  bool MICROMEGAS_SUPPORT = false;
  int MICROMEGAS_VERBOSITY = 0;

  bool TRACKING_TRACK = false;
  bool TRACKING_EVAL = false;
  bool TRACK_MATCHING = false;
  bool TRACK_MATCHING_TREE = false;
  bool TRACK_MATCHING_TREE_CLUSTERS = false;
  // 0=no output TTree file, 1=output for tracks only, 2=tracks+clusters
  int TRACKING_VERBOSITY = 0;
  bool TRACKING_QA = false;
  bool TRACKING_DIAGNOSTICS = false;
}  // namespace Enable

namespace G4MVTX
{
  int n_maps_layer = 3;        // must be 0-3, setting it to zero removes Mvtx completely, n < 3 gives the first n layers
  double radius_offset = 0.7;  // clearance around radius
}  // namespace G4MVTX

namespace G4MVTXAlignment
{
  std::string alignment_path = string(getenv("CALIBRATIONROOT")) + "/Tracking/MVTX/alignment";
  double z_offset[] = {0.0, 0.0, 200.0};
}  // namespace G4MVTXAlignment

namespace G4INTT
{
  int n_intt_layer = 4;           // must be 4 or 0, setting to zero removes INTT completely
  double intt_radius_max = 140.;  // including stagger radius (mm)
  int laddertype[4] = {PHG4InttDefs::SEGMENTATION_PHI,
                       PHG4InttDefs::SEGMENTATION_PHI,
                       PHG4InttDefs::SEGMENTATION_PHI,
                       PHG4InttDefs::SEGMENTATION_PHI};
  int nladder[4] = {12, 12, 16, 16};
  //! default to survey geometry
  double sensor_radius[4] = {7.453, 8.046, 9.934, 10.569};

  enum enu_InttDeadMapType  // Dead map options for INTT
  {
    kInttNoDeadMap = 0,  // All channel in Intt is alive
    kInttDeadMap = 1,    // with dead channel
  };
  // enu_InttDeadMapType InttDeadMapOption = kInttNoDeadMap;  // Choose Intt deadmap here
  enu_InttDeadMapType InttDeadMapOption = kInttDeadMap;  // Choose Intt deadmap here

}  // namespace G4INTT

namespace G4TPC
{
  int n_tpc_layer_inner = 16;

  int tpc_layer_rphi_count_inner = 1128; // 94 * 12

  int n_tpc_layer_mid = 16;
  int n_tpc_layer_outer = 16;
  int n_gas_layer = n_tpc_layer_inner + n_tpc_layer_mid + n_tpc_layer_outer;
  double tpc_outer_radius = 77. + 2.;

  // use simple clusterizer
  bool USE_SIMPLE_CLUSTERIZER = false;

  // distortions
  bool DISTORTIONS_USE_PHI_AS_RADIANS = true;

  // static distortions
  bool ENABLE_STATIC_DISTORTIONS = false;
  std::string static_distortion_filename = "TPC_STATIC_DISTORTION";

  // time-ordered distortion fluctuations
  bool ENABLE_TIME_ORDERED_DISTORTIONS = false;
  std::string time_ordered_distortion_filename = "TPC_TIMEORDERED_DISTORTION";

  bool ENABLE_REACHES_READOUT = true;

  // module edge distortion corrections
  bool ENABLE_MODULE_EDGE_CORRECTIONS = false;
  std::string module_edge_correction_filename = "";

  // static distortion corrections
  bool ENABLE_STATIC_CORRECTIONS = false;
  std::string static_correction_filename = std::string(getenv("CALIBRATIONROOT")) + "/distortion_maps/static_only_inverted_10-new.root";

  // average distortion corrections
  bool ENABLE_AVERAGE_CORRECTIONS = false;
  std::string average_correction_filename;

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

  std::string TPC_GAS_MIXTURE = "NeCF4";
  // drift velocity is set here for all relevant modules
  //  double tpc_drift_velocity_reco now set in GlobalVariables.C
  double tpc_drift_velocity_sim = 0.007550;  // cm/ns   // this is the ArCF4Isobutane version of the gas
  double tpc_added_smear_long = 0.0;
  double tpc_added_smear_trans = 0.0;

  double NeCF4_drift_velocity = 0.008;
  double NeCF4_diffusion_long = 0.012;
  double NeCF4_diffusion_trans = 0.004;
  double NeCF4_Ne_frac = 0.5;
  double NeCF4_Ar_frac = 0.0;
  double NeCF4_CF4_frac = 0.5;
  double NeCF4_N2_frac = 0.0;
  double NeCF4_isobutane_frac = 0.0;

  double ArCF4_drift_velocity = 0.008;
  double ArCF4_diffusion_long = 0.012;
  double ArCF4_diffusion_trans = 0.004;
  double ArCF4_Ne_frac = 0.0;
  double ArCF4_Ar_frac = 0.6;
  double ArCF4_CF4_frac = 0.4;
  double ArCF4_N2_frac = 0.0;
  double ArCF4_isobutane_frac = 0.0;

  double ArCF4N2_drift_velocity = 0.006965;
  double ArCF4N2_diffusion_long = 0.013613;
  double ArCF4N2_diffusion_trans = 0.005487;
  double ArCF4N2_Ne_frac = 0.0;
  double ArCF4N2_Ar_frac = 0.65;
  double ArCF4N2_CF4_frac = 0.25;
  double ArCF4N2_N2_frac = 0.10;
  double ArCF4N2_isobutane_frac = 0.00;

  double ArCF4Isobutane_drift_velocity = 0.00755;
  double ArCF4Isobutane_diffusion_long = 0.014596;
  double ArCF4Isobutane_diffusion_trans = 0.005313;
  double ArCF4Isobutane_Ne_frac = 0.0;
  double ArCF4Isobutane_Ar_frac = 0.75;
  double ArCF4Isobutane_CF4_frac = 0.20;
  double ArCF4Isobutane_N2_frac = 0.00;
  double ArCF4Isobutane_isobutane_frac = 0.05;

}  // namespace G4TPC

namespace G4TRACKING
{
  // Space Charge calibration flag
  bool SC_CALIBMODE = false;  // this is anded with G4TPC::ENABLE_DISTORTIONS in TrackingInit()
  bool SC_USE_MICROMEGAS = true;
  double SC_COLLISIONRATE = 50e3;                                      // leave at 50 KHz for now, scaling of distortion map not implemented yet
  std::string SC_ROOTOUTPUT_FILENAME = "TpcSpaceChargeMatrices.root";  // space charge calibration output file

  // Vertexing
  bool g4eval_use_initial_vertex = true;  // if true, g4eval uses initial vertices in SvtxVertexMap, not final vertices in SvtxVertexMapRefit

  // Truth seeding options for diagnostics (can use any or all)
  bool use_truth_silicon_seeding = false;     // if true runs truth silicon seeding instead of acts silicon seeding
  bool use_truth_tpc_seeding = false;         // if true runs truth silicon seeding instead of reco TPC seeding
  bool use_truth_si_matching = false;         // if true, associates silicon clusters using best truth track match to TPC seed tracks - for diagnostics only
                                              // Full truth track seeding
  bool use_full_truth_track_seeding = false;  // makes track seeds using truth info, used for both Acts and Genfit
  bool use_truth_vertexing = false;           // if true runs truth vertexing, if false runs PHSimpleVertexFinder

  // genfit track fitter
  bool use_genfit_track_fitter = false;

  // Runs a converter from TrackSeed object to SvtxTrack object to enable
  // use of the various evaluation tools already available
  bool convert_seeds_to_svtxtracks = false;

  // Runs a second pass of seeding to pick up missed seeds in the first pass
  bool iterative_seeding = false;

  // Flag to run commissioning seeding workflow with tuned parameters for
  // misaligned + distorted tracks
  bool use_alignment = false;
  bool filter_conversion_electrons = false;

}  // namespace G4TRACKING

#endif
