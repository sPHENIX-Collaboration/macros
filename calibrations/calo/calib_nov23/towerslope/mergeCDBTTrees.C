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

void mergeCDBTTrees(const char * infile1, const char * infile2, const char * outputfile)
{

  CDBTTree *cdbttree1 = new CDBTTree(infile1);  
  CDBTTree *cdbttree2 = new CDBTTree(infile2);  
  CDBTTree *cdbttreeOut = new CDBTTree(outputfile);

  string m_fieldname = "Femc_datadriven_qm1_correction";

  for(int i = 0; i < 96 ; i++)
  {
    for(int j = 0; j < 256; j++)
    {
      unsigned int key = TowerInfoDefs::encode_emcal(i,j);
      float val1 = cdbttree1->GetFloatValue(key, m_fieldname);
      float val2 = cdbttree2->GetFloatValue(key, m_fieldname);
      cdbttreeOut->SetFloatValue(key,m_fieldname,val1*val2);
    }
  }

  cdbttreeOut->Commit();
  cdbttreeOut->WriteCDBTTree();
  delete cdbttreeOut;
  delete cdbttree1;
  delete cdbttree2;

}//end macro
