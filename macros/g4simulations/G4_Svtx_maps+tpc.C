
int Min_si_layer = 0;
int Max_si_layer = 63;

int n_svx_layer = 3;

void SvtxInit(int verbosity = 0)
{
  Min_si_layer = 0;
  Max_si_layer = 63;
}

double Svtx(PHG4Reco* g4Reco, double radius, 
	    const int absorberactive = 0,
	    int verbosity = 0) {

  float svtx_inner_radius = 2.335; // based on the ALICE ITS (same beam pipe as ours)
  
  if (radius > svtx_inner_radius) {
    cout << "inconsistency: radius: " << radius 
	 << " larger than SVTX inner radius: " << svtx_inner_radius << endl;
    gSystem->Exit(-1);
  }
 
  PHG4CylinderSubsystem *cyl;

  // silicon layers ------------------------------------------------------------

  // inner pixels are a copy of the MAPS ITS inner layers
  // outer strips are from YA at the Santa Fe Tracking Workshop 10/27/2015
  // see: https://indico.bnl.gov/conferenceDisplay.py?confId=1364

  // The updated thicknesses from Yasuyuki are as follows:
  // For Si 1mm = 1.07% X_0
  // For Cu 1mm = 6.96% X_0
  // The thickness of the tracking layers is:
  // 0 MAPS: 0.3% X_0  (0.053% sensor + 0.247% support) sensor =  50 mc Si, support =  35 mc Cu
  // 1 MAPS: 0.3% X_0  (0.053% sensor + 0.247% support) sensor =  50 mc Si, support =  35 mc Cu
  // 2 MAPS: 0.3% X_0  (0.053% sensor + 0.247% support) sensor =  50 mc Si, support =  35 mc Cu
  
  double si_thickness[3] = {0.0050, 0.0050, 0.0050};
  double svxrad[3] = {svtx_inner_radius, 3.132, 3.904};
  double support_thickness[3] = {0.0035, 0.0035, 0.0035};
  double length[3] = {27., 27., 27.};

  for (int ilayer=0;ilayer<n_svx_layer;++ilayer) {
    cyl = new PHG4CylinderSubsystem("SVTX", ilayer);
    radius = svxrad[ilayer];
    cyl->SetRadius(radius);
    cyl->SetLength( length[ilayer] );
    cyl->SetMaterial("G4_Si");
    cyl->SetThickness( si_thickness[ilayer] );
    cyl->SetActive();
    cyl->SuperDetector("SVTX");
    g4Reco->registerSubsystem( cyl );
    
    radius += si_thickness[ilayer] + no_overlapp;
    
    cyl = new PHG4CylinderSubsystem("SVTXSUPPORT", ilayer);
    cyl->SetRadius(radius);
    cyl->SetLength( length[ilayer] );
    cyl->SetMaterial("G4_Cu");
    cyl->SetThickness( support_thickness[ilayer] );
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
  cyl->SetRadius(radius);
  cyl->SetLength(cage_length);
  cyl->SetMaterial("G4_Cu");
  cyl->SetThickness( cage_thickness ); // Cu X_0 = 1.43 cm
  cyl->SuperDetector("SVTXSUPPORT");
  g4Reco->registerSubsystem( cyl );

  radius += cage_thickness;
  
  double outer_radius = 80.;
  int npoints = Max_si_layer - n_svx_layer;
  double delta_radius =  ( outer_radius - cage_thickness - radius )/( (double)npoints );
  
  for(int ilayer=n_svx_layer;ilayer<(2+npoints);++ilayer) {
    cyl = new PHG4CylinderSubsystem("SVTX", ilayer);
    cyl->SetRadius(radius);
    cyl->SetLength( cage_length );
    cyl->SetMaterial(tpcgas.c_str());
    cyl->SetThickness(  delta_radius - 0.01 );
    cyl->SetActive();
    cyl->SuperDetector("SVTX");
    g4Reco->registerSubsystem( cyl );
    
    radius += delta_radius;
  }

  cyl = new PHG4CylinderSubsystem("SVTXSUPPORT", 2+npoints);
  cyl->SetRadius(radius);
  cyl->SetLength(cage_length);
  cyl->SetMaterial("G4_Cu");
  cyl->SetThickness( cage_thickness ); // Cu X_0 = 1.43 cm
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
  double svxcellsizex[3] = {0.0028, 0.0028, 0.0028};

  // 425 micron pixels + 9.6 mm tracking strips
  double svxcellsizey[3] = {0.0028, 0.0028, 0.0028};

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
  for (int i=n_svx_layer;i<63;++i) {
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
  for (int i=n_svx_layer;i<63;++i) {
    digi->set_adc_scale(i, 10000, 1.0e-1);
  }
  se->registerSubsystem( digi );
  
  //-------------------------------------
  // Apply Live Area Inefficiency to Hits
  //-------------------------------------
  // defaults to 1.0 (fully active)
  
  PHG4SvtxDeadArea* deadarea = new PHG4SvtxDeadArea();
  deadarea->Verbosity(verbosity);
  deadarea->set_hit_efficiency(0,0.998);
  deadarea->set_hit_efficiency(1,0.998);
  deadarea->set_hit_efficiency(2,0.998);
  se->registerSubsystem( deadarea );

  //-----------------------------
  // Apply MIP thresholds to Hits
  //-----------------------------

  PHG4SvtxThresholds* thresholds = new PHG4SvtxThresholds();
  thresholds->Verbosity(verbosity);
  thresholds->set_threshold(0,0.33);
  thresholds->set_threshold(1,0.33);
  thresholds->set_threshold(2,0.33);
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
  PHG4HoughTransformTPC* hough = new PHG4HoughTransformTPC(63,56);
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
  hough->set_material(0, mat_scale*0.003);
  hough->set_material(1, mat_scale*0.003);
  hough->set_material(2, mat_scale*0.003);
  hough->set_material(3, mat_scale*0.01);
  for (int i=4;i<63;++i) {
    hough->set_material(i, mat_scale*0.06/60.);
  }
  hough->setUseCellSize(false);
  
  for (int i=3;i<63;++i) {
    hough->setFitErrorScale(i, 1./sqrt(12.));
  }
  for (int i=3;i<63;++i) {
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

  MomentumEvaluator* eval = new MomentumEvaluator(outputfile.c_str(),0.1,0.2,63,2,56,10.,80.);
  se->registerSubsystem( eval );
  
  return;
}
