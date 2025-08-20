/*
 * This macro shows running the full event combining + tracking for 
 * cosmics running.. There are some analysis modules run at the end 
 * which package  hits, clusters, and clusters on tracks into trees 
 * for analysis.
 */

#include <G4_ActsGeom.C>
#include <G4_Magnet.C>
#include <GlobalVariables.C>
#include <QA.C>
#include <Trkr_Clustering.C>
#include <Trkr_Reco.C>
#include <Trkr_RecoInit.C>
#include <Trkr_TpcReadoutInit.C>

#include <intt/InttOdbcQuery.h>
#include <ffamodules/CDBInterface.h>
#include <fun4allraw/Fun4AllStreamingInputManager.h>
#include <fun4allraw/InputManagerType.h>
#include <fun4allraw/SingleGl1PoolInput.h>
#include <fun4allraw/SingleInttPoolInput.h>
#include <fun4allraw/SingleMicromegasPoolInput.h>
#include <fun4allraw/SingleMvtxPoolInput.h>
#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllRunNodeInputManager.h>
#include <fun4all/Fun4AllServer.h>

#include <phool/recoConsts.h>
#include <ffamodules/HeadReco.h>
#include <ffamodules/FlagHandler.h>
#include <ffamodules/SyncReco.h>

#include <fun4allraw/SingleTpcTimeFrameInput.h>

#include <cdbobjects/CDBTTree.h>

#include <mvtxrawhitqa/MvtxRawHitQA.h>
#include <inttrawhitqa/InttRawHitQA.h>
#include <tpcqa/TpcRawHitQA.h>
#include <trackingqa/MvtxClusterQA.h>
#include <trackingqa/InttClusterQA.h>
#include <trackingqa/TpcClusterQA.h>
#include <trackingqa/TpcSeedsQA.h>
#include <trackingqa/SiliconSeedsQA.h>
#include <trackingqa/TpcSiliconQA.h>
#include <trackingqa/TrackFittingQA.h>
#include <trackingqa/MicromegasClusterQA.h>

#include <trackingqa/MvtxClusterQA.h>
#include <trackreco/PHActsSiliconSeeding.h>
#include <trackreco/PHSiliconSeedMerger.h>

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
R__LOAD_LIBRARY(libtrack_reco.so)

bool isGood(const string &infile);

void Fun4All_PRDFReconstruction(
    const int nEvents = 0,
    const int runnumber = 53756,
    const std::string &dir = "/.",
    const std::string &outfilename = "output_tracks",
    const std::string &input_gl1file = "gl1daq.list",
    const string &input_inttfile00 = "intt0.list",
    const string &input_inttfile01 = "intt1.list",
    const string &input_inttfile02 = "intt2.list",
    const string &input_inttfile03 = "intt3.list",
    const string &input_inttfile04 = "intt4.list",
    const string &input_inttfile05 = "intt5.list",
    const string &input_inttfile06 = "intt6.list",
    const string &input_inttfile07 = "intt7.list",
    const string &input_mvtxfile00 = "mvtx0.list",
    const string &input_mvtxfile01 = "mvtx1.list",
    const string &input_mvtxfile02 = "mvtx2.list",
    const string &input_mvtxfile03 = "mvtx3.list",
    const string &input_mvtxfile04 = "mvtx4.list",
    const string &input_mvtxfile05 = "mvtx5.list",
    const string &input_tpcfile00 = "tpc00_0.list",
    const string &input_tpcfile01 = "tpc01_0.list",
    const string &input_tpcfile02 = "tpc02_0.list",
    const string &input_tpcfile03 = "tpc03_0.list",
    const string &input_tpcfile04 = "tpc04_0.list",
    const string &input_tpcfile05 = "tpc05_0.list",
    const string &input_tpcfile06 = "tpc06_0.list",
    const string &input_tpcfile07 = "tpc07_0.list",
    const string &input_tpcfile08 = "tpc08_0.list",
    const string &input_tpcfile09 = "tpc09_0.list",
    const string &input_tpcfile10 = "tpc10_0.list",
    const string &input_tpcfile11 = "tpc11_0.list",
    const string &input_tpcfile12 = "tpc12_0.list",
    const string &input_tpcfile13 = "tpc13_0.list",
    const string &input_tpcfile14 = "tpc14_0.list",
    const string &input_tpcfile15 = "tpc15_0.list",
    const string &input_tpcfile16 = "tpc16_0.list",
    const string &input_tpcfile17 = "tpc17_0.list",
    const string &input_tpcfile18 = "tpc18_0.list",
    const string &input_tpcfile19 = "tpc19_0.list",
    const string &input_tpcfile20 = "tpc20_0.list",
    const string &input_tpcfile21 = "tpc21_0.list",
    const string &input_tpcfile22 = "tpc22_0.list",
    const string &input_tpcfile23 = "tpc23_0.list",
    const string &input_tpotfile = "tpot.list"
)
{
 vector<string> gl1_infile;
  gl1_infile.push_back(input_gl1file);

// MVTX
  vector<string> mvtx_infile;
  mvtx_infile.push_back(input_mvtxfile00);
  mvtx_infile.push_back(input_mvtxfile01);
  mvtx_infile.push_back(input_mvtxfile02);
  mvtx_infile.push_back(input_mvtxfile03);
  mvtx_infile.push_back(input_mvtxfile04);
  mvtx_infile.push_back(input_mvtxfile05);

// INTT
  vector<string> intt_infile;
  intt_infile.push_back(input_inttfile00);
  intt_infile.push_back(input_inttfile01);
  intt_infile.push_back(input_inttfile02);
  intt_infile.push_back(input_inttfile03);
  intt_infile.push_back(input_inttfile04);
  intt_infile.push_back(input_inttfile05);
  intt_infile.push_back(input_inttfile06);
  intt_infile.push_back(input_inttfile07);

  vector<string> tpc_infile;
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

// TPOT
  vector<string> tpot_infile;
  tpot_infile.push_back(input_tpotfile);

 auto se = Fun4AllServer::instance();
  se->Verbosity(1);
  auto rc = recoConsts::instance();
  rc->set_IntFlag("RUNNUMBER", runnumber);

  Enable::CDB = true;
  rc->set_StringFlag("CDB_GLOBALTAG", "ProdA_2024");
  
  rc->set_uint64Flag("TIMESTAMP", runnumber);

  //! flags to set
  Enable::QA = true;
  TRACKING::tpc_zero_supp = true;
  TRACKING::pp_mode = true;
  G4TRACKING::convert_seeds_to_svtxtracks = false;
  G4TPC::REJECT_LASER_EVENTS=true;
  
  Enable::MVTX_APPLYMISALIGNMENT = true;
  ACTSGEOM::mvtx_applymisalignment = Enable::MVTX_APPLYMISALIGNMENT;
  TpcReadoutInit( runnumber );
  std::cout<< " run: " << runnumber
	   << " samples: " << TRACKING::reco_tpc_maxtime_sample
	   << " pre: " << TRACKING::reco_tpc_time_presample
	   << " vdrift: " << G4TPC::tpc_drift_velocity_reco
	   << std::endl;

  CDBInterface::instance()->Verbosity(1);
  std::string geofile = CDBInterface::instance()->getUrl("Tracking_Geometry");
  std::cout << "CDB tracking geometry file "<<geofile << std::endl;
  Fun4AllRunNodeInputManager *ingeo = new Fun4AllRunNodeInputManager("GeoIn");
  ingeo->AddFile(geofile);
  se->registerInputManager(ingeo);

  CDBInterface *cdb = CDBInterface::instance();

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

  int i= 0;
  int NumInputs = 0;
  Fun4AllStreamingInputManager *in = new Fun4AllStreamingInputManager("Comb");

  for (auto iter : gl1_infile)
  {
    if (isGood(iter))
    {
      SingleGl1PoolInput *gl1_sngl = new SingleGl1PoolInput("GL1_" + to_string(i));
      //    gl1_sngl->Verbosity(3);
      gl1_sngl->AddListFile(iter);
      in->registerStreamingInput(gl1_sngl, InputManagerType::GL1);
      i++;
    }
  }
  NumInputs += i;

  i = 0;
  for (auto iter : intt_infile)
  {
    if (isGood(iter))
    {
    SingleInttPoolInput *intt_sngl = new SingleInttPoolInput("INTT_" + to_string(i));
    //    intt_sngl->Verbosity(3);
    InttOdbcQuery query;
    bool isStreaming = false;
    if(runnumber != 0)
      {
	query.Query(runnumber);
	isStreaming = query.IsStreaming();
      }
    intt_sngl->streamingMode(isStreaming);
    intt_sngl->AddListFile(iter);
    in->registerStreamingInput(intt_sngl, InputManagerType::INTT);
    i++;
    }
  }
  NumInputs += i;

  i = 0;
  for (auto iter : mvtx_infile)
  {
    if (isGood(iter))
    {
    SingleMvtxPoolInput *mvtx_sngl = new SingleMvtxPoolInput("MVTX_" + to_string(i));
    //    mvtx_sngl->Verbosity(3);
    mvtx_sngl->AddListFile(iter);
    in->registerStreamingInput(mvtx_sngl, InputManagerType::MVTX);
    i++;
    }
  }
  NumInputs += i;

  i = 0;


  
  for (auto iter : tpc_infile)
  {
    if (isGood(iter))
    {

      SingleTpcTimeFrameInput *tpc_sngl = new SingleTpcTimeFrameInput("TPC_" + to_string(i));
//    tpc_sngl->Verbosity(2);
      //   tpc_sngl->DryRun();
      tpc_sngl->setHitContainerName("TPCRAWHIT");
      tpc_sngl->AddListFile(iter);
      in->registerStreamingInput(tpc_sngl, InputManagerType::TPC);
      i++;
    }
  }
  NumInputs += i;
    i = 0;
  for (auto iter : tpot_infile)
  {
    if (isGood(iter))
    {
    SingleMicromegasPoolInput *mm_sngl = new SingleMicromegasPoolInput("MICROMEGAS_" + to_string(i));
    //   sngl->Verbosity(3);
    mm_sngl->SetBcoRange(10);
    mm_sngl->SetNegativeBco(2);
    mm_sngl->SetBcoPoolSize(50);
    mm_sngl->AddListFile(iter);
    in->registerStreamingInput(mm_sngl, InputManagerType::MICROMEGAS);
    i++;
    }
  }
  NumInputs += i;

  // if there is no input manager this macro will still run - so just quit here
  if (NumInputs == 0)
  {
    std::cout << "no file lists no input manager registered, quitting" << std::endl;
    gSystem->Exit(1);
  }
  se->registerInputManager(in);

  SyncReco *sync = new SyncReco();
  se->registerSubsystem(sync);

  HeadReco *head = new HeadReco();
  se->registerSubsystem(head);

  FlagHandler *flag = new FlagHandler();
  se->registerSubsystem(flag);

  
  Mvtx_HitUnpacking();
  Intt_HitUnpacking();
  Tpc_HitUnpacking();
  Micromegas_HitUnpacking();

  Mvtx_Clustering();

  Intt_Clustering();
  
  Tpc_LaserEventIdentifying();

  TPC_Clustering_run2pp();
  Micromegas_Clustering();

  Reject_Laser_Events();

  
  Tracking_Reco_TrackSeed_run2pp();
  Tracking_Reco_TrackMatching_run2pp();

  
  Tracking_Reco_TrackFit_run2pp();
  //vertexing and propagation to vertex
  Tracking_Reco_Vertex_run2pp();
  
  TString residoutfile = outfilename + "_resid.root";
  std::string residstring(residoutfile.Data());

  auto resid = new TrackResiduals("TrackResiduals");
  resid->Verbosity(0);
  resid->outfileName(residstring);
  resid->alignment(false);
  //resid->hitTree();
  resid->convertSeeds(G4TRACKING::convert_seeds_to_svtxtracks);


  if(ConstField && fieldstrength < 0.1)
  {
    resid->zeroField();
  }
  //se->registerSubsystem(resid);

  // Fun4AllOutputManager *out = new Fun4AllDstOutputManager("out", "/sphenix/tg/tg01/hf/jdosbo/tracking_development/onlineoffline/hitsets.root");
  // se->registerOutputManager(out);

  if (Enable::QA)
  {
    se->registerSubsystem(new MvtxRawHitQA);
    se->registerSubsystem(new InttRawHitQA);
    se->registerSubsystem(new TpcRawHitQA);
    se->registerSubsystem(new MvtxClusterQA);
    se->registerSubsystem(new InttClusterQA);
    se->registerSubsystem(new TpcClusterQA);
    se->registerSubsystem(new MicromegasClusterQA);
    se->registerSubsystem(new SiliconSeedsQA);
    se->registerSubsystem(new TpcSeedsQA);
    se->registerSubsystem(new TpcSiliconQA);
    se->registerSubsystem(new TrackFittingQA);
   
  }

  se->run(nEvents);
  se->End();
  se->PrintTimer();

  if(Enable::QA)
    {
      TString qaname = outfilename + runnumber +"_qa.root";
      std::string qaOutputFileName(qaname.Data());
      QAHistManagerDef::saveQARootFile(qaOutputFileName);
    }
  CDBInterface::instance()->Print();
  delete se;
  std::cout << "Finished" << std::endl;
  gSystem->Exit(0);
}


bool isGood(const string &infile)
{
  ifstream intest;
  intest.open(infile);
  bool goodfile = false;
  if (intest.is_open())
  {
    if (intest.peek() != std::ifstream::traits_type::eof()) // is it non zero?
    {
      goodfile = true;
    }
    intest.close();
  }
  return goodfile;
}
