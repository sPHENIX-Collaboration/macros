#include <cdbobjects/CDBTTree.h>

R__LOAD_LIBRARY(libcdbobjects.so)

void TestCDBTTree(const std::string &fname = "test.root")
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
  for (int i=0; i<10; i++)
  {
    string varname = "dvar" + to_string(i);
  cdbttree->SetDoubleValue(23,varname,28875342.867);
  }
  cdbttree->Commit();
  cdbttree->Print();
  cdbttree->WriteCDBTTree();
  delete cdbttree;
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
//  for (int i=0; i<10000; i++)
  {
    string varname = "dvar";
    string varname2 = "dvar2";
    string varname3 = "dvar3";
    string varname4 = "dvar4";
    for (int j=0; j<25000; j++)
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
//cdbttree->LoadCalibrations();
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
