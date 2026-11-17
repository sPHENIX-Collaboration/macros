#ifndef MACRO_FUN4ALLEMBEDFROMHITS_C
#define MACRO_FUN4ALLEMBEDFROMHITS_C

// Fun4All_Embed_fromHits.C -- redo the calorimeter part of Fun4All_G4_Embed_data.C
// (waveform sim, caloTowerEmbed, waveform fitting) from saved G4 hits, without rerunning
// the generator and Geant4.  For testing changes to the embedding.
//
// Inputs:
//   hits_file  G4 hits with G4HIT_CEMC, G4HIT_HCALIN and G4HIT_HCALOUT: the DST_TRUTH_G4HIT_*
//              of an embedding production (saved since 2026-11-12) or an MC G4Hits file
//   data_file  the prepared data DST (DST_CALOFITTING-*)
//   pedestal   pedestal DST; empty picks a random 2025 run 71666 file, as the production
//   production_calib  convert the sim energy to ADC with the MC default calibrations, as
//              the productions before 2026-11-12 did (see Fun4All_G4_Embed_data.C, DataCalibUrl)
//   pair_sequential  pair hits event i with data event i, for hits not made from this data
//              (MC G4Hits; the vertex then won't agree with the data)
//   save_sim_only  also fit the sim waveforms alone, before the embedding: TOWERSSIM_*
//   shift_mbd_t0  shift the sim pulses by the MBD t0 of the data event (instead of the
//              random 0-0.2 sample shift), so they follow the data tower times
//
// Hits from an embedding production carry the data EventHeader, so each hits event names
// its data event.  The data input is skipped to the first one and the events are checked
// to follow each other; the run stops before the first event that doesn't.
//
// Output: DST_CALO_REDO_<hits name>-data-<run>-<segment>.root with TOWERS_CEMC,
// TOWERS_HCALIN and TOWERS_HCALOUT, as the production DST_CALO (+ TOWERSSIM_*), and the data MbdOut.

#include <caloembedding/caloTowerEmbed.h>
#include <caloembedding/CopyIODataNodes.h>

#include <calowaveformsim/CaloWaveformSim.h>

#include <caloreco/CaloTowerBuilder.h>
#include <caloreco/CaloTowerDefs.h>
#include <caloreco/CaloWaveformProcessing.h>

#include <g4detectors/PHG4FullProjSpacalCellReco.h>

#include <ffamodules/CDBInterface.h>
#include <ffamodules/FlagHandler.h>
#include <ffamodules/SyncReco.h>

#include <ffaobjects/EventHeader.h>

#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllNoSyncDstInputManager.h>
#include <fun4all/Fun4AllRunNodeInputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllSyncManager.h>
#include <fun4all/Fun4AllUtils.h>

#include <sphenixnpc/SphenixClient.h>

#include <phool/PHRandomSeed.h>
#include <phool/recoConsts.h>

#include <TFile.h>
#include <TInterpreter.h>
#include <TRandom3.h>
#include <TTree.h>

#include <format>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libffaobjects.so)
R__LOAD_LIBRARY(libg4dst.so)
R__LOAD_LIBRARY(libg4detectors.so)
R__LOAD_LIBRARY(libCaloWaveformSim.so)
R__LOAD_LIBRARY(libcalo_reco.so)
R__LOAD_LIBRARY(libsphenixnpc.so)
R__LOAD_LIBRARY(libCaloEmbedding.so)

// event sequence numbers of a DST, from its EventHeader
std::vector<int> EventSequences(const std::string &fname)
{
  std::vector<int> seq;
  TFile *f = TFile::Open(fname.c_str());
  if (!f || !f->IsOpen())
  {
    std::cout << "cannot open " << fname << std::endl;
    return seq;
  }
  TTree *t = dynamic_cast<TTree *>(f->Get("T"));
  EventHeader *evthead = nullptr;
  if (!t || !t->GetBranch("DST#EventHeader"))
  {
    std::cout << "no DST#EventHeader in " << fname << std::endl;
    return seq;
  }
  t->SetBranchStatus("*", false);
  t->SetBranchStatus("DST#EventHeader", true);
  t->SetBranchAddress("DST#EventHeader", &evthead);
  for (Long64_t i = 0; i < t->GetEntries(); ++i)
  {
    t->GetEntry(i);
    seq.push_back(evthead->get_EvtSequence());
  }
  f->Close();
  delete f;
  return seq;
}

// does the run tree of a DST have this node
bool HasRunNode(const std::string &fname, const std::string &node)
{
  TFile *f = TFile::Open(fname.c_str());
  TTree *t = f ? dynamic_cast<TTree *>(f->Get("T1")) : nullptr;
  bool found = false;
  if (t)
  {
    for (auto *b : *t->GetListOfBranches())
    {
      std::string name = b->GetName();
      found |= (name.size() >= node.size() && name.compare(name.size() - node.size(), node.size(), node) == 0);
    }
  }
  delete f;
  return found;
}

// setters not in the release yet (coresoftware wf_sim: CaloWaveformSim::set_shift_by_mbd_t0) are
// called through the interpreter, so the macro still runs on the release with those options off.
// Call them directly once merged.
void CallNewSetter(const std::string &cls, void *obj, const std::string &call)
{
  TInterpreter::EErrorCode err = TInterpreter::kNoError;
  gInterpreter->ProcessLine(Form("((%s *) %p)->%s;", cls.c_str(), obj, call.c_str()), &err);
  if (err != TInterpreter::kNoError)
  {
    std::cout << cls << "::" << call << " needs the coresoftware wf_sim build" << std::endl;
    gSystem->Exit(1);
  }
}

int Fun4All_Embed_fromHits(
    const int nEvents = 10,
    const std::string &hits_file = "DST_TRUTH_G4HIT_Jet10-00000021-data-00068145-00000.root",
    const std::string &data_file = "DST_CALOFITTING-00068145-00000.root",
    const std::string &outdir = "./",
    const std::string &pedestal = "",
    const std::string &cdbtag = "MDC2",
    const bool production_calib = false,
    const bool pair_sequential = false,
    const bool save_sim_only = false,
    const bool shift_mbd_t0 = false)
{
  //-----------------
  // line up the hits events with their data events
  //-----------------
  std::vector<int> hitseq = EventSequences(hits_file);
  std::vector<int> dataseq = EventSequences(data_file);
  if (hitseq.empty() || dataseq.empty())
  {
    gSystem->Exit(1);
  }
  int dataskip = 0;
  int nmatched = std::min(hitseq.size(), dataseq.size());
  if (!pair_sequential)
  {
    dataskip = -1;
    for (unsigned int i = 0; i < dataseq.size(); ++i)
    {
      if (dataseq[i] == hitseq[0])
      {
        dataskip = i;
        break;
      }
    }
    if (dataskip < 0)
    {
      std::cout << "first hits event (sequence " << hitseq[0] << ") is not in " << data_file
                << "; pair_sequential = true for hits not made from this data" << std::endl;
      gSystem->Exit(1);
    }
    nmatched = 0;
    while (nmatched < (int) hitseq.size() && dataskip + nmatched < (int) dataseq.size() &&
           hitseq[nmatched] == dataseq[dataskip + nmatched])
    {
      ++nmatched;
    }
    if (nmatched < (int) hitseq.size())
    {
      std::cout << "hits event " << nmatched << " (sequence " << hitseq[nmatched]
                << ") doesn't follow on in the data, stopping there" << std::endl;
    }
  }
  int nrun = (nEvents > 0) ? std::min(nEvents, nmatched) : nmatched;
  std::cout << "hits events " << hitseq.size() << ", data skip " << dataskip << ", paired " << nmatched
            << (pair_sequential ? " (in sequence)" : " (by EventHeader)") << ", running " << nrun << std::endl;

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(1);
  CDBInterface::instance()->Verbosity(1);
  PHRandomSeed::Verbosity(1);

  std::pair<int, int> runseg = Fun4AllUtils::GetRunSegment(data_file);
  int dataRunNumber = runseg.first;
  int dataSegment = runseg.second;

  // as Fun4All_G4_Embed_data.C
  int runnumber = 21;
  recoConsts *rc = recoConsts::instance();
  rc->set_StringFlag("CDB_GLOBALTAG", cdbtag);
  rc->set_uint64Flag("TIMESTAMP", runnumber);
  rc->set_IntFlag("RUNNUMBER", runnumber);
  // the hits (MC run, e.g. 28) and the data come from different runs
  se->getSyncManager()->MixRunsOk(true);

  se->registerSubsystem(new SyncReco());
  se->registerSubsystem(new FlagHandler());

  std::string DataTopNode = "TOPData";
  CopyIODataNodes *cp = new CopyIODataNodes();
  cp->Verbosity(1);
  se->registerSubsystem(cp, DataTopNode);

  // data ADC to energy calibrations, falling back to <domain>_default as CDBInterface
  SphenixClient datacdb("newcdbtag");
  auto datacalib = [&](const std::string &domain)
  {
    if (production_calib)
    {
      // what the old tag switch got: the MC tag at the data run number
      rc->set_uint64Flag("TIMESTAMP", dataRunNumber);
      std::string url = CDBInterface::instance()->getUrl(domain);
      rc->set_uint64Flag("TIMESTAMP", runnumber);
      return url;
    }
    std::string url = datacdb.getCalibration(domain, dataRunNumber);
    if (url.empty())
    {
      url = datacdb.getCalibration(domain + "_default", dataRunNumber);
    }
    if (url.empty())
    {
      std::cout << "no data calibration " << domain << " for run " << dataRunNumber << std::endl;
      gSystem->Exit(1);
    }
    return url;
  };
  std::string cemc_datacalib = datacalib("CEMC_calib_ADC_to_ETower");
  std::string ohcal_datacalib = datacalib("HCALOUT_calib_ADC_to_ETower");
  std::string ihcal_datacalib = datacalib("HCALIN_calib_ADC_to_ETower");
  // data tower mean times for the sim pulse timing (production_calib: the MC tag's, 2024)
  std::string cemc_datatime = production_calib ? "" : datacalib("CEMC_meanTime");
  std::string ohcal_datatime = production_calib ? "" : datacalib("HCALOUT_meanTime");
  std::string ihcal_datatime = production_calib ? "" : datacalib("HCALIN_meanTime");
  std::cout << "sim energy to ADC with" << std::endl
            << "  " << cemc_datacalib << std::endl
            << "  " << ihcal_datacalib << std::endl
            << "  " << ohcal_datacalib << std::endl
            << "sim pulse times with" << std::endl
            << "  " << cemc_datatime << std::endl
            << "  " << ihcal_datatime << std::endl
            << "  " << ohcal_datatime << std::endl;

  // MC G4Hits files don't carry the CEMC cell geometry the waveform sim needs, the
  // cell reco makes it (as CEMC_Cells() in the MC production)
  if (!HasRunNode(hits_file, "CYLINDERCELLGEOM_CEMC"))
  {
    std::cout << "no CYLINDERCELLGEOM_CEMC in " << hits_file << ", running the CEMC cell reco" << std::endl;
    PHG4FullProjSpacalCellReco *cemc_cells = new PHG4FullProjSpacalCellReco("CEMCCYLCELLRECO");
    cemc_cells->Detector("CEMC");
    cemc_cells->get_light_collection_model().load_data_file(
        std::string(getenv("CALIBRATIONROOT")) + std::string("/CEMC/LightCollection/Prototype3Module.xml"),
        "data_grid_light_guide_efficiency", "data_grid_fiber_trans");
    se->registerSubsystem(cemc_cells);
  }

  //-----------------
  // waveform sim, embedding and fitting, as Fun4All_G4_Embed_data.C
  //-----------------
  const std::vector<std::pair<CaloTowerDefs::DetectorSystem, std::string>> dets = {
      {CaloTowerDefs::CEMC, cemc_datacalib},
      {CaloTowerDefs::HCALIN, ihcal_datacalib},
      {CaloTowerDefs::HCALOUT, ohcal_datacalib}};
  const std::vector<std::string> datatimes = {cemc_datatime, ihcal_datatime, ohcal_datatime};
  const std::vector<std::string> detnames = {"CEMC", "HCALIN", "HCALOUT"};
  // software ZS thresholds (ADC) of the fit, as macros/common/Calo_Fitting.C (and the caloTowerEmbed defaults)
  const std::vector<float> szs = {60, 30, 15};
  auto fitter = [&](CaloTowerDefs::DetectorSystem det, float szs_threshold, const std::string &outprefix)
  {
    CaloTowerBuilder *ca2 = new CaloTowerBuilder(std::string("BUILDER_") + outprefix + std::to_string(det));
    ca2->set_detector_type(det);
    ca2->set_dataflag(false);
    ca2->set_processing_type(CaloWaveformProcessing::TEMPLATE);
    ca2->set_builder_type(CaloTowerDefs::kWaveformTowerSimv1);
    ca2->set_outputNodePrefix(outprefix);
    ca2->set_softwarezerosuppression(true, szs_threshold);
    return ca2;
  };
  for (unsigned int i = 0; i < dets.size(); ++i)
  {
    CaloWaveformSim *caloWaveformSim = new CaloWaveformSim();
    caloWaveformSim->set_detector_type(dets[i].first);
    caloWaveformSim->set_detector(detnames[i]);
    caloWaveformSim->set_timewidth(shift_mbd_t0 ? 0. : 0.2);
    if (shift_mbd_t0)
    {
      CallNewSetter("CaloWaveformSim", caloWaveformSim, "set_shift_by_mbd_t0(true, \"" + DataTopNode + "\")");
    }
    caloWaveformSim->set_peakpos(6);
    caloWaveformSim->set_sampletime(17.7623);  // ns per sample, 6 x the 9.383 MHz beam clock (as MbdEvent); default is 16.67
    caloWaveformSim->set_noise_type(CaloWaveformSim::NOISE_NONE);
    caloWaveformSim->set_fixpedestal(0);  // data waveform already carries the pedestal
    caloWaveformSim->set_directURL_calib(dets[i].second);
    if (!datatimes[i].empty())
    {
      caloWaveformSim->set_directURL_timecalib(datatimes[i]);
    }
    se->registerSubsystem(caloWaveformSim);
  }
  if (save_sim_only)
  {
    // the noiseless sim waveforms alone, before caloTowerEmbed adds the data to them
    for (unsigned int i = 0; i < dets.size(); ++i)
    {
      se->registerSubsystem(fitter(dets[i].first, szs[i], "TOWERSSIM_"));
    }
  }
  for (unsigned int i = 0; i < dets.size(); ++i)
  {
    caloTowerEmbed *embedder = new caloTowerEmbed("embedder_" + detnames[i]);
    embedder->set_detector_type(dets[i].first);
    embedder->set_removeBadTowers(false);
    embedder->set_embedwaveform(true);
    embedder->set_inputNodePrefix("TOWERS_");
    se->registerSubsystem(embedder);
  }
  for (unsigned int i = 0; i < dets.size(); ++i)
  {
    se->registerSubsystem(fitter(dets[i].first, szs[i], "TOWERS_"));
  }

  //--------------
  // inputs
  //--------------
  // the hits carry the Sync node; the data and the pedestals follow event by event
  Fun4AllInputManager *hitsin = new Fun4AllDstInputManager("DSTHITS");
  hitsin->fileopen(hits_file);
  se->registerInputManager(hitsin);

  Fun4AllInputManager *datain = new Fun4AllNoSyncDstInputManager("DSTDATA", "DST", DataTopNode);
  datain->fileopen(data_file);
  se->registerInputManager(datain);
  if (dataskip > 0)
  {
    datain->skip(dataskip);
  }

  std::string pedestalname = pedestal;
  if (pedestalname.empty())
  {
    TRandom3 randGen;
    randGen.SetSeed(PHRandomSeed());
    pedestalname = std::format("/sphenix/tg/tg01/commissioning/CaloCalibWG/bseidlitz/pedestal_71666_v2/pedestal-00071666-{:05}.root", randGen.Integer(1000));
  }
  std::cout << "pedestal file " << pedestalname << std::endl;
  Fun4AllInputManager *pedin = new Fun4AllNoSyncDstInputManager("DSTPED");
  pedin->AddFile(pedestalname);
  pedin->Repeat();
  se->registerInputManager(pedin);

  Fun4AllInputManager *ingeo = new Fun4AllRunNodeInputManager("DST_GEO");
  ingeo->AddFile(CDBInterface::instance()->getUrl("calo_geo"));
  se->registerInputManager(ingeo);

  //--------------
  // output
  //--------------
  std::string hitsbase = hits_file.substr(hits_file.rfind('/') + 1);
  hitsbase = std::regex_replace(hitsbase, std::regex("^(DST_TRUTH_G4HIT_|G4Hits_)"), "");
  hitsbase = std::regex_replace(hitsbase, std::regex("(-data-\\d+)?-\\d+\\.root$"), "");
  std::string outfile = std::format("{}/DST_CALO_REDO_{}-data-{:08}-{:05}.root", outdir, hitsbase, dataRunNumber, dataSegment);
  Fun4AllDstOutputManager *out = new Fun4AllDstOutputManager("CALOPROCESSED", outfile);
  out->AddNode("Sync");
  out->AddNode("EventHeader");
  // the data MbdOut (CopyIODataNodes): the calibration picks the ZS cross calibration by bunch t0
  out->AddNode("MbdOut");
  for (const auto &det : detnames)
  {
    out->AddNode("TOWERS_" + det);
    if (save_sim_only)
    {
      out->AddNode("TOWERSSIM_" + det);
    }
  }
  se->registerOutputManager(out);

  se->run(nrun);
  se->PrintTimer();
  CDBInterface::instance()->Print();
  se->End();
  std::cout << "All done" << std::endl;
  delete se;
  gSystem->Exit(0);
  return 0;
}

#endif
