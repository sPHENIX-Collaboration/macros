/*--------------------------------------------------------------------*
 * 06-15-2020                                                         *
 * Ping: Base on G4_TPC_EIC.C                                         *
 *--------------------------------------------------------------------*/
#pragma once

#include "GlobalVariables.C"
#include <g4detectors/PHG4CylinderSubsystem.h>
#include <g4tpc/PHG4TpcSpaceChargeDistortion.h>
#include <g4main/PHG4Reco.h>
#include <fun4all/Fun4AllServer.h>
#include <cmath>
#include <vector>

R__LOAD_LIBRARY(libg4eval.so)
R__LOAD_LIBRARY(libg4mvtx.so)
//---------------------------------------------------------------------//
namespace Enable
{
  static bool BARREL = false;
}
//---------------------------------------------------------------------//
namespace G4Barrel
{
}  // namespace G4Barrel
//---------------------------------------------------------------------//
void BarrelInit(int verbosity = 0)
{
}
//---------------------------------------------------------------------//
double Barrel(PHG4Reco* g4Reco, double radius,
	      const int absorberactive = 0,
	      int verbosity = 0)
{
  int i,j;

  //---------------------------------
  //build barrel detector
  //---------------------------------
  const int nLayer=5;
  const int nSubLayer=7;
  const float um=0.0001;                              //convert um to cm

  double r[nLayer]={3.64, 4.81, 5.98, 16.0, 22.0};    //cm
  double halfLength[nLayer]={20, 20, 25, 25, 25};     //cm
  
  string layerName[nSubLayer]={"SiliconSensor","Metalconnection","HDI","Cooling",
			       "Support1","Support_Gap","Support2"};
  string material[nSubLayer]={"G4_Si","G4_Al","G4_KAPTON","G4_WATER",
			      "G4_GRAPHITE","G4_AIR","G4_GRAPHITE"};
  double thickness[nSubLayer]={285 * um, 15 * um, 20 * um, 100 * um,
			       50 * um, 1, 50 * um};
  
  int k=0;  //layer id. Must be unique.
  PHG4CylinderSubsystem* cyl;  
  for (i=0;i<nLayer;i++) {
    //if (i>2) continue;
    for (j=0;j<nSubLayer;j++) {
      cyl = new PHG4CylinderSubsystem(Form("Barrel_%s", layerName[j].c_str()),k);
      if (j==0) cyl->set_double_param("radius", r[i]);
      else cyl->set_double_param("radius", r[i]+thickness[j-1]);

      cyl->set_double_param("length", 2.0*halfLength[i]);
      cyl->set_string_param("material", material[j].c_str());
      cyl->set_double_param("thickness", thickness[j]);
      cyl->SuperDetector("BARREL");
      if (j==0) cyl->SetActive();    //only the Silicon Sensor is active

      g4Reco->registerSubsystem(cyl);
      k++;
    }
  }

  //fst->get_geometry().AddLayer("SiliconSensor", "G4_Si", 285 * um, true, 100);
  //fst->get_geometry().AddLayer("Metalconnection", "G4_Al", 15 * um, false, 100);
  //fst->get_geometry().AddLayer("HDI", "G4_KAPTON", 20 * um, false, 100);
  //fst->get_geometry().AddLayer("Cooling", "G4_WATER", 100 * um, false, 100);
  //fst->get_geometry().AddLayer("Support", "G4_GRAPHITE", 50 * um, false, 100);
  //fst->get_geometry().AddLayer("Support_Gap", "G4_AIR", 1 * cm, false, 100);
  //fst->get_geometry().AddLayer("Support2", "G4_GRAPHITE", 50 * um, false, 100);

  // update now that we know the outer radius
  //BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, radius);
  //return radius;
  return r[nLayer-1];
}
//---------------------------------------------------------------------//
