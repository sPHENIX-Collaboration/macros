#ifndef __FUN4ALL_MBD_CALPASS_H__
#define __FUN4ALL_MBD_CALPASS_H__

#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllUtils.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4allraw/Fun4AllPrdfInputManager.h>
#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4allraw/Fun4AllEventOutputManager.h>
#include <phool/recoConsts.h>

#include <ffamodules/CDBInterface.h>
#include <ffamodules/FlagHandler.h>
#include <ffamodules/HeadReco.h>
#include <ffamodules/SyncReco.h>

#include <globalvertex/GlobalVertexReco.h>
#include <mbd/MbdReco.h>

#if defined(__CLING__)
R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libphool.so)
R__LOAD_LIBRARY(libmbd.so)
R__LOAD_LIBRARY(libglobalvertex.so)
#endif

void Fun4All_MBD_CalPass(const char *input = "/sphenix/user/pinkenbu/testprdf/beam-00002609-0000.prdf",
    const int calpass = 0, int nEvents = 0, const int nskip = 0, const std::string& cdbtag = "")
{
  // process the input
  // it could be a single prdf, dst, or listfile
  // or it could be a comma separated string of prdf, dst, or listfiles

  std::stringstream ss(input);
  std::vector<std::string> all_inputfnames;
  while ( ss.good() )
  {
    std::string temp;
    getline( ss, temp, ',' );
    all_inputfnames.push_back( temp );
  }

  // Get dst or prdf filename to extract runnumber
  std::string first_line = all_inputfnames[0];
  if ( first_line.ends_with(".list") )
  {
    ifstream listfile(all_inputfnames[0]);
    getline(listfile, first_line);
    listfile.close();
  }

  pair<int, int> runseg = Fun4AllUtils::GetRunSegment(first_line);
  int runnumber = runseg.first;
  int segment = runseg.second;
  cout << "run number = " << runnumber << endl;

  recoConsts *rc = recoConsts::instance();
  if ( cdbtag.length() !=0 )
  {
    cout << "Using cdb " << cdbtag << endl;
    rc->set_StringFlag("CDB_GLOBALTAG",cdbtag.c_str()); 
  }
  else
  {
    cout << "RUN\t" << runnumber << endl;
    rc->set_uint64Flag("TIMESTAMP", runnumber);

    // For local calibrations
    TString bdir = "./results/";
    bdir += runnumber;
    cout << bdir << endl;
    rc->set_StringFlag("MBD_CALDIR",bdir.Data()); 
  }

  if ( calpass==1 && nEvents<0 )
  {
    //nEvents = 100000; // for p+p
    nEvents = 30000;  // for Au+Au
  }

  Fun4AllServer *se = Fun4AllServer::instance();
  //se->Verbosity(1);

  // Sync Headers and Flags
  SyncReco *sync = new SyncReco();new Fun4AllPrdfInputManager("PRDFin");
  se->registerSubsystem(sync);

  HeadReco *head = new HeadReco();
  se->registerSubsystem(head);

  FlagHandler *flag = new FlagHandler();
  se->registerSubsystem(flag);

  // MBD/BBC Reconstruction
  MbdReco *mbdreco = new MbdReco();
  mbdreco->SetCalPass(calpass);
  se->registerSubsystem(mbdreco);

  // Official vertex storage
  //GlobalVertexReco *gvertex = new GlobalVertexReco();
  //se->registerSubsystem(gvertex);

  vector<Fun4AllInputManager *> in;

  if ( all_inputfnames[0].ends_with(".prdf") )
  {
    for ( const auto& inputfname : all_inputfnames )
    {
      Fun4AllPrdfInputManager *inputman = new Fun4AllPrdfInputManager("PRDFin");
      inputman->fileopen( inputfname );
      in.push_back( inputman );
      se->registerInputManager( inputman );
    }
  }
  else if ( all_inputfnames[0].ends_with(".root") )
  {
    for ( const auto& inputfname : all_inputfnames )
    {
      Fun4AllDstInputManager *inputman = new Fun4AllDstInputManager("DST");
      inputman->AddFile( inputfname );
      in.push_back( inputman );
      se->registerInputManager( inputman );
    }
  }
  else if ( all_inputfnames[0].ends_with(".list") )
  {
    for ( const auto& inputfname : all_inputfnames )
    {
      cout << "adding " << inputfname << endl;
      Fun4AllDstInputManager *inputman = new Fun4AllDstInputManager("DST");
      inputman->AddListFile( inputfname );
      in.push_back( inputman );
      se->registerInputManager( inputman );
    }
  }

  if ( calpass == 2 )
  {
    TString outfile = "DST_UNCALMBD-";
    outfile += Form("%08d-%04d",runnumber,segment);
    outfile += ".root";
    cout << outfile << endl;
    Fun4AllDstOutputManager *out = new Fun4AllDstOutputManager("DSTOUT", outfile.Data());
    out->StripNode("CEMCPackets");
    out->StripNode("HCALPackets");
    out->StripNode("ZDCPackets");
    out->StripNode("SEPDPackets");
    out->StripNode("MBDPackets");
    out->StripNode("TOWERS_ZDC");
    out->StripNode("TOWERS_SEPD");
    out->StripNode("TOWERS_CEMC");
    out->StripNode("TOWERS_HCALIN");
    out->StripNode("TOWERS_HCALOUT");
    se->registerOutputManager(out);
  }

  se->skip(nskip);
  se->run(nEvents);

  se->End();
  delete se;
  gSystem->Exit(0);

  cout << "Finished" << endl;
}

#endif

