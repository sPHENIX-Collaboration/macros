#include "LiteCaloEval.h"
#include <GlobalVariables.C>

R__LOAD_LIBRARY(libLiteCaloEvalTowSlope.so)

// regular file, modified file, output file
void drawSpecs(const char * infile)
{
  TStopwatch t; 
  t.Start();

  gSystem->Load("libLiteCaloEvalTowSlope.so");
  LiteCaloEval modlce;

  //reflce.CaloType(LiteCaloEval::CEMC);
  modlce.CaloType(LiteCaloEval::CEMC);

  /// we really dont need to retrieve histos here bc we arent using ref object
  /// reflce.Get_Histos(reffile);

  modlce.Get_Histos(infile,"");

  modlce.drawSpectraEmc(1);
  
  t.Stop();
  t.Print();

}
