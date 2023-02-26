#ifndef MACRO_HIJETRECO_C
#define MACRO_HIJETRECO_C

#include <GlobalVariables.C>

#include <g4jets/FastJetAlgo.h>
#include <g4jets/JetReco.h>
#include <g4jets/TowerJetInput.h>
#include <g4jets/TruthJetInput.h>

#include <jetbackground/CopyAndSubtractJets.h>
#include <jetbackground/DetermineTowerBackground.h>
#include <jetbackground/FastJetAlgoSub.h>
#include <jetbackground/RetowerCEMC.h>
#include <jetbackground/SubtractTowers.h>
#include <jetbackground/SubtractTowersCS.h>

#include <fun4all/Fun4AllServer.h>

R__LOAD_LIBRARY(libg4jets.so)
R__LOAD_LIBRARY(libjetbackground.so)

namespace Enable
{
  bool HIJETS = false;
  int HIJETS_VERBOSITY = 0;
  bool HIJETS_MC = true;
  bool HIJETS_TRUTH = false;
}  // namespace Enable

namespace HIJETS
{
  bool do_flow = false; // should be set to true once the EPD event plane correction is implemented
  bool do_CS = false;
}  // namespace HIJETS


void HIJetReco()
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
 se->registerSubsystem(rcemc);


  JetReco *towerjetreco = new JetReco();
  towerjetreco->add_input(new TowerJetInput(Jet::CEMC_TOWERINFO_RETOWER));
  towerjetreco->add_input(new TowerJetInput(Jet::HCALIN_TOWERINFO));
  towerjetreco->add_input(new TowerJetInput(Jet::HCALOUT_TOWERINFO));
  towerjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.2), "AntiKt_TowerInfo_HIRecoSeedsRaw_r02");
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
  se->registerSubsystem(dtb);



  CopyAndSubtractJets *casj = new CopyAndSubtractJets();
  casj->SetFlowModulation(HIJETS::do_flow);
  casj->Verbosity(verbosity); 
  casj->set_towerinfo(true);
  se->registerSubsystem(casj);
  
  
  DetermineTowerBackground *dtb2 = new DetermineTowerBackground();
  dtb2->SetBackgroundOutputName("TowerInfoBackground_Sub2");
  dtb2->SetFlow(HIJETS::do_flow);
  dtb2->SetSeedType(1);
  dtb2->SetSeedJetPt(7);
  dtb2->Verbosity(verbosity); 
  dtb2->set_towerinfo(true);
  se->registerSubsystem(dtb2);
  

  SubtractTowers *st = new SubtractTowers();
  st->SetFlowModulation(HIJETS::do_flow);
  st->Verbosity(verbosity);
  st->set_towerinfo(true);
  se->registerSubsystem(st);
  
  towerjetreco = new JetReco();
  towerjetreco->add_input(new TowerJetInput(Jet::CEMC_TOWERINFO_SUB1));
  towerjetreco->add_input(new TowerJetInput(Jet::HCALIN_TOWERINFO_SUB1));
  towerjetreco->add_input(new TowerJetInput(Jet::HCALOUT_TOWERINFO_SUB1));
  towerjetreco->add_algo(new FastJetAlgoSub(Jet::ANTIKT, 0.2, 1), "AntiKt_Tower_r02_Sub1");
  towerjetreco->add_algo(new FastJetAlgoSub(Jet::ANTIKT, 0.3, 1), "AntiKt_Tower_r03_Sub1");
  towerjetreco->add_algo(new FastJetAlgoSub(Jet::ANTIKT, 0.4, 1), "AntiKt_Tower_r04_Sub1");
  towerjetreco->add_algo(new FastJetAlgoSub(Jet::ANTIKT, 0.5, 1), "AntiKt_Tower_r05_Sub1");
  towerjetreco->set_algo_node("ANTIKT");
  towerjetreco->set_input_node("TOWER");
  towerjetreco->Verbosity(verbosity);
  se->registerSubsystem(towerjetreco);

  return;

}
#endif
