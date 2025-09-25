#include <TSystem.h>
#include <TString.h>
#include <TFile.h>
#include <TProfile2D.h>
#include <TTree.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <ctime>
#include "TowerInfoDefs.h"
R__LOAD_LIBRARY(libcalo_io.so)

// Helper: run a psql command, capture output, retry with backoff.
// flags should be like "-t -A" (scalar) or "-t -A -F," (CSV)
static bool psql_run(const TString& dbhost,
                     const TString& dbname,
                     const TString& sql,   // already fully quoted: e.g. "\"SELECT ...;\""
                     TString& out,
                     const char* flags,
                     int max_retries = 5,
                     int backoff_ms  = 500)
{
  out.Clear();
  for (int attempt = 1; attempt <= max_retries; ++attempt) {
    // temp file unique per attempt
    Long64_t now = static_cast<Long64_t>(time(nullptr));
    TString tmp = Form("/tmp/getCaloTemp_%d_%lld_%d.txt",
                   gSystem->GetPid(), now, attempt);

    // Build command; stderr suppressed (2>/dev/null) so output is clean
    TString cmd = Form("psql -h %s -d %s %s -c %s > %s 2>/dev/null",
                       dbhost.Data(), dbname.Data(), flags, sql.Data(), tmp.Data());

    int code = gSystem->Exec(cmd.Data());
    TString contents = gSystem->GetFromPipe(Form("cat %s; rm -f %s", tmp.Data(), tmp.Data()));
    contents = contents.Strip(TString::kBoth, '\n');

    // Success if exit code is 0 and we got some content (and not an obvious error string)
    if (code == 0 && contents.Length() > 0 && !contents.Contains("ERROR")) {
      out = contents;
      return true;
    }

    // Backoff before next attempt
    if (attempt < max_retries) gSystem->Sleep(backoff_ms * attempt);
  }
  return false;
}

void getCaloTemp(int runnumber = 54263, TString detector = "HCALIN",
                 int max_retries = 5, int backoff_ms = 500)
{
  // Database info
  const TString dbhost = "sphnxdaqdbreplica";
  const TString dbname = "daq";

  // --- 1) Get run start time (brtimestamp)
  TString sql = Form("\"SELECT brtimestamp FROM run WHERE runnumber=%d;\"", runnumber);

  TString runtime;
  if (!psql_run(dbhost, dbname, sql, runtime, "-t -A", max_retries, backoff_ms)) {
    std::cerr << "[getCaloTemp] Run " << runnumber
              << ": failed to fetch brtimestamp after retries.\n";
    return;
  }
  runtime = runtime.Strip(TString::kBoth, '\n');
  std::cout << "Run " << runnumber << " runtime: " << runtime << std::endl;

  // --- 2) Choose detector table & columns
  TString tablename, tempstring = "temp";
  int det = -1;
  if (detector == "CEMC") {
    tablename = "emcal_heartbeat";
    // two columns exist; alias first one as 'temp' so our parser keeps using column #2 overall
    tempstring = "temp_sipm as temp, temp_pa";
  } else if (detector == "HCALIN") {
    tablename = "hcal_heartbeat";
    det = 1;
  } else if (detector == "HCALOUT") {
    tablename = "hcal_heartbeat";
    det = 0;
  } else {
    std::cerr << "[getCaloTemp] Unknown detector: " << detector << std::endl;
    return;
  }

  // --- 3) Find closest timestamp to run start (retry)
  
  // --- 3) Find closest timestamp to run start (fast + filtered)
  
  sql = Form("\"SELECT time FROM %s "
           "WHERE %s "
           "ORDER BY ABS(EXTRACT(epoch FROM time) - EXTRACT(epoch FROM '%s'::timestamp)) "
           "LIMIT 1;\"",
           tablename.Data(),
           (det >= 0 ? Form("detector=%d", det) : "TRUE"),
           runtime.Data());
  
/*
  sql = Form("\"SELECT time FROM %s " 
             "ORDER BY ABS(EXTRACT(epoch FROM time) - EXTRACT(epoch FROM '%s'::timestamp)) "
             "LIMIT 1;\"", tablename.Data(), runtime.Data()); 
*/ 

  TString closest_time;
  if (!psql_run(dbhost, dbname, sql, closest_time, "-t -A", max_retries, backoff_ms)) {
    std::cerr << "[getCaloTemp] Run " << runnumber
              << ": failed to fetch closest heartbeat time after retries.\n";
    return;
  }
  closest_time = closest_time.Strip(TString::kBoth, '\n');
  std::cout << "Closest DB time: " << closest_time << std::endl;

  // --- 4) Get temperatures (CSV) for that timestamp (retry)
  sql = Form("\"SELECT towerid, %s FROM %s WHERE time='%s'%s;\"",
             tempstring.Data(),
             tablename.Data(),
             closest_time.Data(),
             (det >= 0 ? Form(" AND detector=%d", det) : ""));

  TString csvrows;
  if (!psql_run(dbhost, dbname, sql, csvrows, "-t -A -F,", max_retries, backoff_ms)) {
    std::cerr << "[getCaloTemp] Run " << runnumber
              << ": failed to fetch temperature rows after retries.\n";
    return;
  }

  if (csvrows.Length() == 0) {
    std::cerr << "[getCaloTemp] Run " << runnumber << ": No temperature data returned.\n";
    return;
  }
  // std::cout << "Raw temperature rows:\n" << csvrows << std::endl;

  // --- 5) Parse and fill
  std::istringstream iss(csvrows.Data());
  std::string line;
  
  TString outdir;
  if (detector == "HCALOUT") {
    outdir = "temp_ohcal";
  } else if (detector == "HCALIN") {
    outdir = "temp_ihcal";
  } else if (detector == "CEMC") {
    outdir = "temp_emcal";
  } else {
    outdir = "temp_other";
  }

  gSystem->mkdir(outdir, kTRUE);

  // Build full output filename
  TString outname = Form("%s/%s_temp_%d.root",
                       outdir.Data(),
                       detector.Data(),
                       runnumber);

  TFile* fout = new TFile(outname, "RECREATE");

  TProfile2D* htemp = nullptr;
  if (detector == "CEMC")
    htemp = new TProfile2D("h_cemc_temp", ";eta;phi", 96, 0, 96, 256, 0, 256);
  else
    htemp = new TProfile2D(Form("h_%s_temp", detector.Data()), ";eta;phi", 24, 0, 24, 64, 0, 64);

  while (std::getline(iss, line)) {
    if (line.empty()) continue;
    std::istringstream ls(line);
    std::string token;
    std::vector<std::string> parts;
    while (std::getline(ls, token, ',')) parts.push_back(token);

    // Expect: towerid, temp[, temp_pa]
    if (parts.size() < 2) continue;

    int towerid = 0;
    try { towerid = std::stoi(parts[0]); } catch (...) { continue; }

    float temp = 0.0f;
    try { temp = std::stof(parts[1]); } catch (...) { continue; }

    int calo_key;
    if (detector == "CEMC") { calo_key = TowerInfoDefs::encode_emcal(towerid); }
    else                    { calo_key = TowerInfoDefs::encode_hcal(towerid); }

    const int etabin = TowerInfoDefs::getCaloTowerEtaBin(calo_key);
    const int phibin = TowerInfoDefs::getCaloTowerPhiBin(calo_key);

    htemp->Fill(etabin, phibin, temp);
  }

  htemp->Write();
  fout->Close();
  std::cout << "Histogram written to " << outname << std::endl;
}

