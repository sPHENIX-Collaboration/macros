// these are defined in the Fun4All macro, here we just override the values
// with what is used in this macro
Min_cemc_layer = 1;
Max_cemc_layer = 1;
//Spacal_Tilt = 0;

// just a dummy parameter used by the tilted plate geom
void CEmcInit(const int nslats = 1)
{
  Min_cemc_layer = 1;
  Max_cemc_layer = 1;
//  Spacal_Tilt = 0;
}

double
CEmc(PHG4Reco* g4Reco, double radius, const int crossings, const int absorberactive = 0)
{
  double emc_inner_radius = 95.; // emc inner radius from engineering drawing
  double cemcthickness = 12.7;
  double emc_outer_radius = emc_inner_radius + cemcthickness; // outer radius
  
  if (radius > emc_inner_radius) {
    cout << "inconsistency: preshower radius+thickness: " << radius 
	 << " larger than emc inner radius: " <<  emc_inner_radius
	 << endl;
    gSystem->Exit(-1);
  }
  
  //---------------
  // Load libraries
  //---------------

  gSystem->Load("libg4detectors.so");
  gSystem->Load("libg4testbench.so");

  // the radii are only to determined the thickness of the cemc
  radius = emc_inner_radius;

  int ilayer = Min_cemc_layer;
  PHG4SpacalSubsystem *cemc; // sorry the tilted slats are called HCal
  cemc = new PHG4SpacalSubsystem("CEMC", ilayer);

  cemc ->get_geom().set_radius(emc_inner_radius);
  cemc ->get_geom().set_thickness(cemcthickness);
  cemc ->get_geom().set_construction_verbose(2);

  cemc->SetActive();
  cemc->SuperDetector("CEMC");
  if (absorberactive)  cemc->SetAbsorberActive();
  cemc->OverlapCheck(overlapcheck);
  
  g4Reco->registerSubsystem( cemc );

  if (ilayer > Max_cemc_layer)
    {
      cout << "layer discrepancy, current layer " << ilayer
           << " max cemc layer: " << Max_cemc_layer << endl;
    }

  radius += cemcthickness;
  radius += no_overlapp;
  
  PHG4CylinderSubsystem *cyl = new PHG4CylinderSubsystem("EMCELECTRONICS", 0);
  cyl->SetRadius(radius);
  cyl->SetMaterial("G4_TEFLON"); // plastic
  cyl->SetThickness(0.5);
  if (absorberactive)  cyl->SetActive();
  g4Reco->registerSubsystem( cyl );
  radius += 0.5;
  radius += no_overlapp;
  
  return radius;
}

double
CEmc_Proj(PHG4Reco* g4Reco, double radius, const int crossings, const int absorberactive = 0)
{
  double emc_inner_radius = 95.; // emc inner radius from engineering drawing
  double cemcthickness = 12.9+1.5;
  double emc_outer_radius = emc_inner_radius + cemcthickness; // outer radius
  
  if (radius > emc_inner_radius) {
    cout << "inconsistency: preshower radius+thickness: " << radius 
	 << " larger than emc inner radius: " <<  emc_inner_radius
	 << endl;
    gSystem->Exit(-1);
  }
  
  //---------------
  // Load libraries
  //---------------

  gSystem->Load("libg4detectors.so");
  gSystem->Load("libg4testbench.so");

  cout <<"Get_Spacal_Tilt() = "<<Get_Spacal_Tilt()<<endl;

  // the radii are only to determined the thickness of the cemc
  //double emc_inner_radius = radius; // emc inner radius from engineering spreadsheet
  // double cemcthickness = 12.9+1.5;
  //  double emc_outer_radius = emc_inner_radius + cemcthickness; // outer radius
  radius = emc_outer_radius;
  
  int ilayer = Min_cemc_layer;
  PHG4SpacalSubsystem *cemc; // sorry the tilted slats are called HCal
  cemc = new PHG4SpacalSubsystem("CEMC", ilayer);

  cemc ->get_geom().set_radius(emc_inner_radius);
  cemc ->get_geom().set_thickness(cemcthickness);
  cemc ->get_geom().set_construction_verbose(2);

  cemc ->get_geom().set_config(PHG4CylinderGeom_Spacalv1::kProjective_PolarTaper);
  cemc ->get_geom().set_azimuthal_tilt(Get_Spacal_Tilt());
//  cemc ->get_geom().set_azimuthal_tilt(-2.3315/2./95*1.9);
//  cemc ->get_geom().set_azimuthal_tilt(6.28/256*2);
//  cemc ->get_geom().set_azimuthal_tilt(-6.28/256*4);
//  cemc ->get_geom().set_azimuthal_seg_visible(true);
//  cemc ->get_geom().set_virualize_fiber(false);
//  cemc ->get_geom().set_assembly_spacing(0.001);
  cemc ->get_geom().set_polar_taper_ratio(1.128);
//  cemc ->get_geom().set_polar_taper_ratio(1.123);
//  cemc ->get_geom().set_polar_taper_ratio(1.117);
//  cemc ->get_geom().set_absorber_mat("G4_AIR");
//  cemc ->get_geom().set_azimuthal_n_sec(256/2);

  cemc->SetActive();
  cemc->SuperDetector("CEMC");
  if (absorberactive)  cemc->SetAbsorberActive();
  cemc->OverlapCheck(overlapcheck);

  g4Reco->registerSubsystem( cemc );

  if (ilayer > Max_cemc_layer)
    {
      cout << "layer discrepancy, current layer " << ilayer
           << " max cemc layer: " << Max_cemc_layer << endl;
    }

  radius += cemcthickness;
  radius += no_overlapp;

  PHG4CylinderSubsystem *cyl = new PHG4CylinderSubsystem("EMCELECTRONICS", 0);
  cyl->SetRadius(radius);
  cyl->SetMaterial("G4_TEFLON"); // plastic
  cyl->SetThickness(0.5);
  if (absorberactive)  cyl->SetActive();
  g4Reco->registerSubsystem( cyl );
  radius += 0.5;
  radius += no_overlapp;

  return radius;
}

double
CEmc_Vis(PHG4Reco* g4Reco, double radius, const int crossings, const int absorberactive = 0)
{
  double emc_inner_radius = 95.; // emc inner radius from engineering drawing
  double cemcthickness = 12.7;
  double emc_outer_radius = emc_inner_radius + cemcthickness; // outer radius
  
  if (radius > emc_inner_radius) {
    cout << "inconsistency: preshower radius+thickness: " << radius 
	 << " larger than emc inner radius: " <<  emc_inner_radius
	 << endl;
    gSystem->Exit(-1);
  }
  
  //---------------
  // Load libraries
  //---------------

  gSystem->Load("libg4detectors.so");
  gSystem->Load("libg4testbench.so");

  // the radii are only to determined the thickness of the cemc
  radius = emc_inner_radius;

  int ilayer = Min_cemc_layer;
  PHG4SpacalSubsystem *cemc; // sorry the tilted slats are called HCal
  cemc = new PHG4SpacalSubsystem("CEMC", ilayer);

  cemc ->get_geom().set_radius(emc_inner_radius);
  cemc ->get_geom().set_thickness(cemcthickness);
  cemc ->get_geom().set_virualize_fiber(true);
  cemc ->get_geom().set_construction_verbose(2);

  cemc->SetActive();
  cemc->SuperDetector("CEMC");
  if (absorberactive)  cemc->SetAbsorberActive();
  cemc->OverlapCheck(overlapcheck);

  g4Reco->registerSubsystem( cemc );

  if (ilayer > Max_cemc_layer)
    {
      cout << "layer discrepancy, current layer " << ilayer
           << " max cemc layer: " << Max_cemc_layer << endl;
    }

  radius += cemcthickness;
  radius += no_overlapp;
  
  PHG4CylinderSubsystem *cyl = new PHG4CylinderSubsystem("EMCELECTRONICS", 0);
  cyl->SetRadius(radius);
  cyl->SetMaterial("G4_TEFLON"); // plastic
  cyl->SetThickness(0.5);
  if (absorberactive)  cyl->SetActive();
  g4Reco->registerSubsystem( cyl );
  radius += 0.5;
  radius += no_overlapp;
  
  return radius;
}


double
CEmc_Ionizing(PHG4Reco* g4Reco, double radius, const int crossings, const int absorberactive = 0)
{
  double emc_inner_radius = 95.; // emc inner radius from engineering drawing
  double cemcthickness = 12.7;
  double emc_outer_radius = emc_inner_radius + cemcthickness; // outer radius
  
  if (radius > emc_inner_radius) {
    cout << "inconsistency: preshower radius+thickness: " << radius 
	 << " larger than emc inner radius: " <<  emc_inner_radius
	 << endl;
    gSystem->Exit(-1);
  }
  
  //---------------
  // Load libraries
  //---------------

  gSystem->Load("libg4detectors.so");
  gSystem->Load("libg4testbench.so");

  // the radii are only to determined the thickness of the cemc
  radius = emc_inner_radius;

  int ilayer = Min_cemc_layer;
  PHG4SpacalSubsystem *cemc; // sorry the tilted slats are called HCal
  cemc = new PHG4SpacalSubsystem("CEMC", ilayer);

  cemc ->get_geom().set_radius(emc_inner_radius);
  cemc ->get_geom().set_thickness(cemcthickness);
  cemc ->get_geom().set_construction_verbose(2);
  cemc ->get_geom().set_fiber_use_ionizing_loss(true);

  cemc->SetActive();
  cemc->SuperDetector("CEMC");
  if (absorberactive)  cemc->SetAbsorberActive();
  cemc->OverlapCheck(overlapcheck);

  g4Reco->registerSubsystem( cemc );

  if (ilayer > Max_cemc_layer)
    {
      cout << "layer discrepancy, current layer " << ilayer
           << " max cemc layer: " << Max_cemc_layer << endl;
    }

  radius += cemcthickness;
  radius += no_overlapp;
  
  PHG4CylinderSubsystem *cyl = new PHG4CylinderSubsystem("EMCELECTRONICS", 0);
  cyl->SetRadius(radius);
  cyl->SetMaterial("G4_TEFLON"); // plastic
  cyl->SetThickness(0.5);
  if (absorberactive)  cyl->SetActive();
  g4Reco->registerSubsystem( cyl );
  radius += 0.5;
  radius += no_overlapp;
  
  return radius;
}

double
CEmc_FineSampling(PHG4Reco* g4Reco, double radius, const int crossings, const int absorberactive = 0)
{
  double emc_inner_radius = 95.; // emc inner radius from engineering drawing
  double cemcthickness = 12.7;
  double emc_outer_radius = emc_inner_radius + cemcthickness; // outer radius
  
  if (radius > emc_inner_radius) {
    cout << "inconsistency: preshower radius+thickness: " << radius 
	 << " larger than emc inner radius: " <<  emc_inner_radius
	 << endl;
    gSystem->Exit(-1);
  }
  
  const double scale_ratio = 0.1;
  cout <<"CEmc_FineSampling - WARNING - with scale ratio of "<<scale_ratio<<endl;

  //---------------
  // Load libraries
  //---------------

  gSystem->Load("libg4detectors.so");
  gSystem->Load("libg4testbench.so");

  // the radii are only to determined the thickness of the cemc
  radius = emc_inner_radius;

  int ilayer = Min_cemc_layer;
  PHG4SpacalSubsystem *cemc; // sorry the tilted slats are called HCal
  cemc = new PHG4SpacalSubsystem("CEMC", ilayer);

  cemc ->get_geom().set_radius(emc_inner_radius);
  cemc ->get_geom().set_thickness(cemcthickness);
  cemc ->get_geom().set_construction_verbose(2);
  cemc ->get_geom().set_fiber_core_diameter(0.047 * scale_ratio);
  cemc ->get_geom().set_fiber_clading_thickness(0.003 / 2 * scale_ratio);
  cemc ->get_geom().set_fiber_distance(0.1 * scale_ratio);

  cemc->SetActive();
  cemc->SuperDetector("CEMC");
  if (absorberactive)  cemc->SetAbsorberActive();
  cemc->OverlapCheck(overlapcheck);

  g4Reco->registerSubsystem( cemc );

  if (ilayer > Max_cemc_layer)
    {
      cout << "layer discrepancy, current layer " << ilayer
           << " max cemc layer: " << Max_cemc_layer << endl;
    }

  radius += cemcthickness;
  radius += no_overlapp;
  
  PHG4CylinderSubsystem *cyl = new PHG4CylinderSubsystem("EMCELECTRONICS", 0);
  cyl->SetRadius(radius);
  cyl->SetMaterial("G4_TEFLON"); // plastic
  cyl->SetThickness(0.5);
  if (absorberactive)  cyl->SetActive();
  g4Reco->registerSubsystem( cyl );
  radius += 0.5;
  radius += no_overlapp;
  
  return radius;
}

void CEMC_Cells(int verbosity = 0) {

  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4detectors.so");
  Fun4AllServer *se = Fun4AllServer::instance();
  
  PHG4CylinderCellReco *cemc_cells = new PHG4CylinderCellReco("CEMCCYLCELLRECO");
  cemc_cells->Detector("CEMC");
  cemc_cells->Verbosity(verbosity);
  for (int i = Min_cemc_layer; i <= Max_cemc_layer; i++) {
    cemc_cells->etaphisize(i, 0.024, 0.024);
  }
  se->registerSubsystem(cemc_cells);

  return;
}

void CEMC_Towers(int verbosity = 0) {

  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4detectors.so");
  Fun4AllServer *se = Fun4AllServer::instance();
  
  RawTowerBuilder *TowerBuilder = new RawTowerBuilder("EmcRawTowerBuilder");
  TowerBuilder->Detector("CEMC");
  TowerBuilder->Verbosity(verbosity);
  se->registerSubsystem( TowerBuilder );
      
  return;
}

void CEMC_Clusters(int verbosity = 0) {

  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4detectors.so");
  Fun4AllServer *se = Fun4AllServer::instance();
  
  RawClusterBuilder* ClusterBuilder = new RawClusterBuilder("EmcRawClusterBuilder");
  ClusterBuilder->Detector("CEMC");
  ClusterBuilder->Verbosity(verbosity);
  se->registerSubsystem( ClusterBuilder );
  
  return;
}

void CEMC_Eval(std::string outputfile, int verbosity = 0) {

  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4eval.so");
  Fun4AllServer *se = Fun4AllServer::instance();
    
  PHG4CalEvaluator* eval = new PHG4CalEvaluator("PHG4CEMCEVALUATOR", outputfile.c_str());
  eval->Verbosity(0);
  eval->Detector("CEMC");
  se->registerSubsystem( eval );
      
  return;
}
