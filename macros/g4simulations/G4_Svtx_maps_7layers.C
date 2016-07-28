// development macro for a full ITS style silicon detector

int Min_si_layer = 0;
int Max_si_layer = 6;

void SvtxInit()
{
  Min_si_layer = 0;
  Max_si_layer = 6;
}

double Svtx(PHG4Reco* g4Reco,
	    double radius,
	    const int absorberactive = 0,
	    int verbosity = 0)
{

  float svtx_inner_radius = 2.3;
  
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

  //double svxrad[7] = {2.3, 3.2, 3.9, 19.6, 24.5, 34.4, 39.3}; // ALICE ITS upgrade layer radii in cm
  double svxrad[7] = {svtx_inner_radius, 3.2, 3.9, 19.6, 24.5, 34.4, 64.0}; // ALICE ITS upgrade with outer layer pushed out
  double si_thickness[7] = {0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005};  // ALICE ITS upgrade Si thickness is 50 microns
  double length[7] = {27.0, 27.0, 27.0, 88.0, 88.0, 148.0, 148.0}; // ALICE ladder lengths (stave - 2cm)

  // ALICE ITS total thickness (% of X_0 of 0.3, 0.3, 0.3, 0.8, 0.8, 0.8, 0.8
  // Pixel chip thickness of 50 um (% of X_0 of 0.05 x 1.07 = 0.053%) in all layers
  // so inner 3 layers support thickness = 0.3 - 0.053 = 0.25%, outer 4 layers = 0.8 - 0.053 = 0.75%
  // Support thickness equivalent for Cu in inner layer = 0.25%/6.96% x 1 mm = 0.036 mm = 0.0036 cm
  // Support thickness equivalent for Cu in outer layer = 0.75%/6.96% x 1 mm = 0.108 mm = 0.0108 cm
  double support_thickness[7] = {0.0036, 0.0036, 0.0036, 0.0108, 0.0108, 0.0108, 0.0108};

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
    cout << "========================== G4_Svtx_ITS.C::Svtx() ==========================" << endl;
    cout << " SVTX Material Description: " << endl;
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

  // updated for ITS layers

  // 28 um / sqrt(2) from typical cluster size (not yet simulated for MAPS)
  double svxcellsizex[7] = {0.0020, 0.0020, 0.0020, 0.0020, 0.0020, 0.0020, 0.0020};
  double svxcellsizey[7] = {0.0020, 0.0020, 0.0020, 0.0020, 0.0020, 0.0020, 0.0020};
  
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
  // reconstructs the Svtx with ITS innner detector (7 layers)

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
  
  //------------------------------------------
  // Apply Live Area Inefficiency to Hit Cells
  //------------------------------------------
  // defaults to 1.0 (fully active)
  PHG4SvtxDeadArea* deadarea = new PHG4SvtxDeadArea();
  deadarea->Verbosity(verbosity);
  deadarea->set_hit_efficiency(0,0.99); // Leo says use 1% inefficiency
  deadarea->set_hit_efficiency(1,0.99);
  deadarea->set_hit_efficiency(2,0.99);
  deadarea->set_hit_efficiency(3,0.99);
  deadarea->set_hit_efficiency(4,0.99);
  deadarea->set_hit_efficiency(5,0.99);
  deadarea->set_hit_efficiency(6,0.99);
  se->registerSubsystem( deadarea );
  
  //----------------------------------
  // Apply MIP thresholds to Hit Cells
  //----------------------------------
  PHG4SvtxThresholds* thresholds = new PHG4SvtxThresholds();
  thresholds->Verbosity(verbosity);
  thresholds->set_threshold(0,0.25);
  thresholds->set_threshold(1,0.25);
  thresholds->set_threshold(2,0.25);
  thresholds->set_threshold(3,0.25);
  thresholds->set_threshold(4,0.25);
  thresholds->set_threshold(5,0.25);
  thresholds->set_threshold(6,0.25);
  //thresholds->set_use_thickness_mip(0, true);
  se->registerSubsystem( thresholds );

  //---------------------
  // Make SVTX clusters
  //---------------------
  PHG4SvtxClusterizer* clusterizer = new PHG4SvtxClusterizer();
  clusterizer->Verbosity(verbosity);
  clusterizer->set_threshold(0.33);
  se->registerSubsystem( clusterizer );

  //---------------------
  // Track reconstruction
  //---------------------
  PHG4HoughTransform* hough = new PHG4HoughTransform(7,7);
  hough->set_mag_field(1.4);
  hough->Verbosity(verbosity);
  // ALICE ITS upgrade values for total thickness in X_0
  hough->set_material(0, 0.003);
  hough->set_material(1, 0.003);
  hough->set_material(2, 0.003);
  hough->set_material(3, 0.008);
  hough->set_material(4, 0.008);
  hough->set_material(5, 0.008);
  hough->set_material(6, 0.008);
  hough->setPtRescaleFactor(0.9972/1.00117);
  hough->set_chi2_cut_init(5.0);
  //hough->set_chi2_cut_fast(60.0,0.0,100.0); // 10.0, 50.0, 75.0
  hough->set_chi2_cut_fast(10.0,50.0,75.0); // 10.0, 50.0, 75.0
  hough->set_chi2_cut_full(5.0);
  hough->set_ca_chi2_cut(5.0);
  hough->setMaxClusterError(3.0);
  hough->setRejectGhosts(false);
  hough->setRemoveHits(false);
  hough->setCutOnDCA(true);
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

  SubsysReco* eval = new SvtxEvaluator("SvtxEVALUATOR", outputfile.c_str());
  eval->Verbosity(verbosity);
  se->registerSubsystem( eval );

  return;
}
