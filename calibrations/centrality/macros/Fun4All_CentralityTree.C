// MBD/ZDC tree for the Au+Au centrality calibration (MbdCentralityTree), from DST_CALOFITTING + DST_ZDC_RAW.
// inputlist: one "DST_CALOFITTING DST_ZDC_RAW" pair of the same segment per line.
//   root -b -q 'Fun4All_CentralityTree.C("queue_54912_0000.list", "tree_54912_0000.root")'
#include <centralitycalib/MbdCentralityTree.h>

#include <caloreco/CaloTowerBuilder.h>
#include <globalvertex/GlobalVertexReco.h>
#include <mbd/MbdReco.h>
#include <zdcinfo/ZdcReco.h>

#include <ffamodules/CDBInterface.h>
#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllUtils.h>
#include <phool/recoConsts.h>

#include <Rtypes.h> // defines R__LOAD_LIBRARY macro for clang-tidy
#include <TSystem.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

R__LOAD_LIBRARY(libcalo_reco.so)
R__LOAD_LIBRARY(libcentralitycalib.so)
R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libglobalvertex.so)
R__LOAD_LIBRARY(libmbd.so)
R__LOAD_LIBRARY(libzdcinfo.so)

void Fun4All_CentralityTree(const std::string &inputlist, const std::string &outfile, int nevents = 0,
                            const std::string &cdbtag = "newcdbtag")
{
  std::vector<std::string> calofiles;
  std::vector<std::string> zdcfiles;
  std::ifstream in(inputlist);
  std::string line;
  while (std::getline(in, line))
  {
    std::istringstream iss(line);
    std::string calo;
    std::string zdc;
    if (iss >> calo >> zdc)
    {
      calofiles.push_back(calo);
      zdcfiles.push_back(zdc);
    }
  }
  if (calofiles.empty())
  {
    std::cout << "no \"DST_CALOFITTING DST_ZDC_RAW\" pairs in " << inputlist << std::endl;
    gSystem->Exit(1);
  }
  const int runnumber = Fun4AllUtils::GetRunSegment(calofiles.front()).first;

  Fun4AllServer *se = Fun4AllServer::instance();
  recoConsts *rc = recoConsts::instance();
  rc->set_StringFlag("CDB_GLOBALTAG", cdbtag);
  rc->set_uint64Flag("TIMESTAMP", runnumber);
  CDBInterface::instance()->Verbosity(1);

  se->registerSubsystem(new MbdReco());

  CaloTowerBuilder *zdcbuilder = new CaloTowerBuilder("ZDCBUILDER");
  zdcbuilder->set_detector_type(CaloTowerDefs::ZDC);
  zdcbuilder->set_builder_type(CaloTowerDefs::kPRDFTowerv4);
  zdcbuilder->set_processing_type(CaloWaveformProcessing::FAST);
  zdcbuilder->set_nsamples(16);
  zdcbuilder->set_offlineflag();
  se->registerSubsystem(zdcbuilder);

  ZdcReco *zdcreco = new ZdcReco();
  zdcreco->set_zdc1_cut(0.0);
  zdcreco->set_zdc2_cut(0.0);
  se->registerSubsystem(zdcreco);

  se->registerSubsystem(new GlobalVertexReco());

  se->registerSubsystem(new MbdCentralityTree("MbdCentralityTree", outfile));

  Fun4AllDstInputManager *incalo = new Fun4AllDstInputManager("DST_CALOFITTING");
  for (const std::string &f : calofiles)
  {
    incalo->AddFile(f);
  }
  se->registerInputManager(incalo);
  Fun4AllDstInputManager *inzdc = new Fun4AllDstInputManager("DST_ZDC_RAW");
  for (const std::string &f : zdcfiles)
  {
    inzdc->AddFile(f);
  }
  se->registerInputManager(inzdc);

  se->run(nevents);
  se->End();
  delete se;
  gSystem->Exit(0);
}
