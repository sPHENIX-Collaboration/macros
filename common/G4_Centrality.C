#ifndef MACRO_G4CENTRALITY_C
#define MACRO_G4CENTRALITY_C

#include <GlobalVariables.C>

#include <g4centrality/PHG4CentralityReco.h>

#include <fun4all/Fun4AllServer.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libcentrality_io.so)
R__LOAD_LIBRARY(libg4centrality.so)

namespace Enable
{
  bool CENTRALITY = false;
  int CENTRALITY_VERBOSITY = 0;
}  // namespace Enable

void Centrality()
{
  int verbosity = max(Enable::VERBOSITY, Enable::CENTRALITY_VERBOSITY);
  //---------------
  // Fun4All server
  //---------------

  Fun4AllServer *se = Fun4AllServer::instance();

  PHG4CentralityReco *cent = new PHG4CentralityReco();
  cent->Verbosity(verbosity);
  if (Enable::XPLOAD)
  {
    cent->GetCalibrationParameters().ReadFromCDB("CENTRALITY");
  }
  else
  {
    cent->GetCalibrationParameters().ReadFromFile("centrality", "xml", 0, 0, string(getenv("CALIBRATIONROOT")) + string("/Centrality/"));
  }
  se->registerSubsystem( cent );
  
  return;
}
#endif
