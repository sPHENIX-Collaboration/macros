// Step 3: the Centrality, CentralityScale and CentralityVertexScale CDB payload files of every run of a run list, under
// <outdir>/divs, <outdir>/scales and <outdir>/vertexscales, plus <outdir>/payload_list.csv (what each run got).
//   runlist: "run refrun calibrate" per line; calibrate 0 = the run gets the reference run's tables (scale 1)
//   calibdir: holds centrality_calib_<run>.root of the calibrated runs and of the reference runs (calibrate_run.C)
//   root -b -q 'make_payloads.C("runlists/run24auau.list", "calib", "t0_offsets.csv", "payloads")'
#include <centralitycalib/CentralityCalibrator.h>

#include <fstream>
#include <iostream>
#include <string>

R__LOAD_LIBRARY(libcentralitycalib.so)

int make_payloads(const std::string &runlist, const std::string &calibdir, const std::string &t0table, const std::string &outdir)
{
  std::ifstream in(runlist);
  if (!in.is_open())
  {
    std::cout << "cannot open " << runlist << std::endl;
    gSystem->Exit(1);
  }
  gSystem->mkdir(outdir.c_str(), true);
  std::ofstream log(outdir + "/payload_list.csv");
  log << "run,refrun,source" << std::endl;
  const char *source_names[] = {"own", "reference", "reference_default"};
  int count[3] = {0, 0, 0};
  int run;
  int refrun;
  int calibrate;
  while (in >> run >> refrun >> calibrate)
  {
    const std::string calibfile = calibrate ? calibdir + "/centrality_calib_" + std::to_string(run) + ".root" : "";
    const std::string reffile = calibdir + "/centrality_calib_" + std::to_string(refrun) + ".root";
    const int source = CentralityCalibrator::WritePayloads(run, calibfile, reffile, t0table, outdir);
    if (source < 0)
    {
      gSystem->Exit(1);
    }
    count[source]++;
    log << run << "," << refrun << "," << source_names[source] << std::endl;
  }
  std::cout << "payloads in " << outdir << ": " << count[0] << " runs with their own calibration, " << count[1]
            << " reference runs, " << count[2] << " with the reference run's tables" << std::endl;
  gSystem->Exit(0);
  return 0;
}
