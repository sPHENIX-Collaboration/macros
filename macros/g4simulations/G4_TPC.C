//======
void TPCDetector(PHG4Reco* g4Reco, int verbosity=0) {
  gSystem->Load("libTPCsimu.so");
  Fun4AllServer *se = Fun4AllServer::instance();
  TPCDetectorSubsystem *tpc = new TPCDetectorSubsystem();
  tpc->Verbosity(verbosity);
  g4Reco->registerSubsystem( tpc );
}
//======
void TPCSimulation(int verbosity=0,bool makeTree=false,int event=0) {
  Fun4AllServer *se = Fun4AllServer::instance();
  TPCSimulationSubsystem *tpc = new TPCSimulationSubsystem();
  tpc->Verbosity(verbosity);
  if(makeTree) tpc->SetTreeFileName( Form("tree/hits_%03d.root",event) );
  se->registerSubsystem( tpc );
}
