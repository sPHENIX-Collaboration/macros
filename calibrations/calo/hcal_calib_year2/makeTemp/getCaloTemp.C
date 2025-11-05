#include <TSystem.h>
#include <TString.h>
#include <TFile.h>
#include <TProfile2D.h>
#include <TTree.h>
#include <iostream>
#include <sstream>
#include <vector>

#include "TowerInfoDefs.h"
R__LOAD_LIBRARY(libcalo_io.so)

void getCaloTemp(int runnumber = 54263, TString detector = "HCALIN") {
  // Database info
  TString dbhost = "sphnxdaqdbreplica";
  TString dbname = "daq";

  // 1. Get run start time (brtimestamp)
  TString sql = Form("\"SELECT brtimestamp FROM run WHERE runnumber=%d;\"", runnumber);
  TString cmd = Form("psql -h %s -d %s -t -A -c %s", dbhost.Data(), dbname.Data(), sql.Data());
  TString runtime = gSystem->GetFromPipe(cmd.Data());

  runtime = runtime.Strip(TString::kBoth, '\n');
  if (runtime.Length() == 0) {
    std::cerr << "No runtime found for run " << runnumber << std::endl;
    return;
  }
  std::cout << "Run " << runnumber << " runtime: " << runtime << std::endl;

  // 2. Choose detector table and query closest heartbeat
  TString tablename, tempstring = "temp";
  int det = -1;
  if (detector == "CEMC") {
    tablename = "emcal_heartbeat";
    tempstring = "temp_sipm as temp, temp_pa";
  } else if (detector == "HCALIN") {
    tablename = "hcal_heartbeat";
    det = 1;
  } else if (detector == "HCALOUT") {
    tablename = "hcal_heartbeat";
    det = 0;
  } else {
    std::cerr << "Unknown detector: " << detector << std::endl;
    return;
  }

  // Find closest timestamp to run start
  sql = Form("\"SELECT time FROM %s ORDER BY ABS(EXTRACT(epoch FROM time) - EXTRACT(epoch FROM '%s'::timestamp)) LIMIT 1;\"",
             tablename.Data(), runtime.Data());
  cmd = Form("psql -h %s -d %s -t -A -c %s", dbhost.Data(), dbname.Data(), sql.Data());
  TString closest_time = gSystem->GetFromPipe(cmd.Data()).Strip(TString::kBoth, '\n');

  if (closest_time.Length() == 0) {
    std::cerr << "No heartbeat found near runtime" << std::endl;
    return;
  }
  std::cout << "Closest DB time: " << closest_time << std::endl;

  // 3. Get temperatures for that timestamp
  sql = Form("\"SELECT towerid, %s FROM %s WHERE time='%s'%s;\"",
             tempstring.Data(),
             tablename.Data(),
             closest_time.Data(),
             (det >= 0 ? Form(" AND detector=%d", det) : ""));
  cmd = Form("psql -h %s -d %s -t -A -F, -c %s", dbhost.Data(), dbname.Data(), sql.Data());
  TString results = gSystem->GetFromPipe(cmd.Data());

  if (results.Length() == 0) {
    std::cerr << "No temperature data found" << std::endl;
    return;
  }

  std::cout << "Raw temperature rows:\n" << results << std::endl;

  // 4. (Optional) parse results into histogram
  std::istringstream iss(results.Data());
  string line;
  TFile* fout = new TFile(Form("caloTemp_run%d.root", runnumber), "RECREATE");

  TProfile2D* htemp = nullptr;
  if (detector == "CEMC") htemp = new TProfile2D("h_cemc_temp", ";eta;phi", 96, 0, 96, 256, 0, 256);
  else htemp = new TProfile2D(Form("h_%s_temp", detector.Data()), ";eta;phi", 24, 0, 24, 64, 0, 64);

  while (std::getline(iss, line)) {
    if (line.empty()) continue;
    std::istringstream ls(line);
    std::string token;
    std::vector<std::string> parts;
    while (std::getline(ls, token, ',')) parts.push_back(token);

    if (parts.size() < 2) continue;
    int towerid = std::stoi(parts[0]);
    float temp = std::stof(parts[1]);

    int calo_key;
    if (detector == "CEMC") { calo_key = TowerInfoDefs::encode_emcal(towerid); } 
    else { calo_key = TowerInfoDefs::encode_hcal(towerid); }
    int etabin = TowerInfoDefs::getCaloTowerEtaBin(calo_key);
    int phibin = TowerInfoDefs::getCaloTowerPhiBin(calo_key);

    // For now, just fill with towerid as x
    htemp->Fill(etabin,phibin, temp);
  }

  htemp->Write();
  fout->Close();
  std::cout << "Histogram written to caloTemp_run" << runnumber << ".root" << std::endl;
}

