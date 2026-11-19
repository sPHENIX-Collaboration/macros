#include <cdbobjects/CDBTTree.h> 
#include <calobase/TowerInfoDefs.h>

#include <Rtypes.h> // for R__LOAD_LIBRARY macro
#include <TH2.h>

R__LOAD_LIBRARY(libcdbobjects)
R__LOAD_LIBRARY(libcalo_io.so)

void histToCaloCDBTree(const std::string &outputfile, const std::string &fieldName, int icalo, TH2* hist){

  int neta;
  int nphi;

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

  std::cout << "Writing " << outputfile << "   with mean=" << mean/count << std::endl;
  cdbttree->Commit();
  cdbttree->WriteCDBTTree();
  //cdbttree->Print();
  delete cdbttree;
}



