#ifndef MACRO_G4KFPARTICLE_C
#define MACRO_G4KFPARTICLE_C

#include <GlobalVariables.C>

#define HomogeneousField
#include <kfparticle_sphenix/KFParticle_sPHENIX.h>
#include <decayfinder/DecayFinder.h>
#include <qa_modules/QAG4SimulationKFParticle.h>

#include <fun4all/Fun4AllServer.h>

R__LOAD_LIBRARY(libkfparticle_sphenix.so)
R__LOAD_LIBRARY(libqa_kfparticle.so)
R__LOAD_LIBRARY(libdecayfinder.so)

namespace Enable
{
  bool KFPARTICLE = false;
  bool KFPARTICLE_SAVE_NTUPLE = false;
  bool KFPARTICLE_APPEND_TO_DST = true;
  bool KFPARTICLE_TRUTH_MATCH = false;
  bool KFPARTICLE_DETECTOR_INFO = false;
  int KFPARTICLE_VERBOSITY = 0;
  std::string KFPARTICLE_TRACKMAP = "SvtxTrackMap";
  std::string KFPARTICLE_VERTEXMAP = "SvtxVertexMap";
}  // namespace Enable

namespace KFPARTICLE
{
  bool QA = false;

  bool runUpsilonReco = false;
  std::string UpsilonName = "Upsilon";
  std::string UpsilonDecayDescriptor = UpsilonName + " -> e^+ e^-";
  std::pair<float, float> UpsilonMassRange(8, 11);
  bool UpsilonTrigger = false;
 
  bool runD0Reco = false;
  std::string D0Name = "D0";
  std::string D0DecayDescriptor = "[" + D0Name + " -> K^- pi^+]cc";
  std::pair<float, float> D0MassRange(1.75, 1.95);
  bool D0Trigger = false;

  bool runLambdacReco = false;
  std::string LambdacName = "Lambdac";
  std::string LambdacDecayDescriptor = "[" + LambdacName + " -> proton^+ K^- pi^+]cc";
  std::pair<float, float> LambdacMassRange(2.15, 2.45);
  bool LambdacTrigger = false;
} //namesppace KFPARTICLE

namespace KFParticleBaseCut
{
  float minTrackPT = 0.5; // GeV
  float maxTrackchi2nDoF = 2;
  float minTrackIPchi2 = 15; // IP = DCA of track with vertex
  float maxVertexchi2nDoF = 2;
  float maxTrackTrackDCA = 0.05; // cm
  float minMotherPT = 0; // GeV
}  // namespace KFParticleBaseCut

void KFParticle_Upsilon_Reco()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::KFPARTICLE_VERBOSITY);

  Fun4AllServer *se = Fun4AllServer::instance();

  KFParticle_sPHENIX *kfparticle = new KFParticle_sPHENIX("KFParticle_" + KFPARTICLE::UpsilonName + "_Reco");
  kfparticle->Verbosity(verbosity);
  kfparticle->setDecayDescriptor(KFPARTICLE::UpsilonDecayDescriptor);

  kfparticle->saveDST(Enable::KFPARTICLE_APPEND_TO_DST);
  kfparticle->saveOutput(Enable::KFPARTICLE_SAVE_NTUPLE);
  kfparticle->doTruthMatching(Enable::KFPARTICLE_TRUTH_MATCH);
  kfparticle->getDetectorInfo(Enable::KFPARTICLE_DETECTOR_INFO);

  kfparticle->setMinimumTrackPT(KFParticleBaseCut::minTrackPT);
  kfparticle->setMinimumTrackIPchi2(0); // Upsilon decays are prompt, tracks are more likely to point to vertex
  kfparticle->setMaximumTrackchi2nDOF(KFParticleBaseCut::maxTrackchi2nDoF);

  kfparticle->setMaximumVertexchi2nDOF(KFParticleBaseCut::maxVertexchi2nDoF);
  kfparticle->setMaximumDaughterDCA(KFParticleBaseCut::maxTrackTrackDCA);

  kfparticle->setMinimumMass(KFPARTICLE::UpsilonMassRange.first);
  kfparticle->setMaximumMass(KFPARTICLE::UpsilonMassRange.second);
  kfparticle->setMotherPT(KFParticleBaseCut::minMotherPT);
  kfparticle->constrainToPrimaryVertex(false);

  kfparticle->setContainerName(KFPARTICLE::UpsilonName);
  kfparticle->setOutputName("KFParticleOutput_" + KFPARTICLE::UpsilonName + "_reconstruction.root");

  se->registerSubsystem(kfparticle);

  KFPARTICLE::runUpsilonReco = true;

  return;
}


void KFParticle_D0_Reco()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::KFPARTICLE_VERBOSITY);

  Fun4AllServer *se = Fun4AllServer::instance();

  KFParticle_sPHENIX *kfparticle = new KFParticle_sPHENIX("KFParticle_" + KFPARTICLE::D0Name + "_Reco");
  kfparticle->Verbosity(verbosity);
  kfparticle->setDecayDescriptor(KFPARTICLE::D0DecayDescriptor);

  kfparticle->saveDST(Enable::KFPARTICLE_APPEND_TO_DST);
  kfparticle->saveOutput(Enable::KFPARTICLE_SAVE_NTUPLE);
  kfparticle->doTruthMatching(Enable::KFPARTICLE_TRUTH_MATCH);
  kfparticle->getDetectorInfo(Enable::KFPARTICLE_DETECTOR_INFO);

  kfparticle->setMinimumTrackPT(KFParticleBaseCut::minTrackPT);
  kfparticle->setMinimumTrackIPchi2(KFParticleBaseCut::minTrackIPchi2);
  kfparticle->setMaximumTrackchi2nDOF(KFParticleBaseCut::maxTrackchi2nDoF);

  kfparticle->setMaximumVertexchi2nDOF(KFParticleBaseCut::maxVertexchi2nDoF);
  kfparticle->setMaximumDaughterDCA(KFParticleBaseCut::maxTrackTrackDCA);

  kfparticle->setMinimumMass(KFPARTICLE::D0MassRange.first);
  kfparticle->setMaximumMass(KFPARTICLE::D0MassRange.second);
  kfparticle->setMotherPT(KFParticleBaseCut::minMotherPT);
  kfparticle->constrainToPrimaryVertex(false);

  kfparticle->setContainerName(KFPARTICLE::D0Name);
  kfparticle->setOutputName("KFParticleOutput_" + KFPARTICLE::D0Name + "_reconstruction.root");

  se->registerSubsystem(kfparticle);

  KFPARTICLE::runD0Reco = true;

  return;
}


void KFParticle_Lambdac_Reco()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::KFPARTICLE_VERBOSITY);

  Fun4AllServer *se = Fun4AllServer::instance();

  KFParticle_sPHENIX *kfparticle = new KFParticle_sPHENIX("KFParticle_" + KFPARTICLE::LambdacName + "_Reco");
  kfparticle->Verbosity(verbosity);
  kfparticle->setDecayDescriptor(KFPARTICLE::LambdacDecayDescriptor);

  kfparticle->saveDST(Enable::KFPARTICLE_APPEND_TO_DST);
  kfparticle->saveOutput(Enable::KFPARTICLE_SAVE_NTUPLE);
  kfparticle->doTruthMatching(Enable::KFPARTICLE_TRUTH_MATCH);
  kfparticle->getDetectorInfo(Enable::KFPARTICLE_DETECTOR_INFO);

  kfparticle->setMinimumTrackPT(KFParticleBaseCut::minTrackPT);
  kfparticle->setMinimumTrackIPchi2(KFParticleBaseCut::minTrackIPchi2);
  kfparticle->setMaximumTrackchi2nDOF(KFParticleBaseCut::maxTrackchi2nDoF);

  kfparticle->setMaximumVertexchi2nDOF(KFParticleBaseCut::maxVertexchi2nDoF);
  kfparticle->setMaximumDaughterDCA(KFParticleBaseCut::maxTrackTrackDCA);

  kfparticle->setMinimumMass(KFPARTICLE::LambdacMassRange.first);
  kfparticle->setMaximumMass(KFPARTICLE::LambdacMassRange.second);
  kfparticle->setMotherPT(KFParticleBaseCut::minMotherPT);
  kfparticle->constrainToPrimaryVertex(false);

  kfparticle->setContainerName(KFPARTICLE::LambdacName);
  kfparticle->setOutputName("KFParticleOutput_" + KFPARTICLE::LambdacName + "_reconstruction.root");

  se->registerSubsystem(kfparticle);

  KFPARTICLE::runLambdacReco = true;

  return;
}

void KFParticle_QA()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::KFPARTICLE_VERBOSITY);

  Fun4AllServer *se = Fun4AllServer::instance();

  if (KFPARTICLE::runUpsilonReco)
  {
    DecayFinder *UpsilonFinder = new DecayFinder("DecayFinder_" + KFPARTICLE::UpsilonName);
    UpsilonFinder->Verbosity(verbosity);
    UpsilonFinder->setDecayDescriptor(KFPARTICLE::UpsilonDecayDescriptor);
    UpsilonFinder->triggerOnDecay(KFPARTICLE::UpsilonTrigger);
    UpsilonFinder->saveDST(true);
    UpsilonFinder->allowPi0(true);
    UpsilonFinder->allowPhotons(true);
    se->registerSubsystem(UpsilonFinder);

    QAG4SimulationKFParticle *UpsilonQA = new QAG4SimulationKFParticle("QA_" + KFPARTICLE::UpsilonName, 
      								       KFPARTICLE::UpsilonName, 
								       KFPARTICLE::UpsilonMassRange.first,  
								       KFPARTICLE::UpsilonMassRange.second);
    se->registerSubsystem(UpsilonQA);
  }

  if (KFPARTICLE::runD0Reco)
  {
    DecayFinder *D0Finder = new DecayFinder("DecayFinder_" + KFPARTICLE::D0Name);
    D0Finder->Verbosity(verbosity);
    D0Finder->setDecayDescriptor(KFPARTICLE::D0DecayDescriptor);
    D0Finder->triggerOnDecay(KFPARTICLE::D0Trigger);
    D0Finder->saveDST(true);
    D0Finder->allowPi0(true);
    D0Finder->allowPhotons(true);
    se->registerSubsystem(D0Finder);

    QAG4SimulationKFParticle *D0QA = new QAG4SimulationKFParticle("QA_" + KFPARTICLE::D0Name,
                                                                  KFPARTICLE::D0Name,
                                                                  KFPARTICLE::D0MassRange.first,
                                                                  KFPARTICLE::D0MassRange.second);
    se->registerSubsystem(D0QA);
  }

  if (KFPARTICLE::runLambdacReco)
  {
    DecayFinder *LambdacFinder = new DecayFinder("DecayFinder_" + KFPARTICLE::LambdacName);
    LambdacFinder->Verbosity(verbosity);
    LambdacFinder->setDecayDescriptor(KFPARTICLE::LambdacDecayDescriptor);
    LambdacFinder->triggerOnDecay(KFPARTICLE::LambdacTrigger);
    LambdacFinder->saveDST(true);
    LambdacFinder->allowPi0(true);
    LambdacFinder->allowPhotons(true);
    se->registerSubsystem(LambdacFinder);

    QAG4SimulationKFParticle *LambdacQA = new QAG4SimulationKFParticle("QA_" + KFPARTICLE::LambdacName, 
                                                                       KFPARTICLE::LambdacName, 
                                                                       KFPARTICLE::LambdacMassRange.first,  
                                                                       KFPARTICLE::LambdacMassRange.second);
    se->registerSubsystem(LambdacQA);
  }

  return;
}

#endif
