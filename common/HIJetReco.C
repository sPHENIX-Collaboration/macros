#ifndef MACRO_HIJETRECO_C
#define MACRO_HIJETRECO_C

#include <GlobalVariables.C>

#include <jetbase/FastJetAlgo.h>
#include <jetbase/JetReco.h>
#include <jetbase/TowerJetInput.h>
#include <jetbase/TrackJetInput.h>
#include <g4jets/TruthJetInput.h>

#include <jetbackground/CopyAndSubtractJets.h>
#include <jetbackground/DetermineTowerBackground.h>
#include <jetbackground/DetermineTowerRho.h>
#include <jetbackground/FastJetAlgoSub.h>
#include <jetbackground/RetowerCEMC.h>
#include <jetbackground/SubtractTowers.h>
#include <jetbackground/SubtractTowersCS.h>
#include <jetbackground/TowerRho.h>

#include <fun4all/Fun4AllServer.h>

R__LOAD_LIBRARY(libjetbase.so)
R__LOAD_LIBRARY(libg4jets.so)
R__LOAD_LIBRARY(libjetbackground.so)

namespace Enable
{
  bool HIJETS = false;
  int HIJETS_VERBOSITY = 0;
  bool HIJETS_MC = true;
  bool HIJETS_TRUTH = false;
  bool HIJETS_TOWER = true;   // make tower jets
  bool HIJETS_TRACK = false;  // make track jets
  bool HIJETS_PFLOW = false;  // make particle flow jets
}  // namespace Enable

namespace HIJETS
{
  bool do_flow = false; // should be set to true once the EPD event plane correction is implemented
  bool do_CS = false;
  bool is_pp = false;  // turn off functionality only relevant for nucleon collisions
  std::string tower_prefix = "TOWERINFO_CALIB";
}  // namespace HIJETS


// ----------------------------------------------------------------------------
//! Make jets out of subtracted towers
// ----------------------------------------------------------------------------
void MakeTowerJets()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::HIJETS_VERBOSITY);


  //---------------
  // Fun4All server
  //---------------
  Fun4AllServer *se = Fun4AllServer::instance();

  if (Enable::HIJETS_MC && Enable::HIJETS_TRUTH) 
    {
      JetReco *truthjetreco = new JetReco();
      TruthJetInput *tji = new TruthJetInput(Jet::PARTICLE);
      tji->add_embedding_flag(0);  // changes depending on signal vs. embedded
      truthjetreco->add_input(tji);
      truthjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.2), "AntiKt_Truth_r02");
      truthjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.3), "AntiKt_Truth_r03");
      truthjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.4), "AntiKt_Truth_r04");
      truthjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.5), "AntiKt_Truth_r05");
      truthjetreco->set_algo_node("ANTIKT");
      truthjetreco->set_input_node("TRUTH");
      truthjetreco->Verbosity(verbosity);
      se->registerSubsystem(truthjetreco);
    }
  
  RetowerCEMC *rcemc = new RetowerCEMC(); 
  rcemc->Verbosity(verbosity); 
  rcemc->set_towerinfo(true);
  rcemc->set_frac_cut(0.5); //fraction of retower that must be masked to mask the full retower
  rcemc->set_towerNodePrefix(HIJETS::tower_prefix);
  se->registerSubsystem(rcemc);


  JetReco *towerjetreco = new JetReco();
  towerjetreco->add_input(new TowerJetInput(Jet::CEMC_TOWERINFO_RETOWER,HIJETS::tower_prefix));
  towerjetreco->add_input(new TowerJetInput(Jet::HCALIN_TOWERINFO,HIJETS::tower_prefix));
  towerjetreco->add_input(new TowerJetInput(Jet::HCALOUT_TOWERINFO,HIJETS::tower_prefix));
  towerjetreco->add_algo(new FastJetAlgoSub(Jet::ANTIKT, 0.2), "AntiKt_TowerInfo_HIRecoSeedsRaw_r02");
  towerjetreco->set_algo_node("ANTIKT");
  towerjetreco->set_input_node("TOWER");
  towerjetreco->Verbosity(verbosity);
  se->registerSubsystem(towerjetreco);

  DetermineTowerBackground *dtb = new DetermineTowerBackground();
  dtb->SetBackgroundOutputName("TowerInfoBackground_Sub1");
  dtb->SetFlow(HIJETS::do_flow);
  dtb->SetSeedType(0);
  dtb->SetSeedJetD(3);
  dtb->set_towerinfo(true);
  dtb->Verbosity(verbosity); 
  dtb->set_towerNodePrefix(HIJETS::tower_prefix);
  se->registerSubsystem(dtb);



  CopyAndSubtractJets *casj = new CopyAndSubtractJets();
  casj->SetFlowModulation(HIJETS::do_flow);
  casj->Verbosity(verbosity); 
  casj->set_towerinfo(true);
  casj->set_towerNodePrefix(HIJETS::tower_prefix);
  se->registerSubsystem(casj);
  
  
  DetermineTowerBackground *dtb2 = new DetermineTowerBackground();
  dtb2->SetBackgroundOutputName("TowerInfoBackground_Sub2");
  dtb2->SetFlow(HIJETS::do_flow);
  dtb2->SetSeedType(1);
  dtb2->SetSeedJetPt(7);
  dtb2->Verbosity(verbosity); 
  dtb2->set_towerinfo(true);
  dtb2->set_towerNodePrefix(HIJETS::tower_prefix);
  se->registerSubsystem(dtb2);
  

  SubtractTowers *st = new SubtractTowers();
  st->SetFlowModulation(HIJETS::do_flow);
  st->Verbosity(verbosity);
  st->set_towerinfo(true);
  st->set_towerNodePrefix(HIJETS::tower_prefix);
  se->registerSubsystem(st);
  
  towerjetreco = new JetReco();
  towerjetreco->add_input(new TowerJetInput(Jet::CEMC_TOWERINFO_SUB1,HIJETS::tower_prefix));
  towerjetreco->add_input(new TowerJetInput(Jet::HCALIN_TOWERINFO_SUB1,HIJETS::tower_prefix));
  towerjetreco->add_input(new TowerJetInput(Jet::HCALOUT_TOWERINFO_SUB1,HIJETS::tower_prefix));
  towerjetreco->add_algo(new FastJetAlgoSub(Jet::ANTIKT, 0.2, verbosity), "AntiKt_Tower_r02_Sub1");
  towerjetreco->add_algo(new FastJetAlgoSub(Jet::ANTIKT, 0.3, verbosity), "AntiKt_Tower_r03_Sub1");
  towerjetreco->add_algo(new FastJetAlgoSub(Jet::ANTIKT, 0.4, verbosity), "AntiKt_Tower_r04_Sub1");
  towerjetreco->add_algo(new FastJetAlgoSub(Jet::ANTIKT, 0.5, verbosity), "AntiKt_Tower_r05_Sub1");
  towerjetreco->set_algo_node("ANTIKT");
  towerjetreco->set_input_node("TOWER");
  towerjetreco->Verbosity(verbosity);
  se->registerSubsystem(towerjetreco);

  return;

}


// ----------------------------------------------------------------------------
//! Make jets out of tracks
// ----------------------------------------------------------------------------
void MakeTrackJets()
{

  // set verbosity
  int verbosity = std::max(Enable::VERBOSITY, Enable::HIJETS_VERBOSITY);

  //---------------
  // Fun4All server
  //---------------
  Fun4AllServer *se = Fun4AllServer::instance();

  // for simulation
  if (Enable::HIJETS_MC && Enable::HIJETS_TRUTH)
  {
    // configure truth jet input for charged particles
    TruthJetInput *ctji = new TruthJetInput(Jet::SRC::CHARGED_PARTICLE);
    ctji->add_embedding_flag(0);  // changes depending on signal vs. embedded

    // book jet reconstruction routines on charged truth particles
    JetReco *chargedtruthjetreco = new JetReco();
    chargedtruthjetreco->add_input(ctji);
    chargedtruthjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.2), "AntiKt_ChargedTruth_r02");
    chargedtruthjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.3), "AntiKt_ChargedTruth_r03");
    chargedtruthjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.4), "AntiKt_ChargedTruth_r04");
    chargedtruthjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.5), "AntiKt_ChargedTruth_r05");
    chargedtruthjetreco->set_algo_node("ANTIKT");
    chargedtruthjetreco->set_input_node("TRUTH");
    chargedtruthjetreco->Verbosity(verbosity);
    se->registerSubsystem(chargedtruthjetreco);
  }

  // book jet reconstruction routines on tracks
  JetReco* trackjetreco = new JetReco();
  trackjetreco->add_input(new TrackJetInput(Jet::SRC::TRACK));
  trackjetreco->add_algo(new FastJetAlgo(Jet::ALGO::ANTIKT, 0.2), "AntiKt_Track_r02");
  trackjetreco->add_algo(new FastJetAlgo(Jet::ALGO::ANTIKT, 0.3), "AntiKt_Track_r03");
  trackjetreco->add_algo(new FastJetAlgo(Jet::ALGO::ANTIKT, 0.4), "AntiKt_Track_r04");
  trackjetreco->add_algo(new FastJetAlgo(Jet::ALGO::ANTIKT, 0.5), "AntiKt_Track_r05");
  trackjetreco->set_algo_node("ANTIKT");
  trackjetreco->set_input_node("TRACK");
  trackjetreco->Verbosity(verbosity);
  se->registerSubsystem(trackjetreco);

  // exit back to HIJetReco()
  return;

}

// ----------------------------------------------------------------------------
//! Run jet reconstruction
// ----------------------------------------------------------------------------
void HIJetReco()
{

  // run approriate jet reconstruction routines
  if (Enable::HIJETS_TOWER) MakeTowerJets();
  if (Enable::HIJETS_TRACK) MakeTrackJets();
  /* TODO add pflow here */

}


// ----------------------------------------------------------------------------
//! Determine rho from tower input to jet reco (necessary for jet QA)
// ----------------------------------------------------------------------------
void DoRhoCalculation()
{

  // set verbosity
  int verbosity = std::max(Enable::VERBOSITY, Enable::HIJETS_VERBOSITY);

  //---------------
  // Fun4All server
  //---------------
  Fun4AllServer* se = Fun4AllServer::instance();

  // run rho calculations w/ default parameters
  DetermineTowerRho* towRhoCalc = new DetermineTowerRho();
  towRhoCalc -> add_method(TowerRho::Method::AREA);
  towRhoCalc -> add_method(TowerRho::Method::MULT);
  towRhoCalc -> add_tower_input( new TowerJetInput(Jet::CEMC_TOWERINFO_RETOWER) );
  towRhoCalc -> add_tower_input( new TowerJetInput(Jet::HCALIN_TOWERINFO) );
  towRhoCalc -> add_tower_input( new TowerJetInput(Jet::HCALOUT_TOWERINFO) );
  se -> registerSubsystem( towRhoCalc );

  // exit back to main macro
  return;

}

#endif
