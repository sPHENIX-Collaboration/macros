#ifndef MACRO_G4BBC_C
#define MACRO_G4BBC_C

#include <GlobalVariables.C>

#include <g4detectors/PHG4BbcSubsystem.h>

#include <g4bbc/BbcSimReco.h>
#include <g4bbc/BbcVertexFastSimReco.h>

#include <g4main/PHG4Reco.h>

#include <fun4all/Fun4AllServer.h>

R__LOAD_LIBRARY(libg4bbc.so)
R__LOAD_LIBRARY(libg4detectors.so)

namespace Enable
{
  bool BBC = false;          // Actual BBC detector
  bool BBC_SUPPORT = false;  // BBC Supports
  bool BBCRECO = false;      // run Bbc reconstruction
  bool BBCFAKE = false;      // Just generate fake bbc vtx, t0
  int BBC_VERBOSITY = 0;
}  // namespace Enable

namespace G4BBC
{
  double z_smearing = 0.;    // should be 6 mm, temporarily perfect for TPC initial vertexing
  double t_smearing = 0.02;  // 20ps timing resolution
}  // namespace G4BBC

void BbcInit()
{
  if (Enable::BBC && Enable::BBCFAKE)
  {
    cout << "Enable::BBC and Enable::BBCFAKE cannot be true at the same time" << endl;
    gSystem->Exit(1);
  }
  // Set boundary of tracked particles to include BBC (values in cm)
  if (Enable::BBC)
  {
    BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z, 300.);
    BlackHoleGeometry::min_z = std::min(BlackHoleGeometry::min_z, -300.);
    BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, 125.0);
  }
}

void Bbc(PHG4Reco* g4Reco)
{
  bool SupportActive = Enable::SUPPORT || Enable::BBC_SUPPORT;
  if (Enable::BBC)
  {
    PHG4BbcSubsystem* bbc = new PHG4BbcSubsystem("BBC");
    bbc->SuperDetector("BBC");
    bbc->OverlapCheck(Enable::OVERLAPCHECK);
    bbc->SetActive();
    if (SupportActive)
    {
      bbc->SetSupportActive(SupportActive);
    }
    g4Reco->registerSubsystem(bbc);
  }
  return;
}

void Bbc_Reco()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::BBC_VERBOSITY);

  //---------------
  // Fun4All server
  //---------------

  Fun4AllServer* se = Fun4AllServer::instance();

  if (Enable::BBCFAKE && Enable::BBCRECO)
  {
    cout << "Enable::BBCFAKE and Enable::BBCRECO cannot be enabled together" << endl;
    gSystem->Exit(1);
  }

  if (Enable::BBCFAKE)
  {
    if (verbosity > 0)
    {
      cout << "BBCFAKE: Using smeared vtx and t0 resolutions of "
           << G4BBC::z_smearing << " cm and " << G4BBC::t_smearing * 1000 << " ps" << endl;
    }
    BbcVertexFastSimReco* bbcvertex = new BbcVertexFastSimReco();
    bbcvertex->set_z_smearing(G4BBC::z_smearing);
    bbcvertex->set_t_smearing(G4BBC::t_smearing);
    bbcvertex->Verbosity(verbosity);

    se->registerSubsystem(bbcvertex);
  }
  if (Enable::BBCRECO)
  {
    BbcSimReco* bbcrec = new BbcSimReco();
    bbcrec->Verbosity(verbosity);
    se->registerSubsystem(bbcrec);
  }
  return;
}
#endif
