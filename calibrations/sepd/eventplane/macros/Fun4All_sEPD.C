// c++ includes --
#include <string>
#include <iostream>
#include <memory>

// root includes --
#include <TSystem.h>
#include <TROOT.h>
#include <TF1.h>

#include <mbd/MbdReco.h>
#include <mbd/MbdEvent.h>
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

#include <CDBUtils.C>

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

  std::unique_ptr<FlagHandler> flag = std::make_unique<FlagHandler>();
  se->registerSubsystem(flag.release());

  // MBD Reconstruction
  std::unique_ptr<MbdReco> mbdreco = std::make_unique<MbdReco>();
  se->registerSubsystem(mbdreco.release());

  // sEPD Reconstruction--Calib Info
  std::unique_ptr<EpdReco> epdreco = std::make_unique<EpdReco>();
  se->registerSubsystem(epdreco.release());

  // ZDC Reconstruction--Calib Info
  std::unique_ptr<ZdcReco> zdcreco = std::make_unique<ZdcReco>();
  zdcreco->set_zdc1_cut(0.0);
  zdcreco->set_zdc2_cut(0.0);
  se->registerSubsystem(zdcreco.release());

  // Official vertex storage
  std::unique_ptr<GlobalVertexReco> gvertex = std::make_unique<GlobalVertexReco>();
  gvertex->Verbosity(Fun4AllBase::VERBOSITY_QUIET);
  se->registerSubsystem(gvertex.release());

  // ###############################################################################
  // Temporary Setup until the default Centrality Scale CDB for Run 3 Au+Au is Fixed
  setGlobalTag(dbtag);

  uint64_t default_centrality_run = 54912;
  std::string cdb_centrality_scale = getCalibration("CentralityScale", default_centrality_run);
  // ###############################################################################

  // Minimum Bias Classifier
  std::unique_ptr<MinimumBiasClassifier> mb = std::make_unique<MinimumBiasClassifier>();
  mb->setOverwriteScale(cdb_centrality_scale); // Temporary
  mb->Verbosity(Fun4AllBase::VERBOSITY_QUIET);
  se->registerSubsystem(mb.release());

  // Centrality
  std::unique_ptr<CentralityReco> cent = std::make_unique<CentralityReco>();
  cent->setOverwriteScale(cdb_centrality_scale); // Temporary
  se->registerSubsystem(cent.release());

  // sEPD Tree Gen
  std::unique_ptr<sEPD_TreeGen> sepd_gen = std::make_unique<sEPD_TreeGen>();
  sepd_gen->set_filename(output);
  sepd_gen->set_tree_filename(output_tree);
  sepd_gen->Verbosity(Fun4AllBase::VERBOSITY_MORE);
  se->registerSubsystem(sepd_gen.release());

  std::unique_ptr<Fun4AllInputManager> In = std::make_unique<Fun4AllDstInputManager>("in");
  In->AddListFile(fname);
  se->registerInputManager(In.release());

  se->Verbosity(Fun4AllBase::VERBOSITY_QUIET);
  se->run(nEvents);
  se->End();

  CDBInterface::instance()->Print();  // print used DB files
  se->PrintTimer();
  delete se;
  std::cout << "All done!" << std::endl;
  gSystem->Exit(0);
}
