#include <vector>

// if true, refit tracks with primary vertex included in track fit  - good for analysis of prompt tracks only
// Adds second node to node tree, keeps original track node undisturbed
// Adds second evaluator to process refitted tracks and outputs separate ntuples
bool use_primary_vertex = false;

const int n_maps_layer = 3;  // must be 0-3, setting it to zero removes MVTX completely, n < 3 gives the first n layers
const int n_intt_layer = 4;  // must be 0-4, setting this to zero will remove the INTT completely, n < 4 gives you the first n layers

int n_tpc_layer_inner = 8;
double tpc_layer_thick_inner = 1.25;
int tpc_layer_rphi_count_inner = 1920;

// make inner layers a factor of 2 thinner
/*
int n_tpc_layer_inner = 8 * 2;
double tpc_layer_thick_inner = 1.25 / 2.0;
int tpc_layer_rphi_count_inner = 1920 / 2;
*/

int n_tpc_layer_mid = 16;
double tpc_layer_thick_mid = 1.25;
int tpc_layer_rphi_count_mid = 1536;

int n_tpc_layer_outer = 16;
double tpc_layer_thick_outer = 1.125; // outer later reach from 60-78 cm (instead of 80 cm), that leads to radial thickness of 1.125 cm
int tpc_layer_rphi_count_outer = 2304;

int n_gas_layer = n_tpc_layer_inner + n_tpc_layer_mid + n_tpc_layer_outer;

double inner_cage_radius = 20.;
double inner_readout_radius = 30.;

// TPC gas parameters
// These are set for a variety of gas choices...
//==============================================
enum TPC_Gas
{
  Ne2K_100,
  Ne2K_400,
  NeCF4_100,
  NeCF4_300,
  NeCF4_400,
  ByHand
};
TPC_Gas ether = TPC_Gas::NeCF4_400;
//TPC_Gas ether = TPC_Gas::ByHand;

// Data on gasses @20 C and 760 Torr from the following source:
// http://www.slac.stanford.edu/pubs/icfa/summer98/paper3/paper3.pdf
double Ne_dEdx = 1.56;    // keV/cm
double CF4_dEdx = 7.00;   // keV/cm
double iBut_dEdx = 5.93;  // keV/cm

double Ne_NPrimary = 12;    // Number/cm
double CF4_NPrimary = 51;   // Number/cm
double iBut_NPrimary = 84;  // Number/cm

double Ne_NTotal = 43;     // Number/cm
double CF4_NTotal = 100;   // Number/cm
double iBut_NTotal = 195;  // Number/cm

// TPC Performance Parameter (applies extra smear to mimic the avalanche):
double TPC_SigmaT = 0.03;  // 0.03 means 300 microns...Prakhar Garg Simulation...desire measurement...

// to be overwritten...
double TPCDriftVelocity;
double TPC_Trans_Diffusion;
double TPC_Long_Diffusion;
double TPC_dEdx;
double TPC_NPri;
double TPC_NTot;
double TPC_ElectronsPerKeV;

// TPC readout shaping time and ADC clock parameters
// these set the Z size of the TPC cells
// These need to be set in the init since some of them require the drift velocity...
//=======================================
double TPCADCClock;
double TPCShapingRMSLead;
double TPCShapingRMSTail;
double tpc_cell_z;
double TPC_SmearRPhi;
double TPC_SmearZ;

int Max_si_layer;

void SvtxInit(int n_TPC_layers = 40, int verbosity = 0)
{
  Max_si_layer = n_maps_layer + n_intt_layer + n_gas_layer;

  switch (ether)
  {
  // https://www.phenix.bnl.gov/WWW/p/draft/prakhar/tpc/HTML_Gas_Linear/Ne_CF4_IC4H10_95_3_2.html
  case TPC_Gas::Ne2K_100:
  {
    if (verbosity)
      cout << "Gas Choice:  TPC_Gas::Ne2K_100" << endl;
    TPCDriftVelocity = 3.2 / 1000.0;  // cm/ns
    TPC_Trans_Diffusion = 0.0065;     // cm/SQRT(cm)
    TPC_Long_Diffusion = 0.0300;      // cm/SQRT(cm)
    TPC_dEdx = 0.95 * Ne_dEdx + 0.03 * CF4_dEdx + 0.02 * iBut_dEdx;
    TPC_NPri = 0.95 * Ne_NPrimary + 0.03 * CF4_NPrimary + 0.02 * iBut_NPrimary;
    TPC_NTot = 0.95 * Ne_NTotal + 0.03 * CF4_NTotal + 0.02 * iBut_NTotal;
    break;
  }
  case TPC_Gas::Ne2K_400:
  {
    if (verbosity)
      cout << "Gas Choice:  TPC_Gas::Ne2K_400" << endl;
    TPCDriftVelocity = 5.5 / 1000.0;  // cm/ns
    TPC_Trans_Diffusion = 0.0120;     // cm/SQRT(cm)
    TPC_Long_Diffusion = 0.0175;      // cm/SQRT(cm)
    TPC_dEdx = 0.95 * Ne_dEdx + 0.03 * CF4_dEdx + 0.02 * iBut_dEdx;
    TPC_NPri = 0.95 * Ne_NPrimary + 0.03 * CF4_NPrimary + 0.02 * iBut_NPrimary;
    TPC_NTot = 0.95 * Ne_NTotal + 0.03 * CF4_NTotal + 0.02 * iBut_NTotal;
    break;
  }
  // https://www.phenix.bnl.gov/WWW/p/draft/prakhar/tpc/HTML_Gas_Linear/Ne_CF4_90_10.html
  case TPC_Gas::NeCF4_100:
  {
    if (verbosity)
      cout << "Gas Choice:  TPC_Gas::NeCF4_100" << endl;
    TPCDriftVelocity = 4.0 / 1000.0;  // cm/ns
    TPC_Trans_Diffusion = 0.0045;     // cm/SQRT(cm)
    TPC_Long_Diffusion = 0.0270;      // cm/SQRT(cm)
    TPC_dEdx = 0.90 * Ne_dEdx + 0.10 * CF4_dEdx;
    TPC_NPri = 0.90 * Ne_NPrimary + 0.10 * CF4_NPrimary;
    TPC_NTot = 0.90 * Ne_NTotal + 0.10 * CF4_NTotal;
    break;
  }
  case TPC_Gas::NeCF4_300:
  {
    if (verbosity)
      cout << "Gas Choice:  TPC_Gas::NeCF4_300" << endl;
    TPCDriftVelocity = 7.0 / 1000.0;  // cm/ns
    TPC_Trans_Diffusion = 0.0052;     // cm/SQRT(cm)
    TPC_Long_Diffusion = 0.0170;      // cm/SQRT(cm)
    TPC_dEdx = 0.90 * Ne_dEdx + 0.10 * CF4_dEdx;
    TPC_NPri = 0.90 * Ne_NPrimary + 0.10 * CF4_NPrimary;
    TPC_NTot = 0.90 * Ne_NTotal + 0.10 * CF4_NTotal;
    break;
  }
  case TPC_Gas::NeCF4_400:
  {
    if (verbosity)
      cout << "Gas Choice:  TPC_Gas::NeCF4_400" << endl;
    TPCDriftVelocity = 8.0 / 1000.0;  // cm/ns
    TPC_Trans_Diffusion = 0.0060;     // cm/SQRT(cm)
    TPC_Long_Diffusion = 0.0150;      // cm/SQRT(cm)
    TPC_dEdx = 0.90 * Ne_dEdx + 0.10 * CF4_dEdx;
    TPC_NPri = 0.90 * Ne_NPrimary + 0.10 * CF4_NPrimary;
    TPC_NTot = 0.90 * Ne_NTotal + 0.10 * CF4_NTotal;
    break;
  }
  case TPC_Gas::ByHand:
  {
    if (verbosity)
      cout << "Gas Choice:  TPC_Gas::ByHand" << endl;
    TPCDriftVelocity = 6.0 / 1000.0;  // cm/ns
    TPC_Trans_Diffusion = 0.0130;     // cm/SQRT(cm)
    TPC_Long_Diffusion = 0.0130;      // cm/SQRT(cm)
    TPC_ElectronsPerKeV = 28.0;
    TPC_dEdx = 0.90 * Ne_dEdx + 0.10 * CF4_dEdx;
    TPC_NPri = 0.90 * Ne_NPrimary + 0.10 * CF4_NPrimary;
    TPC_NTot = TPC_ElectronsPerKeV * TPC_dEdx;
    break;
  }
  default:  // defaults to NeCF4_400
  {
    if (verbosity)
      cout << "Gas Choice Undefined...using TPC_Gas::NeCF4_400" << endl;
    TPCDriftVelocity = 8.0 / 1000.0;  // cm/ns
    TPC_Trans_Diffusion = 0.0060;     // cm/SQRT(cm)
    TPC_Long_Diffusion = 0.0150;      // cm/SQRT(cm)
    TPC_dEdx = 0.90 * Ne_dEdx + 0.10 * CF4_dEdx;
    TPC_NPri = 0.90 * Ne_NPrimary + 0.10 * CF4_NPrimary;
    TPC_NTot = 0.90 * Ne_NTotal + 0.10 * CF4_NTotal;
    break;
  }
  }

  TPC_ElectronsPerKeV = TPC_NTot / TPC_dEdx;

  // TPC readout shaping time and ADC clock parameters
  // these set the Z size of the TPC cells
  //=======================================
  // TPCShapingRMSLead = 32.0;  // ns, rising RMS equivalent of shaping amplifier for 80 ns SAMPA
  // TPCShapingRMSTail = 48.0;  // ns, falling RMS equivalent of shaping amplifier for 80 ns SAMPA
  TPCADCClock = 53.0;                           // ns, corresponds to an ADC clock rate of 18.8 MHz
  TPCShapingRMSLead = 16.0;                     // ns, rising RMS equivalent of shaping amplifier for 40 ns SAMPA
  TPCShapingRMSTail = 24.0;                     // ns, falling RMS equivalent of shaping amplifier for 40 ns SAMPA
                                                // TPCADCClock = 27.0;  // ns, corresponds to an ADC clock rate of 18.8 MHz * 2
  tpc_cell_z = TPCADCClock * TPCDriftVelocity;  // cm

  //  TKH does not understand the physical origin of these parameters.
  //  however, their impact seems quite small...
  TPC_SmearRPhi = 0.10;
  TPC_SmearZ = 0.15;
}

double Svtx(PHG4Reco* g4Reco, double radius,
            const int absorberactive = 0,
            int verbosity = 0)
{
  if (n_maps_layer > 0)
  {
    bool maps_overlapcheck = false;  // set to true if you want to check for overlaps

    /*
	The numbers used in the macro below are from the xml file dump of ITS.gdml
	As a sanity check, I got numbers  from Walt Sondheim's drawings, sent by email June 20, 2017:
	OD of Be beam pipe is 41.53 mm, ID is 40 mm
	Layer 0: radius 23.44 mm to sensor center, tilt from normal to radial vector:  17.37 degrees (0.303 rad), spacing btw sensor centers: 30 deg, arc spacing 12.27 mm
	Layer 1: radius 31.54 mm to sensor center, ttilt from normal to radial vector:  17.53 degrees (0.306 rad), spacing btw sensor centers: 22.5 deg, arc spacing 12.38 mm
	Layer 2: radius 39.29 to sensor center, tilt from normal to radial vector: 17.02 degrees (0.297 rad), spacing btw sensor centers: 18.0 deg, arc spacing 12.34 mm
	These are in reasonable agreement with the numbers I extracted from the gdml file, which are what we use below.
	These use a spacing in arc length of 12.37 mm and a tilt of 0.304 for all of the first three layers
      */

    // MAPS inner barrel layers
    //======================================================

    double maps_layer_radius[3] = {23.4, 31.5, 39.3};  // mm  - precise numbers from ITS.gdml
    //double maps_layer_radius[3] = {24.9, 33.0, 40.8};   // mm  - precise numbers from ITS.gdml + 1.5 mm for greater clearance from beam pipe

    // type 1 = inner barrel stave, 2 = middle barrel stave, 3 = outer barrel stave
    // we use only type 0 here
    int stave_type[3] = {0, 0, 0};
    int staves_in_layer[3] = {12, 16, 20};       // Number of staves per layer in sPHENIX MVTX
    double phi_tilt[3] = {0.304, 0.304, 0.304};  // radians, from the gdml file, 0.304 radians is 17.4 degrees

    for (int ilayer = 0; ilayer < n_maps_layer; ilayer++)
    {
      if (verbosity)
        cout << "Create Maps layer " << ilayer << " with radius " << maps_layer_radius[ilayer] << " mm, stave type " << stave_type[ilayer]
             << " pixel size 30 x 30 microns "
             << " active pixel thickness 0.0018 microns" << endl;

      PHG4MapsSubsystem* lyr = new PHG4MapsSubsystem("MAPS", ilayer, stave_type[ilayer]);
      lyr->Verbosity(verbosity);

      lyr->set_double_param("layer_nominal_radius", maps_layer_radius[ilayer]);  // thickness in cm
      lyr->set_int_param("N_staves", staves_in_layer[ilayer]);                   // uses fixed number of staves regardless of radius, if set. Otherwise, calculates optimum number of staves

      // The cell size is used only during pixilization of sensor hits, but it is convemient to set it now because the geometry object needs it
      lyr->set_double_param("pixel_x", 0.0030);          // pitch in cm
      lyr->set_double_param("pixel_z", 0.0030);          // length in cm
      lyr->set_double_param("pixel_thickness", 0.0018);  // thickness in cm
      lyr->set_double_param("phitilt", phi_tilt[ilayer]);

      lyr->set_int_param("active", 1);
      lyr->OverlapCheck(maps_overlapcheck);

      lyr->set_string_param("stave_geometry_file",
                            string(getenv("CALIBRATIONROOT")) + string("/Tracking/geometry/ALICE_ITS_tgeo.gdml"));

      g4Reco->registerSubsystem(lyr);

      radius = maps_layer_radius[ilayer];
    }
  }

  if (n_intt_layer > 0)
  {
    //-------------------
    // INTT ladders
    //-------------------

    bool intt_overlapcheck = false;  // set to true if you want to check for overlaps

    // instantiate the Silicon tracker subsystem and register it
    // We make one instance of PHG4TrackerSubsystem for all four layers of tracker
    // dimensions are in mm, angles are in radians

    // PHG4SiliconTrackerSubsystem creates the detetor layer using PHG4SiliconTrackerDetector
    // and instantiates the appropriate PHG4SteppingAction
    const double intt_radius_max = 140.;  // including stagger radius (mm)

    // The length of vpair is used to determine the number of layers
    std::vector<std::pair<int, int>> vpair;  // (sphxlayer, inttlayer)
    for (int i = 0; i < n_intt_layer; i++)
    {
      // We want the sPHENIX layer numbers for the INTT to be from n_maps_layer to n_maps_layer+n_intt_layer - 1
      vpair.push_back(std::make_pair(n_maps_layer + i, i));  // sphxlayer=n_maps_layer+i corresponding to inttlayer=i
      if (verbosity) cout << "Create strip tracker layer " << vpair[i].second << " as  sphenix layer  " << vpair[i].first << endl;
    }
    PHG4SiliconTrackerSubsystem* sitrack = new PHG4SiliconTrackerSubsystem("SILICON_TRACKER", vpair);
    sitrack->Verbosity(verbosity);
    sitrack->SetActive(1);
    sitrack->OverlapCheck(intt_overlapcheck);
    g4Reco->registerSubsystem(sitrack);

    // outer radius marker (translation back to cm)
    radius = intt_radius_max * 0.1;
  }

  // time projection chamber layers --------------------------------------------

  PHG4CylinderSubsystem* cyl;

  radius = inner_cage_radius;

  double cage_length = 211.0;  // From TPC group, gives eta = 1.1 at 78 cm
  double n_rad_length_cage = 1.13e-02;
  double cage_thickness = 28.6 * n_rad_length_cage;  // Kapton X_0 = 28.6 cm  // mocks up Kapton + carbon fiber structure

  // inner field cage
  cyl = new PHG4CylinderSubsystem("SVTXSUPPORT", n_maps_layer + n_intt_layer);
  cyl->set_double_param("radius", radius);
  cyl->set_int_param("lengthviarapidity", 0);
  cyl->set_double_param("length", cage_length);
  cyl->set_string_param("material", "G4_KAPTON");
  cyl->set_double_param("thickness", cage_thickness);
  cyl->SuperDetector("SVTXSUPPORT");
  cyl->Verbosity(0);
  g4Reco->registerSubsystem(cyl);

  radius += cage_thickness;

  double inner_readout_radius = 30.;
  if (inner_readout_radius < radius) inner_readout_radius = radius;

  string tpcgas = "sPHENIX_TPC_Gas";  //  Ne(90%) CF4(10%) - defined in g4main/PHG4Reco.cc

  // Layer of inert TPC gas from 20-30 cm
  if (inner_readout_radius - radius > 0)
  {
    cyl = new PHG4CylinderSubsystem("SVTXSUPPORT", n_maps_layer + n_intt_layer + 1);
    cyl->set_double_param("radius", radius);
    cyl->set_int_param("lengthviarapidity", 0);
    cyl->set_double_param("length", cage_length);
    cyl->set_string_param("material", tpcgas.c_str());
    cyl->set_double_param("thickness", inner_readout_radius - radius);
    cyl->SuperDetector("SVTXSUPPORT");
    g4Reco->registerSubsystem(cyl);
  }

  radius = inner_readout_radius;

  double outer_radius = 78.;
  //int npoints = Max_si_layer - n_maps_layer - n_intt_layer;
  //double delta_radius =  ( outer_radius - inner_readout_radius )/( (double)npoints );

  // Active layers of the TPC from 30-40 cm (inner layers)

  for (int ilayer = n_maps_layer + n_intt_layer; ilayer < (n_maps_layer + n_intt_layer + n_tpc_layer_inner); ++ilayer)
  {
    if (verbosity)
      cout << "Create TPC gas layer " << ilayer << " with inner radius " << radius << " cm "
           << " thickness " << tpc_layer_thick_inner - 0.01 << " length " << cage_length << endl;

    cyl = new PHG4CylinderSubsystem("SVTX", ilayer);
    cyl->set_double_param("radius", radius);
    cyl->set_int_param("lengthviarapidity", 0);
    cyl->set_double_param("length", cage_length);
    cyl->set_string_param("material", tpcgas.c_str());
    cyl->set_double_param("thickness", tpc_layer_thick_inner - 0.01);
    cyl->SetActive();
    cyl->SuperDetector("SVTX");
    g4Reco->registerSubsystem(cyl);

    radius += tpc_layer_thick_inner;
  }

  // Active layers of the TPC from 40-60 cm (mid layers)

  for (int ilayer = n_maps_layer + n_intt_layer + n_tpc_layer_inner; ilayer < (n_maps_layer + n_intt_layer + n_tpc_layer_inner + n_tpc_layer_mid); ++ilayer)
  {
    if (verbosity)
      cout << "Create TPC gas layer " << ilayer << " with inner radius " << radius << " cm "
           << " thickness " << tpc_layer_thick_mid - 0.01 << " length " << cage_length << endl;

    cyl = new PHG4CylinderSubsystem("SVTX", ilayer);
    cyl->set_double_param("radius", radius);
    cyl->set_int_param("lengthviarapidity", 0);
    cyl->set_double_param("length", cage_length);
    cyl->set_string_param("material", tpcgas.c_str());
    cyl->set_double_param("thickness", tpc_layer_thick_mid - 0.01);
    cyl->SetActive();
    cyl->SuperDetector("SVTX");
    g4Reco->registerSubsystem(cyl);

    radius += tpc_layer_thick_mid;
  }

  // Active layers of the TPC from 60-80 cm (outer layers)

  for (int ilayer = n_maps_layer + n_intt_layer + n_tpc_layer_inner + n_tpc_layer_mid; ilayer < (n_maps_layer + n_intt_layer + n_tpc_layer_inner + n_tpc_layer_mid + n_tpc_layer_outer); ++ilayer)
  {
    if (verbosity)
      cout << "Create TPC gas layer " << ilayer << " with inner radius " << radius << " cm "
           << " thickness " << tpc_layer_thick_outer - 0.01 << " length " << cage_length << endl;

    cyl = new PHG4CylinderSubsystem("SVTX", ilayer);
    cyl->set_double_param("radius", radius);
    cyl->set_int_param("lengthviarapidity", 0);
    cyl->set_double_param("length", cage_length);
    cyl->set_string_param("material", tpcgas.c_str());
    cyl->set_double_param("thickness", tpc_layer_thick_outer - 0.01);
    cyl->SetActive();
    cyl->SuperDetector("SVTX");
    g4Reco->registerSubsystem(cyl);

    radius += tpc_layer_thick_outer;
  }

  // outer field cage
  cyl = new PHG4CylinderSubsystem("SVTXSUPPORT", n_maps_layer + n_intt_layer + n_gas_layer);
  cyl->set_double_param("radius", radius);
  cyl->set_int_param("lengthviarapidity", 0);
  cyl->set_double_param("length", cage_length);
  cyl->set_string_param("material", "G4_KAPTON");
  cyl->set_double_param("thickness", cage_thickness);  // Kapton X_0 = 28.6 cm
  cyl->SuperDetector("SVTXSUPPORT");
  g4Reco->registerSubsystem(cyl);

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

  Fun4AllServer* se = Fun4AllServer::instance();

  //-----------
  // SVTX cells
  //-----------

  if (verbosity)
  {
    cout << "  TPC Drift Velocity: " << TPCDriftVelocity << " cm/nsec" << endl;
    cout << "  TPC Transverse Diffusion: " << TPC_Trans_Diffusion << " cm/SQRT(cm)" << endl;
    cout << "  TPC Longitudinal Diffusion: " << TPC_Long_Diffusion << " cm/SQRT(cm)" << endl;
    cout << "  TPC dE/dx: " << TPC_dEdx << " keV/cm" << endl;
    cout << "  TPC N Primary: " << TPC_NPri << " electrons/cm" << endl;
    cout << "  TPC N Total: " << TPC_NTot << " electrons/cm" << endl;
    cout << "  TPC Electrons Per keV: " << TPC_ElectronsPerKeV << " electrons/keV" << endl;
    cout << "  TPC ADC Clock: " << TPCADCClock << " nsec" << endl;
    cout << "  TPC ADC Rate: " << 1000.0 / TPCADCClock << " MHZ" << endl;
    cout << "  TPC Shaping Lead: " << TPCShapingRMSLead << " nsec" << endl;
    cout << "  TPC Shaping Tail: " << TPCShapingRMSTail << " nsec" << endl;
    cout << "  TPC z cell " << tpc_cell_z << " cm" << endl;
    cout << "  TPC Smear R-Phi " << TPC_SmearRPhi << " cm" << endl;
    cout << "  TPC Smear Z " << TPC_SmearZ << " cm" << endl;
  }

  if (n_maps_layer > 0)
  {
    // MAPS cells
    PHG4MapsCellReco* maps_cells = new PHG4MapsCellReco("MAPS");
    maps_cells->Verbosity(verbosity);
    for (int ilayer = 0; ilayer < n_maps_layer; ilayer++)
    {
      maps_cells->set_timing_window(ilayer, -2000, 2000);
    }
    se->registerSubsystem(maps_cells);
  }

  if (n_intt_layer > 0)
  {
    // INTT cells
    PHG4SiliconTrackerCellReco* reco = new PHG4SiliconTrackerCellReco("SILICON_TRACKER");
    // The timing windows are hard-coded in the INTT ladder model
    reco->Verbosity(verbosity);
    se->registerSubsystem(reco);
  }

  // TPC cell sizes are defined at top of macro, this is for backward compatibility with old hits files
  if (n_gas_layer == 60)
  {
    TPCDriftVelocity = 6.0 / 1000.0;  // cm/ns
    tpc_cell_x = 0.12;
    tpc_cell_y = 0.17;
  }

  // Main switch for TPC distortion
  const bool do_tpc_distortion = true;
  PHG4TPCSpaceChargeDistortion* tpc_distortion = NULL;
  if (do_tpc_distortion)
  {
    if (inner_cage_radius != 20. && inner_cage_radius != 30.)
    {
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

  PHG4CylinderCellTPCReco* svtx_cells = new PHG4CylinderCellTPCReco(n_maps_layer + n_intt_layer);
  svtx_cells->Detector("SVTX");
  svtx_cells->setDistortion(tpc_distortion);
  if (n_gas_layer != 60)
  {
    svtx_cells->setDiffusionT(TPC_Trans_Diffusion);
    svtx_cells->setDiffusionL(TPC_Long_Diffusion);
    svtx_cells->setSigmaT(TPC_SigmaT);

    svtx_cells->setShapingRMSLead(TPCShapingRMSLead * TPCDriftVelocity);
    svtx_cells->setShapingRMSTail(TPCShapingRMSTail * TPCDriftVelocity);
    // Expected cluster resolutions:
    //    r-phi: diffusion + GEM smearing = 750 microns, assume resolution is 20% of that => 150 microns
    //    Z:  amplifier shaping time (RMS 32 ns, 48 ns) and drift vel of 3 cm/microsec gives smearing of 3 x (32+48/2 = 1.2 mm, assume resolution is 20% of that => 240 microns
    svtx_cells->setSmearRPhi(TPC_SmearRPhi);  // additional random displacement of cloud positions wrt hits to give expected cluster resolution of 150 microns for charge at membrane
    svtx_cells->setSmearZ(TPC_SmearZ);        // additional random displacement of cloud positions wrt hits to give expected cluster rsolution of 240 microns for charge at membrane
  }
  else
  {
    // 60 layer tune
    svtx_cells->setDiffusionT(0.0120);
    svtx_cells->setDiffusionL(0.0120);
    svtx_cells->setSmearRPhi(0.09);  // additional smearing of cluster positions
    svtx_cells->setSmearZ(0.06);     // additional smearing of cluster positions
  }
  svtx_cells->set_drift_velocity(TPCDriftVelocity);
  svtx_cells->setHalfLength(105.5);
  svtx_cells->setElectronsPerKeV(TPC_ElectronsPerKeV);
  svtx_cells->Verbosity(0);

  // The maps cell size is set when the detector is constructed because it is needed by the geometry object
  // The INTT ladder cell size is set in the detector construction code

  // set cylinder cell TPC cell sizes
  //======================

  double tpc_timing_window = 105.5 / TPCDriftVelocity;  // half length in cm / Vd in cm/ns => ns

  // inner layers
  double radius_layer = inner_readout_radius + tpc_layer_thick_inner / 2.0;
  int counter_layer = 0;
  for (int i = n_maps_layer + n_intt_layer; i < n_maps_layer + n_intt_layer + n_tpc_layer_inner; i++)
  {
    // this calculates the radius at the middle of the layer
    if (counter_layer > 0)
      radius_layer += tpc_layer_thick_inner;
    double tpc_cell_rphi = 2 * TMath::Pi() * radius_layer / (double) tpc_layer_rphi_count_inner;
    svtx_cells->cellsize(i, tpc_cell_rphi, tpc_cell_z);
    svtx_cells->set_timing_window(i, -tpc_timing_window, +tpc_timing_window);
    if (verbosity)
      cout << "TPC cells inner: layer " << i << " center radius " << radius_layer << " tpc_cell_rphi " << tpc_cell_rphi << " tpc_cell_z " << tpc_cell_z << endl;
    counter_layer++;
  }
  radius_layer += tpc_layer_thick_inner / 2.0;

  // mid layers
  radius_layer += tpc_layer_thick_mid / 2.0;
  counter_layer = 0;
  for (int i = n_maps_layer + n_intt_layer + n_tpc_layer_inner; i < n_maps_layer + n_intt_layer + n_tpc_layer_inner + n_tpc_layer_mid; i++)
  {
    if (counter_layer > 0)
      radius_layer += tpc_layer_thick_mid;
    double tpc_cell_rphi = 2 * TMath::Pi() * radius_layer / (double) tpc_layer_rphi_count_mid;
    svtx_cells->cellsize(i, tpc_cell_rphi, tpc_cell_z);
    svtx_cells->set_timing_window(i, -tpc_timing_window, +tpc_timing_window);
    if (verbosity)
      cout << "TPC cells mid: layer " << i << " center radius " << radius_layer << " tpc_cell_rphi " << tpc_cell_rphi << " tpc_cell_z " << tpc_cell_z << endl;
    counter_layer++;
  }
  radius_layer += tpc_layer_thick_mid / 2.0;

  // outer layers
  radius_layer += tpc_layer_thick_outer / 2.0;
  counter_layer = 0;
  for (int i = n_maps_layer + n_intt_layer + n_tpc_layer_inner + n_tpc_layer_mid; i < n_maps_layer + n_intt_layer + n_tpc_layer_inner + n_tpc_layer_mid + n_tpc_layer_outer; i++)
  {
    if (counter_layer > 0)
      radius_layer += tpc_layer_thick_outer;
    double tpc_cell_rphi = 2 * TMath::Pi() * radius_layer / (double) tpc_layer_rphi_count_outer;
    svtx_cells->cellsize(i, tpc_cell_rphi, tpc_cell_z);
    svtx_cells->set_timing_window(i, -tpc_timing_window, +tpc_timing_window);
    if (verbosity)
      cout << "TPC cells outer: layer " << i << " center radius " << radius_layer << " tpc_cell_rphi " << tpc_cell_rphi << " tpc_cell_z " << tpc_cell_z << endl;
    counter_layer++;
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

  Fun4AllServer* se = Fun4AllServer::instance();

  //----------------------------------
  // Digitize the cell energy into ADC
  //----------------------------------
  PHG4SvtxDigitizer* digi = new PHG4SvtxDigitizer();
  digi->Verbosity(0);
  for (int i = 0; i < n_maps_layer; ++i)
  {
    digi->set_adc_scale(i, 255, 0.4e-6);  // reduced by a factor of 2.5 when going from maps thickess of 50 microns to 18 microns
  }

  if (n_intt_layer > 0)
  {
    // INTT
    std::vector<double> userrange;  // 3-bit ADC threshold relative to the mip_e at each layer.
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
    for (int i = 0; i < n_intt_layer; i++)
    {
      digiintt->set_adc_scale(n_maps_layer + i, userrange);
    }
    se->registerSubsystem(digiintt);
  }

  // TPC layers
  for (int i = n_maps_layer + n_intt_layer; i < Max_si_layer; ++i)
  {
    digi->set_adc_scale(i, 30000, 1.0);
  }
  se->registerSubsystem(digi);

  //-------------------------------------
  // Apply Live Area Inefficiency to Hits
  //-------------------------------------
  // defaults to 1.0 (fully active)

  PHG4SvtxDeadArea* deadarea = new PHG4SvtxDeadArea();

  for (int i = 0; i < n_maps_layer; i++)
  {
    deadarea->Verbosity(verbosity);
    //deadarea->set_hit_efficiency(i,0.99);
    deadarea->set_hit_efficiency(i, 1.0);
  }
  for (int i = n_maps_layer; i < n_maps_layer + n_intt_layer; i++)
  {
    //deadarea->set_hit_efficiency(i,0.99);
    deadarea->set_hit_efficiency(i, 1.0);
  }
  se->registerSubsystem(deadarea);

  //-----------------------------
  // Apply MIP thresholds to Hits
  //-----------------------------

  PHG4SvtxThresholds* thresholds = new PHG4SvtxThresholds();
  thresholds->Verbosity(verbosity);

  // maps
  for (int i = 0; i < n_maps_layer; i++)
  {
    // reduced by x2.5 when going from cylinder maps with 50 microns thickness to actual maps with 18 microns thickness
    // Note the non-use of set_using_thickness here, this is so that the shortest dimension of the cell sets the mip energy loss
    thresholds->set_threshold(i, 0.1);
  }
  // INTT
  for (int i = n_maps_layer; i < n_maps_layer + n_intt_layer; i++)
  {
    thresholds->set_threshold(i, 0.1);
    thresholds->set_use_thickness_mip(i, true);
  }

  se->registerSubsystem(thresholds);

  //-------------
  // Cluster Hits
  //-------------

  PHG4SvtxClusterizer* clusterizer = new PHG4SvtxClusterizer("PHG4SvtxClusterizer", 0, n_maps_layer + n_intt_layer - 1);
  clusterizer->Verbosity(verbosity);
  // Reduced by 2 relative to the cylinder cell maps macro. I found this necessary to get full efficiency
  // Many hits in the present simulation are single cell hits, so it is not clear why the cluster threshold should be higher than the cell threshold
  clusterizer->set_threshold(0.1);  // fraction of a mip
  // no Z clustering for INTT layers (only)
  for (int i = n_maps_layer; i < n_maps_layer + n_intt_layer; i++)
  {
    clusterizer->set_z_clustering(i, false);
  }

  se->registerSubsystem(clusterizer);

  PHG4TPCClusterizer* tpcclusterizer = new PHG4TPCClusterizer();
  tpcclusterizer->Verbosity(0);
  tpcclusterizer->setRangeLayers(n_maps_layer + n_intt_layer, Max_si_layer);
  if (n_gas_layer == 40)
  {
    tpcclusterizer->setEnergyCut(12 /*15 adc*/);
    tpcclusterizer->setFitWindowSigmas(0.0160, 0.0160);  // should be changed when TPC cluster resolution changes
    tpcclusterizer->setFitWindowMax(4 /*rphibins*/, 6 /*zbins*/);
    tpcclusterizer->setFitEnergyThreshold(0.05 /*fraction*/);
  }
  else
  {
    // 60 layer tune
    tpcclusterizer->setEnergyCut(15 /*adc*/);
    tpcclusterizer->setFitWindowSigmas(0.0150, 0.0160);  // should be changed when TPC cluster resolution changes
    tpcclusterizer->setFitWindowMax(4 /*rphibins*/, 3 /*zbins*/);
    tpcclusterizer->setFitEnergyThreshold(0.05 /*fraction*/);
  }
  se->registerSubsystem(tpcclusterizer);

  // This should be true for everything except testing!
  const bool use_kalman_pat_rec = true;
  if (use_kalman_pat_rec)
  {
    //---------------------
    // PHG4KalmanPatRec
    //---------------------

    PHG4KalmanPatRec* kalman_pat_rec = new PHG4KalmanPatRec("PHG4KalmanPatRec", n_maps_layer, n_intt_layer, n_gas_layer);
    kalman_pat_rec->Verbosity(0);
    se->registerSubsystem(kalman_pat_rec);
  }
  else
  {
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
  kalman->Verbosity(0);
  if (use_primary_vertex)
    kalman->set_fit_primary_tracks(true);  // include primary vertex in track fit if true
  se->registerSubsystem(kalman);

  //------------------
  // Track Projections
  //------------------
  PHG4GenFitTrackProjection* projection = new PHG4GenFitTrackProjection();
  projection->Verbosity(verbosity);
  se->registerSubsystem(projection);

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

  Fun4AllServer* se = Fun4AllServer::instance();

  //----------------
  // SVTX evaluation
  //----------------

  SvtxEvaluator* eval;
  eval = new SvtxEvaluator("SVTXEVALUATOR", outputfile.c_str());
  eval->do_cluster_eval(true);
  eval->do_g4hit_eval(true);
  eval->do_hit_eval(false);  // enable to see the hits that includes the chamber physics...
  eval->do_gpoint_eval(false);
  eval->scan_for_embedded(false);  // take all tracks if false - take only embedded tracks if true
  eval->Verbosity(verbosity);
  se->registerSubsystem(eval);

  if (use_primary_vertex)
  {
    // make a second evaluator that records tracks fitted with primary vertex included
    // good for analysis of prompt tracks, particularly if MVTX is not present
    SvtxEvaluator* evalp;
    evalp = new SvtxEvaluator("SVTXEVALUATOR", string(outputfile.c_str()) + "_primary_eval.root", "PrimaryTrackMap");
    evalp->do_cluster_eval(true);
    evalp->do_g4hit_eval(true);
    evalp->do_hit_eval(false);
    evalp->do_gpoint_eval(false);
    evalp->scan_for_embedded(true);  // take all tracks if false - take only embedded tracks if true
    evalp->Verbosity(0);
    se->registerSubsystem(evalp);
  }

  // MomentumEvaluator* eval = new MomentumEvaluator(outputfile.c_str(),0.2,0.4,Max_si_layer,2,Max_si_layer-4,10.,80.);
  // se->registerSubsystem( eval );

  return;
}
