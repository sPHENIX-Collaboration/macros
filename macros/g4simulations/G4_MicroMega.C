#pragma once

#include "GlobalVariables.C"

#include "G4_Intt.C"
#include "G4_Mvtx.C"
#include "G4_TPC.C"

#include <g4micromegas/PHG4MicromegasDigitizer.h>
#include <g4micromegas/PHG4MicromegasHitReco.h>
#include <g4micromegas/PHG4MicromegasSubsystem.h>

#include <g4main/PHG4Reco.h>

#include <fun4all/Fun4AllServer.h>

R__LOAD_LIBRARY(libmicromegas.so)

namespace Enable
{
  bool MICROMEGA = false;
  bool MICROMEGA_CELL = false;
}  // namespace Enable

void MicroMegaInit()
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
}

void MicroMega(PHG4Reco* g4Reco)
{
  const int mm_layer = G4MVTX::n_maps_layer + G4INTT::n_intt_layer + G4TPC::n_gas_layer;
  auto mm = new PHG4MicromegasSubsystem("MICROMEGAS", mm_layer);
  mm->SetActive();
  mm->set_double_param("mm_length", 220);
  mm->set_double_param("mm_radius", 82);
  g4Reco->registerSubsystem(mm);
}

void MicroMega_Cells()
{
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
