#include <calib_emc_pi0/CaloCalibEmc_Pi0.h>
//#include "GetTChainMacro.C"
#include <GetTChainMacro.C>


void runLCELoop(int nevents = -1, const char *ifile="", const char *ofile="",const char *incorr="")
{
	R__LOAD_LIBRARY(libcalibCaloEmc_pi0.so)	

	CaloCalibEmc_Pi0 calo_obj("CaloCalibEmc_Pi0", ofile);
  calo_obj.InitRun(0);
  TTree * intree1 =  GetTChainMacro(ifile);
  //	calo_obj.InitializeBadTowers();
  calo_obj.Loop(nevents, ifile, intree1, incorr);
  calo_obj.End(0);
}
