#ifndef CALO_CALIB_H
#define CALO_CALIB_H

#include <caloreco/CaloTowerCalib.h>
#include <caloreco/CaloTowerStatus.h>
#include <caloreco/RawClusterBuilderTemplate.h>
#include <caloreco/RawClusterDeadHotMask.h>
#include <caloreco/RawClusterPositionCorrection.h>

#include <ffamodules/CDBInterface.h>
#include <ffamodules/FlagHandler.h>
#include <phool/recoConsts.h>

#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllRunNodeInputManager.h>
#include <fun4all/Fun4AllServer.h>  // for Fun4AllServer

#include <TSystem.h>  // for gSystem

R__LOAD_LIBRARY(libcalo_reco.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libfun4allutils.so)

void Process_Calo_Calib()
{
  Fun4AllServer *se = Fun4AllServer::instance();
  recoConsts *rc = recoConsts::instance();

  /////////////////
  // set MC or data
  bool isSim = true;
  int data_sim_runnumber_thres = 1000;
  if (rc->get_uint64Flag("TIMESTAMP") > data_sim_runnumber_thres)
  {
    isSim = false;
  }
  std::cout << "Calo Calib uses runnumber " << rc->get_uint64Flag("TIMESTAMP") << std::endl;

  //////////////////////
  // Input geometry node
  std::cout << "Adding Geometry file" << std::endl;
  Fun4AllInputManager *ingeo = new Fun4AllRunNodeInputManager("DST_GEO");
  std::string geoLocation = CDBInterface::instance()->getUrl("calo_geo");
  ingeo->AddFile(geoLocation);
  se->registerInputManager(ingeo);

  //////////////////////////////
  // set statuses on raw towers
  std::cout << "status setters" << std::endl;
  CaloTowerStatus *statusEMC = new CaloTowerStatus("CEMCSTATUS");
  statusEMC->set_detector_type(CaloTowerDefs::CEMC);
  statusEMC->set_time_cut(1);
  // MC Towers Status
  if(isSim) {
    // Uses threshold of 50% for towers be considered frequently bad.
    std::string calibName_hotMap = "CEMC_hotTowers_status";
    /* Systematic options (to be used as needed). */
    /* Uses threshold of 40% for towers be considered frequently bad. */
    // std::string calibName_hotMap = "CEMC_hotTowers_status_40";

    /* Uses threshold of 60% for towers be considered frequently bad. */
    // std::string calibName_hotMap = "CEMC_hotTowers_status_60";

    std::string calibdir = CDBInterface::instance()->getUrl(calibName_hotMap);
    statusEMC->set_directURL_hotMap(calibdir);
  }
  se->registerSubsystem(statusEMC);

  CaloTowerStatus *statusHCalIn = new CaloTowerStatus("HCALINSTATUS");
  statusHCalIn->set_detector_type(CaloTowerDefs::HCALIN);
  statusHCalIn->set_time_cut(2);
  se->registerSubsystem(statusHCalIn);

  CaloTowerStatus *statusHCALOUT = new CaloTowerStatus("HCALOUTSTATUS");
  statusHCALOUT->set_detector_type(CaloTowerDefs::HCALOUT);
  statusHCALOUT->set_time_cut(2);
  se->registerSubsystem(statusHCALOUT);

  ////////////////////
  // Calibrate towers
  std::cout << "Calibrating EMCal" << std::endl;
  CaloTowerCalib *calibEMC = new CaloTowerCalib("CEMCCALIB");
  calibEMC->set_detector_type(CaloTowerDefs::CEMC);
  se->registerSubsystem(calibEMC);

  std::cout << "Calibrating OHcal" << std::endl;
  CaloTowerCalib *calibOHCal = new CaloTowerCalib("HCALOUT");
  calibOHCal->set_detector_type(CaloTowerDefs::HCALOUT);
  se->registerSubsystem(calibOHCal);

  std::cout << "Calibrating IHcal" << std::endl;
  CaloTowerCalib *calibIHCal = new CaloTowerCalib("HCALIN");
  calibIHCal->set_detector_type(CaloTowerDefs::HCALIN);
  se->registerSubsystem(calibIHCal);

  ////////////////
  // MC Calibration
  if (isSim && rc->get_uint64Flag("TIMESTAMP")<28) //in run28 and beyond we moved the MC calibration into the waveformsim module for data embedding
  {
    std::string MC_Calib = CDBInterface::instance()->getUrl("CEMC_MC_RECALIB");
    if (MC_Calib.empty())
    {
      std::cout << "No MC calibration found :( )" << std::endl;
      gSystem->Exit(0);
    }
    CaloTowerCalib *calibEMC_MC = new CaloTowerCalib("CEMCCALIB_MC");
    calibEMC_MC->set_detector_type(CaloTowerDefs::CEMC);
    calibEMC_MC->set_inputNodePrefix("TOWERINFO_CALIB_");
    calibEMC_MC->set_outputNodePrefix("TOWERINFO_CALIB_");
    calibEMC_MC->set_directURL(MC_Calib);
    calibEMC_MC->set_doCalibOnly(true);
    se->registerSubsystem(calibEMC_MC);
  }

  //////////////////
  // Clusters
  std::cout << "Building clusters" << std::endl;
  RawClusterBuilderTemplate *ClusterBuilder = new RawClusterBuilderTemplate("EmcRawClusterBuilderTemplate");
  ClusterBuilder->Detector("CEMC");
  ClusterBuilder->set_threshold_energy(0.070);  // for when using basic calibration
  std::string emc_prof = getenv("CALIBRATIONROOT");
  emc_prof += "/EmcProfile/CEMCprof_Thresh30MeV.root";
  ClusterBuilder->LoadProfile(emc_prof);
  ClusterBuilder->set_UseTowerInfo(1);  // to use towerinfo objects rather than old RawTower
  ClusterBuilder->set_UseAltZVertex(1); // Use MBD Vertex for vertex-based corrections
  se->registerSubsystem(ClusterBuilder);

  // currently NOT included!
  // std::cout << "Applying Position Dependent Correction" << std::endl;
  // RawClusterPositionCorrection *clusterCorrection = new RawClusterPositionCorrection("CEMC");
  // clusterCorrection->set_UseTowerInfo(1);  // to use towerinfo objects rather than old RawTower
  // se->registerSubsystem(clusterCorrection);
}

#endif
