// these are defined in the Fun4All macro, here we just override the values
// with what is used in this macro
Min_cemc_layer = 1;
Max_cemc_layer = 1;
int Cemc_slats_multiple = 1;

void CEmcInit(const int nslats = 1)
{
  Min_cemc_layer = 1;
  Max_cemc_layer = 1;
  Cemc_slats_multiple = nslats;
}

double
CEmc(PHG4Reco* g4Reco, double radius, const int crossings, const int absorberactive = 0)
{

  //---------------
  // Load libraries
  //---------------

  gSystem->Load("libg4detectors.so");
  gSystem->Load("libg4testbench.so");

  double cemc_thickness = 12.7;
  double tungsten_width = 0.1; // 1mm tungsten
  double scinti_width = 0.1; // 1mm scintillator
  int numscint = int(radius*2*TMath::Pi()/(tungsten_width+scinti_width));
  numscint = (numscint/Cemc_slats_multiple)*Cemc_slats_multiple;
  int ilayer = Min_cemc_layer;
  PHG4HcalSubsystem *cemc; // sorry the tilted slats are called HCal
  cemc = new PHG4HcalSubsystem("CEMC", ilayer);
  cemc->SetRadius(radius);
  cemc->SetMaterial("G4_W");
  cemc->SetThickness(cemc_thickness);
  cemc->SetScintWidth(scinti_width);
  cemc->SetNumScint(numscint);
  cemc->SetTiltViaNcross(crossings);
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

  radius += cemc_thickness;
  radius += no_overlapp;  // if inner radius equal outer radius we have overlapps

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

  PHG4SlatCellReco *cemc_cells = new PHG4SlatCellReco("CEMCSLATCELLRECO");
  cemc_cells->Detector("CEMC");
  cemc_cells->Verbosity(verbosity);
  for (int i = Min_cemc_layer; i <= Max_cemc_layer; i++) {
    cemc_cells->etasize_nslat(i, 0.024, Cemc_slats_per_cell/8);
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
