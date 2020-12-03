#pragma once

#include "GlobalVariables.C"

#include <g4detectors/PHG4CylinderSubsystem.h>

//#include <g4tpc/PHG4TpcSpaceChargeDistortion.h>

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
  //build barrel1 detector
  //---------------------------------
  const int nLayer1 = 3;   //barrel 1
  const int nLayer2 = 2;   //barrel 2
  int nLayer[2]={nLayer1,nLayer2}; 
  
  const int nSubLayer = 7;
  const float um = 0.0001;  //convert um to cm

  double r[2][3] = {{3.64, 4.81, 5.98},   //cm, barrel1
		    {16, 22.0}};          //barrel 2

  double halfLength[2][3] = {{20, 20, 25},  //cm, barrel 1
			     {25,25}};      //barrel 2

  string layerName[nSubLayer] = {"SiliconSensor", "Metalconnection", "HDI", "Cooling",
                                 "Support1", "Support_Gap", "Support2"};
  string material[nSubLayer] = {"G4_Si", "G4_Al", "G4_KAPTON", "G4_WATER",
                                "G4_GRAPHITE", "G4_AIR", "G4_GRAPHITE"};
  double thickness[nSubLayer] = {35 * um, 15 * um, 20 * um, 100 * um,
                                 50 * um, 1, 50 * um};

  int k = 0;  //layer id. Must be unique.
  double max_bh_radius = 0.;
  PHG4CylinderSubsystem* cyl[2];
  for (int n = 0; n < 2; n++) {   //barrel1, barrel2
    for (int i = 0; i < nLayer[n]; i++)
      {
	if (n==1) thickness[0]=85*um;

	for (int j = 0; j < nSubLayer; j++)
	  {
	    //cyl[n] = new PHG4CylinderSubsystem("Barrel"+ n +"_" + + layerName[j], k);
	    cyl[n] = new PHG4CylinderSubsystem(Form("Barrel%d_%s",n,layerName[j].c_str()), k);
	    if (j == 0)
	      {
		cyl[n]->set_double_param("radius", r[n][i]);
	      }
	    else
	      {
		cyl[n]->set_double_param("radius", r[n][i] + thickness[j - 1]);
	      }
	    cyl[n]->set_double_param("length", 2.0 * halfLength[n][i]);
	    cyl[n]->set_string_param("material", material[j]);
	    cyl[n]->set_double_param("thickness", thickness[j]);
	    max_bh_radius = std::max(max_bh_radius, (r[n][i] + thickness[j - 1] + thickness[j]));
	    cyl[n]->SuperDetector(Form("BARREL%d",n));
	    if (j == 0)
	      {
		cyl[n]->SetActive();  //only the Silicon Sensor is active
	      }
	    else
	      {
		if (AbsorberActive) cyl[n]->SetActive();
	      }
	    g4Reco->registerSubsystem(cyl[n]);
	    k++;
	  }
      }
  }

  // update now that we know the outer radius
  BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, max_bh_radius);
  BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z, halfLength[1][0]);
  BlackHoleGeometry::min_z = std::min(BlackHoleGeometry::min_z, -halfLength[0][0]);
  return max_bh_radius;
}
//---------------------------------------------------------------------//
