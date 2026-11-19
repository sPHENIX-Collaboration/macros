#include "../HcalCalibDatasetinputs.h"  // dataset of a run and its cosmics runs

#include <TCanvas.h>
#include <TDatime.h>
#include <TF1.h>
#include <TFile.h>
#include <TGraphErrors.h>
#include <TH1.h>
#include <TH2.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TLine.h>
#include <TPad.h>
#include <TPaveText.h>
#include <TProfile.h>
#include <TProfile2D.h>
#include <TStyle.h>
#include <TSystem.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <format>
#include <fstream>
#include <iomanip>
#include <map>
#include <sstream>

// Hits per event of each beam run vs run time and vs MBD rate, with the pile-up correction, the 1D distribution
// of the hits per event over the runs, and the run temperature. Works for all datasets of HcalCalibDatasetinputs.h.
//
// inputs: anaOut file of ana_hits.C, and the start time and MBD rate of each run from
//         get_mbdrate.py (output/run_timestamps_with_rate_<dataset>.csv)
//
// usage: root -l -b -q 'plot_hits.C("run3oo", 0, "output/anaOut_ohcal_cosmics_tsc_temp.root")'
//        root -l -b -q 'plot_hits.C("run3oo", 0, "output/anaOut_ohcal_cosmics_tsc_temp.root", "output/anaOut_ohcal_cosmics.root")'
//
// dataset   : run2pp, run2auau, run3oo, ... "" for the dataset of the first run in the anaOut file
// isIHcal   : 0 OHCal, 1 IHCal
// anaFile   : anaOut file of ana_hits.C, "" for output/anaOut_ohcal_cosmics.root (anaOut_ihcal_cosmics.root)
// anaFile_step1 : optional second anaOut file (e.g. cosmics only), drawn vs run time together with anaFile
//                 (e.g. cosmics + temperature correction) for the comparison of the two calibration steps
// rateCsv   : run start time and MBD rate, "" for output/run_timestamps_with_rate_<dataset>.csv
//
// pileup    : pile-up correction, the same for every calibration step
//             "lambda" (default): hits_corr = hits / (1 + lambda), lambda from the probability of more than one
//                                 collision per bunch crossing at the MBD rate (corrDouble)
// nBunches  : number of filled bunches for "lambda" (111 in p+p 2024)
//             "pol3":             the hits per event vs MBD rate are fitted with pol3, f(rate), and scaled to
//                                 zero rate: hits_corr = hits * f(0) / f(rate) (corrPileupFit)
//             "none":             no correction
//   root -l -b -q 'plot_hits.C("run3oo", 0, "output/anaOut_ohcal_cosmics_tsc.root", "", "", "pol3")'
//
// output: figures/<dataset>_<tag>_*.pdf, output/plotHits_<dataset>_<tag>.root and .csv
//         (tag = name of the anaOut file without anaOut_ and .root, e.g. ohcal_cosmics_tsc_temp)

std::vector<int> runNumbers;    // Stores the run numbers
std::vector<double> timestamps;  // Stores the timestamps
std::vector<double> values;     // Stores the corresponding values (MBD rate in Hz)
std::vector<int> hours;     // Stores the corresponding values

std::vector<int> runNumbers_cosmic;    // Stores the run numbers
std::vector<double> timestamps_cosmic;  // Stores the timestamps

void getInfo(const std::string &rateCsv);
void getCosmicInfo();
float corrDouble(float hits_per_event, double rate, double nBunches);
float corrPileupFit(float hits_per_event, double rate, TF1 *f_pileup);
float corrPileup(float hits_per_event, double rate, TF1 *f_pileup, const std::string &pileup, double nBunches);
double one_plus_lambda;

const int nbins = 100;
float corr_hits;

namespace
{
  // reads "YYYY-MM-DD hh:mm:ss" into t6, returns the number of fields read (6 if complete)
  int parseTime(const std::string &str, std::array<int, 6> &t6)
  {
    std::istringstream ss(str);
    int n = 0;
    char sep = 0;
    for (int &field : t6)
    {
      if (n > 0 && n != 3 && !(ss >> sep))  // '-' or ':' between the fields, a space before the hour
      {
        break;
      }
      if (!(ss >> field))
      {
        break;
      }
      ++n;
    }
    return n;
  }
}  // namespace

// hits per event, rate and temperature of a beam run
struct RunHits
{
  int run;
  double ts;
  float hits;
  float hits_err;
  double rate;
  double T;
};

// collision system of a dataset for the plot label
std::string collisionLabel(const std::string &dataset)
{
  if (dataset.find("auau") != std::string::npos) return "Au+Au, #sqrt{s_{NN}} = 200 GeV";
  if (dataset.find("oo") != std::string::npos) return "O+O, #sqrt{s_{NN}} = 200 GeV";
  if (dataset.find("pp") != std::string::npos) return "p+p, #sqrt{s} = 200 GeV";
  return dataset;
}

// hits per event of each run of an anaOut file that has a start time in the rate csv
std::vector<RunHits> readRunHits(const std::string &anaFile)
{
  std::vector<RunHits> out;
  TFile *fin = TFile::Open(anaFile.c_str(), "READ");
  if (!fin || fin->IsZombie())
  {
    std::cout << "cannot open " << anaFile << std::endl;
    return out;
  }
  TH1F *h_events_run = (TH1F *) fin->Get("h_events_run");
  TH1F *h_hits = (TH1F *) fin->Get("h_hits");
  TH1F *h_runnumber = (TH1F *) fin->Get("runNumberHist");
  TH1F *h_temp_run = (TH1F *) fin->Get("h_temp_run");

  TH1F *h_hits_per_event = (TH1F *) h_hits->Clone("h_hits_per_event");
  if (h_hits_per_event->GetSumw2N() == 0) h_hits_per_event->Sumw2();
  if (h_events_run->GetSumw2N() == 0) h_events_run->Sumw2();
  h_hits_per_event->Divide(h_events_run);

  int n_runs = h_runnumber->GetNbinsX();
  std::cout << "number of runs from " << anaFile << " is: " << n_runs << std::endl;

  for (int it = 0; it < n_runs; it++)
  {
    int runnumber = h_runnumber->GetBinContent(1 + it);
    for (size_t ir = 0; ir < runNumbers.size(); ir++)
    {
      if (runnumber != runNumbers[ir]) continue;
      float hits = h_hits_per_event->GetBinContent(it + 1);
      if (!std::isfinite(hits) || hits <= 0) break;  // run without spectra
      out.push_back({runnumber, timestamps[ir], hits, (float) h_hits_per_event->GetBinError(it + 1),
                     values[ir], h_temp_run->GetBinContent(1 + it)});
      break;
    }
  }
  return out;
}

// time axis frame from xmi to xma
TH1F *timeFrame(const char *name, double xmi, double xma, double ymi, double yma, const char *ytitle)
{
  double pad = 0.02 * (xma - xmi);
  TH1F *h_frame = new TH1F(name, "", nbins, xmi - pad, xma + pad);
  h_frame->GetXaxis()->SetTimeDisplay(1);
  h_frame->GetXaxis()->SetTimeFormat("%m-%d");
  h_frame->GetXaxis()->SetTimeOffset(0, "local");
  h_frame->GetXaxis()->SetTitle("run time");
  h_frame->GetYaxis()->SetTitle(ytitle);
  h_frame->GetYaxis()->SetRangeUser(ymi, yma);
  h_frame->GetYaxis()->SetTitleSize(0.045);
  h_frame->GetYaxis()->SetLabelSize(0.045);
  h_frame->SetStats(false);
  return h_frame;
}

void drawLabels(const std::string &dataset, const std::string &det)
{
  TLatex *lab1 = new TLatex(0.15, 0.84, "#bf{#it{sPHENIX}} Internal");
  lab1->SetNDC(); lab1->SetTextFont(42); lab1->SetTextSize(0.055); lab1->Draw();

  TLatex *lab2 = new TLatex(0.15, 0.77, (collisionLabel(dataset) + ", " + det).c_str());
  lab2->SetNDC(); lab2->SetTextFont(42); lab2->SetTextSize(0.045); lab2->Draw();
}

std::vector<int> getBeamRunsWhereCosmicChanges(const std::vector<RunHits> &rh,
                                               std::vector<int> cosRuns)
{
  std::vector<int> changeBeamRuns;
  if (cosRuns.empty()) return changeBeamRuns;

  std::sort(cosRuns.begin(), cosRuns.end());

  int previousCosmicRun = -1;

  for (const RunHits &r : rh)
  {
    int beamRun = r.run;

    // same logic as findCosmicCalib
    int target = 0;
    for (size_t j = 0; j < cosRuns.size(); j++)
    {
      if (beamRun < cosRuns[j]) break;
      target = j;
    }

    int currentCosmicRun = cosRuns[target];

    if (currentCosmicRun != previousCosmicRun)
    {
      changeBeamRuns.push_back(beamRun);

      std::cout << "Cosmic calib " << currentCosmicRun
                << " starts at beam run " << beamRun
                << std::endl;
    }

    previousCosmicRun = currentCosmicRun;
  }

  return changeBeamRuns;
}

void plot_hits(std::string dataset = "", int isIHcal = 0, std::string anaFile = "",
               const std::string &anaFile_step1 = "", std::string rateCsv = "",
               const std::string &pileup = "lambda", double nBunches = 111)
{
  gStyle->SetOptStat(0);
  const std::string det = (isIHcal == 1) ? "ihcal" : "ohcal";
  const std::string detLabel = (isIHcal == 1) ? "iHCal" : "oHCal";
  if (anaFile.empty()) anaFile = "output/anaOut_" + det + "_cosmics.root";

  // dataset of the first run in the anaOut file
  if (dataset.empty())
  {
    TFile *f = TFile::Open(anaFile.c_str(), "READ");
    TH1F *h_runnumber = f ? (TH1F *) f->Get("runNumberHist") : nullptr;
    if (!h_runnumber)
    {
      std::cout << "cannot read runNumberHist from " << anaFile << std::endl;
      return;
    }
    dataset = getHCalCalibDataset((int) h_runnumber->GetBinContent(1)).name;
    f->Close();
  }
  if (rateCsv.empty()) rateCsv = "output/run_timestamps_with_rate_" + dataset + ".csv";

  // tag of the output names: anaOut_ohcal_cosmics_tsc_temp.root -> ohcal_cosmics_tsc_temp
  std::string tag = gSystem->BaseName(anaFile.c_str());
  if (tag.rfind("anaOut_", 0) == 0) tag = tag.substr(7);
  if (tag.size() > 5 && tag.substr(tag.size() - 5) == ".root") tag = tag.substr(0, tag.size() - 5);
  const std::string figPre = "figures/" + dataset + "_" + tag;

  std::cout << "[plot_hits] " << dataset << " " << det << ": " << anaFile << ", rates from " << rateCsv << std::endl;

  getInfo(rateCsv);
  getCosmicInfo();
  if (runNumbers.empty()) return;

  gSystem->mkdir("figures", kTRUE);
  gSystem->mkdir("output", kTRUE);

  std::vector<RunHits> rh = readRunHits(anaFile);
  if (rh.empty())
  {
    std::cout << "no runs of " << anaFile << " in " << rateCsv << std::endl;
    return;
  }

  // beam runs where the cosmics calibration changes (cosmics runs of the dataset)
  if (const HCalCalibDataset *ds = findHCalCalibDataset(dataset))
  {
    auto changeRuns = getBeamRunsWhereCosmicChanges(rh, isIHcal ? ds->ih_cosmicRuns : ds->oh_cosmicRuns);
  }

  TGraphErrors *gr_hits_time = new TGraphErrors();
  TGraphErrors *gr_hits_run = new TGraphErrors();
  TGraphErrors *gr_rate_time = new TGraphErrors();
  TGraphErrors *gr_hits_time_corr = new TGraphErrors();
  TProfile *pr_temp_hour = new TProfile("pr_temp_hour", "", 24, -0.5, 23.5);
  TGraphErrors *gr_temp_time = new TGraphErrors();
  TGraphErrors *gr_hits_rate = new TGraphErrors();
  TGraphErrors *gr_corr_hits_rate = new TGraphErrors();

  if (pileup != "lambda" && pileup != "pol3" && pileup != "none")
  {
    std::cout << "unknown pile-up correction " << pileup << " (lambda, pol3 or none)" << std::endl;
    return;
  }
  std::cout << "pile-up correction: " << pileup;
  if (pileup == "lambda") std::cout << ", " << nBunches << " filled bunches";
  std::cout << std::endl;

  // 1) hits per event vs MBD rate, pol3 fit (used for the pile-up correction with pileup = "pol3")
  for (const RunHits &r : rh)
  {
    if (r.rate <= 0) continue;
    int ip = gr_hits_rate->GetN();
    gr_hits_rate->SetPoint(ip, r.rate, r.hits);
    gr_hits_rate->SetPointError(ip, 0.0, r.hits_err);
  }
  double rateMax = 0;
  for (const RunHits &r : rh) rateMax = std::max(rateMax, r.rate);
  TF1 *f_pileup = new TF1("f_pileup", "pol3", 0, rateMax);
  f_pileup->SetLineColor(kRed);
  gr_hits_rate->Fit(f_pileup, "Q");
  std::cout << "pile-up fit pol3: f(0) = " << f_pileup->Eval(0) << ", chi2/ndf = "
            << f_pileup->GetChisquare() << "/" << f_pileup->GetNDF() << std::endl;

  // 2) per run: pile-up correction, graphs vs run time
  double mean_temp = 0.0;
  double sum_temp = 0.0;
  int n_temp = 0;
  double sum_corr_hits = 0;
  int n_corr_hits = 0;

  std::ofstream csvout("output/plotHits_" + dataset + "_" + tag + ".csv");
  csvout << "Run,Timestamp,Hits_per_event,Hits_err,MBD Rate,OnePlusLambda,Hits_corr,Temp\n";

  for (const RunHits &r : rh)
  {
    int ip = gr_hits_time->GetN();
    gr_hits_time->SetPoint(ip, r.ts, r.hits);
    gr_hits_time->SetPointError(ip, 0, r.hits_err);

    gr_hits_run->SetPoint(ip, r.run, r.hits);
    gr_hits_run->SetPointError(ip, 0, r.hits_err);

    gr_rate_time->SetPoint(ip, r.ts, r.rate);

    double T = r.T;
    if (std::isfinite(T) && T > 0)
    {
      TDatime t((UInt_t) r.ts);
      pr_temp_hour->Fill(t.GetHour(), T);
      gr_temp_time->SetPoint(gr_temp_time->GetN(), r.ts, T);
      sum_temp += T;
      ++n_temp;
    }

    corr_hits = 0;
    one_plus_lambda = 0;
    if (r.rate > 0)
    {
      corr_hits = corrPileup(r.hits, r.rate, f_pileup, pileup, nBunches);

      one_plus_lambda = r.hits / corr_hits;
      float corr_err = r.hits_err / one_plus_lambda;
      int ic = gr_hits_time_corr->GetN();
      gr_hits_time_corr->SetPoint(ic, r.ts, corr_hits);
      gr_hits_time_corr->SetPointError(ic, 0, corr_err);
      gr_corr_hits_rate->SetPoint(ic, r.rate, corr_hits);
      gr_corr_hits_rate->SetPointError(ic, 0, corr_err);

      sum_corr_hits += corr_hits;
      n_corr_hits++;
    }

    std::cout << "Run " << r.run
              << " | Hits/event = " << r.hits << " +- " << r.hits_err
              << " | MBD Rate = " << r.rate
              << " | Corr. factor = " << one_plus_lambda
              << " | Hits_corr = " << corr_hits
              << " | Temp = " << T
              << std::endl;

    // send a CSV version to file
    csvout << r.run << ","
           << std::fixed << std::setprecision(0) << r.ts << ","
           << std::setprecision(6)
           << r.hits << ","
           << r.hits_err << ","
           << r.rate << ","
           << one_plus_lambda << ","
           << corr_hits << ","
           << T << "\n";
  }
  csvout.close();

  double mean_corr_hits = (n_corr_hits > 0) ? sum_corr_hits / n_corr_hits : 0;
  if (n_temp > 0)
  {
    mean_temp = sum_temp / n_temp;
    std::cout << "mean_temp is " << mean_temp << std::endl;
  }

  // y range of the hits per event plots: all runs, uncorrected and corrected
  double ymin = 1e30;
  double ymax = -1e30;
  for (TGraph *g : {(TGraph *) gr_hits_time, (TGraph *) gr_hits_time_corr})
  {
    for (int i = 0; i < g->GetN(); i++)
    {
      ymin = std::min(ymin, g->GetY()[i]);
      ymax = std::max(ymax, g->GetY()[i]);
    }
  }
  double span = std::max(ymax - ymin, 0.02 * ymax);
  ymin -= 0.3 * span;
  ymax += 0.5 * span;  // space for the legend

  // 3) hits per event vs MBD rate, before and after the pile-up correction
  TCanvas *c_hits_rate = new TCanvas("c_hits_rate", "Uncorrected hits/event vs MBD raw rate", 900, 700);
  gr_hits_rate->SetTitle("Uncorrected hits/event vs MBD raw rate;MBD raw rate [Hz];hits/event (uncorrected)");
  gr_hits_rate->SetMarkerStyle(20);
  gr_hits_rate->SetMarkerSize(0.9);
  gr_hits_rate->SetMarkerColor(kBlue + 1);
  gr_hits_rate->Draw("AP");  // "A" = axes, "P" = points
  if (pileup != "pol3") gr_hits_rate->GetFunction("f_pileup")->SetBit(TF1::kNotDraw);  // fit only drawn when used
  TLatex *lab_fit = new TLatex(0.15, 0.85, std::format("pol3 fit: f(0) = {:.4f}, #chi^{{2}}/ndf = {:.1f}/{}",
                                                       f_pileup->Eval(0), f_pileup->GetChisquare(), f_pileup->GetNDF()).c_str());
  lab_fit->SetNDC(); lab_fit->SetTextFont(42); lab_fit->SetTextSize(0.035);
  if (pileup == "pol3") lab_fit->Draw();
  c_hits_rate->SaveAs((figPre + "_hits_vs_mbdrate.pdf").c_str());

  TCanvas *c_corrhits_rate = new TCanvas("c_corrhits_rate", "Corrected hits/event vs MBD raw rate", 900, 700);
  gr_corr_hits_rate->SetTitle("Corrected hits/event vs MBD raw rate;MBD raw rate [Hz];hits/event");
  gr_corr_hits_rate->SetMarkerStyle(20);
  gr_corr_hits_rate->SetMarkerSize(0.9);
  gr_corr_hits_rate->SetMarkerColor(kBlue + 1);
  gr_corr_hits_rate->Draw("AP");
  c_corrhits_rate->SaveAs((figPre + "_corrhits_vs_mbdrate.pdf").c_str());

  //  4) 1D distributions of the hits per event over the runs
  double hlo = ymin;
  double hhi = ymin + (ymax - ymin) / 1.5 * 1.3;
  TH1F *h_hits_uncorr = new TH1F("h_hits_uncorr",
                                 "Uncorrected hits/event;hits/event;counts", nbins, hlo, hhi);
  TH1F *h_hits_corr = new TH1F("h_hits_corr",
                               "Pile-up corrected hits/event;hits/event;counts", nbins, hlo, hhi);

  for (int i = 0; i < gr_hits_time->GetN(); ++i)
  {
    double y = gr_hits_time->GetY()[i];
    if (std::isfinite(y) && y > 0) h_hits_uncorr->Fill(y);
  }
  for (int i = 0; i < gr_hits_time_corr->GetN(); ++i)
  {
    double y = gr_hits_time_corr->GetY()[i];
    if (std::isfinite(y) && y > 0) h_hits_corr->Fill(y);
  }

  // Overlay for easy shape comparison
  TCanvas *c5 = new TCanvas("c5", "c5", 800, 600);
  h_hits_uncorr->SetMarkerColor(kBlack);
  h_hits_uncorr->SetMarkerStyle(21);
  h_hits_uncorr->SetMarkerSize(1.2);
  h_hits_corr->SetMarkerColor(kBlue);
  h_hits_corr->SetMarkerSize(1.2);
  h_hits_corr->SetMarkerStyle(21);

  h_hits_uncorr->SetTitle("Hits/event distributions;hits/event;Number of runs");
  h_hits_uncorr->SetMaximum(1.2 * std::max(h_hits_uncorr->GetMaximum(), h_hits_corr->GetMaximum()));
  h_hits_uncorr->Draw("p");
  h_hits_corr->Draw("p same");

  TLegend *legDis = new TLegend(0.60, 0.75, 0.90, 0.90);
  legDis->SetBorderSize(0); legDis->SetFillStyle(0); legDis->SetTextFont(42);
  legDis->AddEntry(h_hits_uncorr, "Not pile-up corrected", "p");
  legDis->AddEntry(h_hits_corr, "Pile-up corrected", "p");
  legDis->Draw();
  c5->SaveAs((figPre + "_hits_event_distributions.pdf").c_str());

  std::cout << "[Uncorr] mean=" << h_hits_uncorr->GetMean()
            << "  RMS=" << h_hits_uncorr->GetRMS() << std::endl;
  std::cout << "[Corr]   mean=" << h_hits_corr->GetMean()
            << "  RMS=" << h_hits_corr->GetRMS() << std::endl;

  // pile-up corrected distribution with a Gaussian fit, RMS/mean and sigma/mu
  TCanvas *c1 = new TCanvas("c1", "c1", 600, 600);
  TH1F *h = (TH1F *) h_hits_corr->Clone("h_hits_corr_dis");
  h->SetTitle(";Hits/event;Number of runs");
  h->SetMarkerStyle(21);
  h->SetMarkerSize(1.0);
  h->Draw("E1");
  // Gaussian fit of the peak: Poisson likelihood (few runs per bin), started at the highest bin with the RMS
  // as width, first in peak +- 1.5 RMS, then refitted in mean +- 2 sigma of the first fit
  double peak = h->GetBinCenter(h->GetMaximumBin());
  double rms = h->GetRMS();
  TF1 *fit = new TF1("f_gaus", "gaus", peak - 1.5 * rms, peak + 1.5 * rms);
  fit->SetParameters(h->GetMaximum(), peak, 0.5 * rms);
  fit->SetParLimits(2, h->GetBinWidth(1), 3 * rms);
  fit->SetLineColor(kRed);
  h->Fit(fit, "QLR0");
  fit->SetRange(fit->GetParameter(1) - 2 * fit->GetParameter(2), fit->GetParameter(1) + 2 * fit->GetParameter(2));
  h->Fit(fit, "QLR");

  double h_cv = h->GetRMS() / h->GetMean();  // RMS/mean
  double f_cv = fit->GetParameter(2) / fit->GetParameter(1);  // sigma/mu

  TPaveText *pt = new TPaveText(0.58, 0.72, 0.88, 0.89, "NDC");
  pt->SetFillColor(0); pt->SetBorderSize(0); pt->SetTextSize(0.035);
  pt->AddText(std::format("RMS/mean (hist) = {:.2f}%", 100.0 * h_cv).c_str());
  pt->AddText(std::format("#sigma/#mu (fit) = {:.2f}%", 100.0 * f_cv).c_str());
  pt->Draw();
  c1->SaveAs((figPre + "_hit_rate_allRuns.pdf").c_str());

  // 5) hits per event vs run time, with the temperature scaled to the mean corrected hits per event
  TGraphErrors *gr_temp_time_scaled = (TGraphErrors *) gr_temp_time->Clone("gr_temp_time_scaled");
  for (int i = 0; i < gr_temp_time_scaled->GetN(); ++i)
  {
    if (mean_temp > 0.0)
    {
      const double T_scaled = (gr_temp_time->GetY()[i] / mean_temp) * mean_corr_hits;
      gr_temp_time_scaled->SetPoint(i, gr_temp_time->GetX()[i], T_scaled);
    }
  }
  gr_temp_time_scaled->SetLineColor(kRed);
  gr_temp_time_scaled->SetLineWidth(2);

  TGraph *gr_mean_corr_hits = new TGraph();
  gr_mean_corr_hits->SetPoint(0, 0, mean_corr_hits);
  gr_mean_corr_hits->SetPoint(1, 1e10, mean_corr_hits);
  gr_mean_corr_hits->SetLineColor(kBlue);

  TGraph *gr_cosmic_times = new TGraph();
  for (size_t ir = 0; ir < runNumbers_cosmic.size(); ir++)
  {
    gr_cosmic_times->SetPoint(ir * 2, timestamps_cosmic[ir], 10000 * (ir % 2));
    gr_cosmic_times->SetPoint(ir * 2 + 1, timestamps_cosmic[ir], 10000 * ((ir + 1) % 2));
  }

  double xmi = *std::min_element(gr_hits_time->GetX(), gr_hits_time->GetX() + gr_hits_time->GetN());
  double xma = *std::max_element(gr_hits_time->GetX(), gr_hits_time->GetX() + gr_hits_time->GetN());

  TCanvas *c2 = new TCanvas("c2", "c2", 2000, 600);
  gPad->SetLeftMargin(0.12);
  timeFrame("h_frame", xmi, xma, ymin, ymax, "mean hits / event / run")->Draw("axis");

  gr_hits_time->SetMarkerStyle(20);
  gr_hits_time->SetMarkerSize(1.0);
  gr_hits_time->Draw("p");

  gr_hits_time_corr->SetMarkerStyle(33);
  gr_hits_time_corr->SetMarkerColor(kBlue);
  gr_hits_time_corr->SetLineColor(kBlue);
  gr_hits_time_corr->Draw("p same");

  /*
  gr_cosmic_times->Draw("l");
  gr_cosmic_times->SetLineStyle(7);
  gr_cosmic_times->SetLineWidth(2);
  gr_cosmic_times->SetLineColor(kRed);
  */
  gr_mean_corr_hits->Draw("l");
  gr_temp_time_scaled->Draw("l same");

  drawLabels(dataset, detLabel);

  TLegend *leg = new TLegend(0.65, 0.70, 0.90, 0.88);
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->SetTextFont(42);
  leg->SetTextSize(0.04);
  leg->AddEntry(gr_hits_time, "Not pile-up corrected", "p");
  leg->AddEntry(gr_hits_time_corr, "Pile-up corrected", "p");
  leg->AddEntry(gr_temp_time_scaled, "Temperature (scaled)", "l");
  leg->Draw();
  c2->SaveAs((figPre + "_hit_rate_vs_time.pdf").c_str());

  // 6) comparison of the two calibration steps vs run time (both with the same pile-up correction)
  TCanvas *c_comp = nullptr;
  if (!anaFile_step1.empty())
  {
    std::vector<RunHits> rh1 = readRunHits(anaFile_step1);
    std::string tag1 = gSystem->BaseName(anaFile_step1.c_str());
    if (tag1.rfind("anaOut_", 0) == 0) tag1 = tag1.substr(7);
    if (tag1.size() > 5 && tag1.substr(tag1.size() - 5) == ".root") tag1 = tag1.substr(0, tag1.size() - 5);

    TGraphErrors *gr_step1 = new TGraphErrors();
    for (const RunHits &r : rh1)
    {
      if (r.rate <= 0) continue;
      float c = corrPileup(r.hits, r.rate, f_pileup, pileup, nBunches);
      int ip = gr_step1->GetN();
      gr_step1->SetPoint(ip, r.ts, c);
      gr_step1->SetPointError(ip, 0, r.hits_err * c / r.hits);
      ymin = std::min(ymin, c - 0.3 * span);
      ymax = std::max(ymax, c + 0.5 * span);
    }

    c_comp = new TCanvas("c_comp", "c_comp", 2000, 600);
    gPad->SetLeftMargin(0.12);
    timeFrame("h_frame_comp", xmi, xma, ymin, ymax, "Hits/Event")->Draw("axis");
    gr_step1->SetMarkerStyle(20);
    gr_step1->SetMarkerColor(kBlack);
    gr_step1->Draw("p");
    TGraphErrors *gr_step2 = (TGraphErrors *) gr_hits_time_corr->Clone("gr_step2");
    gr_step2->SetMarkerStyle(20);
    gr_step2->SetMarkerColor(kBlue);
    gr_step2->Draw("p same");
    gr_temp_time_scaled->Draw("l same");
    drawLabels(dataset, detLabel);

    TLegend *legc = new TLegend(0.65, 0.70, 0.90, 0.88);
    legc->SetBorderSize(0); legc->SetFillStyle(0); legc->SetTextFont(42); legc->SetTextSize(0.04);
    legc->AddEntry(gr_step1, tag1.c_str(), "p");
    legc->AddEntry(gr_step2, tag.c_str(), "p");
    legc->AddEntry(gr_temp_time_scaled, "Temperature (scaled)", "l");
    legc->Draw();
    c_comp->SaveAs((figPre + "_vs_" + tag1 + "_hit_rate_vs_time.pdf").c_str());
  }

  ///////////////////////
  // temp plots
  /////////////////////////

  TFile *fin = TFile::Open(anaFile.c_str(), "READ");
  TProfile2D *pr2d_temp = (TProfile2D *) fin->Get("pr2d_temp");

  TCanvas *c3 = new TCanvas("c3", "c3", 600, 600);
  pr2d_temp->Draw("COLZ");
  pr2d_temp->SetXTitle((detLabel + " #it{#eta}^{i}").c_str());
  pr2d_temp->SetYTitle((detLabel + " #it{#phi}^{i}").c_str());
  gPad->SetRightMargin(0.15);
  c3->SaveAs((figPre + "_temp_map.pdf").c_str());

  TCanvas *c4 = new TCanvas("c4", "c4", 2000, 600);
  gPad->SetLeftMargin(0.12);
  double tmin = 1e30;
  double tmax = -1e30;
  for (int i = 0; i < gr_temp_time->GetN(); i++)
  {
    tmin = std::min(tmin, gr_temp_time->GetY()[i]);
    tmax = std::max(tmax, gr_temp_time->GetY()[i]);
  }
  if (gr_temp_time->GetN() > 0)
  {
    timeFrame("h_frame2", xmi, xma, tmin - 1, tmax + 2, std::format("mean {} temperature / run [^{{o}}C]", detLabel).c_str())->Draw("axis");
    gr_temp_time->SetMarkerStyle(21);
    gr_temp_time->SetMarkerSize(1.0);
    gr_temp_time->Draw("p");
    drawLabels(dataset, detLabel);
    c4->SaveAs((figPre + "_mean_temp.pdf").c_str());
  }

  std::string outRoot = "output/plotHits_" + dataset + "_" + tag + ".root";
  TFile *fout = new TFile(outRoot.c_str(), "RECREATE");
  c2->Write();
  if (c_comp) c_comp->Write();
  h_hits_uncorr->Write();
  h_hits_corr->Write();
  c5->Write();
  c1->Write();
  c_hits_rate->Write();
  c_corrhits_rate->Write();
  f_pileup->Write();
  c3->Write();
  c4->Write();
  fout->Close();
  std::cout << "[plot_hits] wrote " << outRoot << " and " << figPre << "_*.pdf" << std::endl;
}

// pile-up correction chosen with pileup: "lambda" (1 + lambda), "pol3" (fit) or "none"
float corrPileup(float hits_per_event, double rate, TF1 *f_pileup, const std::string &pileup, double nBunches)
{
  if (pileup == "pol3") return corrPileupFit(hits_per_event, rate, f_pileup);
  if (pileup == "lambda") return corrDouble(hits_per_event, rate, nBunches);
  return hits_per_event;
}

// pile-up correction from the pol3 fit of hits/event vs MBD rate: scaled to zero rate (no pile-up)
float corrPileupFit(float hits_per_event, double rate, TF1 *f_pileup)
{
  return hits_per_event * f_pileup->Eval(0) / f_pileup->Eval(rate);
}

// pile-up correction 1 + lambda from the probability of more than one collision per bunch crossing
// (nBunches filled bunches, 78 kHz revolution frequency)
float corrDouble(float hits_per_event, double rate, double nBunches)
{
  double prob1orMore = rate / 78.e3 / nBunches;
  double lambda = -1.0 * std::log(1 - prob1orMore);
  // labda = prob of a single collision
  // p_double = prob of an additional collsion which = lambda
  // x = true probability of a hit per a single mb event
  // obs_x is the observed probability (prob per bunch crossing)
  // (x*events +  x*event*p_double ) / events = ob_x
  // x + x*p_double = ob_x
  // x = ob_x / (1+p_double)
  return hits_per_event / (1 + lambda);
}

void getInfo(const std::string &rateCsv)
{
  std::ifstream file(rateCsv);
  if (!file.is_open())
  {
    std::cerr << "[getInfo] ERROR: cannot open " << rateCsv << " (make it with get_mbdrate.py)\n";
    return;
  }

  auto trim = [](std::string &s)
  {
    size_t i = 0;
    while (i < s.size() && std::isspace((unsigned char) s[i])) ++i;
    s.erase(0, i);
    while (!s.empty() && std::isspace((unsigned char) s.back())) s.pop_back();
  };

  std::string line;
  size_t lineno = 0;
  while (std::getline(file, line))
  {
    ++lineno;
    if (line.empty()) continue;

    std::istringstream ss(line);
    std::string runStr;
    std::string tsStr;
    std::string rateStr;
    if (!std::getline(ss, runStr, ',')) continue;
    if (!std::getline(ss, tsStr, ',')) continue;
    std::getline(ss, rateStr, ',');

    trim(runStr); trim(tsStr); trim(rateStr);

    // Skip header (e.g., "runnumber,timestamp,rate_avg_value")
    if (runStr.empty() || !std::isdigit((unsigned char) runStr[0])) continue;

    int run = 0;
    try { run = std::stoi(runStr); }
    catch (...)
    {
      std::cerr << "[getInfo] bad run at line " << lineno << ": \"" << runStr << "\"\n";
      continue;
    }

    // timestamp "YYYY-MM-DD hh:mm:ss" -> epoch seconds
    std::array<int, 6> t6{};  // year, month, day, hour, minute, second
    if (parseTime(tsStr, t6) < 5)
    {
      std::cerr << "[getInfo] bad timestamp at line " << lineno << ": \"" << tsStr << "\"\n";
      continue;
    }
    TDatime t(t6[0], t6[1], t6[2], t6[3], t6[4], t6[5]);

    double rate = 0.0;
    if (!rateStr.empty())
    {
      try { rate = std::stod(rateStr); }
      catch (...) { rate = 0.0; }
    }

    runNumbers.push_back(run);
    timestamps.push_back(t.Convert());
    values.push_back(rate);
    hours.push_back(t6[3]);
  }

  if (runNumbers.empty())
  {
    std::cerr << "[getInfo] WARNING: parsed 0 usable rows from " << rateCsv << "\n";
  }
}

// cosmics run start times (optional, for the commented-out cosmics lines in the hits vs time plot)
void getCosmicInfo()
{
  std::ifstream file("cosmic_times.csv");
  if (!file.is_open()) return;

  std::string line;
  while (std::getline(file, line))
  {
    std::istringstream ss(line);
    std::string runNumberStr;
    std::string timestampStr;
    std::getline(ss, runNumberStr, ',');
    std::getline(ss, timestampStr, ',');
    if (runNumberStr.empty() || !std::isdigit((unsigned char) runNumberStr[0])) continue;

    std::array<int, 6> t6{};  // year, month, day, hour, minute, second
    if (parseTime(timestampStr, t6) != 6) continue;
    TDatime t(t6[0], t6[1], t6[2], t6[3], t6[4], t6[5]);

    runNumbers_cosmic.push_back(std::stoi(runNumberStr));
    timestamps_cosmic.push_back(t.Convert());
  }
}
