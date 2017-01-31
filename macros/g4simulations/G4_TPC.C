//Three radial modules |-||-||-|
//Sixteen layers per module
//Choosing Readable Radius => 23.1 cm to 75cm => module size is 17.3
//OPTION 1:
//R1:  6 cards/R1  * 12 R1/end * 2 end/TPC = 144 cards/TPC
//R2:  8 cards/R2  * 12 R2/end * 2 end/TPC = 192 cards/TPC
//R3: 12 cards/R3  * 12 R3/end * 2 end/TPC = 288 cards/TPC
//TOTAL = 624 cards/TPC * 8 SAMPA/card = 4992 SAMPA/TPC * 32 channels/SAMPA = 159,744 channels/TPC

const double TPC_CAGE_LENGTH = 211.;
const double TPC_CAGE_IR = 20.;
const double TPC_CAGE_OR = 78.;
const int TPC_MOD_NLAY = 16;
const int TPC_MOD_CH[3] = {1152,1536,2304};
const int TPC_GAS_LAYERS = TPC_MOD_NLAY*5/2;
const int previous_active_layers = 3/*maps*/ + 4/*itt*/;
double TPC_RI[TPC_GAS_LAYERS];

double AddTPC2G4Geo(PHG4Reco* g4Reco, double radius, int verbosity) {
  cout << "TPC::AddTPC2G4Geo called" << endl;
  if(radius > TPC_CAGE_IR) {
    cout << "ConstructDetector_TPC: No space for TPC" << endl;
    exit(0);
  }

  int previous_support_layers = 3/*maps*/ + 4/*itt*/;

  PHG4CylinderSubsystem *cyl;

  radius = TPC_CAGE_IR;

  double n_rad_length_cage = 1.0e-02;
  double cage_thickness = 1.436 * n_rad_length_cage;
  cyl = new PHG4CylinderSubsystem("SVTXSUPPORT", previous_support_layers++ );
  cyl->Verbosity(verbosity);
  cyl->set_double_param("radius",radius);
  cyl->set_int_param("lengthviarapidity",0);
  cyl->set_double_param("length",TPC_CAGE_LENGTH);
  cyl->set_string_param("material","G4_Cu");
  cyl->set_double_param("thickness",cage_thickness );
  cyl->SuperDetector("SVTXSUPPORT");
  g4Reco->registerSubsystem( cyl );

  radius += cage_thickness;

  TString tpcgas = "G4_Ar";
  double st_mod = 23.1;
  const double TPC_MOD_DR = 17.3;
  const double TPC_MOD_WL = 0.3;
  const double lay_dr = (TPC_MOD_DR - TPC_MOD_WL*2)/double(TPC_MOD_NLAY);

  //=== GAS UNTIL FIRST READOUT
  double delta = TPC_MOD_DR/2 + st_mod - radius;
  cyl = new PHG4CylinderSubsystem("SVTXSUPPORT", previous_support_layers++);
  cyl->Verbosity(verbosity);
  cyl->set_double_param("radius",radius);
  cyl->set_int_param("lengthviarapidity",0);
  cyl->set_double_param("length",TPC_CAGE_LENGTH);
  cyl->set_string_param("material",tpcgas.Data());
  cyl->set_double_param("thickness", delta);
  cyl->SuperDetector("SVTXSUPPORT");
  g4Reco->registerSubsystem( cyl );

  radius += delta;

  //=== ACTIVE FIRST HALF MODULE
  delta = lay_dr;
  for(int ilayer=0; ilayer<TPC_MOD_NLAY/2; ++ilayer) {
    TPC_RI[ilayer] = radius;
    cyl = new PHG4CylinderSubsystem("SVTX", previous_active_layers+ilayer );
    cyl->Verbosity(verbosity);
    cyl->set_double_param("radius",radius);
    cyl->set_int_param("lengthviarapidity",0);
    cyl->set_double_param("length",TPC_CAGE_LENGTH);
    cyl->set_string_param("material",tpcgas.Data());
    cyl->set_double_param("thickness", delta-0.0038 );
    cyl->SetActive();
    cyl->SuperDetector("SVTX");
    g4Reco->registerSubsystem( cyl );
    radius += delta;
  }

  //=== GAS UNTIL SECOND READOUT
  double delta = 0.6;
  cyl = new PHG4CylinderSubsystem("SVTXSUPPORT", previous_support_layers++);
  cyl->Verbosity(verbosity);
  cyl->set_double_param("radius",radius);
  cyl->set_int_param("lengthviarapidity",0);
  cyl->set_double_param("length",TPC_CAGE_LENGTH);
  cyl->set_string_param("material",tpcgas.Data());
  cyl->set_double_param("thickness", delta);
  cyl->SuperDetector("SVTXSUPPORT");
  g4Reco->registerSubsystem( cyl );

  radius += delta;

  //=== ACTIVE SECOND MODULE
  delta = lay_dr;
  for(int ilayer=TPC_MOD_NLAY/2; ilayer<TPC_MOD_NLAY*3/2; ++ilayer) {
    TPC_RI[ilayer] = radius;
    cyl = new PHG4CylinderSubsystem("SVTX", previous_active_layers+ilayer );
    cyl->Verbosity(verbosity);
    cyl->set_double_param("radius",radius);
    cyl->set_int_param("lengthviarapidity",0);
    cyl->set_double_param("length",TPC_CAGE_LENGTH);
    cyl->set_string_param("material",tpcgas.Data());
    cyl->set_double_param("thickness", delta-0.0038 );
    cyl->SetActive();
    cyl->SuperDetector("SVTX");
    g4Reco->registerSubsystem( cyl );
    radius += delta;
  }

  //=== GAS UNTIL THIRD READOUT
  double delta = 0.6;
  cyl = new PHG4CylinderSubsystem("SVTXSUPPORT", previous_support_layers++);
  cyl->Verbosity(verbosity);
  cyl->set_double_param("radius",radius);
  cyl->set_int_param("lengthviarapidity",0);
  cyl->set_double_param("length",TPC_CAGE_LENGTH);
  cyl->set_string_param("material",tpcgas.Data());
  cyl->set_double_param("thickness", delta);
  cyl->SuperDetector("SVTXSUPPORT");
  g4Reco->registerSubsystem( cyl );

  radius += delta;

  //=== ACTIVE THIRD MODULE
  delta = lay_dr;
  for(int ilayer=TPC_MOD_NLAY*3/2; ilayer<TPC_MOD_NLAY*5/2; ++ilayer) {
    TPC_RI[ilayer] = radius;
    cyl = new PHG4CylinderSubsystem("SVTX", previous_active_layers+ilayer );
    cyl->Verbosity(verbosity);
    cyl->set_double_param("radius",radius);
    cyl->set_int_param("lengthviarapidity",0);
    cyl->set_double_param("length",TPC_CAGE_LENGTH);
    cyl->set_string_param("material",tpcgas.Data());
    cyl->set_double_param("thickness", delta-0.0038 );
    cyl->SetActive();
    cyl->SuperDetector("SVTX");
    g4Reco->registerSubsystem( cyl );
    radius += delta;
  }

  //=== GAS UNTIL OUTER CAGE
  delta = TPC_CAGE_OR-cage_thickness - radius;
  cyl = new PHG4CylinderSubsystem("SVTXSUPPORT", previous_support_layers++);
  cyl->Verbosity(verbosity);
  cyl->set_double_param("radius",radius);
  cyl->set_int_param("lengthviarapidity",0);
  cyl->set_double_param("length",TPC_CAGE_LENGTH);
  cyl->set_string_param("material",tpcgas.Data());
  cyl->set_double_param("thickness",delta);
  cyl->SuperDetector("SVTXSUPPORT");
  g4Reco->registerSubsystem( cyl );

  radius = TPC_CAGE_OR-cage_thickness;

  //=== OUTER CAGE
  cyl = new PHG4CylinderSubsystem("SVTXSUPPORT", previous_support_layers++);
  cyl->Verbosity(verbosity);
  cyl->set_double_param("radius",radius);
  cyl->set_int_param("lengthviarapidity",0);
  cyl->set_double_param("length",TPC_CAGE_LENGTH);
  cyl->set_string_param("material","G4_Cu");
  cyl->set_double_param("thickness",cage_thickness );
  cyl->SuperDetector("SVTXSUPPORT");
  g4Reco->registerSubsystem( cyl );

  radius = TPC_CAGE_OR;

  return radius;
}

void AddTPC2CellReco(PHG4CylinderCellTPCReco *svtx_cells, int verbosity=0) {
  cout << "TPC::AddTPC2CellReco called" << endl;
  const bool do_tpc_distoration = false;//true;
  const double diffusion = 0.0057;
  const double electrons_per_kev = 38.;

  PHG4TPCSpaceChargeDistortion* tpc_distortion = NULL;
  if(do_tpc_distoration) {
    if(TPC_CAGE_IR != 20. && TPC_CAGE_IR != 30.) {
      cout << "Svtx_Cells - Fatal Error - TPC distoration required that "
	"TPC_CAGE_IR is either 20 or 30 cm."
           << endl;
      exit(3);
    }
    string TPC_distroation_file = string(getenv("CALIBRATIONROOT")) + Form("/Tracking/TPC/SpaceChargeDistortion/sPHENIX%.0f.root",TPC_CAGE_IR);
    PHG4TPCSpaceChargeDistortion* tpc_distortion = new PHG4TPCSpaceChargeDistortion(TPC_distroation_file);
    //  tpc_distortion -> setAccuracy(0); // option to overwrite default
    //  tpc_distortion -> setPrecision(1); // option to overwrite default
  }
  svtx_cells->setDistortion(tpc_distortion); // apply TPC distrotion if tpc_distortion is not NULL
  svtx_cells->setDiffusion(diffusion);
  svtx_cells->setElectronsPerKeV(electrons_per_kev);

  if(verbosity>0) {
    for(int i=0; i!=TPC_GAS_LAYERS; ++i)
      cout << "Added TPC layer " << i << " starting at " << TPC_RI[i] << " cm." << endl;
  }

  const double tpc_dt = 0.17;
  //=== FIRST MODULE
  for(int i=0; i<TPC_MOD_NLAY/2; ++i) {
    double tpc_rdphi = TMath::TwoPi()*TPC_RI[i]/double(TPC_MOD_CH[0]);
    svtx_cells->cellsize(previous_active_layers+i, tpc_rdphi, tpc_dt);
    svtx_cells->set_timing_window(previous_active_layers+i, -14000.0, +14000.0);
  }
  //=== SECOND MODULE
  for(int i=TPC_MOD_NLAY/2; i<TPC_MOD_NLAY*3/2; ++i) {
    double tpc_rdphi = TMath::TwoPi()*TPC_RI[i]/double(TPC_MOD_CH[1]);
    svtx_cells->cellsize(previous_active_layers+i, tpc_rdphi, tpc_dt);
    svtx_cells->set_timing_window(previous_active_layers+i, -14000.0, +14000.0);
  }
  //=== THIRD MODULE
  for(int i=TPC_MOD_NLAY*3/2; i<TPC_MOD_NLAY*5/2; ++i) {
    double tpc_rdphi = TMath::TwoPi()*TPC_RI[i]/double(TPC_MOD_CH[2]);
    svtx_cells->cellsize(previous_active_layers+i, tpc_rdphi, tpc_dt);
    svtx_cells->set_timing_window(previous_active_layers+i, -14000.0, +14000.0);
  }
  return;
}

void AddTPC2Reco(PHG4SvtxDigitizer* digi, PHG4HoughTransformTPC* hough) {
  cout << "TPC::AddTPC2Reco called" << endl;
  Fun4AllServer *se = Fun4AllServer::instance();

  //clusterizer
  PHG4TPCClusterizer* tpcclusterizer = new PHG4TPCClusterizer("PHG4TPCClusterizer",3,4,previous_active_layers,previous_active_layers+TPC_GAS_LAYERS);
  tpcclusterizer->setEnergyCut(20.0*45.0/TPC_GAS_LAYERS);
  se->registerSubsystem( tpcclusterizer );
  //digitizer
  for(int i=previous_active_layers;i<previous_active_layers+TPC_GAS_LAYERS;++i) {
    digi->set_adc_scale(i, 10000, 1.0);
  }
  //hough
  hough->setUseCellSize(true);
  double mat_scale = 1.0;
  for(int i=previous_active_layers;i<previous_active_layers+TPC_GAS_LAYERS;++i) {
    hough->set_material(i, mat_scale*0.06/TPC_GAS_LAYERS);
    hough->setFitErrorScale(i, 1./sqrt(12.));
    hough->setVoteErrorScale(i, 1.0);
  }
  hough->set_material(previous_active_layers, mat_scale*0.010);   // TPC inner field cage wall, 1% of X_0
  return;
}
