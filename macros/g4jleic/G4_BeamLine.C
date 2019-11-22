#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include "GlobalVariables.C"
#include <g4jleic/G4JLeicBeamLineMagnetSubsystem.h>
#include <g4main/PHG4Reco.h>
R__LOAD_LIBRARY(libg4jleic.so)
#endif
void BeamLineInit() {}

double BeamLine(PHG4Reco* g4Reco,
		double radius,
		const int crossings = 0,
		const int absorberactive = 0,
		int verbosity = 0)
{
  
  const char *fname[2]= {"ion_ir_200.txt","e_ir_10.txt"} ;
  gSystem->Load("libg4jleic.so");
  gSystem->Load("libg4testbench.so");
  G4JLeicBeamLineMagnetSubsystem *bl = nullptr;
  int imagnet=0;
  for (int fi=0; fi<2; fi++)
  {
    std::ifstream infile(fname[fi]);
    if (infile.is_open())
    {
      double biggest_z = 0.;
      std::string line;
      while (std::getline(infile, line))
      {
	if (line.find("QUADRUPOLE") != string::npos ||
	    line.find("KICKER") != string::npos ||
	    (line.find("RBEND") != string::npos && fi == 0) ||
	    (line.find("SOLENOID") != string::npos && fi == 0))
	{
	  std::istringstream iss(line);
	  string magname;
	  string magtype;
	  double length;
	  double half_aper;
	  double inner_radius;
	  double outer_radius;
	  double dipole_field_x;
	  double dipole_field_y;
	  double quad_field_n;
	  double quad_field_s;
	  double sext_field;
	  double solenoid_field;
	  double x;
	  double y;
	  double z;
	  double theta;
	  double phi;

	  if (!(iss >> magname >> magtype >> length >> half_aper
		>> inner_radius >> outer_radius >> dipole_field_x
		>> dipole_field_y 
		>> quad_field_n >> quad_field_s >> sext_field >> solenoid_field
		>> x >> y >> z >> theta >> phi)) { break; } // error
	  else
	  {
// convert to our units (cm, deg)
	    x *= 100.;
	    y *= 100.;
	    z *= 100.;
	    length *= 100.;
	    theta = theta/TMath::Pi()*360.;
	    bl = new G4JLeicBeamLineMagnetSubsystem("BEAMLINE",imagnet);
	    if (magtype == "RBEND" || magtype == "KICKER")
	    {
	      magtype = "DIPOLE";
	    }
	    bl->set_double_param("field_x",dipole_field_x);
	    bl->set_double_param("field_y",dipole_field_y);
	    bl->set_double_param("fieldgradient",quad_field_n);
	    bl->set_string_param("magtype",magtype);
	    bl->set_double_param("length",length);
	    bl->set_double_param("place_x",x);
	    bl->set_double_param("place_y",y);
	    bl->set_double_param("place_z",z);
	    bl->set_double_param("rot_y",theta);
	    bl->set_double_param("inner_radius",inner_radius);
	    bl->set_double_param("outer_radius", outer_radius);
	    bl->SetActive();
	    if (absorberactive)  
	    {
	      bl->SetAbsorberActive();
	    }
            bl->OverlapCheck(overlapcheck);
	    bl->SuperDetector("BEAMLINE");
	    g4Reco->registerSubsystem(bl);
	    imagnet++;
	    if (fabs(z)+length > biggest_z)
	    {
	      biggest_z = fabs(z)+length;
	    }
	  }
	}
      }
      if (biggest_z*2. > g4Reco->GetWorldSizeZ())
      {
	g4Reco->SetWorldSizeZ(biggest_z*2.+200.); // leave 1m on both sides
      }
      infile.close();
    }
  }
  radius = 85.;
  return radius;
}
