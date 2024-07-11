#ifndef MACRO_C
#define MACRO_C

#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
R__LOAD_LIBRARY(libfun4all.so)

#include <fun4allraw/Fun4AllStreamingInputManager.h>
#include <fun4allraw/InputManagerType.h>
#include <fun4allraw/SingleInttPoolInput.h>
R__LOAD_LIBRARY(libfun4allraw.so)

#include <inttcalib/InttCalib.h>
R__LOAD_LIBRARY(libinttcalib.so)

#include <phool/recoConsts.h>
R__LOAD_LIBRARY(libphool.so)

#include <filesystem>
#include <iostream>
#include <string>

void
macro (
	int run_num,
	int num_evt,
	std::string const& intt_format,
	std::string const& hotmap_cdb_file,
	std::string const& hotmap_png_file,
	std::string const& bcomap_cdb_file,
	std::string const& bcomap_png_file
) {
	char buff[256];

	// Fun4All
	Fun4AllServer* se = Fun4AllServer::instance();
	se->Verbosity(0);

	recoConsts* rc = recoConsts::instance();
	rc->set_StringFlag("CDB_GLOBALTAG", "ProdA_2023");
	rc->set_uint64Flag("TIMESTAMP", run_num);

	// Input Manager
	Fun4AllStreamingInputManager* in = new Fun4AllStreamingInputManager("Comb");
	std::vector<std::string> missing_list_files = {};

	for(int i = 0; i < 8; ++i) {
		snprintf(buff, sizeof(buff), intt_format.c_str(), run_num, i);
		if(!std::filesystem::exists(buff)) {
			missing_list_files.push_back(buff);
			continue;
		}

		auto* intt_sngl = new SingleInttPoolInput("INTT_" + std::to_string(i));
		intt_sngl->SetNegativeBco(0);
		intt_sngl->SetBcoRange(2);
		intt_sngl->AddListFile(buff);
		in->registerStreamingInput(intt_sngl, InputManagerType::INTT);
	}
	se->registerInputManager(in);

	if(missing_list_files.size()) {
		std::cerr << "Missing expected list files:" << std::endl;
		for(std::string const& file : missing_list_files) {
			std::cerr << "\t" << file << std::endl;
		}

		delete se;
		gSystem->Exit(1);
		exit(1);
	}

	// SubsysReco
	InttCalib* inttcalib = new InttCalib();
	inttcalib->Verbosity(0);
	inttcalib->SetHotMapCdbFile(hotmap_cdb_file);
	inttcalib->SetHotMapPngFile(hotmap_png_file);
	inttcalib->SetBcoMapCdbFile(bcomap_cdb_file);
	inttcalib->SetBcoMapPngFile(bcomap_png_file);
	se->registerSubsystem(inttcalib);

	if(inttcalib->Verbosity()) {
		std::cout << "\n"
		          << "hotmap_cdb_file: " << hotmap_cdb_file << "\n"
		          << "hotmap_png_file: " << hotmap_png_file << "\n"
		          << "bcomap_cdb_file: " << bcomap_cdb_file << "\n"
		          << "bcomap_png_file: " << bcomap_png_file << "\n"
				  << std::endl;
	}

	// Run
	se->run(num_evt);

	// End
	se->End();
	if(inttcalib->Verbosity()) {
		std::cout << "Macro done" << std::endl;
	}
	delete se;
	gSystem->Exit(0);
	exit(0);
}

#endif//MACRO_C
