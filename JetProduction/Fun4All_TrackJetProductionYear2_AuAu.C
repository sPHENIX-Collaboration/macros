#ifndef FUN4ALL_TRACKJETPRODUCTIONYEAR2_AUAU_C
#define FUN4ALL_TRACKJETPRODUCTIONYEAR2_AUAU_C

#include <GlobalVariables.C>

#include <G4_ActsGeom.C>
#include <G4_Centrality.C>
#include <G4_Global.C>
#include <G4_Magnet.C>
#include <HIJetReco.C>
#include <Jet_QA.C>
#include <QA.C>
#include <Trkr_Reco.C>
#include <Trkr_RecoInit.C>
#include <Trkr_TpcReadoutInit.C>

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

#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <utility>
#include <vector>

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
//! Track jet production macro for year 2 (AuAu)
// ============================================================================
/*! Jet production macro for AuAu-running in year 2. Currently used for
 *  producing for QA. Can be adapted for production of JET DSTs in the
 *  future.
 *
 *  Necessary input for track jets:
 *    - DST_CALO (for beam background filter)
 *    - DST_TRKR_TRACKS
 *    - DST_TRKR_CLUSTER (for tracks-in-jets QA)
 */
void Fun4All_TrackJetProductionYear2_AuAu(
  const int nEvents = 0,
  const std::string& inlist = "./input/dsts_track_run2pp-00053877.goldenTrkCaloRun_allSeg.list",
  const std::string& outfile = "DST_JET-00053877-0000.root",
  const std::string& outfile_hist = "HIST_JETQA-00053877-0000.year2aa_tracktest.root",
  const std::string& dbtag = "ProdA_2024"
) {

  // set options --------------------------------------------------------------

  // turn on/off DST output and/or QA
  Enable::DSTOUT           = false;
  Enable::QA               = true;
  Enable::HIJETS_VERBOSITY = 0;
  Enable::JETQA_VERBOSITY  = std::max(Enable::VERBOSITY, Enable::HIJETS_VERBOSITY);

  // jet reco options
  Enable::HIJETS         = true;
  Enable::HIJETS_TOWER   = false;
  Enable::HIJETS_TRACK   = true;
  Enable::HIJETS_PFLOW   = false;
  HIJETS::is_pp          = false;
  HIJETS::do_vertex_type = true;
  HIJETS::vertex_type    = Enable::HIJETS_TRACK ? GlobalVertex::SVTX : GlobalVertex::MBD;

  // qa options
  JetQA::DoInclusive      = true;
  JetQA::DoTriggered      = true;
  JetQA::DoPP             = HIJETS::is_pp;
  JetQA::UseBkgdSub       = true;
  JetQA::RestrictPtToTrig = false;
  JetQA::RestrictEtaByR   = true;
  JetQA::HasTracks        = Enable::HIJETS_TRACK || Enable::HIJETS_PFLOW;
  JetQA::HasCalos         = Enable::HIJETS_TOWER || Enable::HIJETS_PFLOW;

  // tracking options
  G4TPC::ENABLE_MODULE_EDGE_CORRECTIONS = true;
  Enable::MVTX_APPLYMISALIGNMENT        = true;
  ACTSGEOM::mvtx_applymisalignment      = Enable::MVTX_APPLYMISALIGNMENT;
  TRACKING::pp_mode                     = HIJETS::is_pp;

  // initialize interfaces, register inputs -----------------------------------

  // initialize F4A server
  Fun4AllServer* se = Fun4AllServer::instance();
  se -> Verbosity(1);

  // grab 1st file from input lists
  ifstream    files(inlist);
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
  Fun4AllInputManager* indst = new Fun4AllDstInputManager("indst");
  indst -> AddListFile(inlist);
  se -> registerInputManager(indst);

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
  if (!HIJETS::is_pp)
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
  HIJetReco();  

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
