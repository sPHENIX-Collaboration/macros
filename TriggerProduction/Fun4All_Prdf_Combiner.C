#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4allraw/Fun4AllPrdfInputTriggerManager.h>
#include <fun4allraw/InputManagerType.h>
#include <fun4allraw/SingleCemcTriggerInput.h>
#include <fun4allraw/SingleGl1TriggerInput.h>
#include <fun4allraw/SingleHcalTriggerInput.h>
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
                           const string &gl1input = "gl1.list",
                           const string &mbdinput = "mbd.list",
                           const string &hcalinput0 = "hcaleast.list",
                           const string &hcalinput1 = "hcalwest.list",
                           const string &cemcinput0 = "cemc00.list",
                           const string &cemcinput1 = "cemc01.list",
                           const string &cemcinput2 = "cemc02.list",
                           const string &cemcinput3 = "cemc03.list",
                           const string &cemcinput4 = "cemc04.list",
                           const string &cemcinput5 = "cemc05.list",
                           const string &cemcinput6 = "cemc06.list",
                           const string &cemcinput7 = "cemc07.list",
                           const string &zdcinput = "zdc.list")
{
  vector<string> hcalinfile;
  hcalinfile.push_back(hcalinput0);
  hcalinfile.push_back(hcalinput1);

  vector<string> cemcinfile;
  cemcinfile.push_back(cemcinput0);
  cemcinfile.push_back(cemcinput1);
  cemcinfile.push_back(cemcinput2);
  cemcinfile.push_back(cemcinput3);
  cemcinfile.push_back(cemcinput4);
  cemcinfile.push_back(cemcinput5);
  cemcinfile.push_back(cemcinput6);
  cemcinfile.push_back(cemcinput7);

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(1);
  Fun4AllPrdfInputTriggerManager *in = new Fun4AllPrdfInputTriggerManager("Comb");
  //  in->Verbosity(1);
  // this one is the reference
  ifstream ingl1(gl1input);
  if (ingl1.is_open())
  {
    ingl1.close();

    SingleTriggerInput *gl1 = new SingleGl1TriggerInput("Gl1in");
    //  gl1->Verbosity(10);
    gl1->enable_ddump(DDUMP);
    gl1->AddListFile(gl1input);
    in->registerTriggerInput(gl1, InputManagerType::GL1);
  }
  ingl1.open(mbdinput);
  if (ingl1.is_open())
  {
    ingl1.close();
    SingleTriggerInput *mbd = new SingleMbdTriggerInput("Mbdin");
    //  mbd->Verbosity(10);
    mbd->enable_ddump(DDUMP);
    mbd->AddListFile(mbdinput);
    in->registerTriggerInput(mbd, InputManagerType::MBD);
  }

  ingl1.open(zdcinput);
  if (ingl1.is_open())
  {
    ingl1.close();
    SingleTriggerInput *zdc = new SingleZdcTriggerInput("Zdcin");
    //  mbd->Verbosity(10);
    zdc->enable_ddump(DDUMP);
    zdc->AddListFile(zdcinput);
    in->registerTriggerInput(zdc, InputManagerType::ZDC);
  }
  int inpt = 0;
  for (auto iter : hcalinfile)
  {
    ingl1.open(iter);
    if (ingl1.is_open())
    {
      ingl1.close();
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
    ingl1.open(iter);
    if (ingl1.is_open())
    {
      ingl1.close();

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
  // clkchk->Verbosity(3);
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
