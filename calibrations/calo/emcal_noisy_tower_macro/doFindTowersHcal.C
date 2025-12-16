#include <emcnoisytowerfinder/emcNoisyTowerFinder.h>

#include <Rtypes.h> // for R__LOAD_LIBRARY macro

R__LOAD_LIBRARY(libemcNoisyTowerFinder.so)

void doFindTowersHcal(const std::string &infile="/sphenix/u/bseidlitz/work/forBayal/out_40858.root",const std::string &outfile = "output.root")
{
  emcNoisyTowerFinder *calo = new emcNoisyTowerFinder("noisyTowerFinder","");
//  calo->set_hcal();
  calo->FindHot(infile,outfile,"h_CaloValid_ohcal_etaphi_wQA"); 
}

