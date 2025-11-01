/*
 * This macro shows a minimum working example of running the tracking
 * hit unpackers with some basic seeding algorithms to try to put together
 * tracks. There are some analysis modules run at the end which package
 * hits, clusters, and clusters on tracks into trees for analysis.
 */

#include <fun4all/Fun4AllUtils.h>
#include <GlobalVariables.C>
#include <G4_ActsGeom.C>
#include <G4_Global.C>
#include <G4_Magnet.C>
#include <G4_Mbd.C>
#include <Trkr_Clustering.C>
#include <Trkr_LaserClustering.C>
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

//#include <tpc/LaserEventIdentifier.h>

#include <tpccalib/TpcLaminationFitting.h>

#include <stdio.h>
R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libtpc.so)
R__LOAD_LIBRARY(libtpccalib.so)
void Fun4All_LaminationFitting(
    const int nEvents = 10,
    const std::string filelist = "/sphenix/lustre01/sphnxpro/commissioning/slurp/tpcbeam/run_00041900_00042000/",
    const std::string outdir = "/sphenig/tg/tg01/jets/bkimelman/",
    const std::string prod = "ana441_2024p007",
    const std::string type = "run2pp")

{

  auto se = Fun4AllServer::instance();
  se->Verbosity(2);


  TRACKING::tpc_zero_supp = true;
  G4TPC::ENABLE_CENTRAL_MEMBRANE_CLUSTERING = true;
  Enable::MVTX_APPLYMISALIGNMENT = true;
  ACTSGEOM::mvtx_applymisalignment = Enable::MVTX_APPLYMISALIGNMENT;
  
  std::ifstream ifs(filelist);
  std::string filepath;

  auto rc = recoConsts::instance();

  Enable::CDB = true;
  rc->set_StringFlag("CDB_GLOBALTAG", "newcdbtag");

  int i=0;
  int runnumber = 0;
  int segment = 0;
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

    //std::string inputname = "InputManager" + std::to_string(i);
    //auto hitsin = new Fun4AllDstInputManager(inputname);
    //hitsin->fileopen(filepath);
    //hitsin->AddFile(filepath);
    //se->registerInputManager(hitsin);
    i++;
  }

  auto hitsin = new Fun4AllDstInputManager("InputManager");
  hitsin->AddListFile(filelist);
  se->registerInputManager(hitsin);


  std::string geofile = CDBInterface::instance()->getUrl("Tracking_Geometry");
  Fun4AllRunNodeInputManager *ingeo = new Fun4AllRunNodeInputManager("GeoIn");
  ingeo->AddFile(geofile);
  se->registerInputManager(ingeo);

  //  G4TPC::tpc_drift_velocity_reco = (8.0 / 1000) * 107.0 / 105.0;
  G4TPC::ENABLE_MODULE_EDGE_CORRECTIONS = true;
  //to turn on the default static corrections, enable the two lines below
  G4TPC::ENABLE_STATIC_CORRECTIONS = true;

  if(runnumber == 53098 || (runnumber >= 75103 && runnumber <= 75114) || (runnumber >= 75555 && runnumber <= 75574))
  {
    G4TPC::ENABLE_STATIC_CORRECTIONS = false;
  }
  
  G4TPC::USE_PHI_AS_RAD_STATIC_CORRECTIONS=false;


  G4TPC::ENABLE_AVERAGE_CORRECTIONS = false;
  //G4TPC::average_correction_filename = std::string(Form("/sphenix/tg/tg01/jets/bkimelman/BenProduction/Feb25_2025/Laminations_run2pp_ana466_2024p012_v001-%08d.root",runnumber));
  //G4TPC::USE_PHI_AS_RAD_AVERAGE_CORRECTIONS=false;
  //G4TPC::average_correction_interpolate = false;
  
  //G4TPC::DISTORTIONS_USE_PHI_AS_RADIANS = false;

  //TRACKING::reco_tpc_maxtime_sample = 1023;

  G4TPC::laser_adc_threshold = 100;

  //set to false if you want AuAu or true for pp
  if(type.find("pp") != std::string::npos) TRACKING::pp_mode = true;
  else TRACKING::pp_mode = false;


  
  TrackingInit();


  G4TPC::LaminationOutputName = Form("%s/Laminations_%s_%s-%08d.root",outdir.c_str(),type.c_str(),prod.c_str(),runnumber);
  G4TPC::LaminationQAName = Form("%s/LaminationQA_%s_%s-%08d.pdf",outdir.c_str(),type.c_str(),prod.c_str(),runnumber);
  TPC_LaminationFitting();
  
  se->run(nEvents);
  //se->run(-1);
  se->End();
  se->PrintTimer();

  delete se;
  std::cout << "Finished" << std::endl;
  gSystem->Exit(0);
}
