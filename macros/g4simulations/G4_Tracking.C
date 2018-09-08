#include <vector>

// ONLY if backward compatibility with hits files already generated with 8 inner TPC layers is needed, you can set this to "true"
bool tpc_layers_40  = false;

// if true, refit tracks with primary vertex included in track fit  - good for analysis of prompt tracks only
// Adds second node to node tree, keeps original track node undisturbed
// Adds second evaluator to process refitted tracks and outputs separate ntuples
bool use_primary_vertex = false;

const int n_maps_layer = 3;  // must be 0-3, setting it to zero removes MVTX completely, n < 3 gives the first n layers

// default setup for the INTT - please don't change this. The configuration can be redone later in the nacro if desired
int n_intt_layer = 4;  
int laddertype[4] = {PHG4SiliconTrackerDefs::SEGMENTATION_Z, 
		     PHG4SiliconTrackerDefs::SEGMENTATION_PHI,
		     PHG4SiliconTrackerDefs::SEGMENTATION_PHI,
		     PHG4SiliconTrackerDefs::SEGMENTATION_PHI};  // default
int nladder[4] = {34, 30, 36, 42};  // default  - note these have to be changed if laddertype changes for the layer
double sensor_radius_inner[4] = {6.876, 8.987, 10.835, 12.676};  // inner staggered radius for layer default
double sensor_radius_outer[4] = {7.462, 9.545, 11.361, 13.179};  // outer staggered radius for layer  default

// Optionally reconfigure the INTT
//========================================================================
// example re-configurations of INTT - uncomment one to get the reconfiguration
// n_intt must be 0-4, setting it to zero will remove the INTT completely,  otherwise it gives you n layers
//========================================================================
/*
// Four layers, laddertypes 1-1-0-1
n_intt_layer = 4;
laddertype[0] =  PHG4SiliconTrackerDefs::SEGMENTATION_PHI;    laddertype[1] =   PHG4SiliconTrackerDefs::SEGMENTATION_PHI;  
laddertype[2] =  PHG4SiliconTrackerDefs::SEGMENTATION_Z; laddertype[3] =  PHG4SiliconTrackerDefs::SEGMENTATION_PHI; 
nladder[0] = 22;       nladder[1] = 30;  nladder[2] = 52;  nladder[3] = 42;
sensor_radius_inner[0] = 6.876; sensor_radius_inner[1] = 8.987; sensor_radius_inner[2] = 10.835;    sensor_radius_inner[3] = 12.676; 
sensor_radius_outer[0] = 7.462; sensor_radius_outer[1] = 9.545; sensor_radius_outer[2] = 11.361;    sensor_radius_outer[3] = 13.179; 
*/
/*
// Three outer layers, laddertypes 1-0-1 
n_intt_layer = 3;
laddertype[0] =  PHG4SiliconTrackerDefs::SEGMENTATION_PHI;    laddertype[1] =  PHG4SiliconTrackerDefs::SEGMENTATION_Z;  
laddertype[2] =  PHG4SiliconTrackerDefs::SEGMENTATION_PHI;
nladder[0] = 30;  nladder[1] = 52;  nladder[2] = 42;
sensor_radius_inner[0] = 8.987; sensor_radius_inner[1] = 10.835;    sensor_radius_inner[2] = 12.676; 
sensor_radius_outer[0] = 9.545; sensor_radius_outer[1] = 11.361;    sensor_radius_outer[2] = 13.179; 
*/
/*
// Three outer layers, laddertypes 1-1-1 
n_intt_layer = 3;
laddertype[0] =  PHG4SiliconTrackerDefs::SEGMENTATION_PHI;    laddertype[1] =  PHG4SiliconTrackerDefs::SEGMENTATION_PHI;  
laddertype[2] =  PHG4SiliconTrackerDefs::SEGMENTATION_PHI;
nladder[0] = 30;  nladder[1] = 36;  nladder[2] = 42;
sensor_radius_inner[0] = 8.987; sensor_radius_inner[1] = 10.835;    sensor_radius_inner[2] = 12.676; 
sensor_radius_outer[0] = 9.545; sensor_radius_outer[1] = 11.361;    sensor_radius_outer[2] = 13.179; 
*/
/*
// Two outer layers, laddertype 0-1
n_intt_layer = 2;
laddertype[0] = PHG4SiliconTrackerDefs::SEGMENTATION_Z;    laddertype[1] = PHG4SiliconTrackerDefs::SEGMENTATION_PHI; 
nladder[0] = 52;       nladder[1] = 42;
sensor_radius_inner[0] = 10.835;    sensor_radius_inner[1] = 12.676; 
sensor_radius_outer[0] = 11.361;    sensor_radius_outer[1] = 13.179; 
*/
/*
// Two outer layers, laddertype 1-1
n_intt_layer = 2;
laddertype[0] =  PHG4SiliconTrackerDefs::SEGMENTATION_PHI;    laddertype[1] =  PHG4SiliconTrackerDefs::SEGMENTATION_PHI; 
nladder[0] = 36;       nladder[1] = 42;
sensor_radius_inner[0] = 10.835;    sensor_radius_inner[1] = 12.676; 
sensor_radius_outer[0] = 11.361;    sensor_radius_outer[1] = 13.179; 
*/

int n_tpc_layer_inner = 16;
int tpc_layer_rphi_count_inner = 1152;
int n_tpc_layer_mid = 16;
int n_tpc_layer_outer = 16;
int n_gas_layer = n_tpc_layer_inner + n_tpc_layer_mid + n_tpc_layer_outer;

// TPC Performance Parameter (applies extra smear to mimic the avalanche):
double TPC_SigmaT = 0.03;  // 0.03 means 300 microns...Prakhar Garg Simulation...desire measurement...

// to be overwritten...
double TPCDriftVelocity;
double TPC_Trans_Diffusion;
double TPC_Long_Diffusion;
double TPC_dEdx;
double TPC_NPri;
double TPC_NTot;
double TPC_ElectronsPerKeV;

// TPC readout shaping time and ADC clock parameters
// these set the Z size of the TPC cells
// These need to be set in the init since some of them require the drift velocity...
//=======================================
double TPCADCClock;
double TPCShapingRMSLead;
double TPCShapingRMSTail;
double tpc_cell_z;
double TPC_SmearRPhi;
double TPC_SmearZ;

int Max_si_layer;

void TrackingInit(int verbosity = 0)
{
  Max_si_layer = n_maps_layer + n_intt_layer + n_gas_layer;

  /*
  if (verbosity)
    cout << "Gas Choice:  TPC_Gas::NeCF4_400" << endl;
  TPCDriftVelocity = 8.0 / 1000.0;  // cm/ns
  TPC_Trans_Diffusion = 0.0060;     // cm/SQRT(cm)
  TPC_Long_Diffusion = 0.0150;      // cm/SQRT(cm)
  TPC_dEdx = 0.90 * Ne_dEdx + 0.10 * CF4_dEdx;
  TPC_NPri = 0.90 * Ne_NPrimary + 0.10 * CF4_NPrimary;
  TPC_NTot = 0.90 * Ne_NTotal + 0.10 * CF4_NTotal;

  TPC_ElectronsPerKeV = TPC_NTot / TPC_dEdx;

  // TPC readout shaping time and ADC clock parameters
  // these set the Z size of the TPC cells
  //=======================================
  // TPCShapingRMSLead = 32.0;  // ns, rising RMS equivalent of shaping amplifier for 80 ns SAMPA
  // TPCShapingRMSTail = 48.0;  // ns, falling RMS equivalent of shaping amplifier for 80 ns SAMPA
  TPCADCClock = 53.0;                           // ns, corresponds to an ADC clock rate of 18.8 MHz
  TPCShapingRMSLead = 16.0;                     // ns, rising RMS equivalent of shaping amplifier for 40 ns SAMPA
  TPCShapingRMSTail = 24.0;                     // ns, falling RMS equivalent of shaping amplifier for 40 ns SAMPA
  tpc_cell_z = TPCADCClock * TPCDriftVelocity;  // cm

  //  TKH does not understand the physical origin of these parameters.
  //  however, their impact seems quite small...
  //  these are tuned to give 150 microns r-phi and 500 microns Z resolution in the outer TPC layers with the TPC setup used here
  TPC_SmearRPhi = 0.215;
  TPC_SmearZ = 0.20;
  */
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
	  
	  //lyr->set_string_param("stave_geometry_file", "/phenix/hhj3/dcm07e/sPHENIX/macros/macros/g4simulations/mvtx_stave_v01.gdml");
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
      
      // Update the laddertype and ladder spacing configuration
      for(int i=0;i<n_intt_layer;i++)
	{
	  sitrack->set_int_param(i, "laddertype", laddertype[i]);
	  sitrack->set_int_param(i, "nladder", nladder[i]);
	  sitrack->set_double_param(i,"sensor_radius_inner", sensor_radius_inner[i]);  // expecting cm
	  sitrack->set_double_param(i,"sensor_radius_outer", sensor_radius_outer[i]);
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

  //-----------
  // SVTX cells
  //-----------

  if (verbosity)
  {
    cout << "  TPC Drift Velocity: " << TPCDriftVelocity << " cm/nsec" << endl;
    cout << "  TPC Transverse Diffusion: " << TPC_Trans_Diffusion << " cm/SQRT(cm)" << endl;
    cout << "  TPC Longitudinal Diffusion: " << TPC_Long_Diffusion << " cm/SQRT(cm)" << endl;
    cout << "  TPC dE/dx: " << TPC_dEdx << " keV/cm" << endl;
    cout << "  TPC N Primary: " << TPC_NPri << " electrons/cm" << endl;
    cout << "  TPC N Total: " << TPC_NTot << " electrons/cm" << endl;
    cout << "  TPC Electrons Per keV: " << TPC_ElectronsPerKeV << " electrons/keV" << endl;
    cout << "  TPC ADC Clock: " << TPCADCClock << " nsec" << endl;
    cout << "  TPC ADC Rate: " << 1000.0 / TPCADCClock << " MHZ" << endl;
    cout << "  TPC Shaping Lead: " << TPCShapingRMSLead << " nsec" << endl;
    cout << "  TPC Shaping Tail: " << TPCShapingRMSTail << " nsec" << endl;
    cout << "  TPC z cell " << tpc_cell_z << " cm" << endl;
    cout << "  TPC Smear R-Phi " << TPC_SmearRPhi << " cm" << endl;
    cout << "  TPC Smear Z " << TPC_SmearZ << " cm" << endl;
  }

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
    // The timing windows are hard-coded in the INTT ladder model
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

  // Set TPC gas transport parameters for primary ionization
  //========================================
  // These default to NeCF4-400 as follows:
  //set_default_double_param("diffusion_long",0.015); // cm/SQRT(cm)
  //set_default_double_param("diffusion_trans",0.006); // cm/SQRT(cm)
  //set_default_double_param("drift_velocity",8.0 / 1000.0); // cm/ns
  //set_default_double_param("electrons_per_gev",TPC_ElectronsPerKeV*1000000.);
  //set_default_double_param("min_active_radius",30.); // cm
  //set_default_double_param("max_active_radius",75.); // cm
  //set_default_double_param("min_time",0.); // ns
  //set_default_double_param("max_time",14000.); // ns

  // setup TPC readout for filling cells
  // g4tpc/PHG4TPCElectronDrift uses
  // g4tpc/PHG4TPCPadPlaneReadout
  //=========================

  PHG4TPCElectronDrift *edrift = new PHG4TPCElectronDrift();
  edrift->Detector("TPC");
  PHG4TPCPadPlane *padplane = new PHG4TPCPadPlaneReadout();
  edrift->registerPadPlane(padplane);
  se->registerSubsystem(edrift);

  // The pad plane readout default is set in PHG4TPCPadPlaneReadout
  // We may want to change the number of inner layers here
  padplane->set_int_param("ntpc_layers_inner",n_tpc_layer_inner); 
  padplane->set_int_param("ntpc_minlayer_inner",n_maps_layer+n_intt_layer); 
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
    // INTT
    std::vector<double> userrange;  // 3-bit ADC threshold relative to the mip_e at each layer.
    // these should be used for the INTT
    userrange.push_back(0.05);
    userrange.push_back(0.10);
    userrange.push_back(0.15);
    userrange.push_back(0.20);
    userrange.push_back(0.25);
    userrange.push_back(0.30);
    userrange.push_back(0.35);
    userrange.push_back(0.40);

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
  //-------------------------------------
  // defaults to 1.0 (fully active)

  PHG4SvtxDeadArea* deadarea = new PHG4SvtxDeadArea();

  for (int i = 0; i < n_maps_layer; i++)
  {
    deadarea->Verbosity(verbosity);
    //deadarea->set_hit_efficiency(i,0.99);
    deadarea->set_hit_efficiency(i, 1.0);
  }
  for (int i = n_maps_layer; i < n_maps_layer + n_intt_layer; i++)
  {
    //deadarea->set_hit_efficiency(i,0.99);
    deadarea->set_hit_efficiency(i, 1.0);
  }
  se->registerSubsystem(deadarea);

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
    thresholds->set_threshold(i, 0.1);
  }
  // INTT
  for (int i = n_maps_layer; i < n_maps_layer + n_intt_layer; i++)
  {
    thresholds->set_threshold(i, 0.1);
    thresholds->set_use_thickness_mip(i, true);
  }

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


  /*
  // no Z clustering for INTT layers (only)
  for (int i = n_maps_layer; i < n_maps_layer + n_intt_layer; i++)
  {
    clusterizer->set_z_clustering(i, false);
  }
  */

  se->registerSubsystem(clusterizer);

  // For the TPC
  
  PHG4TPCClusterizer* tpcclusterizer = new PHG4TPCClusterizer();
  tpcclusterizer->Verbosity(0);
  tpcclusterizer->setRangeLayers(n_maps_layer + n_intt_layer, Max_si_layer);
  tpcclusterizer->setEnergyCut(15 /*adc*/);
  tpcclusterizer->setFitWindowSigmas(0.0150, 0.0160);  // should be changed when TPC cluster resolution changes
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

void G4_Svtx_Reco()
{
  cout << "\033[31;1m"
       << "Warning: G4_Svtx_Reco() was moved to G4_Svtx.C and renamed to Svtx_Reco(), please update macros"
       << "\033[0m" << endl;
  Svtx_Reco();

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
  //eval = new SvtxEvaluator("SVTXEVALUATOR", outputfile.c_str());
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
    //evalp = new SvtxEvaluator("SVTXEVALUATOR", string(outputfile.c_str()) + "_primary_eval.root", "PrimaryTrackMap");
    evalp = new SvtxEvaluator("SVTXEVALUATOR", string(outputfile.c_str()) + "_primary_eval.root", "PrimaryTrackMap", n_maps_layer, n_intt_layer, n_gas_layer);    evalp->do_cluster_eval(true);
    evalp->do_g4hit_eval(true);
    evalp->do_hit_eval(false);
    evalp->do_gpoint_eval(false);
    evalp->scan_for_embedded(true);  // take all tracks if false - take only embedded tracks if true
    evalp->Verbosity(0);
    se->registerSubsystem(evalp);
  }

  // MomentumEvaluator* eval = new MomentumEvaluator(outputfile.c_str(),0.2,0.4,Max_si_layer,2,Max_si_layer-4,10.,80.);
  // se->registerSubsystem( eval );

  return;
}
