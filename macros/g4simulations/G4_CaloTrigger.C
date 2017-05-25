
void CaloTrigger_Sim(int verbosity = 0) {
  
  //---------------
  // Load libraries
  //---------------

  gSystem->Load("libcalotrigger.so");

  //---------------
  // Fun4All server
  //---------------

  Fun4AllServer *se = Fun4AllServer::instance();

  CaloTriggerSim* calotriggersim = new CaloTriggerSim();
  calotriggersim->Verbosity( verbosity );
  se->registerSubsystem( calotriggersim );

  return;

}  
