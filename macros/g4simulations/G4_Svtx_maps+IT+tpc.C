
const int n_ib_layer = 3;   // number of maps inner barrel layers
const int n_intt_layer = 4; // number of int. tracker layers. Make this number 0 to use MAPS + TPC only.
const int n_gas_layer = 60; // number of TPC layers
double inner_cage_radius = 20.;

int Min_si_layer = 0;
int Max_si_layer = n_ib_layer + n_intt_layer + n_gas_layer;

void SvtxInit(int verbosity = 0)
{
  Min_si_layer = 0;
  Max_si_layer = n_ib_layer + n_intt_layer + n_gas_layer;
  double inner_cage_radius = 20.; // options of 20.0 or 30.0 cm
}

double Svtx(PHG4Reco* g4Reco, double radius, 
      const int absorberactive = 0,
      int verbosity = 0) {

  float svtx_inner_radius = 2.3;
  
  if (radius > svtx_inner_radius) {
    cout << "inconsistency: radius: " << radius 
   << " larger than SVTX inner radius: " << svtx_inner_radius << endl;
    gSystem->Exit(-1);
  }
 
  PHG4CylinderSubsystem *cyl;

  // silicon layers ------------------------------------------------------------

  std::string silicon("G4_Si");
  std::string copper("G4_Cu");
  
  // inner barrel
  
  double ib_si_thickness[3] = {0.0050, 0.0050, 0.0050};
  double ib_rad[3] = {svtx_inner_radius, 3.2, 3.9};
  double ib_support_thickness[3] = {0.0036, 0.0036, 0.0036};
  double ib_length[3] = {27.0, 27.0, 27.0};

  for (int ilayer=0;ilayer<n_ib_layer;++ilayer) {
    cyl = new PHG4CylinderSubsystem("SVTX", ilayer);
    cyl->Verbosity(2);
    radius = ib_rad[ilayer];
    cyl->set_double_param("radius",radius);
    cyl->set_double_param("length",ib_length[ilayer]);
    cyl->set_string_param("material",silicon);
    cyl->set_double_param("thickness",ib_si_thickness[ilayer]);
    cyl->SetActive();
    cyl->SuperDetector("SVTX");
    g4Reco->registerSubsystem( cyl );
    
    cout << "Added SVTX        layer " << ilayer 
	 << " inner barrel layer         radius " << ib_rad[ilayer]
	 << " thickness " << ib_si_thickness[ilayer]
	 << " length " << ib_length[ilayer]
	 << " material " << silicon.c_str()
	 << endl;
    
    radius += ib_si_thickness[ilayer] + no_overlapp;
    
    cyl = new PHG4CylinderSubsystem("SVTXSUPPORT", ilayer);
    cyl->Verbosity(2);
    cyl->set_double_param("radius",radius);
    //cyl->set_int_param("lengthviarapidity",1);
    cyl->set_double_param("length",ib_length[ilayer]);
    cyl->set_string_param("material",copper);
    cyl->set_double_param("thickness",ib_support_thickness[ilayer]);
    cyl->SuperDetector("SVTXSUPPORT");
    g4Reco->registerSubsystem( cyl );

    cout << "Added inner barrel layer with radius " << ib_rad[ilayer]
	 << " si thickness " << ib_si_thickness[ilayer]
	 << " support thickness " << ib_support_thickness[ilayer]
	 << " length " << ib_length[ilayer]
	 << endl;
  }

  // intermediate tracker
  // parameters from RIKEN
  double intt_si_thickness[4] = {0.0120, 0.0120, 0.0120,0.0120};
  double intt_rad[4] = { 6.0, 8.0, 10.0, 12.0};
  // 120 microns of silicon is 0.13% of X_0, so to get 1% total we need 0.87% more in the Cu
  double multiplier = 0.87;  // how many times 1% do you want?
  double apercent = 0.0144;  // Cu thickness in cm corresponding to 1% X_0 
  double intt_support_thickness[4] = {apercent*multiplier, apercent*multiplier, apercent*multiplier, apercent*multiplier};
  double intt_length[4] = {50.0, 50.0, 50.0, 50.0};

  for (int ilayer=n_ib_layer;ilayer<n_intt_layer+n_ib_layer;++ilayer) {
    cyl = new PHG4CylinderSubsystem("SVTX", ilayer);
    cyl->Verbosity(2);
    radius = intt_rad[ilayer-n_ib_layer];
    cyl->set_double_param("radius",radius);
    cyl->set_int_param("lengthviarapidity",1);
    //cyl->set_double_param("length",intt_length[ilayer-n_ib_layer]);
    cyl->set_string_param("material","G4_Si");
    cyl->set_double_param("thickness",intt_si_thickness[ilayer-n_ib_layer]);
    cyl->SetActive();
    cyl->SuperDetector("SVTX");
    g4Reco->registerSubsystem( cyl );

    cout << "Added SVTX        layer " << ilayer 
	 << " INTT layer                 radius " << intt_rad[ilayer-n_ib_layer]
	 << " thickness " << intt_si_thickness[ilayer-n_ib_layer]
	 << " length " << intt_length[ilayer-n_ib_layer]
	 << " material " << silicon.c_str()
	 << endl;
    
    radius += intt_si_thickness[ilayer-n_ib_layer] + no_overlapp;
    
    cyl = new PHG4CylinderSubsystem("SVTXSUPPORT", ilayer);
    cyl->Verbosity(2);
    cyl->set_double_param("radius",radius);
    cyl->set_int_param("lengthviarapidity",1);
    //cyl->set_double_param("length", intt_length[ilayer-n_ib_layer]);
    cyl->set_string_param("material","G4_Cu");
    cyl->set_double_param("thickness",intt_support_thickness[ilayer-n_ib_layer]);
    cyl->SuperDetector("SVTXSUPPORT");
    g4Reco->registerSubsystem( cyl );
 
    cout << "Added intermediate tracker layer with radius " << intt_rad[ilayer-n_ib_layer]
	 << " si thickness " << intt_si_thickness[ilayer-n_ib_layer]
	 << " support thickness " << intt_support_thickness[ilayer-n_ib_layer]
	 << " length " << intt_length[ilayer-n_ib_layer]
	 << endl;
  }
  
  // time projection chamber layers --------------------------------------------
  
  // inner field cage wall
  radius = inner_cage_radius;
  
  double n_rad_length_cage = 1.0e-02;
  double cage_length = 160.; // rough length from Tom, also used in charge distortion calculation
  double cage_thickness = 1.43 * n_rad_length_cage;
  
  cyl = new PHG4CylinderSubsystem("SVTXSUPPORT", n_ib_layer+n_intt_layer);
  cyl->Verbosity(2);
  cyl->set_double_param("radius",radius);
  cyl->set_int_param("lengthviarapidity",0);
  cyl->set_double_param("length",cage_length);
  cyl->set_string_param("material","G4_Cu");
  cyl->set_double_param("thickness",cage_thickness ); // Cu X_0 = 1.43 cm
  cyl->SuperDetector("SVTXSUPPORT");
  g4Reco->registerSubsystem( cyl );

  cout << "Added SVTXSUPPORT layer " << n_ib_layer+n_intt_layer << " TPC inner field cage layer radius " << radius
       << " thickness " << cage_thickness
       << " length " << cage_length
       << " material " << "G4_Cu"
       << endl;
  
  radius += cage_thickness;

  // TPC gas material in region not read out
  double inner_readout_radius = 30.;
  if (inner_readout_radius<radius)  inner_readout_radius = radius;

  string tpcgas = "G4_Ar";

  if (inner_readout_radius - radius > 0) {
    cyl = new PHG4CylinderSubsystem("SVTXSUPPORT", n_ib_layer + n_intt_layer+1);
    cyl->Verbosity(2);
    cyl->set_double_param("radius",radius);
    cyl->set_int_param("lengthviarapidity",0);
    cyl->set_double_param("length",cage_length);
    cyl->set_string_param("material",tpcgas.c_str());
    cyl->set_double_param("thickness",  inner_readout_radius  - radius);
    cyl->SuperDetector("SVTXSUPPORT");
    g4Reco->registerSubsystem( cyl );

    cout << "Added SVTXSUPPORT layer " << n_ib_layer+n_intt_layer+1 << " TPC inactive gas layer     radius " << radius
	 << " thickness " << inner_readout_radius - radius
	 << " length " << cage_length
	 << " material " << tpcgas.c_str()
	 << endl;
  }
  
  
  radius = inner_readout_radius;

  // Active TPC gas layers  
  double outer_radius = 80.; // should be 78 cm, right?
  int npoints = Max_si_layer - n_ib_layer-n_intt_layer;
  double delta_radius =  ( outer_radius - inner_readout_radius )/( (double)npoints );
  
  for(int ilayer=n_ib_layer+n_intt_layer;ilayer<(n_ib_layer+n_intt_layer+npoints);++ilayer) {
    cyl = new PHG4CylinderSubsystem("SVTX", ilayer);
    cyl->Verbosity(2);
    cyl->set_double_param("radius",radius);
    cyl->set_int_param("lengthviarapidity",0);
    cyl->set_double_param("length",cage_length);
    cyl->set_string_param("material",tpcgas.c_str());
    cyl->set_double_param("thickness", delta_radius - 0.01 );
    cyl->SetActive();
    cyl->SuperDetector("SVTX");
    g4Reco->registerSubsystem( cyl );

    cout << "Added SVTX        layer " << ilayer << " TPC active gas layer       radius " << radius
	 << " thickness " << delta_radius - 0.01
	 << " length " << cage_length
	 << " material " << tpcgas.c_str()
	 << endl;
    
    radius += delta_radius;
  }

  // outer field cage wall
  cyl = new PHG4CylinderSubsystem("SVTXSUPPORT", n_ib_layer+n_intt_layer+npoints);
  cyl->Verbosity(2);
  cyl->set_double_param("radius",radius);
  cyl->set_int_param("lengthviarapidity",0);
  cyl->set_double_param("length",cage_length);
  cyl->set_string_param("material","G4_Cu");
  cyl->set_double_param("thickness",cage_thickness ); // Cu X_0 = 1.43 cm
  cyl->SuperDetector("SVTXSUPPORT");
  g4Reco->registerSubsystem( cyl );

  cout << "Added SVTXSUPPORT layer " << n_ib_layer+n_intt_layer+npoints << " TPC outer field cage layer radius " << radius
       << " thickness " << cage_thickness
       << " length " << cage_length
       << " material " << copper.c_str()
       << endl;

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
  // cells
  //-----------

  // inner barrel
  double svxcellsizex[3] = {0.0020, 0.0020, 0.0020};
  double svxcellsizey[3] = {0.0020, 0.0020, 0.0020};

  // intermediate tracker
  double intt_cellsizex[4] = { 0.0080, 0.0080, 0.0080, 0.0080}; // cm
  double intt_cellsizey[4] = { 1.2, 1.2, 1.2, 1.2}; // cm

  // TPC
  double diffusion = 0.0057;
  double electrons_per_kev = 38.;
  
  // tpc_cell_x is the TPC pad size.  The actual hit resolution depends not only on this pad size but also on the diffusion in the gas and amplification step
  double tpc_cell_x = 0.12;
  // tpc_cell_y is the z "bin" size.  It is approximately the z resolution * sqrt(12)
  // eventually this will be replaced with an actual simulation of timing amplitude.
  double tpc_cell_y = 0.17;
  
  // Main switch for TPC distortion
  const bool do_tpc_distoration = true;
  PHG4TPCSpaceChargeDistortion* tpc_distortion = NULL;
  if (do_tpc_distoration) {
    if (inner_cage_radius != 20. && inner_cage_radius != 30.) {
      cout << "Svtx_Cells - Fatal Error - TPC distoration required that "
              "inner_cage_radius is either 20 or 30 cm."
           << endl;
      exit(3);
    }
    string TPC_distroation_file =
        string(getenv("CALIBRATIONROOT")) +
        Form("/Tracking/TPC/SpaceChargeDistortion/sPHENIX%.0f.root",
             inner_cage_radius);
    PHG4TPCSpaceChargeDistortion* tpc_distortion =
        new PHG4TPCSpaceChargeDistortion(TPC_distroation_file);
    //  tpc_distortion -> setAccuracy(0); // option to over write default
    //  factors
    //  tpc_distortion -> setPrecision(1); // option to over write default
    //  factors
  }
  PHG4CylinderCellTPCReco *svtx_cells = new PHG4CylinderCellTPCReco(n_ib_layer+n_intt_layer);
  svtx_cells->setDistortion(tpc_distortion); // apply TPC distrotion if tpc_distortion is not NULL
  svtx_cells->setDiffusion(diffusion);
  svtx_cells->setElectronsPerKeV(electrons_per_kev);
  svtx_cells->Detector("SVTX");

  for (int i=0;i<n_ib_layer;++i) {
    svtx_cells->cellsize(i, svxcellsizex[i], svxcellsizey[i]);
    svtx_cells->set_timing_window(i, -2000.0, +2000.0);
  }
  for (int i=n_ib_layer;i<n_ib_layer+n_intt_layer;++i) {
    svtx_cells->cellsize(i, intt_cellsizex[i-n_ib_layer], intt_cellsizey[i-n_ib_layer]);
    svtx_cells->set_timing_window(i, -50.0, +50.0);
  }
  for (int i=n_ib_layer+n_intt_layer;i<Max_si_layer;++i) {
    svtx_cells->cellsize(i, tpc_cell_x, tpc_cell_y);
    svtx_cells->set_timing_window(i, -14000.0, +14000.0);
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
  for (int i=0;i<n_ib_layer+n_intt_layer;++i) {
    digi->set_adc_scale(i, 255, 1.0e-6);
  }
  for (int i=n_ib_layer+n_intt_layer;i<Max_si_layer;++i) {
    digi->set_adc_scale(i, 10000, 1.0);
  }
  se->registerSubsystem( digi );
  
  //-------------------------------------
  // Apply Live Area Inefficiency to Hits
  //-------------------------------------
  // defaults to 1.0 (fully active)
  
  PHG4SvtxDeadArea* deadarea = new PHG4SvtxDeadArea();
  deadarea->Verbosity(verbosity);
  for(int i=0;i<n_ib_layer;i++)
    deadarea->set_hit_efficiency(i,0.99);

  for(int i=n_ib_layer;i<n_ib_layer+n_intt_layer;i++)
    deadarea->set_hit_efficiency(i-n_ib_layer,0.99);

  se->registerSubsystem( deadarea );

  //-----------------------------
  // Apply MIP thresholds to Hits
  //-----------------------------

  PHG4SvtxThresholds* thresholds = new PHG4SvtxThresholds();
  thresholds->Verbosity(verbosity);
  for(int i=0;i<n_ib_layer;i++)
    thresholds->set_threshold(i,0.25); 

  for(int i=n_ib_layer;i<n_ib_layer+n_intt_layer;i++)
    thresholds->set_threshold(i,0.25);  

  se->registerSubsystem( thresholds );

  //-------------
  // Cluster Hits
  //-------------

  PHG4SvtxClusterizer* clusterizer = new PHG4SvtxClusterizer("PHG4SvtxClusterizer",Min_si_layer,n_ib_layer+n_intt_layer-1);
  clusterizer->set_threshold(0.25);  // should be same as cell threshold, since many hits are single cell
  se->registerSubsystem( clusterizer );
  
  PHG4TPCClusterizer* tpcclusterizer = new PHG4TPCClusterizer("PHG4TPCClusterizer",3,4,n_ib_layer+n_intt_layer,Max_si_layer);
  tpcclusterizer->setEnergyCut(20.0*45.0/n_gas_layer);
  se->registerSubsystem( tpcclusterizer );
  
  //---------------------
  // Track reconstruction
  //---------------------

  PHG4HoughTransformTPC* hough = new PHG4HoughTransformTPC(Max_si_layer,Max_si_layer-4); // allow 4 missed layers-> trade-off between reco track eff. and clusters/layer
  hough->set_mag_field(1.4);
  hough->setPtRescaleFactor(1.00/0.993892);
  hough->set_use_vertex(true);
  hough->setRemoveHits(true);
  hough->setRejectGhosts(true);
  hough->set_min_pT(0.2);
  hough->set_chi2_cut_full( 2.0 );
  hough->set_chi2_cut_init( 2.0 );

  hough->setBinScale(1.0);
  hough->setZBinScale(1.0);

  hough->Verbosity(verbosity);

  double mat_scale = 1.0;
  // maps inner barrel, total of 0.3% of X_0 per layer
  for(int i=0;i<n_ib_layer;i++)
    hough->set_material(i, mat_scale*0.003);
  // intermediate tracker, total 1% of X_0 pair layer
  for(int i=n_ib_layer;i<n_ib_layer+n_intt_layer;i++)
    hough->set_material(i-n_ib_layer, mat_scale*0.010);
  // TPC inner field cage wall, 1% of X_0
  hough->set_material(n_ib_layer+n_intt_layer, mat_scale*0.010);
  // material for inactive gas region here?
  // TPC gas
  for (int i=(n_ib_layer+n_intt_layer+1);i<Max_si_layer;++i) {
    hough->set_material(i, mat_scale*0.06/n_gas_layer);
  }
  hough->setUseCellSize(true);

  // TPC
  for (int i=n_ib_layer+n_intt_layer;i<Max_si_layer;++i) {
    hough->setFitErrorScale(i, 1./sqrt(12.));
  }
  for (int i=n_ib_layer+n_intt_layer;i<Max_si_layer;++i) {
    hough->setVoteErrorScale(i, 1.0);
  }
  // silicon
  for (int i=0;i<n_ib_layer+n_intt_layer;++i) {
    hough->setVoteErrorScale(i, 1.0);
  }
  for (int i=0;i<n_ib_layer+n_intt_layer;++i) {
    hough->setFitErrorScale(i, 1./sqrt(12.));
  }
  
  se->registerSubsystem( hough );

  //-----------------------
  // Momentum Recalibration
  //----------------------- 
  TF1 *corr = new TF1("corr","1.0/(1+0.00908642+5.91337e-05*x+-1.87201e-05*x*x+-3.31928e-06*x*x*x+1.03004e-07*x*x*x*x+-1.05111e-09*x*x*x*x*x)",0.0,40.0);
  PHG4SvtxMomentumRecal* recal = new PHG4SvtxMomentumRecal("PHG4SvtxMomentumRecal",corr);
  se->registerSubsystem(recal);
    
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

  SvtxEvaluator* eval = new SvtxEvaluator("SVTXEVALUATOR", outputfile.c_str());
  eval->do_cluster_eval(true);   // make cluster ntuple
  eval->do_g4hit_eval(false);     // make g4hit ntuple  NOTE: set to true in intt version
  eval->do_hit_eval(false);         // make hit ntuple
  eval->do_gpoint_eval(false);  
  //eval->scan_for_embedded(true);  // evaluator will only collect embedded tracks - it will also ignore decay tracks from embedded particles!
  eval->scan_for_embedded(false); // evaluator takes all tracks
  eval->Verbosity(verbosity);
  se->registerSubsystem( eval );

  // MomentumEvaluator* eval = new MomentumEvaluator(outputfile.c_str(),0.2,0.4,Max_si_layer,2,Max_si_layer-4,10.,80.);
  // se->registerSubsystem( eval );
  
  return;
}
