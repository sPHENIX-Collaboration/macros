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
#include <Trkr_RecoInit.C>
#include <Trkr_TpcReadoutInit.C>

#include <ffamodules/CDBInterface.h>

#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllRunNodeInputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <phool/recoConsts.h>

#include <cdbobjects/CDBTTree.h>

#include <trackingqa/InttClusterQA.h>

#include <trackingqa/MicromegasClusterQA.h>

#include <trackingqa/MvtxClusterQA.h>

#include <trackingqa/TpcClusterQA.h>
#include <trackingqa/TpcSeedsQA.h>

#include <trackingdiagnostics/TrackResiduals.h>
#include <trackingdiagnostics/TrkrNtuplizer.h>

#include <fun4all/Fun4AllUtils.h>

#include <stdio.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libphool.so)
R__LOAD_LIBRARY(libcdbobjects.so)
R__LOAD_LIBRARY(libmvtx.so)
R__LOAD_LIBRARY(libintt.so)
R__LOAD_LIBRARY(libtpc.so)
R__LOAD_LIBRARY(libmicromegas.so)
R__LOAD_LIBRARY(libTrackingDiagnostics.so)
R__LOAD_LIBRARY(libtrackingqa.so)
void Fun4All_Cosmics(
    const int nEvents = 0,
    const std::string filename = "DST_STREAMING_EVENT_cosmics_new_2024p001-00045673-0000.root",
    const std::string dir = ".",
    const std::string outfilename = "cosmics")
{
  std::string inputRawHitFile = dir + filename;
  TRACKING::tpc_zero_supp = true;

  std::pair<int, int>
      runseg = Fun4AllUtils::GetRunSegment(inputRawHitFile);
  int runnumber = runseg.first;
  int segment = runseg.second;
  TpcReadoutInit( runnumber );
  std::cout<< " run: " << runnumber
	   << " samples: " << TRACKING::reco_tpc_maxtime_sample
	   << " pre: " << TRACKING::reco_tpc_time_presample
	   << " vdrift: " << G4TPC::tpc_drift_velocity_reco
	   << std::endl;

  auto se = Fun4AllServer::instance();
  se->Verbosity(0);
  auto rc = recoConsts::instance();
  rc->set_IntFlag("RUNNUMBER", runnumber);

  Enable::CDB = true;
  rc->set_StringFlag("CDB_GLOBALTAG", "2024p007");
  rc->set_uint64Flag("TIMESTAMP", runnumber);
  std::string geofile = CDBInterface::instance()->getUrl("Tracking_Geometry");

  Fun4AllRunNodeInputManager *ingeo = new Fun4AllRunNodeInputManager("GeoIn");
  ingeo->AddFile(geofile);
  se->registerInputManager(ingeo);

  G4TPC::tpc_drift_velocity_reco = (8.0 / 1000) * 107.0 / 105.0;
  CDBInterface *cdb = CDBInterface::instance();
  std::string tpc_dv_calib_dir = cdb->getUrl("TPC_DRIFT_VELOCITY");
  if (tpc_dv_calib_dir.empty())
  {
    std::cout << "No calibrated TPC drift velocity for Run " << runnumber << ". Use default value " << G4TPC::tpc_drift_velocity_reco << " cm/ns" << std::endl;
  }
  else
  {
    CDBTTree *cdbttree = new CDBTTree(tpc_dv_calib_dir);
    cdbttree->LoadCalibrations();
    G4TPC::tpc_drift_velocity_reco = cdbttree->GetSingleFloatValue("tpc_drift_velocity");
    std::cout << "Use calibrated TPC drift velocity for Run " << runnumber << ": " << G4TPC::tpc_drift_velocity_reco << " cm/ns" << std::endl;
  }

  // can use for zero field
  //double fieldstrength = 0.01;
  //G4MAGNET::magfield_tracking = "0.01";
  double fieldstrength = std::numeric_limits<double>::quiet_NaN();
  bool ConstField = isConstantField(G4MAGNET::magfield_tracking,fieldstrength);

  if(ConstField && fieldstrength < 0.1)
  {
    G4MAGNET::magfield = "0.01";
    G4MAGNET::magfield_rescale = 1;
  }
  TrackingInit();

  auto hitsin = new Fun4AllDstInputManager("InputManager");
  hitsin->fileopen(inputRawHitFile);
  se->registerInputManager(hitsin);

  Mvtx_HitUnpacking();
  Intt_HitUnpacking();
  //Tpc_HitUnpacking();
  auto tpcunpacker = new TpcCombinedRawDataUnpacker;
  tpcunpacker->Verbosity(0);
  tpcunpacker->doBaselineCorr(true);
  if(TRACKING::tpc_zero_supp)
    {
      tpcunpacker->ReadZeroSuppressedData();
    }
  se->registerSubsystem(tpcunpacker);

  Micromegas_HitUnpacking();
  Mvtx_Clustering();
  Intt_Clustering();

  auto tpcclusterizer = new TpcClusterizer;
  tpcclusterizer->Verbosity(0);
  tpcclusterizer->set_do_hit_association(G4TPC::DO_HIT_ASSOCIATION);
  tpcclusterizer->set_rawdata_reco();
  se->registerSubsystem(tpcclusterizer);

  Tpc_LaserEventIdentifying();
  Micromegas_Clustering();

  Tracking_Reco_TrackSeed();

  TrackSeedTrackMapConverter *converter = new TrackSeedTrackMapConverter();
  // Default set to full SvtxTrackSeeds. Can be set to
  // SiliconTrackSeedContainer or TpcTrackSeedContainer
  converter->setTrackSeedName("SvtxTrackSeedContainer");
  converter->Verbosity(0);
  converter->cosmics();
  converter->setFieldMap(G4MAGNET::magfield_tracking);
  se->registerSubsystem(converter);

  TString residoutfile = outfilename + filename + "_resid.root";
  std::string residstring(residoutfile.Data());

  auto resid = new TrackResiduals("TrackResiduals");
  resid->Verbosity(0);
  resid->outfileName(residstring);
  resid->alignment(false);
  resid->clusterTree();
  resid->hitTree();
  resid->convertSeeds(true);


  if(ConstField && fieldstrength < 0.1)
  {
    resid->zeroField();
  }
  resid->setSegment(segment);
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
    se->registerSubsystem(new TpcSeedsQA);
   
  }
  
  TString qaname = outfilename + filename + "_qa.root";
  std::string qaOutputFileName(qaname.Data());
  //QAHistManagerDef::saveQARootFile(qaOutputFileName);
  delete se;
  std::cout << "Finished" << std::endl;
  gSystem->Exit(0);
}
