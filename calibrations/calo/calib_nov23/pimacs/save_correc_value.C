#include <calib_emc_pi0/CaloCalibEmc_Pi0.h>

void save_correc_value(
		std::string file_prefix = "m_clust_pi0_iter",
		int n_iter = 23
	)
			
{	
	R__LOAD_LIBRARY(libcalibCaloEmc_pi0.so)	

	// file location
	std::string macro_loc = "/sphenix/user/sregmi/WORKING_AREA/PI_0/pi0_calib_m-clust/macros/RESULTS/mult_clus_loop_iter/";
	
	// input file
	std::string ifile = macro_loc + file_prefix + std::to_string(n_iter) + "_combined.root";
	
	// output file
	std::string ofile = macro_loc + file_prefix + std::to_string(n_iter) + "_combined_fitted.root";
	
	std::string infilent = ""; // used for first iteration ONLY
	if (n_iter > 1)
		{infilent = macro_loc + file_prefix + std::to_string(n_iter-1) + "_combined_fitted.root";}

	std::cout << "input file , " << ifile << std::endl;
	std::cout << "output file , " << ofile << std::endl;
	std::cout << "incorrect file , " << infilent << std::endl;
	 	
	
	CaloCalibEmc_Pi0 calo_obj("CaloCalibEmc_Pi0" /* this name goes to SubsysReco  */, ofile.c_str());
	//	calo_obj.InitRun(0); // to declare the eta_hist[96] 
	calo_obj.Get_Histos(ifile.c_str() , ofile.c_str());	// open the fun4al file and Get eta_hist[96] histos
	calo_obj.Fit_Histos_Etas96(infilent.c_str());	// do fittings
	calo_obj.End(0);	// save the output file
	std::cout << "All Done" << std::endl;
	gSystem->Exit(0);
}

