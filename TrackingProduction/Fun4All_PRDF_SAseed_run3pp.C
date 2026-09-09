/*
 * This macro shows a minimum working example of running the
 * TPC track reconstruction starting with hit unpackers
 */

// leave the GlobalVariables.C at the beginning, an empty line afterwards
// protects its position against reshuffling by clang-format
#include <GlobalVariables.C>

#include <G4Setup_sPHENIX.C>
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

#include <mvtxrawhitqa/MvtxRawHitQA.h>

#include <inttrawhitqa/InttRawHitQA.h>

#include <tpcqa/TpcRawHitQA.h>

#include <trackingqa/InttClusterQA.h>
#include <trackingqa/MicromegasClusterQA.h>
#include <trackingqa/MvtxClusterQA.h>
#include <trackingqa/SiliconSeedsQA.h>
#include <trackingqa/TpcClusterQA.h>
#include <trackingqa/TpcSeedsQA.h>
#include <trackingqa/TpcSiliconQA.h>
#include <trackingqa/TrackFittingQA.h>

#include <ffamodules/CDBInterface.h>
#include <ffamodules/FlagHandler.h>
#include <ffamodules/HeadReco.h>
#include <ffamodules/SyncReco.h>

#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllRunNodeInputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllUtils.h>

#include <fun4all/Fun4AllReturnCodes.h>
#include <fun4all/SubsysReco.h>

#include <phool/recoConsts.h>

#include <tpctrackreco/TpcCrossingFinder.h>
#include <tpctrackreco/TpcPolyClusterTrkrClusterConverter.h>
#include <tpctrackreco/TpcPolyTrackSeedConverter.h>
#include <tpctrackreco/Tpc_AssembledTrackReco.h>
#include <tpctrackreco/Tpc_ModuleTrackReco.h>
#include <tpctrackreco/Tpc_PolyClusterizer.h>
#include <tpctrackreco/Tpc_PolyTrackReco.h>
#include <tpctrackreco/Tpc_PolyTrackVertexer.h>

#include <fun4allraw/Fun4AllStreamingInputManager.h>
#include <fun4allraw/InputManagerType.h>
#include <fun4allraw/SingleGl1PoolInput.h>
#include <fun4allraw/SingleInttPoolInput.h>
#include <fun4allraw/SingleMicromegasPoolInput.h>
#include <fun4allraw/SingleMvtxPoolInput.h>
#include <fun4allraw/SingleTpcTimeFrameInput.h>

#include <trackingdiagnostics/Tpc_AssembledTrackDisplay.h>
#include <trackingdiagnostics/Tpc_ModuleTrackDisplay.h>
#include <trackingdiagnostics/Tpc_PolyClusterDisplay.h>
#include <trackingdiagnostics/Tpc_PolyClusterResiduals.h>
#include <trackingdiagnostics/TrackResiduals.h>

#include <format>
R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libphool.so)
R__LOAD_LIBRARY(libcdbobjects.so)
R__LOAD_LIBRARY(libmvtx.so)
R__LOAD_LIBRARY(libintt.so)
R__LOAD_LIBRARY(libtpc.so)
R__LOAD_LIBRARY(libmicromegas.so)
R__LOAD_LIBRARY(libPHGarfield.so)
R__LOAD_LIBRARY(libtpctrackreco.so)
R__LOAD_LIBRARY(libTrackingDiagnostics.so)
R__LOAD_LIBRARY(libtrackingqa.so)
R__LOAD_LIBRARY(libtrack_reco.so)


bool isGood(const std::string &infile);
int getrunnumber(const std::string &listfile);


void Fun4All_PRDF_SAseed_run3pp(
    const int nEvents = 0,
    const int /*runnumber_unused*/ = 79513,
    const std::string & /*dir*/ = "/.",
    const std::string &outfilename = "output_tracks",
    const std::string &input_gl1file = "gl1daq.list",
    const std::string &input_inttfile00 = "intt0.list",
    const std::string &input_inttfile01 = "intt1.list",
    const std::string &input_inttfile02 = "intt2.list",
    const std::string &input_inttfile03 = "intt3.list",
    const std::string &input_inttfile04 = "intt4.list",
    const std::string &input_inttfile05 = "intt5.list",
    const std::string &input_inttfile06 = "intt6.list",
    const std::string &input_inttfile07 = "intt7.list",
    const std::string &input_mvtxfile00 = "mvtx0.list",
    const std::string &input_mvtxfile01 = "mvtx1.list",
    const std::string &input_mvtxfile02 = "mvtx2.list",
    const std::string &input_mvtxfile03 = "mvtx3.list",
    const std::string &input_mvtxfile04 = "mvtx4.list",
    const std::string &input_mvtxfile05 = "mvtx5.list",
    const std::string &input_tpcfile00 = "tpc00_0.list",
    const std::string &input_tpcfile01 = "tpc01_0.list",
    const std::string &input_tpcfile02 = "tpc02_0.list",
    const std::string &input_tpcfile03 = "tpc03_0.list",
    const std::string &input_tpcfile04 = "tpc04_0.list",
    const std::string &input_tpcfile05 = "tpc05_0.list",
    const std::string &input_tpcfile06 = "tpc06_0.list",
    const std::string &input_tpcfile07 = "tpc07_0.list",
    const std::string &input_tpcfile08 = "tpc08_0.list",
    const std::string &input_tpcfile09 = "tpc09_0.list",
    const std::string &input_tpcfile10 = "tpc10_0.list",
    const std::string &input_tpcfile11 = "tpc11_0.list",
    const std::string &input_tpcfile12 = "tpc12_0.list",
    const std::string &input_tpcfile13 = "tpc13_0.list",
    const std::string &input_tpcfile14 = "tpc14_0.list",
    const std::string &input_tpcfile15 = "tpc15_0.list",
    const std::string &input_tpcfile16 = "tpc16_0.list",
    const std::string &input_tpcfile17 = "tpc17_0.list",
    const std::string &input_tpcfile18 = "tpc18_0.list",
    const std::string &input_tpcfile19 = "tpc19_0.list",
    const std::string &input_tpcfile20 = "tpc20_0.list",
    const std::string &input_tpcfile21 = "tpc21_0.list",
    const std::string &input_tpcfile22 = "tpc22_0.list",
    const std::string &input_tpcfile23 = "tpc23_0.list",
    const std::string &input_tpcfile001 = "tpc00_1.list",
    const std::string &input_tpcfile011 = "tpc01_1.list",
    const std::string &input_tpcfile021 = "tpc02_1.list",
    const std::string &input_tpcfile031 = "tpc03_1.list",
    const std::string &input_tpcfile041 = "tpc04_1.list",
    const std::string &input_tpcfile051 = "tpc05_1.list",
    const std::string &input_tpcfile061 = "tpc06_1.list",
    const std::string &input_tpcfile071 = "tpc07_1.list",
    const std::string &input_tpcfile081 = "tpc08_1.list",
    const std::string &input_tpcfile091 = "tpc09_1.list",
    const std::string &input_tpcfile101 = "tpc10_1.list",
    const std::string &input_tpcfile111 = "tpc11_1.list",
    const std::string &input_tpcfile121 = "tpc12_1.list",
    const std::string &input_tpcfile131 = "tpc13_1.list",
    const std::string &input_tpcfile141 = "tpc14_1.list",
    const std::string &input_tpcfile151 = "tpc15_1.list",
    const std::string &input_tpcfile161 = "tpc16_1.list",
    const std::string &input_tpcfile171 = "tpc17_1.list",
    const std::string &input_tpcfile181 = "tpc18_1.list",
    const std::string &input_tpcfile191 = "tpc19_1.list",
    const std::string &input_tpcfile201 = "tpc20_1.list",
    const std::string &input_tpcfile211 = "tpc21_1.list",
    const std::string &input_tpcfile221 = "tpc22_1.list",
    const std::string &input_tpcfile231 = "tpc23_1.list",
    const std::string &input_tpotfile = "tpot.list")
{

  std::vector<std::string> gl1_infile;
  gl1_infile.push_back(input_gl1file);

  // MVTX
  std::vector<std::string> mvtx_infile;
  mvtx_infile.push_back(input_mvtxfile00);
  mvtx_infile.push_back(input_mvtxfile01);
  mvtx_infile.push_back(input_mvtxfile02);
  mvtx_infile.push_back(input_mvtxfile03);
  mvtx_infile.push_back(input_mvtxfile04);
  mvtx_infile.push_back(input_mvtxfile05);

  // INTT
  std::vector<std::string> intt_infile;
  intt_infile.push_back(input_inttfile00);
  intt_infile.push_back(input_inttfile01);
  intt_infile.push_back(input_inttfile02);
  intt_infile.push_back(input_inttfile03);
  intt_infile.push_back(input_inttfile04);
  intt_infile.push_back(input_inttfile05);
  intt_infile.push_back(input_inttfile06);
  intt_infile.push_back(input_inttfile07);

  std::vector<std::string> tpc_infile;
  tpc_infile.push_back(input_tpcfile00);
  tpc_infile.push_back(input_tpcfile01);
  tpc_infile.push_back(input_tpcfile02);
  tpc_infile.push_back(input_tpcfile03);
  tpc_infile.push_back(input_tpcfile04);
  tpc_infile.push_back(input_tpcfile05);
  tpc_infile.push_back(input_tpcfile06);
  tpc_infile.push_back(input_tpcfile07);
  tpc_infile.push_back(input_tpcfile08);
  tpc_infile.push_back(input_tpcfile09);
  tpc_infile.push_back(input_tpcfile10);
  tpc_infile.push_back(input_tpcfile11);
  tpc_infile.push_back(input_tpcfile12);
  tpc_infile.push_back(input_tpcfile13);
  tpc_infile.push_back(input_tpcfile14);
  tpc_infile.push_back(input_tpcfile15);
  tpc_infile.push_back(input_tpcfile16);
  tpc_infile.push_back(input_tpcfile17);
  tpc_infile.push_back(input_tpcfile18);
  tpc_infile.push_back(input_tpcfile19);
  tpc_infile.push_back(input_tpcfile20);
  tpc_infile.push_back(input_tpcfile21);
  tpc_infile.push_back(input_tpcfile22);
  tpc_infile.push_back(input_tpcfile23);
  tpc_infile.push_back(input_tpcfile001);
  tpc_infile.push_back(input_tpcfile011);
  tpc_infile.push_back(input_tpcfile021);
  tpc_infile.push_back(input_tpcfile031);
  tpc_infile.push_back(input_tpcfile041);
  tpc_infile.push_back(input_tpcfile051);
  tpc_infile.push_back(input_tpcfile061);
  tpc_infile.push_back(input_tpcfile071);
  tpc_infile.push_back(input_tpcfile081);
  tpc_infile.push_back(input_tpcfile091);
  tpc_infile.push_back(input_tpcfile101);
  tpc_infile.push_back(input_tpcfile111);
  tpc_infile.push_back(input_tpcfile121);
  tpc_infile.push_back(input_tpcfile131);
  tpc_infile.push_back(input_tpcfile141);
  tpc_infile.push_back(input_tpcfile151);
  tpc_infile.push_back(input_tpcfile161);
  tpc_infile.push_back(input_tpcfile171);
  tpc_infile.push_back(input_tpcfile181);
  tpc_infile.push_back(input_tpcfile191);
  tpc_infile.push_back(input_tpcfile201);
  tpc_infile.push_back(input_tpcfile211);
  tpc_infile.push_back(input_tpcfile221);
  tpc_infile.push_back(input_tpcfile231);

  // TPOT
  std::vector<std::string> tpot_infile;
  tpot_infile.push_back(input_tpotfile);

  int runnumber = -99999;
  if (!gl1_infile.empty())
  {
    runnumber = getrunnumber(gl1_infile[0]);
  }
  else if (!mvtx_infile.empty())
  {
    runnumber = getrunnumber(mvtx_infile[0]);
  }
  else if (!intt_infile.empty())
  {
    runnumber = getrunnumber(intt_infile[0]);
  }
  else if (!tpc_infile.empty())
  {
    runnumber = getrunnumber(tpc_infile[0]);
  }
  else if (!tpot_infile.empty())
  {
    runnumber = getrunnumber(tpot_infile[0]);
  }
  if (runnumber == -99999)
  {
    std::cout << "could not extract run number from input files (all lists empty?)"
              << std::endl;
    gSystem->Exit(1);
  }


  
  auto *se = Fun4AllServer::instance();
  se->Verbosity(1);
  auto *rc = recoConsts::instance();

  Enable::QA = true;
  TRACKING::tpc_zero_supp = true;
  TRACKING::streaming_mode = true;
  G4TPC::REJECT_LASER_EVENTS = true;
  G4TPC::sampa_tzero_bias = 0;

  // First order corrections will be applied from PHGarfield
  G4TPC::ENABLE_MODULE_EDGE_CORRECTIONS = false;
  G4TPC::ENABLE_STATIC_CORRECTIONS = false;
  G4TPC::ENABLE_AVERAGE_CORRECTIONS = false;



  rc->set_IntFlag("RUNNUMBER", runnumber);
  rc->set_IntFlag("RUNSEGMENT", 0);

  Enable::CDB = true;
  rc->set_StringFlag("CDB_GLOBALTAG", "newcdbtag");
  rc->set_uint64Flag("TIMESTAMP", runnumber);


  G4MAGNET::magfield_rescale = 1;

  TpcReadoutInit(runnumber);
  std::cout << " run: " << runnumber
            << " samples: " << TRACKING::reco_tpc_maxtime_sample
            << " pre: " << TRACKING::reco_tpc_time_presample
            << " vdrift: " << G4TPC::tpc_drift_velocity_reco
            << std::endl;


  Enable::MVTX = true;
  Enable::INTT = true;
  Enable::TPC = true;
  Enable::MICROMEGAS = true;

  G4Init();
  G4Setup();

  TrackingInit();

  SyncReco *sync = new SyncReco();
  se->registerSubsystem(sync);

  HeadReco *head = new HeadReco();
  se->registerSubsystem(head);

  FlagHandler *flag = new FlagHandler();
  se->registerSubsystem(flag);
  
  int i = 0;
  int NumInputs = 0;
  Fun4AllStreamingInputManager *in = new Fun4AllStreamingInputManager("Comb");

  for (const auto &iter : gl1_infile)
  {
    if (isGood(iter))
    {
      SingleGl1PoolInput *gl1_sngl = new SingleGl1PoolInput("GL1_" + std::to_string(i));
      //    gl1_sngl->Verbosity(3);
      gl1_sngl->AddListFile(iter);
      in->registerStreamingInput(gl1_sngl, InputManagerType::GL1);
      i++;
    }
  }

  i=0;
  
  for (const auto &iter : intt_infile)
  {
    if (isGood(iter))
    {
      SingleInttPoolInput *intt_sngl = new SingleInttPoolInput("INTT_" + std::to_string(i));
      // intt_sngl->Verbosity(3);

      /// find the ebdc number from the filename
      std::string filepath;
      std::string felix;
      std::ifstream ifs(iter);
      while (std::getline(ifs, filepath))
      {
        auto pos = filepath.find("intt");
        felix = filepath.substr(pos + 4, 1);
        break;
      }
      intt_sngl->setHitContainerName("INTTRAWHIT_" + felix);
      intt_sngl->AddListFile(iter);
      in->registerStreamingInput(intt_sngl, InputManagerType::INTT);
      i++;
      NumInputs++;
    }
  }
  i = 0;
  for (const auto &iter : mvtx_infile)
  {
    if (isGood(iter))
    {
      /// find the ebdc number from the filename
      std::string filepath;
      std::string felix;
      std::ifstream ifs(iter);
      while (std::getline(ifs, filepath))
      {
        auto pos = filepath.find("mvtx");
        felix = filepath.substr(pos + 4, 1);
        break;
      }

      SingleMvtxPoolInput *mvtx_sngl = new SingleMvtxPoolInput("MVTX_" + std::to_string(i));
      //    mvtx_sngl->Verbosity(5);
      mvtx_sngl->setHitContainerName("MVTXRAWHIT_" + felix);
      mvtx_sngl->setRawEventHeaderName("MVTXRAWEVTHEADER_" + felix);
      mvtx_sngl->AddListFile(iter);
      in->registerStreamingInput(mvtx_sngl, InputManagerType::MVTX);
      i++;
      NumInputs++;
    }
  }
  i = 0;
  for (const auto &iter : tpc_infile)
  {
    if (isGood(iter))
    {
      /// find the ebdc number from the filename
      std::string filepath;
      std::string ebdc;
      std::ifstream ifs(iter);
      while (std::getline(ifs, filepath))
      {
        auto pos = filepath.find("ebdc");
        ebdc = filepath.substr(pos + 4, 4);
        break;
      }

      SingleTpcTimeFrameInput *tpc_sngl = new SingleTpcTimeFrameInput("TPC_" + std::to_string(i));
      //    tpc_sngl->Verbosity(2);
      //   tpc_sngl->DryRun();
      tpc_sngl->setHitContainerName("TPCRAWHIT_" + ebdc);
      tpc_sngl->AddListFile(iter);
      in->registerStreamingInput(tpc_sngl, InputManagerType::TPC);
      i++;
      NumInputs++;
    }
  }
  i = 0;

  for (const auto &iter : tpot_infile)
  {
    if (isGood(iter))
    {
      SingleMicromegasPoolInput *mm_sngl = new SingleMicromegasPoolInput("MICROMEGAS_" + std::to_string(i));
      //   sngl->Verbosity(3);
      mm_sngl->SetBcoRange(10);
      mm_sngl->SetNegativeBco(2);
      mm_sngl->SetBcoPoolSize(150);
      mm_sngl->AddListFile(iter);
      in->registerStreamingInput(mm_sngl, InputManagerType::MICROMEGAS);
      i++;
      NumInputs++;
    }
  }

  // if there is no input manager this macro will still run - so just quit here
  if (NumInputs == 0)
  {
    std::cout << "no file lists no input manager registered, quitting" << std::endl;
    gSystem->Exit(1);
  }
  se->registerInputManager(in);


  for (int felix = 0; felix < 6; felix++)
  {
    Mvtx_HitUnpacking(std::to_string(felix));
  }
  for (int server = 0; server < 8; server++)
  {
    Intt_HitUnpacking(std::to_string(server));
  }
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
  Micromegas_HitUnpacking();

  //==============================================================

  Mvtx_Clustering();
  Intt_Clustering();
  Micromegas_Clustering();

  //==============================================================

  Tpc_LaserEventIdentifying();
  Reject_Laser_Events();

  //==============================================================

  Tracking_Reco_SiliconSeed_run2pp();
  auto *converter = new TrackSeedTrackMapConverter("SiliconSeedToSvtxTrackMap");
  converter->setTrackSeedName("SiliconTrackSeedContainer");
  converter->setTrackMapName("SiliconSvtxTrackMap");
  converter->setClusterMapName("TRKR_CLUSTER");
  se->registerSubsystem(converter);

  auto *finder_svx = new PHSimpleVertexFinder("SiliconVertexFinder");
  finder_svx->Verbosity(0);
  finder_svx->setDcaCut(0.1);
  finder_svx->setTrackPtCut(0.2);
  finder_svx->setBeamLineCut(1);
  finder_svx->setTrackQualityCut(500);
  finder_svx->setNmvtxRequired(3);
  finder_svx->setOutlierPairCut(0.1);
  finder_svx->setTrackMapName("SiliconSvtxTrackMap");
  finder_svx->setVertexMapName("SiliconSvtxVertexMap");
  se->registerSubsystem(finder_svx);

  //==============================================================

  se->registerSubsystem(new Tpc_ModuleTrackReco());     // makes TPC_MODULETRACKS
  se->registerSubsystem(new Tpc_AssembledTrackReco());  // makes TPC_ASSEMBLEDTRACKS

  auto *crossingFinder = new TpcCrossingFinder();
  crossingFinder->Verbosity(0);
  crossingFinder->setInputNodeName("TPC_ASSEMBLEDTRACKS");
  crossingFinder->setOutputNodeName("TPC_CROSSING_DECISIONS");
  crossingFinder->setVertexMapNodeName("SiliconSvtxVertexMap");  // optional, configurable
  se->registerSubsystem(crossingFinder);

  auto *cluster = new Tpc_PolyClusterizer();  // makes TPC_POLYCLUSTERS
  cluster->setUseSurveyGeometry(false);
  cluster->setKEffSide0(1.00);  // OO 82626 - 4.5, AuAu 6x6 76905 -0, pp 79513 - 1.0, 75391 5.8 75405 4.8
  cluster->setKEffSide1(1.60);  // OO 82626 - 5.0, AuAu 6x6 76905 -0, pp 79513 - 1.6, 75391 5.6 75408 4.8
  se->registerSubsystem(cluster);

  se->registerSubsystem(new Tpc_PolyTrackReco());      // makes TPC_POLYTRACKS
  se->registerSubsystem(new Tpc_PolyTrackVertexer());  // makes TPC_POLYTRACKVERTICES

  se->registerSubsystem(new TpcPolyTrackSeedConverter());           // converts TPC_POLYTRACKS to TpcTrackSeed
  se->registerSubsystem(new TpcPolyClusterTrkrClusterConverter());  // converts TPC_POLYCLUSTERS to TRKR_CLUSTER

  Tracking_Reco_TrackMatching_run2pp();

  
  Tracking_Reco_TrackFit_run2pp();
  // vertexing and propagation to vertex
  Tracking_Reco_Vertex_run2pp();

  

  if (Enable::QA)
  {
    se->registerSubsystem(new MvtxRawHitQA);
    se->registerSubsystem(new InttRawHitQA);
    se->registerSubsystem(new TpcRawHitQA);
    se->registerSubsystem(new MvtxClusterQA);
    se->registerSubsystem(new InttClusterQA);
    se->registerSubsystem(new TpcClusterQA);
    se->registerSubsystem(new MicromegasClusterQA);

    auto *siliconqa = new SiliconSeedsQA;
    siliconqa->setTrackMapName("SiliconSvtxTrackMap");
    siliconqa->setVertexMapName("SiliconSvtxVertexMap");
    se->registerSubsystem(siliconqa);

    auto *convertertpc = new TrackSeedTrackMapConverter("TpcSeedConverter");
    // Default set to full SvtxTrackSeeds. Can be set to
    // SiliconTrackSeedContainer or TpcTrackSeedContainer
    convertertpc->setTrackSeedName("TpcTrackSeedContainer");
    convertertpc->setTrackMapName("TpcSvtxTrackMap");
    convertertpc->setFieldMap(G4MAGNET::magfield_tracking);
    convertertpc->Verbosity(0);
    se->registerSubsystem(convertertpc);

    auto *findertpc = new PHSimpleVertexFinder("TpcSimpleVertexFinder");
    findertpc->Verbosity(0);
    findertpc->setDcaCut(1);
    findertpc->setTrackPtCut(0.2);
    findertpc->setBeamLineCut(1.5);
    findertpc->setTrackQualityCut(1000000000);
    //findertpc->setNmvtxRequired(3);
    findertpc->setRequireMVTX(false);
    findertpc->setOutlierPairCut(0.1);
    findertpc->set_pp_mode(false);
    findertpc->setTrackMapName("TpcSvtxTrackMap");
    findertpc->setVertexMapName("TpcSvtxVertexMap");
    se->registerSubsystem(findertpc);

    auto *tpcqa = new TpcSeedsQA;
    tpcqa->setTrackMapName("TpcSvtxTrackMap");
    tpcqa->setVertexMapName("TpcSvtxVertexMap");
    tpcqa->setSegment(rc->get_IntFlag("RUNSEGMENT"));
    se->registerSubsystem(tpcqa);


    se->registerSubsystem(new TpcSiliconQA);
    se->registerSubsystem(new TrackFittingQA);
  }

  se->run(nEvents);
  se->End();
  se->PrintTimer();

  if (Enable::QA)
  {
    std::string qaOutputFileName = outfilename + std::to_string(runnumber) + "_qa.root";
    QAHistManagerDef::saveQARootFile(qaOutputFileName);
  }
 
  CDBInterface::instance()->Print();
  delete se;
  std::cout << "Finished" << std::endl;
  gSystem->Exit(0);
}

bool isGood(const std::string &infile)
{
  std::ifstream intest;
  intest.open(infile);
  bool goodfile = false;
  if (intest.is_open())
  {
    if (intest.peek() != std::ifstream::traits_type::eof())  // is it non zero?
    {
      goodfile = true;
    }
    intest.close();
  }
  return goodfile;
}

int getrunnumber(const std::string &listfile)
{
  if (!isGood(listfile))
  {
    std::cout << "listfile " << listfile << " is bad" << std::endl;
    gSystem->Exit(1);
  }
  std::ifstream ifs(listfile);
  std::string filepath;
  std::getline(ifs, filepath);

  std::pair<int, int> runseg = Fun4AllUtils::GetRunSegment(filepath);
  int runnumber = runseg.first;
  //  int segment = abs(runseg.second);
  return runnumber;
}
