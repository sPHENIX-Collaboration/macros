#include <GlobalVariables.C>

#include "litecaloeval/LiteCaloEval.h"
R__LOAD_LIBRARY(libLiteCaloEvalTowSlope.so)

// regular file, modified file, output file
void rings_do_eta_fit2_ihcal(const char * infile, const char * modfile)
{
  TStopwatch t; 
  t.Start();

  gSystem->Load("libLiteCaloEvalTowSlope.so");
  LiteCaloEval modlce;

  //reflce.CaloType(LiteCaloEval::HCALIN);
  modlce.CaloType(LiteCaloEval::HCALIN);

  //reflce.Get_Histos(reffile);
  modlce.Get_Histos(infile,modfile);

  modlce.setFitMin(0.1);
  modlce.setFitMax(0.4);

  modlce.FitRelativeShifts(&modlce,110);
  
  t.Stop();
  t.Print();

}
