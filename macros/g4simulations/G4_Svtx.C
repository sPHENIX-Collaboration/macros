// reference SVTX macro used for MIE projections

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
  
  float svtx_inner_radius = 2.71;
  
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
  // The thicknesses from Yasuyuki on June 12, 2014 are as follows:
  // For Si 1mm = 1.07% X_0
  // For Cu 1mm = 6.96% X_0
  // The thickness of the tracking layers is:
  // Pixels:         1.3% X_0  (0.21% sensor +  1.07% support)  sensor = 200 mc Si, support = 154 mc Cu
  // Stripixel:      5%   X_0  (0.67% sensor + 4.3% support)    sensor = 624 mc Si, support = 618 mc Cu
  // Outer strips:   2%   X_0 (conservative)  (0.34% sensor + 1.66% support) sensor = 320 mc Si, support = 238 mc Cu
  //=======================================================================================================

  double si_thickness[7] = {0.02, 0.02, 0.032, 0.032, 0.032, 0.032, 0.032};
  double svxrad[7] = {svtx_inner_radius, 4.63, 9.5, 10.5, 44.5, 45.5, 80.0}; // provides 98 MeV Upsilon resolution
  // Thicknesses (in % X_0) of 1.3,1.3,2.7/2,2.7/2,2.0/2,2.0/2,2.0 - YA's most conservative case
  double support_thickness[7] = {0.0154, 0.0154, 0.0338/2.0, 0.0338/2.0, 0.0238/2.0, 0.0238/2.0, 0.0238};
  double length[7] = {20., 20., -1, -1., - 1., - 1., -1}; // -1 use eta coverage to determine length

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

  // 60 micron X strips, 240 micron pattern reco strips
  double svxcellsizex[7] = {0.0050, 0.0050, 0.0060, 0.0240, 0.0060, 0.0240, 0.0060}; 

  // 8 mm tracking strips, 2 mm pattern reco strips
  double svxcellsizey[7] = {0.0425, 0.0425, 0.8000, 0.2000, 0.8000, 0.2000, 0.8000};
  
  PHG4CylinderCellReco *svtx_cells = new PHG4CylinderCellReco();
  svtx_cells->Detector("SVTX");
  svtx_cells->Verbosity(verbosity);
  int idx = 0;
  for (int i = Min_si_layer; i <= Max_si_layer; ++i) {
    svtx_cells->cellsize(i, svxcellsizex[idx], svxcellsizey[idx]);
    ++idx;
  }
  se->registerSubsystem(svtx_cells);

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
  // deadarea->set_hit_efficiency(0,0.90);
  // deadarea->set_hit_efficiency(1,0.90);
  // deadarea->set_hit_efficiency(2,0.98);
  // deadarea->set_hit_efficiency(3,0.98);
  // deadarea->set_hit_efficiency(4,0.98);
  // deadarea->set_hit_efficiency(5,0.98);
  // deadarea->set_hit_efficiency(6,0.98);
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
  hough->Verbosity(verbosity);
  hough->set_mag_field(1.4);
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
