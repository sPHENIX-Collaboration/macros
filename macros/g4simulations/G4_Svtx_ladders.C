// development macro for realistic outer strip ladder geometries

int Min_si_layer = 0;
int Max_si_layer = 6;
 
void SvtxInit(int verbosity = 0)
{
  Min_si_layer = 0;
  Max_si_layer = 6;
}

double Svtx(PHG4Reco* g4Reco, double radius, 
	    const int absorberactive = 0,
	    int verbosity = 0)
{

  //---------------
  // Load libraries
  //---------------

  gSystem->Load("libg4detectors.so");
  gSystem->Load("libg4testbench.so");

  //---------------------------------
  // Inner Cylinder layers for pixels
  //--------------------------------- 

  PHG4CylinderSubsystem *cyl;
 
  //======================================================================================================
  // The thicknesses from Yasuyuki on June 12, 2014 are as follows:
  // For Si 1mm = 1.07% X_0
  // For Cu 1mm = 6.96% X_0
  // The thickness of the tracking layers is:
  // Pixels:         1.3% X_0  (0.21% sensor +  1.07% support)  sensor = 200 mc Si, support = 154 mc Cu
  //=======================================================================================================

  double si_thickness[2] = {0.02, 0.02};
  double svxrad[2] = {2.71, 4.63};
  double support_thickness[2] = {0.0154, 0.0154};
  double length[2] = {20., 20.};

  // here is our silicon:
  double inner_radius = radius;
  for (int ilayer = Min_si_layer; ilayer < 2; ilayer++)
    {
      cyl = new PHG4CylinderSubsystem("SVTX", ilayer);
      cyl->Verbosity(verbosity);
      radius = svxrad[ilayer];
      // protect against installing layer with radius < inner radius from argument
      if (radius < inner_radius)
	{
	  cout << "current radius " << radius << " smaller than inner radius "
	       << inner_radius << endl;
	  gSystem->Exit(-1);
	}
      cyl->set_double_param("radius",radius);
      if (length[ilayer] > 0)
        {
          cyl->set_int_param("lengthviarapidity",0);
          cyl->set_double_param("length",length[ilayer]);
        }
      else
	{
          cyl->set_int_param("lengthviarapidity",1);
	}
      cyl->set_string_param("material","G4_Si");
      cyl->set_double_param("thickness",si_thickness[ilayer]);
      cyl->SetActive();
      cyl->SuperDetector("SVTX");
      g4Reco->registerSubsystem( cyl );

      radius += si_thickness[ilayer] + no_overlapp;
      cyl = new PHG4CylinderSubsystem("SVTXSUPPORT", ilayer);
      cyl->Verbosity(verbosity);
      cyl->set_double_param("radius",radius);
      if (length[ilayer] > 0)
        {
          cyl->set_int_param("lengthviarapidity",0);
          cyl->set_double_param("length",length[ilayer]);
        }
      else
	{
          cyl->set_int_param("lengthviarapidity",1);
	}
      cyl->set_string_param("material","G4_Cu");
      cyl->set_double_param("thickness",support_thickness[ilayer]);
      if (absorberactive)  cyl->SetActive();
      cyl->SuperDetector("SVTXSUPPORT");
      g4Reco->registerSubsystem( cyl );
    }

  //--------------------------------
  // Outer Silicon tracking subsytem
  //--------------------------------
  
  bool overlapcheck = false; // set to true if you want to check for overlaps
  
  // instantiate the Silicon tracker subsystem and register it
  // We make one instance of PHG4TrackerSubsystem per layer of tracker
  // dimensions are in mm, angles are in radians
 
  bool option_double_layer[5] = {false, true, false, true, false};
  double layer_radius[5] = {85.0, 85.0, 400.0, 400.0, 800.0};
  int N_strips_sensor_phi[5] = {256, 256, 512, 512, 1536}; 
  double radius_stagger[5] = {10.0, 10.0, 10.0, 10.0, 10.0};
  int N_staggers[5] = {4, 4, 4, 4, 2};
  bool add_lower_roc[5] = {false, false, true, true, true};
  double strip_tilt[5] = {0.0156, -0.0156, 0.0156, -0.0156, 0.0};  // radians, usually 1:64 tilt
  //double strip_tilt[num_si_layers] = {0.0, 0.0, 0.0, 0.0, 0.0};  // radians, usually 1:64 tilt

  for(int ilayer = 2; ilayer <= Max_si_layer; ++ilayer)
    {  
      // PHG4SiliconTrackerSubsystem creates the detetor layer using PHG4SiliconTrackerDetector
      // and instantiates the appropriate PHG4SteppingAction
      PHG4SiliconTrackerSubsystem *sitrack = new PHG4SiliconTrackerSubsystem("SILICON_TRACKER", ilayer); 
      sitrack->Verbosity(verbosity);
      sitrack->set_nominal_layer_radius(layer_radius[ilayer-2]);   // mm
      sitrack->set_radius_stagger(radius_stagger[ilayer-2]);          // mm
      sitrack->set_N_staggers(N_staggers[ilayer-2]);
      sitrack->set_N_strips_in_sensor_phi(N_strips_sensor_phi[ilayer-2]);
      sitrack->set_strip_tilt(strip_tilt[ilayer-2]);      // radians
      sitrack->set_option_double_layer(option_double_layer[ilayer-2]);
      sitrack->set_add_lower_roc(add_lower_roc[ilayer-2]);
      sitrack->SetActive();
      sitrack->OverlapCheck(overlapcheck);
      g4Reco->registerSubsystem( sitrack);
    }

  // report roughly our outer radius marker (translation back to cm)
  return layer_radius[4]*0.1+radius_stagger[4]*0.1;
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

  // 60 micron X strips, 240 micron pattern reco strips
  double svxcellsizex[2] = {0.0050, 0.0050};

  // 8 mm tracking strips, 2 mm pattern reco strips
  double svxcellsizey[2] = {0.0425, 0.0425};
  
  PHG4CylinderCellReco *svtx_cells = new PHG4CylinderCellReco();
  svtx_cells->Detector("SVTX");
  svtx_cells->Verbosity(verbosity);
  int idx = 0;
  for (int i = Min_si_layer; i < 2; ++i) {
    svtx_cells->cellsize(i, svxcellsizex[idx], svxcellsizey[idx]);
    ++idx;
  }
  se->registerSubsystem(svtx_cells);
  
  PHG4SiliconTrackerCellReco *reco = new PHG4SiliconTrackerCellReco("SILICON_TRACKER");
  reco->Verbosity(verbosity);
  se->registerSubsystem(reco);

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
  // digi->set_adc_scale(0,   1, 6.40e-6); // 6.4 keV / bit
  // digi->set_adc_scale(1,   1, 6.40e-6); // 6.4 keV / bit
  // digi->set_adc_scale(2, 255, 0.36e-6); // 0.36 keV / bit
  // digi->set_adc_scale(3, 255, 1.45e-6); // 1.45 keV / bit
  // digi->set_adc_scale(4, 255, 0.36e-6); // 0.36 keV / bit
  // digi->set_adc_scale(5, 255, 1.45e-6); // 1.45 keV / bit
  // digi->set_adc_scale(6, 255, 0.36e-6); // 0.36 keV / bit
  se->registerSubsystem( digi );
  
  //-------------------------------------
  // Apply Live Area Inefficiency to Hits
  //-------------------------------------
  // defaults to 1.0 (fully active)
  PHG4SvtxDeadArea* deadarea = new PHG4SvtxDeadArea();
  deadarea->Verbosity(verbosity);
  // deadarea->set_hit_efficiency(0,0.995);
  // deadarea->set_hit_efficiency(1,0.995);
  // deadarea->set_hit_efficiency(2,0.995);
  // deadarea->set_hit_efficiency(3,0.995);
  // deadarea->set_hit_efficiency(4,0.995);
  // deadarea->set_hit_efficiency(5,0.995);
  // deadarea->set_hit_efficiency(6,0.995);
  se->registerSubsystem( deadarea );

  //-----------------------------
  // Apply MIP thresholds to Hits
  //-----------------------------
  PHG4SvtxThresholds* thresholds = new PHG4SvtxThresholds();
  thresholds->Verbosity(verbosity);
  thresholds->set_threshold(0,0.25);
  thresholds->set_threshold(1,0.25);
  thresholds->set_threshold(2,0.25);
  thresholds->set_threshold(3,0.25);
  thresholds->set_threshold(4,0.25);
  thresholds->set_threshold(5,0.25);
  thresholds->set_threshold(6,0.25);
  se->registerSubsystem( thresholds );
 
  //---------------------
  // Make SVTX clusters
  //---------------------
  PHG4SvtxClusterizer* clusterizer = new PHG4SvtxClusterizer();
  clusterizer->Verbosity(verbosity);
  clusterizer->set_threshold(0.25);
  clusterizer->set_z_clustering(2, false);
  clusterizer->set_z_clustering(3, false);
  clusterizer->set_z_clustering(4, false);
  clusterizer->set_z_clustering(5, false);
  clusterizer->set_z_clustering(6, false);
  se->registerSubsystem( clusterizer );
 
  //---------------------
  // Track reconstruction
  //---------------------
  PHG4HoughTransform* hough = new PHG4HoughTransform(7,7);
  hough->set_mag_field(1.4);
  hough->Verbosity(verbosity);
  hough->set_material(0, 0.013);
  hough->set_material(1, 0.013);
  hough->set_material(2, 0.013);
  hough->set_material(3, 0.013);
  hough->set_material(4, 0.010);
  hough->set_material(5, 0.010);
  hough->set_material(6, 0.020);
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
  PHG4TrackGhostRejection* rejection = new PHG4TrackGhostRejection(7);
  rejection->Verbosity(verbosity);
  rejection->set_max_shared_hits(3);
  se->registerSubsystem( rejection );

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

  SubsysReco* eval = new PHG4Evaluator("PHG4EVALUATOR", outputfile.c_str());
  eval->Verbosity(verbosity);
  se->registerSubsystem( eval );

  return;
}
