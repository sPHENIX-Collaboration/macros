// Configure the compiled histogram postprocessor. No event DST is read.
#include <tpccalibrationqa/TpcCalibrationQA.h>

#include <Rtypes.h>

R__LOAD_LIBRARY(libtpccalibrationqa.so)

int Fun4All_TpcCalibrationQA(const char *manifest, const char *output,
                             const char *reference = "", double ptMin = .5, double ptMax = 1.,
                             int minEntries = 100, int phiGroup = 3, int etaGroup = 4,
                             const char *prefix = "h_TpcSeedsQA_", bool saveSlices = false,
                             double ratioFloor = .02, bool phiQpt = false,
                             const char *conditions = "[]", double rawTimeShift = 40.,
                             bool makePlots = true, const char *phgReferences = "",
                             bool fitBackground = false)
{
  TpcCalibrationQA qa;
  qa.set_pt_range(ptMin, ptMax);
  qa.set_min_entries(minEntries);
  qa.set_phi_group(phiGroup);
  qa.set_eta_group(etaGroup);
  qa.set_histogram_prefix(prefix);
  qa.set_save_slices(saveSlices);
  qa.set_ratio_floor(ratioFloor);
  qa.set_phi_qoverpt(phiQpt);
  qa.set_conditions_json(conditions);
  qa.set_raw_time_shift(rawTimeShift);
  qa.set_make_plots(makePlots);
  qa.set_phgarfield_table(phgReferences);
  qa.set_fit_background(fitBackground);
  return qa.run(manifest, output, reference);
}
