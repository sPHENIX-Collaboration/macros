
void PipeInit() {}

double Pipe(PHG4Reco* g4Reco,
	    double radius,
	    const int absorberactive = 0,
	    int verbosity = 0) {

  double pipe_radius    = 2.16;   // 2.16 cm based on spec sheet
  double pipe_thickness = 0.0760; // 760 um based on spec sheet
  double pipe_length = g4Reco->GetWorldSizeZ() - no_overlapp; // entire volume
  if (radius > pipe_radius) {
    cout << "inconsistency: radius: " << radius 
	 << " larger than Pipe inner radius: " << pipe_radius << endl;
    gSystem->Exit(-1);
  }
  
  gSystem->Load("libg4detectors.so");
  gSystem->Load("libg4testbench.so");

  PHG4CylinderSubsystem *cyl = new PHG4CylinderSubsystem("PIPE", 0);
  cyl->SetRadius(pipe_radius);
  cyl->SetLengthViaRapidityCoverage(false);
  cyl->SetLength(pipe_length);
  cyl->SetMaterial("G4_Be");
  cyl->SetThickness(pipe_thickness);
  cyl->SuperDetector("PIPE");
  if (absorberactive)  cyl->SetActive();
  g4Reco->registerSubsystem( cyl );

  radius = pipe_radius + pipe_thickness;
  
  if (verbosity > 0) {
    cout << "=========================== G4_Pipe.C::Pipe() =============================" << endl;
    cout << " PIPE Material Description:" << endl;
    cout << "  inner radius = " << pipe_radius << " cm" << endl;
    cout << "  thickness = " << pipe_thickness << " cm" << endl;
    cout << "  outer radius = " << pipe_radius + pipe_thickness << " cm" << endl;
    cout << "  length = " << pipe_length << " cm" << endl;
    cout << "===========================================================================" << endl;
  }

  radius += no_overlapp;
  
  return radius; 
}
