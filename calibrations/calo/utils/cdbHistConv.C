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

  cout << "Writing " << outputfile.c_str() << "   with mean=" << mean/count << endl; 
  cdbttree->Commit();
  cdbttree->WriteCDBTTree();
  //cdbttree->Print();
  delete cdbttree;
}


TH2F* CaloCDBTreeToHist(string inputfile, string fieldName, int icalo){

  CDBTTree* cdbttree = new CDBTTree(inputfile);
  if (!cdbttree){
    cout << "CaloCDBTreeToHist: could not get cdb tree" << endl;
    return nullptr;
  }

  int neta,nphi;

  if (icalo != 0 && icalo != 1) return nullptr;

  if (icalo==0){
    neta = 96;
    nphi = 256;
  }
  if (icalo==1){
    neta = 24;
    nphi = 64;
  }

  TH2F* h2 = new TH2F("h_temp","",neta,0,neta,nphi,0,nphi);

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


void swtichFieldName(string infile,string outfile,string infield,string outfield,int icalo){

  TH2F* h =  CaloCDBTreeToHist(infile, infield, icalo);
  histToCaloCDBTree(outfile, outfield, icalo, h);
  

}

// makes a default calibration (all 0s are set to average calibration factor)
// input: input cdb file name, output cdb file name, field name, calo index (0==CEMC 1==HCAL)
void makeDefaultCalib(string infile,string outfile,string infield,int icalo){

  TH2F* h =  CaloCDBTreeToHist(infile, infield, icalo);

  int c = 0;
  float avg = 0;
  for(int ie=1; ie<h->GetXaxis()->GetNbins()+1; ie++){
    for(int ip=1; ip<h->GetYaxis()->GetNbins()+1; ip++){
      float val = h->GetBinContent(ie,ip);
      if (val != 0) {avg+=val; c++;}
    }
  }
  avg /= c;
  for(int ie=1; ie<h->GetXaxis()->GetNbins()+1; ie++){
    for(int ip=1; ip<h->GetYaxis()->GetNbins()+1; ip++){
      float val = h->GetBinContent(ie,ip);
      if (val == 0) h->SetBinContent(ie,ip,avg);
    }
  }

  histToCaloCDBTree(outfile, infield, icalo, h);
}
