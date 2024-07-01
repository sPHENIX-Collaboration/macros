#ifndef MACRO_JETQA_C
#define MACRO_JETQA_C

#include <fun4all/Fun4AllServer.h>
#include <jetqa/ConstituentsinJets.h>
#include <jetqa/JetKinematicCheck.h>
#include <jetqa/JetQADefs.h>
#include <jetqa/JetSeedCount.h>
#include <jetqa/RhosinEvent.h>
#include <jetqa/StructureinJets.h>
#include <jetqa/TrksInJetQA.h>

#include <HIJetReco.C>
#include <QA.C>

R__LOAD_LIBRARY(/sphenix/u/danderson/install/lib/libjetqa.so)



// ----------------------------------------------------------------------------
//! Namespace to collect various enums, default arguments, etc.
// ----------------------------------------------------------------------------
namespace JetQA
{

  /* TODO will go here */

}  // end JetQA namespace


// ----------------------------------------------------------------------------
//! QA for all jets, regardless of constituents
// ----------------------------------------------------------------------------
// TODO factorize so that you can specify
//   a trigger, and it will create all
//   the relevant modules
//
// TODO clean-up by
//   (1) moving all tags to vectors
//   (2) moving default cuts to a struct
void CommonJetQA()
{

  // FIXME select on max of verbosities
  Enable::QA_VERBOSITY = 10;

  // connect to f4a server
  Fun4AllServer* se = Fun4AllServer::instance();

  // initialize and register mass, eta, and pt qa module 
  JetKinematicCheck* kinematicQA = new JetKinematicCheck(
    "JetKinematicCheck",
    "AntiKt_Tower_r02_Sub1",
    "AntiKt_Tower_r03_Sub1",
    "AntiKt_Tower_r04_Sub1",
    "AntiKt_Tower_r05_Sub1"
  );
  kinematicQA -> setHistTag("AntiKt_Tower_AllTrig");
  kinematicQA -> setPtRange(10., 100.);
  kinematicQA -> setEtaRange(-1.1, 1.1);
  se          -> registerSubsystem(kinematicQA);

  // initialize and register jet seed counter qa module
  JetSeedCount* jetSeedQA = new JetSeedCount(
    "JetSeedCount",
    "AntiKt_Tower_r04_Sub1",
    "AntiKt_TowerInfo_HIRecoSeedsRaw_r02",
    "AntiKt_TowerInfo_HIRecoSeedsSub_r02"
  );
  jetSeedQA -> setHistTag("r04_AntiKt_Tower_Sub1_AllTrig");
  jetSeedQA -> setPtRange(5., 100.);
  jetSeedQA -> setEtaRange(-1.1, 1.1);
  jetSeedQA -> setWriteToOutputFile(false);
  jetSeedQA -> setPPMode(HIJETS::is_pp);
  se        -> registerSubsystem( jetSeedQA );

  // initialize and register constituent checks
  ConstituentsinJets* jetCstsQAR03Jet1 = new ConstituentsinJets(
    "ConstituentsInJetsR03Jet1",
    "AntiKt_Tower_r03_Sub1",
    "TowerInfoBackground_Sub1",
    "r03_AntiKt_Tower_Jet1"
  );
  jetCstsQAR03Jet1 -> setTrgToSelect(JetQADefs::GL1::MBDNSJet1);
  jetCstsQAR03Jet1 -> setPtRange(5., 100.);
  jetCstsQAR03Jet1 -> setEtaRange(-1.1, 1.1);
  se               -> registerSubsystem( jetCstsQAR03Jet1 );

  // initialize and register constituent checks
  ConstituentsinJets* jetCstsQA = new ConstituentsinJets(
    "ConstituentsInJetsR04Jet4",
    "AntiKt_Tower_r04_Sub1",
    "r04_AntiKt_Tower_Jet4"
  );
  jetCstsQA -> setTrgToSelect(JetQADefs::GL1::MBDNSJet4);
  jetCstsQA -> setPtRange(5., 100.);
  jetCstsQA -> setEtaRange(-1.1, 1.1);
  se        -> registerSubsystem( jetCstsQA );

  // initialize and register event-wise rho check
  RhosinEvent* evtRhoQA = new RhosinEvent("EventWiseRho", "AllTrig");
  evtRhoQA -> add_area_rho_node("TowerRho_AREA");
  evtRhoQA -> add_mult_rho_node("TowerRho_MULT");
  se       -> registerSubsystem( evtRhoQA );

}  // end 'CommonJetQA()'



// ----------------------------------------------------------------------------
//! QA for jets with tracks
// ----------------------------------------------------------------------------
void JetsWithTracksQA()
{

  // connect to f4a server
  Fun4AllServer* se = Fun4AllServer::instance();

  // intialize and register sum track vs. jet pt qa module
  StructureinJets* jetStructQA = new StructureinJets("AntiKt_Tower_r04_Sub1", "");
  jetStructQA -> writeToOutputFile(false);
  se          -> registerSubsystem(jetStructQA);

  // initialize and register track jet qa module
  TrksInJetQA* trksInJetQA = new TrksInJetQA("TrksInJetQANode_ClustJets");
  trksInJetQA -> SetHistSuffix("TowerJetSub1");
  trksInJetQA -> Configure(
    {
      .outMode     = TrksInJetQA::OutMode::QA,
      .verbose     = Enable::QA_VERBOSITY,
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

}  // end 'JetWithTracksQA()'

#endif

// end ---------------------------------------------
