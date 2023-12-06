#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4allraw/SingleMvtxPoolInput.h>
#include <fun4all/Fun4AllOutputManager.h>

#include <fun4allraw/Fun4AllEventOutputManager.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libffarawmodules.so)

SingleMvtxPoolInput *sngl[24]{};

vector<string> infile;

//9451
void Fun4All_SingleMvtx(int nEvents = 0,
		      const string &input_file00 = "mvtx-flx0.list",
		      const string &input_file01 = "mvtx-flx1.list",
		      const string &input_file02 = "mvtx-flx2.list",
		      const string &input_file03 = "mvtx-flx3.list",
		      const string &input_file04 = "mvtx-flx4.list",
		      const string &input_file05 = "mvtx-flx5.list"
)
{
  infile.push_back(input_file00);
  infile.push_back(input_file01);
  // infile.push_back(input_file02);
  // infile.push_back(input_file03);
  // infile.push_back(input_file04);
  // infile.push_back(input_file05);

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(1);
//  EventCombiner *evtcomb = new EventCombiner();
//  evtcomb->Verbosity(1);
  // Fun4AllEvtInputPoolManager *in = new Fun4AllEvtInputPoolManager("Comb");
  // in->Verbosity(10);
  int i = 0;
  for (auto iter : infile)
  {
    sngl[i]= new SingleMvtxPoolInput("MVTX_" + to_string(i));
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
//    while(1)
  {
    int j = 0;
    bool quit = false;
    for (auto iter : infile)
    {
      sngl[j]->FillPool(10);
      if (sngl[j]->AllDone())
      {
	quit = true;
      }
      j++;
    }
    if (quit)
    {
      break;
    }
  }
}

void RunBclk(const uint64_t bclk = 0)
{
    int j = 0;
    for (auto iter : infile)
    {
      sngl[j]->FillPool(bclk);
      j++;
  }
}

void CleanUp()
{
    int j = 0;
    for (auto iter : infile)
    {
      sngl[j]->ClearCurrentEvent();
      j++;
    }
}

void Verbosity(int v)
{
    int j = 0;
    for (auto iter : infile)
    {
      sngl[j]->Verbosity(v);
      j++;
    }
}

void Print(const std::string &what = "ALL", int i = -1)
{
  if (i < 0)
  {
    int j = 0;
    for (auto iter : infile)
    {
      sngl[j]->Print(what);
      j++;
    }
    return;
  }

}
