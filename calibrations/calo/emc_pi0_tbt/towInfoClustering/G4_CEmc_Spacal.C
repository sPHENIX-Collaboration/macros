#ifndef MACRO_G4CEMCSPACAL_C
#define MACRO_G4CEMCSPACAL_C

#include <GlobalVariables.C>
#include <QA.C>

#include <g4detectors/PHG4CylinderCellReco.h>
#include <g4detectors/PHG4CylinderGeom_Spacalv1.h>
#include <g4detectors/PHG4CylinderSubsystem.h>
#include <g4detectors/PHG4FullProjSpacalCellReco.h>
#include <g4detectors/PHG4SpacalSubsystem.h>

#include <g4calo/RawTowerBuilder.h>
#include <g4calo/RawTowerDigitizer.h>

#include <g4eval/CaloEvaluator.h>

#include <g4main/PHG4Reco.h>
#include <g4main/PHG4Utils.h>

#include <caloreco/RawClusterBuilderGraph.h>
#include <caloreco/RawClusterBuilderTemplate.h>
#include <caloreco/RawClusterPositionCorrection.h>
#include <caloreco/RawTowerCalibration.h>
#include <qa_modules/QAG4SimulationCalorimeter.h>

#include <fun4all/Fun4AllServer.h>

double
CEmc_2DProjectiveSpacal(PHG4Reco *g4Reco, double radius, const int crossings);

R__LOAD_LIBRARY(libcalo_reco.so)
R__LOAD_LIBRARY(libg4calo.so)
R__LOAD_LIBRARY(libg4detectors.so)
R__LOAD_LIBRARY(libg4eval.so)
R__LOAD_LIBRARY(libqa_modules.so)

namespace Enable
{
  bool CEMC = false;
  bool CEMC_ABSORBER = false;
  bool CEMC_OVERLAPCHECK = false;
  bool CEMC_CELL = false;
  bool CEMC_TOWER = false;
  bool CEMC_CLUSTER = false;
  bool CEMC_EVAL = false;
  bool CEMC_QA = false;
  int CEMC_VERBOSITY = 0;
}  // namespace Enable

namespace G4CEMC
{
  int Min_cemc_layer = 1;
  int Max_cemc_layer = 1;

  // Digitization (default photon digi):
  RawTowerDigitizer::enu_digi_algorithm TowerDigi = RawTowerDigitizer::kSimple_photon_digitization;
  // directly pass the energy of sim tower to digitized tower
  // kNo_digitization
  // simple digitization with photon statistics, single amplitude ADC conversion and pedestal
  // kSimple_photon_digitization
  // digitization with photon statistics on SiPM with an effective pixel N, ADC conversion and pedestal
  // kSiPM_photon_digitization

  //   2D azimuthal projective SPACAL (slow)
  int Cemc_spacal_configuration = PHG4CylinderGeom_Spacalv1::k2DProjectiveSpacal;

  enum enu_Cemc_clusterizer
  {
    kCemcGraphClusterizer,

    kCemcTemplateClusterizer
  };

  //! template clusterizer, RawClusterBuilderTemplate, as developed by Sasha Bazilevsky
  enu_Cemc_clusterizer Cemc_clusterizer = kCemcTemplateClusterizer;
  //! graph clusterizer, RawClusterBuilderGraph
  //enu_Cemc_clusterizer Cemc_clusterizer = kCemcGraphClusterizer;

}  // namespace G4CEMC

// black hole parameters are set in CEmc function
// needs a dummy argument to play with current G4Setup_sPHENIX.C
void CEmcInit(const int i = 0)
{
}

//! EMCal main setup macro
double
CEmc(PHG4Reco *g4Reco, double radius, const int crossings)
{
  return CEmc_2DProjectiveSpacal(/*PHG4Reco**/ g4Reco, /*double*/ radius, /*const int */ crossings);
}

//! 2D full projective SPACAL
double
CEmc_2DProjectiveSpacal(PHG4Reco *g4Reco, double radius, const int crossings)
{
  bool AbsorberActive = Enable::ABSORBER || Enable::CEMC_ABSORBER;
  bool OverlapCheck = Enable::OVERLAPCHECK || Enable::CEMC_OVERLAPCHECK;

  double emc_inner_radius = 92;  // emc inner radius from engineering drawing
  double cemcthickness = 22.50000 - no_overlapp;

  //max radius is 116 cm;
  double emc_outer_radius = emc_inner_radius + cemcthickness;  // outer radius
  assert(emc_outer_radius < 116);

  if (radius > emc_inner_radius)
  {
    cout << "inconsistency: preshower radius+thickness: " << radius
         << " larger than emc inner radius: " << emc_inner_radius << endl;
    gSystem->Exit(-1);
  }

  // the radii are only to determined the thickness of the cemc
  radius = emc_inner_radius;

  // 1.5cm thick teflon as an approximation for EMCAl light collection + electronics (10% X0 total estimated)
  PHG4CylinderSubsystem *cyl = new PHG4CylinderSubsystem("CEMC_ELECTRONICS", 0);
  cyl->set_double_param("radius", radius);
  cyl->set_string_param("material", "G4_TEFLON");
  cyl->set_double_param("thickness", 1.5 - no_overlapp);
  cyl->SuperDetector("CEMC_ELECTRONICS");
  cyl->OverlapCheck(OverlapCheck);
  if (AbsorberActive) cyl->SetActive();
  g4Reco->registerSubsystem(cyl);

  radius += 1.5;
  cemcthickness -= 1.5 + no_overlapp;

  // 0.5cm thick Stainless Steel as an approximation for EMCAl support system
  cyl = new PHG4CylinderSubsystem("CEMC_SPT", 0);
  cyl->SuperDetector("CEMC_SPT");
  cyl->set_double_param("radius", radius + cemcthickness - 0.5);
  cyl->set_string_param("material", "SS310");  // SS310 Stainless Steel
  cyl->set_double_param("thickness", 0.5 - no_overlapp);
  cyl->OverlapCheck(OverlapCheck);
  if (AbsorberActive) cyl->SetActive();
  g4Reco->registerSubsystem(cyl);

  // this is the z extend and outer radius of the support structure and therefore the z extend
  // and radius of the surrounding black holes
  double sptlen = PHG4Utils::GetLengthForRapidityCoverage(radius + cemcthickness);
  BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z, sptlen);
  BlackHoleGeometry::min_z = std::min(BlackHoleGeometry::min_z, -sptlen);
  BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, radius + cemcthickness);

  cemcthickness -= 0.5 + no_overlapp;

  int ilayer = 0;
  PHG4SpacalSubsystem *cemc;

  cemc = new PHG4SpacalSubsystem("CEMC", ilayer);

  cemc->set_int_param("virualize_fiber", 0);
  cemc->set_int_param("azimuthal_seg_visible", 1);
  cemc->set_int_param("construction_verbose", 0);
  cemc->Verbosity(0);
  if (Enable::XPLOAD)
  {
    cemc->UseCDB("CEMC_GEOMETRY");
  }
  else
  {
    cemc->UseCalibFiles(PHG4DetectorSubsystem::xml);
    cemc->SetCalibrationFileDir(string(getenv("CALIBRATIONROOT")) + string("/CEMC/Geometry_2023ProjTilted/"));
  }
  cemc->set_double_param("radius", radius);            // overwrite minimal radius
  cemc->set_double_param("thickness", cemcthickness);  // overwrite thickness

  cemc->SetActive();
  cemc->SuperDetector("CEMC");
  if (AbsorberActive) cemc->SetAbsorberActive();
  cemc->OverlapCheck(OverlapCheck);

  g4Reco->registerSubsystem(cemc);

  if (ilayer > G4CEMC::Max_cemc_layer)
  {
    cout << "layer discrepancy, current layer " << ilayer
         << " max cemc layer: " << G4CEMC::Max_cemc_layer << endl;
  }

  radius += cemcthickness;
  radius += no_overlapp;

  return radius;
}

void CEMC_Cells()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::CEMC_VERBOSITY);

  Fun4AllServer *se = Fun4AllServer::instance();

  if (G4CEMC::Cemc_spacal_configuration == PHG4CylinderGeom_Spacalv1::k1DProjectiveSpacal)
  {
    PHG4CylinderCellReco *cemc_cells = new PHG4CylinderCellReco("CEMCCYLCELLRECO");
    cemc_cells->Detector("CEMC");
    cemc_cells->Verbosity(verbosity);
    for (int i = G4CEMC::Min_cemc_layer; i <= G4CEMC::Max_cemc_layer; i++)
    {
      //          cemc_cells->etaphisize(i, 0.024, 0.024);
      const double radius = 95;
      cemc_cells->cellsize(i, 2 * M_PI / 256. * radius, 2 * M_PI / 256. * radius);
    }
    se->registerSubsystem(cemc_cells);
  }
  else if (G4CEMC::Cemc_spacal_configuration == PHG4CylinderGeom_Spacalv1::k2DProjectiveSpacal)
  {
    PHG4FullProjSpacalCellReco *cemc_cells = new PHG4FullProjSpacalCellReco("CEMCCYLCELLRECO");
    cemc_cells->Detector("CEMC");
    cemc_cells->Verbosity(verbosity);
    cemc_cells->get_light_collection_model().load_data_file(
        string(getenv("CALIBRATIONROOT")) + string("/CEMC/LightCollection/Prototype3Module.xml"),
        "data_grid_light_guide_efficiency", "data_grid_fiber_trans");
    se->registerSubsystem(cemc_cells);
  }
  else
  {
    cout << "G4_CEmc_Spacal.C::CEmc - Fatal Error - unrecognized SPACAL configuration #"
         << G4CEMC::Cemc_spacal_configuration << ". Force exiting..." << endl;
    gSystem->Exit(-1);
    return;
  }

  return;
}

void CEMC_Towers()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::CEMC_VERBOSITY);

  Fun4AllServer *se = Fun4AllServer::instance();

  RawTowerBuilder *TowerBuilder = new RawTowerBuilder("EmcRawTowerBuilder");
  TowerBuilder->Detector("CEMC");
  TowerBuilder->set_sim_tower_node_prefix("SIM");
  TowerBuilder->Verbosity(verbosity);
  se->registerSubsystem(TowerBuilder);

  double sampling_fraction = 1;
  //      sampling_fraction = 0.02244; //from production: /gpfs02/phenix/prod/sPHENIX/preCDR/pro.1-beta.3/single_particle/spacal2d/zerofield/G4Hits_sPHENIX_e-_eta0_8GeV.root
  //    sampling_fraction = 2.36081e-02;  //from production: /gpfs02/phenix/prod/sPHENIX/preCDR/pro.1-beta.5/single_particle/spacal2d/zerofield/G4Hits_sPHENIX_e-_eta0_8GeV.root
  //    sampling_fraction = 1.90951e-02; // 2017 Tilt porjective SPACAL, 8 GeV photon, eta = 0.3 - 0.4
  sampling_fraction = 2e-02;                 // 2017 Tilt porjective SPACAL, tower-by-tower calibration
  const double photoelectron_per_GeV = 500;  //500 photon per total GeV deposition

  RawTowerDigitizer *TowerDigitizer = new RawTowerDigitizer("EmcRawTowerDigitizer");
  TowerDigitizer->Detector("CEMC");
  TowerDigitizer->Verbosity(verbosity);
  TowerDigitizer->set_digi_algorithm(G4CEMC::TowerDigi);
  TowerDigitizer->set_variable_pedestal(true);  //read ped central and width from calibrations file comment next 2 lines if true
                                                //  TowerDigitizer->set_pedstal_central_ADC(0);
                                                //  TowerDigitizer->set_pedstal_width_ADC(8);  // eRD1 test beam setting
  TowerDigitizer->set_photonelec_ADC(1);        //not simulating ADC discretization error
  TowerDigitizer->set_photonelec_yield_visible_GeV(photoelectron_per_GeV / sampling_fraction);
  TowerDigitizer->set_variable_zero_suppression(true);  //read zs values from calibrations file comment next line if true
                                                        //  TowerDigitizer->set_zero_suppression_ADC(16);  // eRD1 test beam setting
  if (Enable::XPLOAD)
  {
    TowerDigitizer->GetParameters().ReadFromCDB("EMCTOWERCALIB");
  }
  else
  {
    TowerDigitizer->GetParameters().ReadFromFile("CEMC", "xml", 0, 0,
                                                 string(getenv("CALIBRATIONROOT")) + string("/CEMC/TowerCalibCombinedParams_2020/"));  // calibration database
  }
  se->registerSubsystem(TowerDigitizer);

  RawTowerCalibration *TowerCalibration = new RawTowerCalibration("EmcRawTowerCalibration");
  TowerCalibration->Detector("CEMC");
  TowerCalibration->Verbosity(verbosity);

  if (G4CEMC::TowerDigi == RawTowerDigitizer::kNo_digitization)
  {
    // just use sampling fraction set previously
    TowerCalibration->set_calib_const_GeV_ADC(1.0 / sampling_fraction);
  }
  else
  {
    TowerCalibration->set_calib_algorithm(RawTowerCalibration::kTower_by_tower_calibration);
    if (Enable::XPLOAD)
    {
      TowerCalibration->GetCalibrationParameters().ReadFromCDB("EMCTOWERCALIB");
    }
    else
    {
      TowerCalibration->GetCalibrationParameters().ReadFromFile("CEMC", "xml", 0, 0,
                                                                string(getenv("CALIBRATIONROOT")) + string("/CEMC/TowerCalibCombinedParams_2020/"));  // calibration database
    }
    TowerCalibration->set_variable_GeV_ADC(true);  //read GeV per ADC from calibrations file comment next line if true
    //    TowerCalibration->set_calib_const_GeV_ADC(1. / photoelectron_per_GeV / 0.9715);                                                             // overall energy scale based on 4-GeV photon simulations
    TowerCalibration->set_variable_pedestal(true);  //read pedestals from calibrations file comment next line if true
    //    TowerCalibration->set_pedstal_ADC(0);
  }
  se->registerSubsystem(TowerCalibration);

  return;
}

void CEMC_Clusters()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::CEMC_VERBOSITY);

  Fun4AllServer *se = Fun4AllServer::instance();

  if (G4CEMC::Cemc_clusterizer == G4CEMC::kCemcTemplateClusterizer)
  {
    RawClusterBuilderTemplate *ClusterBuilder = new RawClusterBuilderTemplate("EmcRawClusterBuilderTemplate");
    ClusterBuilder->Detector("CEMC");
    ClusterBuilder->Verbosity(verbosity);
    ClusterBuilder->set_threshold_energy(0.030);  // This threshold should be the same as in CEMCprof_Thresh**.root file below
    std::string emc_prof = getenv("CALIBRATIONROOT");
    emc_prof += "/EmcProfile/CEMCprof_Thresh30MeV.root";
    ClusterBuilder->LoadProfile(emc_prof);
    //    ClusterBuilder->set_UseTowerInfo(0);
    se->registerSubsystem(ClusterBuilder);

    RawClusterBuilderTemplate *ClusterBuilder2 = new RawClusterBuilderTemplate("EmcRawClusterBuilderTemplate2");
    ClusterBuilder2->Detector("CEMC");
    ClusterBuilder2->Verbosity(verbosity);
    ClusterBuilder2->set_threshold_energy(0.030);  // This threshold should be the same as in CEMCprof_Thresh**.root file below
    std::string emc_prof2 = getenv("CALIBRATIONROOT");
    emc_prof2 += "/EmcProfile/CEMCprof_Thresh30MeV.root";
    ClusterBuilder2->LoadProfile(emc_prof2);
    ClusterBuilder2->set_UseTowerInfo(1);
    se->registerSubsystem(ClusterBuilder2);

  }
  else if (G4CEMC::Cemc_clusterizer == G4CEMC::kCemcGraphClusterizer)
  {
    RawClusterBuilderGraph *ClusterBuilder = new RawClusterBuilderGraph("EmcRawClusterBuilderGraph");
    ClusterBuilder->Detector("CEMC");
    ClusterBuilder->Verbosity(verbosity);
    se->registerSubsystem(ClusterBuilder);
  }
  else
  {
    cout << "CEMC_Clusters - unknown clusterizer setting!" << endl;
    exit(1);
  }

  RawClusterPositionCorrection *clusterCorrection = new RawClusterPositionCorrection("CEMC");
  if (Enable::XPLOAD)
  {
    clusterCorrection->Get_eclus_CalibrationParameters().ReadFromCDB("CEMCRECALIB");
    clusterCorrection->Get_ecore_CalibrationParameters().ReadFromCDB("CEMC_ECORE_RECALIB");
  }
  else
  {
    clusterCorrection->Get_eclus_CalibrationParameters().ReadFromFile("CEMC_RECALIB", "xml", 0, 0,
                                                                      //raw location
                                                                      string(getenv("CALIBRATIONROOT")) + string("/CEMC/PositionRecalibration_EMCal_9deg_tilt/"));
    clusterCorrection->Get_ecore_CalibrationParameters().ReadFromFile("CEMC_ECORE_RECALIB", "xml", 0, 0,
                                                                      //raw location
                                                                      string(getenv("CALIBRATIONROOT")) + string("/CEMC/PositionRecalibration_EMCal_9deg_tilt/"));
  }

  clusterCorrection->Verbosity(verbosity);
  se->registerSubsystem(clusterCorrection);


  RawClusterPositionCorrection *clusterCorrection2 = new RawClusterPositionCorrection("CEMC");

  clusterCorrection2->set_UseTowerInfo(1);
  if (Enable::XPLOAD)
  {
    clusterCorrection2->Get_eclus_CalibrationParameters().ReadFromCDB("CEMCRECALIB");
    clusterCorrection2->Get_ecore_CalibrationParameters().ReadFromCDB("CEMC_ECORE_RECALIB");
  }
  else
  {
    clusterCorrection2->Get_eclus_CalibrationParameters().ReadFromFile("CEMC_RECALIB", "xml", 0, 0,
                                                                      //raw location
                                                                      string(getenv("CALIBRATIONROOT")) + string("/CEMC/PositionRecalibration_EMCal_9deg_tilt/"));
    clusterCorrection2->Get_ecore_CalibrationParameters().ReadFromFile("CEMC_ECORE_RECALIB", "xml", 0, 0,
                                                                      //raw location
                                                                      string(getenv("CALIBRATIONROOT")) + string("/CEMC/PositionRecalibration_EMCal_9deg_tilt/"));
  }

  clusterCorrection2->Verbosity(verbosity);
  se->registerSubsystem(clusterCorrection2);


  return;
}
void CEMC_Eval(const std::string &outputfile)
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::CEMC_VERBOSITY);

  Fun4AllServer *se = Fun4AllServer::instance();

  CaloEvaluator *eval = new CaloEvaluator("CEMCEVALUATOR", "CEMC", outputfile);
  eval->Verbosity(verbosity);
  se->registerSubsystem(eval);

  return;
}

void CEMC_QA()
{
  int verbosity = std::max(Enable::QA_VERBOSITY, Enable::CEMC_VERBOSITY);

  Fun4AllServer *se = Fun4AllServer::instance();
  QAG4SimulationCalorimeter *qa = new QAG4SimulationCalorimeter("CEMC");
  qa->Verbosity(verbosity);
  se->registerSubsystem(qa);

  return;
}

#endif
