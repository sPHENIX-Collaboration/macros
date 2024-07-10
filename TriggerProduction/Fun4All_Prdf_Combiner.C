#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4allraw/Fun4AllPrdfInputTriggerManager.h>
#include <fun4allraw/InputManagerType.h>
#include <fun4allraw/SingleCemcTriggerInput.h>
#include <fun4allraw/SingleGl1TriggerInput.h>
#include <fun4allraw/SingleHcalTriggerInput.h>
#include <fun4allraw/SingleLL1TriggerInput.h>
#include <fun4allraw/SingleMbdTriggerInput.h>
#include <fun4allraw/SingleZdcTriggerInput.h>

#include <fun4allraw/Fun4AllEventOutputManager.h>

#include <ffarawmodules/CemcCheck.h>
#include <ffarawmodules/ClockDiffCheck.h>
#include <ffarawmodules/EventNumberCheck.h>
#include <ffarawmodules/Gl1Check.h>
#include <ffarawmodules/HcalCheck.h>
#include <ffarawmodules/MbdCheck.h>
#include <ffarawmodules/ZdcCheck.h>

#include <ffamodules/FlagHandler.h>
#include <ffamodules/HeadReco.h>
#include <ffamodules/SyncReco.h>

R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libffarawmodules.so)

static const int DDUMP = 0;  // how many events into ddump lists --> 0 - none

void Fun4All_Prdf_Combiner(int nEvents = 0,
                           const string &gl1input = "gl1daq.list",
                           const string &cemcinput00 = "seb00.list",
                           const string &cemcinput01 = "seb01.list",
                           const string &cemcinput02 = "seb02.list",
                           const string &cemcinput03 = "seb03.list",
                           const string &cemcinput04 = "seb04.list",
                           const string &cemcinput05 = "seb05.list",
                           const string &cemcinput06 = "seb06.list",
                           const string &cemcinput07 = "seb07.list",
                           const string &cemcinput08 = "seb08.list",
                           const string &cemcinput09 = "seb09.list",
                           const string &cemcinput10 = "seb10.list",
                           const string &cemcinput11 = "seb11.list",
                           const string &cemcinput12 = "seb12.list",
                           const string &cemcinput13 = "seb13.list",
                           const string &cemcinput14 = "seb14.list",
                           const string &cemcinput15 = "seb15.list",
                           const string &hcalinput0 = "seb16.list",
                           const string &hcalinput1 = "seb17.list",
                           const string &mbdinput = "seb18.list",
                           const string &ll1input = "seb19.list",
                           const string &zdcinput = "seb20.list")
{
  vector<string> hcalinfile;
  hcalinfile.push_back(hcalinput0);
  hcalinfile.push_back(hcalinput1);

  vector<string> cemcinfile;
  cemcinfile.push_back(cemcinput00);
  cemcinfile.push_back(cemcinput01);
  cemcinfile.push_back(cemcinput02);
  cemcinfile.push_back(cemcinput03);
  cemcinfile.push_back(cemcinput04);
  cemcinfile.push_back(cemcinput05);
  cemcinfile.push_back(cemcinput06);
  cemcinfile.push_back(cemcinput07);
  cemcinfile.push_back(cemcinput08);
  cemcinfile.push_back(cemcinput09);
  cemcinfile.push_back(cemcinput10);
  cemcinfile.push_back(cemcinput11);
  cemcinfile.push_back(cemcinput12);
  cemcinfile.push_back(cemcinput13);
  cemcinfile.push_back(cemcinput14);
  cemcinfile.push_back(cemcinput15);

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(1);
  Fun4AllPrdfInputTriggerManager *in = new Fun4AllPrdfInputTriggerManager("Comb");
  in->InitialPoolDepth(10);
  in->SetPoolDepth(3);
  in->Resync(true);

  //in->Verbosity(2);
  // this one is the reference
  ifstream infile(gl1input);
  if (infile.is_open())
  {
    infile.close();

    SingleTriggerInput *gl1 = new SingleGl1TriggerInput("Gl1in");
    //gl1->Verbosity(10);
    gl1->enable_ddump(DDUMP);
    gl1->AddListFile(gl1input);
    in->registerTriggerInput(gl1, InputManagerType::GL1);
  }
  infile.open(mbdinput);
  if (infile.is_open())
  {
    infile.close();
    SingleTriggerInput *mbd = new SingleMbdTriggerInput("Mbdin");
    //mbd->Verbosity(10);
    mbd->enable_ddump(DDUMP);
    mbd->AddListFile(mbdinput);
    in->registerTriggerInput(mbd, InputManagerType::MBD);
  }

  infile.open(zdcinput);
  if (infile.is_open())
  {
    infile.close();
    SingleTriggerInput *zdc = new SingleZdcTriggerInput("Zdcin");
    //  mbd->Verbosity(10);
    zdc->enable_ddump(DDUMP);
    zdc->AddListFile(zdcinput);
    in->registerTriggerInput(zdc, InputManagerType::ZDC);
  }

  infile.open(ll1input);
  if (infile.is_open())
  {
    infile.close();
    SingleTriggerInput *ll1 = new SingleLL1TriggerInput("LL1in");
    //  mbd->Verbosity(10);
    ll1->enable_ddump(DDUMP);
    ll1->AddListFile(ll1input);
    in->registerTriggerInput(ll1, InputManagerType::LL1);
  }
  int inpt = 0;
  for (auto iter : hcalinfile)
  {
    infile.open(iter);
    if (infile.is_open())
    {
      infile.close();
      string name = "Hcalin_" + to_string(inpt);
      SingleTriggerInput *hcal = new SingleHcalTriggerInput(name);
      //    hcal->Verbosity(10);
      hcal->AddListFile(iter);
      hcal->enable_ddump(DDUMP);
      in->registerTriggerInput(hcal, InputManagerType::HCAL);
    }
    inpt++;
  }

  inpt = 0;
  for (auto iter : cemcinfile)
  {
    infile.open(iter);
    if (infile.is_open())
    {
      infile.close();

      string name = "Cemcin_" + to_string(inpt);
      SingleTriggerInput *cemc = new SingleCemcTriggerInput(name);
      //    cemc->Verbosity(10);
      cemc->AddListFile(iter);
      cemc->enable_ddump(DDUMP);
      in->registerTriggerInput(cemc, InputManagerType::CEMC);
    }
    inpt++;
  }

  se->registerInputManager(in);

  SyncReco *sync = new SyncReco();
  se->registerSubsystem(sync);

  HeadReco *head = new HeadReco();
  se->registerSubsystem(head);

  FlagHandler *flag = new FlagHandler();
  se->registerSubsystem(flag);

/*
    Gl1Check *gl1chk = new Gl1Check();
    gl1chk->enable_ddump(DDUMP);
    se->registerSubsystem(gl1chk);

    MbdCheck *mbdchk = new MbdCheck();
    mbdchk->enable_ddump(DDUMP);
    se->registerSubsystem(mbdchk);

    HcalCheck *hcalchk = new HcalCheck();
    hcalchk->enable_ddump(DDUMP);
    se->registerSubsystem(hcalchk);

    CemcCheck *cemcchk = new CemcCheck();
    cemcchk->enable_ddump(DDUMP);
    se->registerSubsystem(cemcchk);

    ZdcCheck *zdcchk = new ZdcCheck();
    zdcchk->enable_ddump(DDUMP);
    se->registerSubsystem(zdcchk);
  */
  ClockDiffCheck *clkchk = new ClockDiffCheck();
  //clkchk->Verbosity(3);
  clkchk->set_delBadPkts(true);
  se->registerSubsystem(clkchk);

  Fun4AllOutputManager *out = new Fun4AllDstOutputManager("TriggerOut", "DST_PRDF.root");
  //  out->DropPacket(21102);
  se->registerOutputManager(out);
  if (nEvents < 0)
  {
    return;
  }
  se->run(nEvents);
  in->Print();
  se->End();
  delete se;
  gSystem->Exit(0);
}

bool isGood(const string &infile)
{
  ifstream intest;
  intest.open(infile);
  bool goodfile = false;
  if (intest.is_open())
  {
    if (intest.peek() != std::ifstream::traits_type::eof())  // is it non zero?
    {
      goodfile = true;
    }
    intest.close();
  }
  return goodfile;
}
