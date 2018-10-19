#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include "GlobalVariables.C"
#include <caloreco/RawClusterBuilderTemplate.h>
#include <caloreco/RawTowerCalibration.h>
#include <caloreco/RawClusterBuilderGraph.h>
#include <caloreco/RawClusterPositionCorrection.h>
#include <fun4all/Fun4AllServer.h>
#include <g4detectors/PHG4CylinderCellReco.h>
#include <g4detectors/PHG4CylinderGeom_Spacalv1.h>
#include <g4detectors/PHG4CylinderSubsystem.h>
#include <g4detectors/PHG4FullProjSpacalCellReco.h>
#include <g4detectors/PHG4SpacalSubsystem.h>
#include <g4calo/RawTowerBuilder.h>
#include <g4calo/RawTowerDigitizer.h>
#include <g4eval/CaloEvaluator.h>
#include <g4main/PHG4Reco.h>
double
CEmc_1DProjectiveSpacal(PHG4Reco *g4Reco, double radius, const int crossings, const int absorberactive = 0);
double
CEmc_2DProjectiveSpacal(PHG4Reco *g4Reco, double radius, const int crossings,
                        const int absorberactive = 0);
R__LOAD_LIBRARY(libcalo_reco.so)
R__LOAD_LIBRARY(libg4calo.so)
R__LOAD_LIBRARY(libg4detectors.so)
R__LOAD_LIBRARY(libg4eval.so)
#endif
int Min_cemc_layer = 1;
int Max_cemc_layer = 1;

// set a default value for SPACAL configuration
//  // 1D azimuthal projective SPACAL (fast)
//int Cemc_spacal_configuration = PHG4CylinderGeom_Spacalv1::k1DProjectiveSpacal;
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

#include <iostream>

// just a dummy parameter used by the tilted plate geom
void CEmcInit(const int nslats = 1)
{
  Min_cemc_layer = 1;
  Max_cemc_layer = 1;
}

//! EMCal main setup macro
double
CEmc(PHG4Reco *g4Reco, double radius, const int crossings,
     const int absorberactive = 0)
{
  if (Cemc_spacal_configuration == PHG4CylinderGeom_Spacalv1::k1DProjectiveSpacal)
  {
    return CEmc_1DProjectiveSpacal(/*PHG4Reco**/ g4Reco, /*double*/ radius, /*const int */
                                   crossings, /*const int*/ absorberactive);
  }
  else if (Cemc_spacal_configuration == PHG4CylinderGeom_Spacalv1::k2DProjectiveSpacal)
  {
    return CEmc_2DProjectiveSpacal(/*PHG4Reco**/ g4Reco, /*double*/ radius, /*const int */
                                   crossings, /*const int*/ absorberactive);
  }
  else
  {
    std::cout
        << "G4_CEmc_Spacal.C::CEmc - Fatal Error - unrecognized SPACAL configuration #"
        << Cemc_spacal_configuration << ". Force exiting..." << std::endl;
    exit(-1);
    return 0;
  }
}

//! EMCal setup macro - 1D azimuthal projective SPACAL
double
CEmc_1DProjectiveSpacal(PHG4Reco *g4Reco, double radius, const int crossings, const int absorberactive = 0)
{
  double emc_inner_radius = 95.;  // emc inner radius from engineering drawing
  double cemcthickness = 12.7;
  double emc_outer_radius = emc_inner_radius + cemcthickness;  // outer radius

  if (radius > emc_inner_radius)
  {
    cout << "inconsistency: pstof outer radius: " << radius
         << " larger than emc inner radius: " << emc_inner_radius
         << endl;
    gSystem->Exit(-1);
  }

  //---------------
  // Load libraries
  //---------------

  gSystem->Load("libg4detectors.so");
  gSystem->Load("libg4testbench.so");

  //  boundary check
  if (radius > emc_inner_radius - 1.5 - no_overlapp)
  {
    cout << "G4_CEmc_Spacal.C::CEmc() - expect radius < " << emc_inner_radius - 1.5 - no_overlapp << " to install SPACAL" << endl;
    exit(1);
  }
  radius = emc_inner_radius - 1.5 - no_overlapp;

  // 1.5cm thick teflon as an approximation for EMCAl light collection + electronics (10% X0 total estimated)
  PHG4CylinderSubsystem *cyl = new PHG4CylinderSubsystem("CEMC_ELECTRONICS", 0);
  cyl->SuperDetector("CEMC_ELECTRONICS");
  cyl->set_double_param("radius", radius);
  cyl->set_string_param("material", "G4_TEFLON");
  cyl->set_double_param("thickness", 1.5);
  if (absorberactive) cyl->SetActive();
  g4Reco->registerSubsystem(cyl);

  radius += 1.5;
  radius += no_overlapp;

  int ilayer = Min_cemc_layer;
  PHG4SpacalSubsystem *cemc;
  cemc = new PHG4SpacalSubsystem("CEMC", ilayer);
  cemc->set_double_param("radius",emc_inner_radius);
  cemc->set_double_param("thickness", cemcthickness); 

  cemc->SetActive();
  cemc->SuperDetector("CEMC");
  if (absorberactive) cemc->SetAbsorberActive();
  cemc->OverlapCheck(overlapcheck);

  g4Reco->registerSubsystem(cemc);

  if (ilayer > Max_cemc_layer)
  {
    cout << "layer discrepancy, current layer " << ilayer
         << " max cemc layer: " << Max_cemc_layer << endl;
  }

  radius += cemcthickness;
  radius += no_overlapp;

  // 0.5cm thick Stainless Steel as an approximation for EMCAl support system
  cyl = new PHG4CylinderSubsystem("CEMC_SPT", 0);
  cyl->SuperDetector("CEMC_SPT");
  cyl->set_double_param("radius", radius);
  cyl->set_string_param("material", "SS310");  // SS310 Stainless Steel
  cyl->set_double_param("thickness", 0.5);
  if (absorberactive)
    cyl->SetActive();
  g4Reco->registerSubsystem(cyl);

  radius += 0.5;
  radius += no_overlapp;

  return radius;
}

//! 2D full projective SPACAL
double
CEmc_2DProjectiveSpacal(PHG4Reco *g4Reco, double radius, const int crossings,
                        const int absorberactive = 0)
{
  double emc_inner_radius = 92;  // emc inner radius from engineering drawing
  double cemcthickness = 24.00000 - no_overlapp;

  //max radius is 116 cm;
  double emc_outer_radius = emc_inner_radius + cemcthickness;  // outer radius
  assert(emc_outer_radius < 116);

  if (radius > emc_inner_radius)
  {
    cout << "inconsistency: preshower radius+thickness: " << radius
         << " larger than emc inner radius: " << emc_inner_radius << endl;
    gSystem->Exit(-1);
  }

  //---------------
  // Load libraries
  //---------------

  gSystem->Load("libg4detectors.so");

  // the radii are only to determined the thickness of the cemc
  radius = emc_inner_radius;

  //---------------
  // Load libraries
  //---------------

  // 1.5cm thick teflon as an approximation for EMCAl light collection + electronics (10% X0 total estimated)
  PHG4CylinderSubsystem *cyl = new PHG4CylinderSubsystem("CEMC_ELECTRONICS", 0);
  cyl->set_double_param("radius", radius);
  cyl->set_string_param("material", "G4_TEFLON");
  cyl->set_double_param("thickness", 1.5 - no_overlapp);
  cyl->SuperDetector("CEMC_ELECTRONICS");
  cyl->OverlapCheck(overlapcheck);
  if (absorberactive) cyl->SetActive();
  g4Reco->registerSubsystem(cyl);

  radius += 1.5;
  cemcthickness -= 1.5 + no_overlapp;

  // 0.5cm thick Stainless Steel as an approximation for EMCAl support system
  cyl = new PHG4CylinderSubsystem("CEMC_SPT", 0);
  cyl->SuperDetector("CEMC_SPT");
  cyl->set_double_param("radius", radius + cemcthickness - 0.5);
  cyl->set_string_param("material", "SS310");  // SS310 Stainless Steel
  cyl->set_double_param("thickness", 0.5 - no_overlapp);
  cyl->OverlapCheck(overlapcheck);
  if (absorberactive)
    cyl->SetActive();
  g4Reco->registerSubsystem(cyl);

  cemcthickness -= 0.5 + no_overlapp;

  //---------------
  // Load libraries
  //---------------

  int ilayer = 0;
  PHG4SpacalSubsystem *cemc;

  const bool use_2015_design = false;
  if (use_2015_design)
  {
    cemc = new PHG4SpacalSubsystem("CEMC", ilayer);

    cemc->set_int_param("config", PHG4CylinderGeom_Spacalv1::kFullProjective_2DTaper_SameLengthFiberPerTower);
    cemc->set_double_param("radius", radius);            // overwrite minimal radius
    cemc->set_double_param("thickness", cemcthickness);  // overwrite thickness
    cemc->set_int_param("azimuthal_n_sec", 32);
    //    cemc->set_int_param("construction_verbose", 2);

    cemc->SetActive();
    cemc->SuperDetector("CEMC");
    if (absorberactive)
      cemc->SetAbsorberActive();
    cemc->OverlapCheck(overlapcheck);
  }

  else
  {
    cemc = new PHG4SpacalSubsystem("CEMC", ilayer);

    cemc->set_int_param("virualize_fiber", 0);
    cemc->set_int_param("azimuthal_seg_visible", 1);
    cemc->set_int_param("construction_verbose", 0);
    cemc->Verbosity(0);

    cemc->UseCalibFiles(PHG4DetectorSubsystem::xml);
    cemc->SetCalibrationFileDir(string(getenv("CALIBRATIONROOT")) + string("/CEMC/Geometry_2018ProjTilted/"));
    cemc->set_double_param("radius", radius);            // overwrite minimal radius
    cemc->set_double_param("thickness", cemcthickness);  // overwrite thickness

    cemc->SetActive();
    cemc->SuperDetector("CEMC");
    if (absorberactive)
      cemc->SetAbsorberActive();
    cemc->OverlapCheck(overlapcheck);
  }

  g4Reco->registerSubsystem(cemc);

  if (ilayer > Max_cemc_layer)
  {
    cout << "layer discrepancy, current layer " << ilayer
         << " max cemc layer: " << Max_cemc_layer << endl;
  }

  radius += cemcthickness;
  radius += no_overlapp;

  return radius;
}

void CEMC_Cells(int verbosity = 0)
{
  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4detectors.so");
  Fun4AllServer *se = Fun4AllServer::instance();

  if (Cemc_spacal_configuration == PHG4CylinderGeom_Spacalv1::k1DProjectiveSpacal)
  {
    PHG4CylinderCellReco *cemc_cells = new PHG4CylinderCellReco("CEMCCYLCELLRECO");
    cemc_cells->Detector("CEMC");
    cemc_cells->Verbosity(verbosity);
    for (int i = Min_cemc_layer; i <= Max_cemc_layer; i++)
    {
      //          cemc_cells->etaphisize(i, 0.024, 0.024);
      const double radius = 95;
      cemc_cells->cellsize(i, 2 * TMath::Pi() / 256. * radius, 2 * TMath::Pi() / 256. * radius);

    }
    se->registerSubsystem(cemc_cells);
  }
  else if (Cemc_spacal_configuration == PHG4CylinderGeom_Spacalv1::k2DProjectiveSpacal)
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
    std::cout
        << "G4_CEmc_Spacal.C::CEmc - Fatal Error - unrecognized SPACAL configuration #"
        << Cemc_spacal_configuration << ". Force exiting..." << std::endl;
    exit(-1);
    return;
  }

  return;
}

void CEMC_Towers(int verbosity = 0)
{
  gSystem->Load("libg4calo.so");
  gSystem->Load("libcalo_reco.so");
  Fun4AllServer *se = Fun4AllServer::instance();

  RawTowerBuilder *TowerBuilder = new RawTowerBuilder("EmcRawTowerBuilder");
  TowerBuilder->Detector("CEMC");
  TowerBuilder->set_sim_tower_node_prefix("SIM");
  TowerBuilder->Verbosity(verbosity);
  se->registerSubsystem(TowerBuilder);

  double sampling_fraction = 1;
  if (Cemc_spacal_configuration == PHG4CylinderGeom_Spacalv1::k1DProjectiveSpacal)
  {
    sampling_fraction = 0.0234335;  //from production:/gpfs02/phenix/prod/sPHENIX/preCDR/pro.1-beta.3/single_particle/spacal1d/zerofield/G4Hits_sPHENIX_e-_eta0_8GeV.root
  }
  else if (Cemc_spacal_configuration == PHG4CylinderGeom_Spacalv1::k2DProjectiveSpacal)
  {
    //      sampling_fraction = 0.02244; //from production: /gpfs02/phenix/prod/sPHENIX/preCDR/pro.1-beta.3/single_particle/spacal2d/zerofield/G4Hits_sPHENIX_e-_eta0_8GeV.root
//    sampling_fraction = 2.36081e-02;  //from production: /gpfs02/phenix/prod/sPHENIX/preCDR/pro.1-beta.5/single_particle/spacal2d/zerofield/G4Hits_sPHENIX_e-_eta0_8GeV.root
//    sampling_fraction = 1.90951e-02; // 2017 Tilt porjective SPACAL, 8 GeV photon, eta = 0.3 - 0.4
    sampling_fraction = 2e-02; // 2017 Tilt porjective SPACAL, tower-by-tower calibration
  }
  else
  {
    std::cout
        << "G4_CEmc_Spacal.C::CEMC_Towers - Fatal Error - unrecognized SPACAL configuration #"
        << Cemc_spacal_configuration << ". Force exiting..." << std::endl;
    exit(-1);
    return;
  }

  const double photoelectron_per_GeV = 500;  //500 photon per total GeV deposition

  RawTowerDigitizer *TowerDigitizer = new RawTowerDigitizer("EmcRawTowerDigitizer");
  TowerDigitizer->Detector("CEMC");
  TowerDigitizer->Verbosity(verbosity);
  TowerDigitizer->set_digi_algorithm(RawTowerDigitizer::kSimple_photon_digitization);
  TowerDigitizer->set_pedstal_central_ADC(0);
  TowerDigitizer->set_pedstal_width_ADC(8);  // eRD1 test beam setting
  TowerDigitizer->set_photonelec_ADC(1);     //not simulating ADC discretization error
  TowerDigitizer->set_photonelec_yield_visible_GeV(photoelectron_per_GeV / sampling_fraction);
  TowerDigitizer->set_zero_suppression_ADC(16);  // eRD1 test beam setting
  se->registerSubsystem(TowerDigitizer);

  if (Cemc_spacal_configuration == PHG4CylinderGeom_Spacalv1::k1DProjectiveSpacal)
  {
    RawTowerCalibration *TowerCalibration = new RawTowerCalibration("EmcRawTowerCalibration");
    TowerCalibration->Detector("CEMC");
    TowerCalibration->Verbosity(verbosity);
    TowerCalibration->set_calib_algorithm(RawTowerCalibration::kSimple_linear_calibration);
    TowerCalibration->set_calib_const_GeV_ADC(1. / photoelectron_per_GeV);
    TowerCalibration->set_pedstal_ADC(0);
    se->registerSubsystem(TowerCalibration);
  }
  else if (Cemc_spacal_configuration == PHG4CylinderGeom_Spacalv1::k2DProjectiveSpacal)
  {
    RawTowerCalibration *TowerCalibration = new RawTowerCalibration("EmcRawTowerCalibration");
    TowerCalibration->Detector("CEMC");
    TowerCalibration->Verbosity(verbosity);
    TowerCalibration->set_calib_algorithm(RawTowerCalibration::kTower_by_tower_calibration);
    TowerCalibration->GetCalibrationParameters().ReadFromFile("CEMC","xml",0,0,
        string(getenv("CALIBRATIONROOT")) + string("/CEMC/TowerCalib_2017ProjTilted/")); // calibration database
    TowerCalibration->set_calib_const_GeV_ADC(1. / photoelectron_per_GeV / 0.9715 ); // overall energy scale based on 4-GeV photon simulations
    TowerCalibration->set_pedstal_ADC(0);
    se->registerSubsystem(TowerCalibration);
  }
  else
  {
    std::cout
        << "G4_CEmc_Spacal.C::CEMC_Towers - Fatal Error - unrecognized SPACAL configuration #"
        << Cemc_spacal_configuration << ". Force exiting..." << std::endl;
    exit(-1);
    return;
  }

  return;
}

void CEMC_Clusters(int verbosity = 0)
{
  gSystem->Load("libcalo_reco.so");
  Fun4AllServer *se = Fun4AllServer::instance();

  if (Cemc_clusterizer == kCemcTemplateClusterizer)
  {
    RawClusterBuilderTemplate *ClusterBuilder = new RawClusterBuilderTemplate("EmcRawClusterBuilderTemplate");
    ClusterBuilder->Detector("CEMC");
    ClusterBuilder->Verbosity(verbosity);
    se->registerSubsystem(ClusterBuilder);
  }
  else if (Cemc_clusterizer == kCemcGraphClusterizer)
  {
    RawClusterBuilderGraph *ClusterBuilder = new RawClusterBuilderGraph("EmcRawClusterBuilderGraph");
    ClusterBuilder->Detector("CEMC");
    ClusterBuilder->Verbosity(verbosity);
    se->registerSubsystem(ClusterBuilder);
  }
  else
  {
    cout <<"CEMC_Clusters - unknown clusterizer setting!"<<endl;
    exit(1);
  }


  RawClusterPositionCorrection *clusterCorrection = new RawClusterPositionCorrection("CEMC");
 
  clusterCorrection->Get_eclus_CalibrationParameters().ReadFromFile("CEMC_RECALIB","xml",0,0,
							//raw location
							string(getenv("CALIBRATIONROOT"))+string("/CEMC/PositionRecalibration_EMCal_9deg_tilt/"));
				        
  clusterCorrection->Get_ecore_CalibrationParameters().ReadFromFile("CEMC_ECORE_RECALIB","xml",0,0,
						       //raw location
								    string(getenv("CALIBRATIONROOT"))+string("/CEMC/PositionRecalibration_EMCal_9deg_tilt/"));
				        
  clusterCorrection->Verbosity(verbosity);
  se->registerSubsystem(clusterCorrection);

  return;
}
void CEMC_Eval(std::string outputfile, int verbosity = 0)
{
  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4eval.so");
  Fun4AllServer *se = Fun4AllServer::instance();

  CaloEvaluator *eval = new CaloEvaluator("CEMCEVALUATOR", "CEMC", outputfile.c_str());
  eval->Verbosity(verbosity);
  se->registerSubsystem(eval);

  return;
}
