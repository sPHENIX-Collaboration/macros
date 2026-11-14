// getCosmicTemps.C

#include <TSystem.h>
#include <TString.h>

#include <ctime>
#include <format>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <cmath>

// --- run a psql command and capture output (no prompts), with retries ---
bool psql_run(const TString& dbhost,
	      const TString& dbname,
	      const TString& sql_quoted,      // e.g. "\"SELECT 1;\""
	      TString& out,
	      const char* flags = "-X -q -t -A -w",
	      int max_retries   = 3,
	      int backoff_ms    = 400)
{
  out.Clear();

  for (int attempt = 1; attempt <= max_retries; ++attempt) {
    Long64_t now = static_cast<Long64_t>(time(nullptr));
    TString tmp = std::format("/tmp/getCaloTemp_{}_{}_{}.txt",
			      gSystem->GetPid(), now, attempt).c_str();

    // stderr to /dev/null; keep output clean for scalar reads
    TString cmd = std::format("psql -h {} -d {} {} -c {} > {} 2>/dev/null",
                       dbhost.Data(), dbname.Data(), flags, sql_quoted.Data(), tmp.Data());

    int code = gSystem->Exec(cmd.Data());
    TString contents = gSystem->GetFromPipe(std::format("cat {}; rm -f {}", tmp.Data(), tmp.Data()).c_str());
    contents = contents.Strip(TString::kBoth, '\n');

    if (code == 0 && contents.Length() > 0 && !contents.Contains("ERROR")) {
      out = contents;
      return true;
    }

    if (attempt < max_retries) gSystem->Sleep(backoff_ms * attempt);
  }
  return false;
}

// fetch average temp (degC) of the towers with 10 < T < 32 C for HCALOUT/HCALIN at closest heartbeat to run start ---
float fetch_hcal_temp_degC(int runnumber,
                           const TString& detector = "HCALOUT",
                           int max_retries = 3,
                           int backoff_ms  = 400)
{
  const TString dbhost = "sphnxdaqdbreplica";
  const TString dbname = "daq";

 // std::cout << "Processing run " << runnumber << " ..." << std::endl;

  TString runtime;
  {
    TString sql = std::format("\"SELECT brtimestamp FROM run WHERE runnumber={};\"", runnumber);
    if (!psql_run(dbhost, dbname, sql, runtime, "-X -q -t -A -w", max_retries, backoff_ms)) {
      std::cout << "Run " << runnumber << ": FAILED to fetch brtimestamp" << std::endl;
      return std::numeric_limits<float>::quiet_NaN();
    }
    runtime = runtime.Strip(TString::kBoth, '\n');
  }

  int det = -1;
  if (detector.EqualTo("HCALOUT", TString::kIgnoreCase)) det = 0;
  else if (detector.EqualTo("HCALIN",  TString::kIgnoreCase)) det = 1;
  else {
    return std::numeric_limits<float>::quiet_NaN();
  }

  TString closest_time;
  {
    // closest heartbeat that has towers with 10 < T < 32 C (some heartbeats have no valid temperature at all).
    // look within one day of the run start first: the search over the whole table takes ~15 s per run
    TString sql = std::format("\"SELECT time FROM hcal_heartbeat "
                       "WHERE detector={} AND temp > 10 AND temp < 32 "
                       "AND time BETWEEN '{}'::timestamp - interval '1 day' AND '{}'::timestamp + interval '1 day' "
                       "ORDER BY ABS(EXTRACT(epoch FROM time) - EXTRACT(epoch FROM '{}'::timestamp)) "
                       "LIMIT 1;\"",
                       det, runtime.Data(), runtime.Data(), runtime.Data());
    TString sql_all = std::format("\"SELECT time FROM hcal_heartbeat "
                       "WHERE detector={} AND temp > 10 AND temp < 32 "
                       "ORDER BY ABS(EXTRACT(epoch FROM time) - EXTRACT(epoch FROM '{}'::timestamp)) "
                       "LIMIT 1;\"",
                       det, runtime.Data());
    if (!psql_run(dbhost, dbname, sql, closest_time, "-X -q -t -A -w", 1, backoff_ms) &&
        !psql_run(dbhost, dbname, sql_all, closest_time, "-X -q -t -A -w", max_retries, backoff_ms)) {
      std::cout << "Run " << runnumber << ": FAILED to fetch closest heartbeat time" << std::endl;
      return std::numeric_limits<float>::quiet_NaN();
    }
    closest_time = closest_time.Strip(TString::kBoth, '\n');
  }

  TString avg_temp_str;
  {
    // towers with 10 < T < 32 C, the same selection as for the beam runs (fileQA/ana_hits.C):
    // dead towers report unphysical temperatures and must not enter the average
    TString sql = std::format("\"SELECT AVG(temp) "
                       "FROM hcal_heartbeat "
                       "WHERE detector={} AND time='{}' "
                       "  AND temp > 10 AND temp < 32;\"",
                       det, closest_time.Data());
    if (!psql_run(dbhost, dbname, sql, avg_temp_str, "-X -q -t -A -w", max_retries, backoff_ms)) {
      return std::numeric_limits<float>::quiet_NaN();
    }
    avg_temp_str = avg_temp_str.Strip(TString::kBoth, '\n');
  }

  float tdeg = std::numeric_limits<float>::quiet_NaN();
  try { tdeg = std::stof(std::string(avg_temp_str.Data())); }
  catch (...) {std::cout << "caught unknown exception from std::stof" << std::endl;}

  return tdeg;
}

// Temperature of a cosmics calibration made from several runs (firstRun to lastRun): the temperature at the
// start of each cosmics run in the range (fetch_hcal_temp_degC), averaged with the length of the run as weight.
//   root -l getCosmicTemps.C
//   root [1] fetch_hcal_temp_degC_period(82716, 82744, "HCALIN")
float fetch_hcal_temp_degC_period(int firstRun, int lastRun,
                                  const TString& detector = "HCALOUT",
                                  int max_retries = 3,
                                  int backoff_ms  = 400)
{
  // cosmics runs of the range and their length in seconds
  TString rows;
  TString sql = std::format("\"SELECT runnumber, EXTRACT(epoch FROM ertimestamp - brtimestamp) FROM run "
                     "WHERE runnumber BETWEEN {} AND {} AND runtype='cosmics' AND ertimestamp IS NOT NULL "
                     "ORDER BY runnumber;\"",
                     firstRun, lastRun);
  if (!psql_run("sphnxdaqdbreplica", "daq", sql, rows, "-X -q -t -A -w -F,", max_retries, backoff_ms)) {
    std::cout << "Runs " << firstRun << "-" << lastRun << ": FAILED to fetch the cosmics runs" << std::endl;
    return std::numeric_limits<float>::quiet_NaN();
  }

  double sum = 0;
  double sumw = 0;
  int nruns = 0;
  std::istringstream iss(rows.Data());
  std::string line;
  while (std::getline(iss, line)) {
    int run = 0;
    double length = 0;
    if (sscanf(line.c_str(), "%d,%lf", &run, &length) != 2 || length <= 0) continue;
    float t = fetch_hcal_temp_degC(run, detector, max_retries, backoff_ms);
    if (std::isnan(t)) continue;
    sum += t * length;
    sumw += length;
    nruns++;
  }
  if (sumw <= 0) return std::numeric_limits<float>::quiet_NaN();
  std::cout << detector << " cosmics runs " << firstRun << "-" << lastRun << ": " << nruns << " runs, "
            << sumw / 3600. << " hours, average temperature " << sum / sumw << " C" << std::endl;
  return sum / sumw;
}

// (Optional) quick driver to test a single runlist directly with this file:
// root -l -q 'getCosmicTemps.C("cos_runs_pp_2024.txt")'
void getCosmicTemps(const char* runlist_path = "cos_runs_pp_2024.txt",
                    const char* detector     = "HCALOUT")
{
  std::ifstream fin(runlist_path);
  if (!fin) { std::cerr << "Could not open " << runlist_path << std::endl; return; }
  std::string line;
  while (std::getline(fin, line)) {
    if (line.empty() || line[0]=='#') continue;
    int run = 0; try { run = std::stoi(line); } catch (...) { continue; }
    (void)fetch_hcal_temp_degC(run, detector);
   // std::cout << "--------------------------------------" << std::endl;
  }
}

