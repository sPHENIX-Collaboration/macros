#ifndef MACRO_TRKRRECO_COSMICS_C
#define MACRO_TRKRRECO_COSMICS_C

#include <G4_TrkrVariables.C>

#include <g4eval/SvtxTruthRecoTableEval.h>
#include <trackingdiagnostics/TrackSeedTrackMapConverter.h>

#include <trackreco/MakeActsGeometry.h>
#include <trackreco/PHActsTrkFitter.h>
#include <trackreco/PHActsVertexPropagator.h>
#include <trackreco/PHCosmicSeeder.h>
#include <trackreco/PHCosmicSiliconPropagator.h>
#include <trackreco/PHCosmicTrackMerger.h>
#include <trackreco/PHCosmicsTrkFitter.h>
#include <trackreco/PHMicromegasTpcTrackMatching.h>
#include <trackreco/PHSiliconHelicalPropagator.h>
#include <trackreco/PHSimpleVertexFinder.h>
#include <trackreco/PHTpcDeltaZCorrection.h>
#include <trackreco/PHTrackCleaner.h>
#include <trackreco/SecondaryVertexFinder.h>

#include <tpc/TpcLoadDistortionCorrection.h>

#include <tpccalib/PHTpcResiduals.h>

#include <trackermillepedealignment/HelicalFitter.h>
#include <trackermillepedealignment/MakeMilleFiles.h>

#include <fun4all/Fun4AllServer.h>

#include <string>

R__LOAD_LIBRARY(libtrack_reco.so)
R__LOAD_LIBRARY(libtpccalib.so)
R__LOAD_LIBRARY(libtpc.so)
R__LOAD_LIBRARY(libtrackeralign.so)
R__LOAD_LIBRARY(libTrackingDiagnostics.so)
void convert_seeds()
{
  Fun4AllServer *se = Fun4AllServer::instance();
  int verbosity = std::max(Enable::VERBOSITY, Enable::TRACKING_VERBOSITY);

  TrackSeedTrackMapConverter *converter = new TrackSeedTrackMapConverter();
  // Default set to full SvtxTrackSeeds. Can be set to
  // SiliconTrackSeedContainer or TpcTrackSeedContainer
  converter->setTrackSeedName("SvtxTrackSeedContainer");
  converter->Verbosity(verbosity);
  converter->cosmics();
  converter->setFieldMap(G4MAGNET::magfield_tracking);
  se->registerSubsystem(converter);
}

void Tracking_Reco_TrackSeed()
{
  // set up verbosity
  int verbosity = std::max(Enable::VERBOSITY, Enable::TRACKING_VERBOSITY);

  // get fun4all server instance
  auto se = Fun4AllServer::instance();

  PHCosmicSeeder *seeder = new PHCosmicSeeder;
  seeder->Verbosity(verbosity);
  se->registerSubsystem(seeder);

  PHCosmicSiliconPropagator *hprop = new PHCosmicSiliconPropagator("HelicalPropagator");
  hprop->Verbosity(verbosity);
  double fieldstrength = std::numeric_limits<double>::quiet_NaN();
  bool ConstField = isConstantField(G4MAGNET::magfield_tracking,fieldstrength);
  if (ConstField && fieldstrength < 0.1)
  {
    hprop->zero_field();
  }
  hprop->set_dca_z_cut(2);
  hprop->set_dca_xy_cut(1.);
  se->registerSubsystem(hprop);

  // Associate Micromegas clusters with the tracks

  auto merger = new PHCosmicTrackMerger("PHCosmicMerger");
  merger->Verbosity(verbosity);
  if (ConstField && fieldstrength < 0.1)
  {
    merger->zero_field();
  }
  se->registerSubsystem(merger);

  PHCosmicSiliconPropagator *hprop2 = new PHCosmicSiliconPropagator("HelicalPropagator2");
  hprop2->Verbosity(verbosity);
  hprop2->resetSvtxSeedContainer();
  if (ConstField && fieldstrength < 0.1)
  {
    hprop2->zero_field();
  }
  hprop2->set_dca_z_cut(2.);
  hprop2->set_dca_xy_cut(0.5);

  se->registerSubsystem(hprop2);

  auto merger2 = new PHCosmicTrackMerger("PHCosmicMerger2");
  merger2->Verbosity(0);
  merger2->dca_xycut(0.5);
  merger2->dca_rzcut(1);
  if (ConstField && fieldstrength < 0.1)
  {
    merger2->zero_field();
  }
  se->registerSubsystem(merger2);
}

void vertexing()
{
  Fun4AllServer *se = Fun4AllServer::instance();
  int verbosity = std::max(Enable::VERBOSITY, Enable::TRACKING_VERBOSITY);

  auto vtxfinder = new PHSimpleVertexFinder;
  vtxfinder->Verbosity(verbosity);
  se->registerSubsystem(vtxfinder);
}

void Tracking_Reco_TrackFit()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::TRACKING_VERBOSITY);
  auto se = Fun4AllServer::instance();

  // correct clusters for particle propagation in TPC
  auto deltazcorr = new PHTpcDeltaZCorrection;
  deltazcorr->Verbosity(verbosity);
  se->registerSubsystem(deltazcorr);

  // perform final track fit with ACTS
  auto actsFit = new PHCosmicsTrkFitter;
  actsFit->Verbosity(verbosity);
  // actsFit->commissioning(G4TRACKING::use_alignment);
  // actsFit->set_cluster_version(G4TRACKING::cluster_version);
  //  in calibration mode, fit only Silicons and Micromegas hits
  actsFit->useActsEvaluator(true);
  actsFit->useOutlierFinder(false);
  actsFit->setFieldMap(G4MAGNET::magfield_tracking);
  se->registerSubsystem(actsFit);
}

void alignment(std::string datafilename = "mille_output_data_file",
               std::string steeringfilename = "mille_steer")
{
  Fun4AllServer *se = Fun4AllServer::instance();
  int verbosity = std::max(Enable::VERBOSITY, Enable::TRACKING_VERBOSITY);

  auto mille = new MakeMilleFiles;
  mille->Verbosity(verbosity);
  mille->set_datafile_name(datafilename + ".bin");
  mille->set_steeringfile_name(steeringfilename + ".txt");
  // mille->set_cluster_version(G4TRACKING::cluster_version);
  se->registerSubsystem(mille);

  auto helical = new HelicalFitter;
  helical->Verbosity(0);
  helical->set_datafile_name(datafilename + "_helical.bin");
  helical->set_steeringfile_name(steeringfilename + "_helical.txt");
  // helical->set_cluster_version(G4TRACKING::cluster_version);
  se->registerSubsystem(helical);
}

void Tracking_Reco()
{
  Tracking_Reco_TrackSeed();
  if (G4TRACKING::convert_seeds_to_svtxtracks)
  {
    convert_seeds();
  }
  else
  {
    Tracking_Reco_TrackFit();
  }

  vertexing();

  if (G4TRACKING::use_alignment)
  {
    alignment();
  }
}

#endif
