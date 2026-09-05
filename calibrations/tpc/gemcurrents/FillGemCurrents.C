#include <cdbobjects/CDBTTree.h>

#include <TSystem.h>

#include <cmath>
#include <iostream>
#include <sstream>
#include <fstream>

R__LOAD_LIBRARY(libcdbobjects.so)

void FillGemCurrents(const std::string &fname = "tpc_GEM_current_status/run_82516_GEM_BCO.csv")
{
  std::ifstream in(fname);
  std::string line;
  int channel = -1; // so we start with channel 0
  CDBTTree *cdbttree = new CDBTTree("cdbttree.root");
  while (std::getline(in, line))
  {
    if (line.empty()) continue;
    std::istringstream iss(line);
    std::string key;
    uint64_t bco;
    float current;
    iss >> key;
    if (key == "bco")
    {
      channel++;
      iss >> bco;
      cdbttree->SetUInt64Value(channel,key,bco);
    }
    else
    {
      iss >> current;
      cdbttree->SetFloatValue(channel,key,current);
    }
  }
  cdbttree->Commit();
  cdbttree->Print();
  cdbttree->WriteCDBTTree();
  delete cdbttree;
  gSystem->Exit(0);
}


void Read(const std::string &fname = "cdbttree.root")
{
  CDBTTree *cdbttree = new CDBTTree(fname);
  cdbttree->LoadCalibrations();
//  cdbttree->Print();
//  return;
  for (unsigned int channel = 0; channel < cdbttree->GetUInt64EntryMap().size(); channel++)
  {
    std::cout << "BCO: " << cdbttree->GetUInt64Value(channel,"bco",1) << std::endl;
    std::cout << "S11R3G4: " << cdbttree->GetFloatValue(channel,"S11R3G4") << std::endl;
  }
  delete cdbttree;
  gSystem->Exit(0);
}
