#ifndef MACRO_G4CALOTRIGGER_C
#define MACRO_G4CALOTRIGGER_C

#include <GlobalVariables.C>

#include <calotrigger/CaloTriggerSim.h>

#include <fun4all/Fun4AllServer.h>

R__LOAD_LIBRARY(libcalotrigger.so)

namespace Enable
{
  bool CALOTRIGGER = false;
  int CALOTRIGGER_VERBOSITY = 0;
}  // namespace Enable

void CaloTrigger_Sim()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::CALOTRIGGER_VERBOSITY);

  //---------------
  // Fun4All server
  //---------------

  Fun4AllServer* se = Fun4AllServer::instance();

  CaloTriggerSim* calotriggersim = new CaloTriggerSim();
  calotriggersim->Verbosity(verbosity);
  se->registerSubsystem(calotriggersim);

  return;
}
#endif
