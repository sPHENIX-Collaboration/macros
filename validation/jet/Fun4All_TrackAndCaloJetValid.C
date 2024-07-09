#ifndef FUN4ALL_TRACKANDCALOJETVALID_C
#define FUN4ALL_TRACKANDCALOJETVALID_C

// c++ utilities
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <utility>
#include <vector>

// coresoftware headers
#include <ffamodules/CDBInterface.h>
#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllRunNodeInputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllUtils.h>
#include <g4centrality/PHG4CentralityReco.h>
#include <phool/recoConsts.h>
#include <jetbackground/DetermineTowerRho.h>
#include <jetbackground/TowerRho.h>
#include <jetqa/ConstituentsinJets.h>
#include <jetqa/JetKinematicCheck.h>
#include <jetqa/JetSeedCount.h>
#include <jetqa/StructureinJets.h>
#include <jetqa/TrksInJetQA.h>
#include <jetqa/RhosinEvent.h>
#include <qautils/QAHistManagerDef.h>

// f4a macros
#include <G4_ActsGeom.C>
#include <G4_Global.C>
#include <G4_Magnet.C>
#include <GlobalVariables.C>
#include <HIJetReco.C>
#include <Jet_QA.C>
#include <QA.C>
#include <Trkr_Clustering.C>

// load libraries
R__LOAD_LIBRARY(libg4centrality.so)
R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libjetbackground.so)
R__LOAD_LIBRARY(libjetqa.so)
R__LOAD_LIBRARY(libqautils.so)

// types for convenience
typedef std::vector<std::string> SVec;



// macro body -----------------------------------------------------------------

void Fun4All_TrackAndCaloJetValid(
  const int  verb   = 10,
  const int  nEvts  = 10,
  const int  nSkip  = 0,
  const SVec inputs = {
    "../input/dst_global.list",
    "../input/dst_mbd_epd.list",
    "../input/dst_calo_cluster.list",
    "../input/dst_trkr_hit.list",
    "../input/dst_trkr_cluster.list",
    "../input/dst_trackseeds.list",
    "../input/dst_tracks.list"
  },
  const std::string  qaBase = "HIST_JET_QA",
  std::optional<int> run    = std::nullopt
) {

  // turn on pp mode
  HIJETS::is_pp = true;

  // qa options
  JetQA::HasTracks = false;
  JetQA::DoInclusive = true;
  JetQA::DoTriggered = true;

  // initialize fun4all ------------------------------------------------------

  // announce start of macro
  std::cout << "\n -------- OwO -- Starting Jet QA Macro -- OwO -------- \n "
            << "              [Using Track-and-Calo Jets]\n"
            << std::endl;

  // grab instances f4a, the cdb, etc.
  Fun4AllServer* se = Fun4AllServer::instance();
  CDBInterface*  cb = CDBInterface::instance();
  recoConsts*    rc = recoConsts::instance();
  se -> Verbosity(verb);

  // turn on cdb
  Enable::CDB = true;

  // if needed, try to grab run from files
  int runNum = 6;
  int segNum = 0;
  if (!run.has_value()) {

    // grab first file from first input list
    ifstream    firstList(inputs.at(0));
    std::string firstFile("");
    std::getline(firstList, firstFile);

    // now extract the run and segment numbers...
    std::pair<int, int> runAndSeg = Fun4AllUtils::GetRunSegment(firstFile);
    runNum = runAndSeg.first;
    segNum = runAndSeg.second;
  } else {
    runNum = run.value();
  }

  // set cdb tags
  rc -> set_StringFlag("CDB_GLOBALTAG", "ProdA_2023");
  rc -> set_uint64Flag("TIMESTAMP", runNum);

  // get url of geo file
  const std::string inGeoFile = cb -> getUrl("Tracking_Geometry");

  // register dst input managers
  for (size_t iInput = 0; iInput < inputs.size(); ++iInput) {
    Fun4AllDstInputManager* inManager = new Fun4AllDstInputManager("InputManager" + std::to_string(iInput));
    inManager -> AddListFile(inputs[iInput], 1);
    se        -> registerInputManager(inManager);
  }

  // register geometry manager
  Fun4AllRunNodeInputManager* geoManager = new Fun4AllRunNodeInputManager("GeometryManager");
  geoManager -> AddFile(inGeoFile.data());
  se         -> registerInputManager(geoManager);

  // initialize acts geometry
  G4TPC::tpc_drift_velocity_reco = (8.0 / 1000) * 107.0 / 105.0;
  G4MAGNET::magfield = "0.01";
  G4MAGNET::magfield_rescale = 1;
  ACTSGEOM::ActsGeomInit();

  // register & run necessary reconstruction ----------------------------------

  // do vertex & centrality reconstruction
  Global_Reco(); 
  if (!HIJETS::is_pp) {
    Centrality();
  }

  // do jet reconstruction & rho calculation
  HIJetReco();  
  DoRhoCalculation();

  // register qa modules ------------------------------------------------------

  // add generic jet QA
  CommonJetQA();

  // add track jet QA
  JetsWithTracksQA();

  // run modules and exit -----------------------------------------------------

  // run4all
  se -> run(nEvts);
  se -> skip(nSkip);
  se -> End();

  // create output file name
  std::string qaFileName = qaBase;
  qaFileName += "-" + std::to_string(runNum);
  qaFileName += "-" + std::to_string(segNum);
  qaFileName += ".root";

  // save qa output and exit
  QAHistManagerDef::saveQARootFile(qaFileName);
  delete se;

  // announce end and exit
  std::cout << "\n -------- UwU -- Finished Jet QA Macro -- UwU -------- \n " << std::endl;
  gSystem -> Exit(0);
  return;

}

#endif

// end ------------------------------------------------------------------------
