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

  DetermineTowerBackground *dtb = new DetermineTowerBackground();
  dtb->SetBackgroundOutputName("TowerBackground_Sub1");
  dtb->SetSeedType( 1 );
  dtb->Verbosity( verbosity );
  se->registerSubsystem( dtb );
  
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
