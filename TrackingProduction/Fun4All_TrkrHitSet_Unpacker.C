/*
 * This macro is run in our daily CI and is intended as a minimum working
 * example showing how to unpack the raw hits into the offline tracker hit
 * format. No other reconstruction or analysis is performed
 */
#include <GlobalVariables.C>
#include <Trkr_Clustering.C>
#include <Trkr_TpcReadoutInit.C>

#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllRunNodeInputManager.h>
#include <fun4all/Fun4AllServer.h>

#include <ffamodules/CDBInterface.h>

#include <phool/recoConsts.h>

#include <stdio.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libmvtx.so)
R__LOAD_LIBRARY(libintt.so)
R__LOAD_LIBRARY(libtpc.so)
R__LOAD_LIBRARY(libmicromegas.so)
void Fun4All_TrkrHitSet_Unpacker(
    const int nEvents = 2,
    const int runnumber = 26048,
    const std::string outfilename = "cosmics",
    const std::string dir = "/sphenix/lustre01/sphnxpro/commissioning/aligned_streaming_all_4/",
    const std::string file = "cosmics-")
{
  gSystem->Load("libg4dst.so");
  std::string inputRawHitFile = dir + file;
  char filename[500];
  sprintf(filename, "%s%08d-0000.root", inputRawHitFile.c_str(), runnumber);

  auto se = Fun4AllServer::instance();
  se->Verbosity(1);
  auto rc = recoConsts::instance();
  TpcReadoutInit( runnumber );
  std::cout<< " run: " << runnumber
	   << " samples: " << TRACKING::reco_tpc_maxtime_sample
	   << " pre: " << TRACKING::reco_tpc_time_presample
	   << " vdrift: " << G4TPC::tpc_drift_velocity_reco
	   << std::endl;

  rc->set_IntFlag("RUNNUMBER", runnumber);

  Enable::CDB = true;
  rc->set_StringFlag("CDB_GLOBALTAG", "ProdA_2023");
  rc->set_uint64Flag("TIMESTAMP", 6);

  std::string geofile = CDBInterface::instance()->getUrl("Tracking_Geometry");
  Fun4AllRunNodeInputManager *ingeo = new Fun4AllRunNodeInputManager("GeoIn");
  ingeo->AddFile(geofile);
  se->registerInputManager(ingeo);

  auto hitsin = new Fun4AllDstInputManager("InputManager");
  hitsin->fileopen(filename);
  se->registerInputManager(hitsin);

  Mvtx_HitUnpacking();
  Intt_HitUnpacking();
  Tpc_HitUnpacking();
  Micromegas_HitUnpacking();

  Fun4AllOutputManager *out = new Fun4AllDstOutputManager("DSTOUT", outfilename);
  out->StripNode("MVTXRAWHIT");
  out->StripNode("INTTRAWHIT");
  out->StripNode("MICROMEGASRAWHIT");
  out->StripNode("TPCRAWHIT");
  out->StripNode("GL1RAWHIT");

  se->registerOutputManager(out);

  se->run(nEvents);
  se->End();
  se->PrintTimer();

  delete se;
  std::cout << "Finished" << std::endl;
  gSystem->Exit(0);
}
