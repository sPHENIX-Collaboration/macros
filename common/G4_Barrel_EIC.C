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

int make_barrel_pixel_layer(const string &name, PHG4Reco *g4Reco,
                            double radius, double halflength, double tSilicon, double tAirgap);

//---------------------------------------------------------------------//
namespace Enable
{
  bool BARREL = false;
  bool BARREL_ABSORBER = false;
}  // namespace Enable

namespace G4BARREL
{
  namespace SETTING
  {
    bool BARRELV0 = false;
    bool BARRELV1 = false;
    bool BARRELV2 = false;
    bool BARRELV3 = false;
    bool BARRELV4 = false;
  }  // namespace SETTING
}  // namespace G4BARREL
//---------------------------------------------------------------------//
void BarrelInit()
{
  if ((G4BARREL::SETTING::BARRELV0 ? 1 : 0) +
      (G4BARREL::SETTING::BARRELV1 ? 1 : 0) +
      (G4BARREL::SETTING::BARRELV2 ? 1 : 0) +
      (G4BARREL::SETTING::BARRELV3 ? 1 : 0) +
      (G4BARREL::SETTING::BARRELV4 ? 1 : 0) > 1)
  {
    cout << "use only G4BARREL::SETTING::BARRELV0=true or G4BARREL::SETTING::BARRELV1=true or G4BARREL::SETTING::BARRELV2=true or G4BARREL::SETTING::BARRELV3=true or G4BARREL::SETTING::BARRELV4=true" << endl;

    gSystem->Exit(1);
  }
}

//---------------------------------------------------------------------//
double Barrel(PHG4Reco *g4Reco, double radius)
{
  const bool AbsorberActive = Enable::ABSORBER || Enable::BARREL_ABSORBER;

  //---------------------------------
  //build barrel detector
  //---------------------------------
  int nLayer = 5;
  const float um = 0.0001;  //convert um to cm

  // Different Barrel versions documented in arXiv:2009.0288
  double r[6] = {3.64, 4.81, 5.98, 16.0, 22.0, -1};  //cm
  double halfLength[6] = {20, 20, 25, 25, 25, 25};   //cm
  double tSilicon[6] = {100 * um, 100 * um, 100 * um, 100 * um, 100 * um, 100 * um};
  double tAirgap[6] = {0.9, 0.9, 1, 1, 1, 1};
  
  if (G4BARREL::SETTING::BARRELV1 || G4BARREL::SETTING::BARRELV2)
  {
    for (Int_t i = 0; i < 3; i++) tSilicon[i] = 50 * um;
  }
  else if (G4BARREL::SETTING::BARRELV3)
  {
    for (Int_t i = 0; i < 5; i++) tSilicon[i] = 35 * um;
  }
  else if (G4BARREL::SETTING::BARRELV4)
  {
    for (Int_t i = 0; i < 3; i++) tSilicon[i] = 50 * um;
    nLayer = 6;
    r[3] = 9.2;
    r[4] = 17.;
    r[5] = 27.;
  }

  double max_bh_radius = 0.;
  for (int i = 0; i < nLayer; i++)
  {
    make_barrel_pixel_layer(Form("BARREL_%d", i), g4Reco, r[i], halfLength[i], tSilicon[i], tAirgap[i]);
    max_bh_radius = r[i] + 1.5;
    //     std::cout << "done with barrel layer intialization at "<< r[i] << std::endl;
  }

  // update now that we know the outer radius
  BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, max_bh_radius);
  BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z, halfLength[nLayer - 1]);
  BlackHoleGeometry::min_z = std::min(BlackHoleGeometry::min_z, -halfLength[nLayer - 1]);
  return max_bh_radius;
}
//---------------------------------------------------------------------//
//-----------------------------------------------------------------------------------//
int make_barrel_pixel_layer(const string &name, PHG4Reco *g4Reco,
                            double radius, double halflength, double tSilicon, double tAirgap)
{
  //---------------------------------
  //build barrel layer
  //---------------------------------
  const int nSubLayer = 7;
  const double cm = PHG4Sector::Sector_Geometry::Unit_cm();
  const double mm = .1 * cm;
  const double um = 1e-3 * mm;

  string layerName[nSubLayer] = {"SiliconSensor", "Metalconnection", "HDI", "Cooling",
                                 "Support1", "Support_Gap", "Support2"};
  string material[nSubLayer] = {"G4_Si", "G4_Al", "G4_KAPTON", "G4_WATER",
                                "G4_GRAPHITE", "G4_AIR", "G4_GRAPHITE"};
  double thickness[nSubLayer] = {tSilicon, 15 * um, 20 * um, 100 * um,
                                 50 * um, tAirgap, 50 * um};

  double max_bh_radius = 0.;
  PHG4CylinderSubsystem *cyl;
  cout << "started to create cylinder layer: " << name << endl;

  double currRadius = radius;
//   cout << currRadius << endl;
  for (int l = 0; l < nSubLayer; l++)
  {
    cout << name << "_" << layerName[l] << "\t" << currRadius ;
    cyl = new PHG4CylinderSubsystem(name + "_" + layerName[l], l);
    cyl->SuperDetector(name);
    cyl->set_double_param("radius", currRadius);
    cyl->set_double_param("length", 2.0 * halflength);
    cyl->set_string_param("material", material[l]);
    cyl->set_double_param("thickness", thickness[l]);
    if (l == 0) cyl->SetActive();  //only the Silicon Sensor is active
    cyl->OverlapCheck(true);
    g4Reco->registerSubsystem(cyl);
    currRadius = currRadius + thickness[l];
    cout << "\t" << currRadius << endl;
  }
  

  return 0;
}

//-----------------------------------------------------------------------------------//

#endif
