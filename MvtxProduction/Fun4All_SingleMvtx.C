#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>

#include <fun4allraw/SingleMvtxPoolInput.h>
#include <fun4allraw/Fun4AllEventOutputManager.h>

#include <Rtypes.h> // for R__LOAD_LIBRARY

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libffarawmodules.so)

SingleMvtxPoolInput *sngl[24]{};

std::vector<std::string> infile;

//9451
void Fun4All_SingleMvtx(int  /*nEvents*/ = 0,
		      const std::string &input_file00 = "mvtx-flx0.list",
		      const std::string &input_file01 = "mvtx-flx1.list",
		      const std::string &input_file02 = "mvtx-flx2.list",
		      const std::string &input_file03 = "mvtx-flx3.list",
		      const std::string &input_file04 = "mvtx-flx4.list",
		      const std::string &input_file05 = "mvtx-flx5.list"
)
{
  infile.push_back(input_file00);
  infile.push_back(input_file01);
  infile.push_back(input_file02);
  infile.push_back(input_file03);
  infile.push_back(input_file04);
  infile.push_back(input_file05);

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(1);
//  EventCombiner *evtcomb = new EventCombiner();
//  evtcomb->Verbosity(1);
  // Fun4AllEvtInputPoolManager *in = new Fun4AllEvtInputPoolManager("Comb");
  // in->Verbosity(10);
  int i = 0;
  for (const auto& iter : infile)
  {
    sngl[i]= new SingleMvtxPoolInput("MVTX_" + std::to_string(i));
    sngl[i]->Verbosity(3);
    sngl[i]->SetBcoRange(10);
    sngl[i]->AddListFile(iter);
    i++;
  }

//  Fun4AllEventOutputManager *out = new Fun4AllEventOutputManager("EvtOut","/sphenix/user/pinkenbu/out-%08d-%04d.evt",20000);
//  out->DropPacket(21102);
//  se->registerOutputManager(out);
}

void Run(const int nEvents = 1)
{
 for (int i = 0; i < nEvents; i++)
  {
    bool quit = false;
    for (auto *iter : sngl)
    {
      iter->FillPool(10);
      if (iter->AllDone())
      {
	quit = true;
      }
    }
    if (quit)
    {
      break;
    }
  }
}

void RunBclk(const uint64_t bclk = 0)
{
  for (auto *iter : sngl)
  {
    iter->FillPool(bclk);
  }
}

void CleanUp()
{
  for (auto *iter : sngl)
  {
    iter->ClearCurrentEvent();
  }
}

void Verbosity(int v)
{
  for (auto *iter : sngl)
  {
    iter->Verbosity(v);
  }
}

void Print(const std::string &what = "ALL", int i = -1)
{
  if (i < 0)
  {
    for (auto *iter : sngl)
    {
      iter->Print(what);
    }
    return;
  }
}
