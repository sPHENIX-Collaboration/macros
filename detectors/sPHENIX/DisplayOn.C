#ifndef MACRO_DISPLAYON_C
#define MACRO_DISPLAYON_C

#include <g4main/PHG4Reco.h>

#include <fun4all/Fun4AllServer.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libg4testbench.so)

namespace Enable
{
  bool DISPLAY = false;
}

// This starts the QT based G4 gui which takes control
// when x'ed out it will return a pointer to PHG4Reco so
// the gui can be startrd again
PHG4Reco *QTGui()
{
  Fun4AllServer *se = Fun4AllServer::instance();
  PHG4Reco *g4 = (PHG4Reco *) se->getSubsysReco("PHG4RECO");
  g4->InitRun(se->topNode());
  g4->ApplyDisplayAction();
  g4->StartGui();
  return g4;
}

// stupid macro to turn on the geant4 display
// we ask Fun4All for a pointer to PHG4Reco
// using the ApplyCommand will start up the
// G4 cmd interpreter and graphics system
// the vis.mac contains the necessary commands to
// start up the visualization, the next event will
// be displayed. Do not execute this macro
// before PHG4Reco was registered with Fun4All
PHG4Reco *DisplayOn(const std::string &mac = "vis.mac")
{
  Fun4AllServer *se = Fun4AllServer::instance();
  PHG4Reco *g4 = (PHG4Reco *) se->getSubsysReco("PHG4RECO");
  g4->InitRun(se->topNode());
  g4->ApplyDisplayAction();
  std::string cmd =  "/control/execute " + mac;
  g4->ApplyCommand(cmd);
  return g4;
}
// print out the commands I always forget
void displaycmd()
{
  std::cout << "draw axis: " << std::endl;
  std::cout << " g4->ApplyCommand(\"/vis/scene/add/axes 0 0 0 50 cm\")" << std::endl;
  std::cout << "zoom" << std::endl;
  std::cout << " g4->ApplyCommand(\"/vis/viewer/zoom 1\")" << std::endl;
  std::cout << "viewpoint:" << std::endl;
  std::cout << " g4->ApplyCommand(\"/vis/viewer/set/viewpointThetaPhi 0 0\")" << std::endl;
  std::cout << "panTo:" << std::endl;
  std::cout << " g4->ApplyCommand(\"/vis/viewer/panTo 0 0 cm\")" << std::endl;
  std::cout << "print to eps:" << std::endl;
  std::cout << " g4->ApplyCommand(\"/vis/ogl/printEPS\")" << std::endl;
  std::cout << "set background color:" << std::endl;
  std::cout << " g4->ApplyCommand(\"/vis/viewer/set/background white\")" << std::endl;
  std::cout << "Overlap check:" << std::endl;
  std::cout << " g4->ApplyCommand(\"/geometry/test/run\")" << std::endl;
}

#endif
