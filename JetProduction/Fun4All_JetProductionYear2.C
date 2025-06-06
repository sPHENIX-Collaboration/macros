#ifndef FUN4ALL_JETPRODUCTIONYEAR2_C
#define FUN4ALL_JETPRODUCTIONYEAR2_C


// c++ utilities
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <utility>
#include <vector>

// coresoftware headers
#include <ffamodules/CDBInterface.h>
#include <ffamodules/FlagHandler.h>
#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllRunNodeInputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllUtils.h>
#include <g4centrality/PHG4CentralityReco.h>
#include <globalvertex/GlobalVertexReco.h>
#include <jetbackground/BeamBackgroundFilterAndQA.h>
#include <mbd/MbdReco.h>
#include <phool/recoConsts.h>
#include <qautils/QAHistManagerDef.h>
#include <zdcinfo/ZdcReco.h>

// f4a macros
#include <G4_ActsGeom.C>
#include <G4_Centrality.C>
#include <G4_Global.C>
#include <G4_Magnet.C>
#include <GlobalVariables.C>
#include <HIJetReco.C>  // n.b. needed for rho calculation
#include <NoBkgdSubJetReco.C>
#include <Jet_QA.C>
#include <QA.C>
#include <Trkr_Reco.C>
#include <Trkr_RecoInit.C>
#include <Trkr_TpcReadoutInit.C>

// load libraries
R__LOAD_LIBRARY(libcentrality.so)
R__LOAD_LIBRARY(libg4centrality.so)
R__LOAD_LIBRARY(libglobalvertex.so)
R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libjetbackground.so)
R__LOAD_LIBRARY(libjetqa.so)
R__LOAD_LIBRARY(libmbd.so)
R__LOAD_LIBRARY(libqautils.so)
R__LOAD_LIBRARY(libzdcinfo.so)



// ============================================================================
//! Jet production macro for year 2 (pp)
// ============================================================================
/*! Jet production macro for pp-running in year 2. Currently used for
 *  producing for QA. Can be adapted for production of JET DSTs in the
 *  future.
 *
 *  Necessary inputs:
 *    - For calo jets:
 *      - DST_CALO
 *    - For track jets:
 *      - DST_CALO (for beam background filter)
 *      - DST_TRKR_TRACKS
 *      - DST_TRKR_CLUSTER (for tracks-in-jets QA)
 */
void Fun4All_JetProductionYear2(
  const int nEvents = 0,
  const bool useTwrs = true,
  const bool useTrks = false,
  const bool useFlow = false,
  const std::vector<std::string>& inlists = {
    "./input/dsts_calo_run2pp-00053877.goldenTrkCaloRun_allSeg.list",
    "./input/dsts_clust_run2pp-00053877.goldenTrkCaloRun_allSeg.list",
    "./input/dsts_track_run2pp-00053877.goldenTrkCaloRun_allSeg.list"
  },
  const std::string& outfile = "DST_JET-00053877-0000.root",
  const std::string& outfile_hist = "HIST_JETQA-00053877-0000.year2_trackandcalotest.root",
  const std::string& dbtag = "ProdA_2024"
) {

  // set options --------------------------------------------------------------

  // turn on/off DST output and/or QA
  Enable::DSTOUT           = false;
  Enable::QA               = true;
  Enable::NSJETS_VERBOSITY = 0;
  Enable::JETQA_VERBOSITY  = std::max(Enable::VERBOSITY, Enable::NSJETS_VERBOSITY);

  // jet reco options
  Enable::NSJETS         = true;
  Enable::NSJETS_TOWER   = useTwrs;
  Enable::NSJETS_TRACK   = useTrks;
  Enable::NSJETS_PFLOW   = useFlow;
  NSJETS::is_pp          = true;
  NSJETS::do_vertex_type = true;
  NSJETS::vertex_type    = Enable::NSJETS_TRACK ? GlobalVertex::SVTX : GlobalVertex::MBD;

  // make sure HIJetReco inputs are the same
  // for rho calculation
  Enable::HIJETS_TOWER = Enable::NSJETS_TOWER;
  Enable::HIJETS_TRACK = Enable::NSJETS_TRACK;
  Enable::HIJETS_PFLOW = Enable::NSJETS_PFLOW;

  // qa options
  JetQA::DoInclusive      = true;
  JetQA::DoTriggered      = true;
  JetQA::DoPP             = NSJETS::is_pp;
  JetQA::UseBkgdSub       = false;
  JetQA::RestrictPtToTrig = false;
  JetQA::RestrictEtaByR   = true;
  JetQA::HasTracks        = Enable::NSJETS_TRACK || Enable::NSJETS_PFLOW;
  JetQA::HasCalos         = Enable::NSJETS_TOWER || Enable::NSJETS_PFLOW;

  // tracking options
  G4TPC::ENABLE_MODULE_EDGE_CORRECTIONS = true;
  Enable::MVTX_APPLYMISALIGNMENT        = true;
  ACTSGEOM::mvtx_applymisalignment      = Enable::MVTX_APPLYMISALIGNMENT;
  TRACKING::pp_mode                     = NSJETS::is_pp;

  // initialize interfaces, register inputs -----------------------------------

  // initialize F4A server
  Fun4AllServer* se = Fun4AllServer::instance();
  se -> Verbosity(1);

  // grab 1st file from input lists
  ifstream    files(inlists.front());
  std::string first("");
  std::getline(files, first);

  // grab run and segment no.s
  pair<int, int> runseg = Fun4AllUtils::GetRunSegment(first);
  int runnumber = runseg.first;

  // set up reconstruction constants, DB tag, timestamp
  recoConsts* rc = recoConsts::instance();
  rc -> set_StringFlag("CDB_GLOBALTAG", dbtag);
  rc -> set_uint64Flag("TIMESTAMP", runnumber);

  // connect to conditions database
  CDBInterface* cdb = CDBInterface::instance();
  cdb -> Verbosity(1);

  // set up flag handler
  FlagHandler* flag = new FlagHandler();
  se -> registerSubsystem(flag);

  // read in input
  for (std::size_t iin = 0; iin < inlists.size(); ++iin)
  {
    Fun4AllInputManager* indst = new Fun4AllDstInputManager("indst" + std::to_string(iin));
    indst -> AddListFile(inlists[iin]);
    se -> registerInputManager(indst);
  }

  // set up tracking
  if (JetQA::HasTracks)
  {
    // register tracking geometry
    Fun4AllRunNodeInputManager* ingeom = new Fun4AllRunNodeInputManager("ingeom");
    ingeom -> AddFile(cdb -> getUrl("Tracking_Geometry"));
    se -> registerInputManager(ingeom);

    // initialize tracking
    TpcReadoutInit(runnumber);
    TrackingInit();
  }

  // register reconstruction modules ------------------------------------------

  // do vertex & centrality reconstruction
  Global_Reco();
  if (!NSJETS::is_pp)
  {
    Centrality();
  }

  // filter out beam-background events (use default parameters for
  // streak-sideband filter)
  BeamBackgroundFilterAndQA* filter = new BeamBackgroundFilterAndQA("BeamBackgroundFilterAndQA");
  filter -> Verbosity(std::max(Enable::QA_VERBOSITY, Enable::JETQA_VERBOSITY));
  filter -> SetConfig(
    {
      .debug          = false,
      .doQA           = Enable::QA,
      .doEvtAbort     = false,
      .filtersToApply = {"StreakSideband"}
    }
  );
  se -> registerSubsystem(filter);

  // do jet reconstruction
  NoBkgdSubJetReco();

  // register modules necessary for QA
  if (Enable::QA)
  {
    DoRhoCalculation();
    Jet_QA();
  }

  // if needed, save DST output
  if (Enable::DSTOUT)
  {
    Fun4AllDstOutputManager* out = new Fun4AllDstOutputManager("DSTOUT", outfile);
    out -> StripNode("CEMCPackets");
    out -> StripNode("HCALPackets");
    out -> StripNode("ZDCPackets");
    out -> StripNode("SEPDPackets");
    out -> StripNode("MBDPackets");
    se -> registerOutputManager(out);
  }

  // run & exit ---------------------------------------------------------------

  // run4all
  se -> run(nEvents);
  se -> End();

  // if needed, save QA output
  if (Enable::QA)
  {
    QAHistManagerDef::saveQARootFile(outfile_hist);
  }

  // print used DB files, time elapsed and delete server
  cdb -> Print();
  se -> PrintTimer();
  delete se;

  // announce end and exit
  std::cout << "Jets are done!" << std::endl;
  gSystem -> Exit(0);

}

#endif

// end ------------------------------------------------------------------------
