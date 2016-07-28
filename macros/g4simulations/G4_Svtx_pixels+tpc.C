
int Min_si_layer = 0;
int Max_si_layer = 62;

int n_svx_layer = 2;

void SvtxInit(int verbosity = 0)
{
  Min_si_layer = 0;
  Max_si_layer = 62;
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
 
  PHG4CylinderSubsystem *cyl;

  // silicon layers ------------------------------------------------------------

  // inner pixels are a 13/19 ladder reconfiguration developed by MPM and YA
  // outer strips are from YA at the Santa Fe Tracking Workshop 10/27/2015
  // see: https://indico.bnl.gov/conferenceDisplay.py?confId=1364

  // The updated thicknesses from Yasuyuki are as follows:
  // For Si 1mm = 1.07% X_0
  // For Cu 1mm = 6.96% X_0
  // The thickness of the tracking layers is:
  // 0 Pixels: 1.3% X_0  (0.214% sensor + 1.086% support) sensor = 200 mc Si, support = 154 mc Cu
  // 1 Pixels: 1.3% X_0  (0.214% sensor + 1.086% support) sensor = 200 mc Si, support = 154 mc Cu
  
  double si_thickness[2] = {0.0200, 0.0200};
  double svxrad[2] = {svtx_inner_radius, 3.63};
  double support_thickness[2] = {0.0154, 0.0154};
  double length[2] = {20., 20.};

  for (int ilayer=0;ilayer<n_svx_layer;++ilayer) {
    cyl = new PHG4CylinderSubsystem("SVTX", ilayer);
    radius = svxrad[ilayer];
    cyl->set_double_param("radius",radius);
    cyl->set_int_param("lengthviarapidity",0);
    cyl->set_double_param("length",length[ilayer]);
    cyl->set_string_param("material","G4_Si");
    cyl->set_double_param("thickness",si_thickness[ilayer]);
    cyl->SetActive();
    cyl->SuperDetector("SVTX");
    g4Reco->registerSubsystem( cyl );
    
    radius += si_thickness[ilayer] + no_overlapp;
    
    cyl = new PHG4CylinderSubsystem("SVTXSUPPORT", ilayer);
    cyl->set_double_param("radius",radius);
    cyl->set_int_param("lengthviarapidity",0);
    cyl->set_double_param("length",length[ilayer]);
    cyl->set_string_param("material","G4_Cu");
    cyl->set_double_param("thickness",support_thickness[ilayer]);
    cyl->SuperDetector("SVTXSUPPORT");
    g4Reco->registerSubsystem( cyl );
  }

  // time projection chamber layers --------------------------------------------

  double inner_cage_radius = 30.;
  string tpcgas = "G4_Ar";

  radius = inner_cage_radius;
  
  double n_rad_length_cage = 1.0e-02;
  double cage_length = 400.;
  double cage_thickness = 1.43 * n_rad_length_cage;
  
  cyl = new PHG4CylinderSubsystem("SVTXSUPPORT", 3);
  cyl->set_double_param("radius",radius);
  cyl->set_int_param("lengthviarapidity",0);
  cyl->set_double_param("length",cage_length);
  cyl->set_string_param("material","G4_Cu");
  cyl->set_double_param("thickness",cage_thickness ); // Cu X_0 = 1.43 cm
  cyl->SuperDetector("SVTXSUPPORT");
  g4Reco->registerSubsystem( cyl );

  radius += cage_thickness;
  
  double outer_radius = 80.;
  int npoints = Max_si_layer - n_svx_layer;
  double delta_radius =  ( outer_radius - cage_thickness - radius )/( (double)npoints );
  
  for(int ilayer=n_svx_layer;ilayer<(2+npoints);++ilayer) {
    cyl = new PHG4CylinderSubsystem("SVTX", ilayer);
    cyl->set_double_param("radius",radius);
    cyl->set_int_param("lengthviarapidity",0);
    cyl->set_double_param("length",cage_length);
    cyl->set_string_param("material",tpcgas.c_str());
    cyl->set_double_param("thickness", delta_radius - 0.01 );
    cyl->SetActive();
    cyl->SuperDetector("SVTX");
    g4Reco->registerSubsystem( cyl );
    
    radius += delta_radius;
  }

  cyl = new PHG4CylinderSubsystem("SVTXSUPPORT", 2+npoints);
  cyl->set_double_param("radius",radius);
  cyl->set_int_param("lengthviarapidity",0);
  cyl->set_double_param("length",cage_length);
  cyl->set_string_param("material","G4_Cu");
  cyl->set_double_param("thickness",cage_thickness ); // Cu X_0 = 1.43 cm
  cyl->SuperDetector("SVTXSUPPORT");
  g4Reco->registerSubsystem( cyl );

  radius += cage_thickness;

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

  // 50 micron pixels + 60 micron strips
  double svxcellsizex[2] = {0.0050, 0.0050};

  // 425 micron pixels + 9.6 mm tracking strips
  double svxcellsizey[2] = {0.0425, 0.0425};

  double diffusion = 0.0057;
  double electrons_per_kev = 38.;
  
  // tpc_cell_x is the TPC pad size.  The actual hit resolution depends not only on this pad size but also on the diffusion in the gas and amplification step
  double tpc_cell_x = 0.12;
  // tpc_cell_y is the z "bin" size.  It is approximately the z resolution * sqrt(12)
  // eventually this will be replaced with an actual simulation of timing amplitude.
  double tpc_cell_y = 0.17;
  
  PHG4CylinderCellTPCReco *svtx_cells = new PHG4CylinderCellTPCReco();
  svtx_cells->setDiffusion(diffusion);
  svtx_cells->setElectronsPerKeV(electrons_per_kev);
  svtx_cells->Detector("SVTX");
  for (int i=0;i<n_svx_layer;++i) {
    svtx_cells->cellsize(i, svxcellsizex[i], svxcellsizey[i]);
  }
  for (int i=n_svx_layer;i<62;++i) {
    svtx_cells->cellsize(i, tpc_cell_x, tpc_cell_y);
  }
  
  se->registerSubsystem(svtx_cells);

  return;
}

void Svtx_Reco(int verbosity = 0)
{
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
  PHG4SvtxDigitizer* digi = new PHG4SvtxDigitizer();
  digi->Verbosity(0);
  for (int i=0;i<n_svx_layer;++i) {
    digi->set_adc_scale(i, 255, 1.0e-6);
  }
  for (int i=n_svx_layer;i<62;++i) {
    digi->set_adc_scale(i, 10000, 1.0e-1);
  }
  se->registerSubsystem( digi );
  
  //-------------------------------------
  // Apply Live Area Inefficiency to Hits
  //-------------------------------------
  // defaults to 1.0 (fully active)
  
  PHG4SvtxDeadArea* deadarea = new PHG4SvtxDeadArea();
  deadarea->Verbosity(verbosity);
  deadarea->set_hit_efficiency(0,0.880 * 0.9); // Run16 live pixels X perfect coverage fraction
  deadarea->set_hit_efficiency(1,0.705 * 0.9); // 90% is a place holder value
  se->registerSubsystem( deadarea );

  //-----------------------------
  // Apply MIP thresholds to Hits
  //-----------------------------

  PHG4SvtxThresholds* thresholds = new PHG4SvtxThresholds();
  thresholds->Verbosity(verbosity);
  thresholds->set_threshold(0,0.33);
  thresholds->set_threshold(1,0.33);
  thresholds->set_use_thickness_mip(0, true);
  se->registerSubsystem( thresholds );

  //-------------
  // Cluster Hits
  //------------- 
  PHG4TPCClusterizer* clusterizer = new PHG4TPCClusterizer("PHG4SvtxClusterizer",4,1);;
  se->registerSubsystem( clusterizer );

  //---------------------
  // Track reconstruction
  //---------------------
  PHG4HoughTransformTPC* hough = new PHG4HoughTransformTPC(62,56);
  hough->set_mag_field(1.4);
  hough->set_use_vertex(true);
  hough->setRemoveHits(true);
  hough->setRejectGhosts(true);
  hough->set_min_pT(0.2);
  hough->set_chi2_cut_full( 3.0 );
  hough->set_chi2_cut_init( 3.0 );

  hough->setBinScale(1.0);
  hough->setZBinScale(1.0);

  hough->Verbosity(0);
  double mat_scale = 1.0;
  hough->set_material(0, mat_scale*0.013);
  hough->set_material(1, mat_scale*0.013);
  hough->set_material(2, mat_scale*0.01);
  for (int i=3;i<62;++i) {
    hough->set_material(i, mat_scale*0.06/60.);
  }
  hough->setUseCellSize(false);
  
  for (int i=2;i<62;++i) {
    hough->setFitErrorScale(i, 1./sqrt(12.));
  }
  for (int i=2;i<62;++i) {
    hough->setVoteErrorScale(i, 0.2);
  }
  
  se->registerSubsystem( hough );
 
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

  // SubsysReco* eval = new SvtxEvaluator("SVTXEVALUATOR", outputfile.c_str());
  // eval->do_cluster_eval(false);
  // eval->do_g4hit_eval(false);
  // eval->do_hit_eval(false);
  // eval->do_gpoint_eval(false);
  // eval->Verbosity(verbosity);
  // se->registerSubsystem( eval );

  MomentumEvaluator* eval = new MomentumEvaluator(outputfile.c_str(),0.1,0.2,62,2,56,10.,80.);
  se->registerSubsystem( eval );
  
  return;
}
