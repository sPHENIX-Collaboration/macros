#include <GlobalVariables.C>
#include <cdbobjects/CDBTTree.h>  // for CDBTTree
#include <ffamodules/CDBInterface.h>
#include <phool/recoConsts.h>
#include "TowerInfo.h"
#include "TowerInfoDefs.h"

R__LOAD_LIBRARY(libLiteCaloEvalTowSlope.so)

R__LOAD_LIBRARY(libcdbobjects)
R__LOAD_LIBRARY(libphool.so)
R__LOAD_LIBRARY(libcalo_reco.so)
R__LOAD_LIBRARY(libcalo_io.so)

void TSCtoCDBTTree(const char * infile, const char * outputfile)
{

  bool chk4file = gSystem->AccessPathName(infile);
  TFile *f = nullptr;

  if(!chk4file)
    {
      f = new TFile(infile,"READ");
    }  
  else
    {
      std::cout << "File " << infile << " cant be found in current directory." << std::endl;
      exit(0);
    }

  //write to cdb tree
  CDBTTree *cdbttree = new CDBTTree(outputfile);

  //gain values lie in the 2d histogram called corrPat
  TH2F *cp = (TH2F *)f->Get("corrPat");

  for(int i = 0; i < 96 ; i++)
  {
    for(int j = 0; j < 256; j++)
    {
      unsigned int key = TowerInfoDefs::encode_emcal(i,j);
      float gain = (1.0 / cp->GetBinContent(i+1,j+1) );
      if (cp->GetBinContent(i+1,j+1)==0) gain = 0;
      if (isnan(cp->GetBinContent(i+1,j+1))) {gain = 0; cout << i << "," << j << endl;}
      cdbttree->SetFloatValue(key,"Femc_datadriven_qm1_correction",gain);
    }
  }

  cdbttree->Commit();
  cdbttree->WriteCDBTTree();
  //cdbttree->Print();
  f->Close();
  delete f;
  delete cdbttree;

}//end macro
