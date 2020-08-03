#ifndef MACRO_G4MICROMEGAS_C
#define MACRO_G4MICROMEGAS_C

#include "GlobalVariables.C"

#include "G4_Intt.C"
#include "G4_Mvtx.C"
#include "G4_TPC.C"

#include <g4micromegas/PHG4MicromegasDigitizer.h>
#include <g4micromegas/PHG4MicromegasHitReco.h>
#include <g4micromegas/PHG4MicromegasSubsystem.h>

#include <g4main/PHG4Reco.h>

#include <micromegas/MicromegasClusterizer.h>

#include <fun4all/Fun4AllServer.h>

R__LOAD_LIBRARY(libmicromegas.so)
R__LOAD_LIBRARY(libg4micromegas.so)

namespace Enable
{
  bool MICROMEGAS = false;
  bool MICROMEGAS_CELL = false;
  bool MICROMEGAS_CLUSTER = false;
}  // namespace Enable

namespace G4MICROMEGAS
{
  int n_micromegas_layer = 2;
}

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
  BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, 83.);
  BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z, 220. / 2.);
  BlackHoleGeometry::min_z = std::min(BlackHoleGeometry::min_z, -220. / 2.);
}

void Micromegas(PHG4Reco* g4Reco)
{
  const int mm_layer = G4MVTX::n_maps_layer + G4INTT::n_intt_layer + G4TPC::n_gas_layer;
  auto mm = new PHG4MicromegasSubsystem("MICROMEGAS", mm_layer);
  mm->SetActive();
  mm->set_double_param("mm_length", 220);
  mm->set_double_param("mm_radius", 82);
  g4Reco->registerSubsystem(mm);
}

void Micromegas_Cells()
{
  Fun4AllServer* se = Fun4AllServer::instance();
  // micromegas
  auto reco = new PHG4MicromegasHitReco;
  reco->Verbosity(0);

  static constexpr double radius = 82;
  static constexpr double tile_length = 50;
  static constexpr double tile_width = 25;

  // 12 tiles at mid rapidity, one in front of each TPC sector
  static constexpr int ntiles = 12;
  MicromegasTile::List tiles;
  for (int i = 0; i < ntiles; ++i)
  {
    tiles.push_back({{2. * M_PI * (0.5 + i) / ntiles, 0, tile_width / radius, tile_length}});
  }
  reco->set_tiles(tiles);

  se->registerSubsystem(reco);
}

void Micromegas_Clustering()
{
  Fun4AllServer* se = Fun4AllServer::instance();
  se->registerSubsystem(new PHG4MicromegasDigitizer);
  se->registerSubsystem(new MicromegasClusterizer);
}
#endif
