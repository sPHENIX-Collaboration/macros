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

class ServiceProperties
{
  public:
   ServiceProperties();

    explicit ServiceProperties(const string &name,
                               const double &rad_len_aluminum,
                               const double &rad_len_carbon,
                               const double &z_south,
                               const double &z_north,
                               const double &r_south,
                               const double &r_north);

    virtual ~ServiceProperties(){};

    const string get_name();
    const double get_rad_len_aluminum();
    const double get_rad_len_carbon();
    const double get_z_south();
    const double get_z_north();
    const double get_r_south();
    const double get_r_north();
  
  private:
    const string m_name = "service";
    const double m_rad_len_aluminum = 0.0;
    const double m_rad_len_carbon = 0.0;
    const double m_z_south = 0.0;
    const double m_z_north = 0.0;
    const double m_r_south = 0.0;
    const double m_r_north = 0.0;
};

ServiceProperties::ServiceProperties(const string &name,
                                     const double &rad_len_aluminum,
                                     const double &rad_len_carbon,
                                     const double &z_south,
                                     const double &z_north,
                                     const double &r_south,
                                     const double &r_north)
  : m_name(name)
  , m_rad_len_aluminum(rad_len_aluminum)
  , m_rad_len_carbon(rad_len_carbon)
  , m_z_south(z_south)
  , m_z_north(z_north)
  , m_r_south(r_south)
  , m_r_north(r_north)
{}

const string ServiceProperties::get_name() { return m_name; }
const double ServiceProperties::get_rad_len_aluminum() { return m_rad_len_aluminum; }
const double ServiceProperties::get_rad_len_carbon() { return m_rad_len_carbon; }
const double ServiceProperties::get_z_south() { return m_z_south; }
const double ServiceProperties::get_z_north() { return m_z_north; }
const double ServiceProperties::get_r_south() { return m_r_south; }
const double ServiceProperties::get_r_north() { return m_r_north; }

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

vector<double> get_thickness(ServiceProperties *object)
{
  vector<double> thickness = {(object->get_rad_len_aluminum()/100)*G4TrackingService::materials[0].second
                             ,(object->get_rad_len_carbon()/100)*G4TrackingService::materials[1].second};
  return thickness;
}

void TrackingServiceInit()
{
}

double TrackingServiceCone(ServiceProperties *object, PHG4Reco* g4Reco, double radius)
{
  bool AbsorberActive = Enable::ABSORBER || Enable::TrackingService_ABSORBER;
  bool OverlapCheck = Enable::OVERLAPCHECK || Enable::TrackingService_OVERLAPCHECK;
  int verbosity = max(Enable::VERBOSITY, Enable::TrackingService_VERBOSITY);

  PHG4ConeSubsystem* cone;

  double innerRadiusSouth = object->get_r_south();
  double innerRadiusNorth = object->get_r_north();
  double length = abs(object->get_z_north() - object->get_z_south());
  vector<double> thickness = get_thickness(object);

  for (int i = 0; i < G4TrackingService::nMaterials; ++i)
  {
    if (thickness[i] == 0) continue;
    cone = new PHG4ConeSubsystem(object->get_name(), G4TrackingService::subsysID);
    cone->Verbosity(verbosity);
    cone->SetR1(innerRadiusSouth, innerRadiusSouth + thickness[i]);
    cone->SetR2(innerRadiusNorth, innerRadiusNorth + thickness[i]);
    cone->SetPlaceZ(object->get_z_south() + length/2 + G4TrackingService::GlobalOffset);
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

double TrackingServiceCylinder(ServiceProperties *object, PHG4Reco* g4Reco, double radius)
{
  bool AbsorberActive = Enable::ABSORBER || Enable::TrackingService_ABSORBER;
  bool OverlapCheck = Enable::OVERLAPCHECK || Enable::TrackingService_OVERLAPCHECK;
  int verbosity = max(Enable::VERBOSITY, Enable::TrackingService_VERBOSITY);

  PHG4CylinderSubsystem* cyl;

  double innerRadius = object->get_r_south();
  double length = abs(object->get_z_north() - object->get_z_south());
  vector<double> thickness = get_thickness(object);

  for (int i = 0; i < G4TrackingService::nMaterials; ++i)
  {
    if (thickness[i] == 0) continue;
    cyl = new PHG4CylinderSubsystem(object->get_name(), G4TrackingService::subsysID);
    cyl->Verbosity(verbosity);
    cyl->set_double_param("place_z", object->get_z_south() + length/2 + G4TrackingService::GlobalOffset);
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

double TrackingService(PHG4Reco* g4Reco, double radius)
{
  vector<ServiceProperties*> cylinders, cones;

  double shellX0 = 100*G4TrackingService::ShellThickness/G4TrackingService::materials[4].second;
  double layerX0 = 100*G4TrackingService::LayerThickness/G4TrackingService::materials[4].second;
  double shellOffset = 18.679;

  cylinders.push_back(new ServiceProperties("MVTXServiceBarrel", 0, shellX0, -1.*(G4TrackingService::ShellLength + shellOffset), -1.*shellOffset , 10.33, 0));

  cylinders.push_back(new ServiceProperties("L0_1", 0, layerX0, -18.680, -16.579, 5.050, 0));
  cones.push_back(new ServiceProperties("L0_2", 0, layerX0, -16.579, -9.186, 5.050, 2.997));
  cylinders.push_back(new ServiceProperties("L0_3", 0, layerX0, -9.186, 0, 2.997, 0));

  cylinders.push_back(new ServiceProperties("L1_1", 0, layerX0, -17.970, -15.851, 7.338, 0));
  cones.push_back(new ServiceProperties("L1_2", 9/16, 0, 0.42/16, 0.32/16, layerX0, -15.851, -8.938, 7.338, 3.799));
  cylinders.push_back(new ServiceProperties("L1_3", 9/16, 0, 0.42/16, 0.32/16, layerX0, -8.938, 0, 3.799, 0));

  cylinders.push_back(new ServiceProperties("L2_1", 0, layerX0, -22.300, -15.206, 9.650, 0));
  cones.push_back(new ServiceProperties("L2_2", 0, layerX0, -15.206, -8.538, 9.650, 4.574));
  cylinders.push_back(new ServiceProperties("L2_3", 0, layerX0, -8.538, 0, 4.574, 0));
  

  for (ServiceProperties *cylinder : cylinders) radius += TrackingServiceCylinder(cylinder, g4Reco, radius); 
  for (ServiceProperties *cone : cones) radius += TrackingServiceCone(cone, g4Reco, radius);

  return radius;
}

#endif
