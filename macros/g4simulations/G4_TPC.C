
void TPCInit() {}

double TPC(PHG4Reco* g4Reco,
	    double radius,
	    const int absorberactive = 0,
	    int verbosity = 0) {

  overlappcheck = 1;  
  if (radius > 21-1.17) {
    cout << "inconsistency: radius: " << radius 
	 << " larger than tpc inner  radius: " << 21-1.17 << endl;
    gSystem->Exit(-1);
  }
  
  gSystem->Load("libg4tpc.so");
  gSystem->Load("libg4testbench.so");

  PHG4TPCSubsystem *tpc = new PHG4TPCSubsystem("TPC");
  tpc->SetActive();
  tpc->SuperDetector("TPC");
  // tpc_set_double_param("gas_inner_radius",21.);
  // tpc_set_double_param("gas_outer_radius",77.);
  // tpc_set_double_param("place_x", 0.);
  // tpc_set_double_param("place_y", 0.);
  // tpc_set_double_param("place_z", 0.);
  // tpc_set_double_param("rot_x", 0.);
  // tpc_set_double_param("rot_y", 0.);
  // tpc_set_double_param("rot_z", 0.);
  // tpc_set_double_param("tpc_length",211.);

  // tpc_set_double_param("steplimits", NAN);

  // tpc_set_string_param("tpc_gas", "sPHENIX_TPC_Gas");

// material budget:
// Cu (all layers): 0.5 oz cu per square foot, 1oz == 0.0347mm --> 0.5 oz ==  0.00347cm/2. 
// Kapton insulation 18 layers of * 5mil = 18*0.0127=0.2286
// 250 um FR4 (Substrate for Cu layers)
// HoneyComb (nomex) 1/2 inch=0.5*2.54 cm
  // tpc_set_string_param("cage_layer_1_material","G4_Cu");
  // tpc_set_double_param("cage_layer_1_thickness",0.00347/2.);

  // tpc_set_string_param("cage_layer_2_material","FR4");
  // tpc_set_double_param("cage_layer_2_thickness",0.025);

  // tpc_set_string_param("cage_layer_3_material","NOMEX");
  // tpc_set_double_param("cage_layer_3_thickness",0.5*2.54);

  // tpc_set_string_param("cage_layer_4_material","G4_Cu");
  // tpc_set_double_param("cage_layer_4_thickness",0.00347/2.);

  // tpc_set_string_param("cage_layer_5_material","FR4");
  // tpc_set_double_param("cage_layer_5_thickness",0.025);

  // tpc_set_string_param("cage_layer_6_material","G4_KAPTON");
  // tpc_set_double_param("cage_layer_6_thickness",0.2286);

  // tpc_set_string_param("cage_layer_7_material","G4_Cu");
  // tpc_set_double_param("cage_layer_7_thickness",0.00347/2.);

  // tpc_set_string_param("cage_layer_8_material","G4_KAPTON");
  // tpc_set_double_param("cage_layer_8_thickness",0.05); // 50 um

  // tpc_set_string_param("cage_layer_9_material","G4_Cu");
  // tpc_set_double_param("cage_layer_9_thickness",0.00347/2.);
  if (absorberactive)  
    {
      tpc->SetAbsorberActive();
    }
  tpc->OverlapCheck(overlapcheck);

  g4Reco->registerSubsystem( tpc );  

  radius = 77.+1.17;
  

  radius += no_overlapp;
  
  return radius; 
}
