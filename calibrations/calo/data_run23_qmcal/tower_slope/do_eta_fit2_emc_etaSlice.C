#include <GlobalVariables.C>

#include "litecaloeval/LiteCaloEval.h"
R__LOAD_LIBRARY(libLiteCaloEvalTowSlope.so)

// File parameters - not modified histos, modified histos, output
void do_eta_fit2_emc_etaSlice(const char * reffile, const char * infile, const char * modfile)
{

  TStopwatch t;
  t.Start();


  gSystem->Load("libLiteCaloEvalTowSlope.so");

  /// create LCE objects, one for a mod set of histos (modlce) and other for non mod set of histos (reflce)
  LiteCaloEval reflce, modlce;


  ///set the calo type to run over
  reflce.CaloType(LiteCaloEval::CEMC);
  modlce.CaloType(LiteCaloEval::CEMC);


  /// get histograms for not mod histos
  reflce.Get_Histos(reffile);

  /// get histograms for mod histos
  modlce.Get_Histos(infile,modfile);


  modlce.m_myminbin =  11;
  modlce.m_mymaxbin =  95;  // this actually means maxbin is 1 less


  ///set the fit range
  modlce.setFitMin(0.12);
  modlce.setFitMax(0.7);


  /*Fit histos,obtain fit values. Second parameter determines smoothing/include phi loop (individual towers). 
    If set to e.g. 10 - do 1 smoothing and run over phi. 
    If set to 11, do one smoothing and dont do phi loop. Can do just 0 or 1. Which would be no smoothing, and 
    include phi if 0, and dont include if 1.
  */
  modlce.FitRelativeShifts(&reflce,1);
  
  t.Stop();
  t.Print();

}
