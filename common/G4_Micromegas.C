#ifndef MACRO_G4MICROMEGAS_C
#define MACRO_G4MICROMEGAS_C

#include <GlobalVariables.C>

#include <G4_ActsGeom.C>
#include <G4_Intt.C>
#include <G4_Mvtx.C>
#include <G4_TPC.C>
#include <G4_Tracking.C>

#include <g4micromegas/PHG4MicromegasDigitizer.h>
#include <g4micromegas/PHG4MicromegasHitReco.h>
#include <g4micromegas/PHG4MicromegasSubsystem.h>

#include <g4main/PHG4Reco.h>

#include <micromegas/MicromegasClusterizer.h>
#include <qa_modules/QAG4SimulationMicromegas.h>

#include <fun4all/Fun4AllServer.h>

R__LOAD_LIBRARY(libmicromegas.so)
R__LOAD_LIBRARY(libg4micromegas.so)

// some of the micromegas variables have been moved to GlobalVariables.C:
// bool MICROMEGAS = false; // moved to GlobalVariables.C
// int n_micromegas_layer = 2;
// because they are also needed in other macros

void MicromegasInit()
{
  if (!Enable::MVTX)
  {
    G4MVTX::n_maps_layer = 0;
  }
  // same for the INTT
  if (!Enable::INTT)
  {
    G4INTT::n_intt_layer = 0;
  }
  if (!Enable::TPC)
  {
    G4TPC::n_gas_layer = 0;
  }
  BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, 88.);
  BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z, 220. / 2.);
  BlackHoleGeometry::min_z = std::min(BlackHoleGeometry::min_z, -220. / 2.);
}

void Micromegas(PHG4Reco* g4Reco)
{
  const int mm_layer = G4MVTX::n_maps_layer + G4INTT::n_intt_layer + G4TPC::n_gas_layer;
  auto mm = new PHG4MicromegasSubsystem("MICROMEGAS", mm_layer);
  mm->OverlapCheck( Enable::OVERLAPCHECK );
  mm->SetActive();
  g4Reco->registerSubsystem(mm);
}

void Micromegas_Cells()
{
// the acts geometry needs to go here since it will be used by the PHG4MicromegasHitReco
  ACTSGEOM::ActsGeomInit();
  auto se = Fun4AllServer::instance();
  // micromegas
  auto reco = new PHG4MicromegasHitReco;
  reco->Verbosity(0);
  se->registerSubsystem(reco);

  se->registerSubsystem(new PHG4MicromegasDigitizer);
}

void Micromegas_Clustering()
{
  auto se = Fun4AllServer::instance();
  auto mm_clus = new MicromegasClusterizer;
  mm_clus->set_cluster_version(G4TRACKING::cluster_version);
  se->registerSubsystem(mm_clus);
}

void Micromegas_QA()
{
  auto se = Fun4AllServer::instance();
  auto qa_mm = new QAG4SimulationMicromegas;
  qa_mm->Verbosity(Enable::QA_VERBOSITY);
  qa_mm->set_cluster_version(G4TRACKING::cluster_version);
  se->registerSubsystem(qa_mm);
}

#endif
