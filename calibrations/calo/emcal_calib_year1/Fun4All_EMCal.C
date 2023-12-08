#ifndef FUN4ALL_EMCAL_C
#define FUN4ALL_EMCAL_C

#include <caloreco/CaloTowerCalib.h>
#include <caloreco/RawClusterBuilderTemplate.h>
#include <caloreco/RawClusterPositionCorrection.h>

#include <ffamodules/FlagHandler.h>
#include <ffamodules/HeadReco.h>
#include <ffamodules/SyncReco.h>

#include <fun4allraw/Fun4AllPrdfInputManager.h>

#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllRunNodeInputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllUtils.h>
#include <fun4all/SubsysReco.h>

#include <phool/recoConsts.h>

#include <cdbobjects/CDBTTree.h>  // for CDBTTree
#include <ffamodules/CDBInterface.h>
#include <GlobalVariables.C>

#include <litecaloeval/LiteCaloEval.h>
#include <calib_emc_pi0/pi0EtaByEta.h>

R__LOAD_LIBRARY(libcdbobjects)
R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libcalo_reco.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libLiteCaloEvalTowSlope.so)
R__LOAD_LIBRARY(libcalibCaloEmc_pi0.so)

void createLocalEMCalCalibFile(const string fname, int runNumber);




void Fun4All_EMCal(int nevents = 0, const std::string &fname = "inputdata.txt",int iter = 2, const std::string &calib_fname="local_calib_copy.root")
{
  bool enableMasking = 0;

  bool doFit = 0;
  bool doHistMake = 1;

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(0);

  // se->Verbosity(verbosity);
  recoConsts *rc = recoConsts::instance();

  ifstream file(fname);
  string first_file;
  getline(file, first_file);

  //===============
  // conditions DB flags
  //===============
  pair<int, int> runseg = Fun4AllUtils::GetRunSegment(first_file);
  int runnumber = runseg.first;
  cout << "run number = " << runnumber << endl;

  // global tag
  rc->set_StringFlag("CDB_GLOBALTAG", "MDC2");
  // // 64 bit timestamp
  rc->set_uint64Flag("TIMESTAMP", runnumber);

  Fun4AllInputManager *in = new Fun4AllDstInputManager("DST_TOWERS");
  in->AddListFile(fname);
  se->registerInputManager(in);

  std::string filename = first_file.substr(first_file.find_last_of("/\\") + 1);
  std::string OutFile = Form("OUTHIST_iter%d_%s",iter , filename.c_str());


  if (iter == 0)
  {
    createLocalEMCalCalibFile(calib_fname.c_str(), runnumber);
    cout << "creating " << calib_fname.c_str() << " and exiting" << endl;
    gSystem->Exit(0);
  }

  ////////////////////
  // Calibrate towers
  
  std::cout << "Calibrating EMCal" << std::endl;
  CaloTowerCalib *calibEMC = new CaloTowerCalib("CEMCCALIB");
  calibEMC->set_detector_type(CaloTowerDefs::CEMC);
  calibEMC->set_directURL(calib_fname.c_str());
  se->registerSubsystem(calibEMC);


  //////////////////
  // Clusters
  std::cout << "Building clusters" << std::endl;
  RawClusterBuilderTemplate *ClusterBuilder = new RawClusterBuilderTemplate("EmcRawClusterBuilderTemplate");
  ClusterBuilder->Detector("CEMC");
  ClusterBuilder->set_threshold_energy(0.20);  // for when using basic calibration
  std::string emc_prof = getenv("CALIBRATIONROOT");
  emc_prof += "/EmcProfile/CEMCprof_Thresh30MeV.root";
  ClusterBuilder->LoadProfile(emc_prof);
  ClusterBuilder->set_UseTowerInfo(1);  // to use towerinfo objects rather than old RawTower
  ClusterBuilder->setOutputClusterNodeName("CLUSTERINFO_CEMC2");
  se->registerSubsystem(ClusterBuilder);
  
/*
  std::cout << "Applying Position Dependent Correction" << std::endl;
  RawClusterPositionCorrection *clusterCorrection = new RawClusterPositionCorrection("CEMC");
  clusterCorrection->set_UseTowerInfo(1);  // to use towerinfo objects rather than old RawTower
  se->registerSubsystem(clusterCorrection);
*/

  ///////////////////
  // analysis modules
  if (iter==1){
    LiteCaloEval *eval7e = new LiteCaloEval("CEMCEVALUATOR2", "CEMC",OutFile);
    eval7e->CaloType(LiteCaloEval::CEMC);
    eval7e->setInputTowerNodeName("TOWERINFO_CALIB_CEMC");
    se->registerSubsystem(eval7e);
  }

  if (iter>1){
    pi0EtaByEta *ca = new pi0EtaByEta("calomodulename", OutFile);
    ca->set_timing_cut_width(16);
    ca->apply_vertex_cut(false);
    ca->set_vertex_cut(20.);
    se->registerSubsystem(ca);
  }

  se->run(nevents);
  se->End();
  se->PrintTimer();
  delete se;

  TFile* f_done_signal = new TFile("DONE.root","recreate");
  std::cout << "All done!" << std::endl;
  gSystem->Exit(0);
}




void createLocalEMCalCalibFile(const string fname, int runNumber)
{
  string default_time_independent_calib = "cemc_pi0_twrSlope_v1_default";
  string m_calibName = "cemc_pi0_twrSlope_v1";

  string calibdir = CDBInterface::instance()->getUrl(m_calibName);
  string filePath;

  if (!calibdir.empty())
  {
    filePath = calibdir;
    // cdbttree = new CDBTTree(calibdir);
  }
  else
  {
    calibdir = CDBInterface::instance()->getUrl(default_time_independent_calib);

    if (calibdir.empty())
    {
      std::cout << "No EMCal Calibration NOT even a default" << std::endl;
      exit(1);
    }
    filePath = calibdir;
    // cdbttree = new CDBTTree(calibdir);
    std::cout << "No specific file for " << m_calibName << " found, using default calib " << default_time_independent_calib << std::endl;
  }

  TFile *f_cdb = new TFile(filePath.c_str());
  f_cdb->Cp(fname.c_str());

  std::cout << "created local Calib file for run " << runNumber << " named " << fname << std::endl;

  delete f_cdb;
}

#endif
