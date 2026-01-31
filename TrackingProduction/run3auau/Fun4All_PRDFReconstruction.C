/*
 * This macro shows running the full event combining + tracking for
 * cosmics running.. There are some analysis modules run at the end
 * which package  hits, clusters, and clusters on tracks into trees
 * for analysis.
 */

// leave the GlobalVariables.C at the beginning, an empty line afterwards
// protects its position against reshuffling by clang-format
#include <GlobalVariables.C>

#include <G4_ActsGeom.C>
#include <G4_Magnet.C>
#include <QA.C>
#include <Trkr_Clustering.C>
#include <Trkr_Reco.C>
#include <Trkr_RecoInit.C>
#include <Trkr_TpcReadoutInit.C>

#include <trackreco/PHActsSiliconSeeding.h>
#include <trackreco/PHSiliconSeedMerger.h>

#include <cdbobjects/CDBTTree.h>

#include <ffamodules/CDBInterface.h>
#include <ffamodules/FlagHandler.h>
#include <ffamodules/HeadReco.h>
#include <ffamodules/SyncReco.h>

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

#include <trackingdiagnostics/TrackResiduals.h>
#include <trackingdiagnostics/TrkrNtuplizer.h>

#include <fun4allraw/Fun4AllStreamingInputManager.h>
#include <fun4allraw/InputManagerType.h>
#include <fun4allraw/SingleGl1PoolInput.h>
#include <fun4allraw/SingleInttPoolInput.h>
#include <fun4allraw/SingleMicromegasPoolInput.h>
#include <fun4allraw/SingleMvtxPoolInput.h>
#include <fun4allraw/SingleTpcTimeFrameInput.h>

#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllRunNodeInputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllUtils.h>

#include <phool/recoConsts.h>

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

bool isGood(const std::string &infile);
int getrunnumber(const std::string &listfile);

void Fun4All_PRDFReconstruction(
    const int nEvents = 0,
    const int  /*runnumber_unused*/ = 53756, // just kept to preserve the API
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
    const std::string &input_tpcfile00_0 = "tpc00_0.list",
    const std::string &input_tpcfile00_1 = "tpc00_1.list",
    const std::string &input_tpcfile01_0 = "tpc01_0.list",
    const std::string &input_tpcfile01_1 = "tpc01_1.list",
    const std::string &input_tpcfile02_0 = "tpc02_0.list",
    const std::string &input_tpcfile02_1 = "tpc02_1.list",
    const std::string &input_tpcfile03_0 = "tpc03_0.list",
    const std::string &input_tpcfile03_1 = "tpc03_1.list",
    const std::string &input_tpcfile04_0 = "tpc04_0.list",
    const std::string &input_tpcfile04_1 = "tpc04_1.list",
    const std::string &input_tpcfile05_0 = "tpc05_0.list",
    const std::string &input_tpcfile05_1 = "tpc05_1.list",
    const std::string &input_tpcfile06_0 = "tpc06_0.list",
    const std::string &input_tpcfile06_1 = "tpc06_1.list",
    const std::string &input_tpcfile07_0 = "tpc07_0.list",
    const std::string &input_tpcfile07_1 = "tpc07_1.list",
    const std::string &input_tpcfile08_0 = "tpc08_0.list",
    const std::string &input_tpcfile08_1 = "tpc08_1.list",
    const std::string &input_tpcfile09_0 = "tpc09_0.list",
    const std::string &input_tpcfile09_1 = "tpc09_1.list",
    const std::string &input_tpcfile10_0 = "tpc10_0.list",
    const std::string &input_tpcfile10_1 = "tpc10_1.list",
    const std::string &input_tpcfile11_0 = "tpc11_0.list",
    const std::string &input_tpcfile11_1 = "tpc11_1.list",
    const std::string &input_tpcfile12_0 = "tpc12_0.list",
    const std::string &input_tpcfile12_1 = "tpc12_1.list",
    const std::string &input_tpcfile13_0 = "tpc13_0.list",
    const std::string &input_tpcfile13_1 = "tpc13_1.list",
    const std::string &input_tpcfile14_0 = "tpc14_0.list",
    const std::string &input_tpcfile14_1 = "tpc14_1.list",
    const std::string &input_tpcfile15_0 = "tpc15_0.list",
    const std::string &input_tpcfile15_1 = "tpc15_1.list",
    const std::string &input_tpcfile16_0 = "tpc16_0.list",
    const std::string &input_tpcfile16_1 = "tpc16_1.list",
    const std::string &input_tpcfile17_0 = "tpc17_0.list",
    const std::string &input_tpcfile17_1 = "tpc17_1.list",
    const std::string &input_tpcfile18_0 = "tpc18_0.list",
    const std::string &input_tpcfile18_1 = "tpc18_1.list",
    const std::string &input_tpcfile19_0 = "tpc19_0.list",
    const std::string &input_tpcfile19_1 = "tpc19_1.list",
    const std::string &input_tpcfile20_0 = "tpc20_0.list",
    const std::string &input_tpcfile20_1 = "tpc20_1.list",
    const std::string &input_tpcfile21_0 = "tpc21_0.list",
    const std::string &input_tpcfile21_1 = "tpc21_1.list",
    const std::string &input_tpcfile22_0 = "tpc22_0.list",
    const std::string &input_tpcfile22_1 = "tpc22_1.list",
    const std::string &input_tpcfile23_0 = "tpc23_0.list",
    const std::string &input_tpcfile23_1 = "tpc23_1.list",
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
  tpc_infile.push_back(input_tpcfile00_0);
  tpc_infile.push_back(input_tpcfile01_0);
  tpc_infile.push_back(input_tpcfile02_0);
  tpc_infile.push_back(input_tpcfile03_0);
  tpc_infile.push_back(input_tpcfile04_0);
  tpc_infile.push_back(input_tpcfile05_0);
  tpc_infile.push_back(input_tpcfile06_0);
  tpc_infile.push_back(input_tpcfile07_0);
  tpc_infile.push_back(input_tpcfile08_0);
  tpc_infile.push_back(input_tpcfile09_0);
  tpc_infile.push_back(input_tpcfile10_0);
  tpc_infile.push_back(input_tpcfile11_0);
  tpc_infile.push_back(input_tpcfile12_0);
  tpc_infile.push_back(input_tpcfile13_0);
  tpc_infile.push_back(input_tpcfile14_0);
  tpc_infile.push_back(input_tpcfile15_0);
  tpc_infile.push_back(input_tpcfile16_0);
  tpc_infile.push_back(input_tpcfile17_0);
  tpc_infile.push_back(input_tpcfile18_0);
  tpc_infile.push_back(input_tpcfile19_0);
  tpc_infile.push_back(input_tpcfile20_0);
  tpc_infile.push_back(input_tpcfile21_0);
  tpc_infile.push_back(input_tpcfile22_0);
  tpc_infile.push_back(input_tpcfile23_0);
  tpc_infile.push_back(input_tpcfile00_1);
  tpc_infile.push_back(input_tpcfile01_1);
  tpc_infile.push_back(input_tpcfile02_1);
  tpc_infile.push_back(input_tpcfile03_1);
  tpc_infile.push_back(input_tpcfile04_1);
  tpc_infile.push_back(input_tpcfile05_1);
  tpc_infile.push_back(input_tpcfile06_1);
  tpc_infile.push_back(input_tpcfile07_1);
  tpc_infile.push_back(input_tpcfile08_1);
  tpc_infile.push_back(input_tpcfile09_1);
  tpc_infile.push_back(input_tpcfile10_1);
  tpc_infile.push_back(input_tpcfile11_1);
  tpc_infile.push_back(input_tpcfile12_1);
  tpc_infile.push_back(input_tpcfile13_1);
  tpc_infile.push_back(input_tpcfile14_1);
  tpc_infile.push_back(input_tpcfile15_1);
  tpc_infile.push_back(input_tpcfile16_1);
  tpc_infile.push_back(input_tpcfile17_1);
  tpc_infile.push_back(input_tpcfile18_1);
  tpc_infile.push_back(input_tpcfile19_1);
  tpc_infile.push_back(input_tpcfile20_1);
  tpc_infile.push_back(input_tpcfile21_1);
  tpc_infile.push_back(input_tpcfile22_1);
  tpc_infile.push_back(input_tpcfile23_1);

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
  se->Verbosity(2);
  auto *rc = recoConsts::instance();
  rc->set_IntFlag("RUNNUMBER", runnumber);

  Enable::CDB = true;
  rc->set_StringFlag("CDB_GLOBALTAG", "newcdbtag");

  rc->set_uint64Flag("TIMESTAMP", runnumber);

  //! flags to set
  Enable::QA = true;
  TRACKING::tpc_zero_supp = true;
  G4TRACKING::convert_seeds_to_svtxtracks = false;

  Enable::MVTX_APPLYMISALIGNMENT = true;
  ACTSGEOM::mvtx_applymisalignment = Enable::MVTX_APPLYMISALIGNMENT;
  TpcReadoutInit(runnumber);
  std::cout << " run: " << runnumber
            << " samples: " << TRACKING::reco_tpc_maxtime_sample
            << " pre: " << TRACKING::reco_tpc_time_presample
            << " vdrift: " << G4TPC::tpc_drift_velocity_reco
            << std::endl;

  CDBInterface::instance()->Verbosity(1);
  std::string geofile = CDBInterface::instance()->getUrl("Tracking_Geometry");
  std::cout << "CDB tracking geometry file " << geofile << std::endl;
  Fun4AllRunNodeInputManager *ingeo = new Fun4AllRunNodeInputManager("GeoIn");
  ingeo->AddFile(geofile);
  se->registerInputManager(ingeo);

  // can use for zero field
  // double fieldstrength = 0.01;
  // G4MAGNET::magfield_tracking = "0.01";
  double fieldstrength = std::numeric_limits<double>::quiet_NaN();
  bool ConstField = isConstantField(G4MAGNET::magfield_tracking, fieldstrength);

  if (ConstField && fieldstrength < 0.1)
  {
    G4MAGNET::magfield = "0.01";
    G4MAGNET::magfield_rescale = 1;
  }

  TrackingInit();

  int i = 0;
  int NumInputs = 0;
  Fun4AllStreamingInputManager *in = new Fun4AllStreamingInputManager("Comb");

  for (const auto& iter : gl1_infile)
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
  NumInputs += i;

  i = 0;
  for (const auto& iter : intt_infile)
  {
    if (isGood(iter))
    {
      SingleInttPoolInput *intt_sngl = new SingleInttPoolInput("INTT_" + std::to_string(i));
      //    intt_sngl->Verbosity(3);
      intt_sngl->AddListFile(iter);
      in->registerStreamingInput(intt_sngl, InputManagerType::INTT);
      i++;
    }
  }
  NumInputs += i;

  i = 0;
  for (const auto& iter : mvtx_infile)
  {
    if (isGood(iter))
    {
      SingleMvtxPoolInput *mvtx_sngl = new SingleMvtxPoolInput("MVTX_" + std::to_string(i));
      //    mvtx_sngl->Verbosity(3);
      mvtx_sngl->AddListFile(iter);
      in->registerStreamingInput(mvtx_sngl, InputManagerType::MVTX);
      i++;
    }
  }
  NumInputs += i;

  i = 0;

  for (const auto& iter : tpc_infile)
  {
    if (isGood(iter))
    {
      SingleTpcTimeFrameInput *tpc_sngl = new SingleTpcTimeFrameInput("TPC_" + std::to_string(i));
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
  for (const auto& iter : tpot_infile)
  {
    if (isGood(iter))
    {
      SingleMicromegasPoolInput *mm_sngl = new SingleMicromegasPoolInput("MICROMEGAS_" + std::to_string(i));
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

  MvtxClusterizer *mvtxclusterizer = new MvtxClusterizer("MvtxClusterizer");

  se->registerSubsystem(mvtxclusterizer);

  Intt_Clustering();

  Tpc_LaserEventIdentifying();

  auto *tpcclusterizer = new TpcClusterizer;
  tpcclusterizer->Verbosity(0);
  tpcclusterizer->set_do_hit_association(G4TPC::DO_HIT_ASSOCIATION);
  tpcclusterizer->set_rawdata_reco();
  tpcclusterizer->set_reject_event(G4TPC::REJECT_LASER_EVENTS);
  se->registerSubsystem(tpcclusterizer);

  Micromegas_Clustering();

  auto *silicon_Seeding = new PHActsSiliconSeeding;
  silicon_Seeding->Verbosity(0);
  // these get us to about 83% INTT > 1
  silicon_Seeding->setinttRPhiSearchWindow(0.4);
  silicon_Seeding->setinttZSearchWindow(2.0);
  silicon_Seeding->setStrobeRange(-5, 5);
  silicon_Seeding->seedAnalysis(false);
  se->registerSubsystem(silicon_Seeding);

  auto *merger = new PHSiliconSeedMerger;
  merger->Verbosity(0);
  se->registerSubsystem(merger);

  /*
   * Tpc Seeding
   */
  auto *seeder = new PHCASeeding("PHCASeeding");
  if (ConstField)
  {
    seeder->useConstBField(true);
    seeder->constBField(fieldstrength);
  }
  else
  {
    seeder->set_field_dir(-1 * G4MAGNET::magfield_rescale);
    seeder->useConstBField(false);
    seeder->magFieldFile(G4MAGNET::magfield_tracking);  // to get charge sign right
  }
  seeder->Verbosity(0);
  seeder->SetLayerRange(7, 55);
  seeder->SetSearchWindow(2., 0.05);           // z-width and phi-width, default in macro at 1.5 and 0.05
  seeder->SetClusAdd_delta_window(3.0, 0.06);  //  (0.5, 0.005) are default; sdzdr_cutoff, d2/dr2(phi)_cutoff
  // seeder->SetNClustersPerSeedRange(4,60); // default is 6, 6
  seeder->SetMinHitsPerCluster(0);
  seeder->SetMinClustersPerTrack(3);
  seeder->useFixedClusterError(true);
  seeder->set_pp_mode(true);
  se->registerSubsystem(seeder);

  // expand stubs in the TPC using simple kalman filter
  auto *cprop = new PHSimpleKFProp("PHSimpleKFProp");
  cprop->set_field_dir(G4MAGNET::magfield_rescale);
  if (ConstField)
  {
    cprop->useConstBField(true);
    cprop->setConstBField(fieldstrength);
  }
  else
  {
    cprop->magFieldFile(G4MAGNET::magfield_tracking);
    cprop->set_field_dir(-1 * G4MAGNET::magfield_rescale);
  }
  cprop->useFixedClusterError(true);
  cprop->set_max_window(5.);
  cprop->Verbosity(0);
  cprop->set_pp_mode(true);
  se->registerSubsystem(cprop);

  // Always apply preliminary distortion corrections to TPC clusters before silicon matching
  // and refit the trackseeds. Replace KFProp fits with the new fit parameters in the TPC seeds.
  auto *prelim_distcorr = new PrelimDistortionCorrection;
  prelim_distcorr->set_pp_mode(true);
  prelim_distcorr->Verbosity(0);
  se->registerSubsystem(prelim_distcorr);

  /*
   * Track Matching between silicon and TPC
   */
  // The normal silicon association methods
  // Match the TPC track stubs from the CA seeder to silicon track stubs from PHSiliconTruthTrackSeeding
  auto *silicon_match = new PHSiliconTpcTrackMatching;
  silicon_match->Verbosity(0);
  silicon_match->set_pp_mode(TRACKING::pp_mode);
  if (G4TPC::ENABLE_AVERAGE_CORRECTIONS)
  {
    // for general tracking
    // Eta/Phi window is determined by 3 sigma window
    // X/Y/Z window is determined by 4 sigma window
    silicon_match->window_deta.set_posQoverpT_maxabs({-0.014, 0.0331, 0.48});
    silicon_match->window_deta.set_negQoverpT_maxabs({-0.006, 0.0235, 0.52});
    silicon_match->set_deltaeta_min(0.03);
    silicon_match->window_dphi.set_QoverpT_range({-0.15, 0, 0}, {0.15, 0, 0});
    silicon_match->window_dx.set_QoverpT_maxabs({3.0, 0, 0});
    silicon_match->window_dy.set_QoverpT_maxabs({3.0, 0, 0});
    silicon_match->window_dz.set_posQoverpT_maxabs({1.138, 0.3919, 0.84});
    silicon_match->window_dz.set_negQoverpT_maxabs({0.719, 0.6485, 0.65});
    silicon_match->set_crossing_deltaz_max(30);
    silicon_match->set_crossing_deltaz_min(2);

    // for distortion correction using SI-TPOT fit and track pT>0.5
    if (G4TRACKING::SC_CALIBMODE)
    {
      silicon_match->window_deta.set_posQoverpT_maxabs({0.016, 0.0060, 1.13});
      silicon_match->window_deta.set_negQoverpT_maxabs({0.022, 0.0022, 1.44});
      silicon_match->set_deltaeta_min(0.03);
      silicon_match->window_dphi.set_QoverpT_range({-0.15, 0, 0}, {0.09, 0, 0});
      silicon_match->window_dx.set_QoverpT_maxabs({2.0, 0, 0});
      silicon_match->window_dy.set_QoverpT_maxabs({1.5, 0, 0});
      silicon_match->window_dz.set_posQoverpT_maxabs({1.213, 0.0211, 2.09});
      silicon_match->window_dz.set_negQoverpT_maxabs({1.307, 0.0001, 4.52});
      silicon_match->set_crossing_deltaz_min(1.2);
    }
  }
  se->registerSubsystem(silicon_match);

  // Match TPC track stubs from CA seeder to clusters in the micromegas layers
  auto *mm_match = new PHMicromegasTpcTrackMatching;
  mm_match->Verbosity(0);
  mm_match->set_rphi_search_window_lyr1(3.);
  mm_match->set_rphi_search_window_lyr2(15.0);
  mm_match->set_z_search_window_lyr1(30.0);
  mm_match->set_z_search_window_lyr2(3.);

  mm_match->set_min_tpc_layer(38);             // layer in TPC to start projection fit
  mm_match->set_test_windows_printout(false);  // used for tuning search windows only
  se->registerSubsystem(mm_match);

  /*
   * End Track Seeding
   */

  /*
   * Either converts seeds to tracks with a straight line/helix fit
   * or run the full Acts track kalman filter fit
   */
  if (G4TRACKING::convert_seeds_to_svtxtracks)
  {
    auto *converter = new TrackSeedTrackMapConverter;
    // Default set to full SvtxTrackSeeds. Can be set to
    // SiliconTrackSeedContainer or TpcTrackSeedContainer
    converter->setTrackSeedName("SvtxTrackSeedContainer");
    converter->setFieldMap(G4MAGNET::magfield_tracking);
    converter->Verbosity(0);
    se->registerSubsystem(converter);
  }
  else
  {
    auto *deltazcorr = new PHTpcDeltaZCorrection;
    deltazcorr->Verbosity(0);
    se->registerSubsystem(deltazcorr);

    // perform final track fit with ACTS
    auto *actsFit = new PHActsTrkFitter;
    actsFit->Verbosity(0);
    actsFit->commissioning(G4TRACKING::use_alignment);
    // in calibration mode, fit only Silicons and Micromegas hits
    actsFit->fitSiliconMMs(G4TRACKING::SC_CALIBMODE);
    actsFit->setUseMicromegas(G4TRACKING::SC_USE_MICROMEGAS);
    actsFit->set_pp_mode(TRACKING::pp_mode);
    actsFit->set_use_clustermover(true);  // default is true for now
    actsFit->useActsEvaluator(false);
    actsFit->useOutlierFinder(false);
    actsFit->setFieldMap(G4MAGNET::magfield_tracking);
    se->registerSubsystem(actsFit);

    auto *cleaner = new PHTrackCleaner();
    cleaner->Verbosity(0);
    cleaner->set_pp_mode(TRACKING::pp_mode);
    // se->registerSubsystem(cleaner);
  }

  auto *finder = new PHSimpleVertexFinder;
  finder->Verbosity(0);
  finder->setDcaCut(0.5);
  finder->setTrackPtCut(-99999.);
  finder->setBeamLineCut(1);
  finder->setTrackQualityCut(1000000000);
  finder->setNmvtxRequired(3);
  finder->setOutlierPairCut(0.1);
  se->registerSubsystem(finder);

  std::string residstring = outfilename + "_resid.root";

  auto *resid = new TrackResiduals("TrackResiduals");
  resid->Verbosity(0);
  resid->outfileName(residstring);
  resid->alignment(false);
  resid->clusterTree();
  // resid->failedTree();
  // resid->hitTree();
  // resid->noEventTree(); // not implemented anymore?
  resid->convertSeeds(G4TRACKING::convert_seeds_to_svtxtracks);

  if (ConstField && fieldstrength < 0.1)
  {
    resid->zeroField();
  }
  se->registerSubsystem(resid);

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
  }

  se->run(nEvents);
  se->End();
  se->PrintTimer();

  if (Enable::QA)
  {
    TString qaname = outfilename + runnumber + "_qa.root";
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
  if (! isGood(listfile))
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
