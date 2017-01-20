#include "G4_TPC.C"

const int n_ib_layer = 3;   // number of maps inner barrel layers
const int n_intt_layer = 4; // number of int. tracker layers. Make this number 0 to use MAPS + TPC only.
const int n_gas_layer = 40; // number of TPC layers

int Min_si_layer = 0;
int Max_si_layer = n_ib_layer + n_intt_layer + n_gas_layer;

void SvtxInit(int verbosity = 0)
{
  Min_si_layer = 0;
  Max_si_layer = n_ib_layer + n_intt_layer + n_gas_layer;
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

  // inner barrel
  
  double ib_si_thickness[3] = {0.0050, 0.0050, 0.0050};
  double ib_rad[3] = {svtx_inner_radius, 3.2, 3.9};
  double ib_support_thickness[3] = {0.0036, 0.0036, 0.0036};
  double ib_length[3] = {27.0, 27.0, 27.0};

  for (int ilayer=0;ilayer<n_ib_layer;++ilayer) {
    cyl = new PHG4CylinderSubsystem("SVTX", ilayer);
    cyl->Verbosity(verbosity);
    radius = ib_rad[ilayer];
    cyl->set_double_param("radius",radius);
    //cyl->set_int_param("lengthviarapidity",0);
    cyl->set_double_param("length",ib_length[ilayer]);
    cyl->set_string_param("material","G4_Si");
    cyl->set_double_param("thickness",ib_si_thickness[ilayer]);
    cyl->SetActive();
    cyl->SuperDetector("SVTX");
    g4Reco->registerSubsystem( cyl );
    
    radius += ib_si_thickness[ilayer] + no_overlapp;
    
    cyl = new PHG4CylinderSubsystem("SVTXSUPPORT", ilayer);
    cyl->Verbosity(verbosity);
    cyl->set_double_param("radius",radius);
    //cyl->set_int_param("lengthviarapidity",1);
    cyl->set_double_param("length",ib_length[ilayer]);
    cyl->set_string_param("material","G4_Cu");
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
    cyl->Verbosity(verbosity);
    radius = intt_rad[ilayer-n_ib_layer];
    cyl->set_double_param("radius",radius);
    cyl->set_int_param("lengthviarapidity",1);
    //cyl->set_double_param("length",intt_length[ilayer-n_ib_layer]);
    cyl->set_string_param("material","G4_Si");
    cyl->set_double_param("thickness",intt_si_thickness[ilayer-n_ib_layer]);
    cyl->SetActive();
    cyl->SuperDetector("SVTX");
    g4Reco->registerSubsystem( cyl );
    
    radius += intt_si_thickness[ilayer-n_ib_layer] + no_overlapp;
    
    cyl = new PHG4CylinderSubsystem("SVTXSUPPORT", ilayer);
    cyl->Verbosity(verbosity);
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
  
  //TPC
  radius = AddTPC2G4Geo(g4Reco,radius,0);

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

  PHG4CylinderCellTPCReco *svtx_cells = new PHG4CylinderCellTPCReco(n_ib_layer+n_intt_layer);
  svtx_cells->Verbosity(1);
  svtx_cells->Detector("SVTX");

  for (int i=0;i<n_ib_layer;++i) {
    svtx_cells->cellsize(i, svxcellsizex[i], svxcellsizey[i]);
    svtx_cells->set_timing_window(i, -2000.0, +2000.0);
  }
  for (int i=n_ib_layer;i<n_ib_layer+n_intt_layer;++i) {
    svtx_cells->cellsize(i, intt_cellsizex[i-n_ib_layer], intt_cellsizey[i-n_ib_layer]);
    svtx_cells->set_timing_window(i, -50.0, +50.0);
  }

  // TPC
  AddTPC2CellReco(svtx_cells);
  
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
    thresholds->set_threshold(i,0.25);  // reduce to 0.1 for increased efficiency

  for(int i=n_ib_layer;i<n_ib_layer+n_intt_layer;i++)
    thresholds->set_threshold(i,0.25);  

  se->registerSubsystem( thresholds );

  //-------------
  // Cluster Hits
  //-------------

  PHG4SvtxClusterizer* clusterizer = new PHG4SvtxClusterizer("PHG4SvtxClusterizer",Min_si_layer,n_ib_layer+n_intt_layer-1);
  clusterizer->set_threshold(0.25);  // reduced from 0.5, should be same as cell threshold, since many hits are single cell
  se->registerSubsystem( clusterizer );
  
  //---------------------
  // Track reconstruction
  //---------------------
  PHG4HoughTransformTPC* hough = new PHG4HoughTransformTPC(Max_si_layer,Max_si_layer-30);
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

  // silicon
  for (int i=0;i<n_ib_layer+n_intt_layer;++i) {
    hough->setVoteErrorScale(i, 1.0);
  }
  for (int i=0;i<n_ib_layer+n_intt_layer;++i) {
    hough->setFitErrorScale(i, 1./sqrt(12.));
  }

  //TPC
  AddTPC2Reco(digi,hough);
  
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
  eval->do_g4hit_eval(false);     // make g4hit ntuple
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
