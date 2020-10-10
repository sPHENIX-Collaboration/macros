#pragma once

#include <g4epd/PHG4EPDSubsystem.h>

#include <g4main/PHG4Reco.h>

R__LOAD_LIBRARY(libg4detectors.so)
R__LOAD_LIBRARY(libg4epd.so)

namespace Enable {
  bool EPD = false;
  bool EPD_OVERLAPCHECK = false;
}

void EPDInit() { }

void EPDSetup(PHG4Reco* g4Reco) {
  bool overlap_check = Enable::OVERLAPCHECK || Enable::EPD_OVERLAPCHECK;

  PHG4EPDSubsystem* epd = new PHG4EPDSubsystem("EPD");

  epd->SuperDetector("EPD");
  epd->OverlapCheck(overlap_check);

  g4Reco->registerSubsystem(epd);
}
