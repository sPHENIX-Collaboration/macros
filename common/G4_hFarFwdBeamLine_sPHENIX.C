#ifndef MACRO_G4HFARFWDBEAMLINE_SPHENIX_C
#define MACRO_G4HFARFWDBEAMLINE_SPHENIX_C

#include <GlobalVariables.C>

#include <g4detectors/BeamLineMagnetSubsystem.h>
#include <g4detectors/PHG4BlockSubsystem.h>
#include <g4detectors/PHG4ConeSubsystem.h>
#include <g4detectors/PHG4CylinderSubsystem.h>

#include <g4main/PHG4Reco.h>

#include <TSystem.h>

R__LOAD_LIBRARY(libg4detectors.so)

float PosFlip(float pos);
float AngleFlip(float angle);
float MagFieldFlip(float Bfield);

// This creates the Enable Flag to be used in the main steering macro
namespace Enable
{
  bool HFARFWD_MAGNETS = false;
 
  bool HFARFWD_OVERLAPCHECK = false;
  int HFARFWD_VERBOSITY = 0;
 
}  // namespace Enable

namespace hFarFwdBeamLine
{
  double starting_z = -3000.;  //cm as center-forward interface
  double enclosure_z_max = NAN;
  double enclosure_r_max = NAN;
  double enclosure_center = NAN;

  PHG4CylinderSubsystem *hFarFwdBeamLineEnclosure(nullptr);

  BeamLineMagnetSubsystem *B0Magnet = (nullptr);
}  // namespace hFarFwdBeamLine

void hFarFwdBeamLineInit()
{

  hFarFwdBeamLine::enclosure_z_max = 3000.;
  BlackHoleGeometry::min_z = std::min(BlackHoleGeometry::min_z, hFarFwdBeamLine::starting_z);
  hFarFwdBeamLine::enclosure_r_max = 200.;
  
  hFarFwdBeamLine::enclosure_center = 0.5 * (hFarFwdBeamLine::starting_z + hFarFwdBeamLine::enclosure_z_max);
  
  BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z, hFarFwdBeamLine::enclosure_z_max);
  BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, hFarFwdBeamLine::enclosure_r_max);
}

void hFarFwdDefineMagnets(PHG4Reco *g4Reco)
{
  bool overlapCheck = Enable::OVERLAPCHECK || Enable::HFARFWD_OVERLAPCHECK;
  int verbosity = std::max(Enable::VERBOSITY, Enable::HFARFWD_VERBOSITY);

  hFarFwdBeamLine::hFarFwdBeamLineEnclosure = new PHG4CylinderSubsystem("hFarFwdBeamLineEnclosure");
  hFarFwdBeamLine::hFarFwdBeamLineEnclosure->set_double_param("place_z", hFarFwdBeamLine::enclosure_center);
  hFarFwdBeamLine::hFarFwdBeamLineEnclosure->set_double_param("radius", 0);
  hFarFwdBeamLine::hFarFwdBeamLineEnclosure->set_double_param("thickness", hFarFwdBeamLine::enclosure_r_max);  // This is intentionally made large 25cm radius
  hFarFwdBeamLine::hFarFwdBeamLineEnclosure->set_double_param("length", hFarFwdBeamLine::enclosure_z_max - hFarFwdBeamLine::starting_z);
  hFarFwdBeamLine::hFarFwdBeamLineEnclosure->set_string_param("material", "G4_Galactic");
  hFarFwdBeamLine::hFarFwdBeamLineEnclosure->set_color(.5, .5, .5, 0.2);
  hFarFwdBeamLine::hFarFwdBeamLineEnclosure->OverlapCheck(overlapCheck);
  if (verbosity)
    hFarFwdBeamLine::hFarFwdBeamLineEnclosure->Verbosity(verbosity);
  g4Reco->registerSubsystem(hFarFwdBeamLine::hFarFwdBeamLineEnclosure);

  string magFile;
  // magFile = string(getenv("CALIBRATIONROOT")) + "/Beam/D0DXMagnets.dat";
    magFile = "/sphenix/u/shuhang98/macros/detectors/sPHENIX/D0DXMagnets.dat";
  // make magnet active volume if you want to study the hits
  bool magnet_active = false;
  int absorberactive = 0;

  // if you insert numbers it only displays those magnets, do not comment out the set declaration
  set<int> magnetlist;
  //magnetlist.insert(7);

  BeamLineMagnetSubsystem *bl = nullptr;
  std::ifstream infile(magFile);
  if (infile.is_open())
  {
    double biggest_z = 0.;
    int imagnet = 0;
    std::string line;
    while (std::getline(infile, line))
    {
      if (!line.compare(0, 1, "B") ||
          !line.compare(0, 1, "Q") ||
          !line.compare(0, 1, "S"))
      {
        std::istringstream iss(line);
        string magname;
        double x;
        double y;
        double z;
        double inner_radius_zin;
        double inner_radius_zout;
        double outer_magnet_diameter;
        double length;
        double angle;
        double dipole_field_x;
        double fieldgradient;
        if (!(iss >> magname >> x >> y >> z >> inner_radius_zin >> inner_radius_zout >> outer_magnet_diameter >> length >> angle >> dipole_field_x >> fieldgradient))
        {
          cout << "coud not decode " << line << endl;
          gSystem->Exit(1);
        }
        else
        {
	  //------------------------

          string magtype;
          if (inner_radius_zin != inner_radius_zout)
          {
            cout << "inner radius at front of magnet " << inner_radius_zin
                 << " not equal radius at back of magnet " << inner_radius_zout
                 << " needs change in code (replace tube by cone for beamline)" << endl;
            gSystem->Exit(1);
          }
          if (verbosity > 0)
          {
            cout << endl
                 << endl
                 << "\tID number " << imagnet << endl;
            cout << "magname: " << magname << endl;
            cout << "x: " << x << endl;
            cout << "y: " << y << endl;
            cout << "z: " << z << endl;
            cout << "inner_radius_zin: " << inner_radius_zin << endl;
            cout << "inner_radius_zout: " << inner_radius_zout << endl;
            cout << "outer_magnet_diameter: " << outer_magnet_diameter << endl;
            cout << "length: " << length << endl;
            cout << "angle: " << angle << endl;
            cout << "dipole_field_x: " << dipole_field_x << endl;
            cout << "fieldgradient: " << fieldgradient << endl;
          }
          if (!magname.compare(0, 1, "B"))
          {
            magtype = "DIPOLE";
          }
          else if (!magname.compare(0, 1, "Q"))
          {
            magtype = "QUADRUPOLE";
          }
          else if (!magname.compare(0, 1, "S"))
          {
            magtype = "SEXTUPOLE";
          }
          else
          {
            cout << "cannot decode magnet name " << magname << endl;
            gSystem->Exit(1);
          }
          // convert to our units (cm, deg)
          x *= 100.;
          y *= 100.;
          z *= 100.;
          length *= 100.;
          inner_radius_zin *= 100.;
          outer_magnet_diameter *= 100.;
          angle = (angle / TMath::Pi() * 180.) / 1000.;  // given in mrad

	  //------------------------
	  
          if (magnetlist.empty() || magnetlist.find(imagnet) != magnetlist.end())
          {
            bl = new BeamLineMagnetSubsystem("BEAMLINEMAGNET", imagnet);
            bl->set_double_param("field_y", MagFieldFlip(dipole_field_x));
            bl->set_double_param("fieldgradient", MagFieldFlip(fieldgradient));
            bl->set_string_param("magtype", magtype);
            bl->set_double_param("length", length);
            bl->set_double_param("place_x", PosFlip(x));// relative position to mother vol.
            bl->set_double_param("place_y", y);// relative position to mother vol.
            bl->set_double_param("place_z", z - hFarFwdBeamLine::enclosure_center);// relative position to mother vol.
            bl->set_double_param("field_global_position_x", PosFlip(x));// abs. position to world for field manager
            bl->set_double_param("field_global_position_y", y);// abs. position to world for field manager
            bl->set_double_param("field_global_position_z", z);// abs. position to world for field manager
            bl->set_double_param("rot_y", AngleFlip(angle));
            bl->set_double_param("field_global_rot_y", AngleFlip(angle));// abs. rotation to world for field manager
            bl->set_double_param("inner_radius", inner_radius_zin);
            bl->set_double_param("outer_radius", outer_magnet_diameter / 2.);
            bl->SetActive(magnet_active);
            bl->BlackHole();
            bl->SetMotherSubsystem(hFarFwdBeamLine::hFarFwdBeamLineEnclosure);
            if (absorberactive)
            {
              bl->SetAbsorberActive();
            }
            bl->OverlapCheck(overlapCheck);
            bl->SuperDetector("BEAMLINEMAGNET");
            if (verbosity)
              bl->Verbosity(verbosity);
            g4Reco->registerSubsystem(bl);

            // rag the B0 magnet
            if (imagnet == 0)
              hFarFwdBeamLine::B0Magnet = bl;
          }
          imagnet++;
          if (fabs(z) + length > biggest_z)
          {
            biggest_z = fabs(z) + length;
          }
        }
      }
    }
    infile.close();
  }
}

void hFarFwdDefineBeamPipe(PHG4Reco *g4Reco)
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::HFARFWD_VERBOSITY);
 

  const int ntube = 10; 
  const string nm[ntube] = {"B00", "B01", "B10", "B11", "B20", "B21", "B30", "B31", "B32", "B33"};
  const double qlen[ntube] = {207.88, 207.88, 115.79, 115.79, 217.16,217.16, 183, 183, 183, 183};
  const double qir[ntube] = {6.08, 6.08, 14.60, 14.60, 20.0, 20.0, 6.07, 6.07, 6.07, 6.07};
  const double qor[ntube] = {6.35, 6.35, 15.24, 15.24, 20.96, 20.96, 6.35, 6.35, 6.35, 6.35};
  const double qrot[ntube] = {0, 0, 0, 0, 0, 0, 1.074, -1.074, -1.074, 1.074}; //degree 
  const double qxC[ntube] = {0, 0, 0, 0, 0, 0, 12.82, -12.82, 12.82, -12.82};
  const double qyC[ntube] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  const double qzC[ntube] = {850.14, -850.14, 1475.935, -1475.935, 1642.4, -1642.4, 1843.2, 1843.2, -1843.2, -1843.2};
  for (int i = 0; i < ntube; i++)
  {
    PHG4CylinderSubsystem *pipe = new PHG4CylinderSubsystem(Form("beamPipe%s", nm[i].c_str()), 0);
    pipe->set_double_param("radius", qir[i]);
    pipe->set_double_param("thickness", qor[i] - qir[i]);
    pipe->set_double_param("length", qlen[i]);
    pipe->set_double_param("rot_y", qrot[i]);
    pipe->set_string_param("material", "G4_STAINLESS-STEEL");
    pipe->set_double_param("place_x", PosFlip(qxC[i]));
    pipe->set_double_param("place_y", qyC[i]);
    pipe->set_double_param("place_z", qzC[i]);
    pipe->SetActive(false);
    //    pipe->SetActive(true);
    g4Reco->registerSubsystem(pipe);
  }

  //Roman Pot pipe
  const int nSec = 2;
  const double len[nSec] = {20.87, 20.87};
  const double ir1[nSec] = {7.14, 14.60};
  const double or1[nSec] = {7.77, 15.24};
  const double ir2[nSec] = {14.60, 7.14};
  const double or2[nSec] = {15.24, 7.77};
  const double xC[nSec] = {0, 0};
  const double yC[nSec] = {0, 0};
  const double zC[nSec] = {1384.25, -1384.25};
  for (int i = 0; i < nSec; i++)
  {
    PHG4ConeSubsystem *pipe = new PHG4ConeSubsystem(Form("beamPipeRP%d", i), 0);
    pipe->set_string_param("material", "G4_STAINLESS-STEEL");
    pipe->set_double_param("place_x", PosFlip(xC[i]));
    pipe->set_double_param("place_y", yC[i]);
    pipe->set_double_param("place_z", zC[i]);
    pipe->set_double_param("length", len[i]);
    pipe->set_double_param("rmin1", ir1[i]);
    pipe->set_double_param("rmin2", ir2[i]);
    pipe->set_double_param("rmax1", or1[i]);
    pipe->set_double_param("rmax2", or2[i]);
    pipe->set_double_param("rot_y", AngleFlip(-0.047 * TMath::RadToDeg()));
    g4Reco->registerSubsystem(pipe);
  }
}


float PosFlip(float pos){
  return pos;
};
float AngleFlip(float angle){
  return angle;
};
float MagFieldFlip(float Bfield){
  return Bfield;
};

#endif
