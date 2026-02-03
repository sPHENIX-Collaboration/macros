#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4allraw/Fun4AllPrdfInputManager.h>
#include <fun4all/Fun4AllDstInputManager.h>
//#include <rawwaveformtowerbuilder/RawWaveformTowerBuilder.h>
#include <fun4all/Fun4AllDstOutputManager.h>

#include <litecaloeval/LiteCaloEval.h>


R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
//R__LOAD_LIBRARY(libCaloTowerBuilder.so)
R__LOAD_LIBRARY(libLiteCaloEvalTowSlope.so)


#endif

// to get files from my local area
void dst_simple_towerslope_runmacro(int nevents = 1e5, const char *fname = "/sphenix/user/sregmi/FINAL_VERSION/tower_slope_old_tower_method/3_RawWaveformTowerBuilder/macro/testoutput_400.root")

{
  gSystem->Load("libg4dst");

  Fun4AllServer *se = Fun4AllServer::instance();

  // CaloWaveFormSim *ca = new CaloWaveFormSim("CALOWAVEFORMSIM",outfile);
  // ca->Detector("CEMC");
  // se->registerSubsystem(ca);

  // Fun4AllInputManager *in = new Fun4AllPrdfInputManager("in");
  // in->fileopen(fname);

  Fun4AllInputManager *in = new Fun4AllDstInputManager("in");

  in->fileopen(fname);
  //CaloTowerBuilder *ca = new CaloTowerBuilder();


  LiteCaloEval *eval7e = new LiteCaloEval("CEMCEVALUATOR2", "CEMC", "output_towerslope.root");
  //  LiteCaloEval *eval = new LiteCaloEval("HOCEMCEVALUATOR2", "HCALOUT", outputfile.c_str());
  //  eval->Verbosity(verbosity);
  eval7e->CaloType(LiteCaloEval::CEMC);
  //eval->CaloType(LiteCaloEval::HCALOUT);
  se->registerSubsystem(eval7e);




  // in->AddListFile("g4hits.list");

// Fun4All
  se->registerInputManager(in);

  //Fun4AllDstOutputManager *out = new Fun4AllDstOutputManager("DSTOUT", "testoutput_400.root");

  //out->StripNode("WAVEFORMS_CEMC");
  //se->registerOutputManager(out);

  se->run(nevents);

  se->End();
  se->PrintTimer();
  gSystem->Exit(0);
}
