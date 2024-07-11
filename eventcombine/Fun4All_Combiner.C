#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4allraw/SinglePrdfInput.h>
#include <fun4allraw/Fun4AllPrdfInputManager.h>
#include <fun4allraw/Fun4AllPrdfInputPoolManager.h>
#include <fun4all/Fun4AllOutputManager.h>

#include <fun4allraw/Fun4AllEventOutputManager.h>

//#include <ffarawmodules/EventCombiner.h>
#include <ffarawmodules/EventNumberCheck.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libffarawmodules.so)
//9451
void Fun4All_Combiner(int nEvents = 0,
		      const string &input_file00 = "seb00.list",
		      const string &input_file01 = "seb01.list",
		      const string &input_file02 = "seb02.list",
		      const string &input_file03 = "seb03.list",
		      const string &input_file04 = "seb04.list",
		      const string &input_file05 = "seb05.list",
		      const string &input_file06 = "seb06.list",
		      const string &input_file07 = "seb07.list",
		      const string &input_file08 = "seb14.list",
		      const string &input_file09 = "seb18.list",
		      const string &input_file10 = "hcaleast.list",
		      const string &input_file11 = "hcalwest.list"
)
{
  vector<string> infile;
  infile.push_back(input_file00);
  infile.push_back(input_file01);
  infile.push_back(input_file02);
  infile.push_back(input_file03);
  infile.push_back(input_file04);
  infile.push_back(input_file05);
  infile.push_back(input_file06);
  infile.push_back(input_file07);
  infile.push_back(input_file08);
 infile.push_back(input_file10);
 infile.push_back(input_file11);

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(1);
  Fun4AllPrdfInputPoolManager *in = new Fun4AllPrdfInputPoolManager("Comb");
  in->Verbosity(1);
// this one is the reference with the mbd data
  in->AddPrdfInputList(input_file09)->MakeReference(true);
  for (auto iter : infile)
  {
      in->AddPrdfInputList(iter);//->Verbosity(2);
  }

  se->registerInputManager(in);

  // EventNumberCheck *evtchk = new EventNumberCheck();
  // evtchk->MyPrdfNode("PRDF");
  // se->registerSubsystem(evtchk);

  Fun4AllEventOutputManager *out = new Fun4AllEventOutputManager("EvtOut","/sphenix/lustre01/sphnxpro/commissioning/aligned_prdf/beam-%08d-%04d.prdf",20000);
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
