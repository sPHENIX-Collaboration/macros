// -- c++ includes --
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <map>
#include <vector>

// -- root includes --
#include <CDBUtils.C>

#include "myUtils.C"

using std::cout;
using std::endl;
using std::string;
using std::stringstream;
using std::ofstream;
using std::map;
using std::vector;
using std::pair;

namespace fs = std::filesystem;

namespace myAnalysis
{

  void analyze(const string &input, const string &output);
  void readRunInfo(const string &line);

  vector<pair<string, string>> m_runInfo;
  map<string, Int_t> m_ctr;

  vector<string> m_cdbName = {"CEMC_BadTowerMap"
                            , "CEMC_meanTime", "HCALIN_meanTime", "HCALOUT_meanTime"
                            , "CEMC_hotTowers_fracBadChi2", "HCALIN_hotTowers_fracBadChi2", "HCALOUT_hotTowers_fracBadChi2"};

  Bool_t m_debug = false;
}  // namespace myAnalysis

void myAnalysis::readRunInfo(const string &line) {
    vector<string> tokens = myUtils::split(line, ',');

    if(tokens.size() < 2) {
      cout << "ERROR: line does not contain both RUN and DATASET information! Skipping: " << line << endl;
      return;
    }

    m_runInfo.push_back(make_pair(tokens[0], tokens[1]));
}

void myAnalysis::analyze(const string& input, const string &outputDir) {

  string filename = outputDir + "/" + fs::path(input).filename().stem().string() + "-process.csv";

  ofstream file(filename);

  if (!file.is_open()) {
      cout << "ERROR: Could not open file " << filename << " for writing." << endl;
      return; // Exit if file cannot be opened
  }

  // write the header for the CSV
  file << "runnumber" << endl;

  // loop over each run to check if any is missing the latest calibration
  // if a run is missing the latest calibration then write it to the CSV for processing
  for(const auto &run_dataset : m_runInfo) {
    string run = run_dataset.first;
    string dataset = run_dataset.second;
    ++m_ctr["ctr_run"];
    cout << "Run: " << run << ", Dataset: " << dataset << ", Processing: "
         << m_ctr["ctr_run"] << ", " << m_ctr["ctr_run"] * 100. / static_cast<Double_t>(m_runInfo.size()) << " %" << endl;

    Bool_t keep = false;

    for(const auto &cdbName : m_cdbName) {
      if(m_debug) {
        cout << "Attempt to get Calibration" << endl;
      }
      string cdb = getCalibration(cdbName, std::stoul(run));
      if(m_debug) {
        cout << "Get Calibration Calls: " << ++m_ctr["getCalib_calls"] << endl;
      }

      string suffix = (cdbName == "CEMC_BadTowerMap") ? "EMCalHotMap_" + dataset + "-" + run + "cdb.root" :
                                                         cdbName + "_" + dataset + "-" + run + ".root";

      if(!cdb.ends_with(suffix)) {
        if(cdb.starts_with("DataBaseException")) {
          ++m_ctr["cdb_missing_"+cdbName];
        }
        else {
          ++m_ctr["cdb_outdated_"+cdbName];
        }
        keep = true;
      }
    }

    if(keep) {
      file << run << endl;
    }
  }

  cout << "===============================" << endl;
  cout << "Stats" << endl;
  cout << "Total Runs: " << m_runInfo.size() << endl;
  for (const auto& [name, value] : m_ctr) {
    if(name.starts_with("cdb")) {
      cout << "CDB: " << name << ", Counts: " << value << ", " << value * 100./ static_cast<Double_t>(m_runInfo.size()) << " %" << endl;
    }
  }
  cout << "===============================" << endl;

  file.close();
}

void filter_datasets(const string &input, const string &output = ".", Bool_t debug = false)
{
  cout << "#############################" << endl;
  cout << "Run Parameters" << endl;
  cout << "input: " << input << endl;
  cout << "output: " << output << endl;
  cout << "Debug: " << ((debug) ? "True" : "False") << endl;
  cout << "#############################" << endl;

  myAnalysis::m_debug = debug;

  setGlobalTag("ProdA_2024");

  if(!myUtils::readCSV(input, myAnalysis::readRunInfo)) return;

  myAnalysis::analyze(input, output);
}

#ifndef __CINT__
Int_t main(Int_t argc, const char* const argv[])
{
  if (argc < 2 || argc > 4)
  {
    cout << "usage: ./" << argv[0] << " input [output] [debug]" << endl;
    cout << "input: input csv" << endl;
    cout << "output: Output directory. Default: ." << endl;
    cout << "debug: debug mode. Default false" << endl;
    return 1;
  }

  string output = ".";
  Bool_t debug = false;

  if (argc >= 3)
  {
    output = argv[2];
  }
  if (argc >= 4)
  {
     debug = std::atoi(argv[3]);
  }

  filter_datasets(argv[1], output, debug);

  cout << "======================================" << endl;
  cout << "done" << endl;
  return 0;
}
#endif
