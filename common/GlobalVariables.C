#ifndef MACRO_GLOBALVARIABLES_C
#define MACRO_GLOBALVARIABLES_C

#include <g4decayer/EDecayType.hh>
#include <set>

double no_overlapp = 0.0001;

// These Input settings are needed in multiple Input selections
// Putting those here avoids include file ordering problems
namespace Input
{
  bool HEPMC = false;
  bool EMBED = false;
  bool READEIC = false;

  bool UPSILON = false;
  std::set<int> UPSILON_EmbedIds;
}  // namespace Input

namespace DstOut
{
  string OutputDir = ".";
  string OutputFile = "test.root";
}  // namespace DstOut

// Global settings affecting multiple subsystems
namespace Enable
{
  bool OVERLAPCHECK = false;
  bool ABSORBER = false;
  bool DSTOUT = false;
  bool DSTOUT_COMPRESS = false;
  int VERBOSITY = 0;
}  // namespace Enable

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

namespace G4MAGNET
{
  // initialize to garbage values - the override is done in the respective
  // MagnetInit() functions. If used standalone (without the G4_Magnet include)
  // like in the tracking - those need to be set in the Fun4All macro
  double magfield_rescale = NAN;
  string magfield;
}  // namespace G4MAGNET
#endif
