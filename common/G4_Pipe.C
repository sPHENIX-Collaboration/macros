#ifndef MACRO_G4PIPE_C
#define MACRO_G4PIPE_C

#include <GlobalVariables.C>

#include <g4detectors/PHG4ConeSubsystem.h>
#include <g4detectors/PHG4CylinderSubsystem.h>

#include <g4main/PHG4Reco.h>

R__LOAD_LIBRARY(libg4detectors.so)

namespace Enable
{
  bool PIPE = false;
  bool PIPE_ABSORBER = false;
  bool PIPE_OVERLAPCHECK = false;
  int PIPE_VERBOSITY = 0;
}  // namespace Enable

namespace G4PIPE
{
  double be_pipe_radius = 2.0005;     // 4.001 cm inner diameter from spec sheet
  double be_pipe_thickness = 0.0760;  // 760 um based on spec sheet
  double be_pipe_length = 80.0;       // +/- 40 cm

  double al_pipe_radius = 2.0005;     // same as Be pipe
  double al_pipe_thickness = 0.1600;  // 1.6 mm based on spec
  double al_pipe_length = 171.44;     // extension beyond +/- 40 cm

  double al_pipe_cone_length = 8.56;

  double al_pipe_ext_radius = 2.5005;
  double al_pipe_ext_length = 60.0;  // extension beyond conical part
}  // namespace G4PIPE

void PipeInit()
{
  BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, G4PIPE::al_pipe_ext_radius + G4PIPE::al_pipe_thickness);
  BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z, G4PIPE::be_pipe_length / 2. + G4PIPE::al_pipe_length + G4PIPE::al_pipe_cone_length + G4PIPE::al_pipe_ext_length +no_overlapp );
  BlackHoleGeometry::min_z = std::min(BlackHoleGeometry::min_z, -(G4PIPE::be_pipe_length / 2. + G4PIPE::al_pipe_length + G4PIPE::al_pipe_cone_length + G4PIPE::al_pipe_ext_length) - no_overlapp);
}

double Pipe(PHG4Reco* g4Reco, double radius)
{
  bool AbsorberActive = Enable::ABSORBER || Enable::PIPE_ABSORBER;
  bool OverlapCheck = Enable::OVERLAPCHECK || Enable::PIPE_OVERLAPCHECK;
  int verbosity = std::max(Enable::VERBOSITY, Enable::PIPE_VERBOSITY);

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
  cyl->set_double_param("length", G4PIPE::be_pipe_length);
  cyl->set_string_param("material", "G4_Galactic");
  cyl->set_double_param("thickness", G4PIPE::be_pipe_radius);
  cyl->SuperDetector("PIPE");
  if (AbsorberActive) cyl->SetActive();
  cyl->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(cyl);

  cyl = new PHG4CylinderSubsystem("BE_PIPE", 1);
  cyl->set_double_param("radius", G4PIPE::be_pipe_radius);
  cyl->set_int_param("lengthviarapidity", 0);
  cyl->set_double_param("length", G4PIPE::be_pipe_length);
  cyl->set_string_param("material", "G4_Be");
  cyl->set_double_param("thickness", G4PIPE::be_pipe_thickness);
  cyl->SuperDetector("PIPE");
  if (AbsorberActive) cyl->SetActive();
  cyl->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(cyl);

  // north aluminum pipe
  cyl = new PHG4CylinderSubsystem("VAC_N_AL_PIPE", 2);
  cyl->set_double_param("place_z", 0.5 * G4PIPE::be_pipe_length + 0.5 * G4PIPE::al_pipe_length + no_overlapp);
  cyl->set_double_param("radius", 0.0);
  cyl->set_int_param("lengthviarapidity", 0);
  cyl->set_double_param("length", G4PIPE::al_pipe_length);
  cyl->set_string_param("material", "G4_Galactic");
  cyl->set_double_param("thickness", G4PIPE::al_pipe_radius);
  cyl->SuperDetector("PIPE");
  if (AbsorberActive) cyl->SetActive();
  cyl->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(cyl);

  cyl = new PHG4CylinderSubsystem("N_AL_PIPE", 3);
  cyl->set_double_param("place_z", 0.5 * G4PIPE::be_pipe_length + 0.5 * G4PIPE::al_pipe_length + no_overlapp);
  cyl->set_double_param("radius", G4PIPE::al_pipe_radius);
  cyl->set_int_param("lengthviarapidity", 0);
  cyl->set_double_param("length", G4PIPE::al_pipe_length);
  cyl->set_string_param("material", "G4_Al");
  cyl->set_double_param("thickness", G4PIPE::al_pipe_thickness);
  cyl->SuperDetector("PIPE");
  if (AbsorberActive) cyl->SetActive();
  cyl->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(cyl);

  // south aluminum pipe
  cyl = new PHG4CylinderSubsystem("VAC_S_AL_PIPE", 4);
  cyl->set_double_param("place_z", -0.5 * G4PIPE::be_pipe_length - 0.5 * G4PIPE::al_pipe_length - no_overlapp);
  cyl->set_double_param("radius", 0.0);
  cyl->set_int_param("lengthviarapidity", 0);
  cyl->set_double_param("length", G4PIPE::al_pipe_length);
  cyl->set_string_param("material", "G4_Galactic");
  cyl->set_double_param("thickness", G4PIPE::al_pipe_radius);
  cyl->SuperDetector("PIPE");
  if (AbsorberActive) cyl->SetActive();
  cyl->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(cyl);

  cyl = new PHG4CylinderSubsystem("S_AL_PIPE", 5);
  cyl->set_double_param("place_z", -0.5 * G4PIPE::be_pipe_length - 0.5 * G4PIPE::al_pipe_length - no_overlapp);
  cyl->set_double_param("radius", G4PIPE::al_pipe_radius);
  cyl->set_int_param("lengthviarapidity", 0);
  cyl->set_double_param("length", G4PIPE::al_pipe_length);
  cyl->set_string_param("material", "G4_Al");
  cyl->set_double_param("thickness", G4PIPE::al_pipe_thickness);
  cyl->SuperDetector("PIPE");
  if (AbsorberActive) cyl->SetActive();
  cyl->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(cyl);

  PHG4ConeSubsystem* cone = nullptr;

  double cone_position = 0.5 * G4PIPE::be_pipe_length + G4PIPE::al_pipe_length + 0.5 * G4PIPE::al_pipe_cone_length;
  double ext_position = 0.5 * G4PIPE::be_pipe_length + G4PIPE::al_pipe_length + G4PIPE::al_pipe_cone_length + 0.5 * G4PIPE::al_pipe_ext_length + no_overlapp;

  /* north aluminum pipe (conical part) */
  cone = new PHG4ConeSubsystem("N_AL_PIPE_CONE", 6);
  cone->SetR1(G4PIPE::al_pipe_radius, G4PIPE::al_pipe_radius + G4PIPE::al_pipe_thickness);
  cone->SetR2(G4PIPE::al_pipe_ext_radius, G4PIPE::al_pipe_ext_radius + G4PIPE::al_pipe_thickness);
  cone->SetZlength(G4PIPE::al_pipe_cone_length / 2);
  cone->SetPlaceZ(cone_position + no_overlapp);
  cone->SetMaterial("G4_Al");
  cone->SuperDetector("PIPE");
  if (AbsorberActive) cone->SetActive();
  cone->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(cone);

  /* north aluminum pipe (extension) */
  cyl = new PHG4CylinderSubsystem("N_AL_PIPE_EXT", 7);
  cyl->set_double_param("place_z", ext_position + no_overlapp);
  cyl->set_double_param("radius", G4PIPE::al_pipe_ext_radius);
  cyl->set_int_param("lengthviarapidity", 0);
  cyl->set_double_param("length", G4PIPE::al_pipe_ext_length);
  cyl->set_string_param("material", "G4_Al");
  cyl->set_double_param("thickness", G4PIPE::al_pipe_thickness);
  cyl->SuperDetector("PIPE");
  if (AbsorberActive) cyl->SetActive();
  cyl->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(cyl);

  /* south aluminum pipe (conical part) */
  cone = new PHG4ConeSubsystem("S_AL_PIPE_CONE", 8);
  cone->SetR1(G4PIPE::al_pipe_ext_radius, G4PIPE::al_pipe_ext_radius + G4PIPE::al_pipe_thickness);
  cone->SetR2(G4PIPE::al_pipe_radius, G4PIPE::al_pipe_radius + G4PIPE::al_pipe_thickness);
  cone->SetZlength(G4PIPE::al_pipe_cone_length / 2);
  cone->SetPlaceZ(-cone_position - no_overlapp);
  cone->SetMaterial("G4_Al");
  cone->SuperDetector("PIPE");
  if (AbsorberActive) cone->SetActive();
  cone->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(cone);

  /* south aluminum pipe (extension) */
  cyl = new PHG4CylinderSubsystem("S_AL_PIPE_EXT", 9);
  cyl->set_double_param("place_z", -ext_position - no_overlapp);
  cyl->set_double_param("radius", G4PIPE::al_pipe_ext_radius);
  cyl->set_int_param("lengthviarapidity", 0);
  cyl->set_double_param("length", G4PIPE::al_pipe_ext_length);
  cyl->set_string_param("material", "G4_Al");
  cyl->set_double_param("thickness", G4PIPE::al_pipe_thickness);
  cyl->SuperDetector("PIPE");
  if (AbsorberActive) cyl->SetActive();
  cyl->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(cyl);

  /* north aluminum pipe (conical part) [vacuum] */
  cone = new PHG4ConeSubsystem("VAC_N_AL_PIPE_CONE", 10);
  cone->SetR1(0., G4PIPE::al_pipe_radius);
  cone->SetR2(0., G4PIPE::al_pipe_ext_radius);
  cone->SetZlength(G4PIPE::al_pipe_cone_length / 2);
  cone->SetPlaceZ(cone_position + no_overlapp);
  cone->SetMaterial("G4_Galactic");
  cone->SuperDetector("PIPE");
  if (AbsorberActive) cone->SetActive();
  cone->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(cone);

  /* north aluminum pipe (extension) [vacuum] */
  cyl = new PHG4CylinderSubsystem("VAC_N_AL_PIPE_EXT", 11);
  cyl->set_double_param("place_z", ext_position + no_overlapp);
  cyl->set_double_param("radius", 0.);
  cyl->set_int_param("lengthviarapidity", 0);
  cyl->set_double_param("length", G4PIPE::al_pipe_ext_length);
  cyl->set_string_param("material", "G4_Galactic");
  cyl->set_double_param("thickness", G4PIPE::al_pipe_ext_radius);
  cyl->SuperDetector("PIPE");
  if (AbsorberActive) cyl->SetActive();
  cyl->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(cyl);

  /* south aluminum pipe (conical part) [vacuum] */
  cone = new PHG4ConeSubsystem("VAC_S_AL_PIPE_CONE", 12);
  cone->SetR1(0., G4PIPE::al_pipe_ext_radius);
  cone->SetR2(0., G4PIPE::al_pipe_radius);
  cone->SetZlength(G4PIPE::al_pipe_cone_length / 2);
  cone->SetPlaceZ(-cone_position - no_overlapp);
  cone->SetMaterial("G4_Galactic");
  cone->SuperDetector("PIPE");
  if (AbsorberActive) cone->SetActive();
  cone->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(cone);

  /* south aluminum pipe (extension) [vacuum] */
  cyl = new PHG4CylinderSubsystem("VAC_S_AL_PIPE_EXT", 13);
  cyl->set_double_param("place_z", -ext_position - no_overlapp);
  cyl->set_double_param("radius", 0.);
  cyl->set_int_param("lengthviarapidity", 0);
  cyl->set_double_param("length", G4PIPE::al_pipe_ext_length);
  cyl->set_string_param("material", "G4_Galactic");
  cyl->set_double_param("thickness", G4PIPE::al_pipe_ext_radius);
  cyl->SuperDetector("PIPE");
  if (AbsorberActive) cyl->SetActive();
  cyl->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(cyl);

  radius = G4PIPE::al_pipe_ext_radius + G4PIPE::al_pipe_thickness;

  if (verbosity > 0)
  {
    cout << "=========================== G4_Pipe.C::Pipe() =============================" << endl;
    cout << " PIPE Material Description:" << endl;
    cout << "  inner radius = " << G4PIPE::be_pipe_radius << " cm" << endl;
    cout << "  thickness = " << G4PIPE::be_pipe_thickness << " cm" << endl;
    cout << "  outer radius = " << G4PIPE::be_pipe_radius + G4PIPE::be_pipe_thickness << " cm" << endl;
    cout << "  length = " << G4PIPE::be_pipe_length << " cm" << endl;
    cout << "===========================================================================" << endl;
  }

  radius += no_overlapp;

  return radius;
}
#endif
