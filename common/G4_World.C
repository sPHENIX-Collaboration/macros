#ifndef MACRO_G4WORLD_C
#define MACRO_G4WORLD_C

#include <GlobalVariables.C>

#include <g4main/PHG4Reco.h>

R__LOAD_LIBRARY(libg4testbench.so)

namespace G4WORLD
{
  double AddSpace = 100.;            // add this much space in cm around edge of detector
  string WorldMaterial = "G4_AIR";   // default world material, use G4_Galactic for material scan
  string PhysicsList = "FTFP_BERT";  // for calorimeters use HP lists
}  // namespace G4WORLD

void WorldInit(PHG4Reco *g4Reco)
{
  g4Reco->SetWorldMaterial(G4WORLD::WorldMaterial);
  g4Reco->SetPhysicsList(G4WORLD::PhysicsList);
}

void WorldSize(PHG4Reco *g4Reco, double radius)
{
  double world_radius = std::max((BlackHoleGeometry::max_radius + BlackHoleGeometry::gap), radius);
  g4Reco->SetWorldSizeY(std::max(g4Reco->GetWorldSizeY(), world_radius + G4WORLD::AddSpace));
  // our world is a symmetric cylinder so the center is at 0/0/0, pick the largest of abs(min_z) || abs(max_z)
  double min_zval = std::min((BlackHoleGeometry::min_z - BlackHoleGeometry::gap), -((g4Reco->GetWorldSizeZ() - 100) / 2.));
  double max_zval = std::max((BlackHoleGeometry::max_z + BlackHoleGeometry::gap), (g4Reco->GetWorldSizeZ() - 100) / 2.);
  double final_zval = std::max(fabs(min_zval), fabs(max_zval) + G4WORLD::AddSpace);
  g4Reco->SetWorldSizeZ(std::max(g4Reco->GetWorldSizeZ(), 2 * (final_zval)));
  return;
}
#endif
