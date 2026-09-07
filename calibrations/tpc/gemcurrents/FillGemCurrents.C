#include <cdbobjects/CDBTTree.h>

#include <TSystem.h>

#include <cmath>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

R__LOAD_LIBRARY(libcdbobjects.so)

void FillGemCurrents(const int runnumber, bool verbose=false)
{
  std::string input_file  ="ConditionsCSVFiles/run_" +std::to_string(runnumber) +"_GEM_BCO.csv";
  std::string output_file ="ConditionsRootFiles/run_" +std::to_string(runnumber) +"_GEM_BCO.root";

  gSystem->mkdir("ConditionsRootFiles", true);  /// just in case it is not already there...

  std::ifstream in(input_file);
  std::string line;
  int channel = -1; // so we start with channel 0
  CDBTTree *cdbttree = new CDBTTree(output_file);
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
  if (verbose) {cdbttree->Print();}
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
    std::cout << "S_11_R3_G4_IMon: " << cdbttree->GetFloatValue(channel,"S_11_R3_G4_IMon") << std::endl;
    std::cout << "gas_temperature: " << cdbttree->GetFloatValue(channel,"gas_temperature") << std::endl;
    std::cout << "gas_pressure: " << cdbttree->GetFloatValue(channel,"gas_pressure") << std::endl;
    std::cout << "FieldOK: " << cdbttree->GetFloatValue(channel,"FieldOK") << std::endl;
    std::cout << "GainOK: " << cdbttree->GetFloatValue(channel,"GainOK") << std::endl;
  }
  delete cdbttree;
  gSystem->Exit(0);
}
