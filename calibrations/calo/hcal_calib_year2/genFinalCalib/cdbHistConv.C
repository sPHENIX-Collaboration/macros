#include <calobase/TowerInfoDefs.h>

#include <cdbobjects/CDBTTree.h> 

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
    for(int ip= 0; ip< nphi; ip++)
    {
      unsigned int key;
      if (icalo==0) key = TowerInfoDefs::encode_emcal(ie,ip);
      if (icalo==1) key = TowerInfoDefs::encode_hcal(ie,ip);
      float val = hist->GetBinContent(ie+1,ip+1);
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


TH2* CaloCDBTreeToHist(const std::string &inputfile, const std::string &fieldName, int icalo){

  CDBTTree* cdbttree = new CDBTTree(inputfile);

  int neta;
  int nphi;

  if (icalo != 0 && icalo != 1) return nullptr;

  if (icalo==0){
    neta = 96;
    nphi = 256;
  }
  if (icalo==1){
    neta = 24;
    nphi = 64;
  }

  TH2* h2 = new TH2F("h_temp","",neta,0,neta,nphi,0,nphi);
  h2->SetDirectory(nullptr);

  for(int ie = 0; ie < neta ; ie++)
  {
    for(int ip= 0; ip< nphi; ip++)
    {
      unsigned int key;
      if (icalo==0) key = TowerInfoDefs::encode_emcal(ie,ip);
      if (icalo==1) key = TowerInfoDefs::encode_hcal(ie,ip);

      float val = cdbttree->GetFloatValue(key, fieldName);
      h2->SetBinContent(ie+1,ip+1,val);
    }
  }

  return h2;
}
