#ifndef FUN4ALL_INTT_BCOFINDER_C
#define FUN4ALL_INTT_BCOFINDER_C

#include <intt/InttMapping.h>

#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllDstInputManager.h>

#include <phool/recoConsts.h>

#include <iostream>
#include <string>

#include <G4Setup_sPHENIX.C>
#include <inttbcofinder/InttBCOFinder.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libffarawmodules.so)
R__LOAD_LIBRARY(libinttbcofinder.so)


void Fun4All_Intt_BCOFinder(int run_num = 41297, int nevents = 500000,
                            std::string in_file = "/sphenix/lustre01/sphnxpro/commissioning/slurp/inttbeam/run_00041200_00041300/DST_INTT_RAW_beam_new_2023p011-00041297-0000.root")
// nevents : # of event used to make BCO distribution
// in_file : input DST file
{      
          //--output file path
        std::string string_num = std::to_string(run_num);
        string_num = std::string(8 - string_num.length(), '0') + string_num;
        std::string cdb_output_path = "./";
        std::string QA_output_path = "./";
        std::string QA_out_file = QA_output_path + "QA_bco_" + string_num + ".root";
        std::string cdb_out_file = cdb_output_path + "cdb_bco_" + string_num + ".root";


        Fun4AllServer *se = Fun4AllServer::instance();
        // se->Verbosity(5);
                // just if we set some flags somewhere in this macro
        recoConsts *rc = recoConsts::instance();

        Enable::CDB = true;
        // global tag
        rc->set_StringFlag("CDB_GLOBALTAG", CDB::global_tag);
        //
        // 64 bit timestamp
        rc->set_uint64Flag("TIMESTAMP", CDB::timestamp);

        //--input
        Fun4AllInputManager *in = new Fun4AllDstInputManager("DSTin");
        in->Verbosity(2);
        in->fileopen(in_file);
        se->registerInputManager(in);


        InttBCOFinder *inttbcofinder = new InttBCOFinder("inttbcofinder",
                                                         QA_out_file.c_str(),
                                                         cdb_out_file.c_str(),
                                                         nevents);
        inttbcofinder-> WriteCDBTTree(true);
        inttbcofinder-> WriteQAFile(true); 
        se->registerSubsystem(inttbcofinder);

        se->run(nevents);
        se->End();

        delete se;
}

#endif // FUN4ALL_INTT_BCOFINDER.C
