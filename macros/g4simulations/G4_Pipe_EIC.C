#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6, 00, 0)
#include <TSystem.h>
#include <g4detectors/PHG4CylinderSubsystem.h>
#include <g4detectors/PHG4GDMLSubsystem.h>
#include <g4main/PHG4Reco.h>
#include "GlobalVariables.C"
R__LOAD_LIBRARY(libg4detectors.so)
#endif

void PipeInit()
{
}

//! construct beam pipe
//! \param[in] use_forward_pipes whether to include the forward pipe extension beyond the Be section
double Pipe(PHG4Reco* g4Reco,
            double radius,
            const int absorberactive = 0,
            int verbosity = 0
            )
{
  // process pipe extentions?
  bool use_forward_pipes = false;
  const static bool do_pipe_hadron_forward_extension = use_forward_pipes && true;
  const static bool do_pipe_electron_forward_extension = use_forward_pipes && true;

  // Central pipe dimension
  // Extracted via mechanical model: Detector chamber 3-20-20
  // directly implimenting the central Be section in G4 cylinder for max speed simulation in the detector region.
  // The jointer lip structure of the pipe R = 3.2cm x L=5mm is ignored here
  static const double be_pipe_radius = 3.1000;
  static const double be_pipe_thickness = 3.1762 - be_pipe_radius;  // 760 um for sPHENIX
  static const double be_pipe_length_plus = 66.8;                   // +z beam pipe extend.
  static const double be_pipe_length_neg = -79.8;                   // -z beam pipe extend.

  static const double be_pipe_length = be_pipe_length_plus - be_pipe_length_neg;  // pipe length
  static const double be_pipe_center = 0.5 * (be_pipe_length_plus + be_pipe_length_neg);

  if (radius > be_pipe_radius)
  {
    cout << "inconsistency: radius: " << radius
         << " larger than pipe inner radius: " << be_pipe_radius << endl;
    gSystem->Exit(-1);
  }

  gSystem->Load("libg4detectors.so");
  gSystem->Load("libg4testbench.so");

  // mid-rapidity beryillium pipe
  PHG4CylinderSubsystem* cyl = new PHG4CylinderSubsystem("VAC_BE_PIPE", 0);
  cyl->set_double_param("radius", 0.0);
  cyl->set_int_param("lengthviarapidity", 0);
  cyl->set_double_param("length", be_pipe_length);
  cyl->set_double_param("place_z", be_pipe_center);
  cyl->set_string_param("material", "G4_Galactic");
  cyl->set_double_param("thickness", be_pipe_radius);
  cyl->SuperDetector("PIPE");
  cyl->OverlapCheck(overlapcheck);
  if (absorberactive) cyl->SetActive();
  g4Reco->registerSubsystem(cyl);

  cyl = new PHG4CylinderSubsystem("BE_PIPE", 1);
  cyl->set_double_param("radius", be_pipe_radius);
  cyl->set_int_param("lengthviarapidity", 0);
  cyl->set_double_param("length", be_pipe_length);
  cyl->set_double_param("place_z", be_pipe_center);
  cyl->set_string_param("material", "G4_Be");
  cyl->set_double_param("thickness", be_pipe_thickness);
  cyl->SuperDetector("PIPE");
  cyl->OverlapCheck(overlapcheck);
  if (absorberactive) cyl->SetActive();
  g4Reco->registerSubsystem(cyl);

  radius = be_pipe_radius + be_pipe_thickness;

  radius += no_overlapp;

  if (do_pipe_electron_forward_extension)
  {
    PHG4GDMLSubsystem* gdml = new PHG4GDMLSubsystem("ElectronForwardEnvelope");
    gdml->set_string_param("GDMPath", string(getenv("CALIBRATIONROOT")) + "/Beam/Detector chamber 3-20-20.G4Import.gdml");
    gdml->set_string_param("TopVolName", "ElectronForwardEnvelope");
    gdml->set_int_param("skip_DST_geometry_export", 1);  // do not export extended beam pipe as it is not supported by TGeo and outside Kalman filter acceptance
    gdml->OverlapCheck(overlapcheck);
    g4Reco->registerSubsystem(gdml);
  }

  if (do_pipe_hadron_forward_extension)
  {
    PHG4GDMLSubsystem* gdml = new PHG4GDMLSubsystem("HadronForwardEnvelope");
    gdml->set_string_param("GDMPath", string(getenv("CALIBRATIONROOT")) + "/Beam/Detector chamber 3-20-20.G4Import.gdml");
    gdml->set_string_param("TopVolName", "HadronForwardEnvelope");
    gdml->set_int_param("skip_DST_geometry_export", 1);  // do not export extended beam pipe as it is not supported by TGeo and outside Kalman filter acceptance
    gdml->OverlapCheck(overlapcheck);
    g4Reco->registerSubsystem(gdml);
  }

  return radius;
}
