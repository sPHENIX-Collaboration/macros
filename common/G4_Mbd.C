#ifndef MACRO_G4MBD_C
#define MACRO_G4MBD_C

#include <GlobalVariables.C>

#include <g4detectors/PHG4BbcSubsystem.h>

#include <g4mbd/MbdDigitization.h>
#include <g4mbd/MbdVertexFastSimReco.h>
#include <mbd/MbdReco.h>

#include <g4main/PHG4Reco.h>

#include <fun4all/Fun4AllServer.h>

R__LOAD_LIBRARY(libg4mbd.so)
R__LOAD_LIBRARY(libg4detectors.so)
R__LOAD_LIBRARY(libmbd.so)

namespace Enable
{
  bool MBD = false;          // Actual MBD detector
  bool MBD_SUPPORT = false;  // MBD Supports
  bool MBDRECO = false;      // run Mbd reconstruction
  bool MBDFAKE = false;      // Just generate fake mbd vtx, t0
  int MBD_VERBOSITY = 0;
}  // namespace Enable

namespace G4MBD
{
  double z_smearing = 0.;    // should be 6 mm, temporarily perfect for TPC initial vertexing
  double t_smearing = 0.02;  // 20ps timing resolution
}  // namespace G4MBD

void MbdInit()
{
  if (Enable::MBD && Enable::MBDFAKE)
  {
    cout << "Enable::MBD and Enable::MBDFAKE cannot be true at the same time" << endl;
    gSystem->Exit(1);
  }
  // Set boundary of tracked particles to include MBD (values in cm)
  if (Enable::MBD)
  {
    BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z, 300.);
    BlackHoleGeometry::min_z = std::min(BlackHoleGeometry::min_z, -300.);
    BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, 125.0);
  }
}

void Mbd(PHG4Reco* g4Reco)
{
  bool SupportActive = Enable::SUPPORT || Enable::MBD_SUPPORT;
  if (Enable::MBD)
  {
    PHG4BbcSubsystem* mbd = new PHG4BbcSubsystem("BBC");
    mbd->SuperDetector("BBC");
    mbd->OverlapCheck(Enable::OVERLAPCHECK);
    mbd->SetActive();
    if (SupportActive)
    {
      mbd->SetSupportActive(SupportActive);
    }
    g4Reco->registerSubsystem(mbd);
  }
  return;
}

void Mbd_Reco()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::MBD_VERBOSITY);

  //---------------
  // Fun4All server
  //---------------

  Fun4AllServer* se = Fun4AllServer::instance();

  if (Enable::MBDFAKE && Enable::MBDRECO)
  {
    cout << "Enable::MBDFAKE and Enable::MBDRECO cannot be enabled together" << endl;
    gSystem->Exit(1);
  }

  if (Enable::MBDFAKE)
  {
    if (verbosity > 0)
    {
      cout << "MBDFAKE: Using smeared vtx and t0 resolutions of "
           << G4MBD::z_smearing << " cm and " << G4MBD::t_smearing * 1000 << " ps" << endl;
    }
    MbdVertexFastSimReco* mbdvertex = new MbdVertexFastSimReco();
    mbdvertex->set_z_smearing(G4MBD::z_smearing);
    mbdvertex->set_t_smearing(G4MBD::t_smearing);
    mbdvertex->Verbosity(verbosity);

    se->registerSubsystem(mbdvertex);
  }
  if (Enable::MBDRECO)
  {
    auto mbddigi = new MbdDigitization();
    mbddigi->Verbosity(verbosity);
    se->registerSubsystem(mbddigi);

    auto mbdreco = new MbdReco();
    mbdreco->Verbosity(verbosity);
    se->registerSubsystem(mbdreco);
  }
  return;
}
#endif
