// c++ includes --
#include <string>
#include <iostream>

// root includes --
#include <TSystem.h>

#include <mbd/MbdReco.h>
#include <epd/EpdReco.h>
#include <zdcinfo/ZdcReco.h>
#include <globalvertex/GlobalVertexReco.h>
#include <centrality/CentralityReco.h>
#include <calotrigger/MinimumBiasClassifier.h>

#include <ffamodules/CDBInterface.h>
#include <ffamodules/FlagHandler.h>

#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllBase.h>

#include <phool/recoConsts.h>

#include <sepd_eventplanecalib/sEPD_TreeGen.h>

R__LOAD_LIBRARY(libsepd_eventplanecalib.so)

void Fun4All_sEPD(const std::string &fname,
                  unsigned int runnumber,
                  const std::string &output = "test.root",
                  const std::string &output_tree = "tree.root",
                  int nEvents = 0,
                  const std::string &dbtag = "newcdbtag")
{
  std::cout << "########################" << std::endl;
  std::cout << "Run Parameters" << std::endl;
  std::cout << "input: " << fname << std::endl;
  std::cout << "Run: " << runnumber << std::endl;
  std::cout << "output: " << output << std::endl;
  std::cout << "output tree: " << output_tree << std::endl;
  std::cout << "nEvents: " << nEvents << std::endl;
  std::cout << "dbtag: " << dbtag << std::endl;
  std::cout << "########################" << std::endl;

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(Fun4AllBase::VERBOSITY_QUIET);

  recoConsts *rc = recoConsts::instance();

  // conditions DB flags and timestamp
  rc->set_StringFlag("CDB_GLOBALTAG", dbtag);
  rc->set_uint64Flag("TIMESTAMP", runnumber);
  CDBInterface::instance()->Verbosity(Fun4AllBase::VERBOSITY_SOME);

  SubsysReco* flag = new FlagHandler();
  se->registerSubsystem(flag);

  // MBD Reconstruction
  SubsysReco* mbdreco = new MbdReco();
  se->registerSubsystem(mbdreco);

  // sEPD Reconstruction--Calib Info
  SubsysReco* epdreco = new EpdReco();
  se->registerSubsystem(epdreco);

  // ZDC Reconstruction--Calib Info
  ZdcReco* zdcreco = new ZdcReco();
  zdcreco->set_zdc1_cut(0.0);
  zdcreco->set_zdc2_cut(0.0);
  se->registerSubsystem(zdcreco);

  // Official vertex storage
  SubsysReco* gvertex = new GlobalVertexReco();
  se->registerSubsystem(gvertex);

  // Minimum Bias Classifier
  MinimumBiasClassifier* mb = new MinimumBiasClassifier();
  mb->Verbosity(Fun4AllBase::VERBOSITY_QUIET);
  se->registerSubsystem(mb);

  // Centrality
  CentralityReco* cent = new CentralityReco();
  se->registerSubsystem(cent);

  // sEPD Tree Gen
  sEPD_TreeGen* sepd_gen = new sEPD_TreeGen();
  sepd_gen->set_filename(output);
  sepd_gen->set_tree_filename(output_tree);
  sepd_gen->Verbosity(Fun4AllBase::VERBOSITY_MORE);
  se->registerSubsystem(sepd_gen);

  Fun4AllInputManager* In = new Fun4AllDstInputManager("in");
  In->AddListFile(fname);
  se->registerInputManager(In);

  se->Verbosity(Fun4AllBase::VERBOSITY_QUIET);
  se->run(nEvents);
  se->End();

  CDBInterface::instance()->Print();  // print used DB files
  se->PrintTimer();
  delete se;
  std::cout << "All done!" << std::endl;
  gSystem->Exit(0);
}
