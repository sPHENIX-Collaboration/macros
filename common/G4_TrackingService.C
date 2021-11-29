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
                            const double &thickness_copper,
                            const double &thickness_water,
                            const double &thickness_plastic,
                            const double &thickness_carbon,
                            const double &zSouth,
                            const double &zNorth,
                            const double &rSouth,
                            const double &rNorth);

  virtual ~ServiceStructure(){};

  const string get_name();
  const double get_thickness_copper();
  const double get_thickness_water();
  const double get_thickness_plastic();
  const double get_thickness_carbon();
  const double get_zSouth();
  const double get_zNorth();
  const double get_rSouth();
  const double get_rNorth();

 private:
  const string m_name = "service";
  const double m_thickness_copper = 0.0;
  const double m_thickness_water = 0.0;
  const double m_thickness_plastic = 0.0;
  const double m_thickness_carbon = 0.0;
  const double m_zSouth = 0.0;
  const double m_zNorth = 0.0;
  const double m_rSouth = 0.0;
  const double m_rNorth = 0.0;
};

ServiceStructure::ServiceStructure(const string &name,
                                   const double &thickness_copper,
                                   const double &thickness_water,
                                   const double &thickness_plastic,
                                   const double &thickness_carbon,
                                   const double &zSouth,
                                   const double &zNorth,
                                   const double &rSouth,
                                   const double &rNorth)
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
const double ServiceStructure::get_thickness_copper() { return m_thickness_copper; }
const double ServiceStructure::get_thickness_water() { return m_thickness_water; }
const double ServiceStructure::get_thickness_plastic() { return m_thickness_plastic; }
const double ServiceStructure::get_thickness_carbon() { return m_thickness_carbon; }
const double ServiceStructure::get_zSouth() { return m_zSouth; }
const double ServiceStructure::get_zNorth() { return m_zNorth; }
const double ServiceStructure::get_rSouth() { return m_rSouth; }
const double ServiceStructure::get_rNorth() { return m_rNorth; }

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

  double GlobalOffset = -15.0;
  double BarrelOffset = 18.679;
  double BarrelRadius = 10.33; //Inner radious of service barrel 
  double BarrelThickness = 0.436;  //Thickness in cm
  double BarrelLength = 121.24;    //Length of cylinder in cm
  double LayerThickness = 0.1;    //
  int subsysID = 0;
}  // namespace G4TrackingService

vector<double> get_thickness(ServiceStructure *object)
{
  vector<double> thickness = { object->get_thickness_copper()
                             , object->get_thickness_water()
                             , object->get_thickness_plastic()
                             , object->get_thickness_carbon()};
  return thickness;
}

void TrackingServiceInit()
{
  double serviceRad = G4TrackingService::BarrelRadius + G4TrackingService::BarrelThickness;
  double serviceMinZ = G4TrackingService::GlobalOffset - G4TrackingService::BarrelOffset - G4TrackingService::BarrelLength;
  double serviceMaxZ =  G4TrackingService::GlobalOffset;
  BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, serviceRad);
  BlackHoleGeometry::min_z = std::min(BlackHoleGeometry::min_z, serviceMinZ);
  BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z, serviceMaxZ);
}

double TrackingServiceCone(ServiceStructure *object, PHG4Reco *g4Reco, double radius)
{
  bool AbsorberActive = Enable::ABSORBER || Enable::TrackingService_ABSORBER;
  bool OverlapCheck = Enable::OVERLAPCHECK || Enable::TrackingService_OVERLAPCHECK;
  int verbosity = max(Enable::VERBOSITY, Enable::TrackingService_VERBOSITY);

  PHG4ConeSubsystem *cone;

  double innerRadiusSouth = object->get_rSouth();
  double innerRadiusNorth = object->get_rNorth();
  double length = abs(object->get_zNorth() - object->get_zSouth());
  vector<double> thickness = get_thickness(object);

  for (int i = 0; i < G4TrackingService::nMaterials; ++i)
  {
    if (thickness[i] == 0) continue;
    cone = new PHG4ConeSubsystem(object->get_name(), G4TrackingService::subsysID);
    cone->Verbosity(verbosity);
    cone->SetR1(innerRadiusSouth, innerRadiusSouth + thickness[i]);
    cone->SetR2(innerRadiusNorth, innerRadiusNorth + thickness[i]);
    cone->SetPlaceZ(object->get_zSouth() + length / 2 + G4TrackingService::GlobalOffset);
    cone->SetZlength(length / 2);
    cone->SetMaterial(G4TrackingService::materials[i]);
    cone->SuperDetector("TrackingService");
    if (AbsorberActive) cone->SetActive();
    cone->OverlapCheck(OverlapCheck);
    g4Reco->registerSubsystem(cone);
    ++G4TrackingService::subsysID;
    innerRadiusSouth += thickness[i];
    innerRadiusNorth += thickness[i];
  }
  radius = max(innerRadiusSouth, innerRadiusNorth);

  return radius;
}

double TrackingServiceCylinder(ServiceStructure *object, PHG4Reco *g4Reco, double radius)
{
  bool AbsorberActive = Enable::ABSORBER || Enable::TrackingService_ABSORBER;
  bool OverlapCheck = Enable::OVERLAPCHECK || Enable::TrackingService_OVERLAPCHECK;
  int verbosity = max(Enable::VERBOSITY, Enable::TrackingService_VERBOSITY);

  PHG4CylinderSubsystem *cyl;

  double innerRadius = object->get_rSouth();
  double length = abs(object->get_zNorth() - object->get_zSouth());
  vector<double> thickness = get_thickness(object);

  for (int i = 0; i < G4TrackingService::nMaterials; ++i)
  {
    if (thickness[i] == 0) continue;
    cyl = new PHG4CylinderSubsystem(object->get_name(), G4TrackingService::subsysID);
    cyl->Verbosity(verbosity);
    cyl->set_double_param("place_z", object->get_zSouth() + length / 2 + G4TrackingService::GlobalOffset);
    cyl->set_double_param("radius", innerRadius);
    cyl->set_double_param("length", length);
    cyl->set_string_param("material", G4TrackingService::materials[i]);
    cyl->set_double_param("thickness", thickness[i]);
    cyl->SuperDetector("TrackingService");
    if (AbsorberActive) cyl->SetActive();
    cyl->OverlapCheck(OverlapCheck);
    g4Reco->registerSubsystem(cyl);
    ++G4TrackingService::subsysID;
    innerRadius += thickness[i];
  }
  radius = innerRadius;

  return radius;
}

double TrackingService(PHG4Reco *g4Reco, double radius)
{
  vector<ServiceStructure *> cylinders, cones;

  cylinders.push_back(new ServiceStructure("MVTXServiceBarrel", 0.049946, 0.00724494, 0.313467, G4TrackingService::BarrelThickness, -1. * (G4TrackingService::BarrelLength + G4TrackingService::BarrelOffset), 
                                           -1. * G4TrackingService::BarrelOffset, G4TrackingService::BarrelRadius, 0));

  cylinders.push_back(new ServiceStructure("L0_1", 0.00463332, 0., 0.0662175, G4TrackingService::LayerThickness, -18.680, -16.579, 5.050, 0));
  cones.push_back(new ServiceStructure("L0_2", 0.006, 0., 0.088, G4TrackingService::LayerThickness, -16.579, -9.186, 5.050, 2.997));
  cylinders.push_back(new ServiceStructure("L0_3", 0.00780066, 0., 0.11028, G4TrackingService::LayerThickness, -9.186, 0, 2.997, 0));

  cylinders.push_back(new ServiceStructure("L1_1", 0.00425224, 0., 0.0609067, G4TrackingService::LayerThickness, -17.970, -15.851, 7.338, 0));
  cones.push_back(new ServiceStructure("L1_2", 0.006, 0., 0.085, G4TrackingService::LayerThickness, -15.851, -8.938, 7.338, 3.799));
  cylinders.push_back(new ServiceStructure("L1_3", 0.00820698, 0., 0.116351, G4TrackingService::LayerThickness, -8.938, 0, 3.799, 0));

  cylinders.push_back(new ServiceStructure("L2_1", 0.00404216, 0., 0.0579591, G4TrackingService::LayerThickness, -22.300, -15.206, 9.650, 0));
  cones.push_back(new ServiceStructure("L2_2", 0.0062, 0., 0.09, G4TrackingService::LayerThickness, -15.206, -8.538, 9.650, 4.574));
  cylinders.push_back(new ServiceStructure("L2_3", 0.0085218, 0., 0.121045, G4TrackingService::LayerThickness, -8.538, 0, 4.574, 0));

  for (ServiceStructure *cylinder : cylinders) radius += TrackingServiceCylinder(cylinder, g4Reco, radius);
  for (ServiceStructure *cone : cones) radius += TrackingServiceCone(cone, g4Reco, radius);

  return radius;
}

#endif
