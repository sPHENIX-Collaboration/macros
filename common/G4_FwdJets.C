#ifndef MACRO_G4FWDJETS_C
#define MACRO_G4FWDJETS_C

// #include <GlobalVariables.C>

#include <g4jets/ClusterJetInput.h>
#include <g4jets/FastJetAlgo.h>
#include <g4jets/JetReco.h>
#include <g4jets/TowerJetInput.h>
#include <g4jets/TruthJetInput.h>
#include <g4jets/TrackJetInput.h>

#include <g4eval/JetEvaluator.h>

#include <fun4all/Fun4AllServer.h>

R__LOAD_LIBRARY(libg4jets.so)
R__LOAD_LIBRARY(libg4eval.so)

namespace Enable
{
  bool FWDJETS = false;
  bool FWDJETS_EVAL = false;
  int FWDJETS_VERBOSITY = 0;
}  // namespace Enable

void Jet_FwdRecoInit() {}

void Jet_FwdReco()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::FWDJETS_VERBOSITY);

  Fun4AllServer *se = Fun4AllServer::instance();

  // truth particle level jets
  JetReco *truthjetreco = new JetReco("TRUTHJETRECO");
  truthjetreco->add_input(new TruthJetInput(Jet::PARTICLE));
  truthjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.5), "AntiKt_Truth_r05");
  truthjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.7), "AntiKt_Truth_r07");
  truthjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 1.0), "AntiKt_Truth_r10");
  truthjetreco->set_algo_node("ANTIKT");
  truthjetreco->set_input_node("TRUTH");
  truthjetreco->Verbosity(verbosity);
  se->registerSubsystem(truthjetreco);

  // tower jets
  JetReco *towerjetreco = new JetReco("TOWERJETRECO");
  towerjetreco->add_input(new TowerJetInput(Jet::FEMC_TOWER));
  towerjetreco->add_input(new TowerJetInput(Jet::FHCAL_TOWER));
  towerjetreco->add_input(new TowerJetInput(Jet::CEMC_TOWER));
  towerjetreco->add_input(new TowerJetInput(Jet::HCALIN_TOWER));
  towerjetreco->add_input(new TowerJetInput(Jet::HCALOUT_TOWER));
  towerjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.5), "AntiKt_Tower_r05");
  towerjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.7), "AntiKt_Tower_r07");
  towerjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 1.0), "AntiKt_Tower_r10");
  towerjetreco->set_algo_node("ANTIKT");
  towerjetreco->set_input_node("TOWER");
  towerjetreco->Verbosity(verbosity);
  se->registerSubsystem(towerjetreco);

  // cluster jets
  JetReco *clusterjetreco = new JetReco("CLUSTERJETRECO");
  clusterjetreco->add_input(new ClusterJetInput(Jet::FEMC_CLUSTER));
  clusterjetreco->add_input(new ClusterJetInput(Jet::FHCAL_CLUSTER));
  clusterjetreco->add_input(new ClusterJetInput(Jet::HCALOUT_CLUSTER));
  clusterjetreco->add_input(new ClusterJetInput(Jet::CEMC_CLUSTER));
  clusterjetreco->add_input(new ClusterJetInput(Jet::HCALIN_CLUSTER));
  clusterjetreco->add_input(new ClusterJetInput(Jet::HCALOUT_CLUSTER));
  clusterjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.5), "AntiKt_Cluster_r05");
  clusterjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.7), "AntiKt_Cluster_r07");
  clusterjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 1.0), "AntiKt_Cluster_r10");
  clusterjetreco->set_algo_node("ANTIKT");
  clusterjetreco->set_input_node("CLUSTER");
  clusterjetreco->Verbosity(verbosity);
  se->registerSubsystem(clusterjetreco);

  // tower jets
  JetReco *towerjetrecofwd = new JetReco("TOWERJETRECOFWD");
  towerjetrecofwd->add_input(new TowerJetInput(Jet::FEMC_TOWER));
  towerjetrecofwd->add_input(new TowerJetInput(Jet::FHCAL_TOWER));
  towerjetrecofwd->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.5), "AntiKt_TowerFwd_r05");
  towerjetrecofwd->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.7), "AntiKt_TowerFwd_r07");
  towerjetrecofwd->add_algo(new FastJetAlgo(Jet::ANTIKT, 1.0), "AntiKt_TowerFwd_r10");
  towerjetrecofwd->set_algo_node("ANTIKT");
  towerjetrecofwd->set_input_node("TOWER");
  towerjetrecofwd->Verbosity(verbosity);
  se->registerSubsystem(towerjetrecofwd);

  // cluster jets
  JetReco *clusterjetrecofwd = new JetReco("CLUSTERJETRECOFWD");
  clusterjetrecofwd->add_input(new ClusterJetInput(Jet::FEMC_CLUSTER));
  clusterjetrecofwd->add_input(new ClusterJetInput(Jet::FHCAL_CLUSTER));
  clusterjetrecofwd->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.5), "AntiKt_ClusterFwd_r05");
  clusterjetrecofwd->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.7), "AntiKt_ClusterFwd_r07");
  clusterjetrecofwd->add_algo(new FastJetAlgo(Jet::ANTIKT, 1.0), "AntiKt_ClusterFwd_r10");
  clusterjetrecofwd->set_algo_node("ANTIKT");
  clusterjetrecofwd->set_input_node("CLUSTER");
  clusterjetrecofwd->Verbosity(verbosity);
  se->registerSubsystem(clusterjetrecofwd);


  // // track jets
  JetReco *trackjetreco = new JetReco("TRACKJETRECO");
  trackjetreco->add_input(new TrackJetInput(Jet::TRACK, TRACKING::TrackNodeName));
  trackjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.5), "AntiKt_Track_r05");
  trackjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.7), "AntiKt_Track_r07");
  trackjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 1.0), "AntiKt_Track_r10");
  trackjetreco->set_algo_node("ANTIKT");
  trackjetreco->set_input_node("TRACK");
  trackjetreco->Verbosity(verbosity);
  se->registerSubsystem(trackjetreco);

  // // track jets
  JetReco *fulljetreco = new JetReco("FULLJETRECO");
  fulljetreco->add_input(new TrackJetInput(Jet::TRACK, TRACKING::TrackNodeName));
  fulljetreco->add_input(new ClusterJetInput(Jet::FEMC_CLUSTER));
  fulljetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.5), "AntiKt_Full_r05");
  fulljetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.7), "AntiKt_Full_r07");
  fulljetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.8), "AntiKt_Full_r08");
  fulljetreco->set_algo_node("ANTIKT");
  fulljetreco->set_input_node("FULL");
  fulljetreco->Verbosity(verbosity);
  se->registerSubsystem(fulljetreco);


  return;
}

void Jet_FwdEval(const std::string &outfilename = "g4fwdjets_eval.root")
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::FWDJETS_VERBOSITY);
  Fun4AllServer *se = Fun4AllServer::instance();

  JetEvaluator *evaltrk05 = new JetEvaluator("JETEVALUATORTRACK05","AntiKt_Track_r05",
                                        "AntiKt_Truth_r05", "g4fwdjets_track_05_eval.root");
  evaltrk05->Verbosity(verbosity);
  se->registerSubsystem(evaltrk05);

  JetEvaluator *evalfull05 = new JetEvaluator("JETEVALUATORFULL05","AntiKt_Full_r05",
                                        "AntiKt_Truth_r05", "g4fwdjets_full_05_eval.root");
  evalfull05->Verbosity(verbosity);
  se->registerSubsystem(evalfull05);

  JetEvaluator *evalt05 = new JetEvaluator("JETEVALUATORTOWER05","AntiKt_Tower_r05",
                                        "AntiKt_Truth_r05", "g4fwdjets_tower_05_eval.root");
  evalt05->Verbosity(verbosity);
  se->registerSubsystem(evalt05);
  JetEvaluator *evalt07 = new JetEvaluator("JETEVALUATORTOWER07","AntiKt_Tower_r07",
                                        "AntiKt_Truth_r07", "g4fwdjets_tower_07_eval.root");
  evalt07->Verbosity(verbosity);
  se->registerSubsystem(evalt07);
  JetEvaluator *evalt10 = new JetEvaluator("JETEVALUATORTOWER10","AntiKt_Tower_r10",
                                        "AntiKt_Truth_r10", "g4fwdjets_tower_10_eval.root");
  evalt10->Verbosity(verbosity);
  se->registerSubsystem(evalt10);

  JetEvaluator *evalc05 = new JetEvaluator("JETEVALUATORCLUSTER05", "AntiKt_Cluster_r05",
                                        "AntiKt_Truth_r05", "g4fwdjets_cluster_05_eval.root");
  evalc05->Verbosity(verbosity);
  se->registerSubsystem(evalc05);

  JetEvaluator *evalc07 = new JetEvaluator("JETEVALUATORCLUSTER07", "AntiKt_Cluster_r07",
                                        "AntiKt_Truth_r07", "g4fwdjets_cluster_07_eval.root");
  evalc07->Verbosity(verbosity);
  se->registerSubsystem(evalc07);

  JetEvaluator *evalc10 = new JetEvaluator("JETEVALUATORCLUSTER10", "AntiKt_Cluster_r10",
                                        "AntiKt_Truth_r10", "g4fwdjets_cluster_10_eval.root");
  evalc10->Verbosity(verbosity);
  se->registerSubsystem(evalc10);

  JetEvaluator *evaltfwd05 = new JetEvaluator("JETEVALUATORFWDTOWER05","AntiKt_TowerFwd_r05",
                                        "AntiKt_Truth_r05", "g4fwdjets_TowerFwd_05_eval.root");
  evaltfwd05->Verbosity(verbosity);
  se->registerSubsystem(evaltfwd05);
  JetEvaluator *evaltfwd07 = new JetEvaluator("JETEVALUATORFWDTOWER07","AntiKt_TowerFwd_r07",
                                        "AntiKt_Truth_r07", "g4fwdjets_TowerFwd_07_eval.root");
  evaltfwd07->Verbosity(verbosity);
  se->registerSubsystem(evaltfwd07);
  JetEvaluator *evaltfwd10 = new JetEvaluator("JETEVALUATORFWDTOWER10","AntiKt_TowerFwd_r10",
                                        "AntiKt_Truth_r10", "g4fwdjets_TowerFwd_10_eval.root");
  evaltfwd10->Verbosity(verbosity);
  se->registerSubsystem(evaltfwd10);

  JetEvaluator *evalcfwd05 = new JetEvaluator("JETEVALUATORFWDCLUSTER05", "AntiKt_ClusterFwd_r05",
                                        "AntiKt_Truth_r05", "g4fwdjets_ClusterFwd_05_eval.root");
  evalcfwd05->Verbosity(verbosity);
  se->registerSubsystem(evalcfwd05);

  JetEvaluator *evalcfwd07 = new JetEvaluator("JETEVALUATORFWDCLUSTER07", "AntiKt_ClusterFwd_r07",
                                        "AntiKt_Truth_r07", "g4fwdjets_ClusterFwd_07_eval.root");
  evalcfwd07->Verbosity(verbosity);
  se->registerSubsystem(evalcfwd07);

  JetEvaluator *evalcfwd10 = new JetEvaluator("JETEVALUATORFWDCLUSTER10", "AntiKt_ClusterFwd_r10",
                                        "AntiKt_Truth_r10", "g4fwdjets_ClusterFwd_10_eval.root");
  evalcfwd10->Verbosity(verbosity);
  se->registerSubsystem(evalcfwd10);

  // JetEvaluator *eval3 = new JetEvaluator("JETEVALUATOR3",
  //                                       "AntiKt_Full_r07",
  //                                       "AntiKt_Truth_r07",
  //                                       "g4fwdjets_full_07_eval.root");
  // eval3->Verbosity(verbosity);
  // se->registerSubsystem(eval3);

  return;
}
#endif
