#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include <fun4all/Fun4AllServer.h>
#include <g4jets/FastJetAlgo.h>
#include <g4jets/JetReco.h>
#include <g4jets/TowerJetInput.h>
#include <g4eval/JetEvaluator.h>
R__LOAD_LIBRARY(libg4jets.so)
R__LOAD_LIBRARY(libg4eval.so)
#endif

void Jet_FwdRecoInit() {}

void Jet_FwdReco(int verbosity = 0) {

  gSystem->Load("libg4jets.so");
  
  Fun4AllServer *se = Fun4AllServer::instance();

  // truth particle level jets
  JetReco *truthjetreco = new JetReco();
  truthjetreco->add_input(new TruthJetInput(Jet::PARTICLE));
  //truthjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT,0.2),"AntiKt_Truth_r02");
  truthjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT,0.3),"AntiKt_Truth_r03");
  // truthjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT,0.4),"AntiKt_Truth_r04");
   truthjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT,0.5),"AntiKt_Truth_r05");
  // truthjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT,0.6),"AntiKt_Truth_r06");
   truthjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT,0.7),"AntiKt_Truth_r07");
  // truthjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT,0.8),"AntiKt_Truth_r08");
  truthjetreco->set_algo_node("ANTIKT");
  truthjetreco->set_input_node("TRUTH");
  truthjetreco->Verbosity(verbosity);
  se->registerSubsystem(truthjetreco);

  // tower jets
  JetReco *towerjetreco = new JetReco();
  towerjetreco->add_input(new TowerJetInput(Jet::FEMC_TOWER));
  towerjetreco->add_input(new TowerJetInput(Jet::FHCAL_TOWER));
  towerjetreco->add_input(new TowerJetInput(Jet::CEMC_TOWER));
  towerjetreco->add_input(new TowerJetInput(Jet::HCALIN_TOWER));
  towerjetreco->add_input(new TowerJetInput(Jet::HCALOUT_TOWER));
  // towerjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT,0.2),"AntiKt_Tower_r02");
  towerjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT,0.3),"AntiKt_Tower_r03");
  // towerjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT,0.4),"AntiKt_Tower_r04");
   towerjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT,0.5),"AntiKt_Tower_r05");
  // towerjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT,0.6),"AntiKt_Tower_r06");
   towerjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT,0.7),"AntiKt_Tower_r07");
  // towerjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT,0.8),"AntiKt_Tower_r08");
  towerjetreco->set_algo_node("ANTIKT");
  towerjetreco->set_input_node("TOWER");
  towerjetreco->Verbosity(verbosity);
  se->registerSubsystem(towerjetreco);

  return; 
}

void Jet_FwdEval(std::string outfilename = "g4fwdjets_eval.root",
	      int verbosity = 0) {

  gSystem->Load("libg4eval.so");
  
  Fun4AllServer *se = Fun4AllServer::instance();

  JetEvaluator* eval = new JetEvaluator("JETEVALUATOR",
   					"AntiKt_Tower_r05",
   					"AntiKt_Truth_r05",
   					outfilename);
  eval->Verbosity(verbosity);
  se->registerSubsystem(eval);
  
  return;
}
