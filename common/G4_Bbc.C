#ifndef MACRO_G4BBC_C
#define MACRO_G4BBC_C

#include <g4bbc/BbcVertexFastSimReco.h>

#include <fun4all/Fun4AllServer.h>

R__LOAD_LIBRARY(libg4bbc.so)

namespace Enable
{
  bool BBC = false;
}

namespace G4BBC
{
  double z_smearing = 0.;     // should be 6mm, temporary to 0 for TPC
  double t_smearing = 0.002;  // 20ps timing resolution
}  // namespace G4BBC

void BbcInit() {}

void Bbc_Reco(int verbosity = 0)
{
  //---------------
  // Fun4All server
  //---------------

  Fun4AllServer* se = Fun4AllServer::instance();

  BbcVertexFastSimReco* bbcvertex = new BbcVertexFastSimReco();
  bbcvertex->set_z_smearing(G4BBC::z_smearing);  // 6 mm, temporarily perfect for TPC initial vertexing
  bbcvertex->set_t_smearing(G4BBC::t_smearing);  // 20 ps
  se->registerSubsystem(bbcvertex);

  return;
}
#endif
