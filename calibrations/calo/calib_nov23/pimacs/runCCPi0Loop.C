#include <calib_emc_pi0/CaloCalibEmc_Pi0.h>
//#include "GetTChainMacro.C"
#include <fstream>
#include <string>
#include "TChain.h"
#include "TTree.h"
#include "TString.h"



void runCCPi0Loop(int nevents = -1, const char *ifile="", const char *ofile="")
{
	R__LOAD_LIBRARY(libcalibCaloEmc_pi0.so)	

	CaloCalibEmc_Pi0 calo_obj("CaloCalibEmc_Pi0", ofile);
  calo_obj.InitRun(0);
  TTree * intree1 =  GetTChainMacro(ifile);
  //	calo_obj.InitializeBadTowers();
  calo_obj.Loop(nevents, ifile, intree1);
  calo_obj.End(0);
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
