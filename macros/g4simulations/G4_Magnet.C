
void MagnetInit() {}

double Magnet(PHG4Reco* g4Reco,
	      double radius,
	      const int crossings = 0,
	      const int absorberactive = 0,
	      int verbosity = 0) {

  double magnet_inner_cryostat_wall_radius = 142;
  double magnet_inner_cryostat_wall_thickness = 1;
  double magnet_outer_cryostat_wall_radius = 174.5;
  double magnet_outer_cryostat_wall_thickness = 2.5;
  double magnet_coil_radius = 150.8;
  double magnet_coil_thickness = 9.38;
  double magnet_length = 379.;
  double coil_length = 361.5;
  if (radius > magnet_inner_cryostat_wall_radius) {
    cout << "inconsistency: radius: " << radius 
	 << " larger than Magnet inner radius: " << magnet_inner_cryostat_wall_radius << endl;
    gSystem->Exit(-1);
  }
  
  gSystem->Load("libg4detectors.so");
  gSystem->Load("libg4testbench.so");

  radius = magnet_inner_cryostat_wall_radius;
  PHG4CylinderSubsystem *cyl = new PHG4CylinderSubsystem("MAGNET", 0);
  cyl->SetRadius(magnet_inner_cryostat_wall_radius);
  cyl->SetLengthViaRapidityCoverage(false);
  cyl->SetLength(magnet_length);
  cyl->SetMaterial("Al5083"); // use 1 radiation length Al for magnet thickness
  cyl->SetThickness(magnet_inner_cryostat_wall_thickness);
  cyl->SuperDetector("MAGNET");
  if (absorberactive)  cyl->SetActive();
  g4Reco->registerSubsystem( cyl );

  cyl = new PHG4CylinderSubsystem("MAGNET", 1);
  cyl->SetRadius(magnet_coil_radius);
  cyl->SetLengthViaRapidityCoverage(false);
  cyl->SetLength(coil_length);
  cyl->SetMaterial("Al5083"); // use 1 radiation length Al for magnet thickness
  cyl->SetThickness(magnet_coil_thickness);
  cyl->SuperDetector("MAGNET");
  if (absorberactive)  cyl->SetActive();
  g4Reco->registerSubsystem( cyl );

  cyl = new PHG4CylinderSubsystem("MAGNET", 2);
  cyl->SetRadius(magnet_outer_cryostat_wall_radius);
  cyl->SetLengthViaRapidityCoverage(false);
  cyl->SetLength(magnet_length);
  cyl->SetMaterial("Al5083"); // use 1 radiation length Al for magnet thickness
  cyl->SetThickness(magnet_outer_cryostat_wall_thickness);
  cyl->SuperDetector("MAGNET");
  if (absorberactive)  cyl->SetActive();
  g4Reco->registerSubsystem( cyl );

  radius = magnet_outer_cryostat_wall_radius + magnet_outer_cryostat_wall_thickness; // outside of magnet
  
  if (verbosity > 0) {
    cout << "========================= G4_Magnet.C::Magnet() ===========================" << endl;
    cout << " MAGNET Material Description:" << endl;
    cout << "  inner radius = " << magnet_inner_cryostat_wall_radius << " cm" << endl;
    cout << "  outer radius = " << magnet_outer_cryostat_wall_radius + magnet_outer_cryostat_wall_thickness << " cm" << endl;
    cout << "  length = " << magnet_length << " cm" << endl;
    cout << "===========================================================================" << endl;
  }

  radius += no_overlapp;
  
  return radius; 
}
