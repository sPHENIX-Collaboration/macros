// Step 1: the main-bunch MBD t0 offset of one run, from its MbdCentralityTree tree. Writes <outdir>/t0offset_<run>.csv
// (one row of the t0 offset table) and <outdir>/t0offset_<run>.root (the t0 histogram).
//   triggerbit: GL1 min-bias bit (Run 24 Au+Au 10, Run 25 Au+Au 14)
//   seedwindow: the main-bunch fit starts at the tallest bin within +-seedwindow ns (Run 24 Au+Au 11.5, whose
//               blocks 54424-54498 and 54935 are shifted by -5 / -10 ns; Run 25 Au+Au 3)
//   root -b -q 'fit_t0offset.C(54912, "run54912.root", "t0offset", 10, 11.5)'
#include <centralitycalib/CentralityCalibrator.h>

#include <Rtypes.h> // defines R__LOAD_LIBRARY macro for clang-tidy
#include <TSystem.h>

#include <string>

R__LOAD_LIBRARY(libcentralitycalib.so)

int fit_t0offset(int run, const std::string &treefile, const std::string &outdir, int triggerbit = 14, double seedwindow = 3.)
{
  CentralityCalibrator cal;
  cal.SetTriggerBit(triggerbit);
  cal.SetT0FitSeedWindow(seedwindow);
  // the exit code tells the job script whether the step worked
  const int rc = cal.FitT0Offset(run, treefile, outdir);
  gSystem->Exit(rc);
  return rc;
}
