#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include <fun4all/Fun4AllServer.h>
#include <calotrigger/CaloTriggerSim.h>
R__LOAD_LIBRARY(libcalotrigger.so)
#endif

void CaloTriggerInit() {}

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
