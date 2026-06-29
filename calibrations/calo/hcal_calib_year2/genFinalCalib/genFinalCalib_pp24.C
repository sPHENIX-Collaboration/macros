#include "tsc_cos_merge.C"  // provides genCdbCorr_HH / genCdbCorr
#include "getCosmicTemps.C" // This macro reads the temp of cosmic runs from DB and it takes few min

#include <TSystem.h>
#include <TFile.h>
#include <TH1F.h>
#include <TString.h>

#include <iostream>
#include <format>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <climits>


// Input files needed
// cos_runs_pp_2024.txt, runList_pp_2024_ana509.txt, anaOut.root, cdbFiles/ohcal_cdb_tsc_cos_calib_%d.root 

float fetch_ohcal_temp_degC(int run,
			    int max_retries = 3,
			    int backoff_ms  = 400)
{
  // default detector is OHCal
  return fetch_hcal_temp_degC(run, "HCALOUT", max_retries, backoff_ms);
}

struct TowerData {
  int   tower_ieta;
  int   tower_iphi;
  float calib_factor;
  int   run_range_low;
  int   run_range_high;
};

void loadCSV(const std::string& filename, std::vector<TowerData>& data);
std::vector<std::vector<float>> getTowersForRun(const std::vector<TowerData>& data, int run_number);

void genFinalCalib_pp24(const char *runs = "cos_runs_pp_2024.txt")
{
  std::ofstream csv_log("beam_cosmic_tempcorr_log_nov22_abscosmic.csv");
  csv_log << "beam_run,cosmic_run,t_beam,t_cosmic,correction\n";

  gSystem->mkdir("final_ohcal_calib_run2pp_hh_abscosmic_nov23", kTRUE);

  // ---- read cosmic runs ----
  std::ifstream runlist(runs);
  std::vector<int> cosmic_runnumbers;
  std::string line;
  while (getline(runlist,line)) {
    if (line.empty() || line[0]=='#') continue;
    cosmic_runnumbers.push_back(stoi(line));
  }
  sort(cosmic_runnumbers.begin(), cosmic_runnumbers.end());

  // ---- read beam runs ----
  std::ifstream runlistBeam("runList_pp_2024_ana509.txt");
  std::vector<int> runnumbersBeam;
  while (getline(runlistBeam, line)) {
    if (line.empty() || line[0]=='#') continue;
    runnumbersBeam.push_back(stoi(line));
  }

  const int RUN_MAX = runnumbersBeam.back(); // get the last element in the list
  std::cout << "last run in the list of beam runs is:" << RUN_MAX << std::endl;

  std::vector<int> runsWritten;   // to collect missing runs
 
  // ---- open anaOut.root ----
  TFile* ftemp = TFile::Open("analyze_hits/anaOut.root","READ"); // input file from ana_hits.C macro
  TH1F* h_runnumbers_temp = (TH1F*) ftemp->Get("runNumberHist");
  TH1F* h_temp_run        = (TH1F*) ftemp->Get("h_temp_run");
  int nBins = h_runnumbers_temp->GetNbinsX();

  // ---- get temperature for cosmic runs (approx: nearest beam bin) ----
  /*
  std::vector<float> cosmic_temps;
  for (int cos_run : cosmic_runnumbers) {
    int minDelta = INT_MAX;
    float cos_temp = 0;
    int matched_beam = -1;
    for (int b=1; b<=nBins; b++) {
      int   beam_run = (int)h_runnumbers_temp->GetBinContent(b);
      float tbin     =      h_temp_run->GetBinContent(b);
      int delta = abs(cos_run - beam_run);
      if (delta < minDelta && tbin > 10) {
        minDelta = delta;
        cos_temp = tbin;
	matched_beam = beam_run;
      }
    }
    cosmic_temps.push_back(cos_temp);
  // --- print debug info ---
  std::cout << "[Cosmic] Run " << cos_run
       << " → nearest beam run " << matched_beam
       << " with temperature = " << cos_temp << " C"
       << std::endl;
  }
  */

  // ---- get temperature for cosmic runs from DB via getCosmicTemps.C ----

  std::vector<float> cosmic_temps;
  cosmic_temps.reserve(cosmic_runnumbers.size());

  for (int cos_run : cosmic_runnumbers) {
    float cos_temp = fetch_ohcal_temp_degC(cos_run);   // function from getCosmicTemps.C
    cosmic_temps.push_back(cos_temp);

    std::cout << "[Cosmic] Run " << cos_run
         << " | extracted OHCal temperature = " << cos_temp << " C"
         << std::endl;
  }


  // ---- load half-height corrections ----
  std::vector<TowerData> hh_data;
  loadCSV("halfHeights_pp_2024_anjaly_dec6.csv", hh_data);

  // ---- loop over beam runs ----
  for (size_t irb=0; irb<runnumbersBeam.size(); irb++) {
    int runnumber = runnumbersBeam[irb];
    
    int runnumber_last;
    if (irb+1  < runnumbersBeam.size() ) runnumber_last = runnumbersBeam[irb+1]-1;
    else runnumber_last = RUN_MAX;
    
    std::cout << "----------------------------------" << std::endl;
    std::cout << "starting run " << runnumber << std::endl;

    int asoCosmic = 0;
    int minDelta  = INT_MAX;
    float asoCosmicTemp = 0;
    for (size_t ic=0; ic<cosmic_runnumbers.size(); ic++) {
      int cos_run = cosmic_runnumbers[ic];
      int delta   =  std::abs(runnumber - cos_run);  // absolute difference
      //int delta   =  runnumber - cos_run; //  if (delta < minDelta && delta <= 0) 
      if (delta < minDelta)  { 
        minDelta       = delta;
        asoCosmic      = cos_run;
        asoCosmicTemp  = cosmic_temps[ic];
      }
    }

    // get beam temp directly from histogram
    float temp_beam = 0;
    for (int it = 1; it <=nBins; it++) {
      if ((int)h_runnumbers_temp->GetBinContent(it) == runnumber) {
        temp_beam = h_temp_run->GetBinContent(it);
        break;
      }
    }

    if (temp_beam < 10) {
      std::cout << "Run " << runnumber << " skipped (temp < 10)" << std::endl;
      continue;
    }

    float alpha = 0.045;
    float deltaT = temp_beam - asoCosmicTemp;
    float correction = 1 + alpha*deltaT;

    std::cout << "Beam run " << runnumber
         << " uses cosmic " << asoCosmic
         << " : T_beam=" << temp_beam
         << " T_cosmic=" << asoCosmicTemp
         << " corr=" << correction << std::endl;

    csv_log << runnumber << "," << asoCosmic << "," << temp_beam << "," << asoCosmicTemp << "," << correction << "\n";


    std::string cosmic_CDB_file_oh = std::format("cdbFiles/ohcal_cdb_tsc_cos_calib_{}.root",asoCosmic);  //input file needed
    auto hh_towersForRun = getTowersForRun(hh_data, runnumber);
    std::string cdbFileName_hh = std::format("final_ohcal_calib_run2pp_hh_abscosmic_nov23/cdb_ohcal_tsc_temp_cosmic_hh_{}_{}.root",
                                 runnumber, runnumber_last); //output file name
    genCdbCorr(correction, cosmic_CDB_file_oh, cdbFileName_hh, 0);                       // Without HH correction
    //genCdbCorr_HH(correction, cosmic_CDB_file_oh, cdbFileName_hh, 0, hh_towersForRun); // With HH correction
    runsWritten.push_back(runnumber);
  } //beam run loop ends

  csv_log << "skipped_run\n";
  std::cout << "\n=== Summary of runs with no file written ===\n";
  int skippedCount = 0;
  for (int r : runnumbersBeam) {
    if (std::find(runsWritten.begin(), runsWritten.end(), r) == runsWritten.end()) {
        std::cout << r << " ";
	csv_log << r << "\n";
	skippedCount++;
    }
}
  std::cout << "\nTotal skipped runs: " << skippedCount << std::endl;
  csv_log.close();


}//end macro

// =============== helpers ===============

void loadCSV(const std::string& filename, std::vector<TowerData>& data) {
  std::ifstream file(filename);
  std::string line;
  while (getline(file, line)) {
    if (line.empty() || line[0]=='#') continue;
    std::stringstream ss(line);
    TowerData t{};
    char comma;
    ss >> t.tower_ieta >> comma
       >> t.tower_iphi >> comma
       >> t.calib_factor >> comma
       >> t.run_range_low >> comma
       >> t.run_range_high;
    data.push_back(t);
  }
}

std::vector<std::vector<float>> getTowersForRun(const std::vector<TowerData>& data, int run_number) {
  std::vector<std::vector<float>> towersForRun;
  for (const auto& t : data) {
    if (run_number >= t.run_range_low && run_number <= t.run_range_high) {
      towersForRun.push_back({
        static_cast<float>(t.tower_ieta),
        static_cast<float>(t.tower_iphi),
        t.calib_factor
      });
    }
  }
  return towersForRun;
}

