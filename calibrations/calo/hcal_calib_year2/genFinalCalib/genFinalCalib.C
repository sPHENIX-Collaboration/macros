#include"tsc_cos_merge.C"
#include "getCosmicTemps.C"       // temperature of a cosmics run from the DB
#include "../HcalCalibDatasetinputs.h" // temperature slope, cosmics runs and their temperatures of each dataset (run2pp, run2auau, run3oo)

#include <TSystem.h>

#include <unistd.h>  // isatty

#include <algorithm>
#include <format>
#include <fstream>
#include <set>
#include <sstream>
#include <string>

struct TowerData {
    int tower_ieta;
    int tower_iphi;
    float calib_factor;
    int run_range_low;
    int run_range_high;
};
void loadCSV(const std::string& filename, std::vector<TowerData>& data);
std::vector<std::vector<float>> getTowersForRun(const std::vector<TowerData>& data, int run_number);
float getBeamTemp(int runnumber, const std::string& tempDir, const std::string& detDB);


// Final calibration files of a dataset: cosmics + towerslope (../genTSCalib) x temperature correction (x half height).
// The inputs of the dataset are taken from ../HcalCalibDatasetinputs.h and printed before the files are made.
//
//   root -l -b -q 'genFinalCalib.C("run3oo", 0)'    OHCal
//   root -l -b -q 'genFinalCalib.C("run3oo", 1)'    IHCal
//
// dataset:  run2pp, run2auau, run3oo, ... The header gives the beam run list, the towerslope periods
//           (or the cosmics runs for datasets without periods), the temperature slope and the cosmics temperatures.
// isIHcal:  0 OHCal, 1 IHCal
// tempDir:  folder with the temperature maps of the beam runs from ../makeTemp/loopCaloTemp.C
//           (HCALOUT_temp_<run>.root, HCALIN_temp_<run>.root), "" for ../makeTemp/temp_ohcal (temp_ihcal)
// hhList:   half height list, "" for no half height correction
// tscDir:   folder with the cosmics + towerslope files of ../genTSCalib ({o,i}hcal_cdb_tsc_cos_calib_<run>.root)
void genFinalCalib(const std::string &dataset = "", int isIHcal = 0, std::string tempDir = "",
                   const std::string &hhList = "", const std::string &tscDir = "../genTSCalib/cdbFiles")
{
  std::string det = (isIHcal == 1) ? "ihcal" : "ohcal";
  std::string detDB = (isIHcal == 1) ? "HCALIN" : "HCALOUT";
  if (tempDir.empty()) tempDir = "../makeTemp/temp_" + det;

  ///////////////////////////////////////
  // inputs of the dataset (../HcalCalibDatasetinputs.h)
  ///////////////////////////////////////
  const HCalCalibDataset *dsPtr = findHCalCalibDataset(dataset);
  if (!dsPtr)
  {
    std::cout << "ERROR: no dataset \"" << dataset << "\" in HcalCalibDatasetinputs.h (" << hcalCalibDatasetNames() << ")" << std::endl;
    return;
  }
  const HCalCalibDataset &ds = *dsPtr;
  int nMissing = 0;  // inputs that are missing

  // temperature slope of the dataset and detector
  float alpha = (isIHcal == 1) ? ds.ih_tempSlope : ds.oh_tempSlope;
  if (alpha <= 0)
  {
    std::cout << "MISSING: temperature slope of " << det << " for " << ds.name << " in HcalCalibDatasetinputs.h" << std::endl;
    nMissing++;
  }

  // beam runs
  std::string line;
  std::set<int> tempset;
  std::ifstream runlistBeam(ds.runList);
  if (ds.runList.empty() || !runlistBeam.is_open())
  {
    std::cout << "MISSING: beam run list of " << ds.name << " (runList in HcalCalibDatasetinputs.h): " << ds.runList << std::endl;
    nMissing++;
  }
  while(getline(runlistBeam,line)){
    if (line.empty() || line[0] == '#') continue;
    tempset.insert(stoi(line));
  }
  std::vector<int> runnumbersBeam(tempset.begin(),tempset.end());
  if (runlistBeam.is_open() && runnumbersBeam.empty())
  {
    std::cout << "MISSING: no runs in the beam run list " << ds.runList << std::endl;
    nMissing++;
  }

  // cosmics + towerslope files: one per towerslope period, or one per cosmics run for datasets without periods.
  // They are made in ../genTSCalib (run_cosmicsTS.C), each towerslope merged with its own cosmics file.
  bool hasPeriods = !ds.tscPeriods.empty();
  std::vector<int> cosmic_runnumbers = hasPeriods ? ds.tscPeriods : ((isIHcal == 1) ? ds.ih_cosmicRuns : ds.oh_cosmicRuns);
  if (cosmic_runnumbers.empty())
  {
    std::cout << "MISSING: no towerslope periods and no " << det << " cosmics runs for " << ds.name << " in HcalCalibDatasetinputs.h" << std::endl;
    nMissing++;
  }
  for (int cos_run : cosmic_runnumbers)
  {
    std::string name = std::format("{}/{}_cdb_tsc_cos_calib_{}.root", tscDir, det, cos_run);
    if (gSystem->AccessPathName(name.c_str()))
    {
      std::cout << "MISSING: cosmics + towerslope file " << name << std::endl;
      nMissing++;
    }
  }

  // temperature maps of the beam runs
  std::vector<int> noTemp;
  for (int run : runnumbersBeam)
  {
    if (gSystem->AccessPathName(std::format("{}/{}_temp_{}.root", tempDir, detDB, run).c_str())) noTemp.push_back(run);
  }
  if (!runnumbersBeam.empty() && noTemp.size() == runnumbersBeam.size())
  {
    std::cout << "MISSING: no temperature map of the beam runs in " << tempDir << " (" << detDB << "_temp_<run>.root)" << std::endl;
    nMissing++;
  }

  // half height list
  if (!hhList.empty() && gSystem->AccessPathName(hhList.c_str()))
  {
    std::cout << "MISSING: half height list " << hhList << std::endl;
    nMissing++;
  }

  if (nMissing > 0)
  {
    std::cout << "ERROR: " << nMissing << " input(s) missing for " << ds.name << " " << det << ", nothing made" << std::endl;
    return;
  }

  ///////////////////////////
  // get cosmic run temps
  ///////////////////////////
  // temperature of each cosmics run itself (towers with 10 < T < 32 C, as for the beam runs):
  // the value noted in ../HcalCalibDatasetinputs.h, or from the DB if the run is not noted there.
  // From the DB it is the average over the cosmics runs from the first to the last run of the cosmics period
  // (oh_cosmicRuns to oh_cosmicLastRuns in the header), the start of the cosmics run if no last run is noted.
  // A dataset with one cosmics base (run3oo) has one cosmics temperature for all runs.
  std::vector<float> cosmic_temps;
  std::vector<std::string> cosmic_tempFrom;
  for (int cos_run : cosmic_runnumbers){
    float cos_temp = hcalCosmicTemp(ds, isIHcal, cos_run);
    cosmic_tempFrom.push_back("header");
    if (cos_temp <= 0){
      int cos_last = hcalCosmicLastRun(ds, isIHcal, cos_run);
      if (cos_last > cos_run) cos_temp = fetch_hcal_temp_degC_period(cos_run, cos_last, detDB); // functions from getCosmicTemps.C
      else cos_temp = fetch_hcal_temp_degC(cos_run, detDB);
      cosmic_tempFrom.back() = "DB";
    }
    cosmic_temps.push_back(cos_temp);
  }

  // output folder and file names from the dataset and the corrections that are made:
  // final_<det>_calib_<dataset>/cdb_<det>_<dataset>_cosmic_tsc_temp[_hh]_<first run>_<last run>.root
  std::string outDir = std::format("final_{}_calib_{}", det, ds.name);
  std::string outName = std::format("{}/cdb_{}_{}_cosmic_tsc_temp{}", outDir, det, ds.name, hhList.empty() ? "" : "_hh");

  ///////////////////////////////////////
  // show the inputs
  ///////////////////////////////////////
  std::cout << "==================================================================" << std::endl;
  std::cout << "genFinalCalib: " << ds.name << " " << det << std::endl;
  std::cout << "  beam run list      : " << ds.runList << std::endl;
  std::cout << "                       " << runnumbersBeam.size() << " runs, " << runnumbersBeam.front() << " to " << runnumbersBeam.back() << std::endl;
  std::cout << "  cosmics + tsc files: " << tscDir << "/" << det << "_cdb_tsc_cos_calib_<run>.root, "
            << cosmic_runnumbers.size() << (hasPeriods ? " towerslope period(s)" : " cosmics run(s)") << std::endl;
  for (size_t ic = 0; ic < cosmic_runnumbers.size(); ic++)
  {
    std::cout << "                       " << cosmic_runnumbers[ic] << "  T_cosmic = " << cosmic_temps[ic] << " C (" << cosmic_tempFrom[ic] << ")" << std::endl;
  }
  std::cout << "  temperature slope  : " << alpha << " per C" << std::endl;
  std::cout << "  temperature maps   : " << tempDir << std::endl;
  std::cout << "                       " << runnumbersBeam.size() - noTemp.size() << " of " << runnumbersBeam.size() << " beam runs have a map" << std::endl;
  if (!noTemp.empty())
  {
    std::cout << "                       no map (no file will be made):";
    for (int run : noTemp) std::cout << " " << run;
    std::cout << std::endl;
  }
  std::cout << "  half height list   : " << (hhList.empty() ? "none (no half height correction)" : hhList) << std::endl;
  std::cout << "  output             : " << outName << "_<first>_<last>.root" << std::endl;
  std::cout << "==================================================================" << std::endl;
  // asked for in a terminal only, so that the macro still runs in a script or a condor job
  if (isatty(fileno(stdin)))
  {
    std::cout << "make the files with these inputs? (y/n): " << std::flush;
    std::string answer;
    std::getline(std::cin, answer);
    if (answer != "y" && answer != "Y" && answer != "yes")
    {
      std::cout << "stopped, nothing made" << std::endl;
      return;
    }
  }

  gSystem->mkdir(outDir.c_str(), kTRUE);

  ///////////////////////////////////
  // Load the half hieght data from the CSV file
  ///////////////////////////////////////
  std::vector<TowerData> hh_data;
  if (!hhList.empty()) loadCSV(hhList, hh_data);


  ///////////////////////////////////////
  // Generate the run-by-run calibrations
  ///////////////////////////////////////
  // This follows the logic laid out in the note
  // cosmics calibration is the base
  // the tsc is the used to adjust the cosmic calib
  // then the temperture difference between the cosmic
  // run and the run of interest to correct the 
  // adjusted cosmics for the rinal calibration
  for(int irb=0; irb<runnumbersBeam.size(); irb++)
  {
    int runnumber = runnumbersBeam[irb];  

    int runnumber_last;
    if (irb+1  < runnumbersBeam.size() ) runnumber_last = runnumbersBeam[irb+1]-1;
    else runnumber_last = std::max(ds.lastRun, runnumber); // last run of the dataset in ../HcalCalibDatasetinputs.h

    std::cout << "----------------------------------" << std::endl;
    std::cout << "starting run " << runnumber << std::endl;

    int asoCosmic = 0;
    int minCosDelta = 1000000;
    float asoCosmicTemp = 0;
    for(int ic=0; ic< cosmic_runnumbers.size(); ic++){
       int cos_run = cosmic_runnumbers[ic]; 
       int delta = runnumber - cos_run;
       // delta = 0: datasets with towerslope periods, the list holds the first beam run of each period
       if (minCosDelta  > delta && delta >= 0){
         asoCosmic     = cos_run; 
         asoCosmicTemp =  cosmic_temps[ic]; 
         minCosDelta = delta;
       }
    }
    std::cout << "closeset cosmic run " << asoCosmic << std::endl;
    if (asoCosmic == 0){
      std::cout << "run " << runnumber << " skipped, it is before the first " << (hasPeriods ? "towerslope period" : "cosmics run") << " of " << ds.name << std::endl;
      continue;
    }


    { // temperature of the beam run from its temperature map (0 if there is no map: the run is skipped)
      {
         float temp = getBeamTemp(runnumber, tempDir, detDB);
         if (temp < 10) std::cout << "WARNING: no temperature of run " << runnumber << " in " << tempDir << ", no file made for it" << std::endl;
         float deltaT =  temp - asoCosmicTemp; 

         std::cout << "of this run = " << temp << "   temp of cosmic run =" << asoCosmicTemp << std::endl;

         float correction = 1+ deltaT*alpha;
         if (temp < 10) continue;
         std::cout << "correction=" << correction << std::endl;

         // std::string cdbFileName = std::format("cdbFiles/cdb_ohcal_tsc_temp_cosmic_{}.root",runnumber);
         //std::string cosmic_CDB_file_oh = std::format("/sphenix/u/bseidlitz/work/macros/calibrations/calo/hcal_towerSlope_y2/cosmicCalibFiles/ohcal_cosmic_calibration_{}.root",asoCosmic);
         std::string cosmic_CDB_file_oh = std::format("{}/{}_cdb_tsc_cos_calib_{}.root",tscDir,det,asoCosmic);

          // gen calibration file with cosmics, tsc and temp correction
          // but no half hieght corrections
         //genCdbCorr(correction,cosmic_CDB_file_oh,cdbFileName,0); 

         // with half hieght correction
         std::vector<std::vector<float>> hh_towersForRun = getTowersForRun(hh_data, runnumber);
         std::string cdbFileName_hh = std::format("{}_{}_{}.root",outName,runnumber,runnumber_last);
         genCdbCorr_HH(correction,cosmic_CDB_file_oh,cdbFileName_hh,isIHcal,hh_towersForRun); 
      }
    }
  }



}//end macro



// average temperature of the towers with 10 < T < 32 C of a beam run, from the map made by
// ../makeTemp/loopCaloTemp.C (same selection as for the cosmics runs in getCosmicTemps.C). 0 if there is no map.
float getBeamTemp(int runnumber, const std::string& tempDir, const std::string& detDB)
{
  std::string name = std::format("{}/{}_temp_{}.root", tempDir, detDB, runnumber);
  if (gSystem->AccessPathName(name.c_str())) return 0;
  TFile f(name.c_str());
  TH2* h = (TH2*) f.Get(std::format("h_{}_temp", detDB).c_str());
  if (!h) return 0;
  float sum = 0;
  int cnt = 0;
  for (int ix = 1; ix <= h->GetNbinsX(); ix++)
  {
    for (int iy = 1; iy <= h->GetNbinsY(); iy++)
    {
      float t = h->GetBinContent(ix, iy);
      if (t > 10 && t < 32)
      {
        sum += t;
        cnt++;
      }
    }
  }
  if (cnt < 3) return 0;
  return sum / cnt;
}

void loadCSV(const std::string& filename, std::vector<TowerData>& data) {
    std::ifstream file(filename);
    std::string line;
    if(!file.is_open())
    {
      std::cout << std::endl << "ccan't find file" << std::endl;
      gSystem->Exit(1);
    }

    while (getline(file, line)) {
        std::stringstream ss(line);
        TowerData towerData{};

        // Parse the line and fill the struct
        char comma;  // To consume the commas between values
        ss >> towerData.tower_ieta >> comma
           >> towerData.tower_iphi >> comma
           >> towerData.calib_factor >> comma
           >> towerData.run_range_low >> comma
           >> towerData.run_range_high;

        // Store the parsed data
        data.push_back(towerData);
    }
}


std::vector<std::vector<float>> getTowersForRun(const std::vector<TowerData>& data, int run_number)
{
    std::vector<std::vector<float>> towersForRun;

    // Loop through all the entries and check if the run_number is within the range
    for (const auto& towerData : data) {
        if (run_number > towerData.run_range_low && run_number < towerData.run_range_high) {
            // If it matches the range, store the tower data
            towersForRun.push_back({static_cast<float>(towerData.tower_ieta),
                                    static_cast<float>(towerData.tower_iphi),
                                    towerData.calib_factor});
        }
    }

    return towersForRun;
}

