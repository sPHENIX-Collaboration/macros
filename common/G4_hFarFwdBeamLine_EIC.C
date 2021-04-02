#ifndef MACRO_G4HFARFWDBEAMLINE_EIC_C
#define MACRO_G4HFARFWDBEAMLINE_EIC_C

#include <GlobalVariables.C>

#include <g4detectors/PHG4CylinderSubsystem.h>
#include <g4detectors/PHG4ConeSubsystem.h>
#include <g4detectors/BeamLineMagnetSubsystem.h>
#include <g4detectors/PHG4BlockSubsystem.h>

#include <g4main/PHG4Reco.h>

#include <TSystem.h>

R__LOAD_LIBRARY(libg4detectors.so)

// This creates the Enable Flag to be used in the main steering macro
namespace Enable
{
  bool HFARFWD_MAGNETS_IP6 = false;
  bool HFARFWD_MAGNETS_IP8 = false;
  bool HFARFWD_VIRTUAL_DETECTORS_IP8 = false;
  bool HFARFWD_VIRTUAL_DETECTORS_IP6 = false;
  bool HFARFWD_PIPE = false;
  bool HFARFWD_OVERLAPCHECK = false;
  int HFARFWD_VERBOSITY = 0;
}  // namespace Enable


void hFarFwdBeamLineInit()
{

  if (Enable::HFARFWD_MAGNETS_IP6 && Enable::HFARFWD_MAGNETS_IP8)
  {
    cout << "You cannot have magnets for both IP6 and IP8 ON at the same time" << endl;
    gSystem->Exit(1);
  }

  if (Enable::HFARFWD_MAGNETS_IP6)
  {
    BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z, 4500.);
    BlackHoleGeometry::min_z = std::min(BlackHoleGeometry::min_z, 0.);
    BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, 180.0);
  }

  if (Enable::HFARFWD_MAGNETS_IP8)
  {
    BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z,  4800.);
    BlackHoleGeometry::min_z = std::min(BlackHoleGeometry::min_z, -2050.);
    BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, 150.0);
  }
}

void hFarFwdDefineMagnets(PHG4Reco* g4Reco){

  bool overlapCheck = Enable::OVERLAPCHECK || Enable::HFARFWD_OVERLAPCHECK;
  int verbosity = std::max(Enable::VERBOSITY, Enable::HFARFWD_VERBOSITY);

  string magFile;
  if(Enable::HFARFWD_MAGNETS_IP6)
    magFile = string(getenv("CALIBRATIONROOT")) + "/Beam/ip6_h_farFwdBeamLineMagnets.dat";
  else if(Enable::HFARFWD_MAGNETS_IP8)
    magFile = string(getenv("CALIBRATIONROOT")) + "/Beam/ip8_35mrad_h_farFwdBeamLineMagnets.dat";
  else{
    cout << " You have to enable either the IP6 or IP8 Magnet configuration to define magnets! "<<endl;
    gSystem->Exit(1);
  }
    
  // make magnet active volume if you want to study the hits
  bool magnet_active=false;
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
	  if (! line.compare(0,1,"B") || 
	      ! line.compare(0,1,"Q") ||
	      ! line.compare(0,1,"S"))
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
	      if (!(iss >> magname >> x >> y >> z 
		    >> inner_radius_zin >> inner_radius_zout
		    >> outer_magnet_diameter >> length
		    >> angle >> dipole_field_x >> fieldgradient))
		{
		  cout << "coud not decode " << line << endl;
		  gSystem->Exit(1);
		}
	      else
		{
		  string magtype;
		  if (inner_radius_zin != inner_radius_zout)
		    {
		      cout << "inner radius at front of magnet " << inner_radius_zin
			   << " not equal radius at back of magnet " << inner_radius_zout
			   << " needs change in code (replace tube by cone for beamline)" << endl;
		      gSystem->Exit(1);
		    }
		  if(verbosity>0){
		    cout << endl << endl << "\tID number "<<imagnet<<endl;
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
		  if (! magname.compare(0,1,"B")){
		    magtype = "DIPOLE";
		  }else if (! magname.compare(0,1,"Q")){
		    magtype = "QUADRUPOLE";
		  }else if (! magname.compare(0,1,"S")){
		    magtype = "SEXTUPOLE";
		  }else{
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
		  angle = (angle/TMath::Pi()*180.)/1000.; // given in mrad

		  if (magnetlist.empty() || magnetlist.find(imagnet) != magnetlist.end())
		    {
		      bl = new BeamLineMagnetSubsystem("BEAMLINEMAGNET",imagnet);
		      bl->set_double_param("field_y",dipole_field_x);
		      bl->set_double_param("field_x",0.);
		      bl->set_double_param("fieldgradient",fieldgradient);
		      bl->set_string_param("magtype",magtype);
		      bl->set_double_param("length",length);
		      bl->set_double_param("place_x",x);
		      bl->set_double_param("place_y",y);
		      bl->set_double_param("place_z",z);
		      bl->set_double_param("rot_y",angle);
		      bl->set_double_param("inner_radius",inner_radius_zin);
		      bl->set_double_param("outer_radius", outer_magnet_diameter/2.);
		      bl->SetActive(magnet_active);
		      bl->BlackHole();
		      if (absorberactive)  
			{
			  bl->SetAbsorberActive();
			}
		      bl->OverlapCheck(overlapCheck);
		      bl->SuperDetector("BEAMLINEMAGNET");
		      if(verbosity)
			bl->Verbosity(verbosity);
		      g4Reco->registerSubsystem(bl);
		    }
		  imagnet++;
		  if (fabs(z)+length > biggest_z)
		    {
		      biggest_z = fabs(z)+length;
		    }
		}
	    }
	}
      infile.close();
    }
}

void hFarFwdDefineDetectors(PHG4Reco* g4Reco){

  if (Enable::HFARFWD_VIRTUAL_DETECTORS_IP6 && Enable::HFARFWD_VIRTUAL_DETECTORS_IP8)
  {
    cout << "You cannot have detectors enabled for both IP6 and IP8 ON at the same time" << endl;
    gSystem->Exit(1);
  }

  if (Enable::HFARFWD_VIRTUAL_DETECTORS_IP8) {
    cout << " IP8 h far forward detectors not defined right now. " <<endl;
    return;
  }

  int verbosity = std::max(Enable::VERBOSITY, Enable::HFARFWD_VERBOSITY);

  auto *detZDC = new PHG4BlockSubsystem("zdcTruth");
  detZDC->SuperDetector("ZDC");
  detZDC->set_double_param("place_x",96.24);
  detZDC->set_double_param("place_y",0);
  detZDC->set_double_param("place_z",3750);
  detZDC->set_double_param("rot_y",-0.025*TMath::RadToDeg());
  detZDC->set_double_param("size_x",60);
  detZDC->set_double_param("size_y",60);
  detZDC->set_double_param("size_z",0.1);
  detZDC->set_string_param("material","G4_Si");
  detZDC->SetActive();
  if(verbosity)
    detZDC->Verbosity(verbosity);
  g4Reco->registerSubsystem(detZDC);
 
  const int rpDetNr = 2;
  const double rp_zCent[rpDetNr]={2600  ,2800};
  const double rp_xCent[rpDetNr]={84.49 ,93.59};
  for(int i=0;i<rpDetNr;i++){
    auto *detRP = new PHG4BlockSubsystem(Form("rpTruth_%d",i));
    //detRP->SuperDetector("RomanPots");
    detRP->set_double_param("place_x",rp_xCent[i]);
    detRP->set_double_param("place_y",0);
    detRP->set_double_param("place_z",rp_zCent[i]);
    detRP->set_double_param("rot_y",-0.025*TMath::RadToDeg());
    detRP->set_double_param("size_x",25);
    detRP->set_double_param("size_y",10);
    detRP->set_double_param("size_z",0.03);
    detRP->set_string_param("material","G4_Si");
    detRP->SetActive();
    if(verbosity)
      detRP->Verbosity(verbosity);
    g4Reco->registerSubsystem(detRP);
  }

  const int b0DetNr = 4;
  const double b0Mag_zCent = 590;
  const double b0Mag_zLen = 120;
  for(int i=0;i<b0DetNr;i++){
    auto *detB0 = new PHG4CylinderSubsystem(Form("b0Truth_%d",i),0);
    //detB0->SuperDetector("B0detectors");
    detB0->set_double_param("radius",0);
    detB0->set_double_param("thickness",20);
    detB0->set_double_param("length",0.1);
    detB0->set_string_param("material","G4_Si");
    detB0->set_double_param("place_x",13.2);
    detB0->set_double_param("place_y",0);
    detB0->set_double_param("place_z", (b0Mag_zCent - b0Mag_zLen/2) + b0Mag_zLen/(b0DetNr-1) * i);
    detB0->SetActive(true);
    if(verbosity)
      detB0->Verbosity(verbosity);
    g4Reco->registerSubsystem(detB0);
  }
 
  
}


void hFarFwdDefineBeamPipe(PHG4Reco* g4Reco){
  int verbosity = std::max(Enable::VERBOSITY, Enable::HFARFWD_VERBOSITY);
  //exit window
  PHG4CylinderSubsystem *exitWin = new PHG4CylinderSubsystem("exitWin",0);
  exitWin->set_double_param("radius"   ,3.2);
  exitWin->set_double_param("thickness",11.8);
  exitWin->set_double_param("length"   ,0.15);
  exitWin->set_double_param("rot_y"    ,-0.025*TMath::RadToDeg());
  exitWin->set_string_param("material" ,"G4_STAINLESS-STEEL");
  exitWin->set_double_param("place_x"  ,12.5);
  exitWin->set_double_param("place_y"  ,0);
  exitWin->set_double_param("place_z"  ,500);
  exitWin->SetActive(false);
  g4Reco->registerSubsystem(exitWin);

  //B0 magnet pipe
  PHG4CylinderSubsystem *pipeB0 = new PHG4CylinderSubsystem("beamPipeB0",0);
  pipeB0->set_double_param("radius",2.8);
  pipeB0->set_double_param("thickness",0.25);
  pipeB0->set_double_param("length",195);
  pipeB0->set_double_param("rot_y",-0.025*TMath::RadToDeg());
  pipeB0->set_string_param("material","G4_Al");
  pipeB0->set_double_param("place_x",14.748);
  pipeB0->set_double_param("place_y",0);
  pipeB0->set_double_param("place_z",590);
  pipeB0->SetActive(false);
  g4Reco->registerSubsystem(pipeB0);

  //Quad pipes
  const int nSecQ = 5; //B0apf, Q1apf, Q1bpf, Q2pf, B1pf
  const string nm  [nSecQ]={"B0apf", "Q1apf", "Q1bpf", "Q2pf", "B1pf"};
  const double qlen[nSecQ]={160    , 150    , 220    , 440   , 330   };
  const double qir [nSecQ]={4      , 5.1    , 7      , 12    , 12.2  };
  const double qor [nSecQ]={4.1    , 5.2    , 7.2    , 12.2  , 12.4  };
  const double qrot[nSecQ]={25     , 19.5   , 15     , 15    , 34    };//mrad
  const double qxC [nSecQ]={19.8   , 24.47  , 30.05  , 39.5  , 48    };
  const double qyC [nSecQ]={0      , 0      , 0      , 0     , 0     };
  const double qzC [nSecQ]={770    , 922.8  , 1106.3 , 1416.7, 1806.7};
  for(int i=0;i<nSecQ;i++){
    PHG4CylinderSubsystem *pipe = new PHG4CylinderSubsystem(Form("beamPipe%s",nm[i].c_str()),0);
    pipe->set_double_param("radius",qir[i]);
    pipe->set_double_param("thickness",qor[i]-qir[i]);
    pipe->set_double_param("length",qlen[i]);
    pipe->set_double_param("rot_y",-qrot[i]/1000*TMath::RadToDeg());
    pipe->set_string_param("material","G4_Al");
    pipe->set_double_param("place_x",qxC[i]);
    pipe->set_double_param("place_y",qyC[i]);
    pipe->set_double_param("place_z",qzC[i]);
    pipe->SetActive(false);
    g4Reco->registerSubsystem(pipe);
  }

  //Electron pipe
  PHG4CylinderSubsystem *pipeElectron = new PHG4CylinderSubsystem("beamPipeElectron",0);
  pipeElectron->set_double_param("radius",1);
  pipeElectron->set_double_param("thickness",1);
  pipeElectron->set_double_param("length",3000);
  pipeElectron->set_double_param("rot_y",-0.025*TMath::RadToDeg());
  pipeElectron->set_string_param("material","G4_Al");
  pipeElectron->set_double_param("place_x",0);
  pipeElectron->set_double_param("place_y",0);
  pipeElectron->set_double_param("place_z",2000);
  pipeElectron->SetActive(false);
  //g4Reco->registerSubsystem(pipeElectron);

  //ZDC pipe
  PHG4CylinderSubsystem *pipeZDC = new PHG4CylinderSubsystem("beamPipeZDC",0);
  pipeZDC->set_double_param("radius",16.5);
  pipeZDC->set_double_param("thickness",0.1);
  pipeZDC->set_double_param("length",170);
  pipeZDC->set_double_param("rot_y",-0.025*TMath::RadToDeg());
  pipeZDC->set_string_param("material","G4_Al");
  pipeZDC->set_double_param("place_x",59);
  pipeZDC->set_double_param("place_y",0);
  pipeZDC->set_double_param("place_z",2041.59);
  pipeZDC->SetActive(false);
  g4Reco->registerSubsystem(pipeZDC);

  //Roman Pot pipe
  const int nSec = 2;
  const double len[nSec]={850,1150 };
  const double ir1[nSec]={17  , 17 };
  const double or1[nSec]={17.1,17.1};
  const double ir2[nSec]={17  ,  7 };
  const double or2[nSec]={17.1, 7.1};
  const double xC[nSec] ={83  , 130};
  const double yC[nSec] ={0   ,   0};
  const double zC[nSec] ={2550,3550};
  for(int i=0;i<nSec;i++){
    PHG4ConeSubsystem *pipe = new PHG4ConeSubsystem(Form("beamPipeRP%d",i),0);
    pipe->set_string_param("material","G4_STAINLESS-STEEL");
    pipe->set_double_param("place_x",xC[i]);
    pipe->set_double_param("place_y",yC[i]);
    pipe->set_double_param("place_z",zC[i]);
    pipe->set_double_param("length",len[i]/2);
    pipe->set_double_param("rmin1",ir1[i]);
    pipe->set_double_param("rmin2",ir2[i]);
    pipe->set_double_param("rmax1",or1[i]);
    pipe->set_double_param("rmax2",or2[i]);
    pipe->set_double_param("rot_y",-0.027*TMath::RadToDeg());
    g4Reco->registerSubsystem(pipe);
  }
}


#endif
