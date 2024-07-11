/*
 * This macro shows a minimum working example of running the tracking
 * hit unpackers with some basic seeding algorithms to try to put together
 * tracks. There are some analysis modules run at the end which package
 * hits, clusters, and clusters on tracks into trees for analysis.
 */

#include <G4_ActsGeom.C>
#include <G4_Magnet.C>
#include <GlobalVariables.C>
#include <QA.C>
#include <Trkr_Clustering.C>
#include <Trkr_Reco_Cosmics.C>

#include <ffamodules/CDBInterface.h>
#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllRunNodeInputManager.h>
#include <fun4all/Fun4AllServer.h>

#include <phool/recoConsts.h>

#include <trackingqa/InttClusterQA.h>

#include <trackingqa/MicromegasClusterQA.h>

#include <trackingqa/MvtxClusterQA.h>

#include <trackingqa/TpcClusterQA.h>

#include <trackingdiagnostics/TrackResiduals.h>
#include <trackingdiagnostics/TrkrNtuplizer.h>

#include <stdio.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libmvtx.so)
R__LOAD_LIBRARY(libintt.so)
R__LOAD_LIBRARY(libtpc.so)
R__LOAD_LIBRARY(libmicromegas.so)
R__LOAD_LIBRARY(libTrackingDiagnostics.so)
R__LOAD_LIBRARY(libtrackingqa.so)
void Fun4All_TrkrClusteringSeeding(
    const int nEvents = 2,
    const std::string filename = "cosmics-00025926-0005.root",
    const std::string outfilename = "cosmics",
    const std::string dir = "/sphenix/lustre01/sphnxpro/commissioning/aligned_streaming_all_4/")
{
  std::string inputRawHitFile = dir + filename;
  auto pos = filename.find("0002");
  std::string runnumber_str = filename.substr(pos, pos + 8);
  int runnumber = std::stoi(runnumber_str);

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
  G4MAGNET::magfield_rescale = 1;
  ACTSGEOM::ActsGeomInit();

  auto hitsin = new Fun4AllDstInputManager("InputManager");
  hitsin->fileopen(inputRawHitFile);
  se->registerInputManager(hitsin);

  Mvtx_HitUnpacking();
  Intt_HitUnpacking();
  Tpc_HitUnpacking();
  Micromegas_HitUnpacking();
  Mvtx_Clustering();
  Intt_Clustering();

  auto tpcclusterizer = new TpcClusterizer;
  tpcclusterizer->Verbosity(0);
  tpcclusterizer->set_do_hit_association(G4TPC::DO_HIT_ASSOCIATION);
  tpcclusterizer->set_rawdata_reco();
  se->registerSubsystem(tpcclusterizer);

  Micromegas_Clustering();

  Tracking_Reco_TrackSeed();
  convert_seeds();

  TString ntupoutfile = outfilename + filename + "_ntup.root";
  std::string ntupstring(ntupoutfile.Data());

  auto ntp = new TrkrNtuplizer("TrkrNtuplizer", ntupstring);
  ntp->do_hit_eval(true);
  ntp->do_cluster_eval(true);
  ntp->do_track_eval(true);
  ntp->do_siseed_eval(true);
  ntp->do_tpcseed_eval(true);
  ntp->do_clus_trk_eval(true);
  ntp->do_vertex_eval(false);
  ntp->set_trkclus_seed_container("SvtxTrackSeedContainer");
  ntp->Verbosity(0);
  ntp->do_info_eval(false);
  se->registerSubsystem(ntp);

  TString residoutfile = outfilename + filename + "_resid.root";
  std::string residstring(residoutfile.Data());

  auto resid = new TrackResiduals("TrackResiduals");
  resid->outfileName(residstring);
  resid->alignment(false);
  resid->clusterTree();
  resid->hitTree();
  resid->zeroField();
  se->registerSubsystem(resid);

  // Fun4AllOutputManager *out = new Fun4AllDstOutputManager("out", "/sphenix/tg/tg01/hf/jdosbo/tracking_development/onlineoffline/hitsets.root");
  // se->registerOutputManager(out);

  se->run(nEvents);
  se->End();
  se->PrintTimer();

  if (Enable::QA)
  {
    se->registerSubsystem(new MvtxClusterQA);
    se->registerSubsystem(new InttClusterQA);
    se->registerSubsystem(new TpcClusterQA);
    se->registerSubsystem(new MicromegasClusterQA);

    TString qaname = outfilename + filename + "_qa.root";
    std::string qaOutputFileName(qaname.Data());
    QAHistManagerDef::saveQARootFile(qaOutputFileName);
  }

  delete se;
  std::cout << "Finished" << std::endl;
  gSystem->Exit(0);
}
