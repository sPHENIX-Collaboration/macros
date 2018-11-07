#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include "GlobalVariables.C"
#include <fun4all/Fun4AllServer.h>
#include <g4detectors/PHG4MapsCellReco.h>
#include <g4detectors/PHG4MapsSubsystem.h>
#include <g4detectors/PHG4SiliconTrackerCellReco.h>
#include <g4detectors/PHG4SiliconTrackerDefs.h>
#include <g4detectors/PHG4SiliconTrackerSubsystem.h>
#include <g4detectors/PHG4TPCSpaceChargeDistortion.h>
#include <g4eval/SvtxEvaluator.h>
#include <g4hough/PHG4GenFitTrackProjection.h>
#include <g4hough/PHG4KalmanPatRec.h>
#include <g4hough/PHG4SiliconTrackerDigitizer.h>
#include <g4hough/PHG4SvtxClusterizer.h>
#include <g4hough/PHG4SvtxDeadArea.h>
#include <g4hough/PHG4SvtxDigitizer.h>
#include <g4hough/PHG4SvtxThresholds.h>
#include <g4hough/PHG4TPCClusterizer.h>
#include <g4hough/PHG4TrackKalmanFitter.h>
#include <g4hough/PHG4TruthPatRec.h>
#include <g4main/PHG4Reco.h>
#include <g4tpc/PHG4TPCElectronDrift.h>
#include <g4tpc/PHG4TPCPadPlane.h>
#include <g4tpc/PHG4TPCPadPlaneReadout.h>
#include <g4tpc/PHG4TPCSubsystem.h>
R__LOAD_LIBRARY(libg4tpc.so)
R__LOAD_LIBRARY(libg4hough.so)
R__LOAD_LIBRARY(libg4eval.so)
#endif

#include <vector>
// define INTTLADDER8, INTTLADDER6, INTTLADDER4_ZP or INTTLADDER4_PP, INTTLADDER0 to get 8, 6, 4 or 0 layers
// one and only one of these has to be defined, because #elseif does not seem to work properly in the interpreter
#define INTTLADDER4_PP

// Dead map options for INTT
enum enu_INTTDeadMapType
{
  kINTTNoDeadMap = 0,       // All channel in INTT is alive
  kINTT4PercentDeadMap = 4, // 4% of dead/masked area (2% sensor + 2% chip) as a typical FVTX Run14 production run.
  kINTT8PercentDeadMap = 8  // 8% dead/masked area (6% sensor + 2% chip) as threshold of operational
};

// Choose INTT deadmap here
enu_INTTDeadMapType INTTDeadMapOption = kINTTNoDeadMap;

// ONLY if backward compatibility with hits files already generated with 8 inner TPC layers is needed, you can set this to "true"
bool tpc_layers_40  = false;

// if true, refit tracks with primary vertex included in track fit  - good for analysis of prompt tracks only
// Adds second node to node tree, keeps original track node undisturbed
// Adds second evaluator to process refitted tracks and outputs separate ntuples
bool use_primary_vertex = false;

const int n_maps_layer = 3;  // must be 0-3, setting it to zero removes MVTX completely, n < 3 gives the first n layers

// Configure the INTT layers
// offsetphi is in deg, every other layer is offset by one half of the phi spacing between ladders
#ifdef INTTLADDER8
int n_intt_layer = 8;  
// default layer configuration
int laddertype[8] = {PHG4SiliconTrackerDefs::SEGMENTATION_Z, 
		     PHG4SiliconTrackerDefs::SEGMENTATION_Z, 
		     PHG4SiliconTrackerDefs::SEGMENTATION_PHI,
		     PHG4SiliconTrackerDefs::SEGMENTATION_PHI,
		     PHG4SiliconTrackerDefs::SEGMENTATION_PHI,
		     PHG4SiliconTrackerDefs::SEGMENTATION_PHI,
		     PHG4SiliconTrackerDefs::SEGMENTATION_PHI,
		     PHG4SiliconTrackerDefs::SEGMENTATION_PHI};  // default
int nladder[8] = {17,  17, 15, 15, 18, 18, 21, 21};  // default
double sensor_radius[8] = {6.876, 7.462, 8.987, 9.545, 10.835, 11.361, 12.676, 13.179};  // radius of center of sensor for layer default
double offsetphi[8] = {0.0, 0.5 * 360.0 / nladder[1] , 0.0, 0.5 * 360.0 / nladder[3], 0.0, 0.5 * 360.0 / nladder[5], 0.0, 0.5 * 360.0 / nladder[7]};
#endif
#ifdef INTTLADDER6
int n_intt_layer = 6;
int laddertype[6] = {PHG4SiliconTrackerDefs::SEGMENTATION_Z, 
		       PHG4SiliconTrackerDefs::SEGMENTATION_Z, 
		       PHG4SiliconTrackerDefs::SEGMENTATION_PHI,
		       PHG4SiliconTrackerDefs::SEGMENTATION_PHI,
		       PHG4SiliconTrackerDefs::SEGMENTATION_PHI,
		       PHG4SiliconTrackerDefs::SEGMENTATION_PHI };
int nladder[6] = {17,  17, 15, 15, 18, 18}; 
double sensor_radius[6] = {6.876, 7.462, 8.987, 9.545, 10.835, 11.361};  // radius of center of sensor for layer default
double offsetphi[6] = {0.0, 0.5 * 360.0 / nladder[1] , 0.0, 0.5 * 360.0 / nladder[3], 0.0, 0.5 * 360.0 / nladder[5]};
#endif
#ifdef INTTLADDER4_ZP
int n_intt_layer = 4;
int laddertype[4] = {PHG4SiliconTrackerDefs::SEGMENTATION_Z, 
		       PHG4SiliconTrackerDefs::SEGMENTATION_Z, 
		       PHG4SiliconTrackerDefs::SEGMENTATION_PHI,
		       PHG4SiliconTrackerDefs::SEGMENTATION_PHI};
int nladder[4] = {17,  17, 18, 18}; 
double sensor_radius[6] = {6.876, 7.462, 10.835, 11.361};  // radius of center of sensor for layer default
double offsetphi[6] = {0.0, 0.5 * 360.0 / nladder[1] , 0.0, 0.5 * 360.0 / nladder[3]};
#endif
#ifdef INTTLADDER4_PP
int n_intt_layer = 4;
int laddertype[4] = {PHG4SiliconTrackerDefs::SEGMENTATION_PHI, 
		       PHG4SiliconTrackerDefs::SEGMENTATION_PHI, 
		       PHG4SiliconTrackerDefs::SEGMENTATION_PHI,
		       PHG4SiliconTrackerDefs::SEGMENTATION_PHI};
int nladder[4] = {15,  15, 18, 18}; 
double sensor_radius[6] = { 8.987, 9.545, 10.835, 11.361};  // radius of center of sensor for layer default
double offsetphi[6] = {0.0, 0.5 * 360.0 / nladder[1] , 0.0, 0.5 * 360.0 / nladder[3]};
#endif
#ifdef INTTLADDER0
int n_intt_layer = 0;
#endif

int n_tpc_layer_inner = 16;
int tpc_layer_rphi_count_inner = 1152;
int n_tpc_layer_mid = 16;
int n_tpc_layer_outer = 16;
int n_gas_layer = n_tpc_layer_inner + n_tpc_layer_mid + n_tpc_layer_outer;

int Max_si_layer;

void TrackingInit(int verbosity = 0)
{
  Max_si_layer = n_maps_layer + n_intt_layer + n_gas_layer;
}

double Tracking(PHG4Reco* g4Reco, double radius,
            const int absorberactive = 0,
            int verbosity = 0)
{
  // create the three tracker subsystems 

  if (n_maps_layer > 0)
    {
      bool maps_overlapcheck = false;  // set to true if you want to check for overlaps
      
      // MAPS inner barrel layers
      //======================================================
      
      double maps_layer_radius[3] = {24.61, 32.59, 39.88}; // mm - numbers from Walt 6 Aug 2018
      
      // D. McGlinchey 6Aug2018 - type no longer is used, included here because I was too lazy to remove it from the code
      int stave_type[3] = {0, 0, 0};
      int staves_in_layer[3] = {12, 16, 20};       // Number of staves per layer in sPHENIX MVTX
      double phi_tilt[3] = {0.300, 0.305, 0.300}; // radians - numbers from Walt 6 Aug 2018
      
      for (int ilayer = 0; ilayer < n_maps_layer; ilayer++)
	{
if (verbosity)
	    cout << "Create Maps layer " << ilayer << " with radius " << maps_layer_radius[ilayer] << " mm, stave type " << stave_type[ilayer]
		 << " pixel size 30 x 30 microns "
		 << " active pixel thickness 0.0018 microns" << endl;
	  
	  PHG4MapsSubsystem* lyr = new PHG4MapsSubsystem("MAPS", ilayer, stave_type[ilayer]);
	  lyr->Verbosity(verbosity);
	  
	  lyr->set_double_param("layer_nominal_radius", maps_layer_radius[ilayer]);  // thickness in cm
	  lyr->set_int_param("N_staves", staves_in_layer[ilayer]);       // uses fixed number of staves regardless of radius, if set. Otherwise, calculates optimum number of staves
	  
	  // The cell size is used only during pixilization of sensor hits, but it is convemient to set it now because the geometry object needs it
	  lyr->set_double_param("pixel_x", 0.0030);          // pitch in cm
	  lyr->set_double_param("pixel_z", 0.0030);          // length in cm
	  lyr->set_double_param("pixel_thickness", 0.0018);  // thickness in cm
	  lyr->set_double_param("phitilt", phi_tilt[ilayer]);
	  
	  lyr->set_int_param("active", 1);
	  lyr->OverlapCheck(maps_overlapcheck);
	  
	  lyr->set_string_param("stave_geometry_file", string(getenv("CALIBRATIONROOT")) + string("/Tracking/geometry/mvtx_stave_v01.gdml"));
	  
	  g4Reco->registerSubsystem(lyr);
	  
	  radius = maps_layer_radius[ilayer];
	}
    }
  
  if (n_intt_layer > 0)
    {
      //-------------------
      // INTT ladders
      //-------------------
      
      bool intt_overlapcheck = false;  // set to true if you want to check for overlaps
      
      // instantiate the Silicon tracker subsystem and register it
      // We make one instance of PHG4TrackerSubsystem for all four layers of tracker
      // dimensions are in mm, angles are in radians
      
      // PHG4SiliconTrackerSubsystem creates the detetor layer using PHG4SiliconTrackerDetector
      // and instantiates the appropriate PHG4SteppingAction
      const double intt_radius_max = 140.;  // including stagger radius (mm)
      
      // The length of vpair is used to determine the number of layers
      std::vector<std::pair<int, int>> vpair;  // (sphxlayer, inttlayer)
      for (int i = 0; i < n_intt_layer; i++)
	{
	  // We want the sPHENIX layer numbers for the INTT to be from n_maps_layer to n_maps_layer+n_intt_layer - 1
	  vpair.push_back(std::make_pair(n_maps_layer + i, i));  // sphxlayer=n_maps_layer+i corresponding to inttlayer=i
	  if (verbosity) cout << "Create strip tracker layer " << vpair[i].second << " as  sphenix layer  " << vpair[i].first << endl;
	}
      
      PHG4SiliconTrackerSubsystem* sitrack = new PHG4SiliconTrackerSubsystem("SILICON_TRACKER", vpair);
      sitrack->Verbosity(verbosity);
      sitrack->SetActive(1);
      sitrack->OverlapCheck(intt_overlapcheck);
      g4Reco->registerSubsystem(sitrack);
      
      // Set the laddertype and ladder spacing configuration
      cout << "INTT has " << n_intt_layer << " layers with layer setup:" << endl;
      for(int i=0;i<n_intt_layer;i++)
	{
	  cout << " INTT layer " << i << " laddertype " << laddertype[i] << " nladders " << nladder[i] 
	       << " sensor radius " << sensor_radius[i] << " offsetphi " << offsetphi[i] << endl; 
	  sitrack->set_int_param(i, "laddertype", laddertype[i]);
	  sitrack->set_int_param(i, "nladder", nladder[i]);
	  sitrack->set_double_param(i,"sensor_radius", sensor_radius[i]);  // expecting cm
	  sitrack->set_double_param(i,"offsetphi",offsetphi[i]);  // expecting degrees
	}
      
      // outer radius marker (translation back to cm)
      radius = intt_radius_max * 0.1;
    }

  // The TPC - always present!
  //================================

  gSystem->Load("libg4tpc.so");

  PHG4TPCSubsystem *tpc = new PHG4TPCSubsystem("TPC");
  tpc->SetActive();
  tpc->SuperDetector("TPC");
  tpc->set_double_param("steplimits", 1);
  // By default uses "sPHENIX_TPC_Gas", defined in PHG4Reco. That is 90:10 Ne:C4

 if (absorberactive)  
    {
      tpc->SetAbsorberActive();
    }
  tpc->OverlapCheck(overlapcheck);

  g4Reco->registerSubsystem( tpc );  

  radius = 77.+1.17;
  

  radius += no_overlapp;
  
  return radius; 
}

void Tracking_Cells(int verbosity = 0)
{
  // runs the cellularization of the energy deposits (g4hits)
  // into detector hits (g4cells)

  //---------------
  // Load libraries
  //---------------

  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4detectors.so");
  gSystem->Load("libg4tpc.so");

  //---------------
  // Fun4All server
  //---------------

  Fun4AllServer* se = Fun4AllServer::instance();

  // MVTX cells
  //=========

  if (n_maps_layer > 0)
  {
    // MAPS cells
    PHG4MapsCellReco* maps_cells = new PHG4MapsCellReco("MAPS");
    maps_cells->Verbosity(verbosity);
    for (int ilayer = 0; ilayer < n_maps_layer; ilayer++)
    {
      maps_cells->set_timing_window(ilayer, -2000, 2000);
    }
    se->registerSubsystem(maps_cells);
  }

  // INTT cells
  //=========
  if (n_intt_layer > 0)
  {
    // INTT cells
    PHG4SiliconTrackerCellReco* reco = new PHG4SiliconTrackerCellReco("SILICON_TRACKER");
    // The timing windows are hard-coded in the INTT ladder model, they can be overridden here
    //reco->set_double_param("tmax",80.0);
    //reco->set_double_param("tmin",-20.0);
    reco->Verbosity(verbosity);
    se->registerSubsystem(reco);
  }

  // Set up TPC distortion calculation
  //========================
  const bool do_tpc_distortion = true;
  PHG4TPCSpaceChargeDistortion* tpc_distortion = NULL;
  if (do_tpc_distortion)
  {
    string TPC_distortion_file =
        string(getenv("CALIBRATIONROOT")) +
        Form("/Tracking/TPC/SpaceChargeDistortion/TPCCAGE_20_78_211_2.root");
    PHG4TPCSpaceChargeDistortion* tpc_distortion =
        new PHG4TPCSpaceChargeDistortion(TPC_distortion_file);
    //tpc_distortion -> setAccuracy(0); // option to over write default  factors
    //tpc_distortion -> setPrecision(0.001); // option to over write default  factors      // default is 0.001
  }

  //=========================
  // setup TPC readout for filling cells
  // g4tpc/PHG4TPCElectronDrift uses
  // g4tpc/PHG4TPCPadPlaneReadout
  //=========================

  PHG4TPCElectronDrift *edrift = new PHG4TPCElectronDrift();
  edrift->Detector("TPC");
  // fudge factors to get drphi 150 microns (in mid and outer TPC) and dz 500 microns cluster resolution
  // They represent effects not due to ideal gas properties and ideal readout plane behavior 
  // defaults are 0.12 and 0.15, they can be changed here to get a different resolution
  edrift->set_double_param("added_smear_trans",0.12);
  edrift->set_double_param("added_smear_long",0.15);
  PHG4TPCPadPlane *padplane = new PHG4TPCPadPlaneReadout();
  edrift->registerPadPlane(padplane);
  se->registerSubsystem(edrift);

  // The pad plane readout default is set in PHG4TPCPadPlaneReadout
  // We may want to change the number of inner layers, and can do that here
  padplane->set_int_param("tpc_minlayer_inner",n_maps_layer+n_intt_layer);   // sPHENIX layer number of first TPC readout layer
  padplane->set_int_param("ntpc_layers_inner",n_tpc_layer_inner); 
   padplane->set_int_param("ntpc_phibins_inner",tpc_layer_rphi_count_inner); 

  return;
}

void Tracking_Reco(int verbosity = 0)
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

  //----------------------------------
  // Digitize the cell energy into ADC
  //----------------------------------
  PHG4SvtxDigitizer* digi = new PHG4SvtxDigitizer();
  digi->Verbosity(0);
  for (int i = 0; i < n_maps_layer; ++i)
  {
    digi->set_adc_scale(i, 255, 0.4e-6);  // reduced by a factor of 2.5 when going from maps thickess of 50 microns to 18 microns
  }

  if (n_intt_layer > 0)
  {

    if (INTTDeadMapOption != kINTTNoDeadMap)
    {
      // Load pre-defined deadmaps
      PHG4SvtxDeadMapLoader* deadMapINTT = new PHG4SvtxDeadMapLoader("SILICON_TRACKER");
      for (int i = 0; i < n_intt_layer; i++)
      {
        const int database_strip_type = (laddertype[i] == PHG4SiliconTrackerDefs::SEGMENTATION_Z) ? 0 : 1;
        string DeadMapConfigName = Form("LadderType%d_RndSeed%d/", database_strip_type, i);


        if (INTTDeadMapOption == kINTT4PercentDeadMap)
        {

          string DeadMapPath = string(getenv("CALIBRATIONROOT")) + string("/Tracking/INTT/DeadMap_4Percent/"); //4% of dead/masked area (2% sensor + 2% chip) as a typical FVTX Run14 production run.
          DeadMapPath +=  DeadMapConfigName;
          deadMapINTT->deadMapPath(n_maps_layer + i, DeadMapPath);

        }
        else if (INTTDeadMapOption == kINTT8PercentDeadMap)
        {

          string DeadMapPath = string(getenv("CALIBRATIONROOT")) + string("/Tracking/INTT/DeadMap_8Percent/"); // 8% dead/masked area (6% sensor + 2% chip) as threshold of operational
          DeadMapPath +=  DeadMapConfigName;
          deadMapINTT->deadMapPath(n_maps_layer + i, DeadMapPath);

        }
        else
        {
          cout <<"Tracking_Reco - fatal error - invalid INTTDeadMapOption = "<<INTTDeadMapOption<<endl;
          exit(1);
        }
      }
//      deadMapINTT -> Verbosity(1);
      se->registerSubsystem(deadMapINTT);
    }

    // INTT
    // these should be used for the INTT
    /*
How threshold are calculated based on default FPHX settings
Four part information goes to the threshold calculation:
1. In 320 um thick silicon, the MIP e-h pair for a nominally indenting tracking is 3.87 MeV/cm * 320 um / 3.62 eV/e-h = 3.4e4 e-h pairs
2. From DOI: 10.1016/j.nima.2014.04.017, FPHX integrator amplifier gain is 100mV / fC. That translate MIP voltage to 550 mV.
3. From [FPHX Final Design Document](https://www.phenix.bnl.gov/WWW/fvtx/DetectorHardware/FPHX/FPHX2_June2009Revision.doc), the DAC0-7 setting for 8-ADC thresholds above the V_ref, as in Table 2 - Register Addresses and Defaults
4, From [FPHX Final Design Document](https://www.phenix.bnl.gov/WWW/fvtx/DetectorHardware/FPHX/FPHX2_June2009Revision.doc) section Front-end Program Bits, the formula to translate DAC setting to comparitor voltages.
The result threshold table based on FPHX default value is as following
| FPHX Register Address | Name            | Default value | Voltage - Vref (mV) | To electrons based on calibration | Electrons | Fraction to MIP |
|-----------------------|-----------------|---------------|---------------------|-----------------------------------|-----------|-----------------|
| 4                     | Threshold DAC 0 | 8             | 32                  | 2500                              | 2000      | 5.85E-02        |
| 5                     | Threshold DAC 1 | 16            | 64                  | 5000                              | 4000      | 1.17E-01        |
| 6                     | Threshold DAC 2 | 32            | 128                 | 10000                             | 8000      | 2.34E-01        |
| 7                     | Threshold DAC 3 | 48            | 192                 | 15000                             | 12000     | 3.51E-01        |
| 8                     | Threshold DAC 4 | 80            | 320                 | 25000                             | 20000     | 5.85E-01        |
| 9                     | Threshold DAC 5 | 112           | 448                 | 35000                             | 28000     | 8.18E-01        |
| 10                    | Threshold DAC 6 | 144           | 576                 | 45000                             | 36000     | 1.05E+00        |
| 11                    | Threshold DAC 7 | 176           | 704                 | 55000                             | 44000     | 1.29E+00        |
DAC0-7 threshold as fraction to MIP voltage are set to PHG4SiliconTrackerDigitizer::set_adc_scale as 3-bit ADC threshold as fractions to MIP energy deposition.
     */
    std::vector<double> userrange;  // 3-bit ADC threshold relative to the mip_e at each layer.
    userrange.push_back(0.0584625322997416);
    userrange.push_back(0.116925064599483);
    userrange.push_back(0.233850129198966);
    userrange.push_back(0.35077519379845);
    userrange.push_back(0.584625322997416);
    userrange.push_back(0.818475452196383);
    userrange.push_back(1.05232558139535);
    userrange.push_back(1.28617571059432);

    PHG4SiliconTrackerDigitizer* digiintt = new PHG4SiliconTrackerDigitizer();
    digiintt->Verbosity(verbosity);
    for (int i = 0; i < n_intt_layer; i++)
    {
      digiintt->set_adc_scale(n_maps_layer + i, userrange);
    }
    se->registerSubsystem(digiintt);
  }

 // TPC layers use the Svtx digitizer
  digi->SetTPCMinLayer(n_maps_layer + n_intt_layer);
  double ENC = 670.0;  // standard
  digi->SetENC(ENC);  
  double ADC_threshold = 4.0*ENC; 
  digi->SetADCThreshold(ADC_threshold);  // 4 * ENC seems OK
    cout << " TPC digitizer: Setting ENC to " << ENC << " ADC threshold to " << ADC_threshold 
       << " maps+INTT layers set to " << n_maps_layer + n_intt_layer << endl;
 
  se->registerSubsystem(digi);

  //-------------------------------------
  // Apply Live Area Inefficiency to Hits
  // This is obsolete, please use PHG4SvtxDeadMapLoader instead for pre-defined deadmap
  //-------------------------------------
  // defaults to 1.0 (fully active)

//  PHG4SvtxDeadArea* deadarea = new PHG4SvtxDeadArea();
//
//  for (int i = 0; i < n_maps_layer; i++)
//  {
//    deadarea->Verbosity(verbosity);
//    //deadarea->set_hit_efficiency(i,0.99);
//    deadarea->set_hit_efficiency(i, 1.0);
//  }
//  for (int i = n_maps_layer; i < n_maps_layer + n_intt_layer; i++)
//  {
//    //deadarea->set_hit_efficiency(i,0.99);
//    deadarea->set_hit_efficiency(i, 1.0);
//  }
//  se->registerSubsystem(deadarea);

  //-----------------------------
  // Apply MIP thresholds to Hits
  //-----------------------------

  PHG4SvtxThresholds* thresholds = new PHG4SvtxThresholds();
  thresholds->Verbosity(verbosity);

  // maps
  for (int i = 0; i < n_maps_layer; i++)
  {
    // reduced by x2.5 when going from cylinder maps with 50 microns thickness to actual maps with 18 microns thickness
    // Note the non-use of set_using_thickness here, this is so that the shortest dimension of the cell sets the mip energy loss
    thresholds->set_threshold(i, -1);
  }
  // INTT: Does not need PHG4SvtxThresholds as the new digitizer handle the zero-suppression threshold with in ASIC

  se->registerSubsystem(thresholds);

  //-------------
  // Cluster Hits
  //-------------

  // For the silicon layers
  PHG4SvtxClusterizer* clusterizer = new PHG4SvtxClusterizer("PHG4SvtxClusterizer", 0, n_maps_layer + n_intt_layer - 1);
  clusterizer->Verbosity(verbosity);
  // Reduced by 2 relative to the cylinder cell maps macro. I found this necessary to get full efficiency
  // Many hits in the present simulation are single cell hits, so it is not clear why the cluster threshold should be higher than the cell threshold
  clusterizer->set_threshold(0.1);  // fraction of a mip

  // no Z clustering for INTT type 1 layers (we DO want Z clustering for type 0 layers)
  // turning off phi clustering for type 0 layers is not necessary, there is only one strip per sensor in phi
  for (int i = n_maps_layer; i < n_maps_layer + n_intt_layer; i++)
  {
    if(laddertype[i-n_maps_layer] == PHG4SiliconTrackerDefs::SEGMENTATION_PHI)
      clusterizer->set_z_clustering(i, false);
  }
  se->registerSubsystem(clusterizer);

  // For the TPC
  
  PHG4TPCClusterizer* tpcclusterizer = new PHG4TPCClusterizer();
  tpcclusterizer->Verbosity(0);
  tpcclusterizer->setRangeLayers(n_maps_layer + n_intt_layer, Max_si_layer);
  tpcclusterizer->setEnergyCut(15 /*adc*/);
  tpcclusterizer->setFitWindowSigmas(0.0150, 0.10);  // should be changed when TPC cluster resolution changes
  tpcclusterizer->setFitWindowMax(5 /*rphibins*/, 5 /*zbins*/);
  se->registerSubsystem(tpcclusterizer);

  // This should be true for everything except testing!
  const bool use_kalman_pat_rec = true;
  if (use_kalman_pat_rec)
  {
    //---------------------
    // PHG4KalmanPatRec
    //---------------------

    PHG4KalmanPatRec* kalman_pat_rec = new PHG4KalmanPatRec("PHG4KalmanPatRec", n_maps_layer, n_intt_layer, n_gas_layer);
    kalman_pat_rec->Verbosity(0);
    
    for(int i = 0;i<n_intt_layer;i++)
      {
	if(laddertype[i] == PHG4SiliconTrackerDefs::SEGMENTATION_Z)
	  {
	    // strip length is along phi
	    kalman_pat_rec->set_max_search_win_theta_intt(i, 0.010);
	    kalman_pat_rec->set_min_search_win_theta_intt(i, 0.00);
	    kalman_pat_rec->set_max_search_win_phi_intt(i, 0.20);
	    kalman_pat_rec->set_min_search_win_phi_intt(i, 0.20);
	  }
	else
	  {
	    // strip length is along theta
	    kalman_pat_rec->set_max_search_win_theta_intt(i, 0.200);
	    kalman_pat_rec->set_min_search_win_theta_intt(i, 0.200);
	    kalman_pat_rec->set_max_search_win_phi_intt(i, 0.0050);
	    kalman_pat_rec->set_min_search_win_phi_intt(i, 0.000);
	  }
      }
    
    se->registerSubsystem(kalman_pat_rec);
  }
  else
  {
    //---------------------
    // Truth Pattern Recognition
    //---------------------
    PHG4TruthPatRec* pat_rec = new PHG4TruthPatRec();
    se->registerSubsystem(pat_rec);
  }

  //---------------------
  // Kalman Filter
  //---------------------

  PHG4TrackKalmanFitter* kalman = new PHG4TrackKalmanFitter();
  kalman->Verbosity(0);
  if (use_primary_vertex)
    kalman->set_fit_primary_tracks(true);  // include primary vertex in track fit if true
  se->registerSubsystem(kalman);

  //------------------
  // Track Projections
  //------------------
  PHG4GenFitTrackProjection* projection = new PHG4GenFitTrackProjection();
  projection->Verbosity(verbosity);
  se->registerSubsystem(projection);

  /*  
  //----------------------
  // Beam Spot Calculation
  //----------------------
  PHG4SvtxBeamSpotReco* beamspot = new PHG4SvtxBeamSpotReco();
  beamspot->Verbosity(verbosity);
  se->registerSubsystem( beamspot );
  */

  return;
}

void Tracking_Eval(std::string outputfile, int verbosity = 0)
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
  // Tracking evaluation
  //----------------

  SvtxEvaluator* eval;
  eval = new SvtxEvaluator("SVTXEVALUATOR", outputfile.c_str(), "SvtxTrackMap", n_maps_layer, n_intt_layer, n_gas_layer);
  eval->do_cluster_eval(true);
  eval->do_g4hit_eval(true);
  eval->do_hit_eval(true);  // enable to see the hits that includes the chamber physics...
  eval->do_gpoint_eval(false);
  eval->scan_for_embedded(false);  // take all tracks if false - take only embedded tracks if true
  eval->Verbosity(0);
  se->registerSubsystem(eval);

  if (use_primary_vertex)
  {
    // make a second evaluator that records tracks fitted with primary vertex included
    // good for analysis of prompt tracks, particularly if MVTX is not present
    SvtxEvaluator* evalp;
    evalp = new SvtxEvaluator("SVTXEVALUATOR", string(outputfile.c_str()) + "_primary_eval.root", "PrimaryTrackMap", n_maps_layer, n_intt_layer, n_gas_layer);    evalp->do_cluster_eval(true);
    evalp->do_g4hit_eval(true);
    evalp->do_hit_eval(false);
    evalp->do_gpoint_eval(false);
    evalp->scan_for_embedded(true);  // take all tracks if false - take only embedded tracks if true
    evalp->Verbosity(0);
    se->registerSubsystem(evalp);
  }

  return;
}
