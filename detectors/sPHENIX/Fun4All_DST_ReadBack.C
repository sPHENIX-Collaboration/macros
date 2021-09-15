// $Id: $

/*!
 * \file Fun4All_DST_ReadBack.C
 * \brief 
 * \author Jin Huang <jhuang@bnl.gov>
 * \version $Revision:   $
 * \date $Date: $
 */

#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/SubsysReco.h>

#include <TSystem.h>

R__LOAD_LIBRARY(libfun4all.so)

void Fun4All_DST_ReadBack(const int nevnt = 0, const char *inputfile = "G4sPHENIX.root")
{
  gSystem->Load("libg4dst");
  Fun4AllServer *se = Fun4AllServer::instance();

  Fun4AllInputManager *in = new Fun4AllDstInputManager("DSTin");
  in->fileopen(inputfile);
  se->registerInputManager(in);

  se->run(nevnt);
  se->End();
  delete se;
  gSystem->Exit(0);
}
