#ifndef FUN4ALL_EMCAL_C
#define FUN4ALL_EMCAL_C

#include <caloreco/CaloTowerCalib.h>
#include <caloreco/CaloTowerStatus.h>
#include <caloreco/RawClusterBuilderTemplate.h>
#include <caloreco/RawClusterLikelihoodProfile.h>
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

#include <calotrigger/MinimumBiasClassifier.h>
#include <centrality/CentralityInfo.h>

#include <cdbobjects/CDBTTree.h>  // for CDBTTree
#include <ffamodules/CDBInterface.h>
//#include <GlobalVariables.C>

#include <litecaloeval/LiteCaloEval.h>
#include <calib_emc_pi0/pi0EtaByEta.h>
#include <calotrigger/MinimumBiasClassifier.h>
#include <calotrigger/TriggerRunInfoReco.h>

#include <mbd/MbdReco.h>
#include <globalvertex/GlobalVertexReco.h>



R__LOAD_LIBRARY(libcdbobjects)
R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libcalo_reco.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libLiteCaloEvalTowSlope.so)
R__LOAD_LIBRARY(libcalibCaloEmc_pi0.so)
R__LOAD_LIBRARY(libcalotrigger.so)
R__LOAD_LIBRARY(libcentrality.so)
R__LOAD_LIBRARY(libzdcinfo.so)
R__LOAD_LIBRARY(libmbd.so)
R__LOAD_LIBRARY(libglobalvertex.so)


void createLocalEMCalCalibFile(const string fname, int runNumber);


void Fun4All_EMCal(int nevents = 1e1, const std::string &fname = "inputdata.txt",int iter = 2, const std::string &calib_fname="base/local_calib_copy.root")
{

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(0);
  CDBInterface::instance()->Verbosity(1);

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

  rc->set_StringFlag("CDB_GLOBALTAG", "ProdA_2024");
  rc->set_uint64Flag("TIMESTAMP",runnumber);// runnumber);

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

  /////////////////////
  // mbd/vertex
  // MBD/BBC Reconstruction
  MbdReco *mbdreco = new MbdReco();
  se->registerSubsystem(mbdreco);

  // Official vertex storage
  GlobalVertexReco *gvertex = new GlobalVertexReco();
  se->registerSubsystem(gvertex);



  /////////////////////
  // Geometry
  std::cout << "Adding Geometry file" << std::endl;
  Fun4AllInputManager *intrue2 = new Fun4AllRunNodeInputManager("DST_GEO");
  std::string geoLocation = CDBInterface::instance()->getUrl("calo_geo");
  intrue2->AddFile(geoLocation);
  se->registerInputManager(intrue2);


  ////////////////////
  // Calibrate towers
  CaloTowerStatus *statusEMC = new CaloTowerStatus("CEMCSTATUS");
  statusEMC->set_detector_type(CaloTowerDefs::CEMC);
  //statusEMC->set_doAbortNoHotMap();
  statusEMC->set_directURL_hotMap("/sphenix/u/bseidlitz/work/forChris/caloStatusCDB_y2/moreMaps/EMCalHotMap_new_2024p006-48837cdb.root");
  se->registerSubsystem(statusEMC);

  CaloTowerCalib *calibEMC = new CaloTowerCalib("CEMCCALIB");
  calibEMC->set_detector_type(CaloTowerDefs::CEMC);
  calibEMC->set_directURL(calib_fname.c_str());
  se->registerSubsystem(calibEMC);


  //////////////////
  // Clusters
  std::cout << "Building clusters" << std::endl;
  RawClusterBuilderTemplate *ClusterBuilder = new RawClusterBuilderTemplate("EmcRawClusterBuilderTemplate");
  ClusterBuilder->Detector("CEMC");
  ClusterBuilder->set_threshold_energy(0.07);  // for when using basic calibration
  std::string emc_prof = getenv("CALIBRATIONROOT");
  emc_prof += "/EmcProfile/CEMCprof_Thresh30MeV.root";
  ClusterBuilder->LoadProfile(emc_prof);
  ClusterBuilder->set_UseTowerInfo(1);  // to use towerinfo objects rather than old RawTower
  ClusterBuilder->setOutputClusterNodeName("CLUSTERINFO_CEMC");
  se->registerSubsystem(ClusterBuilder);

/*
  string clusternodename = "CLUSTERINFO_CEMC";
  // cluster new prob
  RawClusterLikelihoodProfile *ClusterProleBuilder = new RawClusterLikelihoodProfile("RawClusterLikelihoodProleGamma");
  ClusterProleBuilder->set_profile_filepath("/sphenix/user/jpark4/CDBfiles/EMCalProb/EMCal_ProfileLikelihoodD2_Thres70MeV.root"); // default set to single gamma
  ClusterProleBuilder->set_outputNodeName("CLUSTERINFO_CEMC");                                                                    // could keep the same name to overwrite with new prob. values
  ClusterProleBuilder->set_profile_dimension(3);                                                                                  // 5x5
  ClusterProleBuilder->set_min_cluster_e(1);
  //se->registerSubsystem(ClusterProleBuilder);
 */
  

  pi0EtaByEta *ca = new pi0EtaByEta("calomodulename", OutFile);
  const vector<int> triggerList = {10,12};
  //const vector<int> triggerList = {24,25,26};
  //ca->set_reqTrig(true,triggerList);
  ca->set_RunTowByTow(false); // to decide if we want to run tbt (default is true)
  ca->set_RunTBTCompactMode(true);
  ca->apply_vertex_cut(true);
  ca->set_pt1BaseClusCut(1.0);
  ca->set_pt2BaseClusCut(1.0);
  ca->set_NclusDeptFac(0.0);
  se->registerSubsystem(ca);

  se->run(nevents);
  se->End();
  se->PrintTimer();
  delete se;

  gSystem->Exec("echo DONE >> DONE.txt");
  gSystem->Exit(0);
}




void createLocalEMCalCalibFile(const string fname, int runNumber)
{
  string default_time_independent_calib = "cemc_pi0_twrSlope_v1_default";
  //string m_calibName = "cemc_pi0_twrSlope_v1";
  string m_calibName = "getdefault";

  string calibdir = CDBInterface::instance()->getUrl(m_calibName);
  string filePath;

  if (!calibdir.empty())
  {
    filePath = calibdir;
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
    std::cout << "No specific file for " << m_calibName << " found, using default calib " << default_time_independent_calib << std::endl;
  }

  TFile *f_cdb = new TFile(filePath.c_str());
  f_cdb->Cp(fname.c_str());
  f_cdb->Cp("initial_calib.root");

  std::cout << "created local Calib file for run " << runNumber << " named " << fname << std::endl;

  delete f_cdb;
}

#endif
