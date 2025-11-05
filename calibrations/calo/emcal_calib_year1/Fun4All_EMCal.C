#ifndef FUN4ALL_EMCAL_C
#define FUN4ALL_EMCAL_C

// c++ includes --
#include <fstream>
#include <iostream>
#include <string>
#include <memory>
#include <format>
#include <vector>

// root includes --
#include <TF1.h>
#include <TFile.h>
#include <TROOT.h>
#include <TSystem.h>

#include <caloreco/CaloTowerCalib.h>
#include <caloreco/CaloTowerStatus.h>
#include <caloreco/RawClusterBuilderTemplate.h>
#include <caloreco/RawClusterPositionCorrection.h>

#include <ffamodules/FlagHandler.h>
#include <ffamodules/HeadReco.h>
#include <ffamodules/SyncReco.h>

#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllRunNodeInputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllUtils.h>
#include <fun4all/SubsysReco.h>

#include <phool/recoConsts.h>
#include <phool/RunnumberRange.h>

#include <ffamodules/CDBInterface.h>

#include <calib_emc_pi0/pi0EtaByEta.h>
#include <litecaloeval/LiteCaloEval.h>

#include <globalvertex/GlobalVertexReco.h>
#include <mbd/MbdReco.h>
#include <mbd/MbdEvent.h>

R__LOAD_LIBRARY(libcdbobjects)
R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libcalo_reco.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libLiteCaloEvalTowSlope.so)
R__LOAD_LIBRARY(libcalibCaloEmc_pi0.so)
R__LOAD_LIBRARY(libglobalvertex.so)
R__LOAD_LIBRARY(libg4mbd.so)
R__LOAD_LIBRARY(libmbd_io.so)
R__LOAD_LIBRARY(libmbd.so)

void createLocalEMCalCalibFile(const std::string &fname, int runNumber);

void Fun4All_EMCal(int nevents = 1e2,
                   const std::string &fname = "inputdata.txt",
                   int iter = 0,
                   const std::string &calib_fname = "local_calib_copy.root",
                   const std::string &fieldname = "CEMC_calib_ADC_to_ETower")
{
  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(0);
  CDBInterface::instance()->Verbosity(1);

  recoConsts *rc = recoConsts::instance();

  std::ifstream file(fname);
  std::string first_file;
  getline(file, first_file);

  std::string m_fieldname = fieldname;

  //===============
  // conditions DB flags
  //===============
  std::pair<int, int> runseg = Fun4AllUtils::GetRunSegment(first_file);
  int runnumber = runseg.first;
  std::cout << "run number = " << runnumber << std::endl;

  rc->set_StringFlag("CDB_GLOBALTAG", "newcdbtag");
  rc->set_uint64Flag("TIMESTAMP", runnumber);  // runnumber);

  std::unique_ptr<Fun4AllInputManager> in = std::make_unique<Fun4AllDstInputManager>("DST_TOWERS");
  in->AddListFile(fname);
  se->registerInputManager(in.release());

  std::string filename = first_file.substr(first_file.find_last_of("/\\") + 1);
  std::string OutFile = std::format("OUTHIST_iter{}_{}", iter, filename);

  if (iter == 0)
  {
    createLocalEMCalCalibFile(calib_fname, runnumber);
    std::cout << "creating " << calib_fname << " and exiting" << std::endl;
    gSystem->Exit(0);
  }

  /////////////////////
  // mbd/vertex
  // MBD/BBC Reconstruction
  std::unique_ptr<MbdReco> mbdreco = std::make_unique<MbdReco>();
  se->registerSubsystem(mbdreco.release());

  // Official vertex storage
  std::unique_ptr<GlobalVertexReco> gvertex = std::make_unique<GlobalVertexReco>();
  se->registerSubsystem(gvertex.release());

  /////////////////////
  // Geometry
  std::cout << "Adding Geometry file" << std::endl;
  std::unique_ptr<Fun4AllInputManager> intrue2 = std::make_unique<Fun4AllRunNodeInputManager>("DST_GEO");
  std::string geoLocation = CDBInterface::instance()->getUrl("calo_geo");
  intrue2->AddFile(geoLocation);
  se->registerInputManager(intrue2.release());

  ////////////////////
  // Calibrate towers
  std::unique_ptr<CaloTowerStatus> statusEMC = std::make_unique<CaloTowerStatus>("CEMCSTATUS");
  statusEMC->set_detector_type(CaloTowerDefs::CEMC);
  // statusEMC->set_doAbortNoHotMap();
  statusEMC->set_directURL_hotMap("/sphenix/u/bseidlitz/work/forChris/caloStatusCDB_y2/moreMaps/EMCalHotMap_new_2024p006-48837cdb.root");
  se->registerSubsystem(statusEMC.release());

  std::unique_ptr<CaloTowerCalib> calibEMC = std::make_unique<CaloTowerCalib>("CEMCCALIB");
  calibEMC->set_detector_type(CaloTowerDefs::CEMC);
  calibEMC->set_directURL(calib_fname);
  calibEMC->setFieldName(fieldname);
  se->registerSubsystem(calibEMC.release());

  //////////////////
  // Clusters
  std::cout << "Building clusters" << std::endl;
  std::unique_ptr<RawClusterBuilderTemplate> ClusterBuilder = std::make_unique<RawClusterBuilderTemplate>("EmcRawClusterBuilderTemplate");
  ClusterBuilder->Detector("CEMC");
  ClusterBuilder->set_threshold_energy(0.07);  // for when using basic calibration
  std::string emc_prof = getenv("CALIBRATIONROOT");
  emc_prof += "/EmcProfile/CEMCprof_Thresh30MeV.root";
  ClusterBuilder->LoadProfile(emc_prof);
  ClusterBuilder->set_UseTowerInfo(1);  // to use towerinfo objects rather than old RawTower
  ClusterBuilder->setOutputClusterNodeName("CLUSTERINFO_CEMC");
  ClusterBuilder->set_UseAltZVertex(1);
  se->registerSubsystem(ClusterBuilder.release());

  ///////////////////
  // analysis modules
  if (iter == 1 || iter == 2 || iter == 3)
  {
    std::unique_ptr<LiteCaloEval> eval7e = std::make_unique<LiteCaloEval>("CEMCEVALUATOR2", "CEMC", OutFile);
    eval7e->CaloType(LiteCaloEval::CEMC);
    eval7e->set_reqMinBias(false);
    eval7e->setInputTowerNodeName("TOWERINFO_CALIB_CEMC");
    se->registerSubsystem(eval7e.release());
  }

  if (iter > 3)
  {
    std::unique_ptr<pi0EtaByEta> ca = std::make_unique<pi0EtaByEta>("calomodulename", OutFile);
    ca->set_timing_cut_width(16);  // does nothing currently
    ca->apply_vertex_cut(true);    // default
    ca->set_vertex_cut(20.);
    ca->set_pt1BaseClusCut(1.3);  // default
    ca->set_pt2BaseClusCut(0.7);  // default
    ca->set_NclusDeptFac(1.4);    // default
    ca->set_RunTowByTow(true);
    ca->set_reqTrig(false);
    ca->set_GlobalVertexType(GlobalVertex::MBD);
    ca->set_requireVertex(true);
    ca->set_calib_fieldname(m_fieldname);
    se->registerSubsystem(ca.release());
  }

  se->run(nevents);
  se->End();
  se->PrintTimer();
  delete se;

  std::cout << "All done!" << std::endl;
  gSystem->Exit(0);
}

void createLocalEMCalCalibFile(const std::string &fname, int runNumber)
{
  std::string default_time_independent_calib = (runNumber >= RunnumberRange::RUN3AUAU_FIRST) ? "CEMC_calib_ADC_to_ETower_default" : "cemc_pi0_twrSlope_v1_default";
  std::string m_calibName = "getdefault";

  std::string calibdir = CDBInterface::instance()->getUrl(m_calibName);
  std::string filePath;

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

  std::unique_ptr<TFile> f_cdb = std::make_unique<TFile>(filePath.c_str());
  f_cdb->Cp(fname.c_str());
  f_cdb->Cp("initial_calib.root");

  std::cout << "created local Calib file for run " << runNumber << " named " << fname << std::endl;
}
#endif

#ifndef __CINT__
int main(int argc, const char *const argv[])
{
  const std::vector<std::string> args(argv, argv + argc);

  if (args.size() > 6)
  {
    std::cout << "usage: " << args[0] << " [events] [fname] [iter] [calib_fname] [fieldname]" << std::endl;
    return 1;
  }

  int events = 1e2;
  std::string fname = "inputdata.txt";
  int iter = 0;
  std::string calib_fname = "local_calib_copy.root";
  std::string fieldname = "CEMC_calib_ADC_to_ETower";

  if (args.size() >= 2)
  {
    events = std::stoi(args[1]);
  }
  if (args.size() >= 3)
  {
    fname = args[2];
  }
  if (args.size() >= 4)
  {
    iter = std::stoi(args[3]);
  }
  if (args.size() >= 5)
  {
    calib_fname = args[4];
  }
  if (args.size() >= 6)
  {
    fieldname = args[5];
  }

  Fun4All_EMCal(events, fname, iter, calib_fname, fieldname);

  std::cout << "done" << std::endl;
  return 0;
}
#endif
