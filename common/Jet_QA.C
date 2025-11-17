#ifndef MACRO_JETQA_C
#define MACRO_JETQA_C

#include <QA.C>

#include <jetqa/CaloStatusMapper.h>
#include <jetqa/ConstituentsinJets.h>
#include <jetqa/DijetQA.h>
#include <jetqa/EMClusterKinematics.h>
#include <jetqa/JetKinematicCheck.h>
#include <jetqa/JetQADefs.h>
#include <jetqa/JetSeedCount.h>
#include <jetqa/RhosinEvent.h>
#include <jetqa/StructureinJets.h>
#include <jetqa/TrksInJetQA.h>
#include <jetqa/UEvsEtaCentrality.h>

#include <fun4all/Fun4AllServer.h>

#include <map>
#include <optional>
#include <string>
#include <utility>
#include <vector>

R__LOAD_LIBRARY(libjetqa.so)

// ----------------------------------------------------------------------------
//! General options for jet QA
// ----------------------------------------------------------------------------
namespace Enable
{
  int JETQA_VERBOSITY = 0;  ///< verbosity of jet qa
}

// ----------------------------------------------------------------------------
//! Namespace to collect various enums, default arguments, etc.
// ----------------------------------------------------------------------------
namespace JetQA
{

  // flags --------------------------------------------------------------------

  ///! Set to true if input jets utilize tracks (e.g. via particle flow)
  bool HasTracks = false;

  ///! Set to true if input jets utilize calorimeters(e.g. via particle flow)
  bool HasCalos = false;

  ///! Set to true if jets have background subtracted
  ///! (e.g. HIJetReco.C output)
  bool UseBkgdSub = false;

  ///! Set to true to generate histograms for no trigger selection
  bool DoInclusive = true;

  ///! Set to true to generate histograms for a specified set of triggers
  bool DoTriggered = true;

  ///! Set to true to use pp-specific options (otherwise assumes AuAU)
  bool DoPP = false;

  ///! Set to true to restrict minimum jet pt to trigger threshold
  bool RestrictPtToTrig = false;

  ///! Set to true to restrict jet eta acceptance by resolution parameter
  bool RestrictEtaByR = true;

  // enums --------------------------------------------------------------------

  ///! enumerates possible jet types
  enum Type
  {
    AntiKtTowerSubR02,
    AntiKtTowerSubR03,
    AntiKtTowerSubR04,
    AntiKtTowerSubR05,
    AntiKtTowerR02,
    AntiKtTowerR03,
    AntiKtTowerR04,
    AntiKtTowerR05,
    AntiKtTrackR02,
    AntiKtTrackR03,
    AntiKtTrackR04,
    AntiKtTrackR05
  };

  ///! enumerates possible jet sources
  enum class Source : uint32_t
  {
    Calos = 0,
    Tracks = 1
  };

  // constants ----------------------------------------------------------------

  ///! Min jet pt in GeV/c
  double MinJetPt = 6.;

  ///! Max jet pt in GeV/c
  double MaxJetPt = 100.;

  ///! Min eta acceptance
  double MinAcceptEta = -1.1;

  ///! Max eta acceptance
  double MaxAcceptEta = 1.1;

  // maps ---------------------------------------------------------------------

  ///! Map from trigger to histogram tag
  std::map<uint32_t, std::string> GL1Tag = {
      {JetQADefs::GL1::Clock, "clock"},
      {JetQADefs::GL1::ZDCS, "zdcs"},
      {JetQADefs::GL1::ZDCN, "zdcn"},
      {JetQADefs::GL1::ZDCNS, "zdcns"},
      {JetQADefs::GL1::HCalSingle, "hcalsingle"},
      {JetQADefs::GL1::MBDS, "mbds"},
      {JetQADefs::GL1::MBDN, "mbdn"},
      {JetQADefs::GL1::MBDNS1, "mbdns1"},
      {JetQADefs::GL1::MBDNS2, "mbdns2"},
      {JetQADefs::GL1::MBDNSVtx10, "mbdnsvtx10"},
      {JetQADefs::GL1::MBDNSVtx30, "mbdnsvtx30"},
      {JetQADefs::GL1::MBDNSVtx60, "mbdnsvtx60"},
      {JetQADefs::GL1::MBDNSHCalSingle, "mbdnshcalsingle"},
      {JetQADefs::GL1::MBDNSJet1, "mbdnsjet1"},
      {JetQADefs::GL1::MBDNSJet2, "mbdnsjet2"},
      {JetQADefs::GL1::MBDNSJet3, "mbdnsjet3"},
      {JetQADefs::GL1::MBDNSJet4, "mbdnsjet4"},
      {JetQADefs::GL1::Jet1, "jet1"},
      {JetQADefs::GL1::Jet2, "jet2"},
      {JetQADefs::GL1::Jet3, "jet3"},
      {JetQADefs::GL1::Jet4, "jet4"},
      {JetQADefs::GL1::MBDNSPhoton1, "mbdnsphoton1"},
      {JetQADefs::GL1::MBDNSPhoton2, "mbdnsphoton2"},
      {JetQADefs::GL1::MBDNSPhoton3, "mbdnsphoton3"},
      {JetQADefs::GL1::MBDNSPhoton4, "mbdnsphoton4"},
      {JetQADefs::GL1::Photon1, "photon1"},
      {JetQADefs::GL1::Photon2, "photon2"},
      {JetQADefs::GL1::Photon3, "photon3"},
      {JetQADefs::GL1::Photon4, "photon4"},
      {JetQADefs::GL1::MBDNS2Vtx10, "mbdns2vtx10"},
      {JetQADefs::GL1::MBDNS2Vtx30, "mbdns2vtx30"},
      {JetQADefs::GL1::MBDNS2Vtx60, "mbdns2vtx60"},
      {JetQADefs::GL1::MBDNS2Vtx150, "mbdns2vtx150"},
      {JetQADefs::GL1::MBDNS2Photon6Vtx10, "mbdns2photon6vtx10"},
      {JetQADefs::GL1::MBDNS2Photon8Vtx10, "mbdns2photon8vtx10"},
      {JetQADefs::GL1::MBDNS2Photon10Vtx10, "mbdns2photon10vtx10"},
      {JetQADefs::GL1::MBDNS2Photon12Vtx10, "mbdns2photon12vtx10"},
      {JetQADefs::GL1::MBDNS2Photon6Vtx150, "mbdns2photon6vtx150"},
      {JetQADefs::GL1::MBDNS2Photon8Vtx150, "mbdns2photon8vtx150"},
      {JetQADefs::GL1::MBDNS2Photon10Vtx150, "mbdns2photon10vtx150"},
      {JetQADefs::GL1::MBDNS2Photon12Vtx150, "mbdns2photon12vtx150"},
      {JetQADefs::GL1::Inclusive, "inclusive"}};

  ///! Map from jet type to input node
  std::map<uint32_t, std::string> JetInput = {
      {Type::AntiKtTowerSubR02, "AntiKt_Tower_r02_Sub1"},
      {Type::AntiKtTowerSubR03, "AntiKt_Tower_r03_Sub1"},
      {Type::AntiKtTowerSubR04, "AntiKt_Tower_r04_Sub1"},
      {Type::AntiKtTowerSubR05, "AntiKt_Tower_r05_Sub1"},
      {Type::AntiKtTowerR02, "AntiKt_Tower_r02"},
      {Type::AntiKtTowerR03, "AntiKt_Tower_r03"},
      {Type::AntiKtTowerR04, "AntiKt_Tower_r04"},
      {Type::AntiKtTowerR05, "AntiKt_Tower_r05"},
      {Type::AntiKtTrackR02, "AntiKt_Track_r02"},
      {Type::AntiKtTrackR03, "AntiKt_Track_r03"},
      {Type::AntiKtTrackR04, "AntiKt_Track_r04"},
      {Type::AntiKtTrackR05, "AntiKt_Track_r05"}};

  ///! Map from jet type to histogram tag
  std::map<uint32_t, std::string> JetTag = {
      {Type::AntiKtTowerSubR02, "towersub1_antikt_r02"},
      {Type::AntiKtTowerSubR03, "towersub1_antikt_r03"},
      {Type::AntiKtTowerSubR04, "towersub1_antikt_r04"},
      {Type::AntiKtTowerSubR05, "towersub1_antikt_r05"},
      {Type::AntiKtTowerR02, "tower_antikt_r02"},
      {Type::AntiKtTowerR03, "tower_antikt_r03"},
      {Type::AntiKtTowerR04, "tower_antikt_r04"},
      {Type::AntiKtTowerR05, "tower_antikt_r05"},
      {Type::AntiKtTrackR02, "track_antikt_r02"},
      {Type::AntiKtTrackR03, "track_antikt_r03"},
      {Type::AntiKtTrackR04, "track_antikt_r04"},
      {Type::AntiKtTrackR05, "track_antikt_r05"}};

  ///! Map from jet type to resolution parameter
  std::map<uint32_t, double> JetRes = {
      {Type::AntiKtTowerSubR02, 0.2},
      {Type::AntiKtTowerSubR03, 0.3},
      {Type::AntiKtTowerSubR04, 0.4},
      {Type::AntiKtTowerSubR05, 0.5},
      {Type::AntiKtTowerR02, 0.2},
      {Type::AntiKtTowerR03, 0.3},
      {Type::AntiKtTowerR04, 0.4},
      {Type::AntiKtTowerR05, 0.5},
      {Type::AntiKtTrackR02, 0.2},
      {Type::AntiKtTrackR03, 0.3},
      {Type::AntiKtTrackR04, 0.4},
      {Type::AntiKtTrackR05, 0.5}};

  // methods ------------------------------------------------------------------

  // --------------------------------------------------------------------------
  //! Get trigger tag
  // --------------------------------------------------------------------------
  /*! If a trigger index is provided (i.e. a trigger is being selected),
   *  function returns the correpsonding tag. Otherwise (i.e. NO trigger
   *  is being selected), returns the inclusive tag.
   */
  inline std::string GetTriggerTag(std::optional<uint32_t> trg = std::nullopt)
  {
    std::string tag;
    if (trg.has_value())
    {
      tag.append("_" + GL1Tag[trg.value()]);
    }
    else
    {
      tag.append("_" + GL1Tag[JetQADefs::GL1::Inclusive]);
    }
    return tag;

  }  // end 'GetTriggerTag(std::optional<uint32_t>)'

  // --------------------------------------------------------------------------
  //! Get minimum jet pt based on which trigger fired
  // --------------------------------------------------------------------------
  // FIXME it might be prudent to allow for thresholds to change vs. run
  //   number... Before run 46038, the Jet1, 2 thresholds were different
  inline double GetMinJetPt(const uint32_t trg = JetQADefs::GL1::MBDNSJet1)
  {
    // by defult, set min to global constant
    double ptJetMin = MinJetPt;

    // if restricting pt to trigger threhsold, pick out relevant threshold
    if (RestrictPtToTrig)
    {
      switch (trg)
      {
      // Jet4 threshold
      case JetQADefs::GL1::MBDNSJet4:
        [[fallthrough]];
      case JetQADefs::GL1::Jet4:
        ptJetMin = 11.;
        break;

      // Jet3 threshold
      case JetQADefs::GL1::MBDNSJet3:
        [[fallthrough]];
      case JetQADefs::GL1::Jet3:
        ptJetMin = 10.;
        break;

      // Jet2 threshold
      case JetQADefs::GL1::MBDNSJet2:
        [[fallthrough]];
      case JetQADefs::GL1::Jet2:
        ptJetMin = 9.;
        break;

      // Jet1 threshold (default value)
      case JetQADefs::GL1::MBDNSJet1:
        [[fallthrough]];
      case JetQADefs::GL1::Jet1:
        [[fallthrough]];
      default:
        ptJetMin = 6.;
        break;
      }
    }
    return ptJetMin;

  }  // end 'GetMinJetPt(uint32_t)'

  // --------------------------------------------------------------------------
  //! Get default jet pt range
  // --------------------------------------------------------------------------
  inline std::pair<double, double> GetJetPtRange(std::optional<uint32_t> trg = std::nullopt)
  {
    std::pair<double, double> ptJetRange;
    if (trg.has_value())
    {
      ptJetRange = {GetMinJetPt(trg.value()), MaxJetPt};
    }
    else
    {
      ptJetRange = {GetMinJetPt(), MaxJetPt};
    }
    return ptJetRange;

  }  // end 'GetJetPtRange(std::optional<uint32_t>)'

  // --------------------------------------------------------------------------
  //! Get default jet eta range
  // --------------------------------------------------------------------------
  inline std::pair<double, double> GetJetEtaRange(const double res = 0.)
  {
    // determine relevant min/max
    const double etaMin = RestrictEtaByR ? MinAcceptEta + res : MinAcceptEta;
    const double etaMax = RestrictEtaByR ? MaxAcceptEta - res : MaxAcceptEta;

    // return range
    std::pair<double, double> etaJetRange = {etaMin, etaMax};
    return etaJetRange;

  }  // end 'GetJetEtaRange(double)'

  // --------------------------------------------------------------------------
  //! Get default list of triggers to analyze
  // --------------------------------------------------------------------------
  inline std::vector<uint32_t> GetDefaultTriggerList()
  {
    // default jets for pp
    std::vector<uint32_t> vecDefaultTrigsPP = {
        JetQADefs::GL1::MBDNS1,
        JetQADefs::GL1::MBDNSJet1,
        JetQADefs::GL1::MBDNSJet2,
        JetQADefs::GL1::MBDNSJet3,
        JetQADefs::GL1::MBDNSJet4,
        JetQADefs::GL1::Jet1,
        JetQADefs::GL1::Jet2,
        JetQADefs::GL1::Jet3,
        JetQADefs::GL1::Jet4};

    // default triggers for 2025 AuAu
    std::vector<uint32_t> vecDefaultTrigsAA = {
        JetQADefs::GL1::MBDNS2Vtx10,
        JetQADefs::GL1::MBDNS2Vtx150};

    if (JetQA::DoPP)
    {
      return vecDefaultTrigsPP;
    }

    return vecDefaultTrigsAA;

  }  // end 'GetDefaultTriggerList()'

  // --------------------------------------------------------------------------
  //! Get list of jets to analyze
  // --------------------------------------------------------------------------
  inline std::vector<uint32_t> GetJetsToQA(Source source)
  {
    std::vector<uint32_t> vecJetsToQA;
    switch (source)
    {
    case Source::Calos:
      if (UseBkgdSub)
      {
        vecJetsToQA.insert(vecJetsToQA.end(),
                           {Type::AntiKtTowerSubR02,
                            Type::AntiKtTowerSubR03,
                            Type::AntiKtTowerSubR04,
                            Type::AntiKtTowerSubR05});
      }
      else
      {
        vecJetsToQA.insert(vecJetsToQA.end(),
                           {Type::AntiKtTowerR02,
                            Type::AntiKtTowerR03,
                            Type::AntiKtTowerR04,
                            Type::AntiKtTowerR05});
      }
      break;

    case Source::Tracks:
      vecJetsToQA.insert(vecJetsToQA.end(),
                         {Type::AntiKtTrackR02,
                          Type::AntiKtTrackR03,
                          Type::AntiKtTrackR04,
                          Type::AntiKtTrackR05});
      break;
    }
    return vecJetsToQA;

  }  // end 'GetJetsToQA(Source)'

}  // namespace JetQA

// ----------------------------------------------------------------------------
//! Create QA modules for all jets, regardless of constituents
// ----------------------------------------------------------------------------
void CommonJetQA(std::optional<uint32_t> trg = std::nullopt)
{
  // set verbosity
  int verbosity = std::max(Enable::JETQA_VERBOSITY, Enable::QA_VERBOSITY);
  if (verbosity > 1)
  {
    std::cout << ">>> Entering CommonJetQA() with trg="
              << (trg.has_value() ? std::to_string(trg.value()) : "none")
              << std::endl;
  }

  // grab appropriate trigger tag
  std::string trig_tag = JetQA::GetTriggerTag(trg);

  // grab default pt, eta ranges
  std::pair<double, double> ptJetRange = JetQA::GetJetPtRange(trg);
  std::pair<double, double> etaJetMaxRange = JetQA::GetJetEtaRange();

  // connect to f4a server
  Fun4AllServer* se = Fun4AllServer::instance();

  // create modules that are independent of/take multiple R values ------------

  // initialize and register mass, eta, and pt qa module
  // for calos if need be
  if (JetQA::HasCalos)
  {
    auto vecCalos = JetQA::GetJetsToQA(JetQA::Source::Calos);
    if (!vecCalos.empty())
    {
      JetKinematicCheck* kinematicQA = new JetKinematicCheck(
          "JetKinematicCheck" + trig_tag + (JetQA::UseBkgdSub ? "_towersub1_antikt" : "_tower_antikt"),
          JetQA::JetInput[vecCalos[0]],
          JetQA::JetInput[vecCalos[1]],
          JetQA::JetInput[vecCalos[2]],
          JetQA::JetInput[vecCalos[3]]);
      kinematicQA->Verbosity(verbosity);
      kinematicQA->setHistTag("");
      kinematicQA->setRestrictEtaRange(JetQA::RestrictEtaByR);
      kinematicQA->setPtRange(ptJetRange.first, ptJetRange.second);
      kinematicQA->setEtaRange(etaJetMaxRange.first, etaJetMaxRange.second);
      if (trg.has_value())
      {
        kinematicQA->setTrgToSelect(trg.value());
      }
      se->registerSubsystem(kinematicQA);
    }
    else
    {
      std::cerr << ">>> Warning: trying to register JetKinematicCheck for tower jets, but jet list is empty!"
                << std::endl;
    }
  }

  // initialize and register mass, eta, and pt qa module
  // for tracks if need be
  if (JetQA::HasTracks)
  {
    auto vecTrk = JetQA::GetJetsToQA(JetQA::Source::Tracks);
    if (!vecTrk.empty())
    {
      JetKinematicCheck* kinematicQA_Tracks = new JetKinematicCheck(
          "JetKinematicCheck" + trig_tag + "_track_antikt",
          JetQA::JetInput[vecTrk[0]],
          JetQA::JetInput[vecTrk[1]],
          JetQA::JetInput[vecTrk[2]],
          JetQA::JetInput[vecTrk[3]]);
      kinematicQA_Tracks->Verbosity(verbosity);
      kinematicQA_Tracks->setHistTag("");
      kinematicQA_Tracks->setRestrictEtaRange(JetQA::RestrictEtaByR);
      kinematicQA_Tracks->setPtRange(ptJetRange.first, ptJetRange.second);
      kinematicQA_Tracks->setEtaRange(etaJetMaxRange.first, etaJetMaxRange.second);
      if (trg.has_value())
      {
        kinematicQA_Tracks->setTrgToSelect(trg.value());
      }
      se->registerSubsystem(kinematicQA_Tracks);
    }
    else
    {
      std::cerr << ">>> Warning: trying to register JetKinematicCheck for track jets, but jet list is empty!"
                << std::endl;
    }
  }

  // create modules that take single R values ---------------------------------

  // get relevant jets to QA
  std::vector<uint32_t> vecJetsToQA;
  if (JetQA::HasCalos)
  {
    auto vecCalJets = JetQA::GetJetsToQA(JetQA::Source::Calos);
    vecJetsToQA.insert(vecJetsToQA.end(), vecCalJets.begin(), vecCalJets.end());
  }
  if (JetQA::HasTracks)
  {
    auto vecTrkJets = JetQA::GetJetsToQA(JetQA::Source::Tracks);
    vecJetsToQA.insert(vecJetsToQA.end(), vecTrkJets.begin(), vecTrkJets.end());
  }

  // loop over resolution parameters
  for (uint32_t jet : vecJetsToQA)
  {
    // get R-dependent eta range
    std::pair<double, double> etaJetRange = JetQA::GetJetEtaRange(JetQA::JetRes[jet]);

    // intialize and register dijetQA
    DijetQA* dijetQA = new DijetQA(
        "DijetQA" + trig_tag + "_" + JetQA::JetTag[jet],
        JetQA::JetInput[jet]);
    dijetQA->Verbosity(verbosity);
    dijetQA->setPtLeadRange(ptJetRange.first, ptJetRange.second);
    dijetQA->setPtSubRange(1.0, ptJetRange.second);
    dijetQA->setEtaRange(etaJetRange.first, etaJetRange.second);
    if (trg.has_value())
    {
      dijetQA->setTrgToSelect(trg.value());
    }
    se->registerSubsystem(dijetQA);

  }  // end jet loop
  return;

}  // end 'CommonJetQA(std::optional<uint32_t>)'

// ----------------------------------------------------------------------------
//! Create QA modules for jets with tracks
// ----------------------------------------------------------------------------
void JetsWithTracksQA(std::optional<uint32_t> trg = std::nullopt)
{
  // set verbosity
  int verbosity = std::max(Enable::JETQA_VERBOSITY, Enable::QA_VERBOSITY);
  if (verbosity > 1)
  {
    std::cout << ">>> Entering JetsWithTracksQA() with trg="
              << (trg.has_value() ? std::to_string(trg.value()) : "none")
              << std::endl;
  }

  // grab appropriate trigger tag
  std::string trig_tag = JetQA::GetTriggerTag(trg);

  // grab default pt, eta ranges
  std::pair<double, double> ptJetRange = JetQA::GetJetPtRange(trg);
  std::pair<double, double> etaJetRange = JetQA::GetJetEtaRange();

  // get list of jet nodes to analyze
  std::vector<uint32_t> vecJetsToQA = JetQA::GetJetsToQA(JetQA::Source::Tracks);

  // connect to f4a server
  Fun4AllServer* se = Fun4AllServer::instance();

  // initialize and register event-wise rho check
  RhosinEvent* evtRhoQA = new RhosinEvent("EventWiseTrackRho" + trig_tag, "");
  evtRhoQA->Verbosity(verbosity);
  evtRhoQA->add_area_rho_node("EventRho_AREA");
  evtRhoQA->add_mult_rho_node("EventRho_MULT");
  if (trg.has_value())
  {
    evtRhoQA->setTrgToSelect(trg.value());
  }
  se->registerSubsystem(evtRhoQA);

  // create modules that take single R values ---------------------------------

  // loop over resolution parameters
  for (uint32_t jet : vecJetsToQA)
  {
    // intialize and register sum track vs. jet pt qa module
    StructureinJets* jetStructQA = new StructureinJets(
        "StructureInJets" + trig_tag + "_" + JetQA::JetTag[jet],
        JetQA::JetInput[jet],
        "SvtxTrackMap",
        "");
    jetStructQA->Verbosity(verbosity);
    jetStructQA->writeToOutputFile(false);
    jetStructQA->setTrkPtCut(0.1);
    jetStructQA->setTrkQualityCut(100.0);  // set to be big for now, should refine later
    jetStructQA->setTrkNSilCut(4);
    jetStructQA->setTrkNTPCCut(25);
    jetStructQA->setJetRadius(JetQA::JetRes[jet]);
    jetStructQA->setJetPtRange(ptJetRange.first, ptJetRange.second);
    jetStructQA->setJetEtaRange(etaJetRange.first, etaJetRange.second);
    if (trg.has_value())
    {
      jetStructQA->setTrgToSelect(trg.value());
    }
    se->registerSubsystem(jetStructQA);

    // initialize and register track jet qa module
    TrksInJetQA* trksInJetQA = new TrksInJetQA("TrksInJet" + trig_tag + "_" + JetQA::JetTag[jet]);
    trksInJetQA->SetHistSuffix("");
    trksInJetQA->Configure(
        {.outMode = TrksInJetQA::OutMode::QA,
         .verbose = verbosity,
         .doDebug = false,
         .doInclusive = false,  // n.b. inclusive case covered by track QA
         .doInJet = true,
         .doHitQA = false,
         .doClustQA = true,
         .doTrackQA = true,
         .doJetQA = true,
         .doSubsysHist = false,
         .doOptHist = false,  // turn off optional histograms
         .rJet = JetQA::JetRes[jet],
         .jetInNode = JetQA::JetInput[jet]});
    if (trg.has_value())
    {
      trksInJetQA->SetTrgToSelect(trg.value());
    }
    se->registerSubsystem(trksInJetQA);

  }  // end jet loop
  return;

}  // end 'JetWithTracksQA(std::optional<uint32_t>)'

// ----------------------------------------------------------------------------
//! Create QA modules for jets with clusters (Calo)
// ----------------------------------------------------------------------------
void JetsWithCaloQA(std::optional<uint32_t> trg = std::nullopt)
{
  // set verbosity
  int verbosity = std::max(Enable::JETQA_VERBOSITY, Enable::QA_VERBOSITY);
  if (verbosity > 1)
  {
    std::cout << ">>> Entering JetsWithCaloQA() with trg="
              << (trg.has_value() ? std::to_string(trg.value()) : "none")
              << std::endl;
  }

  // grab appropriate trigger tag
  std::string trig_tag = JetQA::GetTriggerTag(trg);

  // grab default pt, eta ranges
  std::pair<double, double> ptJetRange = JetQA::GetJetPtRange(trg);
  //  std::pair<double, double> etaJetRange = JetQA::GetJetEtaRange();

  // connect to f4a server
  Fun4AllServer* se = Fun4AllServer::instance();

  // get list of jet nodes to analyze
  std::vector<uint32_t> vecJetsToQA = JetQA::GetJetsToQA(JetQA::Source::Calos);

  // initialize and register emcluster jet kinematic QA
  EMClusterKinematics* emclusterJetsQA = new EMClusterKinematics(
      "EMClusterKinematics" + trig_tag,
      "CLUSTERINFO_CEMC",
      "");
  emclusterJetsQA->SetDoOptHist(false);
  if (trg.has_value())
  {
    emclusterJetsQA->SetTrgToSelect(trg.value());
  }
  emclusterJetsQA->Verbosity(verbosity);
  se->registerSubsystem(emclusterJetsQA);

  // initialize and register event-wise rho check
  RhosinEvent* evtRhoQA = new RhosinEvent("EventWiseCaloRho" + trig_tag, "");
  evtRhoQA->Verbosity(verbosity);
  evtRhoQA->add_area_rho_node("TowerRho_AREA");
  evtRhoQA->add_mult_rho_node("TowerRho_MULT");
  if (trg.has_value())
  {
    evtRhoQA->setTrgToSelect(trg.value());
  }
  se->registerSubsystem(evtRhoQA);

  // initialize and register jet seed counter qa module
  if (!JetQA::DoPP)
  {
    JetSeedCount* jetSeedQA = new JetSeedCount(
        "JetSeedCount" + trig_tag + "_towersub1_antikt_r02",
        "AntiKt_Tower_r04",  // n.b. unused in module
        "AntiKt_TowerInfo_HIRecoSeedsRaw_r02",
        "AntiKt_TowerInfo_HIRecoSeedsSub_r02");
    jetSeedQA->Verbosity(verbosity);
    jetSeedQA->setHistTag("");
    jetSeedQA->setPtRange(ptJetRange.first, ptJetRange.second);
    jetSeedQA->setEtaRange(JetQA::MinAcceptEta, JetQA::MaxAcceptEta);
    jetSeedQA->setWriteToOutputFile(false);
    jetSeedQA->setPPMode(JetQA::DoPP);
    if (trg.has_value())
    {
      jetSeedQA->setTrgToSelect(trg.value());
    }
    se->registerSubsystem(jetSeedQA);
  }

  // register underlying event module
  UEvsEtaCentrality* UEvsEtaCentralityQA = new UEvsEtaCentrality("UEvsEtaCent" + trig_tag);
  UEvsEtaCentralityQA->SetConfig(
      {
          .debug = false,
          .histTag = "",
      });
  if (trg.has_value())
  {
    UEvsEtaCentralityQA->SetConfig(
        {
            .doTrgSelect = true,
            .trgToSelect = trg.value(),
        });
  }
  se->registerSubsystem(UEvsEtaCentralityQA);

  // create modules that take single R values ---------------------------------

  // loop over resolution parameters
  for (uint32_t jet : vecJetsToQA)
  {
    // get R-dependent eta range
    std::pair<double, double> etaJetRange = JetQA::GetJetEtaRange(JetQA::JetRes[jet]);

    // initialize and register constituent checks
    ConstituentsinJets* jetCstQA = new ConstituentsinJets(
        "ConstituentsInJets" + trig_tag + "_" + JetQA::JetTag[jet],
        JetQA::JetInput[jet],
        "TowerInfoBackground_Sub1",
        "",
        "TOWERINFO_CALIB_CEMC_RETOWER",
        "TOWERINFO_CALIB_HCALIN",
        "TOWERINFO_CALIB_HCALOUT");
    jetCstQA->Verbosity(verbosity);
    jetCstQA->setPtRange(ptJetRange.first, ptJetRange.second);
    jetCstQA->setEtaRange(etaJetRange.first, etaJetRange.second);
    jetCstQA->setPPMode(JetQA::DoPP);
    if (trg.has_value())
    {
      jetCstQA->setTrgToSelect(trg.value());
    }
    se->registerSubsystem(jetCstQA);

  }  // end jet loop
  return;

}  // end 'JetsWithCaloQA(std::optional<uint32_t>)'

// ----------------------------------------------------------------------------
//! Run jet QA
// ----------------------------------------------------------------------------
void Jet_QA(const std::vector<uint32_t>& vecTrigsToUse = JetQA::GetDefaultTriggerList())
{
  // if running with calos, instantiate & register the
  // CaloStatusMapper QA module
  if (JetQA::HasCalos)
  {
    // set verbosity
    int verbosity = std::max(Enable::JETQA_VERBOSITY, Enable::QA_VERBOSITY);

    // connect to f4a server
    Fun4AllServer* se = Fun4AllServer::instance();

    // register module
    CaloStatusMapper* caloStatusQA = new CaloStatusMapper("CaloStatusMapper");
    caloStatusQA->SetConfig(
        {
            .debug = false,
            .doNorm = false,     // do NOT try to normalize histograms
            .doOptHist = false,  // turn off extra histograms
            .histTag = "",
            .doTrgSelect = false  // n.b. differential in trigger not useful here
        });

    se->Verbosity(verbosity);
    se->registerSubsystem(caloStatusQA);
  }

  // run in inclusive mode if needed
  if (JetQA::DoInclusive)
  {
    CommonJetQA();
    if (JetQA::HasTracks)
    {
      JetsWithTracksQA();
    }
    if (JetQA::HasCalos)
    {
      JetsWithCaloQA();
    }
  }

  // run in triggered mode if needed
  if (JetQA::DoTriggered)
  {
    for (uint32_t trg : vecTrigsToUse)
    {
      CommonJetQA(trg);
      if (JetQA::HasTracks)
      {
        JetsWithTracksQA(trg);
      }
      if (JetQA::HasCalos)
      {
        JetsWithCaloQA(trg);
      }
    }  // end trigger loop
  }
  return;

}  // end 'Jet_QA(std::vector<uint32_t>)'

#endif

// end ------------------------------------------------------------------------
