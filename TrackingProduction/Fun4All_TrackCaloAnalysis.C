/*
 * This macro shows a minimum working example of running over the production track and calo DSTs.
 * You can add some analysis modules at the end which package tracks and calo clusters into trees for analysis.
 */

#include <G4_ActsGeom.C>
#include <G4_Global.C>
#include <G4_Magnet.C>
#include <GlobalVariables.C>
#include <Trkr_Clustering.C>
#include <Trkr_Reco.C>
#include <Trkr_RecoInit.C>
#include <Trkr_TpcReadoutInit.C>
#include <QA.C>

#include <ffamodules/CDBInterface.h>

#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllUtils.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllRunNodeInputManager.h>
#include <fun4all/Fun4AllServer.h>

#include <phool/recoConsts.h>

#include <trackreco/PHActsTrackProjection.h>

#include <trackbase_historic/SvtxTrack.h>

#include <stdio.h>
#include <iostream>
#include <filesystem>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libphool.so)
R__LOAD_LIBRARY(libcdbobjects.so)
R__LOAD_LIBRARY(libtrack_reco.so)
R__LOAD_LIBRARY(libcalo_reco.so)
R__LOAD_LIBRARY(libcalotrigger.so)
R__LOAD_LIBRARY(libcentrality.so)
R__LOAD_LIBRARY(libmbd.so)
R__LOAD_LIBRARY(libepd.so)
R__LOAD_LIBRARY(libzdcinfo.so)
void Fun4All_TrackCaloAnalysis(
    const int nEvents = 10,
    const std::string trackfilename = "DST_TRKR_TRACKS_run2pp_ana475_2024p017_v001-00053877-00000.root",
    const std::string trackdir = "/sphenix/lustre01/sphnxpro/production/run2pp/physics/ana475_2024p017_v001/DST_TRKR_TRACKS/run_00053800_00053900/dst/",
    const std::string calofilename = "DST_CALO_run2pp_ana468_2024p012_v001-00053877-00000.root",
    const std::string calodir = "/sphenix/lustre01/sphnxpro/production/run2pp/physics/ana468_2024p012_v001/DST_CALO/run_00053800_00053900/dst/",
    const std::string outfilename = "tracks_calos",
    const std::string outdir = "./")
{
  std::string inputTrackFile = trackdir + trackfilename;
  std::string inputCaloFile = calodir + calofilename;

  std::pair<int, int>
      runseg = Fun4AllUtils::GetRunSegment(trackfilename);
  int runnumber = runseg.first;
  int segment = runseg.second;

  TString outfileheader = outdir + outfilename + "_" + runnumber + "-" + segment;
  std::cout<<"outfile header: "<<outfileheader<<std::endl;
  std::string theOutfileheader = outfileheader.Data();

  Enable::MVTX_APPLYMISALIGNMENT = true;
  ACTSGEOM::mvtx_applymisalignment = Enable::MVTX_APPLYMISALIGNMENT;
  TRACKING::pp_mode = true;

  auto se = Fun4AllServer::instance();
  se->Verbosity(1);

  int verbosity = 0;

  auto rc = recoConsts::instance();
  rc->set_IntFlag("RUNNUMBER", runnumber);
  rc->set_IntFlag("RUNSEGMENT", segment);

  Enable::CDB = true;
  rc->set_StringFlag("CDB_GLOBALTAG", "ProdA_2024");
  rc->set_uint64Flag("TIMESTAMP", runnumber);
  std::string geofile = CDBInterface::instance()->getUrl("Tracking_Geometry");

  Fun4AllRunNodeInputManager *ingeo = new Fun4AllRunNodeInputManager("GeoIn");
  ingeo->AddFile(geofile);
  se->registerInputManager(ingeo);

  TpcReadoutInit( runnumber );
  std::cout << " run: " << runnumber
            << " samples: " << TRACKING::reco_tpc_maxtime_sample
            << " pre: " << TRACKING::reco_tpc_time_presample
            << " vdrift: " << G4TPC::tpc_drift_velocity_reco
            << std::endl;

  G4TPC::ENABLE_MODULE_EDGE_CORRECTIONS = true;

  // to turn on the default static corrections, enable the two lines below
  G4TPC::ENABLE_STATIC_CORRECTIONS = true;
  G4TPC::USE_PHI_AS_RAD_STATIC_CORRECTIONS = false;

  //to turn on the average corrections, enable the three lines below
  //note: these are designed to be used only if static corrections are also applied
  G4TPC::ENABLE_AVERAGE_CORRECTIONS = true;
  G4TPC::USE_PHI_AS_RAD_AVERAGE_CORRECTIONS = false;
   // to use a custom file instead of the database file:
  G4TPC::average_correction_filename = CDBInterface::instance()->getUrl("TPC_LAMINATION_FIT_CORRECTION");

  G4MAGNET::magfield_rescale = 1;
  TrackingInit();

  auto trackin = new Fun4AllDstInputManager("TrackInManager");
  trackin->fileopen(inputTrackFile);
  se->registerInputManager(trackin);

  auto caloin = new Fun4AllDstInputManager("CaloInManager");
  caloin->fileopen(inputCaloFile);
  se->registerInputManager(caloin);

  Global_Reco();

  auto projection = new PHActsTrackProjection("CaloProjection");
  float new_cemc_rad = 99; // from DetailedCalorimeterGeometry, project to inner surface
  bool doEMcalRadiusCorr = true;
  if (doEMcalRadiusCorr)
  {
    projection->setLayerRadius(SvtxTrack::CEMC, new_cemc_rad);
  }
  se->registerSubsystem(projection);

  //add your analysis module here
  TString ananame = theOutfileheader + "_ana.root";
  std::string anaOutputFileName(ananame.Data());

  if (Enable::DSTOUT)
  {
    TString dstname = theOutfileheader + "_dst.root";
    std::string dstOutputFileName(dstname.Data());
    Fun4AllDstOutputManager *out = new Fun4AllDstOutputManager("DSTOUT", dstOutputFileName);
    out->AddNode("Sync");
    out->AddNode("EventHeader");
    out->AddNode("SvtxTrackMap");
    out->AddNode("GlobalVertexMap");
    out->AddNode("CLUSTERINFO_CEMC");
    se->registerOutputManager(out);
  }

  se->run(nEvents);
  se->End();
  se->PrintTimer();

  if(Enable::QA)
  {
    TString qaname = theOutfileheader + "_qa.root";
    std::string qaOutputFileName(qaname.Data());
    QAHistManagerDef::saveQARootFile(qaOutputFileName);
  }

  CDBInterface::instance()->Print();
  delete se;
  std::cout << "Finished" << std::endl;
  gSystem->Exit(0);

  return;
}

std::string GetFirstLine(std::string listname)
{
  std::ifstream file(listname);

  std::string firstLine = "";
  if (file.is_open()) {
      if (std::getline(file, firstLine)) {
          std::cout << "First Line: " << firstLine << std::endl;
      } else {
          std::cerr << "Unable to read first line of file" << std::endl;
      }
      file.close();
  } else {
      std::cerr << "Unable to open file" << std::endl;
  }
  return firstLine;
}
