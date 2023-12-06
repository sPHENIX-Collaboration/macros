#include <GlobalVariables.C>

#include "LiteCaloEval.h"
R__LOAD_LIBRARY(libLiteCaloEvalTowSlope.so)

/// typical parameters, despite the naming here, is modified histo, non modified, output file
void do_eta_fit2_ihcal(const char * reffile, const char * infile, const char * modfile)
{

  TStopwatch t;
  t.Start();

  gSystem->Load("libLiteCaloEvalTowSlope.so");


  LiteCaloEval reflce, modlce;

  reflce.CaloType(LiteCaloEval::HCALIN);
  modlce.CaloType(LiteCaloEval::HCALIN);

  reflce.Get_Histos(reffile); // get histos for modified files
  modlce.Get_Histos(infile,modfile); // get histos for non mod files

  /// we want to fit our nonmod/reference histogram and then
  /// adjust the modified histo according to such fit
  modlce.setFitMin(0.1); 
  modlce.setFitMax(0.4);

  /// Modlce holds histos with no energy modifications. What we then want to
  /// do is give the modified files, fit them, and compare with the nonmod
  modlce.FitRelativeShifts(&reflce,10); 
  
  t.Stop();
  t.Print();

}
