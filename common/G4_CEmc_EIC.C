#ifndef MACRO_G4CEMCEIC_C
#define MACRO_G4CEMCEIC_C

#include <GlobalVariables.C>

#include <g4calo/RawTowerBuilder.h>
#include <g4calo/RawTowerDigitizer.h>

#include <g4detectors/PHG4CylinderCellReco.h>
#include <g4detectors/PHG4CylinderSubsystem.h>

#include <g4eval/CaloEvaluator.h>

#include <g4main/PHG4Reco.h>

#include <caloreco/RawClusterBuilderGraph.h>
#include <caloreco/RawClusterBuilderTemplate.h>
#include <caloreco/RawTowerCalibration.h>

#include <fun4all/Fun4AllServer.h>

#include <cmath>

R__LOAD_LIBRARY(libcalo_reco.so)
R__LOAD_LIBRARY(libg4calo.so)
R__LOAD_LIBRARY(libg4detectors.so)
R__LOAD_LIBRARY(libg4eval.so)

namespace Enable
{
  bool CEMC = false;
  bool CEMC_ABSORBER = false;
  bool CEMC_OVERLAPCHECK = false;
  bool CEMC_CELL = false;
  bool CEMC_TOWER = false;
  bool CEMC_CLUSTER = false;
  bool CEMC_EVAL = false;
  int CEMC_VERBOSITY = 0;
}  // namespace Enable

namespace G4CEMC
{
  double cemcdepth = 9;
  // tungs to scint width ratio of ~10:1
  // corresponds to approx 2% sampling fraction

  // 18 radiation lengths for 40 layers
  double scint_width = 0.05;
  double tungs_width = 0.245;
  double electronics_width = 0.5;

  int min_cemc_layer = 1;
  int max_cemc_layer = 41;

  double topradius = 106.8;  // cm
  double bottomradius = 95;  // cm
  double negrapidity = -1.5;
  double posrapidity = 1.24;
  // this is default set to -1.5<eta<1.24 for 2018 Letter of Intent
  // if the user changes these, the z position of the
  // calorimeter must be changed in the function CEmc(...)

  // Digitization (default photon digi):
  RawTowerDigitizer::enu_digi_algorithm TowerDigi = RawTowerDigitizer::kSimple_photon_digitization;
  // directly pass the energy of sim tower to digitized tower
  // kNo_digitization
  // simple digitization with photon statistics, single amplitude ADC conversion and pedestal
  // kSimple_photon_digitization
  // digitization with photon statistics on SiPM with an effective pixel N, ADC conversion and pedestal
  // kSiPM_photon_digitization

  enum enu_Cemc_clusterizer
  {
    kCemcGraphClusterizer,

    kCemcTemplateClusterizer
  };

  // default: template clusterizer, RawClusterBuilderTemplate, as developed by Sasha Bazilevsky
  enu_Cemc_clusterizer Cemc_clusterizer = kCemcTemplateClusterizer;
  // graph clusterizer, RawClusterBuilderGraph
  // enu_Cemc_clusterizer Cemc_clusterizer = kCemcGraphClusterizer;
}  // namespace G4CEMC

namespace CEMC_TOWER
{
  double emin = NAN;
}

// Black hole and size parameters set in CEmc function
void CEmcInit(const int nslats = 1)
{
}

double CEmc(PHG4Reco *g4Reco, double radius)
{
  bool AbsorberActive = Enable::ABSORBER || Enable::CEMC_ABSORBER;
  bool OverlapCheck = Enable::OVERLAPCHECK || Enable::CEMC_OVERLAPCHECK;

  if (radius > 95)
  {
    cout << "inconsistency, radius: " << radius
         << " larger than allowed inner radius for CEMC = 95 cm" << endl;
    gSystem->Exit(-1);
  }

  radius = 95;

  PHG4CylinderSubsystem *cemc;

  // determine the length of the calorimeter
  // can adjust length coverage by just adjusting these values
  // rapidity coverage will be determined by z shift of EMCAl
  // as indicated in the loop below

  // eta = -ln(tan(theta/2))
  double theta1 = 2. * TMath::ATan(TMath::Exp(-1 * G4CEMC::posrapidity));
  double theta2 = 2. * TMath::ATan(TMath::Exp(-1 * G4CEMC::negrapidity));
  // get the angle between the beam pipe and negative pseudorapidity axis
  theta2 = M_PI - theta2;

  double z1 = G4CEMC::topradius / TMath::Tan(theta1);
  double z2 = G4CEMC::topradius / TMath::Tan(theta2);

  double z3 = G4CEMC::bottomradius / TMath::Tan(theta1);
  double z4 = G4CEMC::bottomradius / TMath::Tan(theta2);

  // this is the top layer length
  double totaltoplength = z1 + z2;
  // this is the bottom layer length
  double totalbottomlength = z3 + z4;

  //Added by Barak, 12/12/19
  double ztemp = 0;
  double layer_shift = 0;

  double height = 0;
  for (int thislayer = G4CEMC::min_cemc_layer; thislayer <= G4CEMC::max_cemc_layer;
       thislayer++)
  {
    // the length for a particular layer is determined from the bottom length
    double thislength = totalbottomlength + (height / TMath::Tan(theta1)) + (height / TMath::Tan(theta2));

    cemc = new PHG4CylinderSubsystem("ABSORBER_CEMC", thislayer);
    cemc->set_double_param("radius", radius);
    cemc->set_string_param("material", "Spacal_W_Epoxy");
    cemc->set_double_param("thickness", G4CEMC::tungs_width);
    cemc->set_double_param("length", thislength);
    cemc->set_int_param("lengthviarapidity", 0);

    // starts centered around IP
    // shift backwards 30 cm for total 370 cm length to cover -1.5<eta<1.24
    //cemc->set_double_param("place_z", -30);

    //Modified by Barak, 12/12/19
    ztemp = radius / TMath::Tan(theta2);
    layer_shift = -1. * (ztemp - (thislength / 2.));
    cemc->set_double_param("place_z", layer_shift);

    cemc->SuperDetector("ABSORBER_CEMC");
    if (AbsorberActive) cemc->SetActive();
    cemc->OverlapCheck(OverlapCheck);

    g4Reco->registerSubsystem(cemc);

    radius += G4CEMC::tungs_width;
    radius += no_overlapp;

    height += G4CEMC::tungs_width;
    height += no_overlapp;  //Added by Barak, 12/13/19

    //Added by Barak, 12/13/19
    thislength = totalbottomlength + (height / TMath::Tan(theta1)) + (height / TMath::Tan(theta2));

    cemc = new PHG4CylinderSubsystem("CEMC", thislayer);
    cemc->set_double_param("radius", radius);
    cemc->set_string_param("material", "PMMA");
    cemc->set_double_param("thickness", G4CEMC::scint_width);
    cemc->set_int_param("lightyield", 1);
    cemc->set_int_param("lengthviarapidity", 0);
    cemc->set_double_param("length", thislength);

    // shift back -30 cm to cover -1.4<eta<1.1
    //cemc->set_double_param("place_z", -30);

    //Modified by Barak, 12/12/19
    cemc->set_double_param("place_z", layer_shift);

    cemc->SuperDetector("CEMC");

    cemc->SetActive();
    cemc->OverlapCheck(OverlapCheck);
    g4Reco->registerSubsystem(cemc);

    radius += G4CEMC::scint_width;
    radius += no_overlapp;

    height += G4CEMC::scint_width;
    height += no_overlapp;  //Added by Barak, 12/13/19
  }

  PHG4CylinderSubsystem *cemc_cyl = new PHG4CylinderSubsystem("CEMC_ELECTRONICS", 0);
  cemc_cyl->set_double_param("radius", radius);
  cemc_cyl->set_string_param("material", "G4_TEFLON");
  cemc_cyl->set_double_param("thickness", G4CEMC::electronics_width);

  double l1 = (radius + 0.5) / TMath::Tan(theta1);
  double l2 = (radius + 0.5) / TMath::Tan(theta2);
  cemc_cyl->set_int_param("lengthviarapidity", 0);
  cemc_cyl->set_double_param("length", l1 + l2);
  // shift back -30 cm to cover -1.4<eta<1.1
  //cemc_cyl->set_double_param("place_z", -30);

  //Modified by Barak, 12/12/19
  layer_shift = -1. * ((l2 - l1) / 2.);
  cemc_cyl->set_double_param("place_z", layer_shift);

  if (AbsorberActive) cemc_cyl->SetActive();
  cemc_cyl->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(cemc_cyl);
  // update black hole settings since we have the values here
  BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, radius + G4CEMC::electronics_width);
  BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z, layer_shift + (l1 + l2) / 2.);
  BlackHoleGeometry::min_z = std::min(BlackHoleGeometry::min_z, layer_shift - (l1 + l2) / 2.);

  return radius;
}

void CEMC_Cells()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::CEMC_VERBOSITY);

  Fun4AllServer *se = Fun4AllServer::instance();

  PHG4CylinderCellReco *cemc_cells = new PHG4CylinderCellReco("CEMCCYLCELLRECO");
  cemc_cells->Detector("CEMC");
  cemc_cells->Verbosity(verbosity);
  double radius = 95;
  for (int i = G4CEMC::min_cemc_layer; i <= G4CEMC::max_cemc_layer; i++)
  {
    //Added by Barak, 12/13/19
    radius += (G4CEMC::tungs_width + no_overlapp);
    if (i > 1) radius += (G4CEMC::scint_width + no_overlapp);

    cemc_cells->cellsize(i, 2. * M_PI / 256. * radius, 2. * M_PI / 256. * radius);
  }
  se->registerSubsystem(cemc_cells);

  return;
}

void CEMC_Towers()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::CEMC_VERBOSITY);

  Fun4AllServer *se = Fun4AllServer::instance();

  RawTowerBuilder *CemcTowerBuilder = new RawTowerBuilder("EmcRawTowerBuilder");
  CemcTowerBuilder->Detector("CEMC");
  CemcTowerBuilder->set_sim_tower_node_prefix("SIM");
  if (isfinite(CEMC_TOWER::emin))
  {
    CemcTowerBuilder->EminCut(CEMC_TOWER::emin);
  }
  CemcTowerBuilder->Verbosity(verbosity);
  se->registerSubsystem(CemcTowerBuilder);

  const double photoelectron_per_GeV = 500;  // 500 photon per total GeV deposition
  // just set a 4% sampling fraction - already tuned by tungs/scint width ratio
  double sampling_fraction = 4e-02;
  RawTowerDigitizer *CemcTowerDigitizer = new RawTowerDigitizer("EmcRawTowerDigitizer");
  CemcTowerDigitizer->Detector("CEMC");
  CemcTowerDigitizer->Verbosity(verbosity);
  CemcTowerDigitizer->set_digi_algorithm(G4CEMC::TowerDigi);
  CemcTowerDigitizer->set_pedstal_central_ADC(0);
  CemcTowerDigitizer->set_pedstal_width_ADC(8);  // eRD1 test beam setting
  CemcTowerDigitizer->set_photonelec_ADC(1);     // not simulating ADC discretization error
  CemcTowerDigitizer->set_photonelec_yield_visible_GeV(photoelectron_per_GeV / sampling_fraction);
  CemcTowerDigitizer->set_zero_suppression_ADC(16);  // eRD1 test beam setting
  se->registerSubsystem(CemcTowerDigitizer);

  RawTowerCalibration *CemcTowerCalibration = new RawTowerCalibration("EmcRawTowerCalibration");
  CemcTowerCalibration->Detector("CEMC");
  CemcTowerCalibration->Verbosity(verbosity);
  CemcTowerCalibration->set_calib_algorithm(RawTowerCalibration::kSimple_linear_calibration);
  if (G4CEMC::TowerDigi == RawTowerDigitizer::kNo_digitization)
  {
    CemcTowerCalibration->set_calib_const_GeV_ADC(1.0 / 0.023);  // 2.3% sampling fraction from test beam
  }
  else
  {
    CemcTowerCalibration->set_calib_const_GeV_ADC(1. / photoelectron_per_GeV / 0.9715);
  }
  CemcTowerCalibration->set_pedstal_ADC(0);

  se->registerSubsystem(CemcTowerCalibration);

  return;
}

void CEMC_Clusters()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::CEMC_VERBOSITY);

  Fun4AllServer *se = Fun4AllServer::instance();

  if (G4CEMC::Cemc_clusterizer == G4CEMC::kCemcTemplateClusterizer)
  {
    RawClusterBuilderTemplate *cemc_clusterbuilder = new RawClusterBuilderTemplate("EmcRawClusterBuilderTemplate");
    cemc_clusterbuilder->Detector("CEMC");
    cemc_clusterbuilder->Verbosity(verbosity);

    cemc_clusterbuilder->set_threshold_energy(0.030);  // This threshold should be the same as in CEMCprof_Thresh**.root file below
    std::string femc_prof = getenv("CALIBRATIONROOT");
    femc_prof += "/EmcProfile/CEMCprof_Thresh30MeV.root";
    cemc_clusterbuilder->LoadProfile(femc_prof);
    se->registerSubsystem(cemc_clusterbuilder);
  }
  else if (G4CEMC::Cemc_clusterizer == G4CEMC::kCemcGraphClusterizer)
  {
    RawClusterBuilderGraph *cemc_clusterbuilder = new RawClusterBuilderGraph("EmcRawClusterBuilderGraph");
    cemc_clusterbuilder->Detector("CEMC");
    cemc_clusterbuilder->Verbosity(verbosity);
    se->registerSubsystem(cemc_clusterbuilder);
  }
  else
  {
    cout << "CEMC_Clusters - unknown clusterizer setting!! " << endl;
    exit(1);
  }
  return;
}
void CEMC_Eval(const std::string &outputfile)
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::CEMC_VERBOSITY);
  Fun4AllServer *se = Fun4AllServer::instance();
  CaloEvaluator *eval = new CaloEvaluator("CEMCEVALUATOR", "CEMC", outputfile.c_str());
  eval->Verbosity(verbosity);
  se->registerSubsystem(eval);
  return;
}
#endif
