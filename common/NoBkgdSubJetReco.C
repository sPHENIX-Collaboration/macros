#ifndef MACRO_NOBKGDSUBJETRECO_C
#define MACRO_NOBKGDSUBJETRECO_C

#include <GlobalVariables.C>

#include <jetbase/FastJetAlgo.h>
#include <jetbase/FastJetOptions.h>
#include <jetbase/JetReco.h>
#include <particleflowreco/ParticleFlowJetInput.h>
#include <jetbase/TowerJetInput.h>
#include <jetbase/TrackJetInput.h>
#include <g4jets/TruthJetInput.h>

#include <globalvertex/GlobalVertex.h>

#include <fun4all/Fun4AllServer.h>

R__LOAD_LIBRARY(libparticleflow.so)
R__LOAD_LIBRARY(libjetbase.so)
R__LOAD_LIBRARY(libg4jets.so)


// ----------------------------------------------------------------------------
//! General options for no subtraction (NS) jet reconstruction
// ----------------------------------------------------------------------------
namespace Enable
{
  int  NSJETS_VERBOSITY = 0;      ///< verbosity
  bool NSJETS           = false;  ///< do no-subtraction jet reco
  bool NSJETS_MC        = false;  ///< is simulation
  bool NSJETS_TRUTH     = false;  ///< make truth jets
  bool NSJETS_TOWER     = false;  ///< make tower jets
  bool NSJETS_TRACK     = true;   ///< make track jets
  bool NSJETS_PFLOW     = false;  ///< make particle flow jets
}  // end namespace Enable


// ----------------------------------------------------------------------------
//! Options specific to no subtraction jet reconstruction
// ----------------------------------------------------------------------------
namespace NSJETS
{
  ///! turn on/off functionality only relevant for
  ///! nucleon collisions
  bool is_pp = false;

  ///! sets prefix of nodes to use as tower jet
  ///! input
  std::string tower_prefix = "TOWERINFO_CALIB";

  ///! if true, sets vertex type to type specified
  ///! by vertex_type
  bool do_vertex_type = true;

  ///! specifies type of vertex to use
  GlobalVertex::VTXTYPE vertex_type = GlobalVertex::MBD;

  ///! Base fastjet options to use. Note that the
  ///! resolution parameter will be overwritten
  ///! to R = 0.2, 0.3, 0.4, and 0.5
  FastJetOptions fj_opts({Jet::ANTIKT, JET_R, 0.4, VERBOSITY, static_cast<float>(Enable::HIJETS_VERBOSITY)});

  ///! sets jet node name
  std::string jet_node = "ANTIKT";

  ///! sets prefix of nodes to store jets
  std::string algo_prefix = "AntiKt";

  ///! enumerates reconstructed resolution
  ///! parameters
  enum Res {
    R02 = 0,
    R03 = 1,
    R04 = 2,
    R05 = 3
  };

  // --------------------------------------------------------------------------
  //! Helper method to generate releveant FastJet algorithms
  // --------------------------------------------------------------------------
  std::vector<FastJetAlgo*> GetFJAlgorithms()
  {
    // algorithms to run
    std::vector<FastJetAlgo*> algos;

    // grab current options
    FastJetOptions opts = fj_opts;

    // lambda to update opts' reso. parameter
    // and add to vector of algorithms
    auto addResoToAlgos = [&opts, &algos](const float reso) {
      opts.jet_R = reso;
      algos.push_back( new FastJetAlgo(opts) );
      return;
    };

    // create and add all algorithms
    addResoToAlgos(0.2);
    addResoToAlgos(0.3);
    addResoToAlgos(0.4);
    addResoToAlgos(0.5);
    return algos;
  }  // end 'GetFJOptions()'
}  // end namespace NSJETS


// ----------------------------------------------------------------------------
//! Make jets out of appropriate truth particles
// ----------------------------------------------------------------------------
void MakeNSTruthJets()
{

  // set verbosity
  int verbosity = std::max(Enable::VERBOSITY, Enable::NSJETS_VERBOSITY);

  // generate fastjet algorithms
  auto algos = NSJETS::GetFJAlgorithms();

  //---------------
  // Fun4All server
  //---------------
  Fun4AllServer *se = Fun4AllServer::instance();

  // if making track jets, make truth jets out of only charged particles
  if (Enable::NSJETS_TRACK)
  {
    // book jet reconstruction on charged FS particles
    JetReco* chrgTruthJets = new JetReco();
    chrgTruthJets->add_input(new TruthJetInput(Jet::SRC::CHARGED_PARTICLE));
    chrgTruthJets->add_algo(algos[NSJETS::Res::R02], NSJETS::algo_prefix + "_ChargedTruth_r02");
    chrgTruthJets->add_algo(algos[NSJETS::Res::R03], NSJETS::algo_prefix + "_ChargedTruth_r03");
    chrgTruthJets->add_algo(algos[NSJETS::Res::R04], NSJETS::algo_prefix + "_ChargedTruth_r04");
    chrgTruthJets->add_algo(algos[NSJETS::Res::R05], NSJETS::algo_prefix + "_ChargedTruth_r05");
    chrgTruthJets->set_algo_node(NSJETS::jet_node);
    chrgTruthJets->set_input_node("TRUTH");
    chrgTruthJets->Verbosity(verbosity);
    se->registerSubsystem(chrgTruthJets);
  }

  // if making tower or pflow jets, make truth jets out of all particles
  if (Enable::NSJETS_TOWER || Enable::NSJETS_PFLOW)
  {
    // book jet reconstruction on all particles
    JetReco* fullTruthJets = new JetReco();
    fullTruthJets->add_input(new TruthJetInput(Jet::PARTICLE));
    fullTruthJets->add_algo(algos[NSJETS::Res::R02], NSJETS::algo_prefix + "_Truth_r02");
    fullTruthJets->add_algo(algos[NSJETS::Res::R03], NSJETS::algo_prefix + "_Truth_r03");
    fullTruthJets->add_algo(algos[NSJETS::Res::R04], NSJETS::algo_prefix + "_Truth_r04");
    fullTruthJets->add_algo(algos[NSJETS::Res::R05], NSJETS::algo_prefix + "_Truth_r05");
    fullTruthJets->set_algo_node(NSJETS::jet_node);
    fullTruthJets->set_input_node("TRUTH");
    fullTruthJets->Verbosity(verbosity);
    se->registerSubsystem(fullTruthJets);
  }

  // exit back to NoBkdSubJetReco()
  return;

}  // end 'MakeNSTruthJets()'


// ----------------------------------------------------------------------------
//! Make jets out of unsubtracted towers
// ----------------------------------------------------------------------------
void MakeNSTowerJets()
{

  // set verbosity
  int verbosity = std::max(Enable::VERBOSITY, Enable::NSJETS_VERBOSITY);

  // generate fastjet algorithms
  auto algos = NSJETS::GetFJAlgorithms();

  //---------------
  // Fun4All server
  //---------------
  Fun4AllServer *se = Fun4AllServer::instance();

  // retower the emcal to match I/OHCal granularity
  RetowerCEMC* rcemc = new RetowerCEMC(); 
  rcemc->Verbosity(verbosity); 
  rcemc->set_towerinfo(true);
  rcemc->set_frac_cut(0.5); //fraction of retower that must be masked to mask the full retower
  rcemc->set_towerNodePrefix(NSJETS::tower_prefix);
  se->registerSubsystem(rcemc);

  // create tower jet input and set vertex type
  TowerJetInput* emTwrInput = new TowerJetInput(Jet::CEMC_TOWERINFO_RETOWER, NSJETS::tower_prefix);
  TowerJetInput* ihTwrInput = new TowerJetInput(Jet::HCALIN_TOWERINFO, NSJETS::tower_prefix);
  TowerJetInput* ohTwrInput = new TowerJetInput(Jet::HCALOUT_TOWERINFO, NSJETS::tower_prefix);
  if (NSJETS::do_vertex_type)
  {
    emTwrInput->set_GlobalVertexType(NSJETS::vertex_type);
    ihTwrInput->set_GlobalVertexType(NSJETS::vertex_type);
    ohTwrInput->set_GlobalVertexType(NSJETS::vertex_type);
  }

  // book jet reconstruction on towers  
  JetReco* twrRecoJets = new JetReco();
  twrRecoJets->add_input(emTwrInput);
  twrRecoJets->add_input(ihTwrInput);
  twrRecoJets->add_input(ohTwrInput);
  twrRecoJets->add_algo(algos[NSJETS::Res::R02], NSJETS::algo_prefix + "_Tower_r02");
  twrRecoJets->add_algo(algos[NSJETS::Res::R03], NSJETS::algo_prefix + "_Tower_r03");
  twrRecoJets->add_algo(algos[NSJETS::Res::R04], NSJETS::algo_prefix + "_Tower_r04");
  twrRecoJets->add_algo(algos[NSJETS::Res::R05], NSJETS::algo_prefix + "_Tower_r05");
  twrRecoJets->set_algo_node(NSJETS::jet_node);
  twrRecoJets->set_input_node("TOWER");
  twrRecoJets->Verbosity(verbosity);
  se->registerSubsystem(twrRecoJets);

  // exit back to NoBkgdSubJetReco()
  return;

}  // end 'MakeNSTowerJets()'


// ----------------------------------------------------------------------------
//! Make jets out of tracks without background subtraction
// ----------------------------------------------------------------------------
void MakeNSTrackJets()
{

  // set verbosity
  int verbosity = std::max(Enable::VERBOSITY, Enable::NSJETS_VERBOSITY);

  auto algos = NSJETS::GetFJAlgorithms();
  //---------------
  // Fun4All server
  //---------------
  Fun4AllServer *se = Fun4AllServer::instance();

  // book jet reconstruction routines on tracks
  JetReco* trkRecoJets = new JetReco();
  trkRecoJets->add_input(new TrackJetInput(Jet::SRC::TRACK));
  trkRecoJets->add_algo(algos[NSJETS::Res::R02], NSJETS::algo_prefix + "_Track_r02");
  trkRecoJets->add_algo(algos[NSJETS::Res::R03], NSJETS::algo_prefix + "_Track_r03");
  trkRecoJets->add_algo(algos[NSJETS::Res::R04], NSJETS::algo_prefix + "_Track_r04");
  trkRecoJets->add_algo(algos[NSJETS::Res::R05], NSJETS::algo_prefix + "_Track_r05");
  trkRecoJets->set_algo_node(NSJETS::jet_node);
  trkRecoJets->set_input_node("TRACK");
  trkRecoJets->Verbosity(verbosity);
  se->registerSubsystem(trkRecoJets);

  // exit back to NoBkgdSubJetReco()
  return;

}  // end 'MakeNSTrackJets()'


// ----------------------------------------------------------------------------
//! Make jets out of particle-flow elements without background subtraction
// ----------------------------------------------------------------------------
void MakeNSPFlowJets()
{

  // set verbosity
  int verbosity = std::max(Enable::VERBOSITY, Enable::NSJETS_VERBOSITY);

  auto algos = NSJETS::GetFJAlgorithms();
  //---------------
  // Fun4All server
  //---------------
  Fun4AllServer *se = Fun4AllServer::instance();

  // book jet reconstruction routines on pflow elements
  JetReco* pfRecoJets = new JetReco();
  pfRecoJets->add_input(new ParticleFlowJetInput());
  pfRecoJets->add_algo(algos[NSJETS::Res::R02], NSJETS::algo_prefix + "_ParticleFlow_r02");
  pfRecoJets->add_algo(algos[NSJETS::Res::R03], NSJETS::algo_prefix + "_ParticleFlow_r03");
  pfRecoJets->add_algo(algos[NSJETS::Res::R04], NSJETS::algo_prefix + "_ParticleFlow_r04");
  pfRecoJets->add_algo(algos[NSJETS::Res::R05], NSJETS::algo_prefix + "_ParticleFlow_r05");
  pfRecoJets->set_algo_node(NSJETS::jet_node);
  pfRecoJets->set_input_node("ELEMENT");
  pfRecoJets->Verbosity(verbosity);
  se->registerSubsystem(pfRecoJets);

  // exit back to NoBkgdSubJetReco()
  return;

}  // end 'MakeNSPFlowJets()'


// ----------------------------------------------------------------------------
//! Run jet reconstruction without background subtraction
// ----------------------------------------------------------------------------
void NoBkgdSubJetReco()
{

  // if simulation, make appropriate truth jets
  if (Enable::NSJETS_MC && Enable::NSJETS_TRUTH) MakeNSTruthJets();

  // run approriate jet reconstruction routines
  if (Enable::NSJETS_TOWER) MakeNSTowerJets();
  if (Enable::NSJETS_TRACK) MakeNSTrackJets();
  if (Enable::NSJETS_PFLOW) MakeNSPFlowJets();

}  // end 'NoBkgdSubJetReco()'

#endif
