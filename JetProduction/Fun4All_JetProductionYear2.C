#ifndef FUN4ALL_JETPRODUCTIONYEAR2_C
#define FUN4ALL_JETPRODUCTIONYEAR2_C


// c++ utilities
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <utility>
#include <vector>

// coresoftware headers
#include <ffamodules/CDBInterface.h>
#include <ffamodules/FlagHandler.h>
#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllRunNodeInputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllUtils.h>
#include <g4centrality/PHG4CentralityReco.h>
#include <phool/recoConsts.h>
#include <jetbackground/DetermineTowerRho.h>
#include <jetbackground/TowerRho.h>
#include <qautils/QAHistManagerDef.h>

// f4a macros
#include <G4_ActsGeom.C>
#include <G4_Centrality.C>
#include <G4_Global.C>
#include <G4_Magnet.C>
#include <GlobalVariables.C>
#include <HIJetReco.C>
#include <Jet_QA.C>
#include <QA.C>

// load libraries
R__LOAD_LIBRARY(libg4centrality.so)
R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libjetbackground.so)
R__LOAD_LIBRARY(libqautils.so)



// macro body -----------------------------------------------------------------

void Fun4All_JetProductionYear2(
  const int nEvents = 0,
  const std::string& infile = "DST_CALO_run2pp_new_2024p001-00042586-0000.root",
  const std::string& outfile = "DST_JET-00042586-0000.root",
  const std::string& outfile_hist = "HIST_JETQA-00042586-0000.root",
  const std::string& dbtag = "ProdA_2024"
) {

  // turn on/off DST output and/or QA
  Enable::DSTOUT = false;
  Enable::QA = true;

  // turn on pp mode
  HIJETS::is_pp = true;

  // qa options
  JetQA::HasTracks = false;
  JetQA::DoInclusive = true;
  JetQA::DoTriggered = true;

  // initialize F4A server
  Fun4AllServer* se = Fun4AllServer::instance();
  se -> Verbosity(1);

  // grab run and segment no.s
  pair<int, int> runseg = Fun4AllUtils::GetRunSegment(infile);
  int runnumber = runseg.first;

  // set up reconstruction constants, DB tag, timestamp
  recoConsts* rc = recoConsts::instance();
  rc -> set_StringFlag("CDB_GLOBALTAG", dbtag);
  rc -> set_uint64Flag("TIMESTAMP", runnumber);

  // connect to conditions database
  CDBInterface::instance()->Verbosity(1);

  // set up flag handler
  FlagHandler* flag = new FlagHandler();
  se -> registerSubsystem(flag);

  // read in input
  Fun4AllInputManager* in = new Fun4AllDstInputManager("in");
  in -> AddFile(infile);
  se -> registerInputManager(in);

  // do vertex & centrality reconstruction
  Global_Reco();
  if (!HIJETS::is_pp)
  {
    Centrality();
  }

  // do jet reconstruction & rho calculation
  HIJetReco();  

  // register modules necessary for QA
  if (Enable::QA)
  {
    DoRhoCalculation();
    Jet_QA();
  }

  // if needed, save DST output
  if (Enable::DSTOUT)
  {
    Fun4AllDstOutputManager* out = new Fun4AllDstOutputManager("DSTOUT", outfile);
    out -> StripNode("CEMCPackets");
    out -> StripNode("HCALPackets");
    out -> StripNode("ZDCPackets");
    out -> StripNode("SEPDPackets");
    out -> StripNode("MBDPackets");
    se -> registerOutputManager(out);
  }

  // run4all
  se -> run(nEvents);
  se -> End();

  // if needed, save QA output
  if (Enable::QA)
  {
    QAHistManagerDef::saveQARootFile(outfile_hist);
  }

  // print used DB files, time elapsed and delete server
  CDBInterface::instance() -> Print();
  se -> PrintTimer();
  delete se;

  // announce end and exit
  std::cout << "Jets are done!" << std::endl;
  gSystem -> Exit(0);

}

#endif

// end ------------------------------------------------------------------------
