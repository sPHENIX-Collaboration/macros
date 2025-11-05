#include <cdbobjects/CDBTTree.h>
#include <tpc/TpcMap.h>


#include <Rtypes.h>  // resolves R__LOAD_LIBRARY for clang-tidy
#include <TSystem.h>

#include <iostream>

R__LOAD_LIBRARY(libcdbobjects.so)
R__LOAD_LIBRARY(libtpc.so)


void TestCDBTTreeTPC(const std::string &fname = "./PadPlane/TPCPadPlaneCDBTTree.root")
{
  TpcMap M;
  // To use the latest CSV files the path should be directly 
  // from the default CDBTTree directory used in TpcMap to files, 
  // otherwise the default CSV Files are used 
  M.setMapNames("/sphenix/user/shulga/Work/TPCPadPlaneMapping/macros/calibrations/tpc/PadPlane/AutoPad-R1-RevA.sch.ChannelMapping.csv", 
                "/sphenix/user/shulga/Work/TPCPadPlaneMapping/macros/calibrations/tpc/PadPlane/AutoPad-R2-RevA-Pads.sch.ChannelMapping.csv", 
                "/sphenix/user/shulga/Work/TPCPadPlaneMapping/macros/calibrations/tpc/PadPlane/AutoPad-R3-RevA.sch.ChannelMapping.csv");


  CDBTTree *cdbttree = new CDBTTree(fname);

  int Nfee = 26;
  int Nch = 256;
  for(int f=0;f<Nfee;f++){
    for(int ch=0;ch<Nch;ch++){
      int layer = M.getLayer(f, ch);
      int padN = M.getPad(f, ch);
      double padR = M.getR(f, ch);
      if (f==5 && ch==30){std::cout<<"layer="<<layer<<" padR="<<padR<<std::endl;}
      double padPhi = M.getPhi(f, ch);
      // int mod = 0;
      // if(f>5) mod = 1;
      // if(f>15) mod =2;
      unsigned int key = 256 * (f) + ch;
      std::string varname = "layer";// + std::to_string(key);
      cdbttree->SetIntValue(key,varname,layer);
      varname = "fee";// + std::to_string(key);
      cdbttree->SetIntValue(key,varname,f);
      varname = "ch";// + std::to_string(key);
      cdbttree->SetIntValue(key,varname,ch);
      varname = "R";// + std::to_string(key);
      cdbttree->SetDoubleValue(key,varname,padR);
      varname = "phi";// + std::to_string(key);
      cdbttree->SetDoubleValue(key,varname,padPhi);
      varname = "pad";// + std::to_string(key);
      cdbttree->SetIntValue(key,varname,padN);

    }
  }

  std::cout<<"cdbttree->Commit();"<<std::endl;
  cdbttree->Commit();
  std::cout<<"cdbttree->Print();"<<std::endl;
  //cdbttree->Print();
  std::cout<<"cdbttree->WriteCDBTTree()"<<std::endl;
  cdbttree->WriteCDBTTree();

  std::cout<<"delete cdbttree;"<<std::endl;
  delete cdbttree;
  //delete M;
  std::cout<<"gSystem->Exit(0);"<<std::endl;

  gSystem->Exit(0);
}
