#include <vector>

const int n_maps_layer = 3;
const int n_intt_layer = 4;   // must be 0-4, setting this to zero will remove the INTT completely, n < 4 gives you the first n layers
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

  bool maps_overlapcheck = false; // set to true if you want to check for overlaps

  // MAPS inner barrel layers 
  //======================================================

  //double maps_layer_radius[3] = {23.4, 31.5, 39.3};   // mm  - precise numbers from ITS.gdml
  double maps_layer_radius[3] = {23.635, 31.5, 39.385};   // mm  - adjusted for closest fit
  // type 1 = inner barrel stave, 2 = middle barrel stave, 3 = outer barrel stave
  // we use only type 0 here
  int stave_type[3] = {0, 0, 0};
   
  for (int ilayer = 0; ilayer < n_maps_layer; ilayer++)
    {
      if (verbosity)
      cout << "Create Maps layer " << ilayer  << " with radius " << maps_layer_radius[ilayer] << " mm, stave type " << stave_type[ilayer] 
	   << " pixel size 30 x 30 microns " << " active pixel thickness 0.0018 microns" << endl;

      PHG4MapsSubsystem  *lyr = new PHG4MapsSubsystem("MAPS", ilayer, stave_type[ilayer]);
      lyr->Verbosity(verbosity);

      lyr->set_double_param("layer_nominal_radius",maps_layer_radius[ilayer]);// thickness in cm

      // The cell size is used only during pixilization of sensor hits, but it is convemient to set it now because the geometry object needs it
      lyr->set_double_param("pixel_x",0.0030);// pitch in cm
      lyr->set_double_param("pixel_z",0.0030);// length in cm
      lyr->set_double_param("pixel_thickness",0.0018);// thickness in cm
      lyr->set_double_param("phitilt",0.304);   // radians, equivalent to 17.4 degrees

      lyr->set_int_param("active",1);
      lyr->OverlapCheck(maps_overlapcheck);

      lyr->set_string_param("stave_geometry_file",
          string(getenv("CALIBRATIONROOT")) + string("/Tracking/geometry/ALICE_ITS_tgeo.gdml"));
      
      g4Reco->registerSubsystem( lyr );      

      radius = maps_layer_radius[ilayer];
    }

  if(n_intt_layer > 0)
    {
      //-------------------
      // INTT ladders
      //-------------------
      
      bool intt_overlapcheck = false; // set to true if you want to check for overlaps
      
      // instantiate the Silicon tracker subsystem and register it
      // We make one instance of PHG4TrackerSubsystem for all four layers of tracker
      // dimensions are in mm, angles are in radians
      
      // PHG4SiliconTrackerSubsystem creates the detetor layer using PHG4SiliconTrackerDetector
      // and instantiates the appropriate PHG4SteppingAction
      const double intt_radius_max = 140.; // including stagger radius (mm)
      
      // The length of vpair is used to determine the number of layers
      std::vector<std::pair<int, int>> vpair; // (sphxlayer, inttlayer)
      for(int i=0;i<n_intt_layer;i++)
	{
	  // We want the sPHENIX layer numbers for the INTT to be from n_maps_layer to n_maps_layer+n_intt_layer - 1
	  vpair.push_back(std::make_pair(n_maps_layer+i, i));  // sphxlayer=n_maps_layer+i corresponding to inttlayer=i
	  if (verbosity)	  cout << "Create strip tracker layer " << vpair[i].second << " as  sphenix layer  "  << vpair[i].first << endl;
	}
      PHG4SiliconTrackerSubsystem *sitrack = new PHG4SiliconTrackerSubsystem("SILICON_TRACKER", vpair);
      sitrack->Verbosity(verbosity);
      sitrack->SetActive(1);
      sitrack->OverlapCheck(intt_overlapcheck);
      g4Reco->registerSubsystem( sitrack);
      
      // outer radius marker (translation back to cm)
      radius = intt_radius_max*0.1;
    }

  // time projection chamber layers --------------------------------------------

  PHG4CylinderSubsystem *cyl;

  radius = inner_cage_radius;
  
  double cage_length = 211.0; // From TPC group, gives eta = 1.1 at 78 cm
  double n_rad_length_cage = 1.13e-02;
  double cage_thickness = 28.6 * n_rad_length_cage;   // Kapton X_0 = 28.6 cm  // mocks up Kapton + carbon fiber structure

  // inner field cage  
  cyl = new PHG4CylinderSubsystem("SVTXSUPPORT", n_maps_layer + n_intt_layer);
  cyl->set_double_param("radius",radius);
  cyl->set_int_param("lengthviarapidity",0);
  cyl->set_double_param("length",cage_length);
  cyl->set_string_param("material","G4_KAPTON");
  cyl->set_double_param("thickness",cage_thickness ); 
  cyl->SuperDetector("SVTXSUPPORT");
  g4Reco->registerSubsystem( cyl );

  radius += cage_thickness;

  double inner_readout_radius = 30.;
  if (inner_readout_radius<radius)  inner_readout_radius = radius;

  string tpcgas = "G4_Ar";
  //string tpcgas = "sPHENIX_TPC_Gas";  // leave this change until later - will require some parameter retuning

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

      if (verbosity)
        cout << "Create TPC gas layer " << ilayer  << " with radius " << radius  << " cm "
	       << " thickness " << delta_radius - 0.01 << " length " << cage_length << endl;

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

  // outer field cage
  cyl = new PHG4CylinderSubsystem("SVTXSUPPORT", n_maps_layer + n_intt_layer + npoints);
  cyl->set_double_param("radius",radius);
  cyl->set_int_param("lengthviarapidity",0);
  cyl->set_double_param("length",cage_length);
  cyl->set_string_param("material","G4_KAPTON");
  cyl->set_double_param("thickness",cage_thickness ); // Kapton X_0 = 28.6 cm
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
  for(int ilayer = 0;ilayer < n_maps_layer;ilayer++)
    {
      maps_cells->set_timing_window(ilayer,-2000,2000);
    }
  se->registerSubsystem(maps_cells);

  if(n_intt_layer > 0)
    {
      // INTT cells
      PHG4SiliconTrackerCellReco *reco = new PHG4SiliconTrackerCellReco("SILICON_TRACKER");
      // The timing windows are hard-coded in the INTT ladder model
      reco->Verbosity(verbosity);
      se->registerSubsystem(reco);
    }

  // TPC cells
  double diffusion = 0.0057; //0.012: Ne(96%),CF4, etc mm/sqrt(cm) 0.0057: by Alan
  double electrons_per_kev = 38.;//28.,  38.;
  
  // tpc_cell_x is the TPC pad size.  The actual hit resolution depends not only on this pad size but also on the diffusion in the gas and amplification step
  double tpc_cell_x = 0.12;
  // tpc_cell_y is the z "bin" size.  It is approximately the z resolution * sqrt(12)
  // eventually this will be replaced with an actual simulation of timing amplitude.
  double tpc_cell_y = 0.17;
  
  // Main switch for TPC distortion
  const bool do_tpc_distortion = true;
  PHG4TPCSpaceChargeDistortion* tpc_distortion = NULL;
  if (do_tpc_distortion) {
    if (inner_cage_radius != 20. && inner_cage_radius != 30.) {
      cout << "Svtx_Cells - Fatal Error - TPC distortion required that "
              "inner_cage_radius is either 20 or 30 cm."
           << endl;
      exit(3);
    }
    

    string TPC_distortion_file =
        string(getenv("CALIBRATIONROOT")) +
        Form("/Tracking/TPC/SpaceChargeDistortion/TPCCAGE_20_78_211_2.root"); 
    PHG4TPCSpaceChargeDistortion* tpc_distortion =
        new PHG4TPCSpaceChargeDistortion(TPC_distortion_file);
    //tpc_distortion -> setAccuracy(0); // option to over write default  factors
    //tpc_distortion -> setPrecision(0.001); // option to over write default  factors      // default is 0.001
  }

  PHG4CylinderCellTPCReco *svtx_cells = new PHG4CylinderCellTPCReco(n_maps_layer+n_intt_layer);
  svtx_cells->Detector("SVTX");
  svtx_cells->setDistortion(tpc_distortion);
  svtx_cells->setDiffusionT(0.0120);
  svtx_cells->setDiffusionL(0.0120);
  svtx_cells->setSmearRPhi(0.09);  // additional smearing of cluster positions 
  svtx_cells->setSmearZ(0.06);       // additional smearing of cluster positions 
  svtx_cells->set_drift_velocity(6.0/1000.0l);
  svtx_cells->setHalfLength( 105.5 );
  svtx_cells->setElectronsPerKeV(28);
  svtx_cells->Verbosity(0);

  // The maps cell size is set when the detector is constructed because it is needed by the geometry object
  // The INTT ladder cell size is set in the detector construction code
  // set cylinder cell TPC cell sizes
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

  if(n_intt_layer > 0)
    {
      // INTT
      std::vector<double> userrange; // 3-bit ADC threshold relative to the mip_e at each layer.
      // these should be used for the INTT
      userrange.push_back(0.05);
      userrange.push_back(0.10);
      userrange.push_back(0.15);
      userrange.push_back(0.20);
      userrange.push_back(0.25);
      userrange.push_back(0.30);
      userrange.push_back(0.35);
      userrange.push_back(0.40);
      
      PHG4SiliconTrackerDigitizer* digiintt = new PHG4SiliconTrackerDigitizer();
      digiintt->Verbosity(verbosity);
      for(int i=0;i<n_intt_layer;i++)
	{
	  digiintt->set_adc_scale(n_maps_layer+i, userrange);
	}
      se->registerSubsystem( digiintt );
    }

  // TPC layers
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
      //deadarea->set_hit_efficiency(i,0.99);
      deadarea->set_hit_efficiency(i,1.0);
    }
  for(int i=n_maps_layer;i<n_maps_layer + n_intt_layer;i++)
    {
      //deadarea->set_hit_efficiency(i,0.99);
      deadarea->set_hit_efficiency(i,1.0);
    }
  se->registerSubsystem( deadarea );

  //-----------------------------
  // Apply MIP thresholds to Hits
  //-----------------------------

  PHG4SvtxThresholds* thresholds = new PHG4SvtxThresholds();
  thresholds->Verbosity(verbosity);
 
  // maps 
  for(int i = 0;i<n_maps_layer;i++)
    {  
      // reduced by x2.5 when going from cylinder maps with 50 microns thickness to actual maps with 18 microns thickness
      // Note the non-use of set_using_thickness here, this is so that the shortest dimension of the cell sets the mip energy loss
      thresholds->set_threshold(i,0.1); 
    }
  // INTT
  for(int i=n_maps_layer;i<n_maps_layer + n_intt_layer;i++)
    {
      thresholds->set_threshold(i,0.25);
      thresholds->set_use_thickness_mip(i, true);

    }
  
  se->registerSubsystem( thresholds );

  //-------------
  // Cluster Hits
  //-------------

  PHG4SvtxClusterizer* clusterizer = new PHG4SvtxClusterizer("PHG4SvtxClusterizer",0, n_maps_layer + n_intt_layer-1);
  clusterizer->Verbosity(verbosity);
  // Reduced by 2 relative to the cylinder cell maps macro. I found this necessary to get full efficiency
  // Many hits in the present simulation are single cell hits, so it is not clear why the cluster threshold should be higher than the cell threshold
  clusterizer->set_threshold(0.1);   // fraction of a mip
  // no Z clustering for INTT layers (only)
  for(int i=n_maps_layer;i<n_maps_layer+n_intt_layer;i++)
    {
      clusterizer->set_z_clustering(i, false);
    }

  se->registerSubsystem( clusterizer );

  PHG4TPCClusterizer* tpcclusterizer = new PHG4TPCClusterizer();
  tpcclusterizer->Verbosity(0);
  tpcclusterizer->setEnergyCut(15/*adc*/);
  tpcclusterizer->setRangeLayers(n_maps_layer+n_intt_layer,Max_si_layer);
  tpcclusterizer->setFitWindowSigmas(0.0150,0.0160);  // should be changed when TPC cluster resolution changes
  tpcclusterizer->setFitWindowMax(4/*rphibins*/,3/*zbins*/);
  tpcclusterizer->setFitEnergyThreshold( 0.05 /*fraction*/ );
  se->registerSubsystem( tpcclusterizer );

  /*
  //---------------
  // Load libraries
  //---------------

  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4hough.so");


  //---------------
  // Fun4All server
  //---------------

  Fun4AllServer *se = Fun4AllServer::instance();
*/

  const bool use_kalman_pat_rec = true;
  if (use_kalman_pat_rec) {
    //---------------------
    // PHG4KalmanPatRec
    //---------------------
    
    PHG4KalmanPatRec* kalman_pat_rec = new PHG4KalmanPatRec("PHG4KalmanPatRec");
    se->registerSubsystem(kalman_pat_rec);
    
  } else {
    //---------------------
    // Truth Pattern Recognition
    //---------------------
    PHG4TruthPatRec* pat_rec = new PHG4TruthPatRec();
    se->registerSubsystem(pat_rec);
    
  }
  
  //---------------------
  // Kalman Filter
  //---------------------
  
  PHG4TrackKalmanFitter* kalman = new PHG4TrackKalmanFitter();
  
  se->registerSubsystem(kalman);
  
    
  //------------------
  // Track Projections
  //------------------
  //  PHG4SvtxTrackProjection* projection = new PHG4SvtxTrackProjection();
  //  projection->Verbosity(verbosity);
  //  se->registerSubsystem( projection );

  /*  
  //----------------------
  // Beam Spot Calculation
  //----------------------
  PHG4SvtxBeamSpotReco* beamspot = new PHG4SvtxBeamSpotReco();
  beamspot->Verbosity(verbosity);
  se->registerSubsystem( beamspot );
  */

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
  eval->do_g4hit_eval(false);
  eval->do_hit_eval(false);
  eval->do_gpoint_eval(false);
  eval->scan_for_embedded(false); // take all tracks if false - take only embedded tracks if true (will not record decay particles!! - loses Upsilon electrons)
  eval->Verbosity(verbosity);
  se->registerSubsystem( eval );

  // MomentumEvaluator* eval = new MomentumEvaluator(outputfile.c_str(),0.2,0.4,Max_si_layer,2,Max_si_layer-4,10.,80.);
  // se->registerSubsystem( eval );
  
  return;
}
