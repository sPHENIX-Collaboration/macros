#ifndef FUN4ALL_JETVALID_C
#define FUN4ALL_JETVALID_C

// c++ utilities
#include <regex>
#include <string>
#include <vector>

// coresoftware headers
#include <g4centrality/PHG4CentralityReco.h>
#include <ffamodules/CDBInterface.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <phool/recoConsts.h>
#include <QA/Jet/JetSeedCount.h>
#include <QA/Jet/StructureinJets.h>
#include <QA/Jet/TrksInJetQA.h>
#include <qautils/QAHistManagerDef.h>

// f4a macros
#include <G4_ActsGeom.C>
#include <G4_Global.C>
#include <G4_Magnet.C>
#include <GlobalVariables.C>
#include <HIJetReco.C>
#include <QA.C>
#include <Trkr_Clustering.C>

// load libraries
R__LOAD_LIBRARY(libg4centrality.so)
R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libjetqa.so)
R__LOAD_LIBRARY(libqautils.so)

// types for convenience
typedef std::vector<std::string> SVec;



// macro body -----------------------------------------------------------------

void Fun4all_JetValid(
  const int  verb   = 0,
  const int  run    = 0,
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
  }
) {

  // macro options -----------------------------------------------------------

  /* TODO will go here */

  // initialize fun4all ------------------------------------------------------

  // grab instances f4a, the cdb, etc.
  Fun4AllServer* se = Fun4AllServer::instance();
  CDBInterface*  cb = CDBInterface::instance();
  recoConsts*    rc = recoConsts::instance();
  se -> Verbosity(verb);
  rc -> Verbosity(verb);

  // turn on cdb
  Enable::CDB = true;

  // set cdb tags
  rc -> set_StringFlag("CDB_GLOBALTAG", "ProdA_2023");
  rc -> set_uint64Flag("TIMESTAMP", 6);

  // get url of geo file
  const std::string inGeoFile = cdb -> getUrl("Tracking_Geometry");

  // register input managers
  for (std::string input : inputs) {
    /* TODO will go here */
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

  Global_Reco(); 

  // initialize and register centrality calculator
  PHG4CentralityReco* centReco = new PHG4CentralityReco();
  centReco -> Verbosity(verb);
  centReco -> GetCalibrationParameters().ReadFromFile(
    "centrality",
    "xml",
    0,
    0,
    string(getenv("CALIBRATIONROOT")) + string("/Centrality/")
  );
  se -> registerSubsystem( centReco );

  HIJetReco();  

  // register qa modules ------------------------------------------------------

  // initialize and register jet seed counter qa module
  JetSeedCount* jetSeedQA = new JetSeedCount("AntiKt_Tower_r04_Sub1", "", "seed_test.root");
  jetSeedQA -> setPtRange(5, 100);
  jetSeedQA -> setEtaRange(-1.1, 1.1);
  se        -> registerSubsystem( jetSeedQA );

  // intialize and register sum track vs. jet pt qa module
  StructureinJets* jetStructQA = new StructureinJets("AntiKt_Tower_r04_Sub1", "trk_sum_test.root");
  se -> registerSubsystem(jetStructQA);  

  // initialize and register track jet qa module
  TrksInJetQA* trksInJetQA = new TrksInJetQA("TrksInJetQANode_ClustJets");
  trksInJetQA -> SetHistSuffix("TowerJetSub1");
  trksInJetQA -> Configure(
    {
      .outMode     = TrksInJetQA::OutMode::QA,
      .verbose     = verb,
      .doDebug     = false,
      .doInclusive = false,
      .doInJet     = true,
      .doHitQA     = false,
      .doClustQA   = true,
      .doTrackQA   = true,
      .doJetQA     = true,
      .rJet        = 0.4,
      .jetInNode   = "AntiKt_Tower_r04_Sub1"
    }
  );
  se -> registerSubsystem(trksInJetQA);

  // run modules and exit -----------------------------------------------------

  // run4all
  se -> run(nEvts);
  se -> skip(nSkip);
  se -> End();

  // save qa output and exit
  QAHistManagerDef::saveQARootFile("qa_test.root");
  delete se;

  // close and exit
  gSystem -> Exit(0);
  return;

}

#endif

// end ------------------------------------------------------------------------
