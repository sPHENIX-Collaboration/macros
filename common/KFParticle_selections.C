#ifndef MACRO_KFPARTICLE_SELECTIONS_C
#define MACRO_KFPARTICLE_SELECTIONS_C

#include <fun4all/Fun4AllServer.h>

#include <calotrigger/TriggerRunInfoReco.h>
#include <globalvertex/GlobalVertexReco.h>

#include <kfparticleqa/QAKFParticle.h>
#include <kfparticleqa/QAKFParticleTrackPtAsymmetry.h>
#include <QA.C>

#include <cfloat>

#pragma GCC diagnostic push

#pragma GCC diagnostic ignored "-Wundefined-internal"

#include <kfparticle_sphenix/KFParticle_sPHENIX.h>

#pragma GCC diagnostic pop

R__LOAD_LIBRARY(libkfparticle_sphenix.so)
R__LOAD_LIBRARY(libcalotrigger.so)
R__LOAD_LIBRARY(libkfparticleqa.so)

namespace KFPARTICLE
{
  int VERBOSITY_KFP = 0;

  bool getQA = false; //Need QA of HF for productions

  bool run_pipi_reco = true;
  bool run_ppi_reco = true; // set to true if needed
  bool run_KK_reco = false; // set to true if needed
  bool run_Lambdapi_reco = true; // set to true if needed
  bool run_Kpi_reco = true; // set to true if needed
  bool run_Kpipi_reco = true; // set to true if needed
  bool run_KKpi_reco = true; // set to true if needed
  bool run_pKpi_reco = true; // set to true if needed
  bool run_ee_reco = false; // set to true if needed

  std::string output_dir = "./"; //Top dir of where the output nTuples will be written
  std::string kfp_header = "outputKFParticle_";
  std::string processing_folder = "inReconstruction/";
  std::string trailer = ".root";

  // Mass windows (GeV), {min, max}
  float KS0_mass[2] = {0.40, 0.60};
  float L0_mass[2] = {1.08, 1.15};
  float Xi_mass[2] = {1.30, 1.35}; //Check mass ranges
  float Xi_L0_intermediate_mass[2] = {1.1057, 1.1257}; //Lambda inside Xi-
  float phi_mass[2] = {0.99, 1.04};
  float D0_mass[2] = {1.7, 2.0};
  float Dp_mass[2] = {1.7, 2.0};
  float Ds_mass[2] = {1.7, 2.2};
  float Lc_mass[2] = {2.1, 2.5};
  float ee_mass[2] = {0.0, 0.1};

  // https://wiki.bnl.gov/sPHENIX/index.php/KFParticle
  std::string pipi_decay_descriptor = "K_S0 -> pi^+ pi^-"; //See twiki on how to set this
  std::string pipi_reconstruction_name = "pipi_reco"; //Used for naming output folder, file and node
  std::string pipi_output_reco_file = "output_KFP_pipi.root";
  std::string pipi_output_dir;

  std::string ppi_decay_descriptor = "[Lambda0 -> proton^+ pi^-]cc"; //See twiki on how to set this
  std::string ppi_reconstruction_name = "ppi_reco"; //Used for naming output folder, file and node
  std::string ppi_output_reco_file = "output_KFP_ppi.root";
  std::string ppi_output_dir;

  std::string KK_decay_descriptor = "phi -> K^+ K^-"; //See twiki on how to set this
  std::string KK_reconstruction_name = "KK_reco"; //Used for naming output folder, file and node
  std::string KK_output_reco_file = "output_KFP_KK.root";
  std::string KK_output_dir;

  std::string Lambdapi_decay_descriptor = "[Xi- -> {Lambda0 -> proton^+ pi^-} pi^-]cc"; //See twiki on how to set this
  std::string Lambdapi_reconstruction_name = "Lambdapi_reco"; //Used for naming output folder, file and node
  std::string Lambdapi_output_reco_file = "output_KFP_Lambdapi.root";
  std::string Lambdapi_output_dir;

  std::string Kpi_decay_descriptor = "[D0 -> K^- pi^+]cc"; //See twiki on how to set this
  std::string Kpi_reconstruction_name = "Kpi_reco"; //Used for naming output folder, file and node
  std::string Kpi_output_reco_file = "output_KFP_Kpi.root";
  std::string Kpi_output_dir;

  std::string Kpipi_decay_descriptor = "[D+ -> K^- pi^+ pi^+]cc"; //See twiki on how to set this
  std::string Kpipi_reconstruction_name = "Kpipi_reco"; //Used for naming output folder, file and node
  std::string Kpipi_output_reco_file = "output_KFP_Kpipi.root";
  std::string Kpipi_output_dir;

  std::string KKpi_decay_descriptor = "[D_s+ -> K^+ K^- pi^+]cc"; //See twiki on how to set this
  std::string KKpi_reconstruction_name = "KKpi_reco"; //Used for naming output folder, file and node
  std::string KKpi_output_reco_file = "output_KFP_KKpi.root";
  std::string KKpi_output_dir;
  bool KKpi_use_phi_intermediate = false; //Reconstruct D_s+ -> {phi -> K+ K-} pi+ instead of the inclusive K+ K- pi+ (used for simulation)
  std::string KKpi_phi_decay_descriptor = "[D_s+ -> {phi -> K^+ K^-} pi^+]cc";
  float KKpi_phi_intermediate_mass[2] = {0.98, 1.05};

  std::string pKpi_decay_descriptor = "[Lambda_c+ -> proton^+ K^- pi^+]cc"; //See twiki on how to set this
  std::string pKpi_reconstruction_name = "pKpi_reco"; //Used for naming output folder, file and node
  std::string pKpi_output_reco_file = "output_KFP_pKpi.root";
  std::string pKpi_output_dir;

  std::string ee_decay_descriptor = "[gamma -> e^+ e^-]cc"; //See twiki on how to set this
  std::string ee_reconstruction_name = "ee_reco"; //Used for naming output folder, file and node
  std::string ee_output_reco_file = "output_KFP_ee.root";
  std::string ee_output_dir;

  bool doRescale = false;
  bool do_truth_matching = false; //Simulation only: match reconstructed candidates to the truth record
  std::string track_map_name; //Track map the reconstruction reads, empty = KFParticle default. Ignored if doRescale is set
  std::string momReweightTrackMap = "SvtxTrackMapMomentumScaleCorrected";
  bool save_kfpntuple = true;
  bool save_kfpntuple_to_qa = false;
  bool use_pid = false;
  bool save_tracks_to_DST = false;
  bool dont_use_global_vertex = true;
  bool require_track_and_vertex_match = true;
  bool save_all_vtx_info = false;
  bool use_2D_matching = false;
  bool get_trigger_info = false;
  bool get_detector_info = false;
  bool get_dEdx_info = true;
  bool get_calo_info = false;
  float pid_frac = 0.4;
  float cemc_proj_radius = 102.9; //Virgile recommendation according to DetailedCalorimeterGeometry
  bool constrain_lambda_mass = true;
  bool constrain_phi_mass = true;
  bool extrapolate_tracks_to_secondary_vertex = false; // Set to false to ensure the pT map is accurate for asymmetry study
  bool use_ordered_prong_combinations = true;

  //Normally loose: the values here switch a cut off
  float cuts_setMotherPV_DCA{FLT_MAX};
  float cuts_setMotherPV_DCA_XY{FLT_MAX};
  float cuts_setMotherPV_DCA_StdDev{FLT_MAX};
  float cuts_setFlightDistancechi2{-1.};
  float cuts_setMinDIRA{-1.1};
  float cuts_setMinDIRA_XY{-1.1};
  float cuts_setDecayLengthRange_max{FLT_MAX};
  float cuts_setDecayLengthRange_XY_min{-10.};
  float cuts_setDecayLengthRange_XY_max{FLT_MAX};
  float cuts_setDecayTimeRange_XY_min{-10000};
  float cuts_setDecayTimeRange_XY_max{FLT_MAX};
  float cuts_setDecayTimeRange_min{-10000};
  float cuts_setDecayTimeRange_max{FLT_MAX};
  float cuts_setMinDecayTimeSignificance{-1e5};
  float cuts_setMinDecayLengthSignificance{-1e5};
  float cuts_setMinDecayLengthSignificance_XY{-1e5};
  float cuts_setMinimumTrackPT{0.0};
  float cuts_setMinimumTrackPV_DCA{-1.};
  float cuts_setMinimumTrackPV_DCA_StdDev{-1.};
  float cuts_setMinimumTrackPV_DCA_StdDev_XY{-1.};
  float cuts_setMaximumTrackPTchi2{FLT_MAX};
  float cuts_setMaximumTrackchi2nDOF{400.};
  float cuts_setMaximumDaughterDCA_XY{1};
  float cuts_setMotherPT{0};
  float cuts_setMaximumMotherVertexVolume{0.1};

  //Light flavor (K_S0, Lambda, Xi) used. Defaults are the pp values; call set_OO_cuts() or set_AuAu_cuts() for ion data
  float lf_cuts_setMinDIRA = 0.95;
  float lf_cuts_setDecayLengthRange_min{0.5};
  float lf_cuts_setMinimumTrackPV_DCA = -1.;
  float lf_cuts_setMinimumTrackPV_DCA_XY = 0.05;
  int lf_cuts_setMinMVTXhits{0};
  int lf_cuts_setMinINTThits{0};
  int lf_cuts_setMinTPChits{25};
  float lf_cuts_setMaximumVertexchi2nDOF{FLT_MAX};
  float lf_cuts_setMaximumDaughterDCA = 0.1;

  //Heavy flavor used (D0, D+, Ds+, Lambda_c+)
  float hf_track_to_track_DCA = 0.05;
  float hf_mother_PV_DCA = 0.02;
  float hf_daughter_PV_DCA = 0.005;
  float hf_daughter_PV_DCA_XY = 0.005;
  float hf_min_flight_distance = 0.006;
  float hf_max_flight_distance = 0.12;
  float hf_min_dira = 0.97;
  float hf_pT = 1.;
  float hf_max_track_chi2nDOF = 300.;
  float hf_max_vertex_chi2nDOF = FLT_MAX; //SV chi2/nDoF cut rejects signal, keep off
  float hf_max_daughter_DCA_XY = 1.;
  int hf_min_MVTX_hits = 1;
  int hf_min_INTT_hits = 1;
  int hf_min_TPC_hits = 25;

  //Exceptions to the hf_ decay lengths above: these channels do NOT use the shared value
  float Lc_min_flight_distance = 0.004; //Lambda_c+ minimum, 40 um (c*tau is ~60 um)
  float Dp_max_flight_distance = 0.2; //D+ maximum, 2 mm (c*tau is ~312 um)
  float Ds_max_flight_distance = 0.2; //Ds+ maximum, 2 mm (D+ -> K K pi also lands in the Ds+ mass window)

  //Species-dependent light flavor (K_S0, Lambda, Xi) selections.
  //Call at most one of these after including this macro and before any reconstruct_*() function.
  //The defaults above are the pp values, so pp needs no call.
  //OO and AuAu currently share the same values but are kept separate so they can be tuned independently.
  void set_OO_cuts()
  {
    lf_cuts_setMinDIRA = 0.999;
    lf_cuts_setMinimumTrackPV_DCA = 0.025;
    lf_cuts_setMinimumTrackPV_DCA_XY = 0.1;
    lf_cuts_setMaximumDaughterDCA = 0.05;
  }
  
  void set_AuAu_cuts()
  {
    lf_cuts_setMinDIRA = 0.999;
    lf_cuts_setMinimumTrackPV_DCA = 0.025;
    lf_cuts_setMinimumTrackPV_DCA_XY = 0.1;
    lf_cuts_setMaximumDaughterDCA = 0.05;
  }

  void init_kfp_dependencies()
  {
    //dE/dx needs TRKR_CLUSTER and CYLINDERCELLGEOM_SVTX which need to be in the DST or loaded from a geo file
    Fun4AllServer *se = Fun4AllServer::instance();
  
    GlobalVertexReco* gblvertex = new GlobalVertexReco();
    gblvertex->Verbosity(VERBOSITY_KFP);
    se->registerSubsystem(gblvertex);
  
  
    if (get_trigger_info)
    {
      TriggerRunInfoReco *triggerruninforeco = new TriggerRunInfoReco();
      se->registerSubsystem(triggerruninforeco);
    }
  }
  
  void create_hf_directories(const std::string reconstruction_name, std::string &final_output_dir, std::string &output_reco_file)
  {
    std::string output_file_name = kfp_header + reconstruction_name + trailer;
    final_output_dir = output_dir + reconstruction_name + "/";
    std::string output_reco_dir = final_output_dir + processing_folder;
    output_reco_file = output_reco_dir + output_file_name;
  
    std::string makeDirectory = "mkdir -p " + output_reco_dir;
    system(makeDirectory.c_str());
  }
  
  void reconstruct_pipi_mass()
  {
    Fun4AllServer *se = Fun4AllServer::instance();
  
    KFParticle_sPHENIX *kfparticle = new KFParticle_sPHENIX(pipi_reconstruction_name);
    kfparticle->Verbosity(VERBOSITY_KFP);
  
    kfparticle->setDecayDescriptor(pipi_decay_descriptor);
  
    kfparticle->extrapolateTracksToSV(extrapolate_tracks_to_secondary_vertex);
  
    kfparticle->saveOutput(save_kfpntuple);
    kfparticle->saveLightweightNTupleToQA(save_kfpntuple_to_qa);
    if (save_kfpntuple_to_qa) //Still need to enable nTuple writing for QA even if we globally turn off nTuple production (which is the case for central productions)
    {
      kfparticle->saveOutput(true);
    }
  
    kfparticle->usePID(use_pid);
    kfparticle->setPIDacceptFraction(pid_frac);
    kfparticle->get_dEdx_info();
    kfparticle->dontUseGlobalVertex(dont_use_global_vertex);
    kfparticle->requireTrackVertexBunchCrossingMatch(require_track_and_vertex_match);
    kfparticle->getAllPVInfo(save_all_vtx_info);
    kfparticle->allowZeroMassTracks();
    kfparticle->use2Dmatching(use_2D_matching);
    kfparticle->getTriggerInfo(get_trigger_info);
    kfparticle->getDetectorInfo(get_detector_info);
    kfparticle->get_dEdx_info(get_dEdx_info);
    kfparticle->saveDST(save_tracks_to_DST);
    if (getQA) kfparticle->saveDST();
    kfparticle->setContainerName(pipi_reconstruction_name);
    kfparticle->saveParticleContainer(true);
    kfparticle->magFieldFile("FIELDMAP_TRACKING");
    if (doRescale) kfparticle->setTrackMapNodeName(momReweightTrackMap);
    else if (!track_map_name.empty()) kfparticle->setTrackMapNodeName(track_map_name);
    if (do_truth_matching) kfparticle->doTruthMatching(true);
    kfparticle->useOrderedProngCombinations(use_ordered_prong_combinations);
  
    //PV to SV cuts
    kfparticle->constrainToPrimaryVertex();
    kfparticle->setMotherPV_DCA_StdDev(cuts_setMotherPV_DCA_StdDev);
    kfparticle->setFlightDistancechi2(cuts_setFlightDistancechi2);
    kfparticle->setMinDIRA(lf_cuts_setMinDIRA);
    kfparticle->setMinDIRA_XY(cuts_setMinDIRA_XY);
    kfparticle->setDecayLengthRange(lf_cuts_setDecayLengthRange_min, cuts_setDecayLengthRange_max);
    kfparticle->setDecayLengthRange_XY(cuts_setDecayLengthRange_XY_min, cuts_setDecayLengthRange_XY_max);
    kfparticle->setDecayTimeRange_XY(cuts_setDecayTimeRange_XY_min, cuts_setDecayTimeRange_XY_max);
    kfparticle->setDecayTimeRange(cuts_setDecayTimeRange_min, cuts_setDecayTimeRange_max);
    kfparticle->setMinDecayTimeSignificance(cuts_setMinDecayTimeSignificance);
    kfparticle->setMinDecayLengthSignificance(cuts_setMinDecayLengthSignificance);
    kfparticle->setMinDecayLengthSignificance_XY(cuts_setMinDecayLengthSignificance_XY);
  
    //Track parameters
    kfparticle->setMinimumTrackPT(cuts_setMinimumTrackPT);
    kfparticle->setMinimumTrackPV_DCA_StdDev(cuts_setMinimumTrackPV_DCA_StdDev);
    kfparticle->setMinimumTrackPV_DCA_StdDev_XY(cuts_setMinimumTrackPV_DCA_StdDev_XY);
    kfparticle->setMinimumTrackPV_DCA(lf_cuts_setMinimumTrackPV_DCA);
    kfparticle->setMinimumTrackPV_DCA_XY(lf_cuts_setMinimumTrackPV_DCA_XY);
    kfparticle->setMaximumTrackchi2nDOF(cuts_setMaximumTrackchi2nDOF);
    kfparticle->setMinMVTXhits(lf_cuts_setMinMVTXhits);
    kfparticle->setMinINTThits(lf_cuts_setMinINTThits);
    kfparticle->setMinTPChits(lf_cuts_setMinTPChits);
  
    //Vertex parameters
    kfparticle->setMaximumVertexchi2nDOF(lf_cuts_setMaximumVertexchi2nDOF);
    kfparticle->setMaximumDaughterDCA(lf_cuts_setMaximumDaughterDCA);
    kfparticle->setMaximumDaughterDCA_XY(cuts_setMaximumDaughterDCA_XY);
  
    //Parent parameters
    kfparticle->setMotherPT(cuts_setMotherPT);
    kfparticle->setMinimumMass(KS0_mass[0]);
    kfparticle->setMaximumMass(KS0_mass[1]);
    kfparticle->setMaximumMotherVertexVolume(cuts_setMaximumMotherVertexVolume);
    kfparticle->setOutputName(pipi_output_reco_file);
  
    se->registerSubsystem(kfparticle);
  
    if (getQA)
    {
      QAKFParticle *kfpqa = new QAKFParticle("QAKFParticle_K_S0", "K_S0", KS0_mass[0], KS0_mass[1]);
      kfpqa->setKFParticleNodeName(pipi_reconstruction_name);
      se->registerSubsystem(kfpqa);
    }
  }
  
  void reconstruct_KK_mass()
  {
    Fun4AllServer *se = Fun4AllServer::instance();
  
    KFParticle_sPHENIX *kfparticle = new KFParticle_sPHENIX(KK_reconstruction_name);
    kfparticle->Verbosity(VERBOSITY_KFP);
  
    kfparticle->setDecayDescriptor(KK_decay_descriptor);
  
    kfparticle->extrapolateTracksToSV(extrapolate_tracks_to_secondary_vertex);
  
    kfparticle->saveOutput(save_kfpntuple);
  
    kfparticle->usePID(use_pid);
    kfparticle->setPIDacceptFraction(pid_frac);
    kfparticle->get_dEdx_info();
    kfparticle->dontUseGlobalVertex(dont_use_global_vertex);
    kfparticle->requireTrackVertexBunchCrossingMatch(require_track_and_vertex_match);
    kfparticle->getAllPVInfo(save_all_vtx_info);
    kfparticle->allowZeroMassTracks();
    kfparticle->use2Dmatching(use_2D_matching);
    kfparticle->getTriggerInfo(get_trigger_info);
    kfparticle->getDetectorInfo(get_detector_info);
    kfparticle->get_dEdx_info(get_dEdx_info);
    kfparticle->saveDST(save_tracks_to_DST);
    if (getQA) kfparticle->saveDST();
    kfparticle->setContainerName(KK_reconstruction_name);
    kfparticle->saveParticleContainer(true);
    kfparticle->magFieldFile("FIELDMAP_TRACKING");
    if (doRescale) kfparticle->setTrackMapNodeName(momReweightTrackMap);
    else if (!track_map_name.empty()) kfparticle->setTrackMapNodeName(track_map_name);
    if (do_truth_matching) kfparticle->doTruthMatching(true);
    kfparticle->useOrderedProngCombinations(use_ordered_prong_combinations);
  
    //PV to SV cuts
    kfparticle->constrainToPrimaryVertex();
    kfparticle->setMotherPV_DCA_StdDev(100);
    kfparticle->setFlightDistancechi2(cuts_setFlightDistancechi2);
  
    //Track parameters
    kfparticle->setMinimumTrackPT(0.1);
    kfparticle->setMaximumTrackPT(0.7);
    kfparticle->setMaximumTrackchi2nDOF(100.);
    kfparticle->setMinTPChits(25);
    kfparticle->setMinMVTXhits(1);
    kfparticle->setMinINTThits(0);
  
    //Vertex parameters
    kfparticle->setMaximumVertexchi2nDOF(lf_cuts_setMaximumVertexchi2nDOF);
    kfparticle->setMaximumDaughterDCA(0.05);
    kfparticle->setMaximumDaughterDCA_XY(100);
  
    //Parent parameters
    kfparticle->setMotherPT(0);
    kfparticle->setMinimumMass(phi_mass[0]);
    kfparticle->setMaximumMass(phi_mass[1]);
    kfparticle->setMaximumMotherVertexVolume(0.1);
    kfparticle->setOutputName(KK_output_reco_file);
  
    se->registerSubsystem(kfparticle);
  
    if (getQA)
    {
      QAKFParticle *kfpqa = new QAKFParticle("QAKFParticle_phi", "phi", phi_mass[0], phi_mass[1]);
      kfpqa->setKFParticleNodeName(KK_reconstruction_name);
      se->registerSubsystem(kfpqa);
    }
  }
  
  void reconstruct_ppi_mass()
  {
    Fun4AllServer *se = Fun4AllServer::instance();
  
    KFParticle_sPHENIX *kfparticle = new KFParticle_sPHENIX(ppi_reconstruction_name);
    kfparticle->Verbosity(VERBOSITY_KFP);
  
    kfparticle->setDecayDescriptor(ppi_decay_descriptor);
  
    kfparticle->extrapolateTracksToSV(extrapolate_tracks_to_secondary_vertex);
  
    kfparticle->saveOutput(save_kfpntuple);
    kfparticle->saveLightweightNTupleToQA(save_kfpntuple_to_qa);
    if (save_kfpntuple_to_qa) //Still need to enable nTuple writing for QA even if we globally turn off nTuple production (which is the case for central productions)
    {
      kfparticle->saveOutput(true);
    }
  
    kfparticle->usePID(use_pid);
    kfparticle->setPIDacceptFraction(pid_frac);
    kfparticle->get_dEdx_info();
    kfparticle->dontUseGlobalVertex(dont_use_global_vertex);
    kfparticle->requireTrackVertexBunchCrossingMatch(require_track_and_vertex_match);
    kfparticle->getAllPVInfo(save_all_vtx_info);
    kfparticle->allowZeroMassTracks();
    kfparticle->use2Dmatching(use_2D_matching);
    kfparticle->getTriggerInfo(get_trigger_info);
    kfparticle->getDetectorInfo(get_detector_info);
    kfparticle->get_dEdx_info(get_dEdx_info);
    kfparticle->saveDST(save_tracks_to_DST);
    if (getQA) kfparticle->saveDST();
    kfparticle->setContainerName(ppi_reconstruction_name);
    kfparticle->saveParticleContainer(true);
    kfparticle->magFieldFile("FIELDMAP_TRACKING");
    if (doRescale) kfparticle->setTrackMapNodeName(momReweightTrackMap);
    else if (!track_map_name.empty()) kfparticle->setTrackMapNodeName(track_map_name);
    if (do_truth_matching) kfparticle->doTruthMatching(true);
    kfparticle->useOrderedProngCombinations(use_ordered_prong_combinations);
  
    //PV to SV cuts
    kfparticle->constrainToPrimaryVertex();
    kfparticle->setMotherPV_DCA_StdDev(cuts_setMotherPV_DCA_StdDev);
    kfparticle->setFlightDistancechi2(cuts_setFlightDistancechi2);
    kfparticle->setMinDIRA(lf_cuts_setMinDIRA);
    kfparticle->setMinDIRA_XY(cuts_setMinDIRA_XY);
    kfparticle->setDecayLengthRange(lf_cuts_setDecayLengthRange_min, cuts_setDecayLengthRange_max);
    kfparticle->setDecayLengthRange_XY(cuts_setDecayLengthRange_XY_min, cuts_setDecayLengthRange_XY_max);
    kfparticle->setDecayTimeRange_XY(cuts_setDecayTimeRange_XY_min, cuts_setDecayTimeRange_XY_max);
    kfparticle->setDecayTimeRange(cuts_setDecayTimeRange_min, cuts_setDecayTimeRange_max);
    kfparticle->setMinDecayTimeSignificance(cuts_setMinDecayTimeSignificance);
    kfparticle->setMinDecayLengthSignificance(cuts_setMinDecayLengthSignificance);
    kfparticle->setMinDecayLengthSignificance_XY(cuts_setMinDecayLengthSignificance_XY);
  
    //Track parameters
    kfparticle->setMinimumTrackPT(cuts_setMinimumTrackPT);
    kfparticle->setMinimumTrackPV_DCA_StdDev(cuts_setMinimumTrackPV_DCA_StdDev);
    kfparticle->setMinimumTrackPV_DCA_StdDev_XY(cuts_setMinimumTrackPV_DCA_StdDev_XY);
    kfparticle->setMinimumTrackPV_DCA(lf_cuts_setMinimumTrackPV_DCA);
    kfparticle->setMinimumTrackPV_DCA_XY(lf_cuts_setMinimumTrackPV_DCA_XY);
    kfparticle->setMaximumTrackchi2nDOF(cuts_setMaximumTrackchi2nDOF);
    kfparticle->setMinMVTXhits(lf_cuts_setMinMVTXhits);
    kfparticle->setMinINTThits(lf_cuts_setMinINTThits);
    kfparticle->setMinTPChits(lf_cuts_setMinTPChits);
  
    //Vertex parameters
    kfparticle->setMaximumVertexchi2nDOF(lf_cuts_setMaximumVertexchi2nDOF);
    kfparticle->setMaximumDaughterDCA(lf_cuts_setMaximumDaughterDCA);
    kfparticle->setMaximumDaughterDCA_XY(cuts_setMaximumDaughterDCA_XY);
  
    //Parent parameters
    kfparticle->setMotherPT(cuts_setMotherPT);
    kfparticle->setMinimumMass(L0_mass[0]);
    kfparticle->setMaximumMass(L0_mass[1]);
    kfparticle->setMaximumMotherVertexVolume(cuts_setMaximumMotherVertexVolume);
    kfparticle->setOutputName(ppi_output_reco_file);
  
    se->registerSubsystem(kfparticle);
    
    if (getQA)
    {
      QAKFParticle *kfpqa = new QAKFParticle("QAKFParticle_Lambda0", "Lambda0", L0_mass[0], L0_mass[1]);
      kfpqa->setKFParticleNodeName(ppi_reconstruction_name);
      se->registerSubsystem(kfpqa);
    }
  }
  
  void reconstruct_Kpi_mass()
  {
    Fun4AllServer *se = Fun4AllServer::instance();
  
    KFParticle_sPHENIX *kfparticle = new KFParticle_sPHENIX(Kpi_reconstruction_name);
    kfparticle->Verbosity(VERBOSITY_KFP);
  
    kfparticle->setDecayDescriptor(Kpi_decay_descriptor);
  
    kfparticle->extrapolateTracksToSV(extrapolate_tracks_to_secondary_vertex);
  
    kfparticle->saveOutput(save_kfpntuple);
  
    kfparticle->usePID(use_pid);
    kfparticle->setPIDacceptFraction(pid_frac);
    kfparticle->get_dEdx_info();
    kfparticle->dontUseGlobalVertex(dont_use_global_vertex);
    kfparticle->requireTrackVertexBunchCrossingMatch(require_track_and_vertex_match);
    kfparticle->getAllPVInfo(save_all_vtx_info);
    kfparticle->allowZeroMassTracks();
    kfparticle->use2Dmatching(use_2D_matching);
    kfparticle->getTriggerInfo(get_trigger_info);
    kfparticle->getDetectorInfo(get_detector_info);
    kfparticle->get_dEdx_info(get_dEdx_info);
    kfparticle->saveDST(save_tracks_to_DST);
    if (getQA) kfparticle->saveDST();
    kfparticle->setContainerName(Kpi_reconstruction_name);
    kfparticle->saveParticleContainer(true);
    kfparticle->magFieldFile("FIELDMAP_TRACKING");
    if (doRescale) kfparticle->setTrackMapNodeName(momReweightTrackMap);
    else if (!track_map_name.empty()) kfparticle->setTrackMapNodeName(track_map_name);
    if (do_truth_matching) kfparticle->doTruthMatching(true);
    kfparticle->useOrderedProngCombinations(use_ordered_prong_combinations);
  
    //PV to SV cuts
    kfparticle->constrainToPrimaryVertex();
    kfparticle->setMotherPV_DCA_StdDev(cuts_setMotherPV_DCA_StdDev);
    kfparticle->setMotherPV_DCA(hf_mother_PV_DCA);
    kfparticle->setMotherPV_DCA_XY(cuts_setMotherPV_DCA_XY);
    kfparticle->setFlightDistancechi2(cuts_setFlightDistancechi2);
    kfparticle->setMinDIRA(hf_min_dira);
    kfparticle->setMinDIRA_XY(cuts_setMinDIRA_XY);
    kfparticle->setDecayLengthRange(hf_min_flight_distance, hf_max_flight_distance);
    kfparticle->setDecayLengthRange_XY(cuts_setDecayLengthRange_XY_min, cuts_setDecayLengthRange_XY_max);
    kfparticle->setDecayTimeRange_XY(cuts_setDecayTimeRange_XY_min, cuts_setDecayTimeRange_XY_max);
    kfparticle->setDecayTimeRange(cuts_setDecayTimeRange_min, cuts_setDecayTimeRange_max);
    kfparticle->setMinDecayTimeSignificance(cuts_setMinDecayTimeSignificance);
    kfparticle->setMinDecayLengthSignificance(cuts_setMinDecayLengthSignificance);
    kfparticle->setMinDecayLengthSignificance_XY(cuts_setMinDecayLengthSignificance_XY);
  
    //Track parameters
    kfparticle->setMinimumTrackPT(cuts_setMinimumTrackPT);
    kfparticle->setMinimumTrackPV_DCA_StdDev(cuts_setMinimumTrackPV_DCA_StdDev);
    kfparticle->setMinimumTrackPV_DCA_StdDev_XY(cuts_setMinimumTrackPV_DCA_StdDev_XY);
    kfparticle->setMinimumTrackPV_DCA(hf_daughter_PV_DCA);
    kfparticle->setMinimumTrackPV_DCA_XY(hf_daughter_PV_DCA_XY);
    kfparticle->setMaximumTrackchi2nDOF(hf_max_track_chi2nDOF);
    kfparticle->setMinMVTXhits(hf_min_MVTX_hits);
    kfparticle->setMinINTThits(hf_min_INTT_hits);
    kfparticle->setMinTPChits(hf_min_TPC_hits);
  
    //Vertex parameters
    kfparticle->setMaximumVertexchi2nDOF(hf_max_vertex_chi2nDOF); //SV chi2/nDoF cut rejects signal, keep off
    kfparticle->setMaximumDaughterDCA(hf_track_to_track_DCA);
    kfparticle->setMaximumDaughterDCA_XY(hf_max_daughter_DCA_XY);
  
    //Parent parameters
    kfparticle->setMotherPT(hf_pT);
    kfparticle->setMinimumMass(D0_mass[0]);
    kfparticle->setMaximumMass(D0_mass[1]);
    kfparticle->setMaximumMotherVertexVolume(0.1);
    kfparticle->setOutputName(Kpi_output_reco_file);
  
    se->registerSubsystem(kfparticle);
  
    if (getQA)
    {
      QAKFParticle *kfpqa = new QAKFParticle("QAKFParticle_D0", "D0", D0_mass[0], D0_mass[1]);
      kfpqa->setKFParticleNodeName(Kpi_reconstruction_name);
      se->registerSubsystem(kfpqa);
    }
  }
  
  void reconstruct_Kpipi_mass()
  {
    Fun4AllServer *se = Fun4AllServer::instance();
  
    KFParticle_sPHENIX *kfparticle = new KFParticle_sPHENIX(Kpipi_reconstruction_name);
    kfparticle->Verbosity(VERBOSITY_KFP);
  
    kfparticle->setDecayDescriptor(Kpipi_decay_descriptor);
  
    kfparticle->extrapolateTracksToSV(extrapolate_tracks_to_secondary_vertex);
  
    kfparticle->saveOutput(save_kfpntuple);
  
    kfparticle->usePID(use_pid);
    kfparticle->setPIDacceptFraction(pid_frac);
    kfparticle->get_dEdx_info();
    kfparticle->dontUseGlobalVertex(dont_use_global_vertex);
    kfparticle->requireTrackVertexBunchCrossingMatch(require_track_and_vertex_match);
    kfparticle->getAllPVInfo(save_all_vtx_info);
    kfparticle->allowZeroMassTracks();
    kfparticle->use2Dmatching(use_2D_matching);
    kfparticle->getTriggerInfo(get_trigger_info);
    kfparticle->getDetectorInfo(get_detector_info);
    kfparticle->get_dEdx_info(get_dEdx_info);
    kfparticle->saveDST(save_tracks_to_DST);
    if (getQA) kfparticle->saveDST();
    kfparticle->setContainerName(Kpipi_reconstruction_name);
    kfparticle->saveParticleContainer(true);
    kfparticle->magFieldFile("FIELDMAP_TRACKING");
    if (doRescale) kfparticle->setTrackMapNodeName(momReweightTrackMap);
    else if (!track_map_name.empty()) kfparticle->setTrackMapNodeName(track_map_name);
    if (do_truth_matching) kfparticle->doTruthMatching(true);
    kfparticle->useOrderedProngCombinations(use_ordered_prong_combinations);
  
    //PV to SV cuts
    kfparticle->constrainToPrimaryVertex();
    kfparticle->setMotherPV_DCA_StdDev(cuts_setMotherPV_DCA_StdDev);
    kfparticle->setMotherPV_DCA(hf_mother_PV_DCA);
    kfparticle->setMotherPV_DCA_XY(cuts_setMotherPV_DCA_XY);
    kfparticle->setFlightDistancechi2(cuts_setFlightDistancechi2);
    kfparticle->setMinDIRA(hf_min_dira);
    kfparticle->setMinDIRA_XY(cuts_setMinDIRA_XY);
    kfparticle->setDecayLengthRange(hf_min_flight_distance, Dp_max_flight_distance); //Dp_max_flight_distance, not hf_max_flight_distance
    kfparticle->setDecayLengthRange_XY(cuts_setDecayLengthRange_XY_min, cuts_setDecayLengthRange_XY_max);
    kfparticle->setDecayTimeRange_XY(cuts_setDecayTimeRange_XY_min, cuts_setDecayTimeRange_XY_max);
    kfparticle->setDecayTimeRange(cuts_setDecayTimeRange_min, cuts_setDecayTimeRange_max);
    kfparticle->setMinDecayTimeSignificance(cuts_setMinDecayTimeSignificance);
    kfparticle->setMinDecayLengthSignificance(cuts_setMinDecayLengthSignificance);
    kfparticle->setMinDecayLengthSignificance_XY(cuts_setMinDecayLengthSignificance_XY);
  
    //Track parameters
    kfparticle->setMinimumTrackPT(cuts_setMinimumTrackPT);
    kfparticle->setMinimumTrackPV_DCA_StdDev(cuts_setMinimumTrackPV_DCA_StdDev);
    kfparticle->setMinimumTrackPV_DCA_StdDev_XY(cuts_setMinimumTrackPV_DCA_StdDev_XY);
    kfparticle->setMinimumTrackPV_DCA(hf_daughter_PV_DCA);
    kfparticle->setMinimumTrackPV_DCA_XY(hf_daughter_PV_DCA_XY);
    kfparticle->setMaximumTrackchi2nDOF(hf_max_track_chi2nDOF);
    kfparticle->setMinMVTXhits(hf_min_MVTX_hits);
    kfparticle->setMinINTThits(hf_min_INTT_hits);
    kfparticle->setMinTPChits(hf_min_TPC_hits);
  
    //Vertex parameters
    kfparticle->setMaximumVertexchi2nDOF(hf_max_vertex_chi2nDOF); //SV chi2/nDoF cut rejects signal, keep off
    kfparticle->setMaximumDaughterDCA(hf_track_to_track_DCA);
    kfparticle->setMaximumDaughterDCA_XY(hf_max_daughter_DCA_XY);
  
    //Parent parameters
    kfparticle->setMotherPT(hf_pT);
    kfparticle->setMinimumMass(Dp_mass[0]);
    kfparticle->setMaximumMass(Dp_mass[1]);
    kfparticle->setMaximumMotherVertexVolume(0.1);
    kfparticle->setOutputName(Kpipi_output_reco_file);
  
    se->registerSubsystem(kfparticle);
  
    if (getQA)
    {
      QAKFParticle *kfpqa = new QAKFParticle("QAKFParticle_Dplus", "D+", Dp_mass[0], Dp_mass[1]);
      kfpqa->setKFParticleNodeName(Kpipi_reconstruction_name);
      se->registerSubsystem(kfpqa);
    }
  }
  
  void reconstruct_KKpi_mass()
  {
    Fun4AllServer *se = Fun4AllServer::instance();
  
    KFParticle_sPHENIX *kfparticle = new KFParticle_sPHENIX(KKpi_reconstruction_name);
    kfparticle->Verbosity(VERBOSITY_KFP);
  
    kfparticle->setDecayDescriptor(KKpi_use_phi_intermediate ? KKpi_phi_decay_descriptor : KKpi_decay_descriptor);
  
    kfparticle->extrapolateTracksToSV(extrapolate_tracks_to_secondary_vertex);
  
    kfparticle->saveOutput(save_kfpntuple);
  
    kfparticle->usePID(use_pid);
    kfparticle->setPIDacceptFraction(pid_frac);
    kfparticle->get_dEdx_info();
    kfparticle->dontUseGlobalVertex(dont_use_global_vertex);
    kfparticle->requireTrackVertexBunchCrossingMatch(require_track_and_vertex_match);
    kfparticle->getAllPVInfo(save_all_vtx_info);
    kfparticle->allowZeroMassTracks();
    kfparticle->use2Dmatching(use_2D_matching);
    kfparticle->getTriggerInfo(get_trigger_info);
    kfparticle->getDetectorInfo(get_detector_info);
    kfparticle->get_dEdx_info(get_dEdx_info);
    kfparticle->saveDST(save_tracks_to_DST);
    kfparticle->setContainerName(KKpi_reconstruction_name);
    kfparticle->saveParticleContainer(true);
    kfparticle->magFieldFile("FIELDMAP_TRACKING");
    if (doRescale) kfparticle->setTrackMapNodeName(momReweightTrackMap);
    else if (!track_map_name.empty()) kfparticle->setTrackMapNodeName(track_map_name);
    if (do_truth_matching) kfparticle->doTruthMatching(true);
    kfparticle->useOrderedProngCombinations(use_ordered_prong_combinations);
  
    //PV to SV cuts
    kfparticle->constrainToPrimaryVertex();
    kfparticle->setMotherPV_DCA_StdDev(cuts_setMotherPV_DCA_StdDev);
    kfparticle->setMotherPV_DCA(hf_mother_PV_DCA);
    kfparticle->setMotherPV_DCA_XY(cuts_setMotherPV_DCA_XY);
    kfparticle->setFlightDistancechi2(cuts_setFlightDistancechi2);
    kfparticle->setMinDIRA(hf_min_dira);
    kfparticle->setMinDIRA_XY(cuts_setMinDIRA_XY);
    kfparticle->setDecayLengthRange(hf_min_flight_distance, Ds_max_flight_distance); //Ds_max_flight_distance, not hf_max_flight_distance
    kfparticle->setDecayLengthRange_XY(cuts_setDecayLengthRange_XY_min, cuts_setDecayLengthRange_XY_max);
    kfparticle->setDecayTimeRange_XY(cuts_setDecayTimeRange_XY_min, cuts_setDecayTimeRange_XY_max);
    kfparticle->setDecayTimeRange(cuts_setDecayTimeRange_min, cuts_setDecayTimeRange_max);
    kfparticle->setMinDecayTimeSignificance(cuts_setMinDecayTimeSignificance);
    kfparticle->setMinDecayLengthSignificance(cuts_setMinDecayLengthSignificance);
    kfparticle->setMinDecayLengthSignificance_XY(cuts_setMinDecayLengthSignificance_XY);
  
    //Track parameters
    kfparticle->setMinimumTrackPT(cuts_setMinimumTrackPT);
    kfparticle->setMinimumTrackPV_DCA_StdDev(cuts_setMinimumTrackPV_DCA_StdDev);
    kfparticle->setMinimumTrackPV_DCA_StdDev_XY(cuts_setMinimumTrackPV_DCA_StdDev_XY);
    kfparticle->setMinimumTrackPV_DCA(hf_daughter_PV_DCA);
    kfparticle->setMinimumTrackPV_DCA_XY(hf_daughter_PV_DCA_XY);
    kfparticle->setMaximumTrackchi2nDOF(hf_max_track_chi2nDOF);
    kfparticle->setMinMVTXhits(hf_min_MVTX_hits);
    kfparticle->setMinINTThits(hf_min_INTT_hits);
    kfparticle->setMinTPChits(hf_min_TPC_hits);
  
    //Vertex parameters
    kfparticle->setMaximumVertexchi2nDOF(hf_max_vertex_chi2nDOF); //SV chi2/nDoF cut rejects signal, keep off
    kfparticle->setMaximumDaughterDCA(hf_track_to_track_DCA);
    kfparticle->setMaximumDaughterDCA_XY(hf_max_daughter_DCA_XY);
  
    //Parent parameters
    kfparticle->setMotherPT(hf_pT);
    kfparticle->setMinimumMass(Ds_mass[0]);
    kfparticle->setMaximumMass(Ds_mass[1]);
    kfparticle->setMaximumMotherVertexVolume(0.1);
  
    if (KKpi_use_phi_intermediate)
    {
      kfparticle->constrainIntermediateMasses(constrain_phi_mass);
      kfparticle->setIntermediateMassRange({{KKpi_phi_intermediate_mass[0], KKpi_phi_intermediate_mass[1]}});
      kfparticle->setIntermediateMinPT({0.});
      kfparticle->setIntermediatePV_DCARange({{-1., FLT_MAX}});
      kfparticle->setIntermediatePV_DCARange_XY({{-1., FLT_MAX}});
      kfparticle->setIntermediatePV_DCA_StdDevRange({{-1e5, 1e5}});
      kfparticle->setIntermediatePV_DCA_StdDevRange_XY({{-1e5, 1e5}});
      kfparticle->setIntermediateMinDIRA({-1.1});
      kfparticle->setIntermediateMinFDchi2({-1.});
      kfparticle->setIntermediateMaxVertexVolume({0.1});
    }
  
    kfparticle->setOutputName(KKpi_output_reco_file);
  
    se->registerSubsystem(kfparticle);
  }
  
  void reconstruct_pKpi_mass()
  {
    Fun4AllServer *se = Fun4AllServer::instance();
  
    KFParticle_sPHENIX *kfparticle = new KFParticle_sPHENIX(pKpi_reconstruction_name);
    kfparticle->Verbosity(VERBOSITY_KFP);
  
    kfparticle->setDecayDescriptor(pKpi_decay_descriptor);
  
    kfparticle->extrapolateTracksToSV(extrapolate_tracks_to_secondary_vertex);
  
    kfparticle->saveOutput(save_kfpntuple);
  
    kfparticle->usePID(use_pid);
    kfparticle->setPIDacceptFraction(pid_frac);
    kfparticle->get_dEdx_info();
    kfparticle->dontUseGlobalVertex(dont_use_global_vertex);
    kfparticle->requireTrackVertexBunchCrossingMatch(require_track_and_vertex_match);
    kfparticle->getAllPVInfo(save_all_vtx_info);
    kfparticle->allowZeroMassTracks();
    kfparticle->use2Dmatching(use_2D_matching);
    kfparticle->getTriggerInfo(get_trigger_info);
    kfparticle->getDetectorInfo(get_detector_info);
    kfparticle->get_dEdx_info(get_dEdx_info);
    kfparticle->saveDST(save_tracks_to_DST);
    kfparticle->setContainerName(pKpi_reconstruction_name);
    kfparticle->saveParticleContainer(true);
    kfparticle->magFieldFile("FIELDMAP_TRACKING");
    if (doRescale) kfparticle->setTrackMapNodeName(momReweightTrackMap);
    else if (!track_map_name.empty()) kfparticle->setTrackMapNodeName(track_map_name);
    if (do_truth_matching) kfparticle->doTruthMatching(true);
    kfparticle->useOrderedProngCombinations(use_ordered_prong_combinations);
  
    //PV to SV cuts
    kfparticle->constrainToPrimaryVertex();
    kfparticle->setMotherPV_DCA_StdDev(cuts_setMotherPV_DCA_StdDev);
    kfparticle->setMotherPV_DCA(hf_mother_PV_DCA);
    kfparticle->setMotherPV_DCA_XY(cuts_setMotherPV_DCA_XY);
    kfparticle->setFlightDistancechi2(cuts_setFlightDistancechi2);
    kfparticle->setMinDIRA(hf_min_dira);
    kfparticle->setMinDIRA_XY(cuts_setMinDIRA_XY);
    kfparticle->setDecayLengthRange(Lc_min_flight_distance, hf_max_flight_distance); //Lc_min_flight_distance, not hf_min_flight_distance
    kfparticle->setDecayLengthRange_XY(cuts_setDecayLengthRange_XY_min, cuts_setDecayLengthRange_XY_max);
    kfparticle->setDecayTimeRange_XY(cuts_setDecayTimeRange_XY_min, cuts_setDecayTimeRange_XY_max);
    kfparticle->setDecayTimeRange(cuts_setDecayTimeRange_min, cuts_setDecayTimeRange_max);
    kfparticle->setMinDecayTimeSignificance(cuts_setMinDecayTimeSignificance);
    kfparticle->setMinDecayLengthSignificance(cuts_setMinDecayLengthSignificance);
    kfparticle->setMinDecayLengthSignificance_XY(cuts_setMinDecayLengthSignificance_XY);
  
    //Track parameters
    kfparticle->setMinimumTrackPT(cuts_setMinimumTrackPT);
    kfparticle->setMinimumTrackPV_DCA_StdDev(cuts_setMinimumTrackPV_DCA_StdDev);
    kfparticle->setMinimumTrackPV_DCA_StdDev_XY(cuts_setMinimumTrackPV_DCA_StdDev_XY);
    kfparticle->setMinimumTrackPV_DCA(hf_daughter_PV_DCA);
    kfparticle->setMinimumTrackPV_DCA_XY(hf_daughter_PV_DCA_XY);
    kfparticle->setMaximumTrackchi2nDOF(hf_max_track_chi2nDOF);
    kfparticle->setMinMVTXhits(hf_min_MVTX_hits);
    kfparticle->setMinINTThits(hf_min_INTT_hits);
    kfparticle->setMinTPChits(hf_min_TPC_hits);
  
    //Vertex parameters
    kfparticle->setMaximumVertexchi2nDOF(hf_max_vertex_chi2nDOF); //SV chi2/nDoF cut rejects signal, keep off
    kfparticle->setMaximumDaughterDCA(hf_track_to_track_DCA);
    kfparticle->setMaximumDaughterDCA_XY(hf_max_daughter_DCA_XY);
  
    //Parent parameters
    kfparticle->setMotherPT(hf_pT);
    kfparticle->setMinimumMass(Lc_mass[0]);
    kfparticle->setMaximumMass(Lc_mass[1]);
    kfparticle->setMaximumMotherVertexVolume(0.1);
    kfparticle->setOutputName(pKpi_output_reco_file);
  
    se->registerSubsystem(kfparticle);
  }
  
  void reconstruct_ee_mass()
  {
    Fun4AllServer *se = Fun4AllServer::instance();
  
    KFParticle_sPHENIX *kfparticle = new KFParticle_sPHENIX(ee_reconstruction_name);
  
    kfparticle->setDecayDescriptor(ee_decay_descriptor);
  
    kfparticle->extrapolateTracksToSV(extrapolate_tracks_to_secondary_vertex);
  
    kfparticle->saveOutput(save_kfpntuple);
  
    kfparticle->usePID(use_pid);
    kfparticle->setPIDacceptFraction(pid_frac);
    kfparticle->dontUseGlobalVertex(dont_use_global_vertex);
    kfparticle->requireTrackVertexBunchCrossingMatch(require_track_and_vertex_match);
    kfparticle->getAllPVInfo(save_all_vtx_info);
    kfparticle->allowZeroMassTracks();
    kfparticle->use2Dmatching(use_2D_matching);
    kfparticle->getTriggerInfo(get_trigger_info);
    kfparticle->getDetectorInfo(get_detector_info);
    kfparticle->get_dEdx_info(get_dEdx_info);
    kfparticle->getCaloInfo(get_calo_info);
    kfparticle->saveDST(save_tracks_to_DST);
    kfparticle->setContainerName(ee_reconstruction_name);
    kfparticle->saveParticleContainer(true);
    kfparticle->magFieldFile("FIELDMAP_TRACKING");
    if (doRescale) kfparticle->setTrackMapNodeName(momReweightTrackMap);
    else if (!track_map_name.empty()) kfparticle->setTrackMapNodeName(track_map_name);
    if (do_truth_matching) kfparticle->doTruthMatching(true);
    kfparticle->useOrderedProngCombinations(use_ordered_prong_combinations);
  
    //PV to SV cuts
    kfparticle->constrainToPrimaryVertex();
    kfparticle->setMotherPV_DCA_StdDev(cuts_setMotherPV_DCA_StdDev);
    kfparticle->setFlightDistancechi2(cuts_setFlightDistancechi2);
    kfparticle->setMinDIRA(cuts_setMinDIRA);
    kfparticle->setMinDIRA_XY(cuts_setMinDIRA_XY);
    kfparticle->setDecayLengthRange(0., cuts_setDecayLengthRange_max);
    kfparticle->setDecayLengthRange_XY(-10000, cuts_setDecayLengthRange_XY_max);
    kfparticle->setDecayTimeRange_XY(cuts_setDecayTimeRange_XY_min, cuts_setDecayTimeRange_XY_max);
    kfparticle->setDecayTimeRange(cuts_setDecayTimeRange_min, cuts_setDecayTimeRange_max);
    kfparticle->setMinDecayTimeSignificance(cuts_setMinDecayTimeSignificance);
    kfparticle->setMinDecayLengthSignificance(cuts_setMinDecayLengthSignificance);
    kfparticle->setMinDecayLengthSignificance_XY(cuts_setMinDecayLengthSignificance_XY);
  
    //Track parameters
    kfparticle->bunchCrossingZeroOnly(true);
    kfparticle->setMinimumTrackPT(0.2);
    kfparticle->setMaximumTrackPTchi2(cuts_setMaximumTrackPTchi2);
    kfparticle->setMinimumTrackPV_DCA_StdDev(cuts_setMinimumTrackPV_DCA_StdDev);
    kfparticle->setMinimumTrackPV_DCA_StdDev_XY(cuts_setMinimumTrackPV_DCA_StdDev_XY);
    kfparticle->setMinimumTrackPV_DCA(cuts_setMinimumTrackPV_DCA);
    kfparticle->setMinimumTrackPV_DCA_XY(-100.);
    kfparticle->setMaximumTrackchi2nDOF(300.);
    kfparticle->setMinMVTXhits(0);
    kfparticle->setMinINTThits(0);
    kfparticle->setMinTPChits(25);
  
    //Track-Calo matching
    kfparticle->set_emcal_radius_user(cemc_proj_radius);
    //narrow window
    kfparticle->set_dphi_cut_low(-0.02); //rad
    kfparticle->set_dphi_cut_high(0.09); //rad
    kfparticle->set_dz_cut_low(-4); //cm
    kfparticle->set_dz_cut_high(4); //cm
    kfparticle->set_emcal_e_low_cut(0.2); //GeV
    kfparticle->requireTrackEMCalMatch(true);
  
    //Vertex parameters
    kfparticle->setMaximumVertexchi2nDOF(FLT_MAX);
    kfparticle->setMaximumDaughterDCA(0.5);
    kfparticle->setMaximumDaughterDCA_XY(100);
  
    //Parent parameters
    kfparticle->setMotherPT(0);
    kfparticle->setMinimumMass(ee_mass[0]);
    kfparticle->setMaximumMass(ee_mass[1]);
    kfparticle->setMaximumMotherVertexVolume(0.1);
  
    kfparticle->setOutputName(ee_output_reco_file);
  
    se->registerSubsystem(kfparticle);
  }
  
  //adding xi and omega
  void reconstruct_Lambdapi_mass()
  {
    Fun4AllServer *se = Fun4AllServer::instance();
  
    KFParticle_sPHENIX *kfparticle = new KFParticle_sPHENIX(Lambdapi_reconstruction_name);
    kfparticle->Verbosity(VERBOSITY_KFP);
  
    kfparticle->setDecayDescriptor(Lambdapi_decay_descriptor);
  
    kfparticle->extrapolateTracksToSV(extrapolate_tracks_to_secondary_vertex);
  
    kfparticle->saveOutput(save_kfpntuple);
  
    kfparticle->usePID(use_pid);
    kfparticle->setPIDacceptFraction(pid_frac);
    kfparticle->get_dEdx_info();
    kfparticle->dontUseGlobalVertex(dont_use_global_vertex);
    kfparticle->requireTrackVertexBunchCrossingMatch(require_track_and_vertex_match);
    kfparticle->getAllPVInfo(save_all_vtx_info);
    kfparticle->allowZeroMassTracks();
    kfparticle->use2Dmatching(use_2D_matching);
    kfparticle->getTriggerInfo(get_trigger_info);
    kfparticle->getDetectorInfo(get_detector_info);
    kfparticle->get_dEdx_info(get_dEdx_info);
    kfparticle->saveDST(save_tracks_to_DST);
    kfparticle->setContainerName(Lambdapi_reconstruction_name);
    kfparticle->saveParticleContainer(true);
    kfparticle->magFieldFile("FIELDMAP_TRACKING");
    if (doRescale) kfparticle->setTrackMapNodeName(momReweightTrackMap);
    else if (!track_map_name.empty()) kfparticle->setTrackMapNodeName(track_map_name);
    if (do_truth_matching) kfparticle->doTruthMatching(true);
    kfparticle->useOrderedProngCombinations(use_ordered_prong_combinations);
  
    //PV to SV cuts
    kfparticle->constrainToPrimaryVertex();
    kfparticle->setMotherPV_DCA_StdDev(cuts_setMotherPV_DCA_StdDev);
    kfparticle->setFlightDistancechi2(cuts_setFlightDistancechi2);
    kfparticle->setMinDIRA(lf_cuts_setMinDIRA);
    kfparticle->setMinDIRA_XY(cuts_setMinDIRA_XY);
    kfparticle->setDecayLengthRange(0.15, cuts_setDecayLengthRange_max);
    kfparticle->setDecayLengthRange_XY(cuts_setDecayLengthRange_XY_min, cuts_setDecayLengthRange_XY_max);
    kfparticle->setDecayTimeRange_XY(cuts_setDecayTimeRange_XY_min, cuts_setDecayTimeRange_XY_max);
    kfparticle->setDecayTimeRange(cuts_setDecayTimeRange_min, cuts_setDecayTimeRange_max);
    kfparticle->setMinDecayTimeSignificance(cuts_setMinDecayTimeSignificance);
    kfparticle->setMinDecayLengthSignificance(cuts_setMinDecayLengthSignificance);
    kfparticle->setMinDecayLengthSignificance_XY(cuts_setMinDecayLengthSignificance_XY);
  
    //Track parameters
    kfparticle->setMinimumTrackPT(cuts_setMinimumTrackPT);
    kfparticle->setMinimumTrackPV_DCA_StdDev(cuts_setMinimumTrackPV_DCA_StdDev);
    kfparticle->setMinimumTrackPV_DCA_StdDev_XY(cuts_setMinimumTrackPV_DCA_StdDev_XY);
    kfparticle->setMinimumTrackPV_DCA(cuts_setMinimumTrackPV_DCA);
    kfparticle->setMinimumTrackPV_DCA_XY(lf_cuts_setMinimumTrackPV_DCA_XY);
    kfparticle->setMaximumTrackchi2nDOF(cuts_setMaximumTrackchi2nDOF);
    kfparticle->setMinMVTXhits(lf_cuts_setMinMVTXhits);
    kfparticle->setMinINTThits(lf_cuts_setMinINTThits);
    kfparticle->setMinTPChits(lf_cuts_setMinTPChits);
  
    //Vertex parameters
    kfparticle->setMaximumVertexchi2nDOF(lf_cuts_setMaximumVertexchi2nDOF);
    kfparticle->setMaximumDaughterDCA(lf_cuts_setMaximumDaughterDCA);
    kfparticle->setMaximumDaughterDCA_XY(cuts_setMaximumDaughterDCA_XY);
  
    //Parent parameters
    kfparticle->setMotherPT(cuts_setMotherPT);
    kfparticle->setMinimumMass(Xi_mass[0]);
    kfparticle->setMaximumMass(Xi_mass[1]);
    kfparticle->setMaximumMotherVertexVolume(0.1);
  
    //Intermediate parameters
    kfparticle->constrainIntermediateMasses(constrain_lambda_mass);
  
    std::vector<std::pair<float, float>> intermediate_mass_range;
    intermediate_mass_range.emplace_back(std::make_pair(Xi_L0_intermediate_mass[0], Xi_L0_intermediate_mass[1]));
    kfparticle->setIntermediateMassRange(intermediate_mass_range);
  
    std::vector<float> intermediate_min_pt = {0.0};
    kfparticle->setIntermediateMinPT(intermediate_min_pt);
  
    std::vector<std::pair<float, float>> intermediate_PV_DCA_range;
    intermediate_PV_DCA_range.emplace_back(std::make_pair(-1., FLT_MAX));
    kfparticle->setIntermediatePV_DCARange(intermediate_PV_DCA_range);
  
    std::vector<std::pair<float, float>> intermediate_PV_DCAchi2_range;
    intermediate_PV_DCAchi2_range.emplace_back(std::make_pair(0., 400.));
    kfparticle->setIntermediatePV_DCA_StdDevRange(intermediate_PV_DCAchi2_range);
  
    kfparticle->setIntermediatePV_DCARange_XY(intermediate_PV_DCA_range);
    kfparticle->setIntermediatePV_DCA_StdDevRange_XY(intermediate_PV_DCAchi2_range);
  
    std::vector<float> intermediate_min_dira = {0.90};
    kfparticle->setIntermediateMinDIRA(intermediate_min_dira);
  
    std::vector<float> intermediate_min_FDchi2 = {-1.};
    kfparticle->setIntermediateMinFDchi2(intermediate_min_FDchi2);
  
    std::vector<float> intermediate_max_vertex_vol = {1.1};
    kfparticle->setIntermediateMaxVertexVolume(intermediate_max_vertex_vol);
  
    kfparticle->setIntermediateDecayLengthRange({{0.1, 80.}});
    kfparticle->setIntermediateDecayLengthRange_XY({{0, FLT_MAX}});
    kfparticle->setIntermediateDecayTimeRange({{0, FLT_MAX}});
    kfparticle->setIntermediateDecayTimeRange_XY({{0, FLT_MAX}});
  
    kfparticle->setOutputName(Lambdapi_output_reco_file);
  
    se->registerSubsystem(kfparticle);
  }
  
  void end_kfparticle(const std::string &full_file_name, const std::string &final_path)
  {
    std::ifstream file(full_file_name.c_str());
    if (file.good())
    {
      std::string moveOutput = "mv " + full_file_name + " " + final_path;
      system(moveOutput.c_str());
    }
  }
  
  }  // namespace KFPARTICLE

#endif
