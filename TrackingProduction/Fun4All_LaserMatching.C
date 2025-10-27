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

#include <tpccalib/TpcCentralMembraneMatching.h>
#include <lasermatchingqa/laserMatchingQApp.h>

#include <stdio.h>
R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libtpc.so)
R__LOAD_LIBRARY(libtpccalib.so)
R__LOAD_LIBRARY(libLaserMatchingQA.so)
void Fun4All_LaserMatching(
    const int nEvents = 10,
    const std::string filelist = "",
    const std::string distortionCorrection = "",
    const std::string outdir = "/sphenig/tg/tg01/jets/bkimelman/")
{

  auto se = Fun4AllServer::instance();
  se->Verbosity(2);

  //std::string inputFile = tpcdir + tpcfilename;
  
  //std::pair<int, int>
  //runseg = Fun4AllUtils::GetRunSegment(tpcfilename);
  //int runnumber = runseg.first;
  //int segment = runseg.second;
  
  auto rc = recoConsts::instance();

  Enable::CDB = true;
  rc->set_StringFlag("CDB_GLOBALTAG", "newcdbtag");
  //rc->set_IntFlag("RUNNUMBER",runnumber);
  //rc->set_uint64Flag("TIMESTAMP",runnumber);

  std::ifstream ifs(filelist);
  std::string filepath;


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
    //se->registerInputManager(hitsin);
    i++;
  }


  auto hitsin = new Fun4AllDstInputManager("InputManager");
  hitsin->AddListFile(filelist);
  se->registerInputManager(hitsin);
  
  //auto hitsin = new Fun4AllDstInputManager("InputManager");
  //hitsin->fileopen(inputFile);
  //se->registerInputManager(hitsin);
  

  TRACKING::tpc_zero_supp = true;
  Enable::MVTX_APPLYMISALIGNMENT = true;
  ACTSGEOM::mvtx_applymisalignment = Enable::MVTX_APPLYMISALIGNMENT;
  
  std::string geofile = CDBInterface::instance()->getUrl("Tracking_Geometry");
  Fun4AllRunNodeInputManager *ingeo = new Fun4AllRunNodeInputManager("GeoIn");
  ingeo->AddFile(geofile);
  se->registerInputManager(ingeo);

  
  //  G4TPC::tpc_drift_velocity_reco = (8.0 / 1000) * 107.0 / 105.0;
  G4TPC::ENABLE_MODULE_EDGE_CORRECTIONS = true;
  //to turn on the default static corrections, enable the two lines below

  if(runnumber == 52077 || runnumber == 52078 || (runnumber >= 75103 && runnumber <= 75114) || (runnumber >= 75555 && runnumber <= 75574))
  {

    G4TPC::ENABLE_STATIC_CORRECTIONS = false;  
  }
  else
  {
    G4TPC::ENABLE_STATIC_CORRECTIONS = true;
    G4TPC::USE_PHI_AS_RAD_STATIC_CORRECTIONS=false;
  }

  if (distortionCorrection != "")
  {    
    G4TPC::ENABLE_AVERAGE_CORRECTIONS = true;
    
    G4TPC::average_correction_filename = distortionCorrection.c_str();
    G4TPC::USE_PHI_AS_RAD_AVERAGE_CORRECTIONS=false;
    G4TPC::average_correction_interpolate = false;
  }
  else
  {
    G4TPC::ENABLE_AVERAGE_CORRECTIONS = false;
  }
    
  
  //G4TPC::DISTORTIONS_USE_PHI_AS_RADIANS = false;

  //TRACKING::reco_tpc_maxtime_sample = 1023;

  TrackingInit();


  auto centralMembraneMatcher = new TpcCentralMembraneMatching;
  centralMembraneMatcher->setSavehistograms( true );
  centralMembraneMatcher->Verbosity( 1 );
  centralMembraneMatcher->set_fieldOn(true);
  centralMembraneMatcher->set_doFancy(false);
  centralMembraneMatcher->set_doHadd(true);
  centralMembraneMatcher->set_averageMode(true);
  centralMembraneMatcher->set_event_sequence(segment);
  centralMembraneMatcher->setHistogramOutputfile(Form("%s/PHTpcCentralMembraneMatcher-%08d-all.root",outdir.c_str(),runnumber));
  centralMembraneMatcher->setOutputfile(Form("%s/CMDistortionCorrections-%08d-all.root",outdir.c_str(),runnumber));
  centralMembraneMatcher->setDebugOutputFile(Form("%s/CMMatching-%08d-all.root",outdir.c_str(),runnumber));
  centralMembraneMatcher->set_grid_dimensions(500,500);
  se->registerSubsystem(centralMembraneMatcher);


  auto lmQA = new laserMatchingQApp;
  lmQA->setOutfile(Form("%s/LaserMatchingQA-%08d-all",outdir.c_str(),runnumber));
  lmQA->setRunnumber(runnumber);
  se->registerSubsystem(lmQA);
  
  se->run(nEvents);
  //se->run(-1);
  se->End();
  se->PrintTimer();

  delete se;
  std::cout << "Finished" << std::endl;
  gSystem->Exit(0);
}
