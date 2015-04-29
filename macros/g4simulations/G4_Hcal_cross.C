// these are defined in the Fun4All macro, here we just override the values
// with what is used in this macro
Min_hcal_in_layer = 1;
Max_hcal_in_layer = 1;
Min_hcal_out_layer = 1;
Max_hcal_out_layer = 1;

void HCalInit()
{
  Min_hcal_in_layer = 1;
  Max_hcal_in_layer = 1;
  Min_hcal_out_layer = 1;
  Max_hcal_out_layer = 1;
}

double
HCal(PHG4Reco* g4Reco, double radius, const int crossings, const int absorberactive = 0)
{

  //---------------
  // Load libraries
  //---------------

  gSystem->Load("libg4detectors.so");
  gSystem->Load("libg4testbench.so");

  double inner_hcal_inner_radius = 180.00;
  double outer_hcal_inner_radius = 210.00;
  double outer_hcal_outer_radius = 270.00;
  double hcal_inner_thickness = outer_hcal_inner_radius - inner_hcal_inner_radius;
  double hcal_outer_thickness = outer_hcal_outer_radius - outer_hcal_inner_radius;

  int ilayer = Min_hcal_in_layer;
  PHG4HcalSubsystem *hcal;
  hcal = new PHG4HcalSubsystem("HCALIN", ilayer);
  hcal->SetRadius(radius);
  hcal->SetMaterial("G4_Fe");
  hcal->SetThickness(hcal_inner_thickness);
  hcal->SetScintWidth(0.7);
  hcal->SetNumScint(64*6);
  hcal->SetTiltViaNcross(-crossings);
  hcal->SetActive();
  hcal->SuperDetector("HCALIN");
  if (absorberactive)  hcal->SetAbsorberActive();
  hcal->OverlapCheck(overlapcheck);
  
  g4Reco->registerSubsystem( hcal );

  radius += hcal_inner_thickness;
  radius += no_overlapp;  // if inner radius equal outer radius we have overlapps

  ilayer = Min_hcal_out_layer;
  hcal = new PHG4HcalSubsystem("HCALOUT", ilayer);
  hcal->SetRadius(radius);
  hcal->SetMaterial("G4_Fe");
  hcal->SetThickness(hcal_outer_thickness);
  hcal->SetScintWidth(0.7);
  hcal->SetNumScint(64*6);
  hcal->SetTiltViaNcross(crossings);
  hcal->SetActive();
  hcal->SuperDetector("HCALOUT");
  if (absorberactive)  hcal->SetAbsorberActive();
  hcal->OverlapCheck(overlapcheck);
  g4Reco->registerSubsystem( hcal );

  if (ilayer > Max_hcal_in_layer)
    {
      cout << "layer discrepancy, current layer " << ilayer
           << " max hcal layer: " << Max_hcal_in_layer << endl;
    }

  radius += hcal_outer_thickness;
  radius += no_overlapp;  // if inner radius equal outer radius we have overlapps
  return radius;
  
}
