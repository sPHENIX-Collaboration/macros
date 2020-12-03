#ifndef MACRO_G4MVTX_C
#define MACRO_G4MVTX_C

#include <GlobalVariables.C>
#include <QA.C>

#include <g4detectors/PHG4CylinderSubsystem.h>
#include <g4mvtx/PHG4MvtxDefs.h>
#include <g4mvtx/PHG4MvtxDigitizer.h>
#include <g4mvtx/PHG4MvtxHitReco.h>
#include <g4mvtx/PHG4MvtxSubsystem.h>

#include <g4main/PHG4Reco.h>

#include <mvtx/MvtxClusterizer.h>
#include <qa_modules/QAG4SimulationMvtx.h>

#include <fun4all/Fun4AllServer.h>

#include <cmath>
#include <vector>

R__LOAD_LIBRARY(libg4mvtx.so)
R__LOAD_LIBRARY(libmvtx.so)
R__LOAD_LIBRARY(libqa_modules.so)

namespace Enable
{
  bool MVTX = false;
  bool MVTX_OVERLAPCHECK = false;
  bool MVTX_CELL = false;
  bool MVTX_CLUSTER = false;
  bool MVTX_QA = false;
  bool MVTX_ABSORBER = false;
  bool MVTX_SERVICE = true;
  int MVTX_VERBOSITY = 0;

}  // namespace Enable

namespace G4MVTX
{
  int n_maps_layer = 3;        // must be 0-3, setting it to zero removes Mvtx completely, n < 3 gives the first n layers
  double radius_offset = 0.7;  // clearance around radius

  double single_stave_service_copper_area = 0.0677;   //Cross-sectional area of copper for 1 stave [cm^2]
  double single_stave_service_water_area = 0.0098;    //Cross-sectional area of water for 1 stave [cm^2]
  double single_stave_service_plastic_area = 0.4303;  //Cross-sectional area of plastic for 1 stave [cm^2]

  const int n_service_layers = 1;               //Number of service cable service_layers to generate
  double service_layer_start_radius[] = {8.5};  //Inner radius of where the cables begin [cm]
  int n_staves_service_layer[] = {48};          //Number of staves associated to each service layer

  double service_barrel_radius = 10.75;  // [cm] From final design review
  double service_barrel_start = -35;     //[cm] Approx.
  double service_barrel_length = 150;    // [cm] length of service barrel ~(to patch panel)
}  // namespace G4MVTX

void MvtxInit()
{
  //BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, (PHG4MvtxDefs::mvtxdat[G4MVTX::n_maps_layer - 1][PHG4MvtxDefs::kRmd]) / 10. + G4MVTX::radius_offset);
  //BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z, 16.);
  BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, G4MVTX::service_barrel_radius);
  BlackHoleGeometry::min_z = std::min(BlackHoleGeometry::min_z, -1 * (G4MVTX::service_barrel_length + G4MVTX::service_barrel_start));
  BlackHoleGeometry::min_z = std::min(BlackHoleGeometry::min_z, -17.);
}

double calculateArea(double inner_radius, double outer_radius)  //Calculate the area of a disk
{
  return M_PI * (std::pow(outer_radius, 2) - std::pow(inner_radius, 2));
}

double calculateOR(double inner_radius, double area)  //Calculate the outer radius of a disk, knowing the inner radius and the area
{
  return std::sqrt(area / M_PI + std::pow(inner_radius, 2));
}

void calculateMaterialBoundaries(int& service_layer_ID, double& outer_copper_radius, double& outer_water_radius, double& outer_plastic_radius)  //Calculate where the transition between each material occurs
{
  outer_copper_radius = calculateOR(G4MVTX::service_layer_start_radius[service_layer_ID], G4MVTX::n_staves_service_layer[service_layer_ID] * G4MVTX::single_stave_service_copper_area);
  outer_water_radius = calculateOR(outer_copper_radius, G4MVTX::n_staves_service_layer[service_layer_ID] * G4MVTX::single_stave_service_water_area);
  outer_plastic_radius = calculateOR(outer_water_radius, G4MVTX::n_staves_service_layer[service_layer_ID] * G4MVTX::single_stave_service_plastic_area);
}

double MVTXService(PHG4Reco* g4Reco, double radius)
{
  // Note, cables are all south
  // Setup service_layers
  bool AbsorberActive = Enable::ABSORBER || Enable::MVTX_ABSORBER;
  bool OverlapCheck = Enable::OVERLAPCHECK || Enable::MVTX_OVERLAPCHECK;
  int verbosity = std::max(Enable::VERBOSITY, Enable::MVTX_VERBOSITY);

  double copper_OR[G4MVTX::n_service_layers], water_OR[G4MVTX::n_service_layers], plastic_OR[G4MVTX::n_service_layers];  //Objects for material outer radii
  int subsystem_service_layer = 0;
  std::string copper_name, water_name, plastic_name;
  PHG4CylinderSubsystem* cyl;

  for (int i = 0; i < G4MVTX::n_service_layers; ++i)  //Build a service_layer of copper, then water, then plastic
  {
    calculateMaterialBoundaries(i, copper_OR[i], water_OR[i], plastic_OR[i]);

    copper_name = "MVTX_Service_copper_service_layer_" + std::to_string(i);
    water_name = "MVTX_Service_water_service_layer_" + std::to_string(i);
    plastic_name = "MVTX_Service_plastic_service_layer_" + std::to_string(i);

    cyl = new PHG4CylinderSubsystem(copper_name, subsystem_service_layer);
    cyl->set_double_param("place_z", -1 * (G4MVTX::service_barrel_length + G4MVTX::service_barrel_start) - no_overlapp);
    cyl->set_double_param("radius", G4MVTX::service_layer_start_radius[i]);
    cyl->set_int_param("lengthviarapidity", 0);
    cyl->set_double_param("length", G4MVTX::service_barrel_length);
    cyl->set_string_param("material", "G4_Cu");
    cyl->set_double_param("thickness", copper_OR[i] - G4MVTX::service_layer_start_radius[i]);
    cyl->SuperDetector("MVTXSERVICE");
    if (AbsorberActive) cyl->SetActive();
    cyl->OverlapCheck(OverlapCheck);
    g4Reco->registerSubsystem(cyl);
    subsystem_service_layer += 1;

    cyl = new PHG4CylinderSubsystem(water_name, subsystem_service_layer);
    cyl->set_double_param("place_z", -1 * (G4MVTX::service_barrel_length + G4MVTX::service_barrel_start) - no_overlapp);
    cyl->set_double_param("radius", copper_OR[i]);
    cyl->set_int_param("lengthviarapidity", 0);
    cyl->set_double_param("length", G4MVTX::service_barrel_length);
    cyl->set_string_param("material", "G4_WATER");
    cyl->set_double_param("thickness", water_OR[i] - copper_OR[i]);
    cyl->SuperDetector("MVTXSERVICE");
    if (AbsorberActive) cyl->SetActive();
    cyl->OverlapCheck(OverlapCheck);
    g4Reco->registerSubsystem(cyl);
    subsystem_service_layer += 1;

    cyl = new PHG4CylinderSubsystem(plastic_name, subsystem_service_layer);
    cyl->set_double_param("place_z", -1 * (G4MVTX::service_barrel_length + G4MVTX::service_barrel_start) - no_overlapp);
    cyl->set_double_param("radius", water_OR[i]);
    cyl->set_int_param("lengthviarapidity", 0);
    cyl->set_double_param("length", G4MVTX::service_barrel_length);
    cyl->set_string_param("material", "G4_POLYETHYLENE");
    cyl->set_double_param("thickness", plastic_OR[i] - water_OR[i]);
    cyl->SuperDetector("MVTXSERVICE");
    if (AbsorberActive) cyl->SetActive();
    cyl->OverlapCheck(OverlapCheck);
    g4Reco->registerSubsystem(cyl);
    subsystem_service_layer += 1;
  }

  cyl = new PHG4CylinderSubsystem("MVTX_Service_shell_service_layer", subsystem_service_layer);
  cyl->set_double_param("place_z", -1 * (G4MVTX::service_barrel_length + G4MVTX::service_barrel_start) - no_overlapp);
  cyl->set_double_param("radius", G4MVTX::service_barrel_radius);
  cyl->set_int_param("lengthviarapidity", 0);
  cyl->set_double_param("length", G4MVTX::service_barrel_length);
  cyl->set_string_param("material", "PEEK");  //Service barrel is carbon fibre (peek?)
  cyl->set_double_param("thickness", 0.1);    //Service barrel is 1mm thick
  cyl->SuperDetector("MVTXSERVICE");
  if (AbsorberActive) cyl->SetActive();
  cyl->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(cyl);

  radius = G4MVTX::service_barrel_radius;

  if (verbosity > 0)
  {
    cout << "=========================== MVTX Service Barrel =============================" << endl;
    cout << " MVTX Service Material Description:" << endl;

    cout << "  Single stave copper area  = " << G4MVTX::single_stave_service_copper_area << " cm^2" << endl;
    cout << "  Single stave water area   = " << G4MVTX::single_stave_service_water_area << " cm^2" << endl;
    cout << "  Single stave plastic area = " << G4MVTX::single_stave_service_plastic_area << " cm^2" << endl;

    for (int j = 0; j < G4MVTX::n_service_layers; ++j)
    {
      cout << "  service_layer " << j << " starts at " << G4MVTX::service_layer_start_radius[j] << " cm" << endl;
      cout << "  service_layer " << j << " services " << G4MVTX::n_staves_service_layer[j] << " staves" << endl;
    }

    cout << "  Service barrel radius = " << G4MVTX::service_barrel_radius << " cm" << endl;
    cout << "  Service barrel start = " << G4MVTX::service_barrel_start << " cm" << endl;
    cout << "  Service barrel length = " << G4MVTX::service_barrel_length << " cm" << endl;
    cout << "===============================================================================" << endl;
  }

  radius += no_overlapp;

  return radius;
}

double Mvtx(PHG4Reco* g4Reco, double radius,
            const int absorberactive = 0)
{
  bool maps_overlapcheck = Enable::OVERLAPCHECK || Enable::MVTX_OVERLAPCHECK;
  int verbosity = std::max(Enable::VERBOSITY, Enable::MVTX_VERBOSITY);

  PHG4MvtxSubsystem* mvtx = new PHG4MvtxSubsystem("MVTX");
  mvtx->Verbosity(verbosity);

  for (int ilayer = 0; ilayer < G4MVTX::n_maps_layer; ilayer++)
  {
    double radius_lyr = PHG4MvtxDefs::mvtxdat[ilayer][PHG4MvtxDefs::kRmd];
    if (verbosity)
    {
      cout << "Create Maps layer " << ilayer << " with radius " << radius_lyr << " mm." << endl;
    }
    radius = radius_lyr / 10.;
  }
  mvtx->set_string_param(PHG4MvtxDefs::GLOBAL, "stave_geometry_file", string(getenv("CALIBRATIONROOT")) + string("/Tracking/geometry/mvtx_stave_v1.gdml"));
  mvtx->SetActive();
  mvtx->OverlapCheck(maps_overlapcheck);
  g4Reco->registerSubsystem(mvtx);
  radius += G4MVTX::radius_offset;
  if (Enable::MVTX_SERVICE) radius += MVTXService(g4Reco, radius);
  return radius;
}

// Central detector cell reco is disabled as EIC setup use the fast tracking sim for now
void Mvtx_Cells()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::MVTX_VERBOSITY);
  Fun4AllServer* se = Fun4AllServer::instance();
  // new storage containers
  PHG4MvtxHitReco* maps_hits = new PHG4MvtxHitReco("MVTX");
  maps_hits->Verbosity(verbosity);
  for (int ilayer = 0; ilayer < G4MVTX::n_maps_layer; ilayer++)
  {
    // override the default timing window for this layer - default is +/- 5000 ns
    maps_hits->set_timing_window(ilayer, -5000, 5000);
  }
  se->registerSubsystem(maps_hits);
  return;
}

void Mvtx_Clustering()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::MVTX_VERBOSITY);
  Fun4AllServer* se = Fun4AllServer::instance();
  PHG4MvtxDigitizer* digimvtx = new PHG4MvtxDigitizer();
  digimvtx->Verbosity(verbosity);
  // energy deposit in 25 microns = 9.6 KeV = 1000 electrons collected after recombination
  //digimvtx->set_adc_scale(0.95e-6);  // default set in code is 0.95e-06, which is 99 electrons
  se->registerSubsystem(digimvtx);
  // For the Mvtx layers
  //================
  MvtxClusterizer* mvtxclusterizer = new MvtxClusterizer("MvtxClusterizer");
  mvtxclusterizer->Verbosity(verbosity);
  se->registerSubsystem(mvtxclusterizer);
}

void Mvtx_QA()
{
  int verbosity = std::max(Enable::QA_VERBOSITY, Enable::MVTX_VERBOSITY);

  Fun4AllServer* se = Fun4AllServer::instance();
  QAG4SimulationMvtx* qa = new QAG4SimulationMvtx;
  qa->Verbosity(verbosity);
  se->registerSubsystem(qa);
}

#endif
