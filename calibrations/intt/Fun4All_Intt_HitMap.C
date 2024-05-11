#ifndef FUN4ALL_INTT_HITMAP_C
#define FUN4ALL_INTT_HITMAP_C

#include <intt/InttMapping.h>

#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllDstInputManager.h>

#include <phool/recoConsts.h>

#include <iostream>
#include <string>

#include <G4Setup_sPHENIX.C>
#include <intthitmap/InttHitMap.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libffarawmodules.so)
R__LOAD_LIBRARY(libintthitmap.so)
void Fun4All_Intt_HitMap(int run_num = 41297,
			 int nevents = 500000,
			 std::string in_file = "/sphenix/lustre01/sphnxpro/commissioning/slurp/inttbeam/run_00041200_00041300/DST_INTT_RAW_beam_new_2023p011-00041297-0000.root")
{
        // --output file path
        std::string string_num = std::to_string(run_num);
		string_num = std::string(8 - string_num.length(), '0') + string_num;       
		std::string hitmap_out_file = "./hitmap_run" +string_num + ".root";
        
        Fun4AllServer *se = Fun4AllServer::instance();
        // se->Verbosity(5);

        // just if we set some flags somewhere in this macro
        recoConsts *rc = recoConsts::instance();

        Enable::CDB = true;
        // global tag
        rc->set_StringFlag("CDB_GLOBALTAG", CDB::global_tag);
        // 64 bit timestamp
        rc->set_uint64Flag("TIMESTAMP", CDB::timestamp);

		
        
        //--input
        Fun4AllInputManager *in = new Fun4AllDstInputManager("DSTin");
        in->Verbosity(2);
        in->fileopen(in_file);
        se->registerInputManager(in);

        InttHitMap *intthitmap = new InttHitMap("intthitmap",
                                                              hitmap_out_file.c_str(),
                                                              nevents);
        intthitmap->SetRunNumber(run_num);
        intthitmap->IsBeam(true);


		//BCO cut can be applied before making the hitmap. We won't use it for pp run. keep it just in case.		
		//std::string bco_input_file = "/sphenix/tg/tg01/commissioning/INTT/QA/bco_bcofull_difference/rootfile/2023/ladder_20869_3BCOcut.root";
        //intthitmap->SetBCOcut(false);
		//intthitmap->SetBCOFile(bco_input_file.c_str());

        //The case for if InttFeeMap will be updated.
		//intthitmap->SetFeeMapFile("InttFeeMap.root");
		se->registerSubsystem(intthitmap);

        se->run(nevents);
        se->End();

        delete se;
}

#endif // FUN4ALL_INTT_HITMAP.C
