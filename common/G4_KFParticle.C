#ifndef MACRO_G4KFPARTICLE_C
#define MACRO_G4KFPARTICLE_C

#include <GlobalVariables.C>

#define HomogeneousField
#include <kfparticle_sphenix/KFParticle_sPHENIX.h>
#include <decayfinder/DecayFinder.h>

#include <fun4all/Fun4AllServer.h>

R__LOAD_LIBRARY(libkfparticle_sphenix.so)

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

namespace KFParticle
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
  std::string LambdacDecayDescriptor = "[" + motherName + " -> proton^+ K^- pi^+]cc";
  std::pair<float, float> LambdacMassRange(2.15, 2.45);
  bool LambdacTrigger = false;
} //namesppace KFParticle

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

  KFParticle_sPHENIX *kfparticle = new KFParticle_sPHENIX("KFParticle_" + KFParticle::UpsilonName + "_Reco");
  kfparticle->Verbosity(verbosity);
  kfparticle->setDecayDescriptor(KFParticle::UpsilonDecayDescriptor);

  kfparticle->saveDST(Enable::KFPARTICLE_APPEND_TO_DST);
  kfparticle->saveOutput(Enable::KFPARTICLE_SAVE_NTUPLE);
  kfparticle->doTruthMatching(Enable::KFPARTICLE_TRUTH_MATCH);
  kfparticle->getDetectorInfo(Enable::KFPARTICLE_DETECTOR_INFO);

  kfparticle->setMinimumTrackPT(KFParticleBaseCut::minTrackPT);
  kfparticle->setMinimumTrackIPchi2(0); // Upsilon decays are prompt, tracks are more likely to point to vertex
  kfparticle->setMaximumTrackchi2nDOF(KFParticleBaseCut::maxTrackchi2nDoF);

  kfparticle->setMaximumVertexchi2nDOF(KFParticleBaseCut::maxVertexchi2nDoF);
  kfparticle->setMaximumDaughterDCA(KFParticleBaseCut::maxTrackTrackDCA);

  kfparticle->setMinimumMass(UpsilonMassRange.first);
  kfparticle->setMaximumMass(UpsilonMassRange.second);
  kfparticle->setMotherPT(KFParticleBaseCut::minMotherPT);
  kfparticle->constrainToPrimaryVertex(false);

  kfparticle->setContainerName(KFParticle::UpsilonName);
  kfparticle->setOutputName("KFParticleOutput_" + KFParticle::UpsilonName + "_reconstruction.root");

  se->registerSubsystem(kfparticle);

  KFParticle::runUpsilonReco = true;

  return;
}


void KFParticle_D0_Reco()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::KFPARTICLE_VERBOSITY);

  Fun4AllServer *se = Fun4AllServer::instance();

  KFParticle_sPHENIX *kfparticle = new KFParticle_sPHENIX("KFParticle_" + KFParticle::D0Name + "_Reco");
  kfparticle->Verbosity(verbosity);
  kfparticle->setDecayDescriptor(KFParticle::D0DecayDescriptor);

  kfparticle->saveDST(Enable::KFPARTICLE_APPEND_TO_DST);
  kfparticle->saveOutput(Enable::KFPARTICLE_SAVE_NTUPLE);
  kfparticle->doTruthMatching(Enable::KFPARTICLE_TRUTH_MATCH);
  kfparticle->getDetectorInfo(Enable::KFPARTICLE_DETECTOR_INFO);

  kfparticle->setMinimumTrackPT(KFParticleBaseCut::minTrackPT);
  kfparticle->setMinimumTrackIPchi2(KFParticleBaseCut::minTrackIPchi2);
  kfparticle->setMaximumTrackchi2nDOF(KFParticleBaseCut::maxTrackchi2nDoF);

  kfparticle->setMaximumVertexchi2nDOF(KFParticleBaseCut::maxVertexchi2nDoF);
  kfparticle->setMaximumDaughterDCA(KFParticleBaseCut::maxTrackTrackDCA);

  kfparticle->setMinimumMass(D0MassRange.first);
  kfparticle->setMaximumMass(D0MassRange.second);
  kfparticle->setMotherPT(KFParticleBaseCut::minMotherPT);
  kfparticle->constrainToPrimaryVertex(false);

  kfparticle->setContainerName(KFParticle::D0Name);
  kfparticle->setOutputName("KFParticleOutput_" + KFParticle::D0Name + "_reconstruction.root");

  se->registerSubsystem(kfparticle);

  KFParticle::runD0Reco = true;

  return;
}


void KFParticle_Lambdac_Reco()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::KFPARTICLE_VERBOSITY);

  Fun4AllServer *se = Fun4AllServer::instance();

  KFParticle_sPHENIX *kfparticle = new KFParticle_sPHENIX("KFParticle_" + KFParticle::LambdacName + "_Reco");
  kfparticle->Verbosity(verbosity);
  kfparticle->setDecayDescriptor(KFParticle::LambdacDecayDescriptor);

  kfparticle->saveDST(Enable::KFPARTICLE_APPEND_TO_DST);
  kfparticle->saveOutput(Enable::KFPARTICLE_SAVE_NTUPLE);
  kfparticle->doTruthMatching(Enable::KFPARTICLE_TRUTH_MATCH);
  kfparticle->getDetectorInfo(Enable::KFPARTICLE_DETECTOR_INFO);

  kfparticle->setMinimumTrackPT(KFParticleBaseCut::minTrackPT);
  kfparticle->setMinimumTrackIPchi2(KFParticleBaseCut::minTrackIPchi2);
  kfparticle->setMaximumTrackchi2nDOF(KFParticleBaseCut::maxTrackchi2nDoF);

  kfparticle->setMaximumVertexchi2nDOF(KFParticleBaseCut::maxVertexchi2nDoF);
  kfparticle->setMaximumDaughterDCA(KFParticleBaseCut::maxTrackTrackDCA);

  kfparticle->setMinimumMass(LambdacMassRange.first);
  kfparticle->setMaximumMass(LambdacMassRange.second);
  kfparticle->setMotherPT(KFParticleBaseCut::minMotherPT);
  kfparticle->constrainToPrimaryVertex(false);

  kfparticle->setContainerName(KFParticle::LambdacName);
  kfparticle->setOutputName("KFParticleOutput_" + KFParticle::LambdacName + "_reconstruction.root");

  se->registerSubsystem(kfparticle);

  KFParticle::runLambdacReco = true;

  return;
}

void KFParticle_QA()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::KFPARTICLE_VERBOSITY);

  Fun4AllServer *se = Fun4AllServer::instance();

  if (KFParticle::runUpsilonReco)
  {
    DecayFinder *UpsilonFinder = new DecayFinder("DecayFinder_" + KFParticle::UpsilonName);
    UpsilonFinder->Verbosity(verbosity);
    UpsilonFinder->setDecayDescriptor(KFParticle::UpsilonDecayDescriptor);
    UpsilonFinder->triggerOnDecay(KFParticle::UpsilonTrigger);
    UpsilonFinder->saveDST(true);
    UpsilonFinder->allowPi0(true);
    UpsilonFinder->allowPhotons(true);
    se->registerSubsystem(UpsilonFinder);

    QAG4SimulationKFParticle *UpsilonQA = new QAG4SimulationKFParticle("QA_" + KFParticle::UpsilonName, 
      								       KFParticle::UpsilonName, 
								       UpsilonMassRange.first,  
								       UpsilonMassRange.second);
    se->registerSubsystem(UpsilonQA);
  }

  if (KFParticle::runD0Reco)
  {
    DecayFinder *D0Finder = new DecayFinder("DecayFinder_" + KFParticle::D0Name);
    D0Finder->Verbosity(verbosity);
    D0Finder->setDecayDescriptor(KFParticle::D0DecayDescriptor);
    D0Finder->triggerOnDecay(KFParticle::D0Trigger);
    D0Finder->saveDST(true);
    D0Finder->allowPi0(true);
    D0Finder->allowPhotons(true);
    se->registerSubsystem(D0Finder);

    QAG4SimulationKFParticle *D0QA = new QAG4SimulationKFParticle("QA_" + KFParticle::D0Name,
                                                                  KFParticle::D0Name,
                                                                  D0MassRange.first,
                                                                  D0MassRange.second);
    se->registerSubsystem(D0QA);
  }

  if (KFParticle::runLambdacReco)
  {
    DecayFinder *LambdacFinder = new DecayFinder("DecayFinder_" + KFParticle::LambdacName);
    LambdacFinder->Verbosity(verbosity);
    LambdacFinder->setDecayDescriptor(KFParticle::LambdacDecayDescriptor);
    LambdacFinder->triggerOnDecay(KFParticle::LambdacTrigger);
    LambdacFinder->saveDST(true);
    LambdacFinder->allowPi0(true);
    LambdacFinder->allowPhotons(true);
    se->registerSubsystem(LambdacFinder);

    QAG4SimulationKFParticle *LambdacQA = new QAG4SimulationKFParticle("QA_" + KFParticle::LambdacName, 
                                                                       KFParticle::LambdacName, 
                                                                       LambdacMassRange.first,  
                                                                       LambdacMassRange.second);
    se->registerSubsystem(LambdacQA);
  }

  return;
}

#endif
