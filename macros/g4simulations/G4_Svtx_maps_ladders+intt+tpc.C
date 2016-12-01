
const int n_maps_layer = 3;
const int n_intt_layer = 4;
const int n_gas_layer = 60;
double inner_cage_radius = 20.;

int Max_si_layer = n_maps_layer + n_intt_layer + n_gas_layer;

void SvtxInit(int verbosity = 0)
{
  Max_si_layer = n_maps_layer + n_intt_layer + n_gas_layer;
  double inner_cage_radius = 20.; // options of 20.0 or 30.0 cm
}

double Svtx(PHG4Reco* g4Reco, double radius, 
      const int absorberactive = 0,
      int verbosity = 0) {

  bool overlapcheck = false; // set to true if you want to check for overlaps

  // MAPS inner barrel layers 
  //======================================================

  //double maps_layer_radius[3] = {23.4, 31.5, 39.3};   // mm  - precise numbers from ITS.gdml
  double maps_layer_radius[3] = {23.635, 31.5, 39.385};   // mm  - adjusted for closest fit
  // type 1 = inner barrel stave, 2 = middle barrel stave, 3 = outer barrel stave
  int stave_type[3] = {0, 0, 0};
   
  for (int ilayer = 0; ilayer < n_maps_layer; ilayer++)
    {
      cout << "Create Maps layer " << ilayer  << " with radius " << maps_layer_radius[ilayer] << " mm, stave type " << stave_type[ilayer] 
	   << " pixel thickness " << 0.0018 << endl;

      PHG4MapsSubsystem  *lyr = new PHG4MapsSubsystem("MAPS", ilayer, stave_type[ilayer]);
      lyr->Verbosity(verbosity);

      lyr->set_double_param("layer_nominal_radius",maps_layer_radius[ilayer]);// thickness in cm

      // The cell size is used only during pixilization of sensor hits, but it is convemient to set it now because the geometry object needs it
      lyr->set_double_param("pixel_x",0.0020);// pitch in cm
      lyr->set_double_param("pixel_z",0.0020);// length in cm
      lyr->set_double_param("pixel_thickness",0.0018);// thickness in cm
      lyr->set_double_param("phitilt",0.304);   // radians, equivalent to 17.4 degrees

      lyr->set_int_param("active",1);
      lyr->OverlapCheck(overlapcheck);

      lyr->set_string_param("stave_geometry_file",
          string(getenv("CALIBRATIONROOT")) + string("/Tracking/geometry/ALICE_ITS_tgeo.gdml"));
      
      //if (verbosity)
	//cout << "Created Maps layer for layer " << ilayer << " with radius  " << maps_layer_radius[ilayer]  << endl;
      
      g4Reco->registerSubsystem( lyr );      

      //if (verbosity)
      //cout << "Registered  Maps layer for layer " << ilayer <<  endl;

      radius = maps_layer_radius[ilayer];
    }

  // Add 4 layers in cylinder cell geometry for INTT
  //===================================

  double si_thickness[4] = {0.020, 0.020, 0.020, 0.020};   // 0.21% of a radiation length
  double svxrad[4] = {6.0, 8.0, 10.0, 12.0};
  double support_thickness[4] = {0.0114, 0.0114, 0.0114, 0.0114}; // 0.79% of a radiation length
  double length[4] = {50.0, 50.0, 50.0, 50.0};

 for (int ilayer=n_maps_layer;ilayer<n_maps_layer+n_intt_layer;++ilayer) {

   cout << "Create Cylinder cell INTT  layer " << ilayer  << " with radius " << svxrad[ilayer - n_maps_layer] << " cm, length " << length[ilayer - n_maps_layer] 
	<< " si thickness " << si_thickness[ilayer - n_maps_layer] << endl;

    cyl = new PHG4CylinderSubsystem("SVTX", ilayer);
    radius = svxrad[ilayer - n_maps_layer];
    cyl->set_double_param("radius",radius);
    cyl->set_int_param("lengthviarapidity",0);
    cyl->set_double_param("length",length[ilayer - n_maps_layer]);
    cyl->set_string_param("material","G4_Si");
    cyl->set_double_param("thickness",si_thickness[ilayer-n_maps_layer]);
    cyl->SetActive();
    cyl->SuperDetector("SVTX");
    g4Reco->registerSubsystem( cyl );
    
    radius += si_thickness[ilayer - n_maps_layer] + no_overlapp;
    
    cyl = new PHG4CylinderSubsystem("SVTXSUPPORT", ilayer);
    cyl->set_double_param("radius",radius);
    cyl->set_int_param("lengthviarapidity",1);
    cyl->set_string_param("material","G4_Cu");
    cyl->set_double_param("thickness",support_thickness[ilayer - n_maps_layer]);
    cyl->SuperDetector("SVTXSUPPORT");
    g4Reco->registerSubsystem( cyl );
  }

  // time projection chamber layers --------------------------------------------

  PHG4CylinderSubsystem *cyl;

  radius = inner_cage_radius;
  
  double n_rad_length_cage = 1.0e-02;
  double cage_length = 160.; // rough length from Tom, also used in charge distortion calculation
  double cage_thickness = 1.43 * n_rad_length_cage;

  // inner field cage  
  cyl = new PHG4CylinderSubsystem("SVTXSUPPORT", n_maps_layer + n_intt_layer);
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

  // Layer of inert TPC gas from 20-30 cm
  if (inner_readout_radius - radius > 0) {
    cyl = new PHG4CylinderSubsystem("SVTXSUPPORT", n_maps_layer + n_intt_layer + 1);
    cyl->set_double_param("radius",radius);
    cyl->set_int_param("lengthviarapidity",0);
    cyl->set_double_param("length",cage_length);
    cyl->set_string_param("material",tpcgas.c_str());
    cyl->set_double_param("thickness",  inner_readout_radius  - radius);
    cyl->SuperDetector("SVTXSUPPORT");
    g4Reco->registerSubsystem( cyl );
  }

  radius = inner_readout_radius;
  
  double outer_radius = 78.;
  int npoints = Max_si_layer - n_maps_layer - n_intt_layer;
  double delta_radius =  ( outer_radius - inner_readout_radius )/( (double)npoints );
  
  for(int ilayer=n_maps_layer + n_intt_layer;ilayer<(n_maps_layer + n_intt_layer + npoints);++ilayer) {

    cout << "Create TPC gas layer " << ilayer  << " with radius " << radius  << " cm " << " thickness " << delta_radius - 0.01 << " length " << cage_length << endl;

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

  cyl = new PHG4CylinderSubsystem("SVTXSUPPORT", n_maps_layer + n_intt_layer + npoints);
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

  // MAPS cells
 PHG4MapsCellReco *maps_cells = new PHG4MapsCellReco("MAPS");
  maps_cells->Verbosity(verbosity);
  se->registerSubsystem(maps_cells);

  // INTT cells 
  double inttcellsizex[4] = {0.0080, 0.0080, 0.0080, 0.0080};
  double inttcellsizey[4] = {1.2, 1.2, 1.2, 1.2};

  // TPC cells
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

  PHG4CylinderCellTPCReco *svtx_cells = new PHG4CylinderCellTPCReco(n_maps_layer+n_intt_layer);
  svtx_cells->setDistortion(tpc_distortion); // apply TPC distrotion if tpc_distortion is not NULL
  svtx_cells->setDiffusion(diffusion);
  svtx_cells->setElectronsPerKeV(electrons_per_kev);
  svtx_cells->Detector("SVTX");

  // set cylinder cell (intt + TPC) cell sizes
  for (int i=n_maps_layer;i<n_maps_layer + n_intt_layer;++i) {
    svtx_cells->cellsize(i, inttcellsizex[i-n_maps_layer], inttcellsizey[i-n_maps_layer]);
    svtx_cells->set_timing_window(i, -2000.0, +2000.0);
  }
  for (int i=n_maps_layer + n_intt_layer;i<Max_si_layer;++i) {
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
  digi->Verbosity(verbosity);
  for (int i=0;i<n_maps_layer;++i) {
      digi->set_adc_scale(i, 255, 0.4e-6);  // reduced by a factor of 2.5 when going from maps thickess of 50 microns to 18 microns
  }

  for (int i=n_maps_layer;i<n_maps_layer + n_intt_layer;++i) {
    digi->set_adc_scale(i, 255, 1.0e-6);  // used for cylinder cell maps, wjere thickness was set to 50 microns
  }

  for (int i=n_maps_layer+n_intt_layer;i<Max_si_layer;++i) {
    digi->set_adc_scale(i, 10000, 1.0);
  }
  se->registerSubsystem( digi );
  
  //-------------------------------------
  // Apply Live Area Inefficiency to Hits
  //-------------------------------------
  // defaults to 1.0 (fully active)
  
  PHG4SvtxDeadArea* deadarea = new PHG4SvtxDeadArea();
  for(int i = 0;i<n_maps_layer;i++)
    {  
      deadarea->Verbosity(verbosity);
      deadarea->set_hit_efficiency(i,0.99);
    }
  for(int i=n_maps_layer;i<n_maps_layer + n_intt_layer;i++)
    {
      deadarea->set_hit_efficiency(i,0.99);
    }
  se->registerSubsystem( deadarea );

  //-----------------------------
  // Apply MIP thresholds to Hits
  //-----------------------------

  PHG4SvtxThresholds* thresholds = new PHG4SvtxThresholds();
  thresholds->Verbosity(verbosity);
  // reduced by x2.5 when going from cylinder maps with 50 microns thickness to actual maps with 18 microns thickness
  // to set it at a similar fraction of charge deposited
  // Note the non-use of set_using_thickness here, this is so that the shortest dimension of the cell sets the mip energy loss

  for(int i = 0;i<n_maps_layer;i++)
    {  
      thresholds->set_threshold(i,0.25);
    }
  for(int i=n_maps_layer;i<n_maps_layer + n_intt_layer;i++)
    {
      thresholds->set_threshold(i,0.25);
    }
  
  se->registerSubsystem( thresholds );

  //-------------
  // Cluster Hits
  //-------------

  PHG4SvtxClusterizer* clusterizer = new PHG4SvtxClusterizer("PHG4SvtxClusterizer",0, n_maps_layer + n_intt_layer - 1);
  clusterizer->Verbosity(verbosity);
  // Reduced by 2 relative to cylinder cell maps macro. I found this necessary to get full efficiency
  // Many hits in the present simulation are single cell hits, so it is not clear why the cluster 
  // threshold should be higher than the cell threshold
  //clusterizer->set_threshold(0.5);   // fraction of a mip
  clusterizer->set_threshold(0.1);   // fraction of a mip
  se->registerSubsystem( clusterizer );

  PHG4TPCClusterizer* tpcclusterizer = new PHG4TPCClusterizer("PHG4TPCClusterizer",3,4,n_maps_layer+n_intt_layer,Max_si_layer-1);
  tpcclusterizer->setEnergyCut(20.0*45.0/n_gas_layer);
  tpcclusterizer->Verbosity(verbosity);
  se->registerSubsystem( tpcclusterizer );


  //---------------------
  // Track reconstruction
  //---------------------
  PHG4HoughTransformTPC* hough = new PHG4HoughTransformTPC(Max_si_layer,Max_si_layer-20);
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
//  hough->Verbosity(21);
  double mat_scale = 1.0;

  for(int i = 0;i<n_maps_layer;i++)
    {  
      hough->set_material(0, mat_scale*0.003);
    }
  for(int i=n_maps_layer;i<n_maps_layer + n_intt_layer;i++)
    {
       hough->set_material(0, mat_scale*0.010);
    }

  hough->set_material(3, mat_scale*0.010);  // entrance to field cage

  for (int i=(n_maps_layer + n_intt_layer + 1);i<Max_si_layer;++i) {
    hough->set_material(i, mat_scale*0.06/n_gas_layer);
  }
  hough->setUseCellSize(true);
  
  for (int i=n_maps_layer + n_intt_layer;i<Max_si_layer;++i) {
    hough->setFitErrorScale(i, 1./sqrt(12.));
  }
  for (int i=n_maps_layer + n_intt_layer;i<Max_si_layer;++i) {
    hough->setVoteErrorScale(i, 1.0);
  }
  for (int i=0;i<n_maps_layer + n_intt_layer;++i) {
    hough->setVoteErrorScale(i, 1.0);
  }
  for (int i=0;i<n_maps_layer + n_intt_layer;++i) {
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
