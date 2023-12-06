#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllRunNodeInputManager.h>
#include <fun4allraw/Fun4AllPrdfInputManager.h>
#include <fun4all/Fun4AllDstInputManager.h>
//#include <rawwaveformtowerbuilder/RawWaveformTowerBuilder.h>
#include <fun4all/Fun4AllDstOutputManager.h>

//#include <litecaloeval/LiteCaloEval.h>
#include <caloreco/CaloTowerCalib.h>
#include <caloreco/RawClusterBuilderTemplate.h>
#include <calib_emc_pi0/CaloCalibEmc_Pi0.h>

#include <phool/recoConsts.h>
#include <g4centrality/PHG4CentralityReco.h> 


R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libcalo_reco.so)
//R__LOAD_LIBRARY(libLiteCaloEvalTowSlope.so)
R__LOAD_LIBRARY(libcalibCaloEmc_pi0.so)



#endif

// to get files from my local area
void clus_dst_simple_superfastcalib( const int file_num = 0 )

{
	const int nEvents = -1;

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(0);

	///sphenix/lustre01/sphnxpro/mdc2/shijing_hepmc/fm_0_20/calocluster/run0007

  string inputFile0 = "DST_CALO_G4HIT_sHijing_0_20fm_50kHz_bkg_0_20fm-0000000007-";
	string inputFile1 = "DST_CALO_CLUSTER_sHijing_0_20fm_50kHz_bkg_0_20fm-0000000007-";	
  //string inputFile2 = "DST_TRUTH_G4HIT_sHijing_0_20fm_50kHz_bkg_0_20fm-0000000007-";
  string inputFile3 = "DST_GLOBAL_sHijing_0_20fm_50kHz_bkg_0_20fm-0000000007-";
	
  string outputFile = "RESULTS/mult_clus_ntuple/iter_23/Fun4All_ntuples_run7_sHijing_iter23_";

	int ynum_int = 100000 + file_num;
  TString yn_tstr = "";
  yn_tstr += ynum_int;
  yn_tstr.Remove(0,1);
  inputFile0 += yn_tstr.Data();
  inputFile1 += yn_tstr.Data();
  //inputFile2 += yn_tstr.Data();
  inputFile3 += yn_tstr.Data();  
	outputFile += yn_tstr.Data();

  inputFile0 += ".root";
  inputFile1 += ".root";
  //inputFile2 += ".root";
  inputFile3 += ".root";
  outputFile += ".root";
	
	std::cout << "-------------------------------------------------------------------" << std::endl;
	std::cout << " input files are" << std::endl;
	std::cout << "-------------------------------------------------------------------" << std::endl;
	std::cout << inputFile0 << std::endl;
	std::cout << inputFile1 << std::endl;
	//std::cout << inputFile2 << std::endl;
	std::cout << inputFile3 << std::endl;

	std::cout << "-------------------------------------------------------------------" << std::endl;
	std::cout << "output file is " << std::endl;
	std::cout << "-------------------------------------------------------------------" << std::endl;
	std::cout << outputFile << std::endl;


  gSystem->Load("libg4dst");

  //Fun4AllServer *se = Fun4AllServer::instance();

  // CaloWaveFormSim *ca = new CaloWaveFormSim("CALOWAVEFORMSIM",outfile);
  // ca->Detector("CEMC");
  // se->registerSubsystem(ca);

  // Fun4AllInputManager *in = new Fun4AllPrdfInputManager("in");
  // in->fileopen(fname);

  Fun4AllInputManager *in1 = new Fun4AllDstInputManager("in1");
	in1->fileopen(inputFile1);
  se->registerInputManager(in1);

  Fun4AllInputManager *in3 = new Fun4AllDstInputManager("in3");
	in3->fileopen(inputFile3);
  se->registerInputManager(in3);

  //CaloTowerBuilder *ca = new CaloTowerBuilder();
  //Enable::CDB = true;

  recoConsts *rc = recoConsts::instance();
  //===============
  // conditions DB flags
  //===============
  // global tag
  rc->set_StringFlag("CDB_GLOBALTAG","MDC2");
  // // 64 bit timestamp
  rc->set_uint64Flag("TIMESTAMP",6);

	
  CaloTowerCalib *calib = new CaloTowerCalib("CALO_TOWER_CEMC");
	// make sure there is not any mistake in "full file path + name"
	// for first iteration file name can be any as we will not use correction factor and default is "1"
	calib->setCalibUseMode(1); // decide if we want to use correction factor("0" = do not use (set for first iteration), "1" = use (set for remaining))
  
	// these two lines are needed to choose your own correction factor (this is filename)
	calib->setCalibName("/sphenix/user/sregmi/WORKING_AREA/PI_0/pi0_calib_m-clust/CDB_CEMC/iter_file/m_clust_pi0_iter22_CDB.root");
	calib->setFieldName("cemc_pi0_abs_calib"); // (this is field name)
  calib->set_detector_type(CaloTowerCalib::CEMC);

  se->registerSubsystem(calib);
	
  	
  RawClusterBuilderTemplate *ClusterBuilder = new RawClusterBuilderTemplate("EmcRawClusterBuilderTemplate2");
  ClusterBuilder->Detector("CEMC");
  ClusterBuilder->Verbosity(10);	
  ClusterBuilder->set_threshold_energy(0.032);  // This threshold should be the same as in CEMCprof_Thresh**.root file below
  std::string emc_prof = getenv("CALIBRATIONROOT");
  emc_prof += "/EmcProfile/CEMCprof_Thresh30MeV.root";
  ClusterBuilder->LoadProfile(emc_prof);
  ClusterBuilder->set_UseTowerInfo(1); // to use towerinfo objects rather than old RawTower	
  se->registerSubsystem(ClusterBuilder);
		
	
  CaloCalibEmc_Pi0 *eval_pi2 = new CaloCalibEmc_Pi0("dummy", outputFile);
                                                // this call is needed for embedding
  //  eval_pi2->set_centrality_nclusters_cut(350);  // which uses more central events
                                                // than we will for data to enhance Bkg
                                                // to match the enhanced signal from embed
  eval_pi2->set_UseTowerInfo(1); // to use towerinfo objects rather than old RawTower
  se->registerSubsystem(eval_pi2);
	

  // in->AddListFile("g4hits.list");

// Fun4All
//  se->registerInputManager(in);

  //Fun4AllDstOutputManager *out = new Fun4AllDstOutputManager("DSTOUT", "testoutput_400.root");

  //out->StripNode("WAVEFORMS_CEMC");
  //se->registerOutputManager(out);

  se->run(nEvents);

  se->End();
  se->PrintTimer();
  gSystem->Exit(0);
}
