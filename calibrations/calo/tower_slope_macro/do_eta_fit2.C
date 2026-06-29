
//double fitf(Double_t * f, Double_t *p);
//TGraph * grff = 0;
#include <GlobalVariables.C>

#include <litecaloeval/LiteCaloEval.h>

#include <Rtypes.h>
#include <TSystem.h>

R__LOAD_LIBRARY(libLiteCaloEvalTowSlope.so)

void do_eta_fit2(const std::string &reffile, const std::string &infile, const std::string &modfile)
{
  gSystem->Load("libLiteCaloEvalTowSlope.so");
  LiteCaloEval reflce;
  LiteCaloEval modlce;
  reflce.CaloType(LiteCaloEval::CEMC);
  modlce.CaloType(LiteCaloEval::CEMC);
  reflce.Get_Histos(reffile);
  modlce.Get_Histos(infile,modfile);
  modlce.setFitMin(0.12);
  modlce.setFitMax(0.7);
  modlce.FitRelativeShifts(&reflce,1);
  


}


