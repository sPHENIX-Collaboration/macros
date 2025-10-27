/*
 * This macro shows a minimum working example of running the tracking
 * hit unpackers with some basic seeding algorithms to try to put together
 * tracks. There are some analysis modules run at the end which package
 * hits, clusters, and clusters on tracks into trees for analysis.
 */

#include <fun4all/Fun4AllUtils.h>
#include <GlobalVariables.C>
#include <G4_ActsGeom.C>
//#include <G4_Global.C>
//#include <G4_Magnet.C>
//#include <G4_Mbd.C>
#include <Trkr_Clustering.C>
#include <Trkr_LaserClustering.C>
#include <Trkr_RecoInit.C>
#include <Trkr_TpcReadoutInit.C>

#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllRunNodeInputManager.h>
#include <fun4all/Fun4AllServer.h>

#include <ffamodules/FlagHandler.h>
#include <ffamodules/CDBInterface.h>

#include <phool/recoConsts.h>

#include <tpc/LaserEventIdentifier.h>

#include <stdio.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libtpc.so)
void Fun4All_LaserClustering(
    const int nEvents = 10,
    const std::string filelist = "/sphenix/lustre01/sphnxpro/commissioning/slurp/tpcbeam/run_00041900_00042000/",
    const std::string outdir = "/sphenig/tg/tg01/jets/bkimelman/",
    const std::string prod = "ana441_2024p007",
    const std::string type = "run2pp")

{
  gSystem->Load("libg4dst.so");

  
  auto se = Fun4AllServer::instance();
  se->Verbosity(1);
  auto rc = recoConsts::instance();
  CDBInterface::instance()->Verbosity(1);
  Enable::CDB = true;
  
  rc->set_StringFlag("CDB_GLOBALTAG", "newcdbtag");

  FlagHandler *flag = new FlagHandler();
  se->registerSubsystem(flag);


  std::ifstream ifs(filelist);
  std::string filepath;


  int i=0;
  //int nTpcFiles = 0;
  int runnumber = 0;
  int segment = 0;
  bool process_endpoints = false;
  while(std::getline(ifs,filepath))
  {
    std::cout << "Adding DST with filepath: " << filepath << std::endl;
    if(i==0)
    {
      std::pair<int, int> runseg = Fun4AllUtils::GetRunSegment(filepath);
      runnumber = runseg.first;
      segment = runseg.second;
      rc->set_IntFlag("RUNNUMBER",runnumber);
      rc->set_uint64Flag("TIMESTAMP",runnumber);
    }

    if(filepath.find("ebdc") != std::string::npos)
    {
      //if(filepath.find("ebdc39") == std::string::npos)
      if(filepath.find("_0_") != std::string::npos or filepath.find("_1_") != std::string::npos)
      {
	//nTpcFiles++;
	process_endpoints = true;
      }
    }
    
    std::string inputname = "InputManager" + std::to_string(i);
    auto hitsin = new Fun4AllDstInputManager(inputname);
    hitsin->fileopen(filepath);
    se->registerInputManager(hitsin);
    i++;
  }

  TRACKING::pp_mode = false;
  TRACKING::tpc_zero_supp = true;
  G4TPC::ENABLE_CENTRAL_MEMBRANE_CLUSTERING = true;
  Enable::MVTX_APPLYMISALIGNMENT = true;
  ACTSGEOM::mvtx_applymisalignment = Enable::MVTX_APPLYMISALIGNMENT;
  
  
  TString out_DST = Form("%s/DST_LASER_CLUSTER_%s_%s-%08d-%05d.root",outdir.c_str(), type.c_str(), prod.c_str(), runnumber, segment);
  std::string theOutDST = out_DST.Data();
  
  std::string geofile = CDBInterface::instance()->getUrl("Tracking_Geometry");
  Fun4AllRunNodeInputManager *ingeo = new Fun4AllRunNodeInputManager("GeoIn");
  ingeo->AddFile(geofile);
  se->registerInputManager(ingeo);

  TpcReadoutInit( runnumber );

  
  //  G4TPC::tpc_drift_velocity_reco = (8.0 / 1000) * 107.0 / 105.0;
  G4TPC::ENABLE_MODULE_EDGE_CORRECTIONS = true;
  //to turn on the default static corrections, enable the two lines below
  G4TPC::ENABLE_STATIC_CORRECTIONS = true;
  G4TPC::USE_PHI_AS_RAD_STATIC_CORRECTIONS=false;


  //TRACKING::reco_tpc_maxtime_sample = 1023;
  TRACKING::tpc_zero_supp = true;

  

  TrackingInit();


  ostringstream ebdcname;
  for(int ebdc = 0; ebdc < 24; ebdc++)
  {
    if(!process_endpoints)
    {
      ebdcname.str("");
      if(ebdc < 10)
      {
	ebdcname<<"0";
      }
      ebdcname<<ebdc;
      Tpc_HitUnpacking(ebdcname.str());
    }
    else if(process_endpoints)
    {
      for(int endpoint = 0; endpoint < 2; endpoint++){
	ebdcname.str("");
	if(ebdc < 10)
	{
	  ebdcname<<"0";
	}
	ebdcname << ebdc << "_" << endpoint;
	Tpc_HitUnpacking(ebdcname.str());
      }
    }
    else
    {
      std::cout << "Wrong number of tpc files input! Exiting now." << std::endl;
      gSystem->Exit(1);
    }
	
  }


  G4TPC::laser_event_debug_filename = Form("%s/laserEventQA-%08d-%05d.root",outdir.c_str(),runnumber,segment);

  Tpc_LaserEventIdentifying();

  G4TPC::ENABLE_CENTRAL_MEMBRANE_CLUSTERING = true; 
  G4TPC::laser_adc_threshold = 100;
  G4TPC::LaserClusteringSequential = true;
  G4TPC::laserClusterFitting = false;
  
  TPC_LaserClustering();
  
  
  Fun4AllOutputManager *out = new Fun4AllDstOutputManager("out",theOutDST);
  // Don't put the matching results on the node tree as it's complicated to run over again without more changes (only for debugging purposes)
  //out->StripNode("CM_FLASH_DIFFERENCES");
  out->AddNode("Sync");
  out->AddNode("EventHeader");
  out->AddNode("GL1RAWHIT");
  out->AddNode("LaserEventInfo");
  out->AddNode("LASER_CLUSTER");
  se->registerOutputManager(out);
  se->run(nEvents);
  //se->run(-1);
  se->End();
  se->PrintTimer();

  CDBInterface::instance()->Print();

  
  delete se;
  std::cout << "Finished" << std::endl;
  gSystem->Exit(0);
}
