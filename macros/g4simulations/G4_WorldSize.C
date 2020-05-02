#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include "GlobalVariables.C"
#include <g4main/PHG4Reco.h>
R__LOAD_LIBRARY(libg4testbench.so)
#endif

void WorldSize(PHG4Reco *g4Reco, double radius)
{
  double world_radius = radius;
  if (world_radius < BlackHoleGeometry::max_radius)
  {
    world_radius = BlackHoleGeometry::max_radius;
  }
  if (g4Reco->GetWorldSizeX() < (world_radius+100)) // add 1m
  {
    g4Reco->SetWorldSizeX(world_radius+100);
  }
// our world is symmetric, pick the largest of abs(min_z) || abs(max_z)
  double max_zval = std::max(fabs(BlackHoleGeometry::min_z),fabs(BlackHoleGeometry::max_z));
  cout << "max_zval: " << max_zval << endl;
  if (g4Reco->GetWorldSizeZ() < 2*(max_zval+100))
  {
    g4Reco->SetWorldSizeZ( 2*(max_zval+100));
  }
  cout << "worldsize X: " << g4Reco->GetWorldSizeX() << endl;
  cout <<  "worldsize Z: " << g4Reco->GetWorldSizeZ() << endl;
  return;
}
