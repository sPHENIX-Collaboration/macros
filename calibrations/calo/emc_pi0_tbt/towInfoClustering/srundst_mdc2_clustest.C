#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4allraw/Fun4AllPrdfInputManager.h>
#include <fun4all/Fun4AllDstInputManager.h>
//#include <calotowerbuilder/RawWaveformTowerBuilder.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <litecaloeval/LiteCaloEval.h>

#include <calib_emc_pi0/CaloCalibEmc_Pi0.h>

#include <caloreco/RawClusterBuilderGraph.h>
#include <caloreco/RawClusterBuilderTemplate.h>
#include <caloreco/RawClusterPositionCorrection.h>
#include <caloreco/RawTowerCalibration.h>


R__LOAD_LIBRARY(libfun4all.so)
//R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libcalo_reco.so)
R__LOAD_LIBRARY(libcalibCaloEmc_pi0.so)
R__LOAD_LIBRARY(libLiteCaloEvalTowSlope.so)


#endif

// void Fun4All_TowerBuilder(const char *fname = "/sphenix/data/data02/sphenix/cemc/combinedEvents/EmCalSEB01-000000222-0000_x.prdf", const char *outfile = "trees/0.root")
void srundst_mdc2_clustest(
			   const int nEvents = 5,
			     const string &outputFile = "secoutput4_raw",
			     const string &inputFile0 = "ongoing/dstnewoutput5_calo5.root"
			   //const string &inputFile1 = "DST_VERTEX_sHijing_0_20fm_50kHz_bkg_0_20fm-0000000004-10000.root",
			   //			   const int mdc2_4_file_num = 1,
			     )
{

  //  gSystem->Load("libg4dst");

  Fun4AllServer *se = Fun4AllServer::instance();

/*
   string inputFile0 = "DST_CALO_CLUSTER_sHijing_0_20fm_50kHz_bkg_0_20fm-0000000062-";
   string inputFile1 = "DST_VERTEX_sHijing_0_20fm_50kHz_bkg_0_20fm-0000000062-";

   
   int ynum_int = 100000+ mdc2_4_file_num;
   TString yn_tstr = "";
   yn_tstr += ynum_int;
   yn_tstr.Remove(0,1);
   inputFile0 += yn_tstr.Data();
   inputFile1 += yn_tstr.Data();

   inputFile0 += ".root";
   inputFile1 += ".root";
   
   cout << "running over these files" << endl;
   cout << inputFile0 << endl;
   cout << inputFile1 << endl;
*/

  // CaloWaveFormSim *ca = new CaloWaveFormSim("CALOWAVEFORMSIM",outfile);
  // ca->Detector("CEMC");
  // se->registerSubsystem(ca);

  // Fun4AllInputManager *in = new Fun4AllPrdfInputManager("in");
  // in->fileopen(fname);

Fun4AllInputManager *in = new Fun4AllDstInputManager("in");

//  in->fileopen(inputFile0.c_str());
//  in->fileopen(inputFile1.c_str());
  in->AddFile(inputFile0.c_str());
//in->AddFile(inputFile1.c_str());

  //CaloTowerBuilder *ca = new CaloTowerBuilder();
  //  RawWaveformTowerBuilder *rw = new RawWaveformTowerBuilder();

  //  rw->set_detector_type(RawWaveformTowerBuilder::CEMC);
  //  se->registerSubsystem(rw);
/////



////////
  // in->AddListFile("g4hits.list");



// Fun4All
  se->registerInputManager(in);


    RawClusterBuilderTemplate *ClusterBuilder = new RawClusterBuilderTemplate("EmcRawClusterBuilderTemplate");
    ClusterBuilder->Detector("CEMC");
//    ClusterBuilder->Verbosity(verbosity);
    ClusterBuilder->set_threshold_energy(0.030);  // This threshold should be the same as in CEMCprof_Thresh**.root file below
    std::string emc_prof = getenv("CALIBRATIONROOT");
    emc_prof += "/EmcProfile/CEMCprof_Thresh30MeV.root";
    ClusterBuilder->LoadProfile(emc_prof);
    se->registerSubsystem(ClusterBuilder);


  RawClusterPositionCorrection *clusterCorrection = new RawClusterPositionCorrection("CEMC");

  clusterCorrection->Get_eclus_CalibrationParameters().ReadFromFile("CEMC_RECALIB", "xml", 0, 0,
                                                                      //raw location
                                                                      string(getenv("CALIBRATIONROOT")) + string("/CEMC/PositionRecalibration_EMCal_9deg_tilt/"));
  clusterCorrection->Get_ecore_CalibrationParameters().ReadFromFile("CEMC_ECORE_RECALIB", "xml", 0, 0,
                                                                      //raw location
                                                                      string(getenv("CALIBRATIONROOT")) + string("/CEMC/PositionRecalibration_EMCal_9deg_tilt/"));
  

//clusterCorrection->Verbosity(verbosity);
  se->registerSubsystem(clusterCorrection);



    RawClusterBuilderTemplate *ClusterBuilder2 = new RawClusterBuilderTemplate("EmcRawClusterBuilderTemplate2");
    ClusterBuilder2->Detector("CEMC");
    //    ClusterBuilder2->Verbosity(verbosity);
    ClusterBuilder2->set_threshold_energy(0.030);  // This threshold should be the same as in CEMCprof_Thresh**.root file below
    std::string emc_prof2 = getenv("CALIBRATIONROOT");
    emc_prof2 += "/EmcProfile/CEMCprof_Thresh30MeV.root";
    ClusterBuilder2->LoadProfile(emc_prof2);
    ClusterBuilder2->set_UseTowerInfo(1);
    se->registerSubsystem(ClusterBuilder2);

  RawClusterPositionCorrection *clusterCorrection2 = new RawClusterPositionCorrection("CEMC");

  clusterCorrection2->set_UseTowerInfo(1);
  if (false)
  {
    clusterCorrection2->Get_eclus_CalibrationParameters().ReadFromCDB("CEMCRECALIB");
    clusterCorrection2->Get_ecore_CalibrationParameters().ReadFromCDB("CEMC_ECORE_RECALIB");
  }
  else
  {
    clusterCorrection2->Get_eclus_CalibrationParameters().ReadFromFile("CEMC_RECALIB", "xml", 0, 0,
                                                                      //raw location
                                                                      string(getenv("CALIBRATIONROOT")) + string("/CEMC/PositionRecalibration_EMCal_9deg_tilt/"));
    clusterCorrection2->Get_ecore_CalibrationParameters().ReadFromFile("CEMC_ECORE_RECALIB", "xml", 0, 0,
                                                                      //raw location
                                                                      string(getenv("CALIBRATIONROOT")) + string("/CEMC/PositionRecalibration_EMCal_9deg_tilt/"));
  }

  //  clusterCorrection2->Verbosity(verbosity);
  se->registerSubsystem(clusterCorrection2);
    


  string outputFile2 = outputFile.c_str();
  outputFile2 = outputFile2 + ".root";
  string outputfile5 = outputFile2 + "_cemc_eval5.root";
  string outputFile3 = outputFile2 + "_cemc_eval3.root";



  LiteCaloEval *eval5 = new LiteCaloEval("CEMCEVALUATOR2", "CEMC", outputfile5.c_str());
  //  eval->Verbosity(verbosity);
  eval5->CaloType(LiteCaloEval::CEMC);
  se->registerSubsystem(eval5);


  CaloCalibEmc_Pi0 *eval_pi1 = new CaloCalibEmc_Pi0("CEMC_CALIB_PI0", outputFile2);
  //  eval_pi1->set_mode(1);
  //  eval->Verbosity(verbosity);
  se->registerSubsystem(eval_pi1);


  CaloCalibEmc_Pi0 *eval_pi2 = new CaloCalibEmc_Pi0("CEMC_CALIB_PI02", outputFile3);
  //  eval_pi2->set_mode(1);
  //  eval->Verbosity(verbosity);
  eval_pi2->set_UseTowerInfo(1);
  se->registerSubsystem(eval_pi2);
  cout << "successful registration of pi0 2" << endl;

  

  /*
  Fun4AllDstOutputManager *out = new Fun4AllDstOutputManager("DSTOUT", "testoutput_simfile_ver2_5.root");

  out->StripNode("WAVEFORMS_CEMC");
  se->registerOutputManager(out);

  */

  se->run(nEvents);
  se->End();
  se->PrintTimer();
  //  gSystem->Exit(0);
}
