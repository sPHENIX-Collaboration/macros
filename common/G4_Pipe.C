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
  // STAR Beampipe for sPHENIX - based on STAR DWG 30006
  double be_pipe_radius = 2.00025;       // 4.0005 cm inner diameter
  double be_pipe_thickness = 0.0762;     // 0.762 mm
  double be_pipe_length = 120.015;       // 120 cm or 47.25"
  double be_pipe_zshift = -41.1639;      // amount Be pipe is shifted south

  double al_pipe_radius = 2.00025;       // same as Be pipe
  double al_pipe_thickness = 0.1397;     // 1.397 mm or 0.055"
  double al_pipe_south_length = 101.486; // Al extension south (from Dan's drawing, slight diff from STAR drawing)
  double al_pipe_north_length = 61.51;   // Al extension north (from Dan's drawing, slight diff from STAR drawing)
  double al_pipe_north_ext_length = 123.393;  // additional north extension

  double flange_thickness = 6.934/2 - (al_pipe_radius + al_pipe_thickness); // Atlas 2.75" flange, radial thickness
  double flange_length = 1.2825;                                                // Atlas 2.75" flange

  double outer_pipe_precone_length = 22.86;
  double outer_pipe_precone_radius = 2.00025;       // same as Be pipe
  double outer_pipe_thickness = 0.1397;             // 1.397 mm or 0.055"
  double outer_pipe_cone_length = 38.1;
  double outer_pipe_ext_radius = 3.81;    // past the cone
//  double outer_pipe_ext_length = 67.087;    // extension beyond conical part
  double outer_pipe_ext_length = 100.;    // extension beyond conical part through epd

  // maximum extent of the central part of beampipe (the forward regions are implemented in G4_Beamline.C)
  double max_z = be_pipe_zshift + be_pipe_length / 2. + al_pipe_north_length + al_pipe_north_ext_length +
    outer_pipe_precone_length + outer_pipe_cone_length + outer_pipe_ext_length;

}  // namespace G4PIPE

void PipeInit()
{
  BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, G4PIPE::outer_pipe_ext_radius + G4PIPE::outer_pipe_thickness);
  BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z, G4PIPE::max_z + no_overlapp );
  BlackHoleGeometry::min_z = std::min(BlackHoleGeometry::min_z, -(G4PIPE::max_z + no_overlapp) );
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

  int ilayer = 0;

  // mid-rapidity beryllium pipe
  PHG4CylinderSubsystem* cyl = new PHG4CylinderSubsystem("VAC_BE_PIPE", ilayer++);
  cyl->set_double_param("place_z", G4PIPE::be_pipe_zshift);
  cyl->set_double_param("radius", 0.0);
  cyl->set_int_param("lengthviarapidity", 0);
  cyl->set_double_param("length", G4PIPE::be_pipe_length);
  cyl->set_string_param("material", "G4_Galactic");
  cyl->set_double_param("thickness", G4PIPE::be_pipe_radius);
  cyl->SuperDetector("PIPE");
  if (AbsorberActive) cyl->SetActive();
  cyl->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(cyl);

  cyl = new PHG4CylinderSubsystem("BE_PIPE", ilayer++);
  cyl->set_double_param("place_z", G4PIPE::be_pipe_zshift);
  cyl->set_double_param("radius", G4PIPE::be_pipe_radius);  // inner radius
  cyl->set_int_param("lengthviarapidity", 0);
  cyl->set_double_param("length", G4PIPE::be_pipe_length);
  cyl->set_string_param("material", "G4_Be");
  cyl->set_double_param("thickness", G4PIPE::be_pipe_thickness);
  cyl->SuperDetector("PIPE");
  if (AbsorberActive) cyl->SetActive();
  cyl->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(cyl);

  // north aluminum pipe
  double north_pipe_place_z = G4PIPE::be_pipe_zshift + 0.5 * G4PIPE::be_pipe_length + 0.5 * (G4PIPE::al_pipe_north_length + G4PIPE::al_pipe_north_ext_length);
  cyl = new PHG4CylinderSubsystem("VAC_N_AL_PIPE", ilayer++);
  cyl->set_double_param("place_z", north_pipe_place_z + no_overlapp);
  cyl->set_double_param("radius", 0.0);
  cyl->set_int_param("lengthviarapidity", 0);
  cyl->set_double_param("length", G4PIPE::al_pipe_north_length + G4PIPE::al_pipe_north_ext_length);
  cyl->set_string_param("material", "G4_Galactic");
  cyl->set_double_param("thickness", G4PIPE::al_pipe_radius);
  cyl->SuperDetector("PIPE");
  if (AbsorberActive) cyl->SetActive();
  cyl->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(cyl);

  cyl = new PHG4CylinderSubsystem("N_AL_PIPE", ilayer++);
  cyl->set_double_param("place_z", north_pipe_place_z + no_overlapp);
  cyl->set_double_param("radius", G4PIPE::al_pipe_radius);
  cyl->set_int_param("lengthviarapidity", 0);
  cyl->set_double_param("length", G4PIPE::al_pipe_north_length + G4PIPE::al_pipe_north_ext_length);
  cyl->set_string_param("material", "G4_Al");
  cyl->set_double_param("thickness", G4PIPE::al_pipe_thickness);
  cyl->SuperDetector("PIPE");
  if (AbsorberActive) cyl->SetActive();
  cyl->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(cyl);

  // north flange 1A
  double flange_place_z = G4PIPE::be_pipe_zshift + 0.5 * G4PIPE::be_pipe_length + G4PIPE::al_pipe_north_length; // center of flange coupling
  // Al half, towards IP
  cyl = new PHG4CylinderSubsystem("N_FLANGE_1A_AL", ilayer++);
  cyl->set_double_param("place_z", flange_place_z - 0.75*G4PIPE::flange_length);
  cyl->set_double_param("radius", G4PIPE::al_pipe_radius + G4PIPE::al_pipe_thickness);
  cyl->set_int_param("lengthviarapidity", 0);
  cyl->set_double_param("length", G4PIPE::flange_length/2);
  cyl->set_string_param("material", "G4_Al");
  cyl->set_double_param("thickness", G4PIPE::flange_thickness);
  cyl->SuperDetector("PIPE");
  if (AbsorberActive) cyl->SetActive();
  cyl->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(cyl);

  // SS half, away from IP
  cyl = new PHG4CylinderSubsystem("N_FLANGE_1A_SS", ilayer++);
  cyl->set_double_param("place_z", flange_place_z - 0.25*G4PIPE::flange_length);
  cyl->set_double_param("radius", G4PIPE::al_pipe_radius + G4PIPE::al_pipe_thickness);
  cyl->set_int_param("lengthviarapidity", 0);
  cyl->set_double_param("length", G4PIPE::flange_length/2);
  cyl->set_string_param("material", "G4_STAINLESS-STEEL");
  cyl->set_double_param("thickness", G4PIPE::flange_thickness);
  cyl->SuperDetector("PIPE");
  if (AbsorberActive) cyl->SetActive();
  cyl->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(cyl);

  // north flange 1B
  // SS half, towards IP
  cyl = new PHG4CylinderSubsystem("N_FLANGE_1B_SS", ilayer++);
  cyl->set_double_param("place_z", flange_place_z + 0.25*G4PIPE::flange_length);
  cyl->set_double_param("radius", G4PIPE::al_pipe_radius + G4PIPE::al_pipe_thickness);
  cyl->set_int_param("lengthviarapidity", 0);
  cyl->set_double_param("length", G4PIPE::flange_length/2);
  cyl->set_string_param("material", "G4_STAINLESS-STEEL");
  cyl->set_double_param("thickness", G4PIPE::flange_thickness);
  cyl->SuperDetector("PIPE");
  if (AbsorberActive) cyl->SetActive();
  cyl->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(cyl);

  // Al half, away from IP
  cyl = new PHG4CylinderSubsystem("N_FLANGE_1B_Al", ilayer++);
  cyl->set_double_param("place_z", flange_place_z + 0.75*G4PIPE::flange_length);
  cyl->set_double_param("radius", G4PIPE::al_pipe_radius + G4PIPE::al_pipe_thickness);
  cyl->set_int_param("lengthviarapidity", 0);
  cyl->set_double_param("length", G4PIPE::flange_length/2);
  cyl->set_string_param("material", "G4_Al");
  cyl->set_double_param("thickness", G4PIPE::flange_thickness);
  cyl->SuperDetector("PIPE");
  if (AbsorberActive) cyl->SetActive();
  cyl->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(cyl);
 
  // north flange 2A
  flange_place_z = G4PIPE::be_pipe_zshift + 0.5 * G4PIPE::be_pipe_length + G4PIPE::al_pipe_north_length + G4PIPE::al_pipe_north_ext_length;
  // Al half, towards IP
  cyl = new PHG4CylinderSubsystem("N_FLANGE_2A_Al", ilayer++);
  cyl->set_double_param("place_z", flange_place_z - 0.75*G4PIPE::flange_length);
  cyl->set_double_param("radius", G4PIPE::al_pipe_radius + G4PIPE::al_pipe_thickness);
  cyl->set_int_param("lengthviarapidity", 0);
  cyl->set_double_param("length", G4PIPE::flange_length/2);
  cyl->set_string_param("material", "G4_Al");
  cyl->set_double_param("thickness", G4PIPE::flange_thickness);
  cyl->SuperDetector("PIPE");
  if (AbsorberActive) cyl->SetActive();
  cyl->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(cyl);

  // SS half, away from IP
  cyl = new PHG4CylinderSubsystem("N_FLANGE_2A_SS", ilayer++);
  cyl->set_double_param("place_z", flange_place_z - 0.25*G4PIPE::flange_length);
  cyl->set_double_param("radius", G4PIPE::al_pipe_radius + G4PIPE::al_pipe_thickness);
  cyl->set_int_param("lengthviarapidity", 0);
  cyl->set_double_param("length", G4PIPE::flange_length/2);
  cyl->set_string_param("material", "G4_STAINLESS-STEEL");
  cyl->set_double_param("thickness", G4PIPE::flange_thickness);
  cyl->SuperDetector("PIPE");
  if (AbsorberActive) cyl->SetActive();
  cyl->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(cyl);

  // north flange 2B
  // SS half, towards IP
  cyl = new PHG4CylinderSubsystem("N_FLANGE_2B_SS", ilayer++);
  cyl->set_double_param("place_z", flange_place_z + 0.25*G4PIPE::flange_length);
  cyl->set_double_param("radius", G4PIPE::al_pipe_radius + G4PIPE::al_pipe_thickness);
  cyl->set_int_param("lengthviarapidity", 0);
  cyl->set_double_param("length", G4PIPE::flange_length/2);
  cyl->set_string_param("material", "G4_STAINLESS-STEEL");
  cyl->set_double_param("thickness", G4PIPE::flange_thickness);
  cyl->SuperDetector("PIPE");
  if (AbsorberActive) cyl->SetActive();
  cyl->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(cyl);

  // Al half, away from IP
  cyl = new PHG4CylinderSubsystem("N_FLANGE_2B_Al", ilayer++);
  cyl->set_double_param("place_z", flange_place_z + 0.75*G4PIPE::flange_length);
  cyl->set_double_param("radius", G4PIPE::al_pipe_radius + G4PIPE::al_pipe_thickness);
  cyl->set_int_param("lengthviarapidity", 0);
  cyl->set_double_param("length", G4PIPE::flange_length/2);
  cyl->set_string_param("material", "G4_Al");
  cyl->set_double_param("thickness", G4PIPE::flange_thickness);
  cyl->SuperDetector("PIPE");
  if (AbsorberActive) cyl->SetActive();
  cyl->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(cyl);

  // south aluminum pipe
  double south_pipe_place_z = G4PIPE::be_pipe_zshift - 0.5 * G4PIPE::be_pipe_length - 0.5 * G4PIPE::al_pipe_south_length;
  cyl = new PHG4CylinderSubsystem("VAC_S_AL_PIPE", ilayer++);
  cyl->set_double_param("place_z", south_pipe_place_z - no_overlapp);
  cyl->set_double_param("radius", 0.0);
  cyl->set_int_param("lengthviarapidity", 0);
  cyl->set_double_param("length", G4PIPE::al_pipe_south_length);
  cyl->set_string_param("material", "G4_Galactic");
  cyl->set_double_param("thickness", G4PIPE::al_pipe_radius);
  cyl->SuperDetector("PIPE");
  if (AbsorberActive) cyl->SetActive();
  cyl->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(cyl);

  cyl = new PHG4CylinderSubsystem("S_AL_PIPE", ilayer++);
  cyl->set_double_param("place_z", south_pipe_place_z - no_overlapp);
  cyl->set_double_param("radius", G4PIPE::al_pipe_radius);
  cyl->set_int_param("lengthviarapidity", 0);
  cyl->set_double_param("length", G4PIPE::al_pipe_south_length);
  cyl->set_string_param("material", "G4_Al");
  cyl->set_double_param("thickness", G4PIPE::al_pipe_thickness);
  cyl->SuperDetector("PIPE");
  if (AbsorberActive) cyl->SetActive();
  cyl->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(cyl);

  // south flange 1A
  flange_place_z = G4PIPE::be_pipe_zshift - 0.5 * G4PIPE::be_pipe_length - G4PIPE::al_pipe_south_length;
  // Al half, towards IP
  cyl = new PHG4CylinderSubsystem("S_FLANGE_1A_Al", ilayer++);
  cyl->set_double_param("place_z", flange_place_z + 0.75*G4PIPE::flange_length);
  cyl->set_double_param("radius", G4PIPE::al_pipe_radius + G4PIPE::al_pipe_thickness);
  cyl->set_int_param("lengthviarapidity", 0);
  cyl->set_double_param("length", G4PIPE::flange_length/2);
  cyl->set_string_param("material", "G4_Al");
  cyl->set_double_param("thickness", G4PIPE::flange_thickness);
  cyl->SuperDetector("PIPE");
  if (AbsorberActive) cyl->SetActive();
  cyl->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(cyl);

  // SS half, away from IP
  cyl = new PHG4CylinderSubsystem("S_FLANGE_1A_SS", ilayer++);
  cyl->set_double_param("place_z", flange_place_z + 0.25*G4PIPE::flange_length);
  cyl->set_double_param("radius", G4PIPE::al_pipe_radius + G4PIPE::al_pipe_thickness);
  cyl->set_int_param("lengthviarapidity", 0);
  cyl->set_double_param("length", G4PIPE::flange_length/2);
  cyl->set_string_param("material", "G4_STAINLESS-STEEL");
  cyl->set_double_param("thickness", G4PIPE::flange_thickness);
  cyl->SuperDetector("PIPE");
  if (AbsorberActive) cyl->SetActive();
  cyl->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(cyl);

  // south flange 1B
  // SS half, towards IP
  cyl = new PHG4CylinderSubsystem("S_FLANGE_1B_SS", ilayer++);
  cyl->set_double_param("place_z", flange_place_z - 0.25*G4PIPE::flange_length);
  cyl->set_double_param("radius", G4PIPE::al_pipe_radius + G4PIPE::al_pipe_thickness);
  cyl->set_int_param("lengthviarapidity", 0);
  cyl->set_double_param("length", G4PIPE::flange_length/2);
  cyl->set_string_param("material", "G4_STAINLESS-STEEL");
  cyl->set_double_param("thickness", G4PIPE::flange_thickness);
  cyl->SuperDetector("PIPE");
  if (AbsorberActive) cyl->SetActive();
  cyl->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(cyl);

  // Al half, away from IP
  cyl = new PHG4CylinderSubsystem("S_FLANGE_1B_AL", ilayer++);
  cyl->set_double_param("place_z", flange_place_z - 0.75*G4PIPE::flange_length);
  cyl->set_double_param("radius", G4PIPE::al_pipe_radius + G4PIPE::al_pipe_thickness);
  cyl->set_int_param("lengthviarapidity", 0);
  cyl->set_double_param("length", G4PIPE::flange_length/2);
  cyl->set_string_param("material", "G4_Al");
  cyl->set_double_param("thickness", G4PIPE::flange_thickness);
  cyl->SuperDetector("PIPE");
  if (AbsorberActive) cyl->SetActive();
  cyl->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(cyl);

  /* north outermost pipe (extension) [vacuum] */
  north_pipe_place_z += 0.5 * (G4PIPE::al_pipe_north_length + G4PIPE::al_pipe_north_ext_length) + 0.5 * G4PIPE::outer_pipe_precone_length;
  cyl = new PHG4CylinderSubsystem("VAC_N_OUTER_PIPE_PRECONE", ilayer++);
  cyl->set_double_param("place_z", north_pipe_place_z + no_overlapp);
  cyl->set_double_param("radius", 0.);
  cyl->set_int_param("lengthviarapidity", 0);
  cyl->set_double_param("length", G4PIPE::outer_pipe_precone_length);
  cyl->set_string_param("material", "G4_Galactic");
  cyl->set_double_param("thickness", G4PIPE::outer_pipe_precone_radius);
  cyl->SuperDetector("PIPE");
  if (AbsorberActive) cyl->SetActive();
  cyl->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(cyl);

  /* north outermost pipe (pre-conical part) */
  cyl = new PHG4CylinderSubsystem("N_OUTER_PIPE_PRECONE", ilayer++);
  cyl->set_double_param("place_z", north_pipe_place_z + no_overlapp);
  cyl->set_double_param("radius", G4PIPE::outer_pipe_precone_radius);
  cyl->set_int_param("lengthviarapidity", 0);
  cyl->set_double_param("length", G4PIPE::outer_pipe_precone_length);
  cyl->set_string_param("material", "G4_Al");
  cyl->set_double_param("thickness", G4PIPE::outer_pipe_thickness);
  cyl->SuperDetector("PIPE");
  if (AbsorberActive) cyl->SetActive();
  cyl->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(cyl);

  PHG4ConeSubsystem* cone = nullptr;

  /* north outermost pipe (conical part) [vacuum] */
  north_pipe_place_z += 0.5 * G4PIPE::outer_pipe_precone_length + 0.5 * G4PIPE::outer_pipe_cone_length;
  cone = new PHG4ConeSubsystem("VAC_N_OUTER_PIPE_CONE", ilayer++);
  cone->SetR1(0., G4PIPE::outer_pipe_precone_radius);
  cone->SetR2(0., G4PIPE::outer_pipe_ext_radius);
  cone->SetZlength(G4PIPE::outer_pipe_cone_length / 2);
  cone->SetPlaceZ(north_pipe_place_z + no_overlapp);
  cone->SetMaterial("G4_Galactic");
  cone->SuperDetector("PIPE");
  if (AbsorberActive) cone->SetActive();
  cone->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(cone);

  /* north outermost pipe (conical part) */
  cone = new PHG4ConeSubsystem("N_OUTER_PIPE_CONE", ilayer++);
  cone->SetR1(G4PIPE::outer_pipe_precone_radius, G4PIPE::outer_pipe_precone_radius + G4PIPE::outer_pipe_thickness);
  cone->SetR2(G4PIPE::outer_pipe_ext_radius, G4PIPE::outer_pipe_ext_radius + G4PIPE::outer_pipe_thickness);
  cone->SetZlength(G4PIPE::outer_pipe_cone_length / 2);
  cone->SetPlaceZ(north_pipe_place_z + no_overlapp);
  cone->SetMaterial("G4_Al");
  cone->SuperDetector("PIPE");
  if (AbsorberActive) cone->SetActive();
  cone->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(cone);

  /* north outermost pipe (extension) [vacuum] */
  north_pipe_place_z += 0.5 * G4PIPE::outer_pipe_cone_length + 0.5 * G4PIPE::outer_pipe_ext_length;
  cyl = new PHG4CylinderSubsystem("VAC_N_OUTER_PIPE_EXT", ilayer++);
  cyl->set_double_param("place_z", north_pipe_place_z + no_overlapp);
  cyl->set_double_param("radius", 0.);
  cyl->set_int_param("lengthviarapidity", 0);
  cyl->set_double_param("length", G4PIPE::outer_pipe_ext_length);
  cyl->set_string_param("material", "G4_Galactic");
  cyl->set_double_param("thickness", G4PIPE::outer_pipe_ext_radius);
  cyl->SuperDetector("PIPE");
  if (AbsorberActive) cyl->SetActive();
  cyl->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(cyl);

  /* north outermost pipe (extension) */
  cyl = new PHG4CylinderSubsystem("N_OUTER_PIPE_EXT", ilayer++);
  cyl->set_double_param("place_z", north_pipe_place_z + no_overlapp);
  cyl->set_double_param("radius", G4PIPE::outer_pipe_ext_radius);
  cyl->set_int_param("lengthviarapidity", 0);
  cyl->set_double_param("length", G4PIPE::outer_pipe_ext_length);
  cyl->set_string_param("material", "G4_Al");
  cyl->set_double_param("thickness", G4PIPE::outer_pipe_thickness);
  cyl->SuperDetector("PIPE");
  if (AbsorberActive) cyl->SetActive();
  cyl->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(cyl);

  /* south outermost pipe (pre-conical part) [vacuum] */
  south_pipe_place_z -= (0.5 * G4PIPE::al_pipe_south_length + 0.5 * G4PIPE::outer_pipe_precone_length);
  cyl = new PHG4CylinderSubsystem("VAC_S_OUTER_PIPE_PRECONE", ilayer++);
  cyl->set_double_param("place_z", south_pipe_place_z - no_overlapp);
  cyl->set_double_param("radius", 0.);
  cyl->set_int_param("lengthviarapidity", 0);
  cyl->set_double_param("length", G4PIPE::outer_pipe_precone_length);
  cyl->set_string_param("material", "G4_Galactic");
  cyl->set_double_param("thickness", G4PIPE::outer_pipe_precone_radius);
  cyl->SuperDetector("PIPE");
  if (AbsorberActive) cyl->SetActive();
  cyl->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(cyl);

  /* south outermost pipe (pre-conical part) */
  cyl = new PHG4CylinderSubsystem("S_OUTER_PIPE_PRECONE", ilayer++);
  cyl->set_double_param("place_z", south_pipe_place_z - no_overlapp);
  cyl->set_double_param("radius", G4PIPE::outer_pipe_precone_radius);
  cyl->set_int_param("lengthviarapidity", 0);
  cyl->set_double_param("length", G4PIPE::outer_pipe_precone_length);
  cyl->set_string_param("material", "G4_Al");
  cyl->set_double_param("thickness", G4PIPE::outer_pipe_thickness);
  cyl->SuperDetector("PIPE");
  if (AbsorberActive) cyl->SetActive();
  cyl->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(cyl);

  /* south outermost pipe (conical part) [vacuum] */
  south_pipe_place_z -= (0.5 * G4PIPE::outer_pipe_precone_length + 0.5 * G4PIPE::outer_pipe_cone_length);
  cone = new PHG4ConeSubsystem("VAC_S_AL_PIPE_CONE", ilayer++);
  cone->SetR1(0., G4PIPE::outer_pipe_ext_radius);
  cone->SetR2(0., G4PIPE::outer_pipe_precone_radius);
  cone->SetZlength(G4PIPE::outer_pipe_cone_length / 2);
  cone->SetPlaceZ(south_pipe_place_z - no_overlapp);
  cone->SetMaterial("G4_Galactic");
  cone->SuperDetector("PIPE");
  if (AbsorberActive) cone->SetActive();
  cone->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(cone);

  /* south outermost pipe (conical part) */
  cone = new PHG4ConeSubsystem("S_OUTER_PIPE_CONE", ilayer++);
  cone->SetR1(G4PIPE::outer_pipe_ext_radius, G4PIPE::outer_pipe_ext_radius + G4PIPE::outer_pipe_thickness);
  cone->SetR2(G4PIPE::outer_pipe_precone_radius, G4PIPE::outer_pipe_precone_radius + G4PIPE::outer_pipe_thickness);
  cone->SetZlength(G4PIPE::outer_pipe_cone_length / 2);
  cone->SetPlaceZ(south_pipe_place_z - no_overlapp);
  cone->SetMaterial("G4_Al");
  cone->SuperDetector("PIPE");
  if (AbsorberActive) cone->SetActive();
  cone->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(cone);

  /* south outermost pipe (extension) [vacuum] */
  south_pipe_place_z -= (0.5 * G4PIPE::outer_pipe_cone_length + 0.5 * G4PIPE::outer_pipe_ext_length);
  cyl = new PHG4CylinderSubsystem("VAC_S_OUTER_PIPE_EXT", ilayer++);
  cyl->set_double_param("place_z", south_pipe_place_z - no_overlapp);
  cyl->set_double_param("radius", 0.);
  cyl->set_int_param("lengthviarapidity", 0);
  cyl->set_double_param("length", G4PIPE::outer_pipe_ext_length);
  cyl->set_string_param("material", "G4_Galactic");
  cyl->set_double_param("thickness", G4PIPE::outer_pipe_ext_radius);
  cyl->SuperDetector("PIPE");
  if (AbsorberActive) cyl->SetActive();
  cyl->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(cyl);

  /* south outermost pipe (extension) */
  cyl = new PHG4CylinderSubsystem("S_OUTER_PIPE_EXT", ilayer++);
  cyl->set_double_param("place_z", south_pipe_place_z - no_overlapp);
  cyl->set_double_param("radius", G4PIPE::outer_pipe_ext_radius);
  cyl->set_int_param("lengthviarapidity", 0);
  cyl->set_double_param("length", G4PIPE::outer_pipe_ext_length);
  cyl->set_string_param("material", "G4_Al");
  cyl->set_double_param("thickness", G4PIPE::outer_pipe_thickness);
  cyl->SuperDetector("PIPE");
  if (AbsorberActive) cyl->SetActive();
  cyl->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(cyl);

  radius = G4PIPE::outer_pipe_ext_radius + G4PIPE::outer_pipe_thickness;

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
