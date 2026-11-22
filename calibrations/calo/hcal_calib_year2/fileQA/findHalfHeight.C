#include "../HcalCalibDatasetinputs.h"  // run list and last run of a dataset

#include <TAxis.h>
#include <TBox.h>
#include <TCanvas.h>
#include <TDirectory.h>
#include <TFile.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TH1.h>
#include <TLatex.h>
#include <TLine.h>
#include <TMultiGraph.h>
#include <TPad.h>
#include <TStyle.h>
#include <TSystem.h>

#include <algorithm>
#include <cmath>
#include <format>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

// Half height towers: towers whose hits per event jump by a factor 2 or 0.5 for a range of runs
// (poor signal cable connection, see the run2pp calibration note). The cosmics calibration absorbs
// the state the tower had during the cosmics run; the runs in the other state need the per-tower
// correction of genFinalCalib (genCdbCorr_HH), listed in a CSV file.
//
// Reads the hits per event of each tower vs run number (hpe_tower_vs_runnumber of ana_hits.C) and,
// for each tower:
//  - takes the median of the live towers of its eta ring in the same run as the reference: the ratio
//    tower / ring median is the same in every run for a tower that does not change, whatever the temperature or
//    the rate of the run. The normal level of the tower is the median of this ratio over the runs in which it is
//    within kNormalLo to kNormalHi (1 for most towers; chimney and support towers, or towers with a wrong
//    towerslope, sit at another level in all runs). Half and double height states are outside this band, so the
//    normal level does not depend on how many runs the tower spent in the bad state, unlike the tower's own
//    median, which is the wrong reference for a tower at half height in most of the runs. A tower with no run
//    in the band takes the median of all its runs and is listed in the summary.
//  - flags a run as "Half" or "Double" if tower / ring median / normal level is within kBandLo to kBandHi times
//    the hit ratio expected for a tower with gain 0.5 or 2 (h_hitRatio_gainHalf, h_hitRatio_gainDouble of
//    ana_hits.C: the ring spectrum integrated over the energy window moved by the gain factor, per eta ring;
//    e.g. OHCal 1-5 GeV: 0.17 and 3.5, so the windows are 0.10-0.34 and 2.1-7.0). An anaOut file without them
//    (old ana_hits.C) gets the fixed windows kHalfLo..kHalfHi and kDoubleLo..kDoubleHi, made for a 0.25-5 GeV
//    window. A run below kStepLo or above kStepHi x the normal level that is neither is a "Step": a gain change
//    that is not a factor 2 (e.g. 1.4). A run within kMinSigma x its error of the normal level is never flagged:
//    a tower with few hits per event scatters outside the windows by statistics alone. Step blocks are listed in
//    the summary with the implied gain factor, never written to the CSV file,
//  - groups flagged runs that follow each other in the graph with the same reason into blocks; a single run
//    of another state inside a block (e.g. a run with an odd rate in the whole detector) is bridged,
//  - writes the blocks with at least minRuns runs to the CSV file of genFinalCalib
//    (ieta,iphi,factor,run_lo,run_hi: factor 2 for Half, 0.5 for Double; run_lo and run_hi are the runs
//    of the run list before and after the block, 0 and last run + 1 at the ends, because genFinalCalib
//    applies an entry for run_lo < run < run_hi). Shorter blocks are only noted in the summary.
//  - counts per run how many towers are flagged: a run with many flagged towers has an odd rate in the whole
//    detector (not a cable), blocks made of such runs only are suspect. Printed for runs with at least
//    kMinTowersPerRunNote flagged towers.
//
// How to run (from fileQA):
//
//   source /opt/sphenix/core/bin/sphenix_setup.sh -n new
//   // 1. hits per event of each tower from the Step 2 spectra (out_run_cosmics_tsc_temp, see ana_hits.C)
//   root -l -b -q 'ana_hits.C("run3oo", 0, "../condor_histmaker/out_run_cosmics_tsc_temp")'
//   root -l -b -q 'ana_hits.C("run3oo", 1, "../condor_histmaker/out_run_cosmics_tsc_temp")'
//   // 2. half height towers -> output/halfHeightList_<det>_run3oo.csv, the <half height list> of genFinalCalib.C
//   root -l -b -q 'findHalfHeight.C("run3oo", 0)'   // OHCal: output/anaOut_ohcal_cosmics_tsc_temp.root
//   root -l -b -q 'findHalfHeight.C("run3oo", 1)'   // IHCal
//   // all blocks, also single runs, in the CSV and the figures:
//   root -l -b -q 'findHalfHeight.C("run3oo", 0, "", "", 1)'
//   // other anaOut file, blocks of at least 2 runs:
//   root -l -b -q 'findHalfHeight.C("run3oo", 0, "output/anaOut_ohcal_cosmics_tsc.root", "", 2)'
//   // before / after figures: 4th argument = anaOut file made with the half height corrected calibration
//   root -l -b -q 'findHalfHeight.C("run3oo", 0, "", "output/anaOut_ohcal_cosmics_tsc_temp_hh.root")'
//
// dataset:     run2pp, run2auau, run3oo, ... (run list and last run in ../HcalCalibDatasetinputs.h)
//              or a beam run list, one run per line
// isIHcal:     0 OHCal, 1 IHCal
// anaOut:      anaOut file of ana_hits.C, "" for output/anaOut_<det>_cosmics_tsc_temp.root
// anaOutAfter: anaOut file made with the corrected calibration, "" for none. Only for the figures:
//              the flagged towers are drawn before and after the correction
// minRuns:     a block of flagged runs is written to the CSV file if it has at least this many runs
//
// output: output/halfHeightList_<det>_<dataset>.csv     CSV file for genFinalCalib
//         output/halfHeight_<det>_<dataset>_summary.txt  all blocks, also the ones shorter than minRuns
//         output/halfHeight_<det>_<dataset>.root         graphs of all towers with the flagged runs colored
//                                                        (green normal, blue Half, red Double, orange Step):
//                                                        hits per event with the ring median per run (g_hpe_...)
//                                                        and tower / ring median with the normal level (g_ratio_...)
//         output/figures_halfHeight_<det>_<dataset>/     png of each tower with a block of at least minRuns:
//                                                        hits per event on top, tower / ring median with the
//                                                        normal level and the Half and Double windows below

namespace
{
  // a run is Half (Double) if tower / ring median is within this band around the expected ratio of a tower
  // with gain 0.5 (2); wide because the spectrum slope of a tower differs from its ring (chimney, edges)
  const double kBandLo = 0.6;
  const double kBandHi = 2.0;
  // fixed windows for an anaOut file without the expected ratios (made with the pp24 window 0.25-5 GeV)
  const double kDoubleLo = 1.65;
  const double kDoubleHi = 2.6;
  const double kHalfLo = 0.28;
  const double kHalfHi = 0.7;
  // a run outside kStepLo..kStepHi x the ring that is neither Half nor Double is a Step (other gain change)
  const double kStepLo = 0.7;
  const double kStepHi = 1.5;
  // a run is flagged only if its hits/event are more than kMinSigma x its error away from the ring level
  // (a tower with few hits per event scatters outside the windows by statistics alone)
  const double kMinSigma = 2.0;
  // the ring median of a run is used only if at least this many towers of the ring are live in the run
  const int kMinRingTowers = 40;
  // the normal level of a tower is the median of tower / ring median over the runs within this band
  // (between the Half and the Double windows, so a half or double height state cannot set the level)
  const double kNormalLo = 0.4;
  const double kNormalHi = 2.0;
  // note the runs with at least this many flagged towers
  const int kMinTowersPerRunNote = 10;

  enum class RunState
  {
    kNormal,
    kHalf,
    kDouble,
    kStepDown,  // other gain change, hits below kStepLo x the normal level
    kStepUp     // other gain change, hits above kStepHi x the normal level
  };

  bool isStep(RunState s)
  {
    return s == RunState::kStepDown || s == RunState::kStepUp;
  }

  // windows of tower / ring median of one eta ring
  struct RatioWindows
  {
    double expHalf{0};  // expected ratio of a tower with gain 0.5
    double expDouble{0};
    double halfLo{0};
    double halfHi{0};
    double doubleLo{0};
    double doubleHi{0};
  };

  // a block of flagged runs of a tower
  struct HalfHeightBlock
  {
    int ieta{0};
    int iphi{0};
    RunState state{RunState::kNormal};
    int firstRun{0};
    int lastRun{0};
    int nRuns{0};
    int nBridged{0};        // runs of another state inside the block
    double meanRatio{0};    // mean of tower / ring median / normal level over the runs of the block in its state
    double normalLevel{1};  // normal level of the tower (tower / ring median)
  };

  // classification of the runs of one tower
  struct TowerScan
  {
    std::vector<double> run;
    std::vector<double> hpe;
    std::vector<double> err;
    std::vector<double> ref;       // ring median of the run
    std::vector<double> ratio;     // hpe / ref
    std::vector<double> ratioErr;  // err / ref
    std::vector<RunState> state;
    double normalLevel{1};       // median of ratio over the runs within kNormalLo..kNormalHi
    bool normalFallback{false};  // no run in the band: normalLevel is the median of all runs
    int nFlagged{0};
    std::vector<HalfHeightBlock> blocks;
  };

  double median(std::vector<double> v)
  {
    if (v.empty())
    {
      return 0.0;
    }
    std::sort(v.begin(), v.end());
    const size_t n = v.size();
    if (n % 2 == 1)
    {
      return v[n / 2];
    }
    return 0.5 * (v[n / 2 - 1] + v[n / 2]);
  }

  // reference level of an eta ring: median over the live towers of the ring, per run
  using RingReference = std::map<int, double>;

  RingReference ringReference(TDirectory *dir, int ieta)
  {
    std::map<int, std::vector<double>> perRun;
    for (int ip = 0; ip < 64; ++ip)
    {
      const std::string gName = std::format("g_hpe_eta{:02d}_phi{:02d}", ieta, ip);
      const TGraph *g = dynamic_cast<TGraph *>(dir->Get(gName.c_str()));
      if (!g)
      {
        continue;
      }
      for (int i = 0; i < g->GetN(); ++i)
      {
        if (g->GetY()[i] > 0)
        {
          perRun[static_cast<int>(g->GetX()[i])].push_back(g->GetY()[i]);
        }
      }
    }
    RingReference ref;
    for (auto &[run, values] : perRun)
    {
      if (static_cast<int>(values.size()) >= kMinRingTowers)
      {
        ref[run] = median(values);
      }
    }
    return ref;
  }

  const char *stateName(RunState s)
  {
    if (s == RunState::kHalf)
    {
      return "Half";
    }
    if (s == RunState::kDouble)
    {
      return "Double";
    }
    if (isStep(s))
    {
      return "Step";
    }
    return "Normal";
  }

  double calibFactor(RunState s)
  {
    if (s == RunState::kHalf)
    {
      return 2.0;
    }
    if (s == RunState::kDouble)
    {
      return 0.5;
    }
    return 1.0;
  }

  RunState classify(double ratio, double nSigma, const RatioWindows &w)
  {
    if (nSigma < kMinSigma)
    {
      return RunState::kNormal;
    }
    if (ratio >= w.doubleLo && ratio <= w.doubleHi)
    {
      return RunState::kDouble;
    }
    if (ratio >= w.halfLo && ratio <= w.halfHi)
    {
      return RunState::kHalf;
    }
    if (ratio < kStepLo)
    {
      return RunState::kStepDown;
    }
    if (ratio > kStepHi)
    {
      return RunState::kStepUp;
    }
    return RunState::kNormal;
  }

  // gain factor that gives the hit ratio, from the expected ratios of gain 0.5 and 2 (power law spectrum)
  double impliedGain(double ratio, const RatioWindows &w)
  {
    if (ratio <= 0 || w.expHalf <= 0 || w.expDouble <= 0)
    {
      return 0.0;
    }
    if (ratio > 1)
    {
      return std::pow(2.0, std::log(ratio) / std::log(w.expDouble));
    }
    return std::pow(0.5, std::log(ratio) / std::log(w.expHalf));
  }

  // classify the runs of a tower and group the flagged ones into blocks
  TowerScan scanTower(const TGraph *g, int ieta, int iphi, const RatioWindows &w, const RingReference &ringRef)
  {
    TowerScan scan;
    for (int i = 0; i < g->GetN(); ++i)
    {
      const auto ref = ringRef.find(static_cast<int>(g->GetX()[i]));
      if (g->GetY()[i] <= 0 || ref == ringRef.end() || ref->second <= 0)
      {
        continue;  // tower not live or no ring reference in this run
      }
      scan.run.push_back(g->GetX()[i]);
      scan.hpe.push_back(g->GetY()[i]);
      scan.err.push_back(g->GetErrorY(i) > 0 ? g->GetErrorY(i) : 0.0);  // 0 for a TGraph without errors
      scan.ref.push_back(ref->second);
      scan.ratio.push_back(g->GetY()[i] / ref->second);
      scan.ratioErr.push_back(scan.err.back() / ref->second);
    }
    if (scan.ratio.empty())
    {
      return scan;
    }
    // normal level of the tower
    std::vector<double> inBand;
    for (const double r : scan.ratio)
    {
      if (r >= kNormalLo && r <= kNormalHi)
      {
        inBand.push_back(r);
      }
    }
    scan.normalFallback = inBand.empty();
    scan.normalLevel = scan.normalFallback ? median(scan.ratio) : median(inBand);
    if (scan.normalLevel <= 0)
    {
      return scan;
    }
    for (size_t k = 0; k < scan.hpe.size(); ++k)
    {
      // significance of the deviation from the normal level; a graph without errors is always significant
      const double expected = scan.ref[k] * scan.normalLevel;
      const double nSigma = (scan.err[k] > 0) ? std::abs(scan.hpe[k] - expected) / scan.err[k] : 1e9;
      const RunState s = classify(scan.ratio[k] / scan.normalLevel, nSigma, w);
      scan.state.push_back(s);
      if (s != RunState::kNormal)
      {
        scan.nFlagged++;
      }
    }
    // blocks: flagged runs that follow each other in the graph with the same state
    size_t i = 0;
    while (i < scan.state.size())
    {
      if (scan.state[i] == RunState::kNormal)
      {
        ++i;
        continue;
      }
      HalfHeightBlock block;
      block.ieta = ieta;
      block.iphi = iphi;
      block.state = scan.state[i];
      block.normalLevel = scan.normalLevel;
      block.firstRun = static_cast<int>(scan.run[i]);
      double ratioSum = 0.0;
      size_t j = i;
      while (j < scan.state.size())
      {
        if (scan.state[j] == block.state)
        {
          ratioSum += scan.ratio[j] / scan.normalLevel;
          ++j;
          continue;
        }
        // bridge a single run of another state if the block goes on after it
        if (j + 1 < scan.state.size() && scan.state[j + 1] == block.state)
        {
          block.nBridged++;
          ++j;
          continue;
        }
        break;
      }
      block.lastRun = static_cast<int>(scan.run[j - 1]);
      block.nRuns = static_cast<int>(j - i);
      block.meanRatio = ratioSum / (block.nRuns - block.nBridged);
      scan.blocks.push_back(block);
      i = j;
    }
    return scan;
  }

  // graphs of the runs of a tower colored by state: the hits per event with the ring median per run,
  // or (ratio = true) tower / ring median with a line at 1
  TMultiGraph *coloredGraph(const TowerScan &scan, const std::string &name, const std::string &title, bool ratio)
  {
    TMultiGraph *mg = new TMultiGraph(name.c_str(), title.c_str());
    const std::vector<std::pair<RunState, Color_t>> styles = {{RunState::kNormal, kGreen + 2},
                                                              {RunState::kHalf, kBlue},
                                                              {RunState::kDouble, kRed},
                                                              {RunState::kStepDown, kOrange + 7},
                                                              {RunState::kStepUp, kOrange + 7}};
    for (const auto &[state, color] : styles)
    {
      std::vector<double> x;
      std::vector<double> y;
      std::vector<double> ey;
      for (size_t i = 0; i < scan.state.size(); ++i)
      {
        if (scan.state[i] == state)
        {
          x.push_back(scan.run[i]);
          y.push_back(ratio ? scan.ratio[i] : scan.hpe[i]);
          ey.push_back(ratio ? scan.ratioErr[i] : scan.err[i]);
        }
      }
      if (x.empty())
      {
        continue;
      }
      TGraphErrors *g = new TGraphErrors(x.size(), x.data(), y.data(), nullptr, ey.data());
      g->SetName(std::format("{}_{}", name, stateName(state)).c_str());
      g->SetMarkerColor(color);
      g->SetMarkerStyle(20);
      g->SetMarkerSize(0.8);
      mg->Add(g, "P");
    }
    if (!scan.run.empty())
    {
      // the reference: ring median per run on the hits, the normal level on the ratio
      TGraph *line = new TGraph(ratio ? 2 : static_cast<int>(scan.run.size()));
      line->SetName(std::format("{}_reference", name).c_str());
      if (ratio)
      {
        line->SetPoint(0, scan.run.front(), scan.normalLevel);
        line->SetPoint(1, scan.run.back(), scan.normalLevel);
        line->SetLineStyle(2);
      }
      else
      {
        for (size_t i = 0; i < scan.run.size(); ++i)
        {
          line->SetPoint(static_cast<int>(i), scan.run[i], scan.ref[i]);
        }
      }
      line->SetLineColor(kBlue);
      line->SetLineWidth(2);
      mg->Add(line, "L");
    }
    return mg;
  }

  // read the run list: one run per line, sorted
  std::vector<int> readRunList(const std::string &runListFile)
  {
    std::vector<int> runs;
    std::ifstream in(runListFile);
    std::string line;
    while (std::getline(in, line))
    {
      if (line.empty() || line[0] == '#')
      {
        continue;
      }
      try
      {
        runs.push_back(std::stoi(line));
      }
      catch (...)
      {
        std::cerr << "WARN: skip bad line: " << line << "\n";
      }
    }
    std::sort(runs.begin(), runs.end());
    runs.erase(std::unique(runs.begin(), runs.end()), runs.end());
    return runs;
  }

  // run of the run list before run (0 if none)
  int runBefore(const std::vector<int> &runList, int run)
  {
    auto it = std::lower_bound(runList.begin(), runList.end(), run);
    if (it == runList.begin())
    {
      return 0;
    }
    return *std::prev(it);
  }

  // run of the run list after run (lastRun + 1 if none)
  int runAfter(const std::vector<int> &runList, int run, int lastRun)
  {
    auto it = std::upper_bound(runList.begin(), runList.end(), run);
    if (it == runList.end())
    {
      return std::max(lastRun, run) + 1;
    }
    return *it;
  }

  // hits per event of a tower with the ring median per run
  void drawPanel(TMultiGraph *mg, const std::string &title, const std::string &label)
  {
    mg->SetTitle(std::format("{};Run Number;Hits/Event", title).c_str());
    gPad->SetLeftMargin(0.14);
    mg->SetMinimum(0);
    mg->Draw("A");
    mg->SetMaximum(1.35 * mg->GetYaxis()->GetXmax());  // room for the label above the points
    mg->Draw("A");
    mg->GetXaxis()->SetNdivisions(505);
    mg->GetYaxis()->SetTitleOffset(1.6);
    TLatex tex;
    tex.SetNDC();
    tex.SetTextSize(0.04);
    tex.DrawLatex(0.55, 0.85, "#bf{#it{sPHENIX}} Internal");
    tex.DrawLatex(0.55, 0.79, label.c_str());
  }

  // tower / ring median of a tower with the normal level, the Half and Double windows and the Step limits
  void drawRatioPanel(TMultiGraph *mg, const RatioWindows &w, double normalLevel, const std::string &title)
  {
    mg->SetTitle(std::format("{};Run Number;tower / ring median", title).c_str());
    gPad->SetLeftMargin(0.14);
    gPad->SetLogy();
    mg->SetMinimum(0.5 * normalLevel * w.halfLo);
    mg->SetMaximum(2.0 * normalLevel * w.doubleHi);
    mg->Draw("A");
    mg->GetXaxis()->SetNdivisions(505);
    mg->GetYaxis()->SetTitleOffset(1.6);
    const double xLo = mg->GetXaxis()->GetXmin();
    const double xHi = mg->GetXaxis()->GetXmax();
    TBox *half = new TBox(xLo, normalLevel * w.halfLo, xHi, normalLevel * w.halfHi);
    half->SetFillColorAlpha(kBlue, 0.15);
    half->Draw();
    TBox *dbl = new TBox(xLo, normalLevel * w.doubleLo, xHi, normalLevel * w.doubleHi);
    dbl->SetFillColorAlpha(kRed, 0.15);
    dbl->Draw();
    for (const double y : {kStepLo, kStepHi})
    {
      TLine *l = new TLine(xLo, normalLevel * y, xHi, normalLevel * y);
      l->SetLineColor(kOrange + 7);
      l->SetLineStyle(3);
      l->Draw();
    }
    mg->Draw("P");  // points above the bands
    TLatex tex;
    tex.SetNDC();
    tex.SetTextSize(0.035);
    tex.DrawLatex(0.16, 0.85, std::format("normal level {:.2f} (dashed)", normalLevel).c_str());
    tex.DrawLatex(0.16, 0.80, std::format("Half window {:.2f} - {:.2f} x normal", w.halfLo, w.halfHi).c_str());
    tex.DrawLatex(0.16, 0.75, std::format("Double window {:.1f} - {:.1f} x normal", w.doubleLo, w.doubleHi).c_str());
    tex.DrawLatex(0.16, 0.70, std::format("Step below {} or above {} x normal", kStepLo, kStepHi).c_str());
  }
}  // namespace

// ------------------------------------------------------------

void findHalfHeight(const std::string &dataset = "run3oo", int isIHcal = 0,
                    std::string anaOut = "", const std::string &anaOutAfter = "",
                    int minRuns = 2)
{
  const std::string det = (isIHcal == 1) ? "ihcal" : "ohcal";
  const std::string detLabel = (isIHcal == 1) ? "IHCal" : "OHCal";
  if (anaOut.empty())
  {
    anaOut = std::format("output/anaOut_{}_cosmics_tsc_temp.root", det);
  }

  // run list and last run of the dataset (or a run list file)
  std::string runListFile = dataset;
  std::string dsName = gSystem->BaseName(dataset.c_str());
  int lastRunDataset = 0;
  if (const HCalCalibDataset *ds = findHCalCalibDataset(dataset))
  {
    runListFile = ds->runList;
    dsName = ds->name;
    lastRunDataset = ds->lastRun;
  }
  const std::vector<int> runList = readRunList(runListFile);
  if (runList.empty())
  {
    std::cerr << "ERROR: no runs in the run list " << runListFile << std::endl;
    return;
  }
  if (lastRunDataset == 0)
  {
    lastRunDataset = runList.back();
  }

  const std::string outCsv = std::format("output/halfHeightList_{}_{}.csv", det, dsName);
  const std::string outSummary = std::format("output/halfHeight_{}_{}_summary.txt", det, dsName);
  const std::string outRoot = std::format("output/halfHeight_{}_{}.root", det, dsName);
  const std::string figDir = std::format("output/figures_halfHeight_{}_{}", det, dsName);

  std::cout << "[findHalfHeight] " << detLabel << " " << dsName << std::endl;
  std::cout << "  hits per event from " << anaOut << std::endl;
  std::cout << "  run list            " << runListFile << " (" << runList.size() << " runs, last run of the dataset "
            << lastRunDataset << ")" << std::endl;
  std::cout << "  min runs per block  " << minRuns << std::endl;
  std::cout << "  CSV file            " << outCsv << std::endl;

  std::unique_ptr<TFile> fin(TFile::Open(anaOut.c_str(), "READ"));
  if (!fin || fin->IsZombie())
  {
    std::cerr << "ERROR: cannot open " << anaOut << std::endl;
    return;
  }
  TDirectory *dirIn = fin->GetDirectory("hpe_tower_vs_runnumber");
  if (!dirIn)
  {
    std::cerr << "ERROR: no directory hpe_tower_vs_runnumber in " << anaOut
              << " (run ana_hits.C with the current version)" << std::endl;
    return;
  }
  // windows per eta ring from the expected hit ratios of a tower with gain 0.5 and 2
  std::vector<RatioWindows> windows(24);
  const TH1 *hRatioHalf = dynamic_cast<TH1 *>(fin->Get("h_hitRatio_gainHalf"));
  const TH1 *hRatioDouble = dynamic_cast<TH1 *>(fin->Get("h_hitRatio_gainDouble"));
  const bool derivedWindows = hRatioHalf && hRatioDouble;
  for (int ie = 0; ie < 24; ++ie)
  {
    RatioWindows &w = windows[ie];
    if (derivedWindows && hRatioHalf->GetBinContent(ie + 1) > 0 && hRatioDouble->GetBinContent(ie + 1) > 0)
    {
      w.expHalf = hRatioHalf->GetBinContent(ie + 1);
      w.expDouble = hRatioDouble->GetBinContent(ie + 1);
      w.halfLo = kBandLo * w.expHalf;
      w.halfHi = kBandHi * w.expHalf;
      w.doubleLo = kBandLo * w.expDouble;
      w.doubleHi = kBandHi * w.expDouble;
    }
    else
    {
      w.halfLo = kHalfLo;
      w.halfHi = kHalfHi;
      w.doubleLo = kDoubleLo;
      w.doubleHi = kDoubleHi;
    }
  }
  if (derivedWindows)
  {
    std::cout << "  windows of tower / ring median from the expected ratios of a tower with gain 0.5 and 2 in "
              << anaOut << " (" << hRatioHalf->GetTitle() << "), " << kBandLo << " - " << kBandHi
              << " x expected, per eta ring:" << std::endl;
    for (int ie = 0; ie < 24; ++ie)
    {
      const RatioWindows &w = windows[ie];
      std::cout << std::format("    eta {:2d}: Half {:.3f} ({:.3f} - {:.3f})  Double {:.2f} ({:.2f} - {:.2f})", ie,
                               w.expHalf, w.halfLo, w.halfHi, w.expDouble, w.doubleLo, w.doubleHi)
                << std::endl;
    }
  }
  else
  {
    std::cout << "  WARNING: no h_hitRatio_gainHalf / h_hitRatio_gainDouble in " << anaOut
              << " (old ana_hits.C): fixed windows Half " << kHalfLo << " - " << kHalfHi << ", Double " << kDoubleLo
              << " - " << kDoubleHi << " x the ring, made for a 0.25 - 5 GeV hit window" << std::endl;
  }
  std::cout << "  normal level of a tower: median of tower / ring median over the runs within " << kNormalLo << " - "
            << kNormalHi << " x the ring (all runs if none)" << std::endl;
  std::cout << "  Step: below " << kStepLo << " or above " << kStepHi
            << " x the normal level and neither Half nor Double"
            << "; a run within " << kMinSigma << " sigma of the normal level is never flagged"
            << std::endl;

  std::unique_ptr<TFile> finAfter;
  TDirectory *dirAfter = nullptr;
  if (!anaOutAfter.empty())
  {
    finAfter.reset(TFile::Open(anaOutAfter.c_str(), "READ"));
    if (!finAfter || finAfter->IsZombie())
    {
      std::cerr << "ERROR: cannot open " << anaOutAfter << std::endl;
      return;
    }
    dirAfter = finAfter->GetDirectory("hpe_tower_vs_runnumber");
    if (!dirAfter)
    {
      std::cerr << "ERROR: no directory hpe_tower_vs_runnumber in " << anaOutAfter << std::endl;
      return;
    }
  }

  gSystem->mkdir("output", true);
  gSystem->mkdir(figDir.c_str(), true);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(1);

  TFile fout(outRoot.c_str(), "RECREATE");
  TDirectory *dirOut = fout.mkdir("hpe_tower_vs_runnumber");

  std::vector<HalfHeightBlock> written;
  std::vector<HalfHeightBlock> noted;
  std::vector<HalfHeightBlock> steps;  // Step blocks of at least minRuns runs
  int nShortSteps = 0;
  std::map<int, int> flaggedTowersPerRun;   // run -> number of towers flagged in it
  std::vector<std::string> fallbackTowers;  // towers with no run within kNormalLo..kNormalHi x the ring
  int nTowers = 0;

  for (int ie = 0; ie < 24; ++ie)
  {
    const RingReference ringRef = ringReference(dirIn, ie);
    const RingReference ringRefAfter = dirAfter ? ringReference(dirAfter, ie) : RingReference{};
    for (int ip = 0; ip < 64; ++ip)
    {
      const std::string gName = std::format("g_hpe_eta{:02d}_phi{:02d}", ie, ip);
      const std::string rName = std::format("g_ratio_eta{:02d}_phi{:02d}", ie, ip);
      const TGraph *g = dynamic_cast<TGraph *>(dirIn->Get(gName.c_str()));
      if (!g)
      {
        continue;
      }
      nTowers++;
      const TowerScan scan = scanTower(g, ie, ip, windows[ie], ringRef);
      if (scan.run.empty())
      {
        continue;
      }
      if (scan.normalFallback)
      {
        fallbackTowers.push_back(std::format("({},{}) {:.2f}", ie, ip, scan.normalLevel));
      }
      for (size_t i = 0; i < scan.state.size(); ++i)
      {
        if (scan.state[i] != RunState::kNormal)
        {
          flaggedTowersPerRun[static_cast<int>(scan.run[i])]++;
        }
      }
      bool drawTower = false;
      for (const HalfHeightBlock &block : scan.blocks)
      {
        if (isStep(block.state))
        {
          if (block.nRuns >= minRuns)
          {
            steps.push_back(block);
            drawTower = true;
          }
          else
          {
            nShortSteps++;
          }
        }
        else if (block.nRuns >= minRuns)
        {
          written.push_back(block);
          drawTower = true;
        }
        else
        {
          noted.push_back(block);
        }
      }

      // colored graphs of every tower: hits per event and tower / ring median
      dirOut->cd();
      const std::string gTitle = std::format("{} hits/event, tower eta={} phi={};Run Number;Hits/Event", det, ie, ip);
      TMultiGraph *mg = coloredGraph(scan, gName, gTitle, false);
      mg->Write();
      const std::string rTitle =
          std::format("{} tower / ring median, tower eta={} phi={};Run Number;tower / ring median", det, ie, ip);
      TMultiGraph *mgRatio = coloredGraph(scan, rName, rTitle, true);
      mgRatio->Write();

      // figure of the towers with a block to correct: hits per event on top, ratio below,
      // before (and after) the correction side by side
      if (drawTower)
      {
        const TGraph *gAfter = dirAfter ? dynamic_cast<TGraph *>(dirAfter->Get(gName.c_str())) : nullptr;
        const int nCols = gAfter ? 2 : 1;
        TCanvas c("c", "", 700 * nCols, 900);
        c.Divide(nCols, 2);
        c.cd(1);
        drawPanel(mg, std::format("Tower (#eta, #phi) = ({}, {}) before half height correction", ie, ip),
                  std::format("{} {}", detLabel, dsName));
        c.cd(nCols + 1);
        drawRatioPanel(mgRatio, windows[ie], scan.normalLevel,
                       std::format("Tower (#eta, #phi) = ({}, {}) before", ie, ip));
        if (gAfter)
        {
          const TowerScan scanAfter = scanTower(gAfter, ie, ip, windows[ie], ringRefAfter);
          TMultiGraph *mgAfter = coloredGraph(scanAfter, std::format("{}_after", gName), "", false);
          TMultiGraph *mgRatioAfter = coloredGraph(scanAfter, std::format("{}_after", rName), "", true);
          c.cd(2);
          drawPanel(mgAfter, std::format("Tower (#eta, #phi) = ({}, {}) after half height correction", ie, ip),
                    std::format("{} {}", detLabel, dsName));
          c.cd(4);
          drawRatioPanel(mgRatioAfter, windows[ie], scanAfter.normalLevel,
                         std::format("Tower (#eta, #phi) = ({}, {}) after", ie, ip));
        }
        c.SaveAs(std::format("{}/{}_eta{:02d}_phi{:02d}.png", figDir, det, ie, ip).c_str());
      }
    }
  }
  fout.Close();

  // sort the blocks by tower and run
  auto byTowerAndRun = [](const HalfHeightBlock &a, const HalfHeightBlock &b)
  {
    if (a.ieta != b.ieta)
    {
      return a.ieta < b.ieta;
    }
    if (a.iphi != b.iphi)
    {
      return a.iphi < b.iphi;
    }
    return a.firstRun < b.firstRun;
  };
  std::sort(written.begin(), written.end(), byTowerAndRun);
  std::sort(noted.begin(), noted.end(), byTowerAndRun);
  std::sort(steps.begin(), steps.end(), byTowerAndRun);
  // runs with many flagged towers, most first
  std::vector<std::pair<int, int>> oddRuns;
  for (const auto &[run, n] : flaggedTowersPerRun)
  {
    if (n >= kMinTowersPerRunNote)
    {
      oddRuns.emplace_back(n, run);
    }
  }
  std::sort(oddRuns.begin(), oddRuns.end(), std::greater<>());

  // CSV file of genFinalCalib: ieta,iphi,factor,run_lo,run_hi with run_lo < run < run_hi
  std::ofstream csv(outCsv);
  for (const HalfHeightBlock &b : written)
  {
    csv << b.ieta << "," << b.iphi << "," << calibFactor(b.state) << ","
        << runBefore(runList, b.firstRun) << "," << runAfter(runList, b.lastRun, lastRunDataset) << "\n";
  }
  csv.close();

  // summary
  std::ofstream sum(outSummary);
  sum << "# " << detLabel << " " << dsName << ", hits per event from " << anaOut << "\n";
  if (derivedWindows)
  {
    sum << "# windows of tower / ring median: " << kBandLo << " - " << kBandHi
        << " x the expected ratio of a tower with gain 0.5 (Half) and 2 (Double), per eta ring ("
        << hRatioHalf->GetTitle() << ")\n";
    for (int ie = 0; ie < 24; ++ie)
    {
      const RatioWindows &w = windows[ie];
      sum << std::format("#   eta {:2d}: Half {:.3f} ({:.3f} - {:.3f})  Double {:.2f} ({:.2f} - {:.2f})\n", ie,
                         w.expHalf, w.halfLo, w.halfHi, w.expDouble, w.doubleLo, w.doubleHi);
    }
  }
  else
  {
    sum << "# fixed windows (no expected ratios in the anaOut file): Half " << kHalfLo << " - " << kHalfHi
        << ", Double " << kDoubleLo << " - " << kDoubleHi << " x the ring\n";
  }
  sum << "# normal level of a tower: median of tower / ring median over the runs within " << kNormalLo << " - "
      << kNormalHi << " x the ring (all runs if none)\n";
  sum << "# Step: below " << kStepLo << " or above " << kStepHi << " x the normal level and neither Half nor Double"
      << "; a run within " << kMinSigma << " sigma of the normal level is never flagged\n";
  sum << "# blocks with at least " << minRuns << " runs are written to " << outCsv << "\n";
  sum << "# ieta iphi state factor first_run last_run n_runs mean_ratio(tower / normal level) written"
      << " (bridged: runs of another state inside the block)\n";
  for (const HalfHeightBlock &b : written)
  {
    sum << std::format("{:4d} {:4d} {:7s} {:4.1f} {:6d} {:6d} {:4d} {:6.2f} yes{}\n", b.ieta, b.iphi,
                       stateName(b.state), calibFactor(b.state), b.firstRun, b.lastRun, b.nRuns, b.meanRatio,
                       b.nBridged > 0 ? std::format(" ({} bridged)", b.nBridged) : "");
  }
  for (const HalfHeightBlock &b : noted)
  {
    sum << std::format("{:4d} {:4d} {:7s} {:4.1f} {:6d} {:6d} {:4d} {:6.2f} no (fewer than {} runs)\n", b.ieta,
                       b.iphi, stateName(b.state), calibFactor(b.state), b.firstRun, b.lastRun, b.nRuns,
                       b.meanRatio, minRuns);
  }
  for (const HalfHeightBlock &b : steps)
  {
    sum << std::format(
        "{:4d} {:4d} {:7s} {:4.1f} {:6d} {:6d} {:4d} {:6.2f} no (gain step of about x{:.2f},"
        " not a half height)\n",
        b.ieta, b.iphi, stateName(b.state), calibFactor(b.state), b.firstRun, b.lastRun, b.nRuns,
        b.meanRatio, impliedGain(b.meanRatio, windows[b.ieta]));
  }
  if (!oddRuns.empty())
  {
    sum << "# runs with at least " << kMinTowersPerRunNote
        << " flagged towers (odd rate of the whole run, not a cable; blocks made of these runs only are suspect):\n";
    for (const auto &[n, run] : oddRuns)
    {
      sum << "# run " << run << ": " << n << " towers flagged\n";
    }
  }
  if (!fallbackTowers.empty())
  {
    sum << "# towers with no run within " << kNormalLo << " - " << kNormalHi
        << " x the ring, normal level (tower / ring median) taken from all their runs:";
    for (const std::string &t : fallbackTowers)
    {
      sum << " " << t;
    }
    sum << "\n";
  }
  sum.close();

  std::cout << "\n[findHalfHeight] towers read: " << nTowers << std::endl;
  std::cout << "  blocks written to the CSV (at least " << minRuns << " runs): " << written.size() << std::endl;
  for (const HalfHeightBlock &b : written)
  {
    std::cout << std::format(
                     "    tower ({:2d},{:2d}) {:6s} x{:.1f}  runs {} - {} ({} runs, tower / normal level"
                     " {:.2f}{})",
                     b.ieta, b.iphi, stateName(b.state), calibFactor(b.state), b.firstRun, b.lastRun, b.nRuns,
                     b.meanRatio, b.nBridged > 0 ? std::format(", {} bridged", b.nBridged) : "")
              << std::endl;
  }
  std::cout << "  blocks noted only (fewer than " << minRuns << " runs): " << noted.size() << ", listed in "
            << outSummary << std::endl;
  std::cout << "  gain steps that are not a factor 2 (at least " << minRuns << " runs, not written): " << steps.size()
            << " (" << nShortSteps << " shorter ones dropped)" << std::endl;
  for (const HalfHeightBlock &b : steps)
  {
    std::cout << std::format(
                     "    tower ({:2d},{:2d}) Step   x{:.2f} runs {} - {} ({} runs, tower / normal level"
                     " {:.2f})",
                     b.ieta, b.iphi, impliedGain(b.meanRatio, windows[b.ieta]), b.firstRun, b.lastRun, b.nRuns,
                     b.meanRatio)
              << std::endl;
  }
  if (!fallbackTowers.empty())
  {
    std::cout << "  towers with no run within " << kNormalLo << " - " << kNormalHi
              << " x the ring, normal level taken from all their runs (" << fallbackTowers.size() << "):";
    for (const std::string &t : fallbackTowers)
    {
      std::cout << " " << t;
    }
    std::cout << std::endl;
  }
  if (!oddRuns.empty())
  {
    std::cout << "  runs with at least " << kMinTowersPerRunNote
              << " flagged towers (odd rate of the whole run, not a cable):";
    for (const auto &[n, run] : oddRuns)
    {
      std::cout << " " << run << " (" << n << ")";
    }
    std::cout << std::endl;
  }
  std::cout << "  wrote " << outCsv << ", " << outSummary << ", " << outRoot << ", figures in " << figDir << std::endl;
}
