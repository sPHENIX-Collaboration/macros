
int Min_si_layer = 0;
int Max_si_layer = 6;

void SvtxInit(int verbosity = 0)
{
  Min_si_layer = 0;
  Max_si_layer = 6;
}

double Svtx(PHG4Reco* g4Reco, double radius, 
	    const int absorberactive = 0,
	    int verbosity = 0) {
  
  float svtx_inner_radius = 2.48; // based on a 13/19 pixel ladder reconfig
  
  if (radius > svtx_inner_radius) {
    cout << "inconsistency: radius: " << radius 
	 << " larger than SVTX inner radius: " << svtx_inner_radius << endl;
    gSystem->Exit(-1);
  }
 
  //---------------
  // Load libraries
  //---------------

  gSystem->Load("libg4detectors.so");
  gSystem->Load("libg4testbench.so");

  PHG4CylinderSubsystem *cyl;
 
  //======================================================================================================
  // inner pixels are a 13/19 ladder reconfiguration developed by MPM and YA
  // outer strips are from YA at the Santa Fe Tracking Workshop 10/27/2015
  // see: https://indico.bnl.gov/conferenceDisplay.py?confId=1364

  // The updated thicknesses from Yasuyuki are as follows:
  // For Si 1mm = 1.07% X_0
  // For Cu 1mm = 6.96% X_0
  // The thickness of the tracking layers is:
  // 0 Pixels: 1.3% X_0  (0.214% sensor + 1.086% support) sensor = 200 mc Si, support = 154 mc Cu
  // 1 Pixels: 1.3% X_0  (0.214% sensor + 1.086% support) sensor = 200 mc Si, support = 154 mc Cu
  // 2 S0a:    1.0% X_0  (0.257% sensor + 0.743% support) sensor = 240 mc Si, support = 107 mc Cu
  // 3 S0b:    1.0% X_0  (0.257% sensor + 0.743% support) sensor = 240 mc Si, support = 107 mc Cu
  // 4 S1a:    0.6% X_0  (0.257% sensor + 0.343% support) sensor = 240 mc Si, support =  49 mc Cu
  // 5 S1a:    0.6% X_0  (0.257% sensor + 0.343% support) sensor = 240 mc Si, support =  49 mc Cu
  // 6 S2:     1.0% X_0  (0.342% sensor + 0.658% support) sensor = 320 mc Si, support =  95 mc Cu
  //=======================================================================================================

  double si_thickness[7] = {0.0200, 0.0200, 0.0240, 0.0240, 0.0240, 0.0240, 0.0320};
  double svxrad[7] = {svtx_inner_radius, 3.63, 7.5, 8.5, 31.0, 34.0, 64.0};
  double support_thickness[7] = {0.0154, 0.0154, 0.0107, 0.0107, 0.0049, 0.0049, 0.0095};
  double length[7] = {20., 20., -1, -1., -1., -1., -1}; // -1 use eta coverage to determine length

  // here is our silicon:
  double inner_radius = radius;
  for (int ilayer = Min_si_layer; ilayer <= Max_si_layer; ilayer++)
    {
      cyl = new PHG4CylinderSubsystem("SVTX", ilayer);
      radius = svxrad[ilayer];
      // protect against installing layer with radius < inner radius from argument
      if (radius < inner_radius)
	{
	  cout << "current radius " << radius << " smaller than inner radius "
	       << inner_radius << endl;
	  gSystem->Exit(-1);
	}
      cyl->SetRadius(radius);
      if (length[ilayer] > 0)
        {
          cyl->SetLengthViaRapidityCoverage(false);
          cyl->SetLength(length[ilayer]);
        }
      cyl->SetMaterial("G4_Si");
      cyl->SetThickness(si_thickness[ilayer]);
      cyl->SetActive();
      cyl->SuperDetector("SVTX");
      g4Reco->registerSubsystem( cyl );

      radius += si_thickness[ilayer] + no_overlapp;
      cyl = new PHG4CylinderSubsystem("SVTXSUPPORT", ilayer);
      cyl->SetRadius(radius);
      if (length[ilayer] > 0)
        {
          cyl->SetLengthViaRapidityCoverage(false);
          cyl->SetLength(length[ilayer]);
        }
      cyl->SetMaterial("G4_Cu");
      cyl->SetThickness(support_thickness[ilayer]);
      if (absorberactive)  cyl->SetActive();
      cyl->SuperDetector("SVTXSUPPORT");
      g4Reco->registerSubsystem( cyl );
    }
  if (ilayer != (Max_si_layer+1)) // coming out of the loop, layer is layer+1
    {
      cout << "layer number mismatch for Max_si_layer, Max_si_layer "
           << Max_si_layer << " should be " << ilayer << endl;
      gSystem->Exit(-1);
    }
  radius += support_thickness[Max_si_layer] + no_overlapp;

  if (verbosity > 0) {
    cout << "============================ G4_Svtx.C::Svtx() ============================" << endl;
    cout << " SVTX Material Description:" << endl;
    for (int ilayer = Min_si_layer; ilayer <= Max_si_layer; ilayer++) {
      cout << "  layer " << ilayer
	   << "  radius " << svxrad[ilayer]
	   << "  zlength " << length[ilayer]
	   << "  thickness (Si) " << si_thickness[ilayer]
	   << "  support thickness (Cu) " << support_thickness[ilayer]
	   << endl;
    }
    cout << "===========================================================================" << endl;
  }
  return radius;
}

void Svtx_Cells(int verbosity = 0)
{
  // runs the cellularization of the energy deposits (g4hits) 
  // into detector hits (g4cells)

  //---------------
  // Load libraries
  //---------------

  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4detectors.so");

  //---------------
  // Fun4All server
  //---------------

  Fun4AllServer *se = Fun4AllServer::instance();

  //-----------
  // SVTX cells
  //-----------

  // The pattern recognition layers (4 & 6) are set at 2 mm in Z and 240 microns 
  // pitch to make the area the same as the long strips
  // Layers 3, 5 and 7 are long strips parallel to the beam line

  // 50 micron pixels + 60 micron strips
  double svxcellsizex[7] = {0.0050, 0.0050, 0.0060, 0.0060, 0.0060, 0.0060, 0.0060}; 

  // 425 micron pixels + 9.6 mm tracking strips
  double svxcellsizey[7] = {0.0425, 0.0425, 0.9600, 0.96000, 0.9600, 0.9600, 0.9600};
  
  PHG4CylinderCellReco *svtx_cells = new PHG4CylinderCellReco();
  svtx_cells->Detector("SVTX");
  svtx_cells->Verbosity(verbosity);
  int idx = 0;
  for (int i = Min_si_layer; i <= Max_si_layer; ++i) {
    svtx_cells->cellsize(i, svxcellsizex[idx], svxcellsizey[idx]);
    ++idx;
  }
  se->registerSubsystem(svtx_cells);

  // strips are ganged together into a common readout channel
  PHG4SvtxAddConnectedCells *gang = new PHG4SvtxAddConnectedCells();
  gang->Verbosity(verbosity);
  gang->set_ncells_connected(4,3);
  gang->set_ncells_connected(5,3);
  gang->set_ncells_connected(6,6);
  se->registerSubsystem(gang);
  
  return;
}

void Svtx_Reco(int verbosity = 0)
{
  // reconstructs the MIE Svtx v2 detector (7 layers)

  // requires Svtx setup and Svtx cell routines
  // prefers calorimeter reconstruction prior (once projections are working)

  //---------------
  // Load libraries
  //---------------

  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4hough.so");

  //---------------
  // Fun4All server
  //---------------

  Fun4AllServer *se = Fun4AllServer::instance();

  //----------------------------------
  // Digitize the cell energy into ADC
  //----------------------------------
  // defaults to 8-bit ADC with MIP at 0.25% dynamic range
  PHG4SvtxDigitizer* digi = new PHG4SvtxDigitizer();
  digi->Verbosity(verbosity);
  digi->set_adc_scale(0, 255, 1.0e-6); // 1.0 keV / bit
  digi->set_adc_scale(1, 255, 1.0e-6); // 1.0 keV / bit
  digi->set_adc_scale(2, 255, 1.6e-6); // 1.6 keV / bit
  digi->set_adc_scale(3, 255, 1.6e-6); // 1.6 keV / bit
  digi->set_adc_scale(4, 255, 1.6e-6); // 1.6 keV / bit
  digi->set_adc_scale(5, 255, 1.6e-6); // 1.6 keV / bit
  digi->set_adc_scale(6, 255, 1.6e-6); // 1.6 keV / bit
  se->registerSubsystem( digi );
  
  //-------------------------------------
  // Apply Live Area Inefficiency to Hits
  //-------------------------------------
  // defaults to 1.0 (fully active)
  PHG4SvtxDeadArea* deadarea = new PHG4SvtxDeadArea();
  deadarea->Verbosity(verbosity);
  deadarea->set_hit_efficiency(0,0.925);
  deadarea->set_hit_efficiency(1,0.725);
  deadarea->set_hit_efficiency(2,0.98);
  deadarea->set_hit_efficiency(3,0.98);
  deadarea->set_hit_efficiency(4,0.98);
  deadarea->set_hit_efficiency(5,0.98);
  deadarea->set_hit_efficiency(6,0.98);
  se->registerSubsystem( deadarea );

  //-----------------------------
  // Apply MIP thresholds to Hits
  //-----------------------------
  PHG4SvtxThresholds* thresholds = new PHG4SvtxThresholds();
  thresholds->Verbosity(verbosity);
  thresholds->set_threshold(0,0.33);
  thresholds->set_threshold(1,0.33);
  thresholds->set_threshold(2,0.33);
  thresholds->set_threshold(3,0.33);
  thresholds->set_threshold(4,0.33);
  thresholds->set_threshold(5,0.33);
  thresholds->set_threshold(6,0.33);
  thresholds->set_use_thickness_mip(0, true);
  se->registerSubsystem( thresholds );

  //-------------
  // Cluster Hits
  //-------------
  // needs to have clusters hold hit ids not cell ids
  // will require changes to evaluation
  PHG4SvtxClusterizer* clusterizer = new PHG4SvtxClusterizer();
  clusterizer->Verbosity(verbosity);
  clusterizer->set_threshold(0.33);
  clusterizer->set_z_clustering(2, false);
  clusterizer->set_z_clustering(3, false);
  clusterizer->set_z_clustering(4, false);
  clusterizer->set_z_clustering(5, false);
  clusterizer->set_z_clustering(6, false);
  // clusterizer->set_energy_weighting(2, true);
  // clusterizer->set_energy_weighting(3, true);
  // clusterizer->set_energy_weighting(4, true);
  // clusterizer->set_energy_weighting(5, true);
  // clusterizer->set_energy_weighting(6, true);
  se->registerSubsystem( clusterizer );

  //---------------------
  // Track reconstruction
  //---------------------
  PHG4HoughTransform* hough = new PHG4HoughTransform(7,7);
  hough->set_mag_field(1.4);
  hough->Verbosity(verbosity);
  hough->set_material(0, 0.013);
  hough->set_material(1, 0.013);
  hough->set_material(2, 0.010);
  hough->set_material(3, 0.010);
  hough->set_material(4, 0.006);
  hough->set_material(5, 0.006);
  hough->set_material(6, 0.010);
  hough->setPtRescaleFactor(0.9972);
  hough->set_chi2_cut_init(3.0);
  hough->set_chi2_cut_full(3.0);
  hough->set_ca_chi2_cut(3.0);
  hough->setCutOnDCA(true);
  hough->setDCACut(0.1);
  hough->setDCAZCut(0.1);
  hough->setRejectGhosts(false);
  hough->setRemoveHits(false);
  se->registerSubsystem( hough );

  //---------------------
  // Ghost rejection
  //---------------------
  // needs updates to merge split tracks when possible
  PHG4TrackGhostRejection* rejection = new PHG4TrackGhostRejection(7); 
  rejection->Verbosity(verbosity);
  rejection->set_max_shared_hits(3);
  se->registerSubsystem( rejection );

  //------------------------
  // Final Track Refitting
  //------------------------
  // PHG4TrackKalmanFitter *kalman = new PHG4TrackKalmanFitter
  // we need a module to redo the Kalman fit with G4 material and real mag field
  // to update the track container

  //------------------------
  // Primary Track Refitting
  //------------------------
  // PHG4TrackKalmanFitter *kalman = new PHG4TrackKalmanFitter
  // we need a module to redo the Kalman fit including the vertex position
  // and create a separate stream of output tracks
  
  //------------------
  // Track Projections
  //------------------
  PHG4SvtxTrackProjection* projection = new PHG4SvtxTrackProjection();
  projection->Verbosity(verbosity);
  se->registerSubsystem( projection );

  //----------------------
  // Beam Spot Calculation
  //----------------------
  PHG4SvtxBeamSpotReco* beamspot = new PHG4SvtxBeamSpotReco();
  beamspot->Verbosity(verbosity);
  se->registerSubsystem( beamspot );

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

  Fun4AllServer *se = Fun4AllServer::instance();

  //----------------
  // SVTX evaluation
  //----------------

  SubsysReco* eval = new SvtxEvaluator("SVTXEVALUATOR", outputfile.c_str());
  eval->Verbosity(verbosity);
  se->registerSubsystem( eval );

  return;
}
