// Step 2: vertex scales and run scale of one run for the three bunch classes, from its MbdCentralityTree tree.
//   t0table:  the t0 offset table (all t0offset_<run>.csv rows under CentralityCalibrator::T0TableHeader())
//   reffile:  the reference run's output of this macro; empty for the reference run itself, which also gets the
//             NBD+Glauber fit (glauberfile: histogram hNpart) and ndivisions centrality divisions
//   triggerbit: GL1 min-bias bit (Run 24 Au+Au 10, Run 25 Au+Au 14)
//   root -b -q 'calibrate_run.C(54912, "run54912.root", "t0_offsets.csv", "", "calib/centrality_calib_54912.root", 10, "glauber/lime_auau_hists_nominal.root", 92)'
//   root -b -q 'calibrate_run.C(54913, "run54913.root", "t0_offsets.csv", "calib/centrality_calib_54912.root", "calib/centrality_calib_54913.root", 10)'
#include <centralitycalib/CentralityCalibrator.h>

#include <Rtypes.h> // defines R__LOAD_LIBRARY macro for clang-tidy
#include <TSystem.h>

#include <string>

R__LOAD_LIBRARY(libcentralitycalib.so)

int calibrate_run(int run, const std::string &treefile, const std::string &t0table, const std::string &reffile, const std::string &outfile,
                  int triggerbit = 14, const std::string &glauberfile = "", int ndivisions = 92)
{
  CentralityCalibrator cal;
  cal.SetTriggerBit(triggerbit);
  cal.SetGlauberFile(glauberfile);
  cal.SetNDivisions(ndivisions);
  // the exit code tells the job script whether the step worked
  const int rc = cal.Calibrate(run, treefile, t0table, reffile, outfile);
  gSystem->Exit(rc);
  return rc;
}
