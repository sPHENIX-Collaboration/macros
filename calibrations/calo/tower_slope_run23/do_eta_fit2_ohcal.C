#include <GlobalVariables.C>

#include "litecaloeval/LiteCaloEval.h"
R__LOAD_LIBRARY(libLiteCaloEvalTowSlope.so)

// regular file, modified file, output file
void do_eta_fit2_ohcal(const char * reffile, const char * infile, const char * modfile)
{
  TStopwatch t; 
  t.Start();

  gSystem->Load("libLiteCaloEvalTowSlope.so");
  LiteCaloEval reflce, modlce;

  reflce.CaloType(LiteCaloEval::HCALOUT);
  modlce.CaloType(LiteCaloEval::HCALOUT);

  reflce.Get_Histos(reffile);
  modlce.Get_Histos(infile,modfile);

  modlce.setFitMin(0.12);
  modlce.setFitMax(0.7);

  modlce.FitRelativeShifts(&reflce,10);
  
  t.Stop();
  t.Print();

}
