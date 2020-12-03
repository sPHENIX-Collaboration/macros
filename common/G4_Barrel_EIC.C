#ifndef MACRO_G4BARRELEIC_C
#define MACRO_G4BARRELEIC_C

#include <GlobalVariables.C>

#include <g4detectors/PHG4CylinderSubsystem.h>

#include <g4main/PHG4Reco.h>

#include <fun4all/Fun4AllServer.h>

#include <cmath>
#include <vector>

R__LOAD_LIBRARY(libg4eval.so)
R__LOAD_LIBRARY(libg4mvtx.so)
//---------------------------------------------------------------------//
namespace Enable
{
  bool BARREL = false;
  bool BARREL_ABSORBER = false;
}  // namespace Enable
//---------------------------------------------------------------------//
void BarrelInit()
{
}
//---------------------------------------------------------------------//
double Barrel(PHG4Reco* g4Reco, double radius)
{
  const bool AbsorberActive = Enable::ABSORBER || Enable::BARREL_ABSORBER;

  //---------------------------------
  //build barrel detector
  //---------------------------------
  const int nLayer = 5;
  const int nSubLayer = 7;
  const float um = 0.0001;  //convert um to cm

  double r[nLayer] = {3.64, 4.81, 5.98, 16.0, 22.0};  //cm
  double halfLength[nLayer] = {20, 20, 25, 25, 25};   //cm

  string layerName[nSubLayer] = {"SiliconSensor", "Metalconnection", "HDI", "Cooling",
                                 "Support1", "Support_Gap", "Support2"};
  string material[nSubLayer] = {"G4_Si", "G4_Al", "G4_KAPTON", "G4_WATER",
                                "G4_GRAPHITE", "G4_AIR", "G4_GRAPHITE"};
  double thickness[nSubLayer] = {285 * um, 15 * um, 20 * um, 100 * um,
                                 50 * um, 1, 50 * um};

  int k = 0;  //layer id. Must be unique.
  double max_bh_radius = 0.;
  PHG4CylinderSubsystem* cyl;
  for (int i = 0; i < nLayer; i++)
  {
    //if (i>2) continue;
    for (int j = 0; j < nSubLayer; j++)
    {
      cyl = new PHG4CylinderSubsystem("Barrel_" + layerName[j], k);
      if (j == 0)
      {
        cyl->set_double_param("radius", r[i]);
      }
      else
      {
        cyl->set_double_param("radius", r[i] + thickness[j - 1]);
      }
      cyl->set_double_param("length", 2.0 * halfLength[i]);
      cyl->set_string_param("material", material[j]);
      cyl->set_double_param("thickness", thickness[j]);
      max_bh_radius = std::max(max_bh_radius, (r[i] + thickness[j - 1] + thickness[j]));
      cyl->SuperDetector("BARREL");
      if (j == 0)
      {
        cyl->SetActive();  //only the Silicon Sensor is active
      }
      else
      {
        if (AbsorberActive) cyl->SetActive();
      }
      g4Reco->registerSubsystem(cyl);
      k++;
    }
  }

  // update now that we know the outer radius
  BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, max_bh_radius);
  BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z, halfLength[nLayer - 1]);
  BlackHoleGeometry::min_z = std::min(BlackHoleGeometry::min_z, -halfLength[nLayer - 1]);
  return max_bh_radius;
}
//---------------------------------------------------------------------//
#endif
