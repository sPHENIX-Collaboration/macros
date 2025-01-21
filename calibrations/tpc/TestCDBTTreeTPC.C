#include <cdbobjects/CDBTTree.h>
#include <tpc/TpcMap.h>

R__LOAD_LIBRARY(libcdbobjects.so)
R__LOAD_LIBRARY(libtpc.so)


void TestCDBTTreeTPC(const std::string &fname = "./PadPlane/TPCPadPlaneCDBTTree.root")
{
  TpcMap M;
  // To use the latest CSV files the path should be directly 
  // from the default CDBTTree directory used in TpcMap to files, 
  // otherwise the default CSV Files are used 
  M.setMapNames("../../../../../../../../../../../sphenix/user/shulga/Work/TPCPadPlaneMapping/macros/calibrations/tpc/PadPlane/AutoPad-R1-RevA.sch.ChannelMapping.csv", 
                "../../../../../../../../../../../sphenix/user/shulga/Work/TPCPadPlaneMapping/macros/calibrations/tpc/PadPlane/AutoPad-R2-RevA-Pads.sch.ChannelMapping.csv", 
                "../../../../../../../../../../../sphenix/user/shulga/Work/TPCPadPlaneMapping/macros/calibrations/tpc/PadPlane/AutoPad-R3-RevA.sch.ChannelMapping.csv");


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
      int mod = 0;
      if(f>5) mod = 1;
      if(f>15) mod =2;
      unsigned int key = 256 * (f) + ch;
      string varname = "layer";// + to_string(key);
      cdbttree->SetIntValue(key,varname,layer);
      varname = "fee";// + to_string(key);
      cdbttree->SetIntValue(key,varname,f);
      varname = "ch";// + to_string(key);
      cdbttree->SetIntValue(key,varname,ch);
      varname = "R";// + to_string(key);
      cdbttree->SetDoubleValue(key,varname,padR);
      varname = "phi";// + to_string(key);
      cdbttree->SetDoubleValue(key,varname,padPhi);
      varname = "pad";// + to_string(key);
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

CDBTTree *TestWrite(const std::string &fname = "test.root")
{
  CDBTTree *cdbttree = new CDBTTree(fname);
  cdbttree->SetSingleFloatValue("Test",25);
  cdbttree->SetSingleFloatValue("Test2",22);
  cdbttree->SetSingleFloatValue("Test3",23);
  cdbttree->SetSingleFloatValue("Test4",24);
  cdbttree->SetSingleIntValue("Tes2",24);
  cdbttree->SetSingleDoubleValue("Tes2",TMath::Pi());
  cdbttree->SetSingleUInt64Value("Tes2",12486346984672562);
  cdbttree->CommitSingle();
  cdbttree->SetFloatValue(2,"Tst",25);
  cdbttree->SetFloatValue(2,"Tt2",22);
  cdbttree->SetFloatValue(2,"Tes",23);
  cdbttree->SetFloatValue(2,"gaga",24);
  cdbttree->SetFloatValue(4,"Tst",5);
  cdbttree->SetFloatValue(4,"Tt2",2);
  cdbttree->SetFloatValue(4,"Tes",3);
  cdbttree->SetFloatValue(4,"Tara",7);
  cdbttree->SetIntValue(10,"blar",2864);
  cdbttree->SetUInt64Value(10,"blatr",28);
  for (int i=0; i<100; i++)
  {
    string varname = "dvar";
    string varname2 = "dvar2";
    string varname3 = "dvar3";
    string varname4 = "dvar4";
    for (int j=0; j<25; j++)
    {
      cdbttree->SetDoubleValue(j,varname,28875342.867*j);
      cdbttree->SetDoubleValue(j,varname2,2.867*j);
      cdbttree->SetDoubleValue(j,varname3,28875.8*j);
      cdbttree->SetDoubleValue(j,varname4,28875342*j);
    }
  }
  return cdbttree;
}

void Read(const std::string &fname = "test.root")
{
  CDBTTree *cdbttree = new CDBTTree(fname);
  cdbttree->LoadCalibrations();
  cout << "Test2: " << cdbttree->GetSingleFloatValue("Test2") << endl;
  cout << "Tt2(2): " << cdbttree->GetFloatValue(2,"Tt2") << endl;
  cout << "Tt2(4): " << cdbttree->GetFloatValue(4,"Tt2") << endl;
  cout << "blar: " << cdbttree->GetIntValue(10,"blar") << endl;
  cout << "int Tt2(4): " << cdbttree->GetIntValue(4,"Tt2") << endl;
  cout << "dvar5: " << cdbttree->GetDoubleValue(23,"dvar5");
  cdbttree->Print();
  delete cdbttree;
  gSystem->Exit(0);
}
