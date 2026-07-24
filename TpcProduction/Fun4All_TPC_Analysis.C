/*
 * This macro shows a minimum working example of running the tracking
 * hit unpackers with some basic seeding algorithms to try to put together
 * tracks. There are some analysis modules run at the end which package
 * hits, clusters, and clusters on tracks into trees for analysis.
 */

// leave the GlobalVariables.C at the beginning, an empty line afterwards
// protects its position against reshuffling by clang-format
#include <GlobalVariables.C>

#include <G4_ActsGeom.C>
#include <G4_Global.C>
#include <G4_Magnet.C>
#include <G4_Mbd.C>
#include <QA.C>
#include <Trkr_Clustering.C>
#include <Trkr_LaserClustering.C>
#include <Trkr_Reco.C>
#include <Trkr_RecoInit.C>
#include <Trkr_TpcReadoutInit.C>

#include <cdbobjects/CDBTTree.h>

#include <ffamodules/CDBInterface.h>
#include <ffamodules/FlagHandler.h>

#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllRunNodeInputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllUtils.h>

#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4AllReturnCodes.h>

#include <phool/recoConsts.h>

#include <tpctrackreco/Tpc_ModuleTrackReco.h>
#include <tpctrackreco/Tpc_AssembledTrackReco.h>
#include <tpctrackreco/Tpc_PolyTrackReco.h>
#include <tpctrackreco/Tpc_PolyTrackVertexer.h>
#include <tpctrackreco/Tpc_PolyClusterizer.h>

#include <trackingdiagnostics/Tpc_ModuleTrackDisplay.h>
#include <trackingdiagnostics/Tpc_AssembledTrackDisplay.h>
#include <trackingdiagnostics/Tpc_PolyClusterDisplay.h>
#include <trackingdiagnostics/Tpc_PolyClusterResiduals.h>



R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libphool.so)
R__LOAD_LIBRARY(libcdbobjects.so)
R__LOAD_LIBRARY(libmvtx.so)
R__LOAD_LIBRARY(libintt.so)
R__LOAD_LIBRARY(libtpc.so)
R__LOAD_LIBRARY(libmicromegas.so)
R__LOAD_LIBRARY(libtpctrackreco.so)
R__LOAD_LIBRARY(libTrackingDiagnostics.so)



void Fun4All_TPC_Analysis(
    const int nEvents = 2,
    const int runnumber = 79513,
    const int segment = 0,
    const std::string&  /*outdir*/ = ".",
    const std::string& indir = ".",
    const int  /*nSkip*/ = 0,
    const std::string& collision = "run3pp",
    const std::string& production = "ana532_nocdbtag_v001",
    const std::string& outfilename = "HITS_ppFieldOn")
{
  auto *se = Fun4AllServer::instance();
  se->Verbosity(1);
  auto *rc = recoConsts::instance();


  const std::string dsttype = "TPC";
  std::string filename = indir+"/DST_"+dsttype+"_"+collision+"_"+production+"-"+std::to_string(runnumber)+"-"+std::to_string(segment)+".root";

  auto *hitsinclus = new Fun4AllDstInputManager("TpcInputManager");
  hitsinclus->fileopen(filename);
  se->registerInputManager(hitsinclus);

  rc->set_IntFlag("RUNNUMBER", runnumber);
  rc->set_IntFlag("RUNSEGMENT", segment);

  Enable::QA = false;
  Enable::CDB = true;
  rc->set_StringFlag("CDB_GLOBALTAG", "newcdbtag");
  rc->set_uint64Flag("TIMESTAMP", runnumber);



  TRACKING::streaming_mode = true;

  FlagHandler *flag = new FlagHandler();
  se->registerSubsystem(flag);

  std::string geofile = CDBInterface::instance()->getUrl("Tracking_Geometry");

  Fun4AllRunNodeInputManager *ingeo = new Fun4AllRunNodeInputManager("GeoIn");
  ingeo->AddFile(geofile);
  se->registerInputManager(ingeo);



  TpcReadoutInit(runnumber);


  //For the module tracks display uncomment following line
  se->registerSubsystem(new Tpc_ModuleTrackDisplay("Tpc_ModuleTrackDisplay", "tpc_moduletrack_display_" + outfilename + "_" + std::to_string(runnumber) + ".root"));
  
  //For the assembled tracks display uncomment following line
  //se->registerSubsystem(new Tpc_AssembledTrackDisplay("Tpc_AssembledTrackDisplay", "tpc_assembledtrack_display_" + outfilename + "_" + to_string(runnumber) + ".root"));
  
  //For the  cluster and TPC SA tracks display uncomment following line
  //se->registerSubsystem(new Tpc_PolyClusterDisplay("Tpc_PolyClusterDisplay", "tpc_poly_cluster_display_" + outfilename + "_" + to_string(runnumber) + ".root"));
  
  //For the  residual tree output uncomment following block (options to put cuts on minimum pT and minimum number of clusters in TPC SA are available)
  /*auto resid = new Tpc_PolyClusterResiduals("Tpc_PolyClusterResiduals",
                                        outdir + "/tpc_poly_track_residuals"+ outfilename + "_" + to_string(runnumber) + to_string(segment) + ".root" );
  resid->setMinPt(0);
  resid->setMinTpcClusters(20);
  se->registerSubsystem(resid);
  */

  se->run(nEvents);
  se->Print("NODETREE");
  se->End();
  se->PrintTimer();


  CDBInterface::instance()->Print();
  delete se;
  std::cout << "Finished" << std::endl;
  gSystem->Exit(0);
}
