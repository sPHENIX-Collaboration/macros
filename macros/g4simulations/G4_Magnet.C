
void MagnetInit() {}

double Magnet(PHG4Reco* g4Reco,
	      double radius,
	      const int crossings = 0,
	      const int absorberactive = 0,
	      int verbosity = 0) {

  double magnet_inner_radius = 140;
  double magnet_outer_radius = 173;
  double magnet_thickness = magnet_outer_radius - magnet_inner_radius;
  double magnet_length = 373.98;

  if (radius > magnet_inner_radius) {
    cout << "inconsistency: radius: " << radius 
	 << " larger than Magnet inner radius: " << magnet_inner_radius << endl;
    gSystem->Exit(-1);
  }
  
  gSystem->Load("libg4detectors.so");
  gSystem->Load("libg4testbench.so");

  radius = magnet_inner_radius;
  PHG4CylinderSubsystem *cyl = new PHG4CylinderSubsystem("MAGNET", 0);
  cyl->SetRadius(radius);
  cyl->SetLengthViaRapidityCoverage(false);
  cyl->SetLength(magnet_length);
  cyl->SetMaterial("AL_BABAR_MAG"); // use 1 radiation length Al for magnet thickness
  cyl->SetThickness(magnet_thickness);
  if (absorberactive)  cyl->SetActive();
  g4Reco->registerSubsystem( cyl );

  radius = magnet_inner_radius + magnet_thickness; // outside of magnet
  
  if (verbosity >= 0) {
    cout << "========================= G4_Magnet.C::Magnet() ===========================" << endl;
    cout << " CVS Version: $Id: G4_Magnet.C,v 1.1 2015/04/14 23:19:09 mccumber Exp $" << endl;
    cout << " MAGNET Material Description:" << endl;
    cout << "  inner radius = " << magnet_inner_radius << " cm" << endl;
    cout << "  outer radius = " << magnet_outer_radius << " cm" << endl;
    cout << "  length = " << magnet_length << " cm" << endl;
    cout << "===========================================================================" << endl;
  }

  radius += no_overlapp;
  
  return radius; 
}
