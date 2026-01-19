#include <eventplaneinfo/EventPlaneReco.h>
#include <eventplaneinfo/Eventplaneinfo.h>

#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/SubsysReco.h>

#include <phool/recoConsts.h>

#include <Rtypes.h>  // resolves R__LOAD_LIBRARY for clang-tidy
#include <TSystem.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libeventplaneinfo.so)

void Fun4All_EP2(const int nevents = 2, const std::string &inputcaloList = "dst_calo.list")
{
  gSystem->Load("libg4dst.so");
  Fun4AllServer *se = Fun4AllServer::instance();

  EventPlaneReco *epreco = new EventPlaneReco();
  epreco->set_mbd_epreco(true);
  epreco->set_MBD_Min_Qcut(0.0);
  epreco->set_MBD_Vertex_cut(60.0);
  se->registerSubsystem(epreco);

  Fun4AllInputManager *in = new Fun4AllDstInputManager("DSTcalo");
  in->AddListFile(inputcaloList);
  se->registerInputManager(in);

  se->run(nevents);
  se->End();
  gSystem->Exit(0);
}
