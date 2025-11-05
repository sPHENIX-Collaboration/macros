/*
 * This macro shows a minimum working example of running the tracking
 * hit unpackers with some basic seeding algorithms to try to put together
 * tracks. There are some analysis modules run at the end which package
 * hits, clusters, and clusters on tracks into trees for analysis.
 */

#include <G4_ActsGeom.C>
#include <G4_Magnet.C>
#include <GlobalVariables.C>
#include <Trkr_Clustering.C>
#include <Trkr_RecoInit.C>
#include <Trkr_Reco.C>

#include <ffamodules/CDBInterface.h>
#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllUtils.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllRunNodeInputManager.h>
#include <fun4all/Fun4AllServer.h>


#include <phool/recoConsts.h>

#include <trackreco/AzimuthalSeeder.h>
#include <trackingdiagnostics/TrackResiduals.h>

#include <stdio.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libmvtx.so)
R__LOAD_LIBRARY(libintt.so)
R__LOAD_LIBRARY(libtpc.so)
R__LOAD_LIBRARY(libmicromegas.so)
R__LOAD_LIBRARY(libTrackingDiagnostics.so)
void Fun4All_MvtxVertex(
    const int nEvents = 0,
    const std::string filename = "DST_INTT_RAW_beam_new_2023p011-00041620-0000.root",
    const std::string dir = "/sphenix/lustre01/sphnxpro/commissioning/slurp/inttbeam/run_00041600_00041700/",
    const std::string outfilename = "clusters_seeds_40630.root")
{
  std::string inputRawHitFile = dir + filename;

  std::pair<int, int> runseg = Fun4AllUtils::GetRunSegment(filename);
  int runnumber = runseg.first;
  int segment = runseg.second;

  auto se = Fun4AllServer::instance();
  se->Verbosity(1);
  auto rc = recoConsts::instance();
  rc->set_IntFlag("RUNNUMBER", runnumber);

  Enable::CDB = true;
  rc->set_StringFlag("CDB_GLOBALTAG", "ProdA_2023");
  rc->set_uint64Flag("TIMESTAMP", 6);
  std::string geofile = CDBInterface::instance()->getUrl("Tracking_Geometry");

  Fun4AllRunNodeInputManager *ingeo = new Fun4AllRunNodeInputManager("GeoIn");
  ingeo->AddFile(geofile);
  se->registerInputManager(ingeo);

  G4TPC::tpc_drift_velocity_reco = (8.0 / 1000) * 107.0 / 105.0;
  G4MAGNET::magfield = "0.01";
  G4MAGNET::magfield_tracking = G4MAGNET::magfield;
  G4MAGNET::magfield_rescale = 1;
  TrackingInit();

  auto hitsin = new Fun4AllDstInputManager("InputManager");
  hitsin->fileopen(inputRawHitFile);
  se->registerInputManager(hitsin);


  Mvtx_HitUnpacking();

  Mvtx_Clustering();

  AzimuthalSeeder *seeder = new AzimuthalSeeder;
  se->registerSubsystem(seeder);

  auto converter = new TrackSeedTrackMapConverter;
  converter->setTrackSeedName("SiliconTrackSeedContainer");
  converter->setFieldMap(G4MAGNET::magfield_tracking);
  converter->Verbosity(0);
  converter->constField();
  se->registerSubsystem(converter);

  PHSimpleVertexFinder *finder = new PHSimpleVertexFinder;
  finder->Verbosity(0);
  finder->setDcaCut(0.5);
  finder->setTrackPtCut(-99999.);
  finder->setBeamLineCut(1);
  finder->setTrackQualityCut(1000000000);
  finder->setNmvtxRequired(3);
  finder->setOutlierPairCut(0.1);
  se->registerSubsystem(finder);

  TString residoutfile = outfilename + filename + "_resid.root";
  std::string residstring(residoutfile.Data());

  auto resid = new TrackResiduals("TrackResiduals");
  resid->outfileName(residstring);
  resid->alignment(false);
  resid->clusterTree();
  resid->hitTree();
  resid->zeroField();
  se->registerSubsystem(resid);


  se->run(nEvents);
  se->End();
  se->PrintTimer();

  delete se;
  std::cout << "Finished" << std::endl;
  gSystem->Exit(0);
}
