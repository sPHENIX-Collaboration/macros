#ifndef MACRO_G4JETS_C
#define MACRO_G4JETS_C

#include <GlobalVariables.C>
#include <QA.C>

#include <g4jets/ClusterJetInput.h>
#include <g4jets/FastJetAlgo.h>
#include <g4jets/JetReco.h>
#include <g4jets/TowerJetInput.h>
#include <g4jets/TrackJetInput.h>
#include <g4jets/TruthJetInput.h>

#include <g4eval/JetEvaluator.h>
#include <qa_modules/QAG4SimulationJet.h>

#include <fun4all/Fun4AllServer.h>

R__LOAD_LIBRARY(libg4jets.so)
R__LOAD_LIBRARY(libg4eval.so)
R__LOAD_LIBRARY(libqa_modules.so)

namespace Enable
{
  bool JETS = false;
  bool JETS_EVAL = false;
  bool JETS_QA = false;
  int JETS_VERBOSITY = 0;
}  // namespace Enable

void JetInit() {}

void Jet_Reco()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::JETS_VERBOSITY);

  Fun4AllServer *se = Fun4AllServer::instance();

  // truth particle level jets
  JetReco *truthjetreco = new JetReco("TRUTHJETRECO");
  truthjetreco->add_input(new TruthJetInput(Jet::PARTICLE));
  truthjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.2), "AntiKt_Truth_r02");
  truthjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.3), "AntiKt_Truth_r03");
  truthjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.4), "AntiKt_Truth_r04");
  truthjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.5), "AntiKt_Truth_r05");
  truthjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.6), "AntiKt_Truth_r06");
  truthjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.7), "AntiKt_Truth_r07");
  truthjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.8), "AntiKt_Truth_r08");
  truthjetreco->set_algo_node("ANTIKT");
  truthjetreco->set_input_node("TRUTH");
  truthjetreco->Verbosity(verbosity);
  se->registerSubsystem(truthjetreco);

  // tower jets
  JetReco *towerjetreco = new JetReco("TOWERJETRECO");
  towerjetreco->add_input(new TowerJetInput(Jet::CEMC_TOWER));
  towerjetreco->add_input(new TowerJetInput(Jet::HCALIN_TOWER));
  towerjetreco->add_input(new TowerJetInput(Jet::HCALOUT_TOWER));
  towerjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.2), "AntiKt_Tower_r02");
  towerjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.3), "AntiKt_Tower_r03");
  towerjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.4), "AntiKt_Tower_r04");
  towerjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.5), "AntiKt_Tower_r05");
  towerjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.6), "AntiKt_Tower_r06");
  towerjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.7), "AntiKt_Tower_r07");
  towerjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.8), "AntiKt_Tower_r08");
  towerjetreco->set_algo_node("ANTIKT");
  towerjetreco->set_input_node("TOWER");
  towerjetreco->Verbosity(verbosity);
  se->registerSubsystem(towerjetreco);

  // cluster jets
  JetReco *clusterjetreco = new JetReco("CLUSTERJETRECO");
  clusterjetreco->add_input(new ClusterJetInput(Jet::CEMC_CLUSTER));
  clusterjetreco->add_input(new ClusterJetInput(Jet::HCALIN_CLUSTER));
  clusterjetreco->add_input(new ClusterJetInput(Jet::HCALOUT_CLUSTER));
  clusterjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.2), "AntiKt_Cluster_r02");
  clusterjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.3), "AntiKt_Cluster_r03");
  clusterjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.4), "AntiKt_Cluster_r04");
  clusterjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.5), "AntiKt_Cluster_r05");
  clusterjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.6), "AntiKt_Cluster_r06");
  clusterjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.7), "AntiKt_Cluster_r07");
  clusterjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.8), "AntiKt_Cluster_r08");
  clusterjetreco->set_algo_node("ANTIKT");
  clusterjetreco->set_input_node("CLUSTER");
  clusterjetreco->Verbosity(verbosity);
  se->registerSubsystem(clusterjetreco);

  // track jets
  JetReco *trackjetreco = new JetReco("TRACKJETRECO");
  trackjetreco->add_input(new TrackJetInput(Jet::TRACK, TRACKING::TrackNodeName));
  trackjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.2), "AntiKt_Track_r02");
  trackjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.3), "AntiKt_Track_r03");
  trackjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.4), "AntiKt_Track_r04");
  trackjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.5), "AntiKt_Track_r05");
  trackjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.6), "AntiKt_Track_r06");
  trackjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.7), "AntiKt_Track_r07");
  trackjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.8), "AntiKt_Track_r08");
  trackjetreco->set_algo_node("ANTIKT");
  trackjetreco->set_input_node("TRACK");
  trackjetreco->Verbosity(verbosity);
  se->registerSubsystem(trackjetreco);

  return;
}

void Jet_Eval(const std::string &outfilename = "g4jets_eval.root")
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::JETS_VERBOSITY);

  Fun4AllServer *se = Fun4AllServer::instance();

  JetEvaluator *eval = new JetEvaluator("JETEVALUATOR",
                                        "AntiKt_Tower_r03",
                                        "AntiKt_Truth_r03",
                                        outfilename);
  eval->Verbosity(verbosity);
  se->registerSubsystem(eval);

  return;
}


void Jet_QA()
{
  int verbosity = std::max(Enable::QA_VERBOSITY, Enable::JETS_VERBOSITY);

  Fun4AllServer *se = Fun4AllServer::instance();

  QAG4SimulationJet *calo_jet7 = new QAG4SimulationJet(
      "AntiKt_Truth_r07");
  calo_jet7->add_reco_jet("AntiKt_Tower_r07");
  calo_jet7->add_reco_jet("AntiKt_Cluster_r07");
  calo_jet7->add_reco_jet("AntiKt_Track_r07");
  calo_jet7->Verbosity(verbosity);
  se->registerSubsystem(calo_jet7);

  QAG4SimulationJet *calo_jet4 = new QAG4SimulationJet(
      "AntiKt_Truth_r04");
  calo_jet4->add_reco_jet("AntiKt_Tower_r04");
  calo_jet4->add_reco_jet("AntiKt_Cluster_r04");
  calo_jet4->add_reco_jet("AntiKt_Track_r04");
  calo_jet4->Verbosity(verbosity);
  se->registerSubsystem(calo_jet4);

  QAG4SimulationJet *calo_jet2 = new QAG4SimulationJet(
      "AntiKt_Truth_r02");
  calo_jet2->add_reco_jet("AntiKt_Tower_r02");
  calo_jet2->add_reco_jet("AntiKt_Cluster_r02");
  calo_jet2->add_reco_jet("AntiKt_Track_r02");
  calo_jet2->Verbosity(verbosity);
  se->registerSubsystem(calo_jet2);

  return;
}


#endif
