#include <GlobalVariables.C>

#include "/gpfs/mnt/gpfs02/sphenix/user/jfrantz/install/sphenix/include/litecaloeval/LiteCaloEval.h"
R__LOAD_LIBRARY(libLiteCaloEvalTowSlope.so)

// regular file, modified file, output file
void rings_do_eta_fit2_emc(const char * infile, const char * modfile)
{
  TStopwatch t; 
  t.Start();

  gSystem->Load("libLiteCaloEvalTowSlope.so");
  LiteCaloEval modlce;

  //reflce.CaloType(LiteCaloEval::CEMC);
  modlce.CaloType(LiteCaloEval::CEMC);

  /// we really dont need to retrieve histos here bc we arent using ref object
  /// reflce.Get_Histos(reffile);

  modlce.Get_Histos(infile,modfile);

  modlce.m_myminbin =  10;
  modlce.m_mymaxbin =  95;  // this actually means maxbin is 1 less
  //  modlce.m_mymaxbin =  95;  // this actually means maxbin is 1 less


  modlce.setFitMin(0.17);
  modlce.setFitMax(0.9);

  modlce.FitRelativeShifts(&modlce,110);
  
  t.Stop();
  t.Print();

}
