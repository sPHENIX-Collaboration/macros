#include <fstream>
#include <string>
#include "TChain.h"
#include "TTree.h"
#include "TString.h"

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
