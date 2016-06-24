
const int n_svx_layer = 3;
const int n_gas_layer = 60;
double inner_cage_radius = 20.;

int Min_si_layer = 0;
int Max_si_layer = n_svx_layer + n_gas_layer;

void SvtxInit(int verbosity = 0)
{
  Min_si_layer = 0;
  Max_si_layer = n_svx_layer + n_gas_layer;
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

  // inner pixels are a 13/19 ladder reconfiguration developed by MPM and YA
  // outer strips are from YA at the Santa Fe Tracking Workshop 10/27/2015
  // see: https://indico.bnl.gov/conferenceDisplay.py?confId=1364

  // The updated thicknesses from Yasuyuki are as follows:
  // For Si 1mm = 1.07% X_0
  // For Cu 1mm = 6.96% X_0
  // The thickness of the tracking layers is:
  // 0 Pixels: 1.3% X_0  (0.214% sensor + 1.086% support) sensor = 200 mc Si, support = 154 mc Cu
  // 1 Pixels: 1.3% X_0  (0.214% sensor + 1.086% support) sensor = 200 mc Si, support = 154 mc Cu
  
  double si_thickness[3] = {0.0050, 0.0050, 0.0050};
  double svxrad[3] = {svtx_inner_radius, 3.2, 3.9};
  double support_thickness[3] = {0.0036, 0.0036, 0.0036};
  double length[3] = {27.0, 27.0, 27.0};

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
	  cyl->set_int_param("lengthviarapidity",1);
	  //    cyl->SetLength( length[ilayer] );
      cyl->set_string_param("material","G4_Cu");
      cyl->set_double_param("thickness",support_thickness[ilayer]);
    cyl->SuperDetector("SVTXSUPPORT");
    g4Reco->registerSubsystem( cyl );
  }

  // time projection chamber layers --------------------------------------------

  radius = inner_cage_radius;
  
  double n_rad_length_cage = 1.0e-02;
  double cage_length = 160.; // rough length from Tom, also used in charge distortion calculation
  double cage_thickness = 1.43 * n_rad_length_cage;
  
  cyl = new PHG4CylinderSubsystem("SVTXSUPPORT", n_svx_layer);
  cyl->set_double_param("radius",radius);
  cyl->set_int_param("lengthviarapidity",0);
  cyl->set_double_param("length",cage_length);
  cyl->set_string_param("material","G4_Cu");
  cyl->set_double_param("thickness",cage_thickness ); // Cu X_0 = 1.43 cm
  cyl->SuperDetector("SVTXSUPPORT");
  g4Reco->registerSubsystem( cyl );

  radius += cage_thickness;

  double inner_readout_radius = 30.;
  if (inner_readout_radius<radius)  inner_readout_radius = radius;

  string tpcgas = "G4_Ar";

  if (inner_readout_radius - radius > 0) {
    cyl = new PHG4CylinderSubsystem("SVTXSUPPORT", n_svx_layer + 1);
    cyl->set_double_param("radius",radius);
    cyl->set_int_param("lengthviarapidity",0);
    cyl->set_double_param("length",cage_length);
    cyl->set_string_param("material",tpcgas.c_str());
    cyl->set_double_param("thickness",  inner_readout_radius  - radius);
    cyl->SuperDetector("SVTXSUPPORT");
    g4Reco->registerSubsystem( cyl );
  }

  radius = inner_readout_radius;
  
  double outer_radius = 80.;
  int npoints = Max_si_layer - n_svx_layer;
  double delta_radius =  ( outer_radius - inner_readout_radius )/( (double)npoints );
  
  for(int ilayer=n_svx_layer;ilayer<(n_svx_layer+npoints);++ilayer) {
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

  cyl = new PHG4CylinderSubsystem("SVTXSUPPORT", n_svx_layer+npoints);
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

  double svxcellsizex[3] = {0.0020, 0.0020, 0.0020};
  double svxcellsizey[3] = {0.0020, 0.0020, 0.0020};

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
  PHG4CylinderCellTPCReco *svtx_cells = new PHG4CylinderCellTPCReco(n_svx_layer);
  svtx_cells->setDistortion(tpc_distortion); // apply TPC distrotion if tpc_distortion is not NULL
  svtx_cells->setDiffusion(diffusion);
  svtx_cells->setElectronsPerKeV(electrons_per_kev);
  svtx_cells->Detector("SVTX");

  for (int i=0;i<n_svx_layer;++i) {
    svtx_cells->cellsize(i, svxcellsizex[i], svxcellsizey[i]);
    svtx_cells->set_timing_window(i, -2000.0, +2000.0);
  }
  for (int i=n_svx_layer;i<Max_si_layer;++i) {
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
  for (int i=0;i<n_svx_layer;++i) {
    digi->set_adc_scale(i, 255, 1.0e-6);
  }
  for (int i=n_svx_layer;i<Max_si_layer;++i) {
    digi->set_adc_scale(i, 10000, 1.0);
  }
  se->registerSubsystem( digi );
  
  //-------------------------------------
  // Apply Live Area Inefficiency to Hits
  //-------------------------------------
  // defaults to 1.0 (fully active)
  
  PHG4SvtxDeadArea* deadarea = new PHG4SvtxDeadArea();
  deadarea->Verbosity(verbosity);
  deadarea->set_hit_efficiency(0,0.99);
  deadarea->set_hit_efficiency(1,0.99);
  deadarea->set_hit_efficiency(2,0.99);
  se->registerSubsystem( deadarea );

  //-----------------------------
  // Apply MIP thresholds to Hits
  //-----------------------------

  PHG4SvtxThresholds* thresholds = new PHG4SvtxThresholds();
  thresholds->Verbosity(verbosity);
  thresholds->set_threshold(0,0.25);
  thresholds->set_threshold(1,0.25);
  thresholds->set_threshold(2,0.25);
  se->registerSubsystem( thresholds );

  //-------------
  // Cluster Hits
  //-------------

  PHG4SvtxClusterizer* clusterizer = new PHG4SvtxClusterizer("PHG4SvtxClusterizer",Min_si_layer,n_svx_layer-1);
  se->registerSubsystem( clusterizer );
  
  PHG4TPCClusterizer* tpcclusterizer = new PHG4TPCClusterizer("PHG4TPCClusterizer",3,4,n_svx_layer,Max_si_layer);
  tpcclusterizer->setEnergyCut(20.0*45.0/n_gas_layer);
  se->registerSubsystem( tpcclusterizer );
  
  //---------------------
  // Track reconstruction
  //---------------------
  PHG4HoughTransformTPC* hough = new PHG4HoughTransformTPC(Max_si_layer,Max_si_layer-6);
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
  hough->set_material(0, mat_scale*0.003);
  hough->set_material(1, mat_scale*0.003);
  hough->set_material(2, mat_scale*0.003);
  hough->set_material(3, mat_scale*0.010);
  for (int i=(n_svx_layer+1);i<Max_si_layer;++i) {
    hough->set_material(i, mat_scale*0.06/n_gas_layer);
  }
  hough->setUseCellSize(true);
  
  for (int i=n_svx_layer;i<Max_si_layer;++i) {
    hough->setFitErrorScale(i, 1./sqrt(12.));
  }
  for (int i=n_svx_layer;i<Max_si_layer;++i) {
    hough->setVoteErrorScale(i, 1.0);
  }
  for (int i=0;i<n_svx_layer;++i) {
    hough->setVoteErrorScale(i, 1.0);
  }
  for (int i=0;i<n_svx_layer;++i) {
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
  eval->do_cluster_eval(true);
  eval->do_g4hit_eval(true);
  eval->do_hit_eval(false);
  eval->do_gpoint_eval(false);
  eval->scan_for_embedded(true);
  eval->Verbosity(verbosity);
  se->registerSubsystem( eval );

  // MomentumEvaluator* eval = new MomentumEvaluator(outputfile.c_str(),0.2,0.4,Max_si_layer,2,Max_si_layer-4,10.,80.);
  // se->registerSubsystem( eval );
  
  return;
}
