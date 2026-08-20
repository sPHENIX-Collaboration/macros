#ifndef FUN4ALL_TPCPATTERNRECOV0_C
#define FUN4ALL_TPCPATTERNRECOV0_C

#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllReturnCodes.h>
#include <fun4all/Fun4AllServer.h>

#include <trackingdiagnostics/TpcV0CandidateTree.h>

#include <phfield/PHField.h>
#include <phfield/PHFieldConfig.h>
#include <phfield/PHFieldConfigv1.h>
#include <phfield/PHFieldUtility.h>

#include <TFile.h>
#include <TSystem.h>
#include <TTree.h>

#include <fstream>
#include <iostream>
#include <string>

int Fun4All_TpcPatternRecoV0(
    const int nEvents = 10,
    const std::string &inputDst = "input.root",
    const std::string &outputFile = "tpc_pattern_v0_candidates.root",
    const double preTrackPtMin = 0.05,
    const double preTrackDcaXyMin = -1.0,
    const double prePairDcaMax = 10.0,
    const double preLprojMin = -1.0,
    const double preCosThetaMin = -2.0,
    const bool useFinalTrackHelix = false,
    const std::string &pointOrder = "auto",
    const std::string &fitMethod = "helix",
    const double kalmanSigmaRphiCm = 0.03,
    const double kalmanSigmaRCm = 0.03,
    const double kalmanSigmaZCm = 0.05,
    const bool writeSameSignPairs = false,
    const bool writeClusterResidualTree = false,
    const bool useKalmanFieldMap = true,
    const std::string &kalmanFieldMapFile = "",
    const double kalmanRkMaxStepCm = 5.0,
    const double kalmanRkStepTolerance = 1.0e-4,
    const int kalmanRkMaxStepTrials = 12,
    const int kalmanRkMaxTotalSteps = 2000,
    const bool kalmanFastFieldJacobian = true,
    const bool kalmanFastFieldPca = true,
    const int kalmanFieldPcaRefineIterations = 6,
    const int coarseSteps = 64,
    const int pcaCandidates = 32,
    const bool printTiming = false,
    const int nSkip = 0,
    const bool kalmanAnalyticUniformPropagation = false,
    const double preTrackQualityMax = -1.0,
    const int preTrackNpointsMin = 0,
    const double pairPcaZMax = -1.0,
    const double pairPcaDzMax = -1.0,
    const double pairDecayRadiusMin = -1.0,
    const double pairAlphaAbsMax = -1.0,
    const double pairDcaMax = -1.0,
    const double pairDiraMin = -2.0,
    const double finalTrackHelixMaxUpstreamCm = 80.0,
    const double finalTrackHelixDownstreamMarginCm = 5.0,
    const bool writeKalmanInnovationDiagnostics = false,
    const double primaryVertexX = 0.0,
    const double primaryVertexY = 0.0,
    const double primaryVertexZ = 0.0,
    const bool reconstructPairs = true,
    const int requiredCrossing = TpcV0CandidateTree::NoCrossingSelection,
    const bool requireSameCrossing = false,
    const int maxCrossingTier = -1,
    const std::string &crossingDecisionNode = "TPC_CROSSING_DECISIONS")
{
  const int load_tpc_reco = gSystem->Load("libtpctrackreco.so");
  if (load_tpc_reco < 0)
  {
    std::cerr << "Failed to load libtpctrackreco.so" << std::endl;
    gSystem->Exit(1);
    return 1;
  }

  const int load_tracking_diagnostics = gSystem->Load("libTrackingDiagnostics.so");
  if (load_tracking_diagnostics < 0)
  {
    std::cerr << "Failed to load libTrackingDiagnostics.so" << std::endl;
    gSystem->Exit(1);
    return 1;
  }

  const int load_phfield = gSystem->Load("libphfield.so");
  if (load_phfield < 0)
  {
    std::cerr << "Failed to load libphfield.so" << std::endl;
    gSystem->Exit(1);
    return 1;
  }

  auto *se = Fun4AllServer::instance();
  se->Verbosity(0);

  PHField *kalman_field = nullptr;
  if (useKalmanFieldMap && !kalmanFieldMapFile.empty())
  {
    PHFieldConfigv1 field_config(PHFieldConfig::FieldInterpolated, kalmanFieldMapFile, 1.0);
    kalman_field = PHFieldUtility::GetFieldMapNode(&field_config, se->topNode(), 1);
    if (kalman_field == nullptr)
    {
      std::cerr << "Failed to build Kalman field map from " << kalmanFieldMapFile << std::endl;
      gSystem->Exit(1);
      return 1;
    }
    std::cout << "Using Kalman RK field map: " << kalmanFieldMapFile << std::endl;
  }

  auto *v0 = new TpcV0CandidateTree("TpcPatternRecoV0CandidateTree", outputFile);
  v0->use_pattern_cluster_tracks(true);
  v0->set_tpc_sa_cluster_node("TPC_POLYCLUSTERS");
  v0->set_tpc_sa_track_node("TPC_POLYTRACKS");
  v0->set_tpc_sa_track_vertex_node("TPC_POLYTRACKVERTICES");
  v0->set_crossing_decision_node(crossingDecisionNode);
  v0->set_use_truth_primary_vertex(false);
  v0->set_primary_vertex(primaryVertexX, primaryVertexY, primaryVertexZ);
  std::cout << "Using fixed primary vertex: (" << primaryVertexX << ", "
            << primaryVertexY << ", " << primaryVertexZ << ") cm" << std::endl;
  v0->set_fit_helix(true);
  v0->set_use_final_track_helix(useFinalTrackHelix);
  if (!v0->set_point_order(pointOrder))
  {
    gSystem->Exit(1);
    return 1;
  }
  if (!v0->set_track_fit_method(fitMethod))
  {
    gSystem->Exit(1);
    return 1;
  }
  v0->set_kalman_measurement_sigmas(kalmanSigmaRphiCm, kalmanSigmaRCm, kalmanSigmaZCm);
  const char *innovation_diagnostics_env =
      gSystem->Getenv("V0_WRITE_KALMAN_INNOVATION_DIAGNOSTICS");
  const std::string innovation_diagnostics_value =
      innovation_diagnostics_env != nullptr ? innovation_diagnostics_env : "";
  const bool enable_innovation_diagnostics =
      writeKalmanInnovationDiagnostics || innovation_diagnostics_value == "true" ||
      innovation_diagnostics_value == "TRUE" || innovation_diagnostics_value == "1";
  v0->set_write_kalman_innovation_diagnostics(enable_innovation_diagnostics);
  v0->set_kalman_analytic_uniform_propagation(kalmanAnalyticUniformPropagation);
  v0->use_kalman_field_map(useKalmanFieldMap);
  v0->set_kalman_rkn4(kalmanRkMaxStepCm, kalmanRkStepTolerance,
                      kalmanRkMaxStepTrials, kalmanRkMaxTotalSteps);
  v0->set_kalman_fast_field_jacobian(kalmanFastFieldJacobian);
  v0->set_kalman_fast_field_pca(kalmanFastFieldPca);
  v0->set_kalman_field_pca_refine_iterations(kalmanFieldPcaRefineIterations);
  if (kalman_field != nullptr)
  {
    v0->set_kalman_magnetic_field(kalman_field);
  }
  v0->set_fit_first_points(0);
  v0->set_theta_extension(2.0);
  v0->set_final_track_helix_search(finalTrackHelixMaxUpstreamCm,
                                   finalTrackHelixDownstreamMarginCm);
  v0->set_coarse_steps(coarseSteps);
  v0->set_pca_candidates(pcaCandidates);
  v0->set_print_timing(printTiming);

  // Loose preselection before the expensive helix-helix PCA.  Negative values
  // disable cuts; for TPC-only data the primary vertex is not known reliably,
  // so the default avoids PV-dependent DCA/Lproj/pointing cuts.
  v0->set_pre_track_pt_min(preTrackPtMin);
  v0->set_pre_track_dca_xy_min(preTrackDcaXyMin);
  v0->set_pre_pair_dca_max(prePairDcaMax);
  v0->set_pre_lproj_min(preLprojMin);
  v0->set_pre_cos_theta_min(preCosThetaMin);
  v0->set_pre_track_quality_max(preTrackQualityMax);
  v0->set_pre_track_npoints_min(preTrackNpointsMin);
  v0->set_pair_pca_z_max(pairPcaZMax);
  v0->set_pair_pca_dz_max(pairPcaDzMax);
  v0->set_pair_decay_radius_min(pairDecayRadiusMin);
  v0->set_pair_alpha_abs_max(pairAlphaAbsMax);
  v0->set_pair_dca_max(pairDcaMax);
  v0->set_pair_dira_min(pairDiraMin);
  v0->set_required_crossing(requiredCrossing);
  v0->set_require_same_crossing(requireSameCrossing);
  v0->set_max_crossing_tier(maxCrossingTier);
  v0->set_reconstruct_pairs(reconstructPairs);
  v0->set_write_same_sign_pairs(writeSameSignPairs);
  v0->set_write_cluster_residual_tree(writeClusterResidualTree);
  std::cout << "Using crossing decisions from " << crossingDecisionNode
            << ", required crossing=";
  if (requiredCrossing == TpcV0CandidateTree::NoCrossingSelection)
  {
    std::cout << "any";
  }
  else
  {
    std::cout << requiredCrossing;
  }
  std::cout << ", require same crossing=" << std::boolalpha << requireSameCrossing
            << ", max crossing tier=" << maxCrossingTier << std::noboolalpha
            << std::endl;
  v0->Verbosity(1);
  se->registerSubsystem(v0);

  auto *input = new Fun4AllDstInputManager("TPCPatternRecoInput");
  // V0 reconstruction consumes fitted pattern tracks and cluster centroids,
  // not the much larger raw-hit container.
  input->BranchSelect("DST#TRKR#TRKR_HITSET", 0);
  if (inputDst.size() >= 5 && inputDst.substr(inputDst.size() - 5) == ".list")
  {
    input->AddListFile(inputDst);
  }
  else
  {
    input->fileopen(inputDst);
  }
  se->registerInputManager(input);

  if (nSkip > 0)
  {
    std::cout << "Skipping " << nSkip << " input events" << std::endl;
    se->skip(nSkip);
  }
  const int runStatus = se->run(nEvents);
  se->End();
  se->PrintTimer();
  delete se;

  if (runStatus == Fun4AllReturnCodes::ABORTRUN ||
      runStatus == Fun4AllReturnCodes::ABORTPROCESSING)
  {
    gSystem->Unlink((outputFile + ".complete").c_str());
    std::cerr << "Fun4All processing failed with status " << runStatus
              << "; not creating a completion marker" << std::endl;
    gSystem->Exit(4);
    return 4;
  }

  bool outputIsValid = false;
  Long64_t pairEntries = -1;
  Long64_t trackEntries = -1;
  {
    TFile outputCheck(outputFile.c_str(), "READ");
    if (outputCheck.IsZombie())
    {
      std::cerr << "Output validation failed: ROOT file is a zombie: " << outputFile << std::endl;
    }
    else if (outputCheck.TestBit(TFile::kRecovered))
    {
      std::cerr << "Output validation failed: ROOT recovered an incompletely written file: "
                << outputFile << std::endl;
    }
    else
    {
      auto *pairTree = dynamic_cast<TTree *>(outputCheck.Get("pairTree"));
      auto *trackTree = dynamic_cast<TTree *>(outputCheck.Get("trackTree"));
      if (!pairTree || !trackTree)
      {
        std::cerr << "Output validation failed: pairTree or trackTree is missing from "
                  << outputFile << std::endl;
      }
      else
      {
        pairEntries = pairTree->GetEntries();
        trackEntries = trackTree->GetEntries();
        outputIsValid = pairEntries >= 0 && trackEntries >= 0;
      }
    }
  }

  if (!outputIsValid)
  {
    gSystem->Unlink((outputFile + ".complete").c_str());
    std::cerr << "Not creating completion marker for invalid output " << outputFile << std::endl;
    gSystem->Exit(3);
    return 3;
  }

  std::cout << "Validated output trees: pairTree=" << pairEntries
            << " trackTree=" << trackEntries << std::endl;
  std::cout << "Finished TPC pattern-reco V0 candidate tree: " << outputFile << std::endl;
  std::ofstream completion_marker(outputFile + ".complete");
  completion_marker << "complete\n";
  completion_marker.close();
  gSystem->Exit(0);
  return 0;
}

#endif
