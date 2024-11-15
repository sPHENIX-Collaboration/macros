#include <litecaloeval/LiteCaloEval.h>
R__LOAD_LIBRARY(libLiteCaloEvalTowSlope.so)

void doTscFit(){

  string hist_fname = "condor/combine_out/tsc_hcalout_comb.root";
  string fitoutfile = "fitout_hcalout.root";

  LiteCaloEval modlce;
  modlce.CaloType(LiteCaloEval::HCALOUT);
  modlce.Get_Histos(hist_fname.c_str(),fitoutfile.c_str());
  modlce.setFitMin(0.15);
  modlce.setFitMax(0.8);
  modlce.FitRelativeShifts(&modlce,110);

  hist_fname = "condor/combine_out/tsc_hcalin_comb.root";
  fitoutfile = "fitout_hcalin.root";

  LiteCaloEval modlce2;
  modlce2.CaloType(LiteCaloEval::HCALIN);
  modlce2.Get_Histos(hist_fname.c_str(),fitoutfile.c_str());
  modlce2.setFitMin(0.15);
  modlce2.setFitMax(0.8);
  modlce2.FitRelativeShifts(&modlce2,110);


}
