#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4allraw/Fun4AllPrdfInputManager.h>
#include <fun4all/Fun4AllDstInputManager.h>
#include <phool/recoConsts.h>
#include <fun4all/Fun4AllRunNodeInputManager.h>


// #include <calotowerbuilder/CaloTowerBuilder.h>
#include <caloreco/CaloTowerBuilder.h>
#include <caloreco/CaloWaveformProcessing.h>
#include <caloreco/CaloTowerCalib.h>
#include <caloreco/RawClusterBuilderTemplate.h>
#include <caloreco/RawClusterPositionCorrection.h>

#include <ffamodules/FlagHandler.h>
#include <ffamodules/HeadReco.h>
#include <ffamodules/SyncReco.h>
#include <ffamodules/CDBInterface.h>

#include <fun4all/Fun4AllDstOutputManager.h>
#include <caloreco/DeadHotMapLoader.h>

#include <caloreco/TowerInfoDeadHotMask.h>

#include <caloreco/RawClusterDeadHotMask.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libcalo_reco.so)
R__LOAD_LIBRARY(libffamodules.so)


#endif
void Fun4All_CaloProduction_PreQM23(string fname = "full-00007359-0000.prdf", const char *outfile = "testfile.root", int nEvents = 2)
{ 
   
  gSystem->Load("libg4dst");
  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(0);
  recoConsts *rc = recoConsts::instance();

  //===============
  // conditions DB flags
  //===============
  // ENABLE::CDB = true;
  // global tag
  rc->set_StringFlag("CDB_GLOBALTAG","ProdA_2023");
  // // 64 bit timestamp
  rc->set_uint64Flag("TIMESTAMP",stoi(fname.substr(fname.length()-15,5)));

  Fun4AllInputManager *dstIn = new Fun4AllDstInputManager("DSTIN");
  dstIn -> AddFile(fname.c_str());
  se -> registerInputManager(dstIn);
  
  std::cout << "Calibrating EMCal" << std::endl;
  CaloTowerCalib *calibEMC = new CaloTowerCalib("CEMCCALIB");
  calibEMC -> set_detector_type(CaloTowerCalib::CEMC);
  se -> registerSubsystem(calibEMC);
  
  std::cout << "Calibrating OHcal" << std::endl;

  CaloTowerCalib *calibOHCal = new CaloTowerCalib("HCALOUT");
  calibOHCal -> set_detector_type(CaloTowerCalib::HCALOUT);
  se -> registerSubsystem(calibOHCal);
  
  std::cout << "Calibrating IHcal" << std::endl;

  CaloTowerCalib *calibIHCal = new CaloTowerCalib("HCALIN");
  calibIHCal -> set_detector_type(CaloTowerCalib::HCALIN);
  se -> registerSubsystem(calibIHCal);

  std::cout << "Calibrating ZDC" << std::endl;

  CaloTowerCalib *calibZDC = new CaloTowerCalib("ZDC");
  calibZDC -> set_detector_type(CaloTowerCalib::ZDC);
  se -> registerSubsystem(calibZDC);

  std::cout << "Loading EMCal deadmap" << std::endl;

  DeadHotMapLoader *towerMapCemc = new DeadHotMapLoader("CEMC");
  towerMapCemc -> detector("CEMC");
  se->registerSubsystem(towerMapCemc);

  std::cout << "Loading ihcal deadmap" << std::endl;

  DeadHotMapLoader *towerMapHCalin = new DeadHotMapLoader("HCALIN");
  towerMapHCalin -> detector("HCALIN");
  se->registerSubsystem(towerMapHCalin);

  std::cout << "Loading ohcal deadmap" << std::endl;

  DeadHotMapLoader *towerMapHCalout = new DeadHotMapLoader("HCALOUT");
  towerMapHCalout -> detector("HCALOUT");
  se->registerSubsystem(towerMapHCalout);

  std::cout << "Loading cemc masker" << std::endl;

  TowerInfoDeadHotMask *towerMaskCemc = new TowerInfoDeadHotMask("CEMC");
  towerMaskCemc -> detector("CEMC");
  se->registerSubsystem(towerMaskCemc);

  std::cout << "Loading hcal maskers" << std::endl;

  TowerInfoDeadHotMask *towerMaskHCalin = new TowerInfoDeadHotMask("HCALIN");
  towerMaskHCalin -> detector("HCALIN");
  se->registerSubsystem(towerMaskHCalin);

  TowerInfoDeadHotMask *towerMaskHCalout = new TowerInfoDeadHotMask("HCALOUT");
  towerMaskHCalout -> detector("HCALOUT");
  se->registerSubsystem(towerMaskHCalout);

  std::cout << "Adding Geometry file" << std::endl;

  Fun4AllInputManager *intrue2 = new Fun4AllRunNodeInputManager("DST_GEO");
  CDBInterface *cdb = CDBInterface::instance();
  std::string geoLocation = cdb->getUrl("calo_geo");
  intrue2->AddFile(geoLocation);
  se->registerInputManager(intrue2);

  RawClusterBuilderTemplate *ClusterBuilder = new RawClusterBuilderTemplate("EmcRawClusterBuilderTemplate");
  ClusterBuilder->Detector("CEMC");
  ClusterBuilder->set_threshold_energy(0.030);//for when using basic calibration
  std::string emc_prof = getenv("CALIBRATIONROOT");
  emc_prof += "/EmcProfile/CEMCprof_Thresh30MeV.root";
  ClusterBuilder->LoadProfile(emc_prof);
  ClusterBuilder->set_UseTowerInfo(1); // to use towerinfo objects rather than old RawTower
  se->registerSubsystem(ClusterBuilder);
  
  RawClusterDeadHotMask *clusterMask = new RawClusterDeadHotMask("clusterMask");
  clusterMask->detector("CEMC");
  se->registerSubsystem(clusterMask);

  RawClusterPositionCorrection *clusterCorrection = new RawClusterPositionCorrection("CEMC");
  clusterCorrection->set_UseTowerInfo(1); // to use towerinfo objects rather than old RawTower
  se->registerSubsystem(clusterCorrection);

  //removing unofficial vertex code
  // MBD_VertexReco *ca = new MBD_VertexReco("name");
  // se->registerSubsystem(ca);

  Fun4AllDstOutputManager *out = new Fun4AllDstOutputManager("DSTOUT", outfile);
  out->StripNode("dud");
  se->registerOutputManager(out);

  se->run(nEvents);
  se->End();
  se->PrintTimer();
  std::cout << "All done!" << std::endl;
  gSystem->Exit(0);
}
