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

#include <tpccalib/PHTpcResiduals.h>

#include <mvtxrawhitqa/MvtxRawHitQA.h>

#include <inttrawhitqa/InttRawHitQA.h>

#include <trackingqa/InttClusterQA.h>
#include <trackingqa/MicromegasClusterQA.h>
#include <trackingqa/MvtxClusterQA.h>
#include <trackingqa/SiliconSeedsQA.h>
#include <trackingqa/TpcClusterQA.h>
#include <trackingqa/TpcSeedsQA.h>
#include <trackingqa/TpcSiliconQA.h>

#include <tpcqa/TpcRawHitQA.h>

#include <trackingdiagnostics/KshortReconstruction.h>
#include <trackingdiagnostics/TrackResiduals.h>
#include <trackingdiagnostics/TrkrNtuplizer.h>

#include <kfparticle_sphenix/KFParticle_sPHENIX.h>

#include <ffamodules/CDBInterface.h>
#include <ffamodules/FlagHandler.h>

#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllRunNodeInputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllUtils.h>

#include <phool/recoConsts.h>

R__LOAD_LIBRARY(libkfparticle_sphenix.so)
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
R__LOAD_LIBRARY(libtpcqa.so)

namespace HeavyFlavorReco
{
  int VERBOSITY = 0;

  std::string output_dir = "./";  // Top dir of where the output nTuples will be written
  std::string kfp_header = "outputKFParticle_";
  std::string processing_folder = "inReconstruction/";
  std::string trailer = ".root";

  std::string pipi_decay_descriptor = "K_S0 -> pi^+ pi^-";  // See twiki on how to set this
  std::string pipi_reconstruction_name = "pipi_reco";       // Used for naming output folder, file and node
  std::string pipi_output_reco_file;
  std::string pipi_output_dir;

  std::string ppi_decay_descriptor = "[Lambda0 -> proton^+ pi^-]cc";  // See twiki on how to set this
  std::string ppi_reconstruction_name = "ppi_reco";                   // Used for naming output folder, file and node
  std::string ppi_output_reco_file;
  std::string ppi_output_dir;

  bool save_tracks_to_DST = false;
  bool dont_use_global_vertex = true;
  bool require_track_and_vertex_match = true;
  bool save_all_vtx_info = true;
  bool constrain_phi_mass = false;
  bool constrain_lambda_mass = false;
  bool constrain_D_mass = false;
  bool use_2D_matching = false;
  bool get_trigger_info = true;
  bool get_detector_info = true;
  bool get_dEdx_info = true;
  bool constrain_to_primary_vertex = true;
  bool use_pid = true;
  float pid_frac = 0.4;
};  // namespace HeavyFlavorReco

//using namespace HeavyFlavorReco;

void create_hf_directories(const std::string& reconstruction_name, std::string &final_output_dir, std::string &output_reco_file)
{
  std::string output_file_name = HeavyFlavorReco::kfp_header + reconstruction_name + HeavyFlavorReco::trailer;
  final_output_dir = HeavyFlavorReco::output_dir + reconstruction_name + "/";
  std::string output_reco_dir = final_output_dir + HeavyFlavorReco::processing_folder;
  output_reco_file = output_reco_dir + output_file_name;

  std::string makeDirectory = "mkdir -p " + output_reco_dir;
  system(makeDirectory.c_str());
}

void end_kfparticle(const std::string& full_file_name, const std::string& final_path)
{
  std::ifstream file(full_file_name.c_str());
  if (file.good())
  {
    std::string moveOutput = "mv " + full_file_name + " " + final_path;
    system(moveOutput.c_str());
  }
}

void Fun4All_raw_hit_KFP(
    const int nEvents = 10,
    const std::string& filelist = "filelist.list",
    const std::string& outfilename = "clusters_seeds",
    const bool convertSeeds = false,
    const int nSkip = 0,
    const bool doKFParticle = false)
{
  auto *se = Fun4AllServer::instance();
  se->Verbosity(2);
  auto *rc = recoConsts::instance();

  // input manager for QM production raw hit DST file
  std::ifstream ifs(filelist);
  std::string filepath;

  int i = 0;
  int runnumber = std::numeric_limits<int>::quiet_NaN();
  int segment = std::numeric_limits<int>::quiet_NaN();
  bool process_endpoints = false;

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
    if (filepath.find("ebdc") != std::string::npos)
    {
      if (filepath.find("_0_") != std::string::npos ||
          filepath.find("_1_") != std::string::npos)
      {
        process_endpoints = true;
      }
    }
    std::string inputname = "InputManager" + std::to_string(i);
    auto *hitsin = new Fun4AllDstInputManager(inputname);
    hitsin->fileopen(filepath);
    se->registerInputManager(hitsin);
    i++;
  }

  rc->set_IntFlag("RUNNUMBER", runnumber);
  rc->set_IntFlag("RUNSEGMENT", segment);

  Enable::QA = false;
  Enable::CDB = true;
  rc->set_StringFlag("CDB_GLOBALTAG", "newcdbtag");
  rc->set_uint64Flag("TIMESTAMP", runnumber);

  std::stringstream nice_runnumber;
  nice_runnumber << std::setw(8) << std::setfill('0') << std::to_string(runnumber);

  int rounded_up = 100 * (std::ceil((float) runnumber / 100));
  std::stringstream nice_rounded_up;
  nice_rounded_up << std::setw(8) << std::setfill('0') << std::to_string(rounded_up);

  int rounded_down = 100 * (std::floor((float) runnumber / 100));
  std::stringstream nice_rounded_down;
  nice_rounded_down << std::setw(8) << std::setfill('0') << std::to_string(rounded_down);

  std::stringstream nice_segment;
  nice_segment << std::setw(5) << std::setfill('0') << std::to_string(segment);

  std::stringstream nice_skip;
  nice_skip << std::setw(5) << std::setfill('0') << std::to_string(nSkip);

  HeavyFlavorReco::output_dir = "./";  // Top dir of where the output nTuples will be written
  HeavyFlavorReco::trailer = "_" + nice_runnumber.str() + "_" + nice_segment.str() + "_" + nice_skip.str() + ".root";

  if (doKFParticle)
  {
    create_hf_directories(HeavyFlavorReco::pipi_reconstruction_name, HeavyFlavorReco::pipi_output_dir, HeavyFlavorReco::pipi_output_reco_file);
    create_hf_directories(HeavyFlavorReco::ppi_reconstruction_name, HeavyFlavorReco::ppi_output_dir, HeavyFlavorReco::ppi_output_reco_file);
  }

  G4TRACKING::convert_seeds_to_svtxtracks = convertSeeds;
  std::cout << "Converting to seeds : " << G4TRACKING::convert_seeds_to_svtxtracks << std::endl;

  std::cout << " run: " << runnumber
            << " samples: " << TRACKING::reco_tpc_maxtime_sample
            << " pre: " << TRACKING::reco_tpc_time_presample
            << " vdrift: " << G4TPC::tpc_drift_velocity_reco
            << std::endl;

  TRACKING::pp_mode = false;

  // distortion calibration mode
  /*
   * set to true to enable residuals in the TPC with
   * TPC clusters not participating to the ACTS track fit
   */

  TString outfile = outfilename + "_" + runnumber + "-" + segment + ".root";
  std::string theOutfile = outfile.Data();

  FlagHandler *flag = new FlagHandler();
  se->registerSubsystem(flag);

  std::string geofile = CDBInterface::instance()->getUrl("Tracking_Geometry");

  Fun4AllRunNodeInputManager *ingeo = new Fun4AllRunNodeInputManager("GeoIn");
  ingeo->AddFile(geofile);
  se->registerInputManager(ingeo);

  TpcReadoutInit(runnumber);
  G4TPC::REJECT_LASER_EVENTS = true;
  G4TPC::ENABLE_MODULE_EDGE_CORRECTIONS = true;
  // Flag for running the tpc hit unpacker with zero suppression on
  TRACKING::tpc_zero_supp = true;

  // MVTX
  Enable::MVTX_APPLYMISALIGNMENT = true;
  ACTSGEOM::mvtx_applymisalignment = Enable::MVTX_APPLYMISALIGNMENT;

  // to turn on the default static corrections, enable the two lines below
  G4TPC::ENABLE_STATIC_CORRECTIONS = true;
  G4TPC::USE_PHI_AS_RAD_STATIC_CORRECTIONS = false;

  // to turn on the average corrections derived from simulation, enable the three lines below
  // note: these are designed to be used only if static corrections are also applied
  // G4TPC::ENABLE_AVERAGE_CORRECTIONS = true;
  // G4TPC::USE_PHI_AS_RAD_AVERAGE_CORRECTIONS = false;
  // G4TPC::average_correction_filename = std::string(getenv("CALIBRATIONROOT")) + "/distortion_maps/average_minus_static_distortion_inverted_10-new.root";

  G4MAGNET::magfield_rescale = 1;

  TrackingInit();

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
    if (!process_endpoints)
    {
      ebdcname.str("");
      if (ebdc < 10)
      {
        ebdcname << "0";
      }
      ebdcname << ebdc;
      Tpc_HitUnpacking(ebdcname.str());
    }

    else if (process_endpoints)
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
  }

  Micromegas_HitUnpacking();

  Mvtx_Clustering();

  Intt_Clustering();

  Tpc_LaserEventIdentifying();

  auto *tpcclusterizer = new TpcClusterizer;
  tpcclusterizer->Verbosity(0);
  tpcclusterizer->set_do_hit_association(G4TPC::DO_HIT_ASSOCIATION);
  tpcclusterizer->set_rawdata_reco();
  tpcclusterizer->set_reject_event(G4TPC::REJECT_LASER_EVENTS);
  se->registerSubsystem(tpcclusterizer);

  Micromegas_Clustering();

  Reject_Laser_Events();
  /*
   * Begin Track Seeding
   */

  auto *silicon_Seeding = new PHActsSiliconSeeding;
  silicon_Seeding->Verbosity(0);
  silicon_Seeding->setStrobeRange(-5, 5);
  // these get us to about 83% INTT > 1
  silicon_Seeding->setinttRPhiSearchWindow(0.4);
  silicon_Seeding->setinttZSearchWindow(2.0);
  silicon_Seeding->seedAnalysis(false);
  se->registerSubsystem(silicon_Seeding);

  auto *merger = new PHSiliconSeedMerger;
  merger->Verbosity(0);
  se->registerSubsystem(merger);

  /*
   * Tpc Seeding
   */
  auto *seeder = new PHCASeeding("PHCASeeding");
  double fieldstrength = std::numeric_limits<double>::quiet_NaN();  // set by isConstantField if constant
  bool ConstField = isConstantField(G4MAGNET::magfield_tracking, fieldstrength);
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
    se->registerSubsystem(cleaner);

    if (G4TRACKING::SC_CALIBMODE)
    {
      /*
       * in calibration mode, calculate residuals between TPC and fitted tracks,
       * store in dedicated structure for distortion correction
       */
      auto *residuals = new PHTpcResiduals;
      const TString tpc_residoutfile = theOutfile + "_PhTpcResiduals.root";
      residuals->setOutputfile(tpc_residoutfile.Data());
      residuals->setUseMicromegas(G4TRACKING::SC_USE_MICROMEGAS);

      // matches Tony's analysis
      residuals->setMinPt(0.2);

      // reconstructed distortion grid size (phi, r, z)
      residuals->setGridDimensions(36, 48, 80);
      se->registerSubsystem(residuals);
    }
  }

  auto *finder = new PHSimpleVertexFinder;
  finder->Verbosity(0);
  finder->setDcaCut(0.5);
  finder->setTrackPtCut(0.3);
  finder->setBeamLineCut(1);
  finder->setTrackQualityCut(1000);
  finder->setNmvtxRequired(3);
  finder->setOutlierPairCut(0.1);
  se->registerSubsystem(finder);

  if (!G4TRACKING::convert_seeds_to_svtxtracks)
  {
    // Propagate track positions to the vertex position
    auto *vtxProp = new PHActsVertexPropagator;
    vtxProp->Verbosity(0);
    vtxProp->fieldMap(G4MAGNET::magfield_tracking);
    se->registerSubsystem(vtxProp);
  }

  TString residoutfile = theOutfile + "_resid.root";
  std::string residstring(residoutfile.Data());

  auto *resid = new TrackResiduals("TrackResiduals");
  resid->outfileName(residstring);
  resid->alignment(false);
  resid->vertexTree();
  //   // adjust track map name
  //   if(G4TRACKING::SC_CALIBMODE && !G4TRACKING::convert_seeds_to_svtxtracks)
  //   {
  //     resid->trackmapName("SvtxSiliconMMTrackMap");
  //     if( G4TRACKING::SC_USE_MICROMEGAS )
  //     { resid->set_doMicromegasOnly(true); }
  //   }

  //   resid->clusterTree();
  //   resid->hitTree();
  resid->convertSeeds(G4TRACKING::convert_seeds_to_svtxtracks);

  //   resid->Verbosity(0);
  // se->registerSubsystem(resid);

  // auto ntuplizer = new TrkrNtuplizer("TrkrNtuplizer");
  // se->registerSubsystem(ntuplizer);

  /*
    // To write an output DST
    TString dstfile = theOutfile;
   std::string theDSTFile = dstfile.Data();
   Fun4AllOutputManager *out = new Fun4AllDstOutputManager("DSTOUT", theDSTFile.c_str());
   out->AddNode("Sync");
   out->AddNode("EventHeader");
   out->AddNode("TRKR_CLUSTER");
   out->AddNode("SiliconTrackSeedContainer");
   out->AddNode("TpcTrackSeedContainer");
   out->AddNode("SvtxTrackSeedContainer");
   out->AddNode("SvtxTrackMap");
   out->AddNode("SvtxVertexMap");
   out->AddNode("MbdVertexMap");
   out->AddNode("GL1RAWHIT");
   se->registerOutputManager(out);

  */
  if (Enable::QA)
  {
    se->registerSubsystem(new MvtxRawHitQA);
    se->registerSubsystem(new InttRawHitQA);
    se->registerSubsystem(new TpcRawHitQA);
    se->registerSubsystem(new MvtxClusterQA);
    se->registerSubsystem(new InttClusterQA);
    se->registerSubsystem(new TpcClusterQA);
    se->registerSubsystem(new MicromegasClusterQA);

    auto *converter = new TrackSeedTrackMapConverter("SiliconSeedConverter");
    // Default set to full SvtxTrackSeeds. Can be set to
    // SiliconTrackSeedContainer or TpcTrackSeedContainer
    converter->setTrackSeedName("SiliconTrackSeedContainer");
    converter->setTrackMapName("SiliconSvtxTrackMap");
    converter->setFieldMap(G4MAGNET::magfield_tracking);
    converter->Verbosity(0);
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
    findertpc->setDcaCut(0.5);
    findertpc->setTrackPtCut(0.2);
    findertpc->setBeamLineCut(1);
    findertpc->setTrackQualityCut(1000000000);
    // findertpc->setNmvtxRequired(3);
    findertpc->setRequireMVTX(false);
    findertpc->setOutlierPairCut(0.1);
    findertpc->setTrackMapName("TpcSvtxTrackMap");
    findertpc->setVertexMapName("TpcSvtxVertexMap");
    se->registerSubsystem(findertpc);

    auto *tpcqa = new TpcSeedsQA;
    tpcqa->setTrackMapName("TpcSvtxTrackMap");
    tpcqa->setVertexMapName("TpcSvtxVertexMap");
    tpcqa->setSegment(rc->get_IntFlag("RUNSEGMENT"));
    se->registerSubsystem(tpcqa);

    se->registerSubsystem(new TpcSiliconQA);
  }

  if (doKFParticle)
  {
    // KFParticle dependancy
    Global_Reco();

    // KFParticle setup

    KFParticle_sPHENIX *kfparticle = new KFParticle_sPHENIX("pipi_reco");
    kfparticle->Verbosity(10);
    kfparticle->setDecayDescriptor("K_S0 -> pi^+ pi^-");
    // kfparticle->setDecayDescriptor("[K_S0 -> pi^+ pi^+]cc");

    kfparticle->usePID(HeavyFlavorReco::use_pid);
    kfparticle->setPIDacceptFraction(HeavyFlavorReco::pid_frac);
    kfparticle->dontUseGlobalVertex(HeavyFlavorReco::dont_use_global_vertex);
    kfparticle->requireTrackVertexBunchCrossingMatch(HeavyFlavorReco::require_track_and_vertex_match);
    kfparticle->getAllPVInfo(HeavyFlavorReco::save_all_vtx_info);
    kfparticle->allowZeroMassTracks();
    kfparticle->use2Dmatching(HeavyFlavorReco::use_2D_matching);
    kfparticle->getTriggerInfo(HeavyFlavorReco::get_trigger_info);
    kfparticle->getDetectorInfo(HeavyFlavorReco::get_detector_info);
    kfparticle->saveDST(HeavyFlavorReco::save_tracks_to_DST);
    kfparticle->saveParticleContainer(false);
    kfparticle->magFieldFile("FIELDMAP_TRACKING");

    // PV to SV cuts
    kfparticle->constrainToPrimaryVertex(HeavyFlavorReco::constrain_to_primary_vertex);
    kfparticle->setMotherIPchi2(100);
    kfparticle->setFlightDistancechi2(-1.);
    kfparticle->setMinDIRA(0.88);                    // was .95
    kfparticle->setDecayLengthRange(-0.1, FLT_MAX);  // was 0.1 min

    kfparticle->setDecayLengthRange_XY(-10000, FLT_MAX);
    kfparticle->setDecayTimeRange_XY(-10000, FLT_MAX);
    kfparticle->setDecayTimeRange(-10000, FLT_MAX);
    kfparticle->setMinDecayTimeSignificance(-1e5);
    kfparticle->setMinDecayLengthSignificance(-1e5);
    kfparticle->setMinDecayLengthSignificance_XY(-1e5);
    kfparticle->setMaximumDaughterDCA_XY(100);

    // Track parameters
    kfparticle->setMinimumTrackPT(0.0);
    kfparticle->setMinimumTrackIPchi2(-1.);
    kfparticle->setMinimumTrackIP(-1.);
    kfparticle->setMaximumTrackchi2nDOF(100.);
    kfparticle->setMinINTThits(0);
    kfparticle->setMinMVTXhits(0);
    kfparticle->setMinTPChits(20);

    // Vertex parameters
    kfparticle->setMaximumVertexchi2nDOF(20);
    kfparticle->setMaximumDaughterDCA(0.5);  // 5 mm

    // Parent parameters
    kfparticle->setMotherPT(0);
    kfparticle->setMinimumMass(0.40);  // Check mass ranges
    kfparticle->setMaximumMass(0.60);
    kfparticle->setMaximumMotherVertexVolume(0.1);

    kfparticle->setOutputName(HeavyFlavorReco::pipi_output_reco_file);

    se->registerSubsystem(kfparticle);

    // Lambda reconstruction
    KFParticle_sPHENIX *kfparticleLambda = new KFParticle_sPHENIX("ppi_reco");
    kfparticleLambda->Verbosity(0);
    kfparticleLambda->setDecayDescriptor("[Lambda0 -> proton^+ pi^-]cc");

    kfparticle->usePID(HeavyFlavorReco::use_pid);
    kfparticle->setPIDacceptFraction(HeavyFlavorReco::pid_frac);
    kfparticle->dontUseGlobalVertex(HeavyFlavorReco::dont_use_global_vertex);
    kfparticle->requireTrackVertexBunchCrossingMatch(HeavyFlavorReco::require_track_and_vertex_match);
    kfparticle->getAllPVInfo(HeavyFlavorReco::save_all_vtx_info);
    kfparticle->allowZeroMassTracks();
    kfparticle->use2Dmatching(HeavyFlavorReco::use_2D_matching);
    kfparticle->getTriggerInfo(HeavyFlavorReco::get_trigger_info);
    kfparticle->getDetectorInfo(HeavyFlavorReco::get_detector_info);
    kfparticle->saveDST(HeavyFlavorReco::save_tracks_to_DST);
    kfparticle->saveParticleContainer(false);
    kfparticle->magFieldFile("FIELDMAP_TRACKING");

    // PV to SV cuts
    kfparticle->constrainToPrimaryVertex(HeavyFlavorReco::constrain_to_primary_vertex);
    kfparticle->setMotherIPchi2(100);
    kfparticle->setFlightDistancechi2(-1.);
    kfparticle->setMinDIRA(0.88);
    kfparticle->setDecayLengthRange(0.2, FLT_MAX);

    // Track parameters
    kfparticle->setMinimumTrackPT(0.1);
    kfparticle->setMinimumTrackIPchi2(-1.);
    kfparticle->setMinimumTrackIP(-1.);
    kfparticle->setMaximumTrackchi2nDOF(100.);
    kfparticle->setMinTPChits(25);

    // Vertex parameters
    kfparticle->setMaximumVertexchi2nDOF(20);
    kfparticle->setMaximumDaughterDCA(0.5);  // 5 mm

    // Parent parameters
    kfparticle->setMotherPT(0);
    kfparticle->setMinimumMass(0.900);  // Check mass ranges
    kfparticle->setMaximumMass(1.300);
    kfparticle->setMaximumMotherVertexVolume(0.1);

    kfparticle->setOutputName(HeavyFlavorReco::ppi_output_reco_file);

    se->registerSubsystem(kfparticleLambda);
  }

  se->skip(nSkip);
  se->run(nEvents);
  se->End();
  se->PrintTimer();

  if (doKFParticle)
  {
    end_kfparticle(HeavyFlavorReco::pipi_output_reco_file, HeavyFlavorReco::pipi_output_dir);
    end_kfparticle(HeavyFlavorReco::ppi_output_reco_file, HeavyFlavorReco::ppi_output_dir);
  }
  if (Enable::QA)
  {
    std::string qaOutputFileName = theOutfile + "_qa.root";
    QAHistManagerDef::saveQARootFile(qaOutputFileName);
  }
  CDBInterface::instance()->Print();
  delete se;
  std::cout << "Finished" << std::endl;
  gSystem->Exit(0);
}
