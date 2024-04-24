#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4allraw/Fun4AllStreamingInputManager.h>
#include <fun4allraw/InputManagerType.h>
#include <fun4allraw/SingleInttPoolInput.h>

#include <phool/recoConsts.h>

#include <ffarawmodules/InttCheck.h>

#include <intt/InttCombinedRawDataDecoder.h>

#include <G4Setup_sPHENIX.C>
#include <G4_TrkrVariables.C>
#include <G4_ActsGeom.C>
#include <Trkr_Clustering.C>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libffarawmodules.so)
R__LOAD_LIBRARY(libintt.so)

bool isGood(const string &infile)
{
  ifstream intest;
  intest.open(infile);
  bool goodfile = false;
  if (intest.is_open())
  {
    if (intest.peek() != std::ifstream::traits_type::eof()) // is it non zero?
    {
      goodfile = true;
    }
      intest.close();
  }
  return goodfile;
}

void Fun4All_Intt_Combiner(int nEvents = 0,
                           const int runnumber = 20869,
                           const string cdbglobaltag = "ProdA_2023",
                           const bool runTrkrHits = true,
                           const bool applyHotChannel = true,
                           const bool applyBCOCut = true,
                           const bool applyADCConversion = true,  
                           const bool runTkrkClus = true,
                           const bool usesurveygeom = true,
                           const bool stripRawHit = true)
{

  vector<string> infile = {"intt0.list", "intt1.list", "intt2.list", "intt3.list", "intt4.list", "intt5.list", "intt6.list", "intt7.list"};

  TString outfilename = Form("intt-%08d.root", runnumber);
  TString outdirinitial = "ProdDST";
  if (applyHotChannel)
  {
    outdirinitial += "-HotDead";
  }
  if (applyBCOCut)
  {
    outdirinitial += "-BCO";
  }
  if (applyADCConversion)
  {
    outdirinitial += "-ADC";
  }
  if (usesurveygeom)
  {
    outdirinitial += "-Survey";
  }

  system(Form("mkdir -p %s", outdirinitial.Data()));

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(1);
  recoConsts *rc = recoConsts::instance();

  Enable::CDB = true;
  rc->set_StringFlag("CDB_GLOBALTAG", cdbglobaltag);
  rc->set_uint64Flag("TIMESTAMP", runnumber);

  Fun4AllStreamingInputManager *in = new Fun4AllStreamingInputManager("Comb");
  //  in->Verbosity(10);
  int i = 0;
  for (auto iter : infile)
  {
    if (isGood(iter))
    {
      SingleInttPoolInput *sngl = new SingleInttPoolInput("INTT_" + to_string(i));
      //    sngl->Verbosity(3);
      sngl->AddListFile(iter);
      int nBcoVal = runTrkrHits ? 0 : 2;
      sngl->SetNegativeBco(nBcoVal);
      sngl->SetBcoRange(2);
      in->registerStreamingInput(sngl, InputManagerType::INTT);
      i++;
    }
  }

  se->registerInputManager(in);

  //  InttCheck *inttcheck = new InttCheck();
  //  inttcheck->Verbosity(3);
  //  se->registerSubsystem(inttcheck);

  if (runTrkrHits)
  {
    InttCombinedRawDataDecoder *myDecoder = new InttCombinedRawDataDecoder("myUnpacker");
    myDecoder->runInttStandalone(true);
    myDecoder->writeInttEventHeader(true);
    if (applyHotChannel) myDecoder->LoadHotChannelMapRemote("INTT_HotMap");
    string DACmap = (applyADCConversion) ? "INTT_DACMAP" : "";
    string BCOmap = (applyBCOCut) ? "INTT_BCOMAP" : "";
    myDecoder->SetCalibDAC(DACmap);
    myDecoder->SetCalibBCO(BCOmap);
    se->registerSubsystem(myDecoder);
  }

  if (runTkrkClus)
  {
    Enable::MVTX = true;
    Enable::INTT = true;
    Enable::TPC = true;
    Enable::MICROMEGAS = true;
    //! [TO CONFIRM] The switches for the ideal and survey geometries do not seem to work anymore. Is this intended?
    Enable::INTT_USEG4SURVEYGEOM = usesurveygeom; 
    ACTSGEOM::inttsurvey = usesurveygeom;
    G4Init();
    G4Setup();
    ClusteringInit();   // ActsGeomInit() is called here
    Intt_Clustering();  // Be careful!!! INTT z-clustering may be off which is not what you want!
  }

  Fun4AllOutputManager *out = new Fun4AllDstOutputManager("out", Form("%s/%s", outdirinitial.Data(), outfilename.Data()));
  if (stripRawHit)
  {
    out->StripNode("INTTRAWHIT");
  }
  se->registerOutputManager(out);

  se->run(nEvents);

  se->End();
  delete se;
  gSystem->Exit(0);
}
