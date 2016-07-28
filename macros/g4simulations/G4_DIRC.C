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
  const double length = 470;
  //const double z_shift = -135; // ePHENIX LOI
  const double z_shift = -65; // shift to fit within flux return doors; NF 1/13/2016
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
  cyl = new PHG4CylinderSubsystem("DIRC_CST_Inner_Skin", 10);
  cyl->SetRadius(81.71);
  cyl->SetLengthViaRapidityCoverage(false);
  cyl->SetMaterial("G4_Al");
  cyl->SetLength(length);
  cyl->SetThickness(0.127);
  cyl->SetPosition(0., 0., z_shift);
  cyl->SetActive(0);
  cyl->SuperDetector("DIRC");
  cyl->OverlapCheck(overlapcheck);
  g4Reco->registerSubsystem(cyl);

  cyl = new PHG4CylinderSubsystem("DIRC_CST_Outer_Skin", 11);
  cyl->SetRadius(89.25 - 0.076);
  cyl->SetLengthViaRapidityCoverage(false);
  cyl->SetMaterial("G4_Al");
  cyl->SetLength(length);
  cyl->SetThickness(0.076);
  cyl->SetPosition(0., 0., z_shift);
  cyl->SetActive(0);
  cyl->SuperDetector("DIRC");
  cyl->OverlapCheck(overlapcheck);
  g4Reco->registerSubsystem(cyl);

  return 89.25;

}
