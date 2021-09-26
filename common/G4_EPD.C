#ifndef COMMON_G4EPD_C
#define COMMON_G4EPD_C

#include <GlobalVariables.C>

#include <g4epd/PHG4EPDSubsystem.h>

#include <g4main/PHG4Reco.h>

R__LOAD_LIBRARY(libg4epd.so)

namespace Enable
{
  bool EPD = false;
  bool EPD_SUPPORT = false;
  bool EPD_OVERLAPCHECK = false;
}  // namespace Enable

void EPDInit()
{
  BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, 90.);
  // using default z-position and add 10 cm for tile thickness
  BlackHoleGeometry::min_z = std::min(BlackHoleGeometry::min_z, -310.);
  BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z, 310.);
}

void EPD(PHG4Reco* g4Reco)
{
  bool overlap_check = Enable::OVERLAPCHECK || Enable::EPD_OVERLAPCHECK;

  PHG4EPDSubsystem* epd = new PHG4EPDSubsystem("EPD");

  epd->SuperDetector("EPD");
  epd->OverlapCheck(overlap_check);
  epd->SetActive();

  g4Reco->registerSubsystem(epd);
}

#endif /* COMMON_G4EPD_C */
