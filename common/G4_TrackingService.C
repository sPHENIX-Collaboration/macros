#ifndef MACRO_G4TrackingService_C
#define MACRO_G4TrackingService_C

#include <GlobalVariables.C>
#include <QA.C>

#include <g4detectors/PHG4ConeSubsystem.h>
#include <g4detectors/PHG4CylinderSubsystem.h>
#include <g4main/PHG4Reco.h>

#include <qa_modules/QAG4SimulationMvtx.h>

#include <fun4all/Fun4AllServer.h>

#include <cmath>
#include <vector>

//sPHENIX Tracking Services

using namespace std;

class ServiceStructure
{
  public:
   ServiceStructure();

    explicit ServiceStructure(const string &name,
                              const double &rad_len_aluminum,
                              const double &rad_len_carbon,
                              const double &zSouth,
                              const double &zNorth,
                              const double &rSouth,
                              const double &rNorth);

    virtual ~ServiceStructure(){};

    const string get_name();
    const double get_rad_len_aluminum();
    const double get_rad_len_carbon();
    const double get_zSouth();
    const double get_zNorth();
    const double get_rSouth();
    const double get_rNorth();
  
  private:
    const string m_name = "service";
    const double m_rad_len_aluminum = 0.0;
    const double m_rad_len_carbon = 0.0;
    const double m_zSouth = 0.0;
    const double m_zNorth = 0.0;
    const double m_rSouth = 0.0;
    const double m_rNorth = 0.0;
};

ServiceStructure::ServiceStructure(const string &name,
                                   const double &rad_len_aluminum,
                                   const double &rad_len_carbon,
                                   const double &zSouth,
                                   const double &zNorth,
                                   const double &rSouth,
                                   const double &rNorth)
  : m_name(name)
  , m_rad_len_aluminum(rad_len_aluminum)
  , m_rad_len_carbon(rad_len_carbon)
  , m_zSouth(zSouth)
  , m_zNorth(zNorth)
  , m_rSouth(rSouth)
  , m_rNorth(rNorth)
{}

const string ServiceStructure::get_name() { return m_name; }
const double ServiceStructure::get_rad_len_aluminum() { return m_rad_len_aluminum; }
const double ServiceStructure::get_rad_len_carbon() { return m_rad_len_carbon; }
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
{}

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
{ //List materials and radiation length in cm
  const int nMaterials = 2;
  pair<string, double> materials[nMaterials] = { make_pair("G4_Al",  8.897)
                                               , make_pair("PEEK", 30.00) };
  
  double GlobalOffset = -15.0;
  double ShellThickness = 0.436; //Thickness in cm
  double LayerThickness = 0.1; //
  double ShellLength = 121.24; //Length of cylinder in cm
  int subsysID = 0;
}  // namespace G4TrackingService

vector<double> get_thickness(ServiceStructure *object)
{
  vector<double> thickness = {(object->get_rad_len_aluminum()/100)*G4TrackingService::materials[0].second
                             ,(object->get_rad_len_carbon()/100)*G4TrackingService::materials[1].second};
  return thickness;
}

void TrackingServiceInit()
{
}

double TrackingServiceCone(ServiceStructure *object, PHG4Reco* g4Reco, double radius)
{
  bool AbsorberActive = Enable::ABSORBER || Enable::TrackingService_ABSORBER;
  bool OverlapCheck = Enable::OVERLAPCHECK || Enable::TrackingService_OVERLAPCHECK;
  int verbosity = max(Enable::VERBOSITY, Enable::TrackingService_VERBOSITY);

  PHG4ConeSubsystem* cone;

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
    cone->SetPlaceZ(object->get_zSouth() + length/2 + G4TrackingService::GlobalOffset);
    cone->SetZlength(length/2);
    cone->SetMaterial(G4TrackingService::materials[i].first);
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

double TrackingServiceCylinder(ServiceStructure *object, PHG4Reco* g4Reco, double radius)
{
  bool AbsorberActive = Enable::ABSORBER || Enable::TrackingService_ABSORBER;
  bool OverlapCheck = Enable::OVERLAPCHECK || Enable::TrackingService_OVERLAPCHECK;
  int verbosity = max(Enable::VERBOSITY, Enable::TrackingService_VERBOSITY);

  PHG4CylinderSubsystem* cyl;

  double innerRadius = object->get_rSouth();
  double length = abs(object->get_zNorth() - object->get_zSouth());
  vector<double> thickness = get_thickness(object);

  for (int i = 0; i < G4TrackingService::nMaterials; ++i)
  {
    if (thickness[i] == 0) continue;
    cyl = new PHG4CylinderSubsystem(object->get_name(), G4TrackingService::subsysID);
    cyl->Verbosity(verbosity);
    cyl->set_double_param("place_z", object->get_zSouth() + length/2 + G4TrackingService::GlobalOffset);
    cyl->set_double_param("radius", innerRadius);
    cyl->set_double_param("length", length);
    cyl->set_string_param("material", G4TrackingService::materials[i].first);
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

double CreateCable(Cable *object, PHG4Reco* g4Reco, double radius)
{
  bool AbsorberActive = Enable::ABSORBER || Enable::TrackingService_ABSORBER;
  bool OverlapCheck = Enable::OVERLAPCHECK || Enable::TrackingService_OVERLAPCHECK;
  int verbosity = max(Enable::VERBOSITY, Enable::TrackingService_VERBOSITY);

  string cableMaterials[2] = { object->get_coreMaterial(), "G4_POLYETHYLENE" };
  double IR[2] = { 0, object->get_coreRadius() };
  double OR[2] = { object->get_coreRadius(), object->get_sheathRadius() };

  double length = abs(object->get_zNorth() - object->get_zSouth());
  double setX = (object->get_xSouth() + object->get_xNorth())/2;
  double setY = (object->get_ySouth() + object->get_yNorth())/2;
  double setZ = (object->get_zSouth() + object->get_zNorth())/2;
  
  double radToDeg = 180.0/M_PI;
  double rotX = tan((object->get_yNorth() - object->get_ySouth())/(object->get_zNorth() - object->get_zSouth()))*radToDeg;
  double rotY = tan((object->get_xNorth() - object->get_xSouth())/(object->get_zNorth() - object->get_zSouth()))*radToDeg;
  double rotZ = tan((object->get_xNorth() - object->get_xSouth())/(object->get_yNorth() - object->get_ySouth()))*radToDeg;
  
  PHG4CylinderSubsystem* cyl;
  for (int i = 0; i < 2; ++i)
  {
    cyl = new PHG4CylinderSubsystem(object->get_name(), i);
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

double CreateCableBundle(string superName, PHG4Reco* g4Reco, double radius, 
                         double x1, double x2, double y1, double y2, double z1, double z2)
{
  //Set up basic MVTX cable bundle (24 Samtex cables, 1 power cable, 2 cooling cables)
  double samtecCoreRadius = 0.1275;
  double samtecSheathRadius = 0.05;
  double coolingCoreRadius = 0.056;
  double coolingSheathRadius = 0.08; //?


  //Samtec cables (we use 24 as there are 12 twinax)
  for (unsigned int row = 0; row < 2; ++row)
  {
    for (unsigned int col = 0; col < 12; ++ col)
    {
      Cable *cable = new Cable(superName + "_samtec", "G4_Cu", samtecCoreRadius, samtecSheathRadius, 
                               x1 + -1*(col + 1)*samtecSheathRadius, x2 + -1*(col + 1)*samtecSheathRadius,
                               y1 + -1*(row + 1)*samtecSheathRadius, y2 + -1*(row + 1)*samtecSheathRadius,
                               z1, z2);
      radius += CreateCable(cable, g4Reco, radius);
    }
  }

  //Cooling Cables
  for (unsigned int nCool = 0; nCool < 2; ++nCool)
  {
    Cable *cable = new Cable(superName + "_cooling", "G4_WATER", coolingCoreRadius, coolingSheathRadius, 
                             x1 + (nCool + 1)*coolingSheathRadius, x2 + (nCool + 1)*coolingSheathRadius,
                             y1 + (nCool + 1)*coolingSheathRadius, y2 + (nCool + 1)*coolingSheathRadius,
                             z1, z2);
    radius += CreateCable(cable, g4Reco, radius);
  }

  //Power Cables
  return radius;
}

double TrackingService(PHG4Reco* g4Reco, double radius)
{
  vector<ServiceStructure*> cylinders, cones;

  double shellX0 = 100*G4TrackingService::ShellThickness/G4TrackingService::materials[4].second;
  double layerX0 = 100*G4TrackingService::LayerThickness/G4TrackingService::materials[4].second;
  double shellOffset = 18.679;

  cylinders.push_back(new ServiceStructure("MVTXServiceBarrel", 0, shellX0, -1.*(G4TrackingService::ShellLength + shellOffset), -1.*shellOffset , 10.33, 0));

  cylinders.push_back(new ServiceStructure("L0_1", 0, layerX0, -18.680, -16.579, 5.050, 0));
  cones.push_back(new ServiceStructure("L0_2", 0, layerX0, -16.579, -9.186, 5.050, 2.997));
  cylinders.push_back(new ServiceStructure("L0_3", 0, layerX0, -9.186, 0, 2.997, 0));

  cylinders.push_back(new ServiceStructure("L1_1", 0, layerX0, -17.970, -15.851, 7.338, 0));
  cones.push_back(new ServiceStructure("L1_2", 0, layerX0, -15.851, -8.938, 7.338, 3.799));
  cylinders.push_back(new ServiceStructure("L1_3", 0, layerX0, -8.938, 0, 3.799, 0));

  cylinders.push_back(new ServiceStructure("L2_1", 0, layerX0, -22.300, -15.206, 9.650, 0));
  cones.push_back(new ServiceStructure("L2_2", 0, layerX0, -15.206, -8.538, 9.650, 4.574));
  cylinders.push_back(new ServiceStructure("L2_3", 0, layerX0, -8.538, 0, 4.574, 0));
  

  //for (ServiceStructure *cylinder : cylinders) radius += TrackingServiceCylinder(cylinder, g4Reco, radius); 
  //for (ServiceStructure *cone : cones) radius += TrackingServiceCone(cone, g4Reco, radius);

  radius += CreateCableBundle("Test", g4Reco, radius, 0, 0, 0, 0, 0, 10);

  return radius;
}

#endif

