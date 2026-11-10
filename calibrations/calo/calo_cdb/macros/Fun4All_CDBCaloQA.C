#ifndef FUN4ALL_CDBCALOQA_C
#define FUN4ALL_CDBCALOQA_C

#include <Calo_Calib.C>

#include <mbd/MbdReco.h>

#include <zdcinfo/ZdcReco.h>

#include <epd/EpdReco.h>

#include <calotrigger/MinimumBiasClassifier.h>

#include <centrality/CentralityReco.h>

#include <globalvertex/GlobalVertex.h>
#include <globalvertex/GlobalVertexReco.h>

#include <caloreco/CaloTowerCalib.h>

#include <jetbackground/FastJetAlgoSub.h>
#include <jetbackground/RetowerCEMC.h>

#include <jetbase/JetReco.h>
#include <jetbase/TowerJetInput.h>

#include <calovalid/CDBCaloQA.h>

#include <ffamodules/CDBInterface.h>
#include <ffamodules/FlagHandler.h>

#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllServer.h>

#include <phool/recoConsts.h>

#include <fstream>
#include <sstream>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libcalo_reco.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libmbd.so)
R__LOAD_LIBRARY(libglobalvertex.so)
R__LOAD_LIBRARY(libzdcinfo.so)
R__LOAD_LIBRARY(libepd.so)
R__LOAD_LIBRARY(libcalotrigger.so)
R__LOAD_LIBRARY(libcentrality.so)
R__LOAD_LIBRARY(libjetbase.so)
R__LOAD_LIBRARY(libjetbackground.so)
R__LOAD_LIBRARY(libcalovalid.so)

// CDB calorimeter QA of one run: HIST_CDBCALOQA_<production>-<run>.root in the working directory
// period: Run24pp, Run24AuAu, Run25OO, Run25pp, Run25AuAu, or MC_pp, MC_AuAu, MC_OO (file lists in $CDBQA_LISTS)
// nMax: events read at most
void Fun4All_CDBCaloQA(const std::string &period = "Run24pp", int runnumber = 47289, const std::string &dbtag = "newcdbtag",
                       int nMax = 200000)
{
  Fun4AllServer *se = Fun4AllServer::instance();
  recoConsts *rc = recoConsts::instance();

  std::string production;
  std::string trig_photon;
  std::string trig_jet;
  bool do_centrality = false;
  bool sepd_raw = false;
  bool cent_sum_all = false;
  MinimumBiasInfo::SPECIES mbspecies = MinimumBiasInfo::SPECIES::PP;
  int nMB = 3000;
  int nZdc = 0;
  int nTrig = 3000;
  float cent_vzcut = 1e9;
  const bool mc = (period.rfind("MC_", 0) == 0);
  if (period == "Run24pp" || period == "Run25pp")
  {
    production = (period == "Run24pp") ? "run2pp_pro001_pcdb001_v001" : "run3pp_pro001_pcdb001_v001";
    trig_photon = "Photon 4 GeV + MBD NS >= 1";
    trig_jet = "Jet 10 GeV + MBD NS >= 1";
  }
  else if (period == "Run24AuAu" || period == "Run25AuAu")
  {
    production = (period == "Run24AuAu") ? "run2auau_pro001_pcdb001_v001" : "run3auau_pro001_pcdb001_v001";
    do_centrality = true;
    sepd_raw = true;
    mbspecies = MinimumBiasInfo::SPECIES::AUAU;
    nMB = 10000;
    nZdc = 2000;
  }
  else if (period == "Run25OO")
  {
    production = "run3oo_pro001_pcdb001_v001";
    do_centrality = true;
    cent_sum_all = true;
    mbspecies = MinimumBiasInfo::SPECIES::OO;
    cent_vzcut = 60;
    nMB = 10000;
  }
  else if (period == "MC_pp" || period == "MC_AuAu" || period == "MC_OO")
  {
    production = (period == "MC_pp") ? "pythia8_Detroit" : (period == "MC_AuAu" ? "sHijing_0_20fm" : "sHijing_OO_0_15fm");
    nMB = 10000;
    nMax = nMB;
  }
  else
  {
    std::cout << "period " << period << " not set up" << std::endl;
    gSystem->Exit(1);
  }

  std::vector<std::string> calofiles;
  std::vector<std::string> zdcfiles;
  std::vector<std::string> sepdfiles;
  std::vector<std::string> mbdfiles;
  std::vector<std::string> globalfiles;
  const char *base = gSystem->Getenv("CDBQA_BASE");
  if (mc)
  {
    const char *lists = gSystem->Getenv("CDBQA_LISTS");
    const std::string lbase = Form("%s/%s_run%d_", lists ? lists : ".", period.c_str(), runnumber);
    for (auto [dst, v] : {std::make_pair("DST_CALO_CLUSTER", &calofiles), std::make_pair("DST_MBD_EPD", &mbdfiles),
                          std::make_pair("DST_GLOBAL", &globalfiles)})
    {
      std::ifstream in(lbase + dst + ".list");
      for (std::string f; in >> f;)
      {
        v->push_back(f);
      }
    }
  }
  else
  {
    // segments with DST_ZDC_RAW (and DST_SEPD_RAW for Au+Au); segments on a Lustre OST listed in $CDBQA_BASE/lists/bad_osts.txt are skipped
    const std::string prodtype = production.substr(0, production.find('_'));
    const std::string version = production.substr(production.find('_') + 1);
    const std::string dstdir = "/sphenix/lustre01/sphnxpro/production/" + prodtype + "/physics/" + version + "/DST_CALOFITTING";
    const std::string badosts = Form("%s/lists/bad_osts.txt", base ? base : ".");
    const bool checkost = !gSystem->AccessPathName(badosts.c_str());
    std::istringstream ls(gSystem->GetFromPipe(Form("ls %s/run_*/DST_CALOFITTING_%s-%08d-*.root 2> /dev/null | sort",
                                                    dstdir.c_str(), production.c_str(), runnumber)).Data());
    for (std::string f; ls >> f;)
    {
      std::string z = f;
      z.replace(z.rfind("DST_CALOFITTING"), 15, "DST_ZDC_RAW");
      std::string s = f;
      s.replace(s.rfind("DST_CALOFITTING"), 15, "DST_SEPD_RAW");
      if (gSystem->AccessPathName(z.c_str()) || (sepd_raw && gSystem->AccessPathName(s.c_str())))
      {
        continue;
      }
      if (checkost)
      {
        const TString ost = gSystem->GetFromPipe(Form("lfs getstripe -i %s %s 2> /dev/null | grep -xFf %s; true", f.c_str(), z.c_str(), badosts.c_str()));
        if (ost.Length() > 0)
        {
          std::cout << "skip segment on OST " << ost << ": " << f << std::endl;
          continue;
        }
      }
      calofiles.push_back(f);
      zdcfiles.push_back(z);
      if (sepd_raw)
      {
        sepdfiles.push_back(s);
      }
    }
  }
  if (calofiles.empty())
  {
    std::cout << "run " << runnumber << ": no input" << std::endl;
    gSystem->Exit(1);
  }
  const std::string outfile = Form("HIST_CDBCALOQA_%s-%08d.root", production.c_str(), runnumber);
  std::cout << period << " run " << runnumber << ": " << calofiles.size() << " segments -> " << outfile << std::endl;

  rc->set_StringFlag("CDB_GLOBALTAG", dbtag);
  rc->set_uint64Flag("TIMESTAMP", runnumber);
  CDBInterface::instance()->Verbosity(1);

  se->registerSubsystem(new FlagHandler());

  // events still needed (MB, Au+Au ZDC, p+p photon and jet) go on to the reconstruction, all others are dropped here
  if (!mc)
  {
    CDBCaloQA *sel = new CDBCaloQA("CDBCaloQASelect");
    sel->set_select_only(true);
    sel->set_trigger_photon(trig_photon);
    sel->set_trigger_jet(trig_jet);
    sel->set_min_mb(nMB);
    sel->set_min_zdc(nZdc);
    sel->set_min_trig(nTrig);
    se->registerSubsystem(sel);

    se->registerSubsystem(new MbdReco());
    se->registerSubsystem(new GlobalVertexReco());
  }

  Process_Calo_Calib();
  // ZDC: FermiExp fit for all species, as in the ZDC_SatelliteCorr calibration
  if (CaloTowerBuilder *zdcb = dynamic_cast<CaloTowerBuilder *>(se->getSubsysReco("ZDCBUILDER")))
  {
    zdcb->set_processing_type(CaloWaveformProcessing::FUNCFIT);
    zdcb->set_funcfit_type(2);
  }

  if (!mc)
  {
    ZdcReco *zdcreco = new ZdcReco();
    zdcreco->set_zdc1_cut(0.0);
    zdcreco->set_zdc2_cut(0.0);
    se->registerSubsystem(zdcreco);
  }
  if (sepd_raw)
  {
    CaloTowerBuilder *caEPD = new CaloTowerBuilder("SEPDBUILDER");
    caEPD->set_detector_type(CaloTowerDefs::SEPD);
    caEPD->set_builder_type(CaloTowerDefs::kPRDFTowerv4);
    caEPD->set_processing_type(CaloWaveformProcessing::TEMPLATE);
    se->registerSubsystem(caEPD);
  }
  if (!mc)
  {
    se->registerSubsystem(new EpdReco());
  }
  if (do_centrality)
  {
    MinimumBiasClassifier *mbc = new MinimumBiasClassifier();
    mbc->setSpecies(mbspecies);
    se->registerSubsystem(mbc);
    CentralityReco *cent = new CentralityReco();
    cent->set_sum_all_channels(cent_sum_all);
    se->registerSubsystem(cent);
  }

  // towers again without the ZS cross calibration
  for (auto [det, n] : {std::make_pair(CaloTowerDefs::CEMC, "CEMC"), std::make_pair(CaloTowerDefs::HCALOUT, "HCALOUT"),
                        std::make_pair(CaloTowerDefs::HCALIN, "HCALIN")})
  {
    CaloTowerCalib *c = new CaloTowerCalib(std::string(n) + "_NOZSXCALIB");
    c->set_detector_type(det);
    c->set_outputNodePrefix("TOWERINFO_NOZSXCALIB_");
    c->set_doZScrosscalib(false);
    se->registerSubsystem(c);
  }

  // unsubtracted R=0.4 jets
  RetowerCEMC *rcemc = new RetowerCEMC();
  rcemc->set_towerinfo(true);
  rcemc->set_frac_cut(0.5);
  rcemc->set_towerNodePrefix("TOWERINFO_CALIB");
  se->registerSubsystem(rcemc);
  JetReco *jetreco = new JetReco();
  for (auto src : {Jet::CEMC_TOWERINFO_RETOWER, Jet::HCALIN_TOWERINFO, Jet::HCALOUT_TOWERINFO})
  {
    TowerJetInput *in = new TowerJetInput(src, "TOWERINFO_CALIB");
    in->set_GlobalVertexType(GlobalVertex::MBD);
    jetreco->add_input(in);
  }
  jetreco->add_algo(new FastJetAlgoSub(Jet::ANTIKT, 0.4), "AntiKt_unsubtracted_r04");
  jetreco->set_algo_node("ANTIKT");
  jetreco->set_input_node("TOWER");
  se->registerSubsystem(jetreco);

  CDBCaloQA *qa = new CDBCaloQA("CDBCaloQA", outfile);
  qa->set_trigger_photon(trig_photon);
  qa->set_trigger_jet(trig_jet);
  qa->set_min_mb(nMB);
  qa->set_min_zdc(nZdc);
  qa->set_min_trig(nTrig);
  qa->set_centrality_vz_cut(cent_vzcut);
  if (mc)
  {
    qa->set_mc(true);
    qa->set_species(period.substr(3));
  }
  se->registerSubsystem(qa);

  auto addInput = [se](const std::string &name, const std::vector<std::string> &files)
  {
    if (files.empty())
    {
      return;
    }
    Fun4AllInputManager *in = new Fun4AllDstInputManager(name);
    for (const auto &f : files)
    {
      in->AddFile(f);
    }
    se->registerInputManager(in);
  };
  addInput("in", calofiles);
  addInput("inzdc", zdcfiles);
  addInput("insepd", sepdfiles);
  addInput("inmbd", mbdfiles);
  addInput("inglobal", globalfiles);

  se->run(nMax);
  se->End();
  CDBInterface::instance()->Print();
  delete se;
  gSystem->Exit(0);
}

#endif
