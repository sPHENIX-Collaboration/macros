#ifndef MACRO_G4TrackingService_C
#define MACRO_G4TrackingService_C

#include <GlobalVariables.C>
#include <QA.C>

#include <g4detectors/PHG4ConeSubsystem.h>
#include <g4detectors/PHG4CylinderSubsystem.h>
#include <g4main/PHG4Reco.h>

#include <qa_modules/QAG4SimulationMvtx.h>

#include <fun4all/Fun4AllServer.h>

#include <boost/format.hpp>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

//sPHENIX Tracking Services

using namespace std;

class ServiceStructure
{
 public:
  ServiceStructure();

  explicit ServiceStructure(const string &name,
                            const float &thickness_copper,
                            const float &thickness_water,
                            const float &thickness_plastic,
                            const float &thickness_carbon,
                            const float &zSouth,
                            const float &zNorth,
                            const float &rSouth,
                            const float &rNorth);

  virtual ~ServiceStructure(){};

  const string get_name();
  const float get_thickness_copper();
  const float get_thickness_water();
  const float get_thickness_plastic();
  const float get_thickness_carbon();
  const float get_zSouth();
  const float get_zNorth();
  const float get_rSouth();
  const float get_rNorth();

 private:
  const string m_name = "service";
  const float m_thickness_copper = 0.0;
  const float m_thickness_water = 0.0;
  const float m_thickness_plastic = 0.0;
  const float m_thickness_carbon = 0.0;
  const float m_zSouth = 0.0;
  const float m_zNorth = 0.0;
  const float m_rSouth = 0.0;
  const float m_rNorth = 0.0;
};

ServiceStructure::ServiceStructure(const string &name,
                                   const float &thickness_copper,
                                   const float &thickness_water,
                                   const float &thickness_plastic,
                                   const float &thickness_carbon,
                                   const float &zSouth,
                                   const float &zNorth,
                                   const float &rSouth,
                                   const float &rNorth)
  : m_name(name)
  , m_thickness_copper(thickness_copper)
  , m_thickness_water(thickness_water)
  , m_thickness_plastic(thickness_plastic)
  , m_thickness_carbon(thickness_carbon)
  , m_zSouth(zSouth)
  , m_zNorth(zNorth)
  , m_rSouth(rSouth)
  , m_rNorth(rNorth)
{
}

const string ServiceStructure::get_name() { return m_name; }
const float ServiceStructure::get_thickness_copper() { return m_thickness_copper; }
const float ServiceStructure::get_thickness_water() { return m_thickness_water; }
const float ServiceStructure::get_thickness_plastic() { return m_thickness_plastic; }
const float ServiceStructure::get_thickness_carbon() { return m_thickness_carbon; }
const float ServiceStructure::get_zSouth() { return m_zSouth; }
const float ServiceStructure::get_zNorth() { return m_zNorth; }
const float ServiceStructure::get_rSouth() { return m_rSouth; }
const float ServiceStructure::get_rNorth() { return m_rNorth; }

namespace Enable
{
  bool TrackingService = false;
  bool TrackingService_ABSORBER = false;
  bool TrackingService_OVERLAPCHECK = false;
  int TrackingService_VERBOSITY = 0;

}  // namespace Enable

namespace G4TrackingService
{  //List materials and radiation length in cm
  string materials[] = {"G4_Cu", "G4_WATER", "G4_POLYETHYLENE", "PEEK"};
  const int nMaterials = sizeof(materials)/sizeof(materials[0]);

  float GlobalOffset = -15.0;
  float BarrelOffset = 18.679;
  float BarrelRadius = 10.33; //Inner radious of service barrel 
  float BarrelThickness = 0.436;  //Thickness in cm
  float BarrelLength = 121.24;    //Length of cylinder in cm
  float LayerThickness = 0.1;     //
  float CYSSConeThickness = 0.216;
  float CYSSRibThickness = 0.170;
  int subsysID = 0;
  std::vector<float> grey = {0.4, 0.4, 0.4};
}  // namespace G4TrackingService

using namespace G4TrackingService;

vector<float> get_thickness(ServiceStructure *object)
{
  vector<float> thickness = { object->get_thickness_copper()
                             , object->get_thickness_water()
                             , object->get_thickness_plastic()
                             , object->get_thickness_carbon()};
  return thickness;
}

void TrackingServiceInit()
{
  double serviceRad = BarrelRadius + BarrelThickness;
  double serviceMinZ = GlobalOffset - BarrelOffset - BarrelLength;
  double serviceMaxZ =  GlobalOffset;
  BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, serviceRad);
  BlackHoleGeometry::min_z = std::min(BlackHoleGeometry::min_z, serviceMinZ);
  BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z, serviceMaxZ);
}

float TrackingServiceCone(ServiceStructure *object, PHG4Reco *g4Reco, float radius)
{
  bool AbsorberActive = Enable::ABSORBER || Enable::TrackingService_ABSORBER;
  bool OverlapCheck = Enable::OVERLAPCHECK || Enable::TrackingService_OVERLAPCHECK;
  int verbosity = max(Enable::VERBOSITY, Enable::TrackingService_VERBOSITY);

  PHG4ConeSubsystem *cone;

  float innerRadiusSouth = object->get_rSouth();
  float innerRadiusNorth = object->get_rNorth();
  float length = abs(object->get_zNorth() - object->get_zSouth());
  vector<float> thickness = get_thickness(object);

  for (int i = 0; i < nMaterials; ++i)
  {
    if (thickness[i] == 0) continue;
    cone = new PHG4ConeSubsystem(object->get_name(), subsysID);
    cone->Verbosity(verbosity);
    cone->SetR1(innerRadiusSouth, innerRadiusSouth + thickness[i]);
    cone->SetR2(innerRadiusNorth, innerRadiusNorth + thickness[i]);
    cone->SetPlaceZ(object->get_zSouth() + length / 2 + GlobalOffset);
    cone->SetZlength(length / 2);
    cone->SetMaterial(materials[i]);
    cone->SuperDetector("TrackingService");
    if (materials[i] == "PEEK") cone->set_color(grey[0], grey[1], grey[2], 1.);
    if (AbsorberActive) cone->SetActive();
    cone->OverlapCheck(OverlapCheck);
    g4Reco->registerSubsystem(cone);
    ++subsysID;
    innerRadiusSouth += thickness[i] + 5e-7;
    innerRadiusNorth += thickness[i] + 5e-7;
  }
  radius = max(innerRadiusSouth, innerRadiusNorth);

  return radius;
}

float TrackingServiceCylinder(ServiceStructure *object, PHG4Reco *g4Reco, float radius)
{
  bool AbsorberActive = Enable::ABSORBER || Enable::TrackingService_ABSORBER;
  bool OverlapCheck = Enable::OVERLAPCHECK || Enable::TrackingService_OVERLAPCHECK;
  int verbosity = max(Enable::VERBOSITY, Enable::TrackingService_VERBOSITY);

  PHG4CylinderSubsystem *cyl;

  float innerRadius = object->get_rSouth();
  float length = abs(object->get_zNorth() - object->get_zSouth());
  vector<float> thickness = get_thickness(object);

  for (int i = 0; i < nMaterials; ++i)
  {
    if (thickness[i] == 0) continue;
    cyl = new PHG4CylinderSubsystem(object->get_name(), subsysID);
    cyl->Verbosity(verbosity);
    cyl->set_double_param("place_z", object->get_zSouth() + length / 2 + GlobalOffset);
    cyl->set_double_param("radius", innerRadius);
    cyl->set_double_param("length", length);
    cyl->set_string_param("material", materials[i]);
    cyl->set_double_param("thickness", thickness[i]);
    cyl->SuperDetector("TrackingService");
    if (materials[i] == "PEEK") cyl->set_color(grey[0], grey[1], grey[2], 1.);
    if (AbsorberActive) cyl->SetActive();
    cyl->OverlapCheck(OverlapCheck);
    g4Reco->registerSubsystem(cyl);
    ++subsysID;
    innerRadius += thickness[i] + 5e-7;
  }
  radius = innerRadius;

  return radius;
}

float TrackingService(PHG4Reco *g4Reco, float radius)
{
  vector<ServiceStructure *> cylinders, cones;

  //Service Barrel
  cylinders.push_back(new ServiceStructure("MVTXServiceBarrel_0", 0.05, 0.007, 0.313, BarrelThickness, -1. * (BarrelLength + BarrelOffset), -27.0, BarrelRadius, 0));
  cylinders.push_back(new ServiceStructure("MVTXServiceBarrel_1", 0, 0, 0, BarrelThickness, -27.0, -1. * BarrelOffset, 10.70 + CYSSConeThickness, 0));

  cones.push_back(new ServiceStructure("connectL0", 0.005, 0., 0.066, 0, -26.9, -18.680, 10.10, 5.050));
  cones.push_back(new ServiceStructure("connectL1", 0.004, 0., 0.061, 0, -26.9, -18.000, 10.20, 7.338));
  cones.push_back(new ServiceStructure("connectL2", 0.004, 0., 0.058, 0, -26.9, -22.300, 10.30, 9.580));

  //MVTX Layers
  cylinders.push_back(new ServiceStructure("L0_0", 0.005, 0., 0.066, LayerThickness, -18.680, -16.579, 5.050, 0));
  cones.push_back(new ServiceStructure("L0_1", 0.006, 0., 0.088, LayerThickness, -16.578, -9.186, 5.050, 2.997));
  cylinders.push_back(new ServiceStructure("L0_2", 0.008, 0., 0.110, LayerThickness, -9.185, 0, 2.997, 0));

  cylinders.push_back(new ServiceStructure("L1_0", 0.004, 0., 0.061, LayerThickness, -17.970, -15.851, 7.338, 0));
  cones.push_back(new ServiceStructure("L1_1", 0.006, 0., 0.085, LayerThickness, -15.850, -8.938, 7.338, 3.799));
  cylinders.push_back(new ServiceStructure("L1_2", 0.008, 0., 0.116, LayerThickness, -8.937, 0, 3.799, 0));

  cylinders.push_back(new ServiceStructure("L2_0", 0.004, 0., 0.058, LayerThickness, -22.300, -15.206, 9.580, 0));
  cones.push_back(new ServiceStructure("L2_1", 0.006, 0., 0.09, LayerThickness, -15.205, -8.538, 9.650, 4.574));
  cylinders.push_back(new ServiceStructure("L2_2", 0.009, 0., 0.121, LayerThickness, -8.537, 0, 4.574, 0));

  //CYSS
  cylinders.push_back(new ServiceStructure("CYSS_Cone_0", 0, 0, 0, CYSSConeThickness, -26.208, -15.68, 10.55, 0));
  cones.push_back(new ServiceStructure("CYSS_Cone_1", 0, 0, 0, CYSSConeThickness, -15.679, -8.619, 10.55, 5.302));
  cylinders.push_back(new ServiceStructure("CYSS_Cone_2", 0, 0, 0, CYSSConeThickness, -8.618, -6.18, 5.302, 0));

  cylinders.push_back(new ServiceStructure("CYSS_Rib_0", 0, 0, 0, CYSSRibThickness, -21.719, -20.949, 9.762, 0));
  cones.push_back(new ServiceStructure("CYSS_Rib_1", 0, 0, 0, CYSSRibThickness, -20.948, -20.159, 9.762, 10.36));
  cylinders.push_back(new ServiceStructure("CYSS_Rib_2", 0, 0, 0, CYSSRibThickness, -20.158, -17.749, 10.36, 0));
  cones.push_back(new ServiceStructure("CYSS_Rib_3", 0, 0, 0, CYSSRibThickness, -17.748, -16.959, 10.36, 9.762));
  cylinders.push_back(new ServiceStructure("CYSS_Rib_4", 0, 0, 0, CYSSRibThickness, -16.958, -16.196, 9.762, 0));

  cylinders.push_back(new ServiceStructure("CYSS_Cylinder", 0, 0, 0, 0.112, -8.619, 36.153, 5.15, 0));

  for (ServiceStructure *cylinder : cylinders) radius += TrackingServiceCylinder(cylinder, g4Reco, radius);
  for (ServiceStructure *cone : cones) radius += TrackingServiceCone(cone, g4Reco, radius);

  return radius;
}

#endif
