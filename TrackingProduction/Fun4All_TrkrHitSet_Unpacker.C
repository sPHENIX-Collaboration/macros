#include <GlobalVariables.C>
#include <G4_Magnet.C>
#include <G4_ActsGeom.C>
#include <Trkr_Clustering.C>
#include <Trkr_Reco_Cosmics.C>

#include <fun4all/Fun4AllRunNodeInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <ffamodules/CDBInterface.h>

#include <phool/recoConsts.h>

#include <mvtx/MvtxCombinedRawDataDecoder.h>
#include <intt/InttCombinedRawDataDecoder.h>
#include <tpc/TpcCombinedRawDataUnpacker.h>
#include <micromegas/MicromegasCombinedDataDecoder.h>

#include <trackingdiagnostics/TrkrNtuplizer.h>

#include <stdio.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libmvtx.so)
R__LOAD_LIBRARY(libintt.so)
R__LOAD_LIBRARY(libtpc.so)
R__LOAD_LIBRARY(libmicromegas.so)
R__LOAD_LIBRARY(libTrackingDiagnostics.so)
void Fun4All_TrkrHitSet_Unpacker(
    const int nEvents = 20,
    const int runnumber = 26048,
    const std::string outfilename = "cosmics.root",
    const std::string dir = "/sphenix/lustre01/sphnxpro/commissioning/aligned_streaming_all/",
    const std::string file = "cosmics-")
{
  std::string inputRawHitFile = dir + file;
  char filename[500];
  sprintf(filename, "%s%08d-0000.root",inputRawHitFile.c_str(),runnumber);
 
  auto se = Fun4AllServer::instance();
  se->Verbosity(2);
  auto rc = recoConsts::instance();
  rc->set_IntFlag("RUNNUMBER",runnumber);
  
  Enable::CDB = true;
  G4MAGNET::magfield = "0.01";
  //G4MAGNET::magfield = string(getenv("CALIBRATIONROOT")) + string("/Field/Map/sphenix3dbigmapxyz_gap_rebuild.root");
  //
  rc->set_StringFlag("CDB_GLOBALTAG","ProdA_2023");
  rc->set_uint64Flag("TIMESTAMP",6);
  string geofile = CDBInterface::instance()->getUrl("Tracking_Geometry");
 
  Fun4AllRunNodeInputManager *ingeo = new Fun4AllRunNodeInputManager("GeoIn");
  ingeo->AddFile(geofile);
  se->registerInputManager(ingeo); 


  auto hitsin = new Fun4AllDstInputManager("InputManager");
  hitsin->fileopen(filename);
  //hitsin->AddListFile("filelist.list");
  se->registerInputManager(hitsin);

  auto mvtxunpacker = new MvtxCombinedRawDataDecoder;
  se->registerSubsystem(mvtxunpacker);

  auto inttunpacker = new InttCombinedRawDataDecoder;
  inttunpacker->Verbosity(1);
  se->registerSubsystem(inttunpacker);

  auto tpcunpacker = new TpcCombinedRawDataUnpacker;
  se->registerSubsystem(tpcunpacker);

  auto tpotunpacker = new MicromegasCombinedDataDecoder;
  tpotunpacker->Verbosity(1);
  se->registerSubsystem(tpotunpacker);

  ACTSGEOM::ActsGeomInit();
  Mvtx_Clustering();
  Intt_Clustering();
  TPC_Clustering();
  Micromegas_Clustering();

  Tracking_Reco_TrackSeed();
  convert_seeds();


  auto ntp = new TrkrNtuplizer("TrkrNtuplizer",outfilename +".root");
  ntp->do_hit_eval(true);
  ntp->do_cluster_eval(true);
  ntp->do_track_eval(true);
  ntp->do_siseed_eval(false);
  ntp->do_tpcseed_eval(true);
  ntp->do_clus_trk_eval(false);
  ntp->do_vertex_eval(false);
  ntp->do_info_eval(false);
  se->registerSubsystem(ntp);


  //Fun4AllOutputManager *out = new Fun4AllDstOutputManager("out", "/sphenix/tg/tg01/hf/jdosbo/tracking_development/onlineoffline/hitsets.root");
  //se->registerOutputManager(out);

  se->run(nEvents);
  se->End();
  delete se;
  std::cout << "Finished"<<std::endl;
  gSystem->Exit(0);

}
