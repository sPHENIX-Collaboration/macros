#include <cdbobjects/CDBTTree.h> 
#include "TowerInfoDefs.h"

R__LOAD_LIBRARY(libcdbobjects)
R__LOAD_LIBRARY(libcalo_io.so)

void histToCaloCDBTree(string outputfile, string fieldName, int icalo, TH2F* hist){

  int neta,nphi;

  if (icalo != 0 && icalo != 1) return;

  if (icalo==0){
    neta = 96;
    nphi = 256;
  }
  if (icalo==1){
    neta = 24;
    nphi = 64;
  }

  CDBTTree *cdbttree = new CDBTTree(outputfile);

  float mean = 0;
  int count = 0;

  for(int ie = 0; ie < neta ; ie++)
  {
    for(int iphi= 0; iphi< nphi; iphi++)
    {
      unsigned int key;
      if (icalo==0) key = TowerInfoDefs::encode_emcal(ie,iphi);
      if (icalo==1) key = TowerInfoDefs::encode_hcal(ie,iphi);
      float val = hist->GetBinContent(ie+1,iphi+1);
      cdbttree->SetFloatValue(key,fieldName,val);
      mean += val;
      count++;
    }
  }

  cout << "Writing " << outputfile.c_str() << "   with mean=" << mean/count << endl; 
  cdbttree->Commit();
  cdbttree->WriteCDBTTree();
  //cdbttree->Print();
  delete cdbttree;
}



