#pragma once

#include <g4vertex/GlobalVertexReco.h>
#include <g4vertex/GlobalVertexFastSimReco.h>

#include <fun4all/Fun4AllServer.h>

R__LOAD_LIBRARY(libg4vertex.so)

namespace Enable
{
  bool GLOBAL_RECO = false;
  bool GLOBAL_FASTSIM = false;
}

namespace G4GLOBAL
{
  double x_smearing = 0.01; // 100 um
  double y_smearing = 0.01; // 100 um
  double z_smearing = 0.015; // 150um
  double t_smearing = 0.002; // 20ps
}

void GlobalInit() {}

void Global_Reco(int verbosity = 0) {
  

  //---------------
  // Fun4All server
  //---------------

  Fun4AllServer *se = Fun4AllServer::instance();

  GlobalVertexReco* gblvertex = new GlobalVertexReco();
  se->registerSubsystem(gblvertex);

  return;
}  

void Global_FastSim(int verbosity = 0) {
  

  //---------------
  // Fun4All server
  //---------------

  Fun4AllServer *se = Fun4AllServer::instance();

  GlobalVertexFastSimReco* gblvertex = new GlobalVertexFastSimReco();
  gblvertex->set_x_smearing(G4GLOBAL::x_smearing);
  gblvertex->set_y_smearing(G4GLOBAL::y_smearing);
  gblvertex->set_z_smearing(G4GLOBAL::z_smearing);
  gblvertex->set_t_smearing(G4GLOBAL::t_smearing);
  se->registerSubsystem(gblvertex);

  return;
}  
