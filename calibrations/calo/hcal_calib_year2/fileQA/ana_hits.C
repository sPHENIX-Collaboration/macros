#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TProfile2D.h>
#include <TString.h>
#include <TSystem.h>

#include <cmath>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

// Hit counts, events and temperature of each beam run, and the 2D map of the hit rate per tower.
// Reads the spectra of the runs (out_<run>.root from condor_histmaker) and the temperature maps
// of the runs (<detector>_temp_<run>.root from makeTemp/loopCaloTemp.C).
//
// usage: root -l -b -q 'ana_hits.C("runList.txt", 0)'      OHCal
//        root -l -b -q 'ana_hits.C("runList.txt", 1)'      IHCal
//
// runlist_txt: beam run list, one run per line
// isIHcal:     0 OHCal, 1 IHCal
// spectraDir:  folder with out_<run>.root
// tempDir:     folder with the temperature maps, "" for ../makeTemp/temp_ohcal (temp_ihcal)
// out_root:    output file, "" for output/anaOut_ohcal.root (anaOut_ihcal.root)
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

static float getTempRun(int runnumber, const std::string &tempDir, const std::string &detDB, TProfile2D *pr2d_temp);

// sum of the energy of the entries of a spectrum from bin b1 to b2 (bin center x entries)
static double sumEnergy(TH1 *h, int b1, int b2)
{
  double sum = 0.0;
  for (int b = b1; b <= b2; ++b) sum += h->GetBinCenter(b) * h->GetBinContent(b);
  return sum;
}

// ------------------------------------------------------------

void ana_hits(const char *runlist_txt = "runList.txt", int isIHcal = 0,
              const std::string &spectraDir = "../condor_histmaker/out_run",
              std::string tempDir = "", std::string out_root = "",
              float eLow = -1, float eHigh = -1)
{
  // settings of the detector
  const std::string det = (isIHcal == 1) ? "ihcal" : "ohcal";
  const std::string detDB = (isIHcal == 1) ? "HCALIN" : "HCALOUT";  // name in the temperature files
  const std::string etaHist = (isIHcal == 1) ? "hcalin_eta_" : "hcalout_eta_";  // spectra of an eta ring
  const std::string towerHist = (isIHcal == 1) ? "hcal_in_eta_" : "hcal_out_eta_";  // spectra of a tower
  if (eLow < 0) eLow = (isIHcal == 1) ? 0.2 : 1.0;
  if (eHigh < 0) eHigh = (isIHcal == 1) ? 1.0 : 5.0;
  if (tempDir.empty()) tempDir = "../makeTemp/temp_" + det;
  if (out_root.empty()) out_root = "output/anaOut_" + det + ".root";
  gSystem->mkdir(gSystem->GetDirName(out_root.c_str()).Data(), true);  // folder of the output file
  // eta rings summed for the hits per run
  const int ieL = 1, ieH = 22;

  std::cout << "[ana_hits] " << det << ", hits with " << eLow << " < E < " << eHigh << " GeV" << std::endl;
  std::cout << "  spectra from      " << spectraDir << std::endl;
  std::cout << "  temperatures from " << tempDir << std::endl;
  std::cout << "  output            " << out_root << std::endl;

  // 0) read run list
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

  // 1) book histograms
  TH1F *h_hits = new TH1F("h_hits", Form("%s hit counts;run index;entries (%.1f < E < %.1f GeV)", det.c_str(), eLow, eHigh), N, 0, N);
  TH2F *h_hits_2d = new TH2F("h_hits_2d", Form("2D map of %s hit rate;#eta;#phi;hit rate / average of the eta ring", det.c_str()), 24, 0, 24, 64, 0, 64);

  TH1F *h_meanE_run = new TH1F("h_meanE_run", Form("%s mean energy above threshold per run;run index;mean E (E > %.1f GeV) [GeV]", det.c_str(), eLow), N, 0, N);
  TH2F *h_meanE_2d = new TH2F("h_meanE_2d", Form("%s mean energy above threshold (E > %.1f GeV);#eta;#phi;mean E [GeV]", det.c_str(), eLow), 24, 0, 24, 64, 0, 64);

  TH1F *h_events_run = new TH1F("h_events_run", "Events per run;run index;events", N, 0, N);
  TH1F *runNumberHist = new TH1F("runNumberHist", "Run numbers;run index;run number", N, 0, N);
  TH1F *h_temp_run = new TH1F("h_temp_run", Form("Avg %s tower temp per run;run index;T [^{o}C]", detDB.c_str()), N, 0, N);

  // same tower selection as the average temperature (10 < T < 32 C)
  TProfile2D *pr2d_temp = new TProfile2D("pr2d_temp",
                                         Form("%s tower temperature profile;eta;phi;T [^{o}C]", detDB.c_str()),
                                         24, 0, 24, 64, 0, 64, 10.0, 32.0, "s");

  std::vector<int> missingOut;   // out_<RUN>.root files missing
  std::vector<int> missingTemp;  // temperature files missing

  // hits of each tower summed over all runs, for the 2D map
  double towerHits[24][64] = {};
  // entries above the threshold eLow and their energy, for the mean energy
  double towerN[24][64] = {};
  double towerE[24][64] = {};

  // 2) per-run loop
  for (int i = 0; i < N; ++i)
  {
    const int run = runs[i];
    runNumberHist->SetBinContent(i + 1, run);

    std::cout << "Analyzing run index " << i << " with runnumber: " << run << std::endl;

    // open out_<RUN>.root
    TString infile = Form("%s/out_%d.root", spectraDir.c_str(), run);

    std::unique_ptr<TFile> fin;
    if (!gSystem->AccessPathName(infile)) fin.reset(TFile::Open(infile, "READ"));
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
        TH1 *h_eta = (TH1 *) fin->Get(Form("%s%d", etaHist.c_str(), ieta));
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
      if (TH1 *h_event = (TH1 *) fin->Get("h_event"))
      {
        h_events_run->SetBinContent(i + 1, h_event->GetEntries());
      }

      // hits of each tower, summed over the runs
      for (int ie = 0; ie < 24; ++ie)
      {
        for (int ip = 0; ip < 64; ++ip)
        {
          TH1 *hE = (TH1 *) fin->Get(Form("%s%d_phi_%d", towerHist.c_str(), ie, ip));
          if (!hE) continue;
          towerHits[ie][ip] += hE->Integral(hE->FindBin(eLow), hE->FindBin(eHigh));
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

// average tower temperature of a run from the map made by makeTemp/loopCaloTemp.C, 0 if there is none
static float getTempRun(int runnumber, const std::string &tempDir, const std::string &detDB, TProfile2D *pr2d_temp)
{
  TString tpath = Form("%s/%s_temp_%d.root", tempDir.c_str(), detDB.c_str(), runnumber);

  if (gSystem->AccessPathName(tpath))
  {
    return 0.0F;  // file missing
  }

  std::unique_ptr<TFile> f(TFile::Open(tpath, "READ"));
  if (!f || f->IsZombie())
  {
    return 0.0F;  // file couldn't open
  }

  TH2 *h = (TH2 *) f->Get(Form("h_%s_temp", detDB.c_str()));
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
