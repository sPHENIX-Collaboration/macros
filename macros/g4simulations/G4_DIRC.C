#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include "GlobalVariables.C"
#include <g4detectors/PHG4CylinderSubsystem.h>
#include <g4detectors/PHG4SectorSubsystem.h>
#include <g4main/PHG4Reco.h>
R__LOAD_LIBRARY(libg4detectors.so)
#endif
// $Id: G4_DIRC.C,v 1.3 2013/10/09 01:08:17 jinhuang Exp $

/*!
 * \file G4_DIRC.C
 * \brief Macro setting up the barrel DIRC
 * \author Jin Huang <jhuang@bnl.gov>
 * \version $Revision: 1.3 $
 * \date $Date: 2013/10/09 01:08:17 $
 */

#include <cmath>

void
DIRCInit()
{

}

//! Babar DIRC (Without most of support structure)
//! Ref: I. Adam et al. The DIRC particle identification system for the BaBar experiment.
//! Nucl. Instrum. Meth., A538:281-357, 2005. doi:10.1016/j.nima.2004.08.129.
double
DIRCSetup(PHG4Reco* g4Reco)
{
  const double radiator_R = 83.65;
  const double length = 400;
  const double z_shift = -75; //115
  const double z_start = z_shift + length / 2.;
  const double z_end = z_shift - length / 2.;

  PHG4SectorSubsystem *dirc;
  dirc = new PHG4SectorSubsystem("DIRC");
  dirc->get_geometry().set_normal_polar_angle(3.14159265358979323846/2);
  dirc->get_geometry().set_normal_start(
                                        83.65 * PHG4Sector::Sector_Geometry::Unit_cm());
  dirc->get_geometry().set_min_polar_angle(atan2(radiator_R, z_start));
  dirc->get_geometry().set_max_polar_angle(atan2(radiator_R, z_end));
  dirc->get_geometry().set_min_polar_edge(PHG4Sector::Sector_Geometry::FlatEdge());
  dirc->get_geometry().set_max_polar_edge(PHG4Sector::Sector_Geometry::FlatEdge());
  dirc->get_geometry().set_material("Quartz");
  dirc->get_geometry().set_N_Sector(12);
  dirc->OverlapCheck(overlapcheck);
  dirc->get_geometry().AddLayer("Radiator", "Quartz",
                                1.7 * PHG4Sector::Sector_Geometry::Unit_cm(), true);
  g4Reco->registerSubsystem(dirc);

  PHG4CylinderSubsystem *cyl;

  //  The cylinder skins provide most of the strength
  //  and stiffness of the CST. The thickness of the inner
  //  and outer skins is 1.27 and 0.76 mm, respectively

  // Inner skin:
  cyl = new PHG4CylinderSubsystem("DIRC_CST_Inner_Skin", 10);
  cyl->set_double_param("radius",81.71);
  cyl->set_int_param("lengthviarapidity",0);
  cyl->set_double_param("length",length);
  cyl->set_string_param("material","G4_Al");
  cyl->set_double_param("thickness",0.127);
  cyl->set_double_param("place_x",0.);
  cyl->set_double_param("place_y",0.);
  cyl->set_double_param("place_z",z_shift);
  cyl->SetActive(0);
  cyl->SuperDetector("DIRC");
  cyl->OverlapCheck(overlapcheck);

  g4Reco->registerSubsystem(cyl);

  // Outer skin:
  cyl = new PHG4CylinderSubsystem("DIRC_CST_Outer_Skin", 11);
  cyl->set_double_param("radius",89.25 - 0.076);
  cyl->set_int_param("lengthviarapidity",0);
  cyl->set_double_param("length",length);
  cyl->set_string_param("material","G4_Al");
  cyl->set_double_param("thickness",0.076);
  cyl->set_double_param("place_x",0.);
  cyl->set_double_param("place_y",0.);
  cyl->set_double_param("place_z",z_shift);
  cyl->SetActive(0);
  cyl->SuperDetector("DIRC");
  cyl->OverlapCheck(overlapcheck);

  g4Reco->registerSubsystem(cyl);

  // Done
  return 89.25;

}
