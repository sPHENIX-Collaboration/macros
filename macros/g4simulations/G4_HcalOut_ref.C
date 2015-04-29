// these are defined in the Fun4All macro, here we just override the values
// with what is used in this macro
Min_hcal_out_layer = 0;
Max_hcal_out_layer = 0;

void HCalOuterInit()
{
  Min_hcal_out_layer = 1;
  Max_hcal_out_layer = 1;
}

double HCalOuter(PHG4Reco* g4Reco,
		 double radius,
		 const int crossings,
		 const int absorberactive = 0,
		 int verbosity = 0) {
  
  double outer_hcal_inner_radius = 178.0; // stay clear value
  double outer_hcal_outer_radius = 260.3;
   
  if (radius > outer_hcal_inner_radius) {
    cout << "inconsistency: radius: " << radius 
	 << " larger than HCALOUT inner radius: " << outer_hcal_inner_radius << endl;
    gSystem->Exit(-1);
  }
  
  gSystem->Load("libg4detectors.so");
  gSystem->Load("libg4testbench.so");

  PHG4OuterHcalSubsystem *outerhcal = new PHG4OuterHcalSubsystem("HCALOUT");
  outerhcal->SetActive();
  //outerhcal->SetLightCorrection(178.0,0.65,260.3,1.0);
  outerhcal->SuperDetector("HCALOUT");
  if (absorberactive)  outerhcal->SetAbsorberActive();
  outerhcal->OverlapCheck(overlapcheck);
  g4Reco->registerSubsystem( outerhcal );

  radius = outer_hcal_outer_radius;
  
  if (verbosity >= 0) {
    cout << "==================== G4_HcalOut_ref.C::HCalOuter() ========================" << endl;
    cout << " CVS Version: $Id: G4_HcalOut_ref.C,v 1.2 2015/04/14 23:19:30 mccumber Exp $" << endl;
    cout << " HCALOUT Material Description:" << endl;
    cout << "  outer radius = " << outer_hcal_outer_radius << " cm" << endl;
    cout << "===========================================================================" << endl;
  }

  radius += no_overlapp;
  
  return radius; 
}

void HCALOuter_Cells(int verbosity = 0) {

  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4detectors.so");
  Fun4AllServer *se = Fun4AllServer::instance();
  
  PHG4HcalCellReco *hc = new PHG4HcalCellReco();
  hc->Detector("HCALOUT");
  hc->etasize_nslat(0, 0, 5);
  se->registerSubsystem(hc);

  return;
}

void HCALOuter_Towers(int verbosity = 0) {

  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4detectors.so");
  Fun4AllServer *se = Fun4AllServer::instance();
  
  RawTowerBuilder* TowerBuilder = new RawTowerBuilder("HcalOutRawTowerBuilder");
  TowerBuilder->Detector("HCALOUT");
  TowerBuilder->Verbosity(verbosity);
  se->registerSubsystem( TowerBuilder );
  
  return;
}

void HCALOuter_Clusters(int verbosity = 0) {

  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4detectors.so");
  Fun4AllServer *se = Fun4AllServer::instance();
  
  RawClusterBuilder* ClusterBuilder = new RawClusterBuilder("HcalOutRawClusterBuilder");
  ClusterBuilder->Detector("HCALOUT");
  ClusterBuilder->Verbosity(verbosity);
  se->registerSubsystem( ClusterBuilder );
  
  return;
}

void HCALOuter_Eval(std::string outputfile, int verbosity = 0) {

  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4eval.so");
  Fun4AllServer *se = Fun4AllServer::instance();
  
  PHG4CalEvaluator* eval = new PHG4CalEvaluator("PHG4HCALOUTEVALUATOR", outputfile.c_str());
  eval->Verbosity(verbosity);
  eval->Detector("HCALOUT");
  se->registerSubsystem( eval );
  
  return;
}
