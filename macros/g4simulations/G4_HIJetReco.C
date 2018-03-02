void HIJetReco(int verbosity = 0) {
  
  //---------------
  // Load libraries
  //---------------

  gSystem->Load("libjetbackground.so");

  //---------------
  // Fun4All server
  //---------------

  Fun4AllServer *se = Fun4AllServer::instance();

  RetowerCEMC *rcemc = new RetowerCEMC();
  rcemc->Verbosity( verbosity );
  se->registerSubsystem( rcemc );

  JetReco *towerjetreco = new JetReco();
  towerjetreco->add_input(new TowerJetInput(Jet::CEMC_TOWER_RETOWER));
  towerjetreco->add_input(new TowerJetInput(Jet::HCALIN_TOWER));
  towerjetreco->add_input(new TowerJetInput(Jet::HCALOUT_TOWER));
  towerjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT,0.2),"AntiKt_Tower_HIRecoSeedsRaw_r02");
  towerjetreco->set_algo_node("ANTIKT");
  towerjetreco->set_input_node("TOWER");
  towerjetreco->Verbosity( verbosity );
  se->registerSubsystem(towerjetreco);

  DetermineTowerBackground *dtb = new DetermineTowerBackground();
  dtb->SetBackgroundOutputName("TowerBackground_Sub1");
  dtb->SetSeedType( 0 );
  dtb->Verbosity( verbosity );
  se->registerSubsystem( dtb );

  CopyAndSubtractJets *casj = new CopyAndSubtractJets();
  casj->Verbosity( verbosity );
  se->registerSubsystem( casj );

  DetermineTowerBackground *dtb2 = new DetermineTowerBackground();
  dtb2->SetBackgroundOutputName("TowerBackground_Sub2");
  dtb2->SetSeedType( 1 );
  dtb2->Verbosity( verbosity );
  se->registerSubsystem( dtb2 );
  
  SubtractTowers *st = new SubtractTowers();
  st->Verbosity( verbosity );
  se->registerSubsystem( st );

  JetReco *towerjetreco = new JetReco();
  towerjetreco->add_input(new TowerJetInput(Jet::CEMC_TOWER_SUB1));
  towerjetreco->add_input(new TowerJetInput(Jet::HCALIN_TOWER_SUB1));
  towerjetreco->add_input(new TowerJetInput(Jet::HCALOUT_TOWER_SUB1));
  towerjetreco->add_algo(new FastJetAlgoSub(Jet::ANTIKT,0.2,verbosity),"AntiKt_Tower_r02_Sub1");
  towerjetreco->add_algo(new FastJetAlgoSub(Jet::ANTIKT,0.3,verbosity),"AntiKt_Tower_r03_Sub1");
  towerjetreco->add_algo(new FastJetAlgoSub(Jet::ANTIKT,0.4,verbosity),"AntiKt_Tower_r04_Sub1");
  towerjetreco->set_algo_node("ANTIKT");
  towerjetreco->set_input_node("TOWER");
  towerjetreco->Verbosity( verbosity );
  se->registerSubsystem(towerjetreco);

  return;

}  
