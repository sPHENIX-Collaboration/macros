#ifndef MACRO_JETQA_C
#define MACRO_JETQA_C

#include <fun4all/Fun4AllServer.h>
#include <jetqa/ConstituentsinJets.h>
#include <jetqa/JetKinematicCheck.h>
#include <jetqa/JetQADefs.h>
#include <jetqa/JetSeedCount.h>
#include <jetqa/RhosinEvent.h>
#include <jetqa/StructureinJets.h>
#include <jetqa/TrksInJetQA.h>
#include <map>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include <HIJetReco.C>
#include <QA.C>

R__LOAD_LIBRARY(libjetqa.so)



// ----------------------------------------------------------------------------
//! Namespace to collect various enums, default arguments, etc.
// ----------------------------------------------------------------------------
namespace JetQA
{

  // flags --------------------------------------------------------------------

  //! Set to true if input jets utilize tracks (e.g. via particle flow)
  bool HasTracks = false;

  //! Set to true to generate histograms for no trigger selection
  bool DoInclusive = true;

  //! Set to true to generate histograms for a specified set of triggers
  bool DoTriggered = true;



  // enums --------------------------------------------------------------------

  enum Type
  {
    AntiKtTowerSubR02,
    AntiKtTowerSubR03,
    AntiKtTowerSubR04,
    AntiKtTowerSubR05
  };



  // constants ----------------------------------------------------------------

  //! Max jet pt in GeV/c
  double MaxJetPt = 100.;

  //! Min eta acceptance
  double MinAcceptEta = -1.1;

  //! Max eta acceptance
  double MaxAcceptEta = 1.1;

  //! Inclusive tag
  std::string InclusiveTag = "inclusive";



  // maps ---------------------------------------------------------------------

  std::map<uint32_t, std::string> GL1Tag = {
    {JetQADefs::GL1::Clock, "clock"},
    {JetQADefs::GL1::ZDCS, "zdcs"},
    {JetQADefs::GL1::ZDCN, "zdcn"},
    {JetQADefs::GL1::ZDCNS, "zdcns"},
    {JetQADefs::GL1::Random, "random"},
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
    {JetQADefs::GL1::Photon4, "photon4"} 
  };

  std::map<uint32_t, std::string> JetInput = {
    {Type::AntiKtTowerSubR02, "AntiKt_Tower_r02_Sub1"},
    {Type::AntiKtTowerSubR03, "AntiKt_Tower_r03_Sub1"},
    {Type::AntiKtTowerSubR04, "AntiKt_Tower_r04_Sub1"},
    {Type::AntiKtTowerSubR05, "AntiKt_Tower_r05_Sub1"}
  };

  std::map<uint32_t, std::string> JetTag = {
    {Type::AntiKtTowerSubR02, "towersub1_antikt_r02"},
    {Type::AntiKtTowerSubR03, "towersub1_antikt_r03"},
    {Type::AntiKtTowerSubR04, "towersub1_antikt_r04"},
    {Type::AntiKtTowerSubR05, "towersub1_antikt_r05"}
  };



  // methods ------------------------------------------------------------------

  // --------------------------------------------------------------------------
  //! Get minimum jet pt based on which trigger fired
  // --------------------------------------------------------------------------
  // FIXME it might be prudent to allow for thresholds to change vs. run
  //   number... Before run 46038, the Jet1, 2 thresholds were different
  inline double GetMinJetPt(const uint32_t trg = JetQADefs::GL1::MBDNSJet1)
  {

    double ptJetMin;
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
    return ptJetMin;

  }  // end 'GetMinJetPt(uint32_t)'



  // --------------------------------------------------------------------------
  //! Get default jet pt range
  // --------------------------------------------------------------------------
  inline std::pair<double, double> GetDefaultJetPtRange(std::optional<uint32_t> trg = std::nullopt)
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

  }  // end 'GetDefaultJetPtRange(std::optional<uint32_t>)'



  // --------------------------------------------------------------------------
  //! Get default jet eta range
  // --------------------------------------------------------------------------
  inline std::pair<double, double> GetDefaultJetEtaRange(const double res = 0.)
  {

    const double etaMin = MinAcceptEta + res;
    const double etaMax = MaxAcceptEta - res;
    std::pair<double, double> etaJetRange = {etaMin, etaMax};
    return etaJetRange;

  }  // end 'GetDefaultJetEtaRange(double)'



  // --------------------------------------------------------------------------
  //! Get default list of triggers to analyze
  // --------------------------------------------------------------------------
  inline std::vector<uint32_t> GetDefaultTriggerList()
  {

    std::vector<uint32_t> vecDefaultTrigs = {
      JetQADefs::GL1::MBDNS1,
      JetQADefs::GL1::MBDNSJet1,
      JetQADefs::GL1::MBDNSJet2,
      JetQADefs::GL1::MBDNSJet3,
      JetQADefs::GL1::MBDNSJet4
    };
    return vecDefaultTrigs;

  }  // end 'GetDefaultTriggerList()'



  // --------------------------------------------------------------------------
  //! Get list of jets to analyze
  // --------------------------------------------------------------------------
  inline std::vector<uint32_t> GetJetsToQA()
  {

    std::vector<uint32_t> vecJetsToQA = {
      Type::AntiKtTowerSubR02,
      Type::AntiKtTowerSubR03,
      Type::AntiKtTowerSubR04,
      Type::AntiKtTowerSubR05
    };
    return vecJetsToQA;

  }  // end 'GetJetsToQA()'

}  // end JetQA namespace



// ----------------------------------------------------------------------------
//! Create QA modules for all jets, regardless of constituents
// ----------------------------------------------------------------------------
void CommonJetQA(std::optional<uint32_t> trg = std::nullopt)
{

  // set verbosity
  int verbosity = std::max(Enable::QA_VERBOSITY, Enable::HIJETS_VERBOSITY);

  // if selecting a trigger, add correpsonding tag
  //   otherwise label as "inclusive"
  std::string trig_tag("");
  if (trg.has_value())
  {
    trig_tag.append("_" + JetQA::GL1Tag[trg.value()]);
  }
  else
  {
    trig_tag.append("_" + JetQA::InclusiveTag);
  }

  // grab default pt, eta ranges
  std::pair<double, double> ptJetRange = JetQA::GetDefaultJetPtRange(trg);
  std::pair<double, double> etaJetRange = JetQA::GetDefaultJetEtaRange();

  // get list of jet nodes to analyze
  std::vector<uint32_t> vecJetsToQA = JetQA::GetJetsToQA();

  // connect to f4a server
  Fun4AllServer* se = Fun4AllServer::instance();

  // create modules that are independent of/take multiple R values ------------

  // initialize and register event-wise rho check
  RhosinEvent* evtRhoQA = new RhosinEvent("EventWiseRho" + trig_tag, "");
  evtRhoQA -> Verbosity(verbosity); 
  evtRhoQA -> add_area_rho_node("TowerRho_AREA");
  evtRhoQA -> add_mult_rho_node("TowerRho_MULT");
  if (trg.has_value())
  {
    evtRhoQA -> setTrgToSelect(trg.value());
  }
  se -> registerSubsystem(evtRhoQA);

  // initialize and register mass, eta, and pt qa module 
  JetKinematicCheck* kinematicQA = new JetKinematicCheck(
    "JetKinematicCheck" + trig_tag + "_towersub1_antikt",
    JetQA::JetInput[JetQA::Type::AntiKtTowerSubR02],
    JetQA::JetInput[JetQA::Type::AntiKtTowerSubR03],
    JetQA::JetInput[JetQA::Type::AntiKtTowerSubR04],
    JetQA::JetInput[JetQA::Type::AntiKtTowerSubR05]
  );
  kinematicQA -> Verbosity(verbosity);
  kinematicQA -> setHistTag("");
  kinematicQA -> setPtRange(ptJetRange.first, ptJetRange.second);
  kinematicQA -> setEtaRange(etaJetRange.first, etaJetRange.second);
  if (trg.has_value())
  {
    kinematicQA -> setTrgToSelect(trg.value());
  }
  se -> registerSubsystem(kinematicQA);

  // create modules that take single R values ---------------------------------

  for (uint32_t jet : vecJetsToQA)
  {

    // initialize and register jet seed counter qa module
    JetSeedCount* jetSeedQA = new JetSeedCount(
      "JetSeedCount" + trig_tag + "_" + JetQA::JetTag[jet],
      JetQA::JetInput[jet],
      "AntiKt_TowerInfo_HIRecoSeedsRaw_r02",
      "AntiKt_TowerInfo_HIRecoSeedsSub_r02"
    );
    jetSeedQA -> Verbosity(verbosity);
    jetSeedQA -> setHistTag("");
    jetSeedQA -> setPtRange(ptJetRange.first, ptJetRange.second);
    jetSeedQA -> setEtaRange(etaJetRange.first, etaJetRange.second);
    jetSeedQA -> setWriteToOutputFile(false);
    jetSeedQA -> setPPMode(HIJETS::is_pp);
    if (trg.has_value())
    {
      jetSeedQA -> setTrgToSelect(trg.value());
    }
    se -> registerSubsystem(jetSeedQA);

    // initialize and register constituent checks
    ConstituentsinJets* jetCstQA = new ConstituentsinJets(
      "ConstituentsInJets" + trig_tag + "_" + JetQA::JetTag[jet],
      JetQA::JetInput[jet],
      "TowerInfoBackground_Sub1",
      ""
    );
    jetCstQA -> Verbosity(verbosity);
    jetCstQA -> setPtRange(ptJetRange.first, ptJetRange.second);
    jetCstQA -> setEtaRange(etaJetRange.first, etaJetRange.second);
    if (trg.has_value())
    {
      jetCstQA -> setTrgToSelect();
    }
    se -> registerSubsystem(jetCstQA);

  }  // end jet loop
  return;

}  // end 'CommonJetQA(std::optional<uint32_t>)'



// ----------------------------------------------------------------------------
//! Create QA modules for jets with tracks
// ----------------------------------------------------------------------------
void JetsWithTracksQA(std::optional<uint32_t> trg = std::nullopt)
{

  // set verbosity
  int verbosity = std::max(Enable::QA_VERBOSITY, Enable::HIJETS_VERBOSITY);

  // if selecting a trigger, add correpsonding tag
  //   otherwise label as "inclusive"
  std::string trig_tag("");
  if (trg.has_value())
  {
    trig_tag.append("_" + JetQA::GL1Tag[trg.value()]);
  }
  else
  {
    trig_tag.append("_" + JetQA::InclusiveTag);
  }

  // grab default pt, eta ranges
  std::pair<double, double> ptJetRange = JetQA::GetDefaultJetPtRange(trg);
  std::pair<double, double> etaJetRange = JetQA::GetDefaultJetEtaRange();

  // get list of jet nodes to analyze
  std::vector<uint32_t> vecJetsToQA = JetQA::GetJetsToQA();

  // connect to f4a server
  Fun4AllServer* se = Fun4AllServer::instance();

  // create modules that take single R values ---------------------------------

  for (uint32_t jet : vecJetsToQA)
  {

    // intialize and register sum track vs. jet pt qa module
    StructureinJets* jetStructQA = new StructureinJets(
      "StructureInJets" + trig_tag + "_" + JetQA::JetTag[jet],
      JetQA::JetInput[jet],
      ""
    );
    jetStructQA -> Verbosity(verbosity);
    jetStructQA -> writeToOutputFile(false);
    if (trg.has_value())
    {
      jetStructQA -> setTrgToSelect(trg.value());
    }
    se -> registerSubsystem(jetStructQA);

    // initialize and register track jet qa module
    TrksInJetQA* trksInJetQA = new TrksInJetQA("TrksInJet" + trig_tag + "_" + JetQA::JetTag[jet]);
    trksInJetQA -> SetHistSuffix("");
    trksInJetQA -> Configure(
      {
        .outMode     = TrksInJetQA::OutMode::QA,
        .verbose     = Enable::QA_VERBOSITY,
        .doDebug     = false,
        .doInclusive = false,
        .doInJet     = true,
        .doHitQA     = false,
        .doClustQA   = true,
        .doTrackQA   = true,
        .doJetQA     = true,
        .rJet        = 0.4,
        .jetInNode   = JetQA::JetInput[jet]
      }
    );
    if (trg.has_value())
    {
      trksInJetQA -> SetTrgToSelect(trg.value());
    }
    se -> registerSubsystem(trksInJetQA);

  }  // end jet loop
  return;

}  // end 'JetWithTracksQA(std::optional<uint32_t>)'



// ----------------------------------------------------------------------------
//! Run jet QA
// ----------------------------------------------------------------------------
void Jet_QA(std::vector<uint32_t> vecTrigsToUse = JetQA::GetDefaultTriggerList())
{

  // run in inclusive mode if needed
  if (JetQA::DoInclusive)
  {
    CommonJetQA();
    if (JetQA::HasTracks)
    {
      JetsWithTracksQA();
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
    }  // end trigger loop
  }
  return;

}  // end 'Jet_QA(std::vector<uint32_t>)'

#endif

// end ---------------------------------------------
