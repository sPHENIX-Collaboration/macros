#pragma once

#include <g4decayer/EDecayType.hh>

static bool overlapcheck = false;
static double no_overlapp = 0.0001;  // added to radii to avoid overlapping volumes


// These Input settings are needed in multiple Input selections
// Putting those here avoids include file ordering problems
namespace Input
{
  bool HEPMC = false;
  bool EMBED = false;
  bool READEIC = false;
}

// Global settings affecting multiple subsystems
namespace Enable
{
  bool OVERLAPCHECK = false;
  bool ABSORBER = false;
  bool DSTOUT = false;
  bool DSTOUT_COMPRESS = false;
  int VERBOSITY = 0;
}

// every G4 subsystem needs to implement this
// rather than forcing another include file,
// let's put this into the GlobalVariables.C
namespace BlackHoleGeometry
{
  double max_radius = 0.;  // this is needed for the overall dimension of the black hole
  double min_z = 0.;
  double max_z = 0.;
  double gap = no_overlapp;
  bool visible = false;
};  // namespace BlackHoleGeometry

namespace G4P6DECAYER
{
  EDecayType decayType = EDecayType::kAll;
}

// our various tracking macro
namespace TRACKING
{
  string TrackNodeName = "SvtxTrackMap";
}
