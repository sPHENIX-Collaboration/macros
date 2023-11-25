#include <GlobalVariables.C>

#include <litecaloeval/LiteCaloEval.h>
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

  modlce.m_myminbin =  8;       // this really is min bin (no -1/+1 needed) 
                                //  but not sure if for summer 23 this should
                                // be 8 or 9 I think 8 according to Joey Clement
                                // on ~Nov 1, mattermost post
  modlce.m_mymaxbin =  95 + 1 ;  // this actually means maxbin is 1 less
  //  modlce.m_mymaxbin =  95+1;  // so that means maxbin is 95 which is
  //  really the max eta bin for emc, indexed 0 through 95


  modlce.setFitMin(0.12);
  modlce.setFitMax(0.7);

  modlce.FitRelativeShifts(&modlce,110);
  
  t.Stop();
  t.Print();

}
