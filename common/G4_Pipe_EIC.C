#ifndef MACRO_G4PIPEEIC_C
#define MACRO_G4PIPEEIC_C

#include <GlobalVariables.C>

#include <g4detectors/PHG4CylinderSubsystem.h>
#include <g4detectors/PHG4GDMLSubsystem.h>

#include <g4main/PHG4Reco.h>

#include <TSystem.h>

R__LOAD_LIBRARY(libg4detectors.so)

// This creates the Enable Flag to be used in the main steering macro
namespace Enable
{
  bool PIPE = false;
  bool PIPE_ABSORBER = false;
  bool PIPE_OVERLAPCHECK = false;
  int PIPE_VERBOSITY = 0;
}  // namespace Enable

namespace G4PIPE
{
  // Central pipe dimension
  // Extracted via mechanical model: Detector chamber 3-20-20
  // directly implimenting the central Be section in G4 cylinder for max speed simulation in the detector region.
  // The jointer lip structure of the pipe R = 3.2cm x L=5mm is ignored here
  double be_pipe_radius = 3.1000;
  double be_pipe_thickness = 3.1762 - be_pipe_radius;  // 760 um for sPHENIX
  double be_pipe_length_plus = 66.8;                   // +z beam pipe extend.
  double be_pipe_length_neg = -79.8;                   // -z beam pipe extend.
  bool use_forward_pipes = false;
}  // namespace G4PIPE

void PipeInit()
{
  if (G4PIPE::use_forward_pipes)
  {
    BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, 23.);
    BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z, 450.);
    BlackHoleGeometry::min_z = std::min(BlackHoleGeometry::min_z, -463.);
  }
  else
  {
    BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, G4PIPE::be_pipe_radius + G4PIPE::be_pipe_thickness);
    BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z, G4PIPE::be_pipe_length_plus);
    BlackHoleGeometry::min_z = std::min(BlackHoleGeometry::min_z, G4PIPE::be_pipe_length_neg);
  }
}

//! construct beam pipe
double Pipe(PHG4Reco* g4Reco,
            double radius)
{
  bool AbsorberActive = Enable::ABSORBER || Enable::PIPE_ABSORBER;
  bool OverlapCheck = Enable::OVERLAPCHECK || Enable::PIPE_OVERLAPCHECK;
  int verbosity = std::max(Enable::VERBOSITY, Enable::PIPE_VERBOSITY);
  // process pipe extentions?
  const bool do_pipe_hadron_forward_extension = G4PIPE::use_forward_pipes && true;
  const bool do_pipe_electron_forward_extension = G4PIPE::use_forward_pipes && true;

  const double be_pipe_length = G4PIPE::be_pipe_length_plus - G4PIPE::be_pipe_length_neg;  // pipe length
  const double be_pipe_center = 0.5 * (G4PIPE::be_pipe_length_plus + G4PIPE::be_pipe_length_neg);

  if (radius > G4PIPE::be_pipe_radius)
  {
    cout << "inconsistency: radius: " << radius
         << " larger than pipe inner radius: " << G4PIPE::be_pipe_radius << endl;
    gSystem->Exit(-1);
  }

  // mid-rapidity beryillium pipe
  PHG4CylinderSubsystem* cyl = new PHG4CylinderSubsystem("VAC_BE_PIPE", 0);
  cyl->set_double_param("radius", 0.0);
  cyl->set_int_param("lengthviarapidity", 0);
  cyl->set_double_param("length", be_pipe_length);
  cyl->set_double_param("place_z", be_pipe_center);
  cyl->set_string_param("material", "G4_Galactic");
  cyl->set_double_param("thickness", G4PIPE::be_pipe_radius);
  cyl->SuperDetector("PIPE");
  cyl->OverlapCheck(OverlapCheck);
  if (AbsorberActive) cyl->SetActive();
  g4Reco->registerSubsystem(cyl);

  cyl = new PHG4CylinderSubsystem("BE_PIPE", 1);
  cyl->set_double_param("radius", G4PIPE::be_pipe_radius);
  cyl->set_int_param("lengthviarapidity", 0);
  cyl->set_double_param("length", be_pipe_length);
  cyl->set_double_param("place_z", be_pipe_center);
  cyl->set_string_param("material", "G4_Be");
  cyl->set_double_param("thickness", G4PIPE::be_pipe_thickness);
  cyl->SuperDetector("PIPE");
  cyl->OverlapCheck(OverlapCheck);
  if (AbsorberActive) cyl->SetActive();
  g4Reco->registerSubsystem(cyl);

  radius = G4PIPE::be_pipe_radius + G4PIPE::be_pipe_thickness;

  radius += no_overlapp;

  if (do_pipe_electron_forward_extension)
  {
    PHG4GDMLSubsystem* gdml = new PHG4GDMLSubsystem("ElectronForwardEnvelope");
    gdml->set_string_param("GDMPath", string(getenv("CALIBRATIONROOT")) + "/Beam/Detector_chamber_3-20-20.G4Import.gdml");
    gdml->set_string_param("TopVolName", "ElectronForwardEnvelope");
    gdml->set_int_param("skip_DST_geometry_export", 1);  // do not export extended beam pipe as it is not supported by TGeo and outside Kalman filter acceptance
    gdml->OverlapCheck(OverlapCheck);
    g4Reco->registerSubsystem(gdml);
  }

  if (do_pipe_hadron_forward_extension)
  {
    PHG4GDMLSubsystem* gdml = new PHG4GDMLSubsystem("HadronForwardEnvelope");
    gdml->set_string_param("GDMPath", string(getenv("CALIBRATIONROOT")) + "/Beam/Detector_chamber_3-20-20.G4Import.gdml");
    gdml->set_string_param("TopVolName", "HadronForwardEnvelope");
    gdml->set_int_param("skip_DST_geometry_export", 1);  // do not export extended beam pipe as it is not supported by TGeo and outside Kalman filter acceptance
    gdml->OverlapCheck(OverlapCheck);
    g4Reco->registerSubsystem(gdml);
  }

  return radius;
}
#endif
