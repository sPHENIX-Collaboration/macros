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
#include <cdbobjects/CDBTTree.h>  // for CDBTTree
#include <ffamodules/CDBInterface.h>

//#include <litecaloeval/LiteCaloEval.h>
#include <caloreco/CaloTowerCalib.h>
#include <caloreco/RawClusterBuilderTemplate.h>
#include <calib_emc_pi0/CaloCalibEmc_Pi0.h>

#include <phool/recoConsts.h>

#include <fstream>
#include <string>
#include <TBranch.h>
#include <TLeaf.h>
#include <fstream>
#include <TChain.h>
#include <TTree.h>


R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libcdbobjects)
R__LOAD_LIBRARY(libcalo_reco.so)
//R__LOAD_LIBRARY(libLiteCaloEvalTowSlope.so)
R__LOAD_LIBRARY(libcalibCaloEmc_pi0.so)


#endif

void make_csv(const char * inf, const char * outf);
TTree * GetTChainMacro(TString ifs);
void make_cemcCDBTree(const char * infct, const char * outfct); 


// to get files from my local area
void wholeIter_Pi0EtaByEta(int nevents = -1,  
const int iteration = 1 , 
const char *fname = "pi0input.list", 
const char * inputCDBfnameIn = "inputcdb1.root",  
const char * outfileloc1 =  "./", 
const char * outfileloc2 = "./")
{
  //, const int runNumber = 0)
  
  
  //outfileloc1   -->  $CONDOR_SCRATCH --> throwaway temp files
  //outfileloc2   -->  user defined gfps to watch/monitor iterations  or debug
  
  gSystem->Load("libg4dst");

  std::string inputCDBfname = inputCDBfnameIn;
  
  const std::string defout1 = outfileloc1;
  const std::string defout2 = outfileloc2;
  // filenames, other than input
  std::string outntupfile     = defout1 + "/outntup_iter"+ std::to_string(iteration) + ".root"; 
  std::string outloopfile    = defout1 + "/outntup_loop" + std::to_string(iteration) + ".root"; 
  std::string outfitfile     = defout2 + "/outfit_"       + std::to_string(iteration) + ".root"; 
  std::string outcsvfile     = defout1 + "/csvoutfit" + std::to_string(iteration) + ".csv"; 
  std::string iterCDBfile    = defout2 + "/cdbtree"+ std::to_string(iteration) + ".root"; 

  // in other than the first iteration,  the previous iteration's corrections 
  // are needed both in CDBTree form (for input to clusters)  and in "native" CaloCalibEmc_Pi0 form 
  // (for aggregation of cumulative corrections across  each iteration)
  std::string prevoutfitfile       = defout2 + "/outfit_"       + std::to_string(iteration-1) + ".root";
  if (iteration == 1)
    prevoutfitfile = "";

  // for iterations other than 1, ignore input cdbfname and use previous iteration's
  if (iteration > 1)
    inputCDBfname = defout2 + "/cdbtree" + std::to_string(iteration-1) + ".root";

  Fun4AllServer *se = Fun4AllServer::instance();

  Fun4AllInputManager *in = new Fun4AllDstInputManager("in");

  TString infiletstr(fname);
  if (infiletstr.Contains(".list"))
    in->AddListFile(fname);
  else
    in->fileopen(fname);


  se->registerInputManager(in);

  // JF Nov23 ?? still needed ?
  Fun4AllInputManager *intrue2 = new Fun4AllRunNodeInputManager("DST_GEO");
  intrue2->AddFile("updated_geo.root");
  se->registerInputManager(intrue2);

  

  recoConsts *rc = recoConsts::instance();
  rc->set_StringFlag("CDB_GLOBALTAG","ProdA_2023"); // this points to the global tag in the CDB
  // The calibrations have a validity range set by the beam clock which is not read out of the prdfs as of now
  rc->set_uint64Flag("TIMESTAMP",0);
  //  rc->set_uint64Flag("TIMESTAMP",runNumber);

  
  CaloTowerCalib *calib = new CaloTowerCalib("CEMCCALIB");
  // calib->setCalibName("cemc_abscalib_cosmic");// these two lines are needed to choose your own calibration

  // JF to Blair --> below  change preferred fieldname for first towerslope iter
  if (iteration == 1) // blair-defined towerslope CBDtree 
    {
      calib->setFieldName("cemc_pi0_abs_calib");
    }
  else 
    {
      // leave this, at least at first, using Sijan's defined 
      // cdbtree outputs
      calib->setFieldName("cemc_pi0_abs_calib");
    }
  
  calib->set_directURL(inputCDBfname);
  
  se->registerSubsystem(calib);
  

   RawClusterBuilderTemplate *ClusterBuilder = new RawClusterBuilderTemplate("EmcRawClusterBuilderTemplate2");
  ClusterBuilder->Detector("CEMC");
  ClusterBuilder->Verbosity(10);
  ClusterBuilder->set_threshold_energy(0.030);  // This threshold should be the same as in CEMCprof_Thresh**.root file below
  std::string emc_prof = getenv("CALIBRATIONROOT");
  emc_prof += "/EmcProfile/CEMCprof_Thresh30MeV.root";
  ClusterBuilder->LoadProfile(emc_prof);
  ClusterBuilder->set_UseTowerInfo(1); // to use towerinfo objects rather than old RawTower

  //NOV 23 Blair : new
  ClusterBuilder->setInputTowerNodeName("TOWERINFO_CALIB_CEMC");
  ClusterBuilder->setOutputClusterNodeName("CLUSTERINFO2");  

  se->registerSubsystem(ClusterBuilder);


  CaloCalibEmc_Pi0 *eval_pi2 = new CaloCalibEmc_Pi0("dummy", outntupfile);
  eval_pi2->set_UseTowerInfo(1); // to use towerinfo objects rather than old RawTower
  eval_pi2->setInputTowerNodeName("TOWERINFO_CALIB_CEMC");
  eval_pi2->setInputClusterNodeName("CLUSTERINFO2");  
  se->registerSubsystem(eval_pi2);
  cout << "successful registration of pi0 " << endl;

  se->run(nevents);
  se->End();

  
  //  loop over ntuple file
  CaloCalibEmc_Pi0 calo_obj("CaloCalibEmc_Pi0", outloopfile);
  calo_obj.InitRun(0);
  //    TTree * intree1 =  GetTChainMacro(ifile);  // if parallelized
  calo_obj.Loop(nevents, outntupfile);
  calo_obj.End(0);


  //now fit:
  CaloCalibEmc_Pi0 calo_obj2("CaloCalibEmc_Pi0","");
  //for test using previous
  //  outloopfile    = "/sphenix/user/jfrantz/thdatNov22/pimacs/cutsg33_cpi1_21813.root"; 

  calo_obj2.Get_Histos(outloopfile.c_str() , outfitfile.c_str());   
  calo_obj2.Fit_Histos_Etas96(prevoutfitfile.c_str());	 // see note at beginning during filenames settings
  calo_obj2.End(0);
  
  // now gen CDBTree file using Sijan's methods
  make_csv(outfitfile.c_str(), outcsvfile.c_str());
  make_cemcCDBTree(outcsvfile.c_str(), iterCDBfile.c_str());
  

  se->PrintTimer();

  //  gSystem->Exit(0);
}


TTree* GetTChainMacro(TString ifile="")
{
	ifstream inFile;
	inFile.open(ifile, ios::in); // open to read
	if (!inFile)
	{
		cerr << "Unable to open file:\n ";
		exit(1);
	}

	TChain *pitree = new TChain("_eventTree");
	string root_file;
	int lines=0;
	while (std::getline(inFile, root_file))
	{
		pitree->Add(root_file.c_str());
		lines += 1;
	}
	printf("total lines: %d\n",lines);
	inFile.close();
	
	return (TTree *) pitree;

 }


void make_csv( const char * input_filechar, const char * output_filechar)
{
  // input file
  const string input_file = input_filechar;
  
  // output file
  const string output_file = output_filechar;	
    
			
  // Open the ROOT file
  TFile *file = TFile::Open(input_file.c_str(), "READ");

  // Create an output CSV file
  std::ofstream csvFile(output_file.c_str());
		
  // Get the TTree
  TTree *tree = (TTree*)file->Get("nt_corrVals");
  
  // Get the number of entries (rows) in the TTree
    Long64_t nEntries = tree->GetEntries();
    
    if (nEntries != 24576) {std::cout << "The number of entries does not match with EMCal. The number is " << nEntries << " ." << std::endl;}
    
    
    // Get the number of branches (columns) in the TTree
    TObjArray *branches = tree->GetListOfBranches();
    int nbranches = branches->GetEntries();
    

    // write the header for CSV file
    csvFile << "phibin" << " , " << "etabin" << " , " << "calib-factor" << std::endl;
    
    for (int j = 0; j < nEntries; j++) // for each entry in the Branches
      {
	tree->GetEntry(j);
	
	int ieta, iphi;
	float agg_corr_val;
	
	for (int b = 0; b < nbranches; ++b) 
	  {
            TBranch *branch = (TBranch*)branches->At(b);
            TLeaf *leaf = (TLeaf*)branch->GetListOfLeaves()->At(0);
	    
						if (b == 0) {ieta = leaf->GetValue(0);}
						if (b == 1) {iphi = leaf->GetValue(0);}
						if (b == 3) {agg_corr_val = leaf->GetValue(0);}
	  }
	csvFile << iphi << " , " << ieta << " , " << agg_corr_val << "\n";	
      }
    
    // Clean up
    csvFile.close();
    file->Close();
    delete file;
    std::cout << "All Done making csvfile:" << output_file << std::endl;
    //    return 0;
}


void make_cemcCDBTree(const char * input_file1, // csv file to be supplied
		      const char * output_file1) // output root file having CDB Ttree
{
  // file location, input file and output file
	

  const string input_file = input_file1;
  const string output_file = output_file1;
  
  std::cout << "start making CDBTree: " << output_file1 << std::endl;
  
  float corrArray[256][96];
  string line, cell1, cell2, cell3;
  
  // Open the CSV file
  ifstream file(input_file.c_str());

	// read header row and do nothing
	std::getline(file, line);
  while(getline(file, line))
	{
    stringstream lineStream(line);

    getline(lineStream, cell1, ',');
		//std::cout << "phi: " << std::stoi(cell1) << " ";

    getline(lineStream, cell2, ',');
		//std::cout << "eta: " << std::stoi(cell2) << " ";

		getline(lineStream, cell3, ',');
		//std::cout << "corr: " << std::stof(cell3) << std::endl;

		corrArray[std::stoi(cell1)][std::stoi(cell2)] = std::stof(cell3);
  }
  file.close();

	/* to check if my array was correctly filled	
	 * --------------------------------------
	for (int iphi=0; iphi<256; iphi++)
	{
		for (int ieta=0; ieta<96; ieta++)
		{
			std::cout << iphi << " " << ieta << " " << corrArray[iphi][ieta] << std::endl;
		}
	}
	*/	
	
	CDBTTree *cdbttree = new CDBTTree(output_file.c_str());
  for(int iphi=0; iphi<256; iphi++){
    for(int ieta=0; ieta<96; ieta++){
      int key = iphi + (ieta << 16);
      cdbttree->SetFloatValue(key, "cemc_pi0_abs_calib", corrArray[iphi][ieta]);
    }
  }
 
  cdbttree->Commit();
  //cdbttree->Print();
  cdbttree->WriteCDBTTree();
  delete cdbttree;
  std::cout << "cdb success " << std::endl;
	//  gSystem->Exit(0);
}
