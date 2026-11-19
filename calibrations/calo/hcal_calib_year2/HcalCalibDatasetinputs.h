#ifndef HCALCALIBDATASETINPUTS_H
#define HCALCALIBDATASETINPUTS_H

// One place for the numbers that differ between datasets: run2pp, run2auau and run3oo.
// The macros ask for the dataset of a run number and read the numbers from here,
// so the same macro works for every dataset.
//
// A run belongs to the first dataset whose lastRun is >= the run number, so a
// cosmics run taken before the first beam run of a dataset belongs to that dataset.
// Keep the list ordered by lastRun.
// The 2025 Au+Au and p+p runs (54975-82373) are not in the list, do not use these macros for them:
// they would get the run3oo settings.
//
// Each dataset has four groups of values: spectra, cosmics only, cosmics + tsc, temperature correction.

#include <algorithm>
#include <array>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

struct HCalCalibDataset
{
  std::string name;
  int firstRun{0};  // first beam run with DST_CALOFITTING (for information only)
  int lastRun{0};   // last beam run with DST_CALOFITTING, used to select the dataset

  //------------------------------------------------------------------
  // spectra (Fun4All_HCal.C, condor_histmaker/run.sh)
  //------------------------------------------------------------------
  // beam run list (good run list), one run per line. Change it to the GRL you want to use.
  std::string runList;
  // production tag of the DST_CALOFITTING files: CreateDstList.pl -dataset <name> --tag <dstTag>
  std::string dstTag;

  //------------------------------------------------------------------
  // cosmics only: the baseline calibration (Step 0)
  //------------------------------------------------------------------
  // Cosmics calibration file: the default calibration of Fun4All_HCal.C, and the file the towerslope is merged with.
  // With {} in the name: one file per cosmics run, {} is replaced by the cosmics run number.
  // Without {}: this one file is the base for all runs of the dataset.
  std::string oh_cosmicFile;
  std::string ih_cosmicFile;
  // Cosmics runs. For the files with {} the code takes them from the file names in the folder
  // (a beam run uses the last cosmics run before it); they are noted here as well, and the code
  // tells you when the note and the folder differ.
  std::vector<int> oh_cosmicRuns;
  std::vector<int> ih_cosmicRuns;
  // Last run of the cosmics period of each cosmics run above (same order), when the cosmics calibration was made
  // from several runs. The cosmics temperature is then the average over the cosmics runs from the first to the
  // last run. Empty: the cosmics temperature is the one at the start of the cosmics run.
  std::vector<int> oh_cosmicLastRuns;
  std::vector<int> ih_cosmicLastRuns;
  // Temperature of each cosmics run in degree C (same order as the cosmics runs): the reference of the
  // temperature correction. Average of the towers with 10 < T < 32 C, as for the beam runs
  // (genFinalCalib/getCosmicTemps.C). A cosmics run that is not noted here (or noted as 0) is read from the DB.
  std::vector<float> oh_cosmicTemps;
  std::vector<float> ih_cosmicTemps;

  //------------------------------------------------------------------
  // cosmics + tsc: the towerslope (Step 1, genTSCalib/run_cosmicsTS.C)
  //------------------------------------------------------------------
  // spectra bin width and fit range in GeV, fit minimum per ieta ring (0-23)
  double oh_binWidth{0};
  std::array<float, 24> oh_fitMin{};
  float oh_fitMax{0};
  double ih_binWidth{0};
  std::array<float, 24> ih_fitMin{};
  float ih_fitMax{0};

  // First run of each towerslope period. One towerslope is made per period from all
  // runs of the period, and the run list given to run.sh holds these run numbers.
  // Empty: one towerslope per cosmics run, from the runs after it.
  std::vector<int> tscPeriods;
  // Folder with the cosmics + towerslope files made by genTSCalib ({o,i}hcal_cdb_tsc_cos_calib_<run>.root):
  // the calibration "cosmics_tsc" of Fun4All_HCal.C and condor_histmaker/run.sh
  std::string tscDir;

  //------------------------------------------------------------------
  // temperature correction (Step 2, genFinalCalib/genFinalCalib.C)
  //------------------------------------------------------------------
  // temperature coefficient per degree C: correction = 1 + slope * (T_run - T_cosmic)
  float oh_tempSlope{0};
  float ih_tempSlope{0};
  // Folders with the final files made by genFinalCalib (cosmics + towerslope + temperature, one file per run range):
  // the calibration "cosmics_tsc_temp" of Fun4All_HCal.C and condor_histmaker/run.sh
  std::string oh_finalDir;
  std::string ih_finalDir;
};

// same fit minimum for all 24 eta rings
inline std::array<float, 24> flatFitMin(float v)
{
  std::array<float, 24> a{};
  a.fill(v);
  return a;
}

// fit minimum for the central rings (ieta 4-19) and for the end rings (ieta 0-3 and 20-23, |eta| > 0.7)
inline std::array<float, 24> centralEndFitMin(float central, float end)
{
  std::array<float, 24> a{};
  for (int ie = 0; ie < 24; ie++)
  {
    a[ie] = (ie >= 4 && ie <= 19) ? central : end;
  }
  return a;
}

inline const std::vector<HCalCalibDataset> &hcalCalibDatasets()
{
  static std::vector<HCalCalibDataset> datasets;
  if (!datasets.empty())
  {
    return datasets;
  }

  {
  //====================================================================
  // run2pp (p+p 2024)
  //====================================================================
    HCalCalibDataset ds;
    ds.name = "run2pp";
    ds.firstRun = 47287;
    ds.lastRun = 53880;

    //---- spectra ----
    ds.runList = "/sphenix/user/anjsmenon/work_2026/RunList/Physics/Calo/goodRuns/grls/run2pp_ana509_2024p022_v001_dst_calofitting_grl.list";
    ds.dstTag = "ana509_2024p022_v001";

    //---- cosmics only ----
    // one file per cosmics run, the cosmics runs are the run numbers in the file names
    ds.oh_cosmicFile = "/sphenix/user/anjsmenon/work_2026/cdb_cosmic_only_hcal/run2pp/ohcal_cosmic_calibration_{}.root";
    ds.ih_cosmicFile = "/sphenix/user/anjsmenon/work_2026/cdb_cosmic_only_hcal/run2pp/ihcal_cosmic_calibration_{}.root";
    // cosmics runs of the files in the folder and their temperatures in degree C (towers with 10 < T < 32 C).
    // Only the cosmics runs that a beam run of the run list uses are kept: the last one before the run
    // (Fun4All_HCal.C and genFinalCalib).
    ds.oh_cosmicRuns = {45767, 48795, 49170, 49478, 50572, 50758, 51020, 52930, 53321, 53883};
    ds.oh_cosmicTemps = {22.9749, 20.2160, 19.6201, 19.2413, 20.8888, 20.8888, 21.3948, 21.5909, 21.5941, 21.5605};
    ds.ih_cosmicRuns = {45767, 48795, 53883};
    ds.ih_cosmicTemps = {22.7366, 21.4964, 22.4775};

    //---- cosmics + tsc ----
    ds.oh_binWidth = 0.08;
    ds.oh_fitMin = flatFitMin(0.4);
    ds.oh_fitMax = 2.5;
    ds.ih_binWidth = 0.05;
    ds.ih_fitMin = flatFitMin(0.20);
    ds.ih_fitMax = 1.5;
    // one towerslope per cosmics run

    //---- temperature correction ----
    ds.oh_tempSlope = 0.045;
    ds.ih_tempSlope = 0.045;

    datasets.push_back(ds);
  }

  {
  //====================================================================
  // run2auau (Au+Au 2024)
  //====================================================================
    HCalCalibDataset ds;
    ds.name = "run2auau";
    ds.firstRun = 54128;
    ds.lastRun = 54974;

    //---- spectra ----
    ds.runList = "/sphenix/user/anjsmenon/work_2026/RunList/Physics/Calo/goodRuns/grls/run2auau_ana509_2024p022_v001_dst_calofitting_grl.list";
    // no DST tag set: condor_histmaker/run.sh asks for it

    //---- cosmics only ----
    // one file per cosmics run, the cosmics runs are the run numbers in the file names
    ds.oh_cosmicFile = "/sphenix/user/anjsmenon/work_2026/cdb_cosmic_only_hcal/run2auau/ohcal_cosmic_calibration_{}.root";
    ds.ih_cosmicFile = "/sphenix/user/anjsmenon/work_2026/cdb_cosmic_only_hcal/run2auau/ihcal_cosmic_calibration_{}.root";
    // cosmics runs of the files in the folder and their temperatures in degree C (towers with 10 < T < 32 C).
    // Only the cosmics runs that a beam run of the run list uses are kept: the last one before the run
    // (Fun4All_HCal.C and genFinalCalib).
    ds.oh_cosmicRuns = {53883, 54608};
    ds.oh_cosmicTemps = {21.5605, 21.7239};
    ds.ih_cosmicRuns = {53883, 54608};
    ds.ih_cosmicTemps = {22.4775, 22.5654};

    //---- cosmics + tsc ----
    ds.oh_binWidth = 0.08;
    ds.oh_fitMin = flatFitMin(0.4);
    ds.oh_fitMax = 2.5;
    ds.ih_binWidth = 0.05;
    ds.ih_fitMin = flatFitMin(0.20);
    ds.ih_fitMax = 1.5;
    // one towerslope per cosmics run

    //---- temperature correction ----
    ds.oh_tempSlope = 0.045;
    ds.ih_tempSlope = 0.045;

    datasets.push_back(ds);
  }

  {
  //====================================================================
  // run3oo (O+O 2025)
  // settings from J. Nagle, "O+O HCal calibration in the CDB: good, or needs an update?" (2026-11-02)
  // /sphenix/user/nagle/ProjectGold/hcalslope/handoff_oo_v2
  //====================================================================
    HCalCalibDataset ds;
    ds.name = "run3oo";
    ds.firstRun = 82374;
    ds.lastRun = 82703;

    //---- spectra ----
    ds.runList = "/sphenix/user/anjsmenon/work_2026/RunList/Physics/Calo/goodRuns/grls/run3oo_pro001_pcdb001_v001_dst_calofitting_grl.list";
    ds.dstTag = "pro001_pcdb001_v001";

    //---- cosmics only ----
    // cosmics runs 82716-82744 are the base for all runs (towers next to the dead towers fixed, see cdb_cosmic_only_hcal/fixDeadNeighbors.C)
    ds.oh_cosmicFile = "/sphenix/user/anjsmenon/work_2026/cdb_cosmic_only_hcal/run3oo/ohcal_CDBTTree_82716_82744_onlycos.root";
    ds.ih_cosmicFile = "/sphenix/user/anjsmenon/work_2026/cdb_cosmic_only_hcal/run3oo/ihcal_CDBTTree_82716_82744_onlycos.root";
    // first and last cosmics run, and the temperature in degree C of the whole cosmics period 82716-82744
    // (towers with 10 < T < 32 C): average over all cosmics runs from the first to the last run,
    // run start temperatures averaged with the run length as weight,
    // fetch_hcal_temp_degC_period(82716, 82744, ...) in genFinalCalib/getCosmicTemps.C. The 22 runs merged for the
    // cosmics calibration (/sphenix/user/ahkhan/cosmic_calibrations/offline/runlist_82716_82744.txt) give the same values.
    // At the start of 82716 alone it is 20.4047 and 19.4180. With all towers (dead towers included) it was
    // 20.3834 and 18.2082 (the values the temperature slopes were first fitted with, see the temperature correction below).
    ds.oh_cosmicRuns = {82716};
    ds.oh_cosmicLastRuns = {82744};
    ds.oh_cosmicTemps = {20.5636};
    ds.ih_cosmicRuns = {82716};
    ds.ih_cosmicLastRuns = {82744};
    ds.ih_cosmicTemps = {19.5944};

    //---- cosmics + tsc ----
    ds.oh_binWidth = 0.08;
    ds.oh_fitMin = centralEndFitMin(1.0, 1.2);  // 1.0 GeV for ieta 4-19, 1.2 GeV for ieta 0-3 and 20-23
    ds.oh_fitMax = 2.5;
    ds.ih_binWidth = 0.02;
    ds.ih_fitMin = flatFitMin(0.20);
    ds.ih_fitMax = 1.0;
    // one towerslope from all O+O runs
    ds.tscPeriods = {82374};
    ds.tscDir = "/sphenix/user/anjsmenon/work_2026/hcal_calib_year2oo/genTSCalib/cdbFiles";

    //---- temperature correction ----
    // OHCal: 0.0365 +- 0.0006, Jamie's fit (handoff_oo_v2). With the cosmics and beam temperatures used now
    //        (towers with 10 < T < 32 C, cosmics period average) the same fit gives 0.0360 +- 0.0006.
    // IHCal: 0.0320 +- 0.0001, refitted with the cosmics and beam temperatures used now
    //        (/sphenix/user/anjsmenon/work_2026/redo_alpha_oo_ihcal/fit_alpha.py). It was 0.0368 with the old
    //        temperatures (cosmics temperature 18.2082 from all towers, beam temperature from towers with 15 < T < 30 C).
    ds.oh_tempSlope = 0.0365;
    ds.ih_tempSlope = 0.0320;
    ds.oh_finalDir = "/sphenix/user/anjsmenon/work_2026/hcal_calib_year2oo/genFinalCalib/final_ohcal_calib_run3oo";
    ds.ih_finalDir = "/sphenix/user/anjsmenon/work_2026/hcal_calib_year2oo/genFinalCalib/final_ihcal_calib_run3oo";

    datasets.push_back(ds);
  }

  return datasets;
}

// cosmics calibration file for a cosmics run: {} in the file name of the dataset is replaced by the run number
inline std::string hcalCosmicFile(const std::string &file, const std::string &cosmicRun)
{
  std::string f = file;
  size_t pos = f.find("{}");
  if (pos != std::string::npos)
  {
    f.replace(pos, 2, cosmicRun);
  }
  return f;
}

// cosmics runs of the files that match a file name with {}: the run numbers that replace {} in the files of its folder, sorted
inline std::vector<int> hcalCosmicRunsOnDisk(const std::string &file)
{
  std::vector<int> runs;
  size_t pos = file.find("{}");
  if (pos == std::string::npos)
  {
    return runs;
  }
  std::filesystem::path pattern(file);
  std::string dir = pattern.parent_path().string();
  std::string name = pattern.filename().string();
  size_t npos = name.find("{}");
  std::string prefix = name.substr(0, npos);
  std::string suffix = name.substr(npos + 2);
  std::error_code ec;
  for (const auto &entry : std::filesystem::directory_iterator(dir, ec))
  {
    std::string f = entry.path().filename().string();
    if (f.size() <= prefix.size() + suffix.size()) continue;
    if (f.compare(0, prefix.size(), prefix) != 0) continue;
    if (f.compare(f.size() - suffix.size(), suffix.size(), suffix) != 0) continue;
    std::string run = f.substr(prefix.size(), f.size() - prefix.size() - suffix.size());
    if (run.empty() || run.find_first_not_of("0123456789") != std::string::npos) continue;
    runs.push_back(std::stoi(run));
  }
  if (ec)
  {
    std::cout << "cannot read the cosmics folder " << dir << ": " << ec.message() << std::endl;
  }
  std::sort(runs.begin(), runs.end());
  return runs;
}


// the single file of the dataset, or the file of the last cosmics run before the run.
// The cosmics runs are the run numbers in the names of the files in the folder. A beam run before the first
// cosmics run gets the first cosmics run.
inline std::string findCosmicCalib(const HCalCalibDataset &ds, int runnumber, int isIHcal = 0)
{
  const std::string &file = (isIHcal == 1) ? ds.ih_cosmicFile : ds.oh_cosmicFile;
  if (file.find("{}") == std::string::npos)
  {
    return file;
  }
  std::string det = (isIHcal == 1) ? "IHCal" : "OHCal";
  std::vector<int> cosmicRuns = hcalCosmicRunsOnDisk(file);
  if (cosmicRuns.empty())
  {
    std::cout << "no cosmics files " << file << " ({} = cosmics run number)" << std::endl;
    return "";
  }
  // the cosmics runs are also noted in the dataset, with their temperatures
  std::vector<int> noted = (isIHcal == 1) ? ds.ih_cosmicRuns : ds.oh_cosmicRuns;
  std::sort(noted.begin(), noted.end());
  if (noted != cosmicRuns)
  {
    std::cout << "WARNING: the " << det << " cosmics runs noted for " << ds.name << " in HcalCalibDatasetinputs.h are not the ones of the files in the folder (";
    for (int run : cosmicRuns) std::cout << run << " ";
    std::cout << "). The files in the folder are used, update the note and the temperatures." << std::endl;
  }
  int cosmicRun = cosmicRuns.front();
  for (int run : cosmicRuns)
  {
    if (run <= runnumber) cosmicRun = run;
  }
  std::cout << det << " cosmics run for run " << runnumber << ": " << cosmicRun << std::endl;
  return hcalCosmicFile(file, std::to_string(cosmicRun));
}

inline std::string findCosmicCalib_ihcal(const HCalCalibDataset &ds, int runnumber)
{
  return findCosmicCalib(ds, runnumber, 1);
}

// Last run of the cosmics period that starts with cosmicRun (oh_/ih_cosmicLastRuns), cosmicRun itself if none is noted.
// The cosmics temperature is the average over the cosmics runs from cosmicRun to this run.
inline int hcalCosmicLastRun(const HCalCalibDataset &ds, int isIHcal, int cosmicRun)
{
  const std::vector<int> &runs = (isIHcal == 1) ? ds.ih_cosmicRuns : ds.oh_cosmicRuns;
  const std::vector<int> &lastRuns = (isIHcal == 1) ? ds.ih_cosmicLastRuns : ds.oh_cosmicLastRuns;
  for (size_t i = 0; i < runs.size() && i < lastRuns.size(); i++)
  {
    if (runs[i] == cosmicRun && lastRuns[i] > cosmicRun) return lastRuns[i];
  }
  return cosmicRun;
}

// Temperature of a cosmics run noted in the dataset, 0 if it is not noted (it is then read from the DB).
// A dataset with one cosmics base (file name without {}) has one temperature, used for all runs.
inline float hcalCosmicTemp(const HCalCalibDataset &ds, int isIHcal, int cosmicRun)
{
  const std::string &file = (isIHcal == 1) ? ds.ih_cosmicFile : ds.oh_cosmicFile;
  const std::vector<int> &runs = (isIHcal == 1) ? ds.ih_cosmicRuns : ds.oh_cosmicRuns;
  const std::vector<float> &temps = (isIHcal == 1) ? ds.ih_cosmicTemps : ds.oh_cosmicTemps;
  if (runs.size() != temps.size())
  {
    std::cout << "WARNING: " << ds.name << " has " << runs.size() << " cosmics runs and " << temps.size()
              << " cosmics temperatures noted in HcalCalibDatasetinputs.h, they have to be in the same order" << std::endl;
  }
  if (file.find("{}") == std::string::npos)
  {
    return temps.empty() ? 0 : temps.front();
  }
  for (size_t i = 0; i < runs.size() && i < temps.size(); i++)
  {
    if (runs[i] == cosmicRun) return temps[i];
  }
  return 0;
}

// dataset by name, nullptr if there is no dataset with this name
inline const HCalCalibDataset *findHCalCalibDataset(const std::string &name)
{
  for (const HCalCalibDataset &ds : hcalCalibDatasets())
  {
    if (ds.name == name) return &ds;
  }
  return nullptr;
}

// names of the datasets, e.g. "run2pp, run2auau, run3oo"
inline std::string hcalCalibDatasetNames()
{
  std::string names;
  for (const HCalCalibDataset &ds : hcalCalibDatasets())
  {
    names += (names.empty() ? "" : ", ") + ds.name;
  }
  return names;
}

inline const HCalCalibDataset &getHCalCalibDataset(int runnumber)
{
  const std::vector<HCalCalibDataset> &datasets = hcalCalibDatasets();
  for (const HCalCalibDataset &ds : datasets)
  {
    if (runnumber <= ds.lastRun)
    {
      return ds;
    }
  }
  std::cout << "WARNING: run " << runnumber << " is after the last run of " << datasets.back().name
            << ", using the " << datasets.back().name << " settings. Add the new dataset to HcalCalibDatasetinputs.h" << std::endl;
  return datasets.back();
}

#endif
