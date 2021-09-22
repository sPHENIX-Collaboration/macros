#ifndef MACRO_G4KFPARTICLE_C
#define MACRO_G4KFPARTICLE_C

#include <GlobalVariables.C>

#define HomogeneousField
#include <kfparticle_sphenix/KFParticle_sPHENIX.h>

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

namespace KFParticleBaseCut
{
  float minTrackPT = 0.5; // GeV
  float maxTrackchi2nDoF = 2;
  float minTrackIPchi2 = 15; // IP = DCA of track with vertex
  float maxVertexchi2nDoF = 2;
  float maxTrackTrackDCA = 0.05; // cm
  float minMotherPT = 0; // GeV
}  // namespace KFParticleBaseCut

void KFParticleInit() {} //I guess this line isnt needed

void KFParticle_Upsilon_Reco()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::KFPARTICLE_VERBOSITY);

  Fun4AllServer *se = Fun4AllServer::instance();

  std::string motherName = "Upsilon";
  KFParticle_sPHENIX *kfparticle = new KFParticle_sPHENIX("KFParticle_" + motherName + "_Reco");
  kfparticle->Verbosity(verbosity);

  kfparticle->saveDST(Enable::KFPARTICLE_APPEND_TO_DST);
  kfparticle->saveOutput(Enable::KFPARTICLE_SAVE_NTUPLE);
  kfparticle->doTruthMatching(Enable::KFPARTICLE_TRUTH_MATCH);
  kfparticle->getDetectorInfo(Enable::KFPARTICLE_DETECTOR_INFO);

  std::string decayDescriptor = motherName + " -> e^+ e^-";
  kfparticle->setDecayDescriptor(decayDescriptor);

  kfparticle->setContainerName(motherName);
  kfparticle->setOutputName("KFParticleOutput_" + motherName + "_reconstruction.root");

  kfparticle->setMinimumTrackPT(KFParticleBaseCut::minTrackPT);
  kfparticle->setMinimumTrackIPchi2(0); // Upsilon decays are prompt, tracks are more likely to point to vertex
  kfparticle->setMaximumTrackchi2nDOF(KFParticleBaseCut::maxTrackchi2nDoF);

  kfparticle->setMaximumVertexchi2nDOF(KFParticleBaseCut::maxVertexchi2nDoF);
  kfparticle->setMaximumDaughterDCA(KFParticleBaseCut::maxTrackTrackDCA);

  kfparticle->setMinimumMass(7);
  kfparticle->setMaximumMass(11);
  kfparticle->setMotherPT(KFParticleBaseCut::minMotherPT);
  kfparticle->constrainToPrimaryVertex(false);

  se->registerSubsystem(kfparticle);

  return;
}


void KFParticle_D0_Reco()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::KFPARTICLE_VERBOSITY);

  Fun4AllServer *se = Fun4AllServer::instance();

  std::string motherName = "D0";
  KFParticle_sPHENIX *kfparticle = new KFParticle_sPHENIX("KFParticle_" + motherName + "_Reco");
  kfparticle->Verbosity(verbosity);

  kfparticle->saveDST(Enable::KFPARTICLE_APPEND_TO_DST);
  kfparticle->saveOutput(Enable::KFPARTICLE_SAVE_NTUPLE);
  kfparticle->doTruthMatching(Enable::KFPARTICLE_TRUTH_MATCH);
  kfparticle->getDetectorInfo(Enable::KFPARTICLE_DETECTOR_INFO);

  std::string decayDescriptor = "[" + motherName + " -> K^- pi^+]cc";
  kfparticle->setDecayDescriptor(decayDescriptor);

  kfparticle->setContainerName(motherName);
  kfparticle->setOutputName("KFParticleOutput_" + motherName + "_reconstruction.root");

  kfparticle->setMinimumTrackPT(KFParticleBaseCut::minTrackPT);
  kfparticle->setMinimumTrackIPchi2(KFParticleBaseCut::minTrackIPchi2);
  kfparticle->setMaximumTrackchi2nDOF(KFParticleBaseCut::maxTrackchi2nDoF);

  kfparticle->setMaximumVertexchi2nDOF(KFParticleBaseCut::maxVertexchi2nDoF);
  kfparticle->setMaximumDaughterDCA(KFParticleBaseCut::maxTrackTrackDCA);

  kfparticle->setMinimumMass(1.750);
  kfparticle->setMaximumMass(1.950);
  kfparticle->setMotherPT(KFParticleBaseCut::minMotherPT);
  kfparticle->constrainToPrimaryVertex(false);

  se->registerSubsystem(kfparticle);

  return;
}


void KFParticle_Lambdac_Reco()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::KFPARTICLE_VERBOSITY);

  Fun4AllServer *se = Fun4AllServer::instance();

  std::string motherName = "Lambdac";
  KFParticle_sPHENIX *kfparticle = new KFParticle_sPHENIX("KFParticle_" + motherName + "_Reco");
  kfparticle->Verbosity(verbosity);

  kfparticle->saveDST(Enable::KFPARTICLE_APPEND_TO_DST);
  kfparticle->saveOutput(Enable::KFPARTICLE_SAVE_NTUPLE);
  kfparticle->doTruthMatching(Enable::KFPARTICLE_TRUTH_MATCH);
  kfparticle->getDetectorInfo(Enable::KFPARTICLE_DETECTOR_INFO);

  std::string decayDescriptor = "[" + motherName + " -> proton^+ K^- pi^+]cc";
  kfparticle->setDecayDescriptor(decayDescriptor);

  kfparticle->setContainerName(motherName);
  kfparticle->setOutputName("KFParticleOutput_" + motherName + "_reconstruction.root");

  kfparticle->setMinimumTrackPT(KFParticleBaseCut::minTrackPT);
  kfparticle->setMinimumTrackIPchi2(KFParticleBaseCut::minTrackIPchi2);
  kfparticle->setMaximumTrackchi2nDOF(KFParticleBaseCut::maxTrackchi2nDoF);

  kfparticle->setMaximumVertexchi2nDOF(KFParticleBaseCut::maxVertexchi2nDoF);
  kfparticle->setMaximumDaughterDCA(KFParticleBaseCut::maxTrackTrackDCA);

  kfparticle->setMinimumMass(2.150);
  kfparticle->setMaximumMass(2.400);
  kfparticle->setMotherPT(KFParticleBaseCut::minMotherPT);
  kfparticle->constrainToPrimaryVertex(false);

  se->registerSubsystem(kfparticle);

  return;
}


#endif
