#ifndef MACRO_G4MICROMEGAS_C
#define MACRO_G4MICROMEGAS_C

#include <GlobalVariables.C>

#include <G4_Intt.C>
#include <G4_Mvtx.C>
#include <G4_TPC.C>

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
  enum Config
  {
    CONFIG_MINIMAL,
    CONFIG_PHI_ONE_RING,
    CONFIG_Z_ONE_SECTOR,
    CONFIG_BASELINE,
    CONFIG_MAXIMAL
  };

  Config CONFIG = CONFIG_BASELINE;

  // radius at which micromegas layers are installed
  double mm_radius = 85;

}  // namespace G4MICROMEGAS

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
  mm->set_double_param("mm_radius", G4MICROMEGAS::mm_radius);
  g4Reco->registerSubsystem(mm);
}

void Micromegas_Cells()
{
  auto se = Fun4AllServer::instance();
  // micromegas
  auto reco = new PHG4MicromegasHitReco;
  reco->Verbosity(0);

  static constexpr double length = 210;
  static constexpr int nsectors = 12;
  static constexpr double tile_length = 50;
  static constexpr double tile_width = 25;

  switch (G4MICROMEGAS::CONFIG)
  {
  case G4MICROMEGAS::CONFIG_MINIMAL:
  {
    // one tile at mid rapidity in front of TPC sector
    std::cout << "Micromegas_Cells - Tiles configuration: CONFIG_MINIMAL" << std::endl;
    static constexpr double phi0 = M_PI * (0.5 + 1. / nsectors);
    reco->set_tiles({{{phi0, 0, tile_width/CylinderGeomMicromegas::reference_radius, tile_length}}});
    break;
  }

  case G4MICROMEGAS::CONFIG_PHI_ONE_RING:
  {
    // 12 tiles at mid rapidity, one in front of each TPC sector
    std::cout << "Micromegas_Cells - Tiles configuration: CONFIG_PHI_ONE_RING" << std::endl;
    MicromegasTile::List tiles;
    for (int i = 0; i < nsectors; ++i)
    {
      tiles.emplace_back(2. * M_PI * (0.5 + i) / nsectors, 0, tile_width/CylinderGeomMicromegas::reference_radius, tile_length);
    }
    reco->set_tiles(tiles);
    break;
  }

  case G4MICROMEGAS::CONFIG_Z_ONE_SECTOR:
  {
    // 4 tiles with full z coverage in front of one TPC sector
    std::cout << "Micromegas_Cells - Tiles configuration: CONFIG_Z_ONE_SECTOR" << std::endl;
    MicromegasTile::List tiles;
    static constexpr double phi0 = M_PI * (0.5 + 1. / nsectors);
    static constexpr int ntiles = 4;
    for (int i = 0; i < ntiles; ++i)
    {
      tiles.emplace_back(phi0, length * ((0.5 + i) / ntiles - 0.5), tile_width/CylinderGeomMicromegas::reference_radius, tile_length);
    }
    reco->set_tiles(tiles);
    break;
  }

  case G4MICROMEGAS::CONFIG_BASELINE:
  {
    std::cout << "Micromegas_Cells - Tiles configuration: CONFIG_BASELINE" << std::endl;
    MicromegasTile::List tiles;

    // for the first sector we put 4 tiles with full z coverage
    static constexpr double phi0 = M_PI * (0.5 + 1. / nsectors);
    static constexpr int ntiles_z = 4;
    for (int i = 0; i < ntiles_z; ++i)
    {
      tiles.emplace_back(phi0, length * ((0.5 + i) / ntiles_z - 0.5), tile_width/CylinderGeomMicromegas::reference_radius, tile_length);
    }

    // for the other sectors we put two tiles on either side of the central membrane
    static constexpr double zoffset = 7;
    for (int i = 1; i < nsectors; ++i)
    {
      const double phi = phi0 + 2.*M_PI*i/nsectors;
      tiles.emplace_back( phi, length*(1.5/4-0.5) - zoffset, tile_width/CylinderGeomMicromegas::reference_radius, tile_length );
      tiles.emplace_back( phi, length*(2.5/4-0.5) + zoffset, tile_width/CylinderGeomMicromegas::reference_radius, tile_length );
    }
    reco->set_tiles(tiles);
    break;
  }

  case G4MICROMEGAS::CONFIG_MAXIMAL:
  {
    std::cout << "Micromegas_Cells - Tiles configuration: CONFIG_MAXIMAL" << std::endl;
    MicromegasTile::List tiles;

    // 4 tiles with full z coverage in front of each TPC sector
    static constexpr int ntiles_z = 4;
    for (int iphi = 0; iphi < nsectors; ++iphi)
    {
      const double phi = 2. * M_PI * (0.5 + iphi) / nsectors;
      for (int iz = 0; iz < ntiles_z; ++iz)
      {
        tiles.emplace_back(phi, length * ((0.5 + iz) / ntiles_z - 0.5), tile_width/CylinderGeomMicromegas::reference_radius, tile_length);
      }
    }
    reco->set_tiles(tiles);
    break;
  }
  }

  se->registerSubsystem(reco);

  se->registerSubsystem(new PHG4MicromegasDigitizer);
}

void Micromegas_Clustering()
{
  auto se = Fun4AllServer::instance();
  se->registerSubsystem(new MicromegasClusterizer);
}
#endif
