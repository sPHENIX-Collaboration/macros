#pragma once

#include "GlobalVariables.C"

#include "G4_Pipe.C"

#include <g4detectors/PHG4ConeSubsystem.h>
#include <g4detectors/PHG4CylinderSubsystem.h>

#include <g4main/PHG4Reco.h>

R__LOAD_LIBRARY(libg4detectors.so)

namespace Enable
{
  static bool PISTON = false;
  static bool PISTON_ABSORBER = false;
  static bool PISTON_OVERLAPCHECK = false;
  static int PISTON_VERBOSITY = 0;
}  // namespace Enable

namespace G4PISTON
{
}  // namespace G4PIPE

void PistonInit()
{
  BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, 50.);
  BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z,305. - 20. );
}

void Piston(PHG4Reco* g4Reco,
            const int absorberactive = 0)
{
  bool AbsorberActive = Enable::ABSORBER || Enable::PISTON_ABSORBER || absorberactive;
  bool OverlapCheck = Enable::OVERLAPCHECK || Enable::PISTON_OVERLAPCHECK;
  int verbosity = std::max(Enable::VERBOSITY, Enable::PISTON_VERBOSITY);

  string name = "magpiston";
  const double zpos0 = G4PIPE::al_pipe_length + G4PIPE::be_pipe_length * 0.5; // first large GEM station
  const double zpos1 = 305 - 20; // front of forward ECal/MPC
  const double zpos2 = 335.9 - 10.2 / 2.; // front of the forward field endcap
  const double calorimeter_hole_diamater = 9.92331 *2; // side length of the middle hole of MPC that can hold the piston. Also the max diameter of the piston in that region

  const double beampipe_radius = G4PIPE::be_pipe_radius;

  // teeth cone section specific
  const double number_of_wteeth = 100;
  const double teeth_thickness = 0.3504 * 2; //2 X0
  const double eta_inner = -log(tan(atan((beampipe_radius + 0.1) / zpos0) / 2));
  const double eta_outter = 4.2;
  const double eta_teeth_outter = 4.05;
  double pos = zpos0 + (zpos1 - zpos0) / 2;
//  cout << "MAGNETIC PISTON:" << eta_inner << " " << eta_outter << " " << pos
//      << endl;

  PHG4ConeSubsystem *magpiston = new PHG4ConeSubsystem("Piston", 0);
  magpiston->SetZlength((zpos1 - zpos0) / 2);
  magpiston->SetPlaceZ((zpos1 + zpos0) / 2);
  magpiston->SetR1(beampipe_radius,
      tan(PHG4Sector::Sector_Geometry::eta_to_polar_angle(eta_outter)) * zpos0);
  magpiston->SetR2(beampipe_radius,
      tan(PHG4Sector::Sector_Geometry::eta_to_polar_angle(eta_outter)) * zpos1);
  magpiston->SetMaterial("G4_Fe");
  magpiston->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(magpiston);

//  PHG4ConeSubsystem *magpiston = new PHG4ConeSubsystem(name, 1);
//  magpiston->SetZlength((zpos1 - zpos0) / 2);
//  magpiston->SetPlaceZ(pos);
//  magpiston->Set_eta_range(eta_outter, eta_inner);
//  magpiston->SetMaterial("G4_Fe");
//  magpiston->SuperDetector(name);
//  magpiston->SetActive(false);
//  g4Reco->registerSubsystem(magpiston);

  pos = zpos0 + 1.0 + teeth_thickness / 2;
  for (int i = 0; i < number_of_wteeth; i++)
    {
      stringstream s;
      s << name;
      s << "_teeth_";
      s << i;

      magpiston = new PHG4ConeSubsystem(s.str(), i);
      magpiston->SuperDetector(name);
      magpiston->SetZlength(teeth_thickness / 2);
      magpiston->SetPlaceZ(pos);
      magpiston->SetR1(
          //
          tan(PHG4Sector::Sector_Geometry::eta_to_polar_angle(eta_outter - .01))
              * (pos - teeth_thickness / 2), //
          tan(PHG4Sector::Sector_Geometry::eta_to_polar_angle(eta_teeth_outter))
              * (pos - teeth_thickness / 2) //
              );
      magpiston->SetR2(
          //
          tan(PHG4Sector::Sector_Geometry::eta_to_polar_angle(eta_outter - .01))
              * (pos + teeth_thickness / 2), //
          tan(PHG4Sector::Sector_Geometry::eta_to_polar_angle(eta_outter - .01))
              * (pos + teeth_thickness / 2) + .1 //
              );
      magpiston->SetMaterial("G4_W");
      magpiston->SuperDetector(name);
      if (AbsorberActive) magpiston->SetActive();
      magpiston->OverlapCheck(OverlapCheck);
      g4Reco->registerSubsystem(magpiston);
      pos += ((zpos1 - zpos0 - 10) / number_of_wteeth);
    }

  // last piece connect to the field return
  PHG4CylinderSubsystem *magpiston2 = new PHG4CylinderSubsystem("Piston_EndSection", 0);
  magpiston2->set_double_param("length",zpos2 - zpos1);
  magpiston2->set_double_param("place_z", (zpos2 + zpos1) / 2.);
  magpiston2->set_double_param("radius",beampipe_radius);
  magpiston2->set_double_param("thickness",calorimeter_hole_diamater / 2. - beampipe_radius);
  magpiston2->set_string_param("material","G4_Fe");
  magpiston2->SuperDetector(name);
  if (AbsorberActive) magpiston2->SetActive();
  magpiston2->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(magpiston2);

  return;
}
