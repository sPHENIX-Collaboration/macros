// -- c++ includes --
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <map>

// -- root includes --
#include <TFile.h>
#include <TProfile2D.h>
#include <TLatex.h>

#include <calobase/TowerInfoDefs.h>
#include <cdbobjects/CDBTTree.h>
#include <emcnoisytowerfinder/emcNoisyTowerFinder.h>

using std::cout;
using std::endl;
using std::string;
using std::stringstream;
using std::map;

namespace fs = std::filesystem;

namespace myAnalysis
{

  void histToCaloCDBTree(string outputfile, string fieldName, Int_t icalo, TProfile2D *hist);
  void analyze(const string &output);

  // utils
  void setRunDataset(const string &input);
  Int_t readHists(const string &input);

  string m_run;
  string m_dataset;

  TProfile2D *h_CaloValid_cemc_etaphi_badChi2 = nullptr;
  TProfile2D *h_CaloValid_ihcal_etaphi_badChi2 = nullptr;
  TProfile2D *h_CaloValid_ohcal_etaphi_badChi2 = nullptr;

  TProfile2D *h_CaloValid_cemc_etaphi_time_raw = nullptr;
  TProfile2D *h_CaloValid_ihcal_etaphi_time_raw = nullptr;
  TProfile2D *h_CaloValid_ohcal_etaphi_time_raw = nullptr;

  Int_t cemc_bins_eta = 96;
  Int_t cemc_bins_phi = 256;
  Int_t hcal_bins_eta = 24;
  Int_t hcal_bins_phi = 64;

}  // namespace myAnalysis

void myAnalysis::setRunDataset(const string &input)
{
  string basename = fs::path(input).filename().stem().string();
  m_run = basename.substr(0, basename.find("_"));
  m_dataset = basename.substr(basename.find("_") + 1, basename.size() - basename.find("_"));
}

Int_t myAnalysis::readHists(const string &input)
{
  // Create an input stream
  std::ifstream file(input);

  // Check if the file was successfully opened
  if (!file.is_open())
  {
    cout << "Failed to open file list: " << input << endl;
    return 1;
  }

  cout << "Reading Hists" << endl;
  cout << "======================================" << endl;

  delete h_CaloValid_cemc_etaphi_badChi2;
  delete h_CaloValid_ihcal_etaphi_badChi2;
  delete h_CaloValid_ohcal_etaphi_badChi2;

  delete h_CaloValid_cemc_etaphi_time_raw;
  delete h_CaloValid_ihcal_etaphi_time_raw;
  delete h_CaloValid_ohcal_etaphi_time_raw;

  h_CaloValid_cemc_etaphi_badChi2 = new TProfile2D("cemc_etaphi_badChi2","", cemc_bins_eta, 0, cemc_bins_eta, cemc_bins_phi, 0, cemc_bins_phi);
  h_CaloValid_ihcal_etaphi_badChi2 = new TProfile2D("ihcal_etaphi_badChi2","", hcal_bins_eta, 0, hcal_bins_eta, hcal_bins_phi, 0, hcal_bins_phi);
  h_CaloValid_ohcal_etaphi_badChi2 = new TProfile2D("ohcal_etaphi_badChi2","", hcal_bins_eta, 0, hcal_bins_eta, hcal_bins_phi, 0, hcal_bins_phi);

  h_CaloValid_cemc_etaphi_time_raw = new TProfile2D("cemc_etaphi_time_raw","", cemc_bins_eta, 0, cemc_bins_eta, cemc_bins_phi, 0, cemc_bins_phi);
  h_CaloValid_ihcal_etaphi_time_raw = new TProfile2D("ihcal_etaphi_time_raw","", hcal_bins_eta, 0, hcal_bins_eta, hcal_bins_phi, 0, hcal_bins_phi);
  h_CaloValid_ohcal_etaphi_time_raw = new TProfile2D("ohcal_etaphi_time_raw","", hcal_bins_eta, 0, hcal_bins_eta, hcal_bins_phi, 0, hcal_bins_phi);

  string line;
  map<string, Int_t> ctr;
  while (std::getline(file, line))
  {
    ++ctr["total_files"];

    cout << "Reading File: " << line << endl;
    auto tf = TFile::Open(line.c_str());

    if (!tf || tf->IsZombie()) {
        cout << "Error: Could not open ROOT file: " << line << endl;
        continue; // Indicate an error
    }

    ++ctr["successfully_opened_files"];

    auto h = static_cast<TProfile2D*>(tf->Get("h_CaloValid_cemc_etaphi_badChi2"));

    if (h) {
      h_CaloValid_cemc_etaphi_badChi2->Add(h);
      ++ctr["h_CaloValid_cemc_etaphi_badChi2"];
    }

    h = static_cast<TProfile2D*>(tf->Get("h_CaloValid_ihcal_etaphi_badChi2"));

    if (h) {
      h_CaloValid_ihcal_etaphi_badChi2->Add(h);
      ++ctr["h_CaloValid_ihcal_etaphi_badChi2"];
    }

    h = static_cast<TProfile2D*>(tf->Get("h_CaloValid_ohcal_etaphi_badChi2"));

    if (h) {
      h_CaloValid_ohcal_etaphi_badChi2->Add(h);
      ++ctr["h_CaloValid_ohcal_etaphi_badChi2"];
    }

    h = static_cast<TProfile2D*>(tf->Get("h_CaloValid_cemc_etaphi_time_raw"));

    if (h) {
      h_CaloValid_cemc_etaphi_time_raw->Add(h);
      ++ctr["h_CaloValid_cemc_etaphi_time_raw"];
    }

    h = static_cast<TProfile2D*>(tf->Get("h_CaloValid_ihcal_etaphi_time_raw"));

    if (h) {
      h_CaloValid_ihcal_etaphi_time_raw->Add(h);
      ++ctr["h_CaloValid_ihcal_etaphi_time_raw"];
    }

    h = static_cast<TProfile2D*>(tf->Get("h_CaloValid_ohcal_etaphi_time_raw"));

    if (h) {
      h_CaloValid_ohcal_etaphi_time_raw->Add(h);
      ++ctr["h_CaloValid_ohcal_etaphi_time_raw"];
    }

    tf->Close();
  }

  cout << "===============================" << endl;
  cout << "Stats" << endl;
  cout << "Successfully opened files: " << ctr["successfully_opened_files"] << ", " << ctr["successfully_opened_files"]*100./ctr["total_files"] << " %" << endl;
  for (const auto& [name, value] : ctr) {
    if(name.starts_with("h_CaloValid")) {
      cout << "Hist: " << name << ", Found: " << value << ", " << value * 100./ ctr["successfully_opened_files"] << " %" << endl;
    }
  }
  cout << "===============================" << endl;

  // Close the file
  file.close();

  return 0;
}

void myAnalysis::histToCaloCDBTree(string outputfile, string fieldName, Int_t icalo, TProfile2D *hist)
{
  UInt_t neta, nphi;

  if (icalo != 0 && icalo != 1) return;

  if (icalo == 0)
  {
    neta = 96;
    nphi = 256;
  }
  if (icalo == 1)
  {
    neta = 24;
    nphi = 64;
  }

  CDBTTree *cdbttree = new CDBTTree(outputfile);

  Double_t mean = 0;
  Int_t count = 0;

  for (UInt_t ie = 0; ie < neta; ie++)
  {
    for (UInt_t ip = 0; ip < nphi; ip++)
    {
      UInt_t key;
      if (icalo == 0) key = TowerInfoDefs::encode_emcal(ie, ip);
      if (icalo == 1) key = TowerInfoDefs::encode_hcal(ie, ip);
      Float_t val = static_cast<Float_t>(hist->GetBinContent(static_cast<Int_t>(ie) + 1, static_cast<Int_t>(ip) + 1));
      cdbttree->SetFloatValue(static_cast<Int_t>(key), fieldName, val);
      mean += val;
      count++;
    }
  }

  cout << "Writing " << outputfile.c_str() << "   with mean=" << mean / count << endl;
  cdbttree->Commit();
  cdbttree->WriteCDBTTree();
  // cdbttree->Print();
  delete cdbttree;
}

void myAnalysis::analyze(const string &outputDir)
{
  string detector = "CEMC";
  // fracBadChi2
  string payloadName = outputDir + "/" + detector + "_hotTowers_fracBadChi2" + "_" + m_dataset + "_" + m_run + ".root";
  if (h_CaloValid_cemc_etaphi_badChi2) histToCaloCDBTree(payloadName, "fraction", 0, h_CaloValid_cemc_etaphi_badChi2);
  // time
  payloadName = outputDir + "/" + detector + "_meanTime" + "_" + m_dataset + "_" + m_run + ".root";
  if (h_CaloValid_cemc_etaphi_time_raw) histToCaloCDBTree(payloadName, "time", 0, h_CaloValid_cemc_etaphi_time_raw);

  detector = "HCALIN";
  // fracBadChi2
  payloadName = outputDir + "/" + detector + "_hotTowers_fracBadChi2" + "_" + m_dataset + "_" + m_run + ".root";
  if (h_CaloValid_ihcal_etaphi_badChi2) histToCaloCDBTree(payloadName, "fraction", 1, h_CaloValid_ihcal_etaphi_badChi2);
  // time
  payloadName = outputDir + "/" + detector + "_meanTime" + "_" + m_dataset + "_" + m_run + ".root";
  if (h_CaloValid_ihcal_etaphi_time_raw) histToCaloCDBTree(payloadName, "time", 1, h_CaloValid_ihcal_etaphi_time_raw);

  detector = "HCALOUT";
  // fracBadChi2
  payloadName = outputDir + "/" + detector + "_hotTowers_fracBadChi2" + "_" + m_dataset + "_" + m_run + ".root";
  if (h_CaloValid_ohcal_etaphi_badChi2) histToCaloCDBTree(payloadName, "fraction", 1, h_CaloValid_ohcal_etaphi_badChi2);
  // time
  payloadName = outputDir + "/" + detector + "_meanTime" + "_" + m_dataset + "_" + m_run + ".root";
  if (h_CaloValid_ohcal_etaphi_time_raw) histToCaloCDBTree(payloadName, "time", 1, h_CaloValid_ohcal_etaphi_time_raw);
}

void genStatus(const string &input, const string &output = "output")
{
  cout << "#############################" << endl;
  cout << "Run Parameters" << endl;
  cout << "input: " << input << endl;
  cout << "output: " << output << endl;
  cout << "#############################" << endl;

  myAnalysis::setRunDataset(input);

  cout << "Processing: Run: " << myAnalysis::m_run << ", Dataset: " << myAnalysis::m_dataset << endl;

  stringstream datasetDir;
  datasetDir.str("");
  datasetDir << output << "/" << myAnalysis::m_run << "_" << myAnalysis::m_dataset;

  string outputDir = datasetDir.str();

  string hotMapFile = "EMCalHotMap_"+myAnalysis::m_dataset+"_"+myAnalysis::m_run+".root";
  string hotMapOutput = datasetDir.str()+ "/" + hotMapFile;

  datasetDir << "/QA";

  // create output & QA directory
  std::filesystem::create_directories(datasetDir.str());

  string s_input = input;
  string hotMapOutputQA = datasetDir.str()+ "/" + hotMapFile;

  // merges individal qa into one per run
  myAnalysis::readHists(input);
  myAnalysis::analyze(outputDir);

  emcNoisyTowerFinder* calo = new emcNoisyTowerFinder();
  calo->FindHot(s_input, hotMapOutput, "h_CaloValid_cemc_etaphi");

  if (fs::exists(hotMapOutput)) {
    fs::rename(hotMapOutput, hotMapOutputQA);
  }
  else {
    cout << "ERROR: EMCal Hot Map FAILED to Create." << endl;
  }
}

#ifndef __CINT__
Int_t main(Int_t argc, const char* const argv[])
{
  if (argc < 2 || argc > 3)
  {
    cout << "usage: ./genStatus input [output]" << endl;
    cout << "input: input list" << endl;
    cout << "output: output directory" << endl;
    return 1;
  }

  string output = "output";

  if (argc >= 3)
  {
    output = argv[2];
  }

  genStatus(argv[1], output);

  cout << "======================================" << endl;
  cout << "done" << endl;
  return 0;
}
#endif
