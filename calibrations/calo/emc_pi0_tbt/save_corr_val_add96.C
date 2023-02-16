#include <calib_emc_pi0/CaloCalibEmc_Pi0.h>

void save_correc_value(const char* ifile = "", const char* ofile = "",const char * infilent = "")
{
	R__LOAD_LIBRARY(libcalibCaloEmc_pi0.so)	
	  
	CaloCalibEmc_Pi0 calo_obj("CaloCalibEmc_Pi0", ofile );
	
	calo_obj.Get_Histos(ifile,ofile);

	calo_obj.Add_96();

	calo_obj.Fit_Histos_Eta_Phi_Add96(infilent);

	calo_obj.End(0);
}
