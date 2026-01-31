#include "getCaloTemp.C"

#include <format>
#include <fstream>
#include <set>

//how to run example:root -l -q 'loopCaloTemp.C("runList_pp_2024.txt","HCALOUT")'

void loopCaloTemp(const char* runListFile = "runList.txt",
                  const char* detector    = "HCALIN",
                  int max_retries         = 5,
                  int backoff_ms          = 500,
                  bool skip_if_exists     = true,
                  int passes              = 1)   // set >1 to automatically re-try failed runs
{

  TString outdir;

  if (TString(detector) == "HCALOUT") {
    outdir = "temp_ohcal";
  }
  else if (TString(detector) == "HCALIN") {
    outdir = "temp_ihcal";
  }
  else if (TString(detector) == "CEMC") {
    outdir = "temp_emcal";
  }
  else {
    outdir = "temp_other";
  }

  gSystem->mkdir(outdir, kTRUE);

  std::ifstream fin(runListFile);
  if (!fin.is_open()) {
    std::cerr << "Error: Could not open " << runListFile << std::endl;
    return;
  }

  std::vector<int> runs;
  for (int r; fin >> r;) runs.push_back(r);
  fin.close();

  std::cout << "Found " << runs.size() << " runs in " << runListFile << std::endl;

  std::set<int> pending(runs.begin(), runs.end());

  for (int pass = 1; pass <= std::max(1, passes) && !pending.empty(); ++pass) {
    std::cout << "\n=== Pass " << pass << " (" << pending.size() << " runs) ===\n";
    std::set<int> failures;

  for (int r : pending) {
  // Expected output file
  
    TString outname = std::format("{}/{}_temp_{}.root",
                             outdir.Data(),
                             TString(detector).Data(),
                             r);

  // Skip if already exists in outdir
  if (skip_if_exists && !gSystem->AccessPathName(outname.Data())) {
     std::cout << "[skip] " << outname << " already exists for run " << r << "\n";
     continue;
    }

  std::cout << "Processing run " << r << " ..." << std::endl;
  Long64_t before = gSystem->Now();
  getCaloTemp(r, detector, max_retries, backoff_ms);
  Long64_t after  = gSystem->Now();

  // ---- simple existence check ----
  if (!gSystem->AccessPathName(outname.Data())) {
    std::cout << "  -> OK (" << (after - before) << " ms)\n";
  } else {
    std::cerr << "  -> FAILED for run " << r << "\n";
    failures.insert(r);
  }
}


  // Write failure list for this pass
  std::ofstream flog(std::format("failed_runs_pass{}_{}.txt", pass, detector));
  for (int r : failures) flog << r << "\n";
  flog.close();
  std::cout << "Pass " << pass << " failures written to failed_runs_pass"
	    << pass << "_" << detector << ".txt" << std::endl;

  // Prepare for next pass
  pending = failures;
  }

  std::cout << "\nAll passes done." << std::endl;
}

