#ifndef MACRO_GLOBALVARIABLES_C
#define MACRO_GLOBALVARIABLES_C

#include <G4_TrkrVariables.C>
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

  //! nominal beam parameter configuration choices for BEAM_CONFIGURATION
  enum BeamConfiguration
  {
    AA_COLLISION = 0,
    pA_COLLISION,
    pp_COLLISION
  };

  BeamConfiguration BEAM_CONFIGURATION = AA_COLLISION;
}  // namespace Input

namespace DstOut
{
  std::string OutputDir = ".";
  std::string OutputFile = "test.root";
}  // namespace DstOut

// Global settings affecting multiple subsystems
namespace Enable
{
  bool ABSORBER = false;
  bool CDB = false;
  bool DSTOUT = false;
  bool DSTOUT_COMPRESS = false;
  bool OVERLAPCHECK = false;
  bool SUPPORT = false;
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
  std::string TrackNodeName = "SvtxTrackMap";
  bool pp_mode = false;
  double pp_extended_readout_time = 7000.0;  // ns
  bool reco_tpc_is_configured = false;
  int reco_tpc_maxtime_sample = 425;
  int reco_tpc_time_presample = 40;//120 - 80
}  // namespace TRACKING

namespace G4MAGNET
{
  // initialize to garbage values - the override is done in the respective
  // MagnetInit() functions. If used standalone (without the G4_Magnet include)
  // like in the tracking - those need to be set in the Fun4All macro
  double magfield_rescale = NAN;
  std::string magfield;
  std::string magfield_OHCAL_steel;
  std::string magfield_tracking;
}  // namespace G4MAGNET

namespace Enable
{
  bool MICROMEGAS = false;
}

namespace G4MICROMEGAS
{
  // number of micromegas layers
  int n_micromegas_layer = 2;
}  // namespace G4MICROMEGAS

namespace G4TPC
{
  double tpc_drift_velocity_reco = 8.0 / 1000.0;  // cm/ns   // this is the Ne version of the gas, it is very close to our Ar-CF4 mixture
}

namespace G4TRACKING
{
  bool init_acts_magfield = true;
}

namespace EVTGENDECAYER
{
  std::string DecayFile = "";  // The default is no need to force decay anything and use the default file DECAY.DEC from the official EvtGen software
                               // DECAY.DEC is located at: https://gitlab.cern.ch/evtgen/evtgen/-/blob/master/DECAY.DEC
}

namespace CDB
{
  std::string global_tag = "MDC2";
  uint64_t timestamp = 6;
}  // namespace CDB

// multi purpose functions
// cheap check via extension if we have a root file (pre c++17)
bool isRootFile(const std::string &fname)
{
  std::string tmp = fname;
  size_t i = fname.rfind('.', fname.length());
  if (i != string::npos)
  {
    if (fname.substr(i + 1, fname.length() - i) == "root")
    {
      return true;
    }
  }
  return false;
}

bool isConstantField(const std::string &name, double &fieldstrength)
{
  istringstream stringline(G4MAGNET::magfield_tracking);
  stringline >> fieldstrength;
  if (stringline.fail())
  {  // conversion to double fails -> we have a string (means fieldmap)
    fieldstrength = std::numeric_limits<double>::quiet_NaN();
    return false;
  }
  else
  {
    return true;
  }
}

#endif
