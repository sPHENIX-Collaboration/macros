#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllDstInputManager.h>

#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllUtils.h>

#include <phool/PHRandomSeed.h>
#include <phool/recoConsts.h>
#include <cdbobjects/CDBTTree.h>


#include <emcnoisytowerfinder/emcNoisyTowerFinder.h>

R__LOAD_LIBRARY(libcdbobjects.so)
R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libmbd_io.so)
R__LOAD_LIBRARY(libemcNoisyTowerFinder.so)
#endif

void Fun4All_EMCalBadTowerID(const int nEvents = 5000, const char *inFile = "DST_CALO_run1auau_ana391_2023p006-00021813-1388.root", const char *inName = "commissioning3.root", const std::string cdbname = "test.root", float adccut_sg = 150, float adccut_k = 150,float sigmas_lo = 4, float sigmas_hi = 4, float SG_f = 0.05, float Kur_f = 0.05, float region_f = 1.0, int pass = 0)
{
  

  pair<int, int> runseg = Fun4AllUtils::GetRunSegment(inFile);
  int runnumber = runseg.first;
  int segment = runseg.second;

  emcNoisyTowerFinder *calo = new emcNoisyTowerFinder("noisyTowerFinder",inName,cdbname,adccut_sg,adccut_k,sigmas_lo,sigmas_hi,SG_f,Kur_f,region_f);
  calo -> set_Normalization(0);
  Fun4AllServer *se = Fun4AllServer::instance();
  recoConsts *rc = recoConsts::instance();
	
  if(pass == 0)
    {
      se->registerSubsystem(calo);
      Fun4AllInputManager *in = new Fun4AllDstInputManager("DSTcalo");
      in->AddFile(inFile);
      se->registerInputManager(in);
      se->run(nEvents);
      calo -> FillHistograms(runnumber,segment);
      se->End();
      se->PrintTimer();
    }
  else if(pass == 1)
    {
      calo -> Init(NULL);
      calo -> CalculateCutOffs(runnumber);
      calo -> WriteCDBTree(runnumber);
    }
  

  
  std::cout << "All done!" << std::endl;
  gSystem->Exit(0);
}

