// these are defined in the Fun4All macro, here we just override the values
// with what is used in this macro
Min_cemc_layer = 1;
Max_cemc_layer = 1;
//Spacal_Tilt = 0;

// just a dummy parameter used by the tilted plate geom
void
CEmcInit(const int nslats = 1)
{
  Min_cemc_layer = 1;
  Max_cemc_layer = 1;
//  Spacal_Tilt = 0;
}

double
CEmc(PHG4Reco* g4Reco, double radius, const int crossings,
    const int absorberactive = 0)
{
  double emc_inner_radius = 90.; // emc inner radius from engineering drawing
  double cemcthickness = 26.00000 - 0.001;
  double emc_outer_radius = emc_inner_radius + cemcthickness; // outer radius

  if (radius > emc_inner_radius)
    {
      cout << "inconsistency: preshower radius+thickness: " << radius
          << " larger than emc inner radius: " << emc_inner_radius << endl;
      gSystem->Exit(-1);
    }

  //---------------
  // Load libraries
  //---------------

  gSystem->Load("libg4detectors.so");

  // the radii are only to determined the thickness of the cemc
  radius = emc_inner_radius;

  //---------------
  // Load libraries
  //---------------

  // 1.5cm thick teflon as an approximation for EMCAl light collection + electronics (10% X0 total estimated)
  PHG4CylinderSubsystem *cyl = new PHG4CylinderSubsystem("CEMC_ELECTRONICS", 0);
  cyl->SuperDetector("CEMC_ELECTRONICS");
  cyl->SetRadius(radius);
  cyl->SetMaterial("G4_TEFLON"); // plastic
  cyl->SetThickness(1.5- no_overlapp);
  if (absorberactive)  cyl->SetActive();
  g4Reco->registerSubsystem( cyl );

  radius += 2;
  cemcthickness -= 2;


  // 0.5cm thick Stainless Steel as an approximation for EMCAl support system
  cyl = new PHG4CylinderSubsystem("CEMC_SPT", 0);
  cyl->SuperDetector("CEMC_SPT");
  cyl->SetRadius(radius +cemcthickness - 0.5 );
  cyl->SetMaterial("SS310"); // SS310 Stainless Steel
  cyl->SetThickness(0.5);
  if (absorberactive)
    cyl->SetActive();
  g4Reco->registerSubsystem(cyl);

  cemcthickness -= 0.5;


  //---------------
  // Load libraries
  //---------------

  int ilayer = Min_cemc_layer;
  PHG4SpacalSubsystem *cemc; // sorry the tilted slats are called HCal
  cemc = new PHG4SpacalSubsystem("CEMC", ilayer);

  cemc->get_geom().set_config(
      PHG4CylinderGeom_Spacalv1::kFullProjective_2DTaper_SameLengthFiberPerTower);

  // load from hard code map for now. Need to move to calibration file or database.
  cemc->get_geom().load_demo_sector_tower_map3();
  cemc->get_geom().set_radius(radius);
  cemc->get_geom().set_thickness(cemcthickness);
  cemc->get_geom().set_construction_verbose(1);

  cemc->SetActive();
  cemc->SuperDetector("CEMC");
  if (absorberactive)
    cemc->SetAbsorberActive();
  cemc->OverlapCheck(overlapcheck);

  g4Reco->registerSubsystem(cemc);

  if (ilayer > Max_cemc_layer)
    {
      cout << "layer discrepancy, current layer " << ilayer
          << " max cemc layer: " << Max_cemc_layer << endl;
    }

  radius += cemcthickness;
  radius += no_overlapp;

  return radius;
}

double
CEmc_Vis(PHG4Reco* g4Reco, double radius, const int crossings,
    const int absorberactive = 0)
{
  double emc_inner_radius = 90.; // emc inner radius from engineering drawing
  double cemcthickness = 26.00000 - 0.001;
  ;
  double emc_outer_radius = emc_inner_radius + cemcthickness; // outer radius

  if (radius > emc_inner_radius)
    {
      cout << "inconsistency: preshower radius+thickness: " << radius
          << " larger than emc inner radius: " << emc_inner_radius << endl;
      gSystem->Exit(-1);
    }

  //---------------
  // Load libraries
  //---------------

  gSystem->Load("libg4detectors.so");

  // the radii are only to determined the thickness of the cemc
  radius = emc_inner_radius;

  //---------------
  // Load libraries
  //---------------
  PHG4CylinderSubsystem *cyl = new PHG4CylinderSubsystem("EMCELECTRONICS", 0);
  cyl->SetRadius(radius);
  cyl->SetMaterial("G4_TEFLON"); // plastic
  cyl->SetThickness(2 - no_overlapp);
  if (absorberactive)
    cyl->SetActive();
  g4Reco->registerSubsystem(cyl);

  radius += 2;
  cemcthickness -= 2;

  //---------------
  // Load libraries
  //---------------

  int ilayer = Min_cemc_layer;
  PHG4SpacalSubsystem *cemc; // sorry the tilted slats are called HCal
  cemc = new PHG4SpacalSubsystem("CEMC", ilayer);

  cemc->get_geom().set_config(
      PHG4CylinderGeom_Spacalv1::kFullProjective_2DTaper_SameLengthFiberPerTower);

  // load from hard code map for now. Need to move to calibration file or database.
  cemc->get_geom().load_demo_sector_tower_map3();
  cemc->get_geom().set_radius(radius);
  cemc->get_geom().set_thickness(cemcthickness);

  cemc->get_geom().set_construction_verbose(2);
  cemc->get_geom().set_virualize_fiber(false);
  cemc->get_geom().set_azimuthal_seg_visible(true);

  cemc->SetActive();
  cemc->SuperDetector("CEMC");
  if (absorberactive)
    cemc->SetAbsorberActive();
  cemc->OverlapCheck(overlapcheck);

  g4Reco->registerSubsystem(cemc);

  if (ilayer > Max_cemc_layer)
    {
      cout << "layer discrepancy, current layer " << ilayer
          << " max cemc layer: " << Max_cemc_layer << endl;
    }

  radius += cemcthickness;
  radius += no_overlapp;

  return radius;
}

void
CEMC_Cells(int verbosity = 0)
{

  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4detectors.so");
  Fun4AllServer *se = Fun4AllServer::instance();

  PHG4CylinderCellReco *cemc_cells = new PHG4CylinderCellReco(
      "CEMCCYLCELLRECO");
  cemc_cells->Detector("CEMC");
  cemc_cells->Verbosity(verbosity);
  for (int i = Min_cemc_layer; i <= Max_cemc_layer; i++)
    {
      cemc_cells->etaphisize(i, 0.024, 0.024);
    }
  se->registerSubsystem(cemc_cells);

  return;
}

void
CEMC_Towers(int verbosity = 0)
{

  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4detectors.so");
  Fun4AllServer *se = Fun4AllServer::instance();

  RawTowerBuilder *TowerBuilder = new RawTowerBuilder("EmcRawTowerBuilder");
  TowerBuilder->Detector("CEMC");
  TowerBuilder->Verbosity(verbosity);
  se->registerSubsystem(TowerBuilder);

  return;
}

void
CEMC_Clusters(int verbosity = 0)
{

  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4detectors.so");
  Fun4AllServer *se = Fun4AllServer::instance();

  RawClusterBuilder* ClusterBuilder = new RawClusterBuilder(
      "EmcRawClusterBuilder");
  ClusterBuilder->Detector("CEMC");
  ClusterBuilder->Verbosity(verbosity);
  se->registerSubsystem(ClusterBuilder);

  return;
}

void
CEMC_Eval(std::string outputfile, int verbosity = 0)
{

  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4eval.so");
  Fun4AllServer *se = Fun4AllServer::instance();

  PHG4CalEvaluator* eval = new PHG4CalEvaluator("PHG4CEMCEVALUATOR",
      outputfile.c_str());
  eval->Verbosity(0);
  eval->Detector("CEMC");
  se->registerSubsystem(eval);

  return;
}
