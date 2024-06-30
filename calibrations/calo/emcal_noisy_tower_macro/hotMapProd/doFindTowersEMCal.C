#include <emcnoisytowerfinder/emcNoisyTowerFinder.h>
R__LOAD_LIBRARY(libemcNoisyTowerFinder.so)

void doFindTowersEMCal(std::string infile="/sphenix/u/bseidlitz/work/forBayal/out_40858.root",std::string outfile = "output.root"){

  emcNoisyTowerFinder *calo = new emcNoisyTowerFinder("noisyTowerFinder","");
  calo->FindHot(infile,outfile,"h_CaloValid_cemc_etaphi"); 



}

