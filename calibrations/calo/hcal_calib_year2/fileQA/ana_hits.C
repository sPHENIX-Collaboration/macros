#include "../HcalCalibDatasetinputs.h"  // run list of a dataset

#include <TDirectory.h>
#include <TFile.h>
#include <TGraphErrors.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TProfile2D.h>
#include <TString.h>
#include <TSystem.h>

#include <cmath>
#include <format>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <unistd.h>  // isatty

// Hit counts, events and temperature of each beam run, and the 2D map of the hit rate per tower.
// Reads the spectra of the runs (out_<run>.root from condor_histmaker) and the temperature maps
// of the runs (<detector>_temp_<run>.root from makeTemp/loopCaloTemp.C).
//
// How to run (from fileQA, after condor_histmaker made the spectra and makeTemp the temperature maps):
//
//   // cosmics only: spectra in ../condor_histmaker/out_run -> output/anaOut_ohcal_cosmics.root
//   root -l -b -q 'ana_hits.C("run3oo", 0)'      // OHCal
//   root -l -b -q 'ana_hits.C("run3oo", 1)'      // IHCal
//
//   // cosmics + towerslope: spectra in ../condor_histmaker/out_run_cosmics_tsc -> output/anaOut_ohcal_cosmics_tsc.root
//   root -l -b -q 'ana_hits.C("run3oo", 0, "../condor_histmaker/out_run_cosmics_tsc")'
//   root -l -b -q 'ana_hits.C("run3oo", 1, "../condor_histmaker/out_run_cosmics_tsc")'
//
//   // cosmics + towerslope + temperature: out_run_cosmics_tsc_temp -> output/anaOut_ohcal_cosmics_tsc_temp.root
//   root -l -b -q 'ana_hits.C("run3oo", 0, "../condor_histmaker/out_run_cosmics_tsc_temp")'
//   root -l -b -q 'ana_hits.C("run3oo", 1, "../condor_histmaker/out_run_cosmics_tsc_temp")'
//   // then the half height towers (Step 3) from the hits per event of each tower in this file:
//   root -l -b -q 'findHalfHeight.C("run3oo", 0)'
//   root -l -b -q 'findHalfHeight.C("run3oo", 1)'
//
//   // temperature maps are read from ../makeTemp/temp_ohcal (temp_ihcal), where makeTemp/loopCaloTemp.C writes
//   // them (<detector>_temp_<run>.root); another folder is given as 4th argument, e.g.
//   root -l -b -q 'ana_hits.C("run3oo", 0, "../condor_histmaker/out_run_cosmics_tsc", "/path/to/temp_ohcal")'
//
//   // own run list instead of the run list of the dataset
//   root -l -b -q 'ana_hits.C("myRunList.txt", 0, "../condor_histmaker/out_run_cosmics_tsc")'
//
// The out_run_<name> folders are filled by condor_histmaker/copy_outfiles.sh <name> (out_run for cosmics).
// Before the loop the macro prints how many runs have spectra and temperature maps and, in a terminal,
// asks whether to go on (in a script or a condor job it goes on without asking).
// Then plot the hit rates with plot_hits.C (see there).
//
// runlist_txt: dataset name (run2pp, run2auau, run3oo, ...: its run list in ../HcalCalibDatasetinputs.h)
//              or a beam run list, one run per line
// isIHcal:     0 OHCal, 1 IHCal
// spectraDir:  folder with out_<run>.root: ../condor_histmaker/out_run (cosmics) or out_run_<name>
//              (e.g. out_run_cosmics_tsc). The calibration name is taken from the folder name.
// tempDir:     folder with the temperature maps, "" for ../makeTemp/temp_ohcal (temp_ihcal)
// out_root:    output file, "" for output/anaOut_<detector>_<calibration name>.root,
//              e.g. output/anaOut_ohcal_cosmics_tsc.root
// eLow, eHigh: energy window of a hit in GeV, negative for the default (OHCal 1 to 5, IHCal 0.2 to 1)
//
// output: h_hits        hits in the energy window per run, summed over the eta rings ieL to ieH
//         h_events_run  events per run
//         runNumberHist run number of each run index
//         h_temp_run    average tower temperature per run (towers with 10 < T < 32 C)
//         pr2d_temp     tower temperature profile over the runs
//         h_hits_2d     hits per tower from all runs, divided by the average of the live towers of its eta ring
//                       (eta ring = column of the map)
//         h_meanE_run   mean energy above the threshold eLow (no upper limit) per run, eta rings ieL to ieH
//         h_meanE_2d    mean energy above the threshold eLow (no upper limit) per tower, from all runs (GeV)
//         hpe_tower_vs_runnumber/g_hpe_eta<ie>_phi<ip>
//                       hits per event of each tower (energy window) vs run number, one TGraphErrors per
//                       tower (error: sqrt of the entries / events), only the runs with spectra and events.
//                       Input of findHalfHeight.C.
//         h_hitRatio_gainHalf, h_hitRatio_gainDouble
//                       per eta ring: hits in the energy window of a tower with gain 0.5 (2) relative to a
//                       normal tower, from the ring spectra of all runs. A tower with gain g records g x E, so
//                       its hits in [eLow, eHigh] are the hits of a normal tower in [eLow/g, eHigh/g].
//                       findHalfHeight.C takes its thresholds from them.

namespace
{
  float getTempRun(int runnumber, const std::string &tempDir, const std::string &detDB, TProfile2D *pr2d_temp);

  // sum of the energy of the entries of a spectrum from bin b1 to b2 (bin center x entries)
  double sumEnergy(TH1 *h, int b1, int b2)
  {
    double sum = 0.0;
    for (int b = b1; b <= b2; ++b) sum += h->GetBinCenter(b) * h->GetBinContent(b);
    return sum;
  }
}  // namespace

// ------------------------------------------------------------

void ana_hits(const char *runlist_txt = "runList.txt", int isIHcal = 0,
              const std::string &spectraDir = "../condor_histmaker/out_run",
              std::string tempDir = "", std::string out_root = "",
              float eLow = -1, float eHigh = -1)
{
  // settings of the detector
  const std::string det = (isIHcal == 1) ? "ihcal" : "ohcal";
  const std::string detDB = (isIHcal == 1) ? "HCALIN" : "HCALOUT";                  // name in the temperature files
  const std::string etaHist = (isIHcal == 1) ? "hcalin_eta_" : "hcalout_eta_";      // spectra of an eta ring
  const std::string towerHist = (isIHcal == 1) ? "hcal_in_eta_" : "hcal_out_eta_";  // spectra of a tower
  if (eLow < 0) eLow = (isIHcal == 1) ? 0.2 : 1.0;
  if (eHigh < 0) eHigh = (isIHcal == 1) ? 1.0 : 5.0;
  if (tempDir.empty()) tempDir = "../makeTemp/temp_" + det;
  // calibration name from the spectra folder: out_run -> cosmics, out_run_<name> -> <name>
  std::string spectraPath = spectraDir;
  while (spectraPath.size() > 1 && spectraPath.back() == '/')
  {
    spectraPath.pop_back();
  }
  const std::string spectraName = gSystem->BaseName(spectraPath.c_str());
  std::string calibName = spectraName;
  if (spectraName == "out_run")
  {
    calibName = "cosmics";
  }
  else if (spectraName.starts_with("out_run_"))
  {
    calibName = spectraName.substr(8);
  }
  if (out_root.empty()) out_root = std::format("output/anaOut_{}_{}.root", det, calibName);
  gSystem->mkdir(gSystem->GetDirName(out_root.c_str()).Data(), true);  // folder of the output file
  // eta rings summed for the hits per run
  const int ieL = 1;
  const int ieH = 22;

  std::cout << "[ana_hits] " << det << ", hits with " << eLow << " < E < " << eHigh << " GeV" << std::endl;
  std::cout << "  spectra from      " << spectraDir << std::endl;
  std::cout << "  temperatures from " << tempDir << std::endl;
  std::cout << "  output            " << out_root << std::endl;

  // 0) read run list (the run list of the dataset if a dataset name is given)
  if (const HCalCalibDataset *ds = findHCalCalibDataset(runlist_txt))
  {
    std::cout << "  run list of " << ds->name << ": " << ds->runList << std::endl;
    runlist_txt = ds->runList.c_str();
  }
  std::ifstream in(runlist_txt);
  if (!in.is_open())
  {
    std::cerr << " ERROR: cannot open run list: " << runlist_txt << "\n";
    return;
  }

  std::vector<int> runs;
  std::string line;
  while (getline(in, line))
  {
    if (line.empty() || line[0] == '#') continue;
    try
    {
      runs.push_back(stoi(line));
    }
    catch (...)
    {
      std::cerr << "WARN: skip bad line: " << line << "\n";
    }
  }
  if (runs.empty())
  {
    std::cerr << "[ana_hits] ERROR: no runs found in " << runlist_txt << "\n";
    return;
  }

  const int N = (int) runs.size();
  std::cout << "number of runs in provided runList is:" << N << std::endl;

  // check the inputs before the loop: spectra and temperature maps of the runs
  int nSpectra = 0;
  int nTemp = 0;
  for (int run : runs)
  {
    if (!gSystem->AccessPathName(std::format("{}/out_{}.root", spectraDir, run).c_str())) nSpectra++;
    if (!gSystem->AccessPathName(std::format("{}/{}_temp_{}.root", tempDir, detDB, run).c_str())) nTemp++;
  }
  std::cout << "  spectra (out_<run>.root) found for " << nSpectra << " of " << N << " runs in " << spectraDir
            << std::endl;
  std::cout << "  temperature maps found for    " << nTemp << " of " << N << " runs in " << tempDir << std::endl;
  if (nSpectra == 0)
  {
    std::cerr << "ERROR: no spectra of the run list in " << spectraDir << std::endl;
    return;
  }
  if (nTemp == 0)
  {
    std::cout << "WARNING: no temperature map of the run list in " << tempDir
              << " (4th argument; makeTemp/loopCaloTemp.C makes them). h_temp_run and pr2d_temp stay empty."
              << std::endl;
  }
  // asked for in a terminal only, so that the macro still runs in a script or a condor job
  if (isatty(fileno(stdin)))
  {
    std::cout << "run with these inputs? (y/n): " << std::flush;
    std::string answer;
    std::getline(std::cin, answer);
    if (answer != "y" && answer != "Y" && answer != "yes")
    {
      std::cout << "stopped, nothing made" << std::endl;
      return;
    }
  }

  // 1) book histograms
  const std::string hitsTitle =
      std::format("{} hit counts;run index;entries ({:.1f} < E < {:.1f} GeV)", det, eLow, eHigh);
  TH1F *h_hits = new TH1F("h_hits", hitsTitle.c_str(), N, 0, N);
  const std::string hits2dTitle =
      std::format("2D map of {} hit rate;#eta;#phi;hit rate / average of the eta ring", det);
  TH2F *h_hits_2d = new TH2F("h_hits_2d", hits2dTitle.c_str(), 24, 0, 24, 64, 0, 64);

  TH1F *h_meanE_run = new TH1F(
      "h_meanE_run",
      std::format("{} mean energy above threshold per run;run index;mean E (E > {:.1f} GeV) [GeV]", det, eLow).c_str(),
      N, 0, N);
  TH2F *h_meanE_2d = new TH2F(
      "h_meanE_2d",
      std::format("{} mean energy above threshold (E > {:.1f} GeV);#eta;#phi;mean E [GeV]", det, eLow).c_str(),
      24, 0, 24, 64, 0, 64);

  TH1F *h_events_run = new TH1F("h_events_run", "Events per run;run index;events", N, 0, N);
  TH1F *runNumberHist = new TH1F("runNumberHist", "Run numbers;run index;run number", N, 0, N);
  const std::string tempTitle = std::format("Avg {} tower temp per run;run index;T [^{{o}}C]", detDB);
  TH1F *h_temp_run = new TH1F("h_temp_run", tempTitle.c_str(), N, 0, N);

  // same tower selection as the average temperature (10 < T < 32 C)
  TProfile2D *pr2d_temp = new TProfile2D("pr2d_temp",
                                         std::format("{} tower temperature profile;eta;phi;T [^{{o}}C]", detDB).c_str(),
                                         24, 0, 24, 64, 0, 64, 10.0, 32.0, "s");

  std::vector<int> missingOut;   // out_<RUN>.root files missing
  std::vector<int> missingTemp;  // temperature files missing

  // hits of each tower summed over all runs, for the 2D map
  double towerHits[24][64] = {};
  // hits of each eta ring in the window, and in the window of a tower with gain 0.5 and 2 (all runs)
  double ringHits[24] = {};
  double ringHitsGainHalf[24] = {};
  double ringHitsGainDouble[24] = {};
  // entries above the threshold eLow and their energy, for the mean energy
  double towerN[24][64] = {};
  double towerE[24][64] = {};
  // hits per event of each tower, run by run (the graphs of hpe_tower_vs_runnumber)
  std::vector<double> towerHpeRun[24][64];
  std::vector<double> towerHpeErr[24][64];
  std::vector<double> towerHpeX[24][64];

  // 2) per-run loop
  for (int i = 0; i < N; ++i)
  {
    const int run = runs[i];
    runNumberHist->SetBinContent(i + 1, run);

    std::cout << "Analyzing run index " << i << " with runnumber: " << run << std::endl;

    // open out_<RUN>.root
    const std::string infile = std::format("{}/out_{}.root", spectraDir, run);

    std::unique_ptr<TFile> fin;
    if (!gSystem->AccessPathName(infile.c_str())) fin.reset(TFile::Open(infile.c_str(), "READ"));
    if (!fin || fin->IsZombie())
    {
      std::cout << "out_runnumber file missing or not readable for run: " << run << "\n";
      missingOut.push_back(run);
      h_hits->SetBinContent(i + 1, 0);
      h_events_run->SetBinContent(i + 1, 0);
    }
    else
    {
      // hits of the run: eta rings ieL to ieH
      double hits_sum = 0.0;
      double hits_sum_err2 = 0.0;
      double e_sum = 0.0;  // energy and entries above the threshold eLow
      double n_sum = 0.0;
      for (int ieta = ieL; ieta <= ieH; ++ieta)
      {
        TH1 *h_eta = (TH1 *) fin->Get(std::format("{}{}", etaHist, ieta).c_str());
        if (!h_eta)
        {
          std::cout << "Missing " << etaHist << ieta << " in " << infile << std::endl;
          continue;
        }
        double err = 0.0;
        hits_sum += h_eta->IntegralAndError(h_eta->FindBin(eLow), h_eta->FindBin(eHigh), err);
        hits_sum_err2 += err * err;
        e_sum += sumEnergy(h_eta, h_eta->FindBin(eLow), h_eta->GetNbinsX());
        n_sum += h_eta->Integral(h_eta->FindBin(eLow), h_eta->GetNbinsX());
      }
      if (n_sum > 0) h_meanE_run->SetBinContent(i + 1, e_sum / n_sum);
      h_hits->SetBinContent(i + 1, hits_sum);
      h_hits->SetBinError(i + 1, std::sqrt(hits_sum_err2));

      // events
      double nEvents = 0.0;
      if (TH1 *h_event = (TH1 *) fin->Get("h_event"))
      {
        nEvents = h_event->GetEntries();
        h_events_run->SetBinContent(i + 1, nEvents);
      }

      // hits of each eta ring in the window, and with the window moved as for a tower with gain 0.5 and 2
      for (int ie = 0; ie < 24; ++ie)
      {
        TH1 *h_eta = (TH1 *) fin->Get(std::format("{}{}", etaHist, ie).c_str());
        if (!h_eta) continue;
        ringHits[ie] += h_eta->Integral(h_eta->FindBin(eLow), h_eta->FindBin(eHigh));
        ringHitsGainHalf[ie] += h_eta->Integral(h_eta->FindBin(2 * eLow), h_eta->FindBin(2 * eHigh));
        ringHitsGainDouble[ie] += h_eta->Integral(h_eta->FindBin(0.5 * eLow), h_eta->FindBin(0.5 * eHigh));
      }

      // hits of each tower, summed over the runs
      for (int ie = 0; ie < 24; ++ie)
      {
        for (int ip = 0; ip < 64; ++ip)
        {
          TH1 *hE = (TH1 *) fin->Get(std::format("{}{}_phi_{}", towerHist, ie, ip).c_str());
          if (!hE) continue;
          double hitsTowerErr = 0.0;
          const double hitsTower = hE->IntegralAndError(hE->FindBin(eLow), hE->FindBin(eHigh), hitsTowerErr);
          towerHits[ie][ip] += hitsTower;
          if (nEvents > 0)
          {
            towerHpeX[ie][ip].push_back(run);
            towerHpeRun[ie][ip].push_back(hitsTower / nEvents);
            towerHpeErr[ie][ip].push_back(hitsTowerErr / nEvents);
          }
          towerE[ie][ip] += sumEnergy(hE, hE->FindBin(eLow), hE->GetNbinsX());
          towerN[ie][ip] += hE->Integral(hE->FindBin(eLow), hE->GetNbinsX());
        }
      }
    }

    // temperature
    float Tavg = getTempRun(run, tempDir, detDB, pr2d_temp);

    if (Tavg <= 0)
    {
      missingTemp.push_back(run);
      h_temp_run->SetBinContent(i + 1, 0);
    }
    else
    {
      h_temp_run->SetBinContent(i + 1, Tavg);
    }

  }  // N loop end

  // 2D map: hits of a tower divided by the average over the live towers of its eta ring
  // (towers without hits, e.g. dead towers, are left at 0 and do not enter the average)
  for (int ie = 0; ie < 24; ++ie)
  {
    double ringSum = 0.0;
    int nLive = 0;
    for (int ip = 0; ip < 64; ++ip)
    {
      if (towerHits[ie][ip] > 0)
      {
        ringSum += towerHits[ie][ip];
        nLive++;
      }
    }
    for (int ip = 0; ip < 64; ++ip)
    {
      double val = (ringSum > 0) ? nLive * towerHits[ie][ip] / ringSum : 0.0;
      h_hits_2d->SetBinContent(ie + 1, ip + 1, val);
      if (towerN[ie][ip] > 0) h_meanE_2d->SetBinContent(ie + 1, ip + 1, towerE[ie][ip] / towerN[ie][ip]);
    }
  }

  // expected hit ratio of a tower with gain 0.5 and 2, per eta ring
  const std::string ratioHalfTitle = std::format(
      "{} hits of a tower with gain 0.5 / normal ({:.1f} < E < {:.1f} GeV);#eta;hit ratio", det, eLow, eHigh);
  TH1F *h_hitRatio_gainHalf = new TH1F("h_hitRatio_gainHalf", ratioHalfTitle.c_str(), 24, 0, 24);
  const std::string ratioDoubleTitle =
      std::format("{} hits of a tower with gain 2 / normal ({:.1f} < E < {:.1f} GeV);#eta;hit ratio", det, eLow, eHigh);
  TH1F *h_hitRatio_gainDouble = new TH1F("h_hitRatio_gainDouble", ratioDoubleTitle.c_str(), 24, 0, 24);
  std::cout << "hits of a tower with gain 0.5 (2) relative to a normal tower, " << eLow << " < E < " << eHigh
            << " GeV, per eta ring:" << std::endl;
  for (int ie = 0; ie < 24; ++ie)
  {
    if (ringHits[ie] <= 0) continue;
    h_hitRatio_gainHalf->SetBinContent(ie + 1, ringHitsGainHalf[ie] / ringHits[ie]);
    h_hitRatio_gainDouble->SetBinContent(ie + 1, ringHitsGainDouble[ie] / ringHits[ie]);
    std::cout << std::format("  eta {:2d}: x{:.3f} (x{:.2f})", ie, ringHitsGainHalf[ie] / ringHits[ie],
                             ringHitsGainDouble[ie] / ringHits[ie])
              << std::endl;
  }

  // 3) write output
  TFile fout(out_root.c_str(), "RECREATE");
  h_hits->Write();
  h_events_run->Write();
  runNumberHist->Write();
  h_temp_run->Write();
  pr2d_temp->Write();
  h_hits_2d->Write();
  h_meanE_run->Write();
  h_meanE_2d->Write();
  h_hitRatio_gainHalf->Write();
  h_hitRatio_gainDouble->Write();

  // hits per event of each tower vs run number
  TDirectory *dirHpe = fout.mkdir("hpe_tower_vs_runnumber");
  dirHpe->cd();
  for (int ie = 0; ie < 24; ++ie)
  {
    for (int ip = 0; ip < 64; ++ip)
    {
      TGraphErrors gr(towerHpeX[ie][ip].size(), towerHpeX[ie][ip].data(), towerHpeRun[ie][ip].data(), nullptr,
                      towerHpeErr[ie][ip].data());
      gr.SetName(std::format("g_hpe_eta{:02d}_phi{:02d}", ie, ip).c_str());
      gr.SetTitle(std::format("{} hits/event, tower eta={} phi={} ({:.1f} < E < {:.1f} GeV);run number;hits/event",
                              det, ie, ip, eLow, eHigh)
                      .c_str());
      gr.Write();
    }
  }
  fout.cd();

  fout.Close();

  int nTotal = runs.size();
  int nOutMiss = missingOut.size();
  int nTempMiss = missingTemp.size();

  std::cout << "\n[ana_hits] Summary for runlist: " << runlist_txt << "\n";
  std::cout << "  Total runs processed : " << nTotal << "\n";
  std::cout << "  Missing out_<RUN>.root files : " << nOutMiss
            << " (" << (100.0 * nOutMiss / nTotal) << "%)"
            << "\n";
  std::cout << "  Missing temperature files    : " << nTempMiss
            << " (" << (100.0 * nTempMiss / nTotal) << "%)"
            << "\n";

  // dump run numbers
  if (!missingOut.empty())
  {
    std::cout << "  Runs with missing out files (" << nOutMiss << "): ";
    for (int r : missingOut) std::cout << r << " ";
    std::cout << "\n";
  }
  if (!missingTemp.empty())
  {
    std::cout << "  Runs with missing temp files (" << nTempMiss << "): ";
    for (int r : missingTemp) std::cout << r << " ";
    std::cout << "\n";
  }
  std::cout << "[ana_hits] wrote " << out_root << std::endl;

}  // macro end

// ------------------------------------------------------------

namespace
{
  // average tower temperature of a run from the map made by makeTemp/loopCaloTemp.C, 0 if there is none
  float getTempRun(int runnumber, const std::string &tempDir, const std::string &detDB, TProfile2D *pr2d_temp)
  {
    const std::string tpath = std::format("{}/{}_temp_{}.root", tempDir, detDB, runnumber);

    if (gSystem->AccessPathName(tpath.c_str()))
    {
      return 0.0F;  // file missing
    }

    std::unique_ptr<TFile> f(TFile::Open(tpath.c_str(), "READ"));
    if (!f || f->IsZombie())
    {
      return 0.0F;  // file couldn't open
    }

    TH2 *h = (TH2 *) f->Get(std::format("h_{}_temp", detDB).c_str());
    if (!h) return 0.0F;

    float sum = 0.0F;
    int cnt = 0;
    int nx = h->GetXaxis()->GetNbins();
    int ny = h->GetYaxis()->GetNbins();
    for (int ix = 1; ix <= nx; ++ix)
    {
      for (int iy = 1; iy <= ny; ++iy)
      {
        float t = h->GetBinContent(ix, iy);
        // same tower selection as for the cosmics runs (genFinalCalib/getCosmicTemps.C)
        if (t > 10 && t < 32)
        {
          sum += t;
          cnt++;
        }
        if (pr2d_temp) pr2d_temp->Fill(ix - 1, iy - 1, t);
      }
    }
    if (cnt < 3) return 0.0F;
    return sum / cnt;
  }
}  // namespace
