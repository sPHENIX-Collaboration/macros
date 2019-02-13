#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllDstInputManager.h>
#include <nodedump/Dumper.h>
R__LOAD_LIBRARY(libphnodedump.so)
#endif

void run_dump(const char *infile, const int evts=100)
{
  gSystem->Load("libPrototype4_io.so");
  gSystem->Load("libphnodedump.so");

  Fun4AllServer* se = Fun4AllServer::instance();

  Dumper *dmp = new Dumper();
  gSystem->Exec("mkdir /phenix/scratch/pinkenbu/g4dump");
  dmp->SetOutDir("/phenix/scratch/pinkenbu/g4dump");

  se->registerSubsystem(dmp);

  Fun4AllInputManager *in = new Fun4AllDstInputManager("DSTin");
  se->registerInputManager(in);
  se->fileopen("DSTin",infile);
  se->run(evts);
  se->End();
  delete se;
}
