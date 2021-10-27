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
                            const double &thickness_aluminum,
                            const double &thickness_carbon,
                            const double &zSouth,
                            const double &zNorth,
                            const double &rSouth,
                            const double &rNorth);

  virtual ~ServiceStructure(){};

  const string get_name();
  const double get_thickness_aluminum();
  const double get_thickness_carbon();
  const double get_zSouth();
  const double get_zNorth();
  const double get_rSouth();
  const double get_rNorth();

 private:
  const string m_name = "service";
  const double m_thickness_aluminum = 0.0;
  const double m_thickness_carbon = 0.0;
  const double m_zSouth = 0.0;
  const double m_zNorth = 0.0;
  const double m_rSouth = 0.0;
  const double m_rNorth = 0.0;
};

ServiceStructure::ServiceStructure(const string &name,
                                   const double &thickness_aluminum,
                                   const double &thickness_carbon,
                                   const double &zSouth,
                                   const double &zNorth,
                                   const double &rSouth,
                                   const double &rNorth)
  : m_name(name)
  , m_thickness_aluminum(thickness_aluminum)
  , m_thickness_carbon(thickness_carbon)
  , m_zSouth(zSouth)
  , m_zNorth(zNorth)
  , m_rSouth(rSouth)
  , m_rNorth(rNorth)
{
}

const string ServiceStructure::get_name() { return m_name; }
const double ServiceStructure::get_thickness_aluminum() { return m_thickness_aluminum; }
const double ServiceStructure::get_thickness_carbon() { return m_thickness_carbon; }
const double ServiceStructure::get_zSouth() { return m_zSouth; }
const double ServiceStructure::get_zNorth() { return m_zNorth; }
const double ServiceStructure::get_rSouth() { return m_rSouth; }
const double ServiceStructure::get_rNorth() { return m_rNorth; }

class Cable
{
 public:
  Cable();

  explicit Cable(const string &name,
                 const string &coreMaterial,
                 const double &coreRadius,
                 const double &sheathRadius,
                 const double &xSouth, const double &xNorth,
                 const double &ySouth, const double &yNorth,
                 const double &zSouth, const double &zNorth);

  virtual ~Cable(){};

  const string get_name();
  const string get_coreMaterial();
  const double get_coreRadius();
  const double get_sheathRadius();
  const double get_xSouth();
  const double get_xNorth();
  const double get_ySouth();
  const double get_yNorth();
  const double get_zSouth();
  const double get_zNorth();

 private:
  const string m_name = "cable";
  const string m_coreMaterial = "G4_Cu";
  const double m_coreRadius = 1;
  const double m_sheathRadius = 2;
  const double m_xSouth = 0.;
  const double m_xNorth = 1.;
  const double m_ySouth = 0.;
  const double m_yNorth = 1.;
  const double m_zSouth = 0.;
  const double m_zNorth = 1.;
};

Cable::Cable(const string &name,
             const string &coreMaterial,
             const double &coreRadius,
             const double &sheathRadius,
             const double &xSouth, const double &xNorth,
             const double &ySouth, const double &yNorth,
             const double &zSouth, const double &zNorth)
  : m_name(name)
  , m_coreMaterial(coreMaterial)
  , m_coreRadius(coreRadius)
  , m_sheathRadius(sheathRadius)
  , m_xSouth(xSouth)
  , m_xNorth(xNorth)
  , m_ySouth(ySouth)
  , m_yNorth(yNorth)
  , m_zSouth(zSouth)
  , m_zNorth(zNorth)
{
}

const string Cable::get_name() { return m_name; }
const string Cable::get_coreMaterial() { return m_coreMaterial; }
const double Cable::get_coreRadius() { return m_coreRadius; }
const double Cable::get_sheathRadius() { return m_sheathRadius; }
const double Cable::get_xSouth() { return m_xSouth; }
const double Cable::get_xNorth() { return m_xNorth; }
const double Cable::get_ySouth() { return m_ySouth; }
const double Cable::get_yNorth() { return m_yNorth; }
const double Cable::get_zSouth() { return m_zSouth; }
const double Cable::get_zNorth() { return m_zNorth; }

namespace Enable
{
  bool TrackingService = false;
  bool TrackingService_ABSORBER = false;
  bool TrackingService_OVERLAPCHECK = false;
  int TrackingService_VERBOSITY = 0;

}  // namespace Enable

namespace G4TrackingService
{  //List materials and radiation length in cm
  string materials[] = {"G4_Al", "PEEK"};
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
  vector<double> thickness = {object->get_thickness_aluminum(), object->get_thickness_carbon()};
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

double CreateCable(Cable *object, PHG4Reco *g4Reco, double radius)
{
  bool AbsorberActive = Enable::ABSORBER || Enable::TrackingService_ABSORBER;
  bool OverlapCheck = Enable::OVERLAPCHECK || Enable::TrackingService_OVERLAPCHECK;
  int verbosity = max(Enable::VERBOSITY, Enable::TrackingService_VERBOSITY);

  string cableMaterials[2] = {object->get_coreMaterial(), "G4_POLYETHYLENE"};
  double IR[2] = {0, object->get_coreRadius()};
  double OR[2] = {object->get_coreRadius(), object->get_sheathRadius()};

  double length = abs(object->get_zNorth() - object->get_zSouth());
  double setX = (object->get_xSouth() + object->get_xNorth()) / 2;
  double setY = (object->get_ySouth() + object->get_yNorth()) / 2;
  double setZ = (object->get_zSouth() + object->get_zNorth()) / 2;

  double radToDeg = 180.0 / M_PI;
  double rotX = tan((object->get_yNorth() - object->get_ySouth()) / (object->get_zNorth() - object->get_zSouth())) * radToDeg;
  double rotY = tan((object->get_xNorth() - object->get_xSouth()) / (object->get_zNorth() - object->get_zSouth())) * radToDeg;
  double rotZ = tan((object->get_xNorth() - object->get_xSouth()) / (object->get_yNorth() - object->get_ySouth())) * radToDeg;

  PHG4CylinderSubsystem *cyl;
  for (int i = 0; i < 2; ++i)
  {
    cyl = new PHG4CylinderSubsystem(object->get_name(), G4TrackingService::subsysID);
    cyl->Verbosity(verbosity);
    cyl->set_string_param("material", cableMaterials[i]);
    cyl->set_double_param("length", length);
    cyl->set_double_param("radius", IR[i]);
    cyl->set_double_param("thickness", OR[i]);
    cyl->set_double_param("place_x", setX);
    cyl->set_double_param("place_y", setY);
    cyl->set_double_param("place_z", setZ);
    cyl->set_double_param("rot_x", rotX);
    cyl->set_double_param("rot_y", rotY);
    cyl->set_double_param("rot_z", rotZ);
    cyl->SuperDetector("TrackingService");
    if (AbsorberActive) cyl->SetActive();
    cyl->OverlapCheck(OverlapCheck);
    g4Reco->registerSubsystem(cyl);
    ++G4TrackingService::subsysID;
  }
  return OR[1];
}

double CreateCableBundle(string superName, PHG4Reco *g4Reco, double radius,
                         bool enableSignal, bool enableCooling, bool enablePower,
                         double x1, double x2, double y1, double y2, double z1, double z2)
{
  //Set up basic MVTX cable bundle (24 Samtec cables, 1 power cable, 2 cooling cables)
  double samtecCoreRadius = 0.1275;
  double samtecSheathRadius = 0.05;
  double coolingCoreRadius = 0.056;
  double coolingSheathRadius = 0.08;  //?
  double powerLargeCoreRadius = 0.069;
  double powerLargeSheathRadius = 0.158;
  double powerMediumCoreRadius = 0.033;
  double powerMediumSheathRadius = 0.073;
  double powerSmallCoreRadius = 0.028;
  double powerSmallSheathRadius = 0.028;

  //Samtec cables (we use 24 as there are 12 twinax)
  if (enableSignal)
  {
    unsigned int nSamtecWires = 24;
    unsigned int nRow = 2;
    unsigned int nCol = nSamtecWires / nRow;
    for (unsigned int iRow = 0; iRow < nRow; ++iRow)
    {
      for (unsigned int iCol = 0; iCol < nCol; ++iCol)
      {
        Cable *cable = new Cable(boost::str(boost::format("%1%_samtec_%2%_%3%") % superName % iRow % iCol), "G4_Cu", samtecCoreRadius, samtecSheathRadius,
                                 x1 + (iCol + 1) * samtecSheathRadius * 2, x2 + (iCol + 1) * samtecSheathRadius * 2,
                                 y1 + -1 * (iRow + 1) * samtecSheathRadius * 2, y2 + -1 * (iRow + 1) * samtecSheathRadius * 2,
                                 z1, z2);
        radius += CreateCable(cable, g4Reco, radius);
      }
    }
  }

  //Cooling Cables
  if (enableCooling)
  {
    unsigned int nCool = 2;
    for (unsigned int iCool = 0; iCool < nCool; ++iCool)
    {
      Cable *cable = new Cable(boost::str(boost::format("%1%_cooling_%2%") % superName % iCool), "G4_WATER", coolingCoreRadius, coolingSheathRadius,
                               x1 + (iCool + 1) * coolingSheathRadius * 2, x2 + (iCool + 1) * coolingSheathRadius * 2,
                               y1 + (iCool + 1) * coolingSheathRadius * 2, y2 + (iCool + 1) * coolingSheathRadius * 2,
                               z1, z2);
      radius += CreateCable(cable, g4Reco, radius);
    }
  }

  //Power Cables
  if (enablePower)
  {
    typedef pair<pair<string, string>, pair<double, double>> PowerCableParameters;
    vector<PowerCableParameters> powerCables;

    powerCables.push_back(make_pair(make_pair(boost::str(boost::format("%1%_digiReturn") % superName), "Large"), make_pair(-1 * powerLargeSheathRadius, -1 * powerLargeSheathRadius)));
    powerCables.push_back(make_pair(make_pair(boost::str(boost::format("%1%_digiSupply") % superName), "Large"), make_pair(-3 * powerLargeSheathRadius, powerLargeSheathRadius)));
    powerCables.push_back(make_pair(make_pair(boost::str(boost::format("%1%_anaReturn") % superName), "Medium"), make_pair(-1 * (powerMediumSheathRadius + 2 * powerLargeSheathRadius), -2 * powerMediumSheathRadius)));
    powerCables.push_back(make_pair(make_pair(boost::str(boost::format("%1%_anaSupply") % superName), "Medium"), make_pair(-1 * (3 * powerMediumSheathRadius + 2 * powerLargeSheathRadius), -1 * powerMediumSheathRadius)));
    powerCables.push_back(make_pair(make_pair(boost::str(boost::format("%1%_digiSense") % superName), "Small"), make_pair(-1 * (2 * powerMediumSheathRadius + 4 * powerLargeSheathRadius), powerSmallSheathRadius)));
    powerCables.push_back(make_pair(make_pair(boost::str(boost::format("%1%_anaSense") % superName), "Small"), make_pair(-4 * powerLargeSheathRadius, 2 * powerSmallSheathRadius)));
    powerCables.push_back(make_pair(make_pair(boost::str(boost::format("%1%_bias") % superName), "Small"), make_pair(-2 * powerLargeSheathRadius, powerLargeSheathRadius)));
    powerCables.push_back(make_pair(make_pair(boost::str(boost::format("%1%_ground") % superName), "Small"), make_pair(-1 * powerLargeSheathRadius, powerSmallSheathRadius)));

    for (PowerCableParameters &powerCable : powerCables)
    {
      double coreRad, sheathRad;
      string cableType = powerCable.first.second;
      if (cableType == "Small")
      {
        coreRad = powerSmallCoreRadius;
        sheathRad = powerSmallSheathRadius;
      }
      else if (cableType == "Medium")
      {
        coreRad = powerMediumCoreRadius;
        sheathRad = powerMediumSheathRadius;
      }
      else
      {
        coreRad = powerLargeCoreRadius;
        sheathRad = powerLargeSheathRadius;
      }

      Cable *cable = new Cable(powerCable.first.first, "G4_Cu", coreRad, sheathRad,
                               x1 + powerCable.second.first, x2 + powerCable.second.first,
                               y1 + powerCable.second.second, y2 + powerCable.second.second, z1, z2);

      radius += CreateCable(cable, g4Reco, radius);
    }
  }

  return radius;
}

double TrackingService(PHG4Reco *g4Reco, double radius)
{
  vector<ServiceStructure *> cylinders, cones;


  cylinders.push_back(new ServiceStructure("MVTXServiceBarrel", 0, G4TrackingService::BarrelThickness, -1. * (G4TrackingService::BarrelLength + G4TrackingService::BarrelOffset), 
                                           -1. * G4TrackingService::BarrelOffset, G4TrackingService::BarrelRadius, 0));

  cylinders.push_back(new ServiceStructure("L0_1", 0, G4TrackingService::LayerThickness, -18.680, -16.579, 5.050, 0));
  cones.push_back(new ServiceStructure("L0_2", 0, G4TrackingService::LayerThickness, -16.579, -9.186, 5.050, 2.997));
  cylinders.push_back(new ServiceStructure("L0_3", 0, G4TrackingService::LayerThickness, -9.186, 0, 2.997, 0));

  cylinders.push_back(new ServiceStructure("L1_1", 0, G4TrackingService::LayerThickness, -17.970, -15.851, 7.338, 0));
  cones.push_back(new ServiceStructure("L1_2", 0, G4TrackingService::LayerThickness, -15.851, -8.938, 7.338, 3.799));
  cylinders.push_back(new ServiceStructure("L1_3", 0, G4TrackingService::LayerThickness, -8.938, 0, 3.799, 0));

  cylinders.push_back(new ServiceStructure("L2_1", 0, G4TrackingService::LayerThickness, -22.300, -15.206, 9.650, 0));
  cones.push_back(new ServiceStructure("L2_2", 0, G4TrackingService::LayerThickness, -15.206, -8.538, 9.650, 4.574));
  cylinders.push_back(new ServiceStructure("L2_3", 0, G4TrackingService::LayerThickness, -8.538, 0, 4.574, 0));

  //for (ServiceStructure *cylinder : cylinders) radius += TrackingServiceCylinder(cylinder, g4Reco, radius);
  //for (ServiceStructure *cone : cones) radius += TrackingServiceCone(cone, g4Reco, radius);

  radius += CreateCableBundle("Test", g4Reco, radius, true, true, true, 0, 0, 0, 0, 0, 100);

  return radius;
}

#endif
