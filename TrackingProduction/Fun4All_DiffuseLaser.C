/*
 * This macro shows a minimum working example of running the tracking
 * hit unpackers with some basic seeding algorithms to try to put together
 * tracks. There are some analysis modules run at the end which package
 * hits, clusters, and clusters on tracks into trees for analysis.
 */

#include <GlobalVariables.C>

#include <G4_ActsGeom.C>
#include <Trkr_Clustering.C>
#include <Trkr_LaserClustering.C>
#include <Trkr_RecoInit.C>
#include <Trkr_TpcReadoutInit.C>

#include <tpc/DiffuseLaserEventSelector.h>
#include <tpc/LaserEventIdentifier.h>

#include <ffamodules/CDBInterface.h>

#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllRunNodeInputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllUtils.h>


#include <phool/recoConsts.h>

#include <format>
#include <fstream>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libtpc.so)
R__LOAD_LIBRARY(libtpccalib.so)



void Fun4All_DiffuseLaser(
    const int nEvents = 100,
    const std::string& filelist = "filelist.list",
    const std::string& outdir = "./",
    const std::string& outfilename = "diffuse_laser")
{

  gSystem->Load("libg4dst.so");
  
  auto *se = Fun4AllServer::instance();
  se->Verbosity(0);
  auto *rc = recoConsts::instance();
  CDBInterface::instance()->Verbosity(1);
  
  rc->set_StringFlag("CDB_GLOBALTAG", "newcdbtag");



  std::ifstream ifs(filelist);
  std::string filepath;

  int i = 0;
  int runnumber = std::numeric_limits<int>::quiet_NaN();
  int segment = std::numeric_limits<int>::quiet_NaN();
  
   while (std::getline(ifs, filepath))
  {
    std::cout << "Adding DST with filepath: " << filepath << std::endl;
    if (i == 0)
    {
      std::pair<int, int>
          runseg = Fun4AllUtils::GetRunSegment(filepath);
      runnumber = runseg.first;
      segment = runseg.second;
      rc->set_IntFlag("RUNNUMBER", runnumber);
      rc->set_uint64Flag("TIMESTAMP", runnumber);
    }
  
    std::string inputname = "InputManager" + std::to_string(i);
    auto *hitsin = new Fun4AllDstInputManager(inputname);
    hitsin->fileopen(filepath);
    se->registerInputManager(hitsin);
    i++;
  }

  TpcReadoutInit(runnumber);

  
  TRACKING::tpc_zero_supp = true;
  Enable::MVTX_APPLYMISALIGNMENT = true;
  ACTSGEOM::mvtx_applymisalignment = Enable::MVTX_APPLYMISALIGNMENT;
  

  std::string geofile = CDBInterface::instance()->getUrl("Tracking_Geometry");
  Fun4AllRunNodeInputManager *ingeo = new Fun4AllRunNodeInputManager("GeoIn");
  ingeo->AddFile(geofile);
  se->registerInputManager(ingeo);

  G4TPC::ENABLE_MODULE_EDGE_CORRECTIONS = false;
  //G4TPC::ENABLE_STATIC_CORRECTIONS = false;
  //G4TPC::USE_PHI_AS_RAD_STATIC_CORRECTIONS=false;


  G4TPC::ENABLE_AVERAGE_CORRECTIONS = false;

  TRACKING::pp_mode = true;
  
  TrackingInit();

  std::ostringstream ebdcname;
  for (int ebdc = 0; ebdc < 24; ebdc++)
  {
    for (int endpoint = 0; endpoint < 2; endpoint++)
    {
        ebdcname.str("");
        if (ebdc < 10)
        {
          ebdcname << "0";
        }
        ebdcname << ebdc << "_" << endpoint;
        Tpc_HitUnpacking(ebdcname.str());
    }
  }

  Tpc_LaserEventIdentifying();

  se->registerSubsystem( new DiffuseLaserEventSelector());


  std::cout<< "Output DST "<<std::format("{}/{}_{}_{}.root",outdir, outfilename, runnumber, segment)  << std::endl;
  Fun4AllOutputManager *out = new Fun4AllDstOutputManager("out", std::format("{}/{}_{}_{}.root",outdir, outfilename, runnumber, segment));
  out->AddEventSelector("DiffuseLaserEventSelector");
  out->AddNode("Sync");
  out->AddNode("EventHeader");
  out->AddNode("TRKR_HITSET");
  se->registerOutputManager(out);

  se->run(nEvents);
  se->End();

  CDBInterface::instance()->Print();
  se->PrintTimer();

  delete se;
  std::cout << "Finished" << std::endl;
  gSystem->Exit(0);
}
