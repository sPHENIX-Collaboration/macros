
int Min_preshower_layer = 1;
int Max_preshower_layer = 1;

void PreShowerInit()
{
  Min_preshower_layer = 1;
  Max_preshower_layer = 1;
}

double
PreShower(PHG4Reco* g4Reco, double radius, const int absorberactive = 0)
{
  double preshower_inner_radius = 90.;
  double radlen_w =  0.3504; // radiation length of w in cm
  double tungstenwidth = 2.4*radlen_w;
  
  if (radius > preshower_inner_radius) {
    cout << "inconsistency: svx radius+thickness: " << radius 
	 << " larger than preshower inner radius: " <<  preshower_inner_radius
	 << endl;
    gSystem->Exit(-1);
  }
    
  radius = preshower_inner_radius;

  //---------------
  // Load libraries
  //---------------

  gSystem->Load("libg4detectors.so");
  gSystem->Load("libg4testbench.so");

  PHG4CylinderSubsystem *cyl;

  cyl = new PHG4CylinderSubsystem("ABSORBER_PRESHOWER", 1);
  cyl->SetRadius(radius);
  cyl->SetMaterial("G4_W");
  cyl->SetThickness(tungstenwidth);
  cyl->SuperDetector("ABSORBER_PRESHOWER");
  if (absorberactive) cyl->SetActive();
  g4Reco->registerSubsystem( cyl );
  radius += tungstenwidth;
  radius += no_overlapp;

  cyl = new PHG4CylinderSubsystem("PRESHOWER", 1);
  cyl->SetRadius(radius);
  cyl->SetMaterial("G4_Si");
  cyl->SetThickness(0.0625);
  cyl->SetActive();
  cyl->SuperDetector("PRESHOWER");
  g4Reco->registerSubsystem( cyl );
  radius += 0.0625;
  radius += no_overlapp;

  return radius;
}
