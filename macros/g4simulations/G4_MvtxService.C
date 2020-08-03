/******************************/
/*     MVTX Service Barrel    */
/*        Cameron Dean        */
/*       cdean@lanl.gov       */
/*        07/17/2020          */
/******************************/

#ifndef MACRO_G4MVTXSERVICE_C
#define MACRO_G4MVTXSERVICE_C

#include "GlobalVariables.C"

#include <g4detectors/PHG4CylinderSubsystem.h>

#include <g4main/PHG4Reco.h>

#include <cmath>

R__LOAD_LIBRARY(libg4detectors.so)

namespace Enable
{
  bool MVTXSERVICE = false;
  bool MVTXSERVICE_ABSORBER = false;
  bool MVTXSERVICE_OVERLAPCHECK = false;
   int MVTXSERVICE_VERBOSITY = 0;
}  // namespace Enable

namespace G4MVTXSERVICE
{
  /*
   * These are the parameters for all cables/wires/tubes for the MVTX service barrel
   * Uncomment and use these when it is time to make a more detailed service barrel model
   * DEFAULT G4 UNITS ARE CM, GEV AND NS 

  double signal_wire_copper_area = 0.0102; //[mm^2] Cross sectional area of copper for one signal wire
  double signal_wire_plastic_area = 0.0407; //[mm^2] Cross sectional area of plastic for one signal wire
     int n_signal_wires_per_stave = 12; //Number of signal wires for each stave

  double digital_power_OR = 1.175; //Digital power parameters, [OR/IR] is the [outer/inner] radius of the cable sheath
  double digital_power_IR = 0.775;
  double digital_power_copper_area =  calculateArea( 0, digital_power_IR );
  double digital_power_plastic_area = calculateArea( digital_power_IR, digital_power_OR );
     int n_digital_power = 2;

  double analogue_power_OR = 0.775; //Analogue power parameters
  double analogue_power_IR = 0.375;
  double analogue_power_copper_area =  calculateArea( 0, analogue_power_IR );
  double analogue_power_plastic_area = calculateArea( analogue_power_IR, analogue_power_OR );
     int n_analogue_power = 2;

  double ground_power_OR = 0.725; //Ground power parameters
  double ground_power_IR = 0.325;
  double ground_power_copper_area =  calculateArea( 0, ground_power_IR );
  double ground_power_plastic_area = calculateArea( ground_power_IR, ground_power_OR );
     int n_ground_power = 6;

  double power_cable_sheath_OR = 4.3; //The power cables are enclosed within their own plastic sheath
  double power_cable_sheath_IR = 3.3;
  double power_cable_sheath_area = calculateArea( power_cable_sheath_IR, power_cable_sheath_OR );

  double power_cable_copper_area = n_digital_power*digital_power_copper_area
                                 + n_analogue_power*analogue_power_copper_area
                                 + n_ground_power*ground_power_copper_area; //[mm^2] Cross sectional area of copper for one power cable
  double power_cable_plastic_area = power_cable_sheath_area
                                  + n_digital_power*digital_power_plastic_area
                                  + n_analogue_power*analogue_power_plastic_area
                                  + n_ground_power*ground_power_plastic_area;  //[mm^2] Cross sectional area of plastic for one power cable
  double power_cable_air_area = 27.2; //[mm^2] Free space in each power cable
     int n_power_cabless_per_stave = 1; //Number of power cables for each stave

  double cooling_tube_OR = 0.794; //Outer radius of cooling tube
  double cooling_tube_IR = 0.397; //Inner radius of cooling tube
  double cooling_tube_water_area = calculateArea( 0, cooling_tube_IR); //[mm^2] Cross sectional area of water for one cooling tube
  double cooling_tube_plastic_area = calculateArea( cooling_tube_IR, cooling_tube_OR); //[mm^2] Cross sectional area of plastic for one cooling tube
     int n_cooling_tubes_per_stave = 2; //Number of cooling tubes for each stave

  */
  double single_stave_copper_area  = 0.0677; //Cross-sectional area of copper for 1 stave [cm^2]
  double single_stave_water_area   = 0.0098; //Cross-sectional area of water for 1 stave [cm^2]
  double single_stave_plastic_area = 0.4303; //Cross-sectional area of plastic for 1 stave [cm^2]

  const int n_layers = 1; //Number of service cable layers to generate
  double layer_start_radius[] = {8.5}; //Inner radius of where the cables begin [cm]
     int n_staves_layer[] = {48}; //Number of staves associated to each layer

  double service_barrel_radius = 10.75; // [cm] From final design review
  double service_barrel_start  = -35; //[cm] Approx (Straight barrel length (419mm) - 0.5*stave active length (270mm/2))
  double service_barrel_length = 150; // [cm] length of service barrel ~(to patch panel)
}  // namespace G4MVTXSERVICE


double calculateArea( double inner_radius, double outer_radius ) //Calculate the area of a disk
{
	return M_PI*( std::pow( outer_radius, 2 ) - std::pow( inner_radius, 2) );
}

double calculateOR( double inner_radius, double area ) //Calculate the outer radius of a disk, knowing the inner radius and the area
{
	return std::sqrt( area/M_PI + std::pow( inner_radius, 2 ) );
}

void calculateMaterialBoundaries(int& layer_ID, double& outer_copper_radius, double& outer_water_radius, double& outer_plastic_radius) //Calculate where the transition between each material occurs
{
   outer_copper_radius  = calculateOR( G4MVTXSERVICE::layer_start_radius[layer_ID], G4MVTXSERVICE::n_staves_layer[layer_ID]*G4MVTXSERVICE::single_stave_copper_area );
   outer_water_radius   = calculateOR( outer_copper_radius, G4MVTXSERVICE::n_staves_layer[layer_ID]*G4MVTXSERVICE::single_stave_water_area );
   outer_plastic_radius = calculateOR( outer_water_radius, G4MVTXSERVICE::n_staves_layer[layer_ID]*G4MVTXSERVICE::single_stave_plastic_area );
}

void MVTXServiceInit()
{
  BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, G4MVTXSERVICE::service_barrel_radius);
  BlackHoleGeometry::min_z = std::min(BlackHoleGeometry::min_z, -1*(G4MVTXSERVICE::service_barrel_length + G4MVTXSERVICE::service_barrel_start));
  BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z, -G4MVTXSERVICE::service_barrel_start);
}

double MVTXService(PHG4Reco* g4Reco, double radius)
{
  bool AbsorberActive = Enable::ABSORBER || Enable::MVTXSERVICE_ABSORBER;
  bool OverlapCheck = Enable::OVERLAPCHECK || Enable::MVTXSERVICE_OVERLAPCHECK;
  int verbosity = std::max(Enable::VERBOSITY, Enable::MVTXSERVICE_VERBOSITY);
/*
  if (radius > G4MVTXSERVICE::service_barrel_radius)
  {
    std::cout << "inconsistency: radius: " << radius
              << " larger than service barrel radius: " << G4MVTXSERVICE::service_barrel_radius << std::endl;
    gSystem->Exit(-1);
  }
*/
  // Note, cables are all south
  //Setup layers
  double copper_OR[G4MVTXSERVICE::n_layers], water_OR[G4MVTXSERVICE::n_layers], plastic_OR[G4MVTXSERVICE::n_layers]; //Objects for material outer radii
  int subsystem_layer = 0;
  std::string copper_name, water_name, plastic_name;
  PHG4CylinderSubsystem* cyl;

  for (int i = 0; i < G4MVTXSERVICE::n_layers; ++i) //Build a layer of copper, then water, then plastic
  {
    calculateMaterialBoundaries(i, copper_OR[i], water_OR[i], plastic_OR[i]);

    copper_name  = "MVTX_Service_copper_layer_"  + std::to_string(i);
    water_name   = "MVTX_Service_water_layer_"   + std::to_string(i);
    plastic_name = "MVTX_Service_plastic_layer_" + std::to_string(i);

    cyl = new PHG4CylinderSubsystem(copper_name, subsystem_layer);
    cyl->set_double_param("place_z", -1*(G4MVTXSERVICE::service_barrel_length + G4MVTXSERVICE::service_barrel_start) - no_overlapp);
    cyl->set_double_param("radius", G4MVTXSERVICE::layer_start_radius[i]);
    cyl->set_int_param("lengthviarapidity", 0);
    cyl->set_double_param("length", G4MVTXSERVICE::service_barrel_length);
    cyl->set_string_param("material", "G4_Cu");
    cyl->set_double_param("thickness", copper_OR[i] - G4MVTXSERVICE::layer_start_radius[i]);
    cyl->SuperDetector("MVTXSERVICE");
    if (AbsorberActive) cyl->SetActive();
    cyl->OverlapCheck(OverlapCheck);
    g4Reco->registerSubsystem(cyl);
    subsystem_layer += 1;

    cyl = new PHG4CylinderSubsystem(water_name, subsystem_layer);
    cyl->set_double_param("place_z", -1*(G4MVTXSERVICE::service_barrel_length + G4MVTXSERVICE::service_barrel_start) - no_overlapp);
    cyl->set_double_param("radius", copper_OR[i]);
    cyl->set_int_param("lengthviarapidity", 0);
    cyl->set_double_param("length", G4MVTXSERVICE::service_barrel_length);
    cyl->set_string_param("material", "G4_WATER");
    cyl->set_double_param("thickness", water_OR[i] - copper_OR[i]);
    cyl->SuperDetector("MVTXSERVICE");
    if (AbsorberActive) cyl->SetActive();
    cyl->OverlapCheck(OverlapCheck);
    g4Reco->registerSubsystem(cyl);
    subsystem_layer += 1;

    cyl = new PHG4CylinderSubsystem(plastic_name, subsystem_layer);
    cyl->set_double_param("place_z", -1*(G4MVTXSERVICE::service_barrel_length + G4MVTXSERVICE::service_barrel_start) - no_overlapp);
    cyl->set_double_param("radius", water_OR[i]);
    cyl->set_int_param("lengthviarapidity", 0);
    cyl->set_double_param("length", G4MVTXSERVICE::service_barrel_length);
    cyl->set_string_param("material", "G4_POLYETHYLENE");
    cyl->set_double_param("thickness", plastic_OR[i] - water_OR[i]);
    cyl->SuperDetector("MVTXSERVICE");
    if (AbsorberActive) cyl->SetActive();
    cyl->OverlapCheck(OverlapCheck);
    g4Reco->registerSubsystem(cyl);
    subsystem_layer += 1;

  }

  cyl = new PHG4CylinderSubsystem("MVTX_Service_shell_layer", subsystem_layer);
  cyl->set_double_param("place_z", -1*(G4MVTXSERVICE::service_barrel_length + G4MVTXSERVICE::service_barrel_start) - no_overlapp);
  cyl->set_double_param("radius", G4MVTXSERVICE::service_barrel_radius);
  cyl->set_int_param("lengthviarapidity", 0);
  cyl->set_double_param("length", G4MVTXSERVICE::service_barrel_length);
  cyl->set_string_param("material", "PEEK"); //Service barrel is carbon fibre (peek?)
  cyl->set_double_param("thickness", 0.1); //Service barrel is 1mm thick
  cyl->SuperDetector("MVTXSERVICE");
  if (AbsorberActive) cyl->SetActive();
  cyl->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(cyl);

  radius = G4MVTXSERVICE::service_barrel_radius;

  if (verbosity > 0)
  {
    cout << "=========================== G4_MVTXService.C::MVTXService() =============================" << endl;
    cout << " MVTX Service Material Description:" << endl;

    cout << "  Single stave copper area  = " << G4MVTXSERVICE::single_stave_copper_area << " cm^2" << endl;
    cout << "  Single stave water area   = " << G4MVTXSERVICE::single_stave_water_area << " cm^2" << endl;
    cout << "  Single stave plastic area = " << G4MVTXSERVICE::single_stave_plastic_area << " cm^2" << endl;

    for (int j = 0; j < G4MVTXSERVICE::n_layers; ++j)
    {
        cout << "  Layer " << j << " starts at " << G4MVTXSERVICE::layer_start_radius[j] << " cm" << endl;
        cout << "  Layer " << j << " services " << G4MVTXSERVICE::n_staves_layer[j] << " staves" << endl;
    }

    cout << "  Service barrel radius = " << G4MVTXSERVICE::service_barrel_radius << " cm" << endl;
    cout << "  Service barrel start = " << G4MVTXSERVICE::service_barrel_start << " cm" << endl;
    cout << "  Service barrel length = " << G4MVTXSERVICE::service_barrel_length << " cm" << endl;
    cout << "=========================================================================================" << endl;
  }

  radius += no_overlapp;

  return radius;
}
#endif
