#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllPrdfInputManager.h>

#include <phool/recoConsts.h>

#include <prototype2/CaloCalibration.h>
#include <prototype2/CaloUnpackPRDF.h>
#include <prototype2/GenericUnpackPRDF.h>
#include <prototype2/PROTOTYPE2_FEM.h>
#include <prototype2/Prototype2DSTReader.h>
#include <prototype2/RunInfoUnpackPRDF.h>
#include <prototype2/TempInfoUnpackPRDF.h>

R__LOAD_LIBRARY(libPrototype2.so)
#endif

void
Fun4All_TestBeam(int nEvents = 100,
    const char *input_file = "/sphenix/data/data01/t1044-2016a/fnal/beam/beam_00002609-0000.prdf",
    const char *output_file = "beam_00002609.root")
{
  gSystem->Load("libPrototype2.so");

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(Fun4AllServer::VERBOSITY_SOME);

  recoConsts *rc = recoConsts::instance();
  //rc->set_IntFlag("RUNNUMBER",0);

  // ------------------- Run info -> RUN node -------------------
  RunInfoUnpackPRDF *unpack_run = new RunInfoUnpackPRDF();
//  unpack_run->Verbosity(RunInfoUnpackPRDF::VERBOSITY_SOME);

  int i_offset = 0;

  //    rcdaq_client create_device device_filenumbers_delete 9 911 "$HOME/beam_values.txt"
  //  S:MTNRG  =  120   GeV
  //  F:MT6SC1 =  11127     Cnts
  //  F:MT6SC2 =  10585     Cnts
  //  F:MT6SC3 =  10442     Cnts
  //  F:MT6SC4 =  0         Cnts
  //  F:MT6SC5 =  20251     Cnts
  //  E:2CH    =  981.9 mm
  //  E:2CV    =  93.17 mm
  //  E:2CMT6T =  76.11 F
  //  E:2CMT6H =  18.09 %Hum
  //  F:MT5CP2 =  .0301 Psia
  //  F:MT6CP2 =  .6905 Psia
  i_offset = 0;
  unpack_run->add_channel("beam_MTNRG_GeV", 911, i_offset++, 1e-4);
  unpack_run->add_channel("beam_MT6SC1_Cnts", 911, i_offset++, 1e-4);
  unpack_run->add_channel("beam_MT6SC2_Cnts", 911, i_offset++, 1e-4);
  unpack_run->add_channel("beam_MT6SC3_Cnts", 911, i_offset++, 1e-4);
  unpack_run->add_channel("beam_MT6SC4_Cnts", 911, i_offset++, 1e-4);
  unpack_run->add_channel("beam_MT6SC5_Cnts", 911, i_offset++, 1e-4);
  unpack_run->add_channel("beam_2CH_mm", 911, i_offset++, 1e-4);
  unpack_run->add_channel("beam_2CV_mm", 911, i_offset++, 1e-4);
  unpack_run->add_channel("beam_2CMT6T_F", 911, i_offset++, 1e-4);
  unpack_run->add_channel("beam_2CMT6H_RH", 911, i_offset++, 1e-4);
  unpack_run->add_channel("beam_MT5CP2_Psia", 911, i_offset++, 1e-4);
  unpack_run->add_channel("beam_MT6CP2_Psia", 911, i_offset++, 1e-4);

//  rcdaq_client create_device device_filenumbers_delete 9 984 "$HOME/DB_LOGGER_EMCAL_A0_values.txt"
  unpack_run->add_channel("EMCAL_A0_HighGain", 984, 0, 1); // 1: pre-amp high gain, 0: nominal gain

  //  rcdaq_client create_device device_filenumbers_delete 9 983 "$HOME/DB_LOGGER_EMCAL_GR0.txt"
  unpack_run->add_channel("EMCAL_GR0_BiasOffset_Tower21", 983, 21-1, 1); // bias offset in mV for tower 21

    // rcdaq_client create_device device_filenumbers_delete 9 982 "$HOME/DB_LOGGER_EMCAL_T0_values.txt"
  unpack_run->add_channel("EMCAL_T0_Tower21", 982, 21-1, 1e-3); // temperature reading in C for tower 21

  se->registerSubsystem(unpack_run);

  // ------------------- Temperature output -------------------
  TempInfoUnpackPRDF *unpack_temp = new TempInfoUnpackPRDF();
  unpack_temp->Verbosity(RunInfoUnpackPRDF::VERBOSITY_SOME);
  se->registerSubsystem(unpack_temp);

  // ------------------- HCal and EMcal -------------------
  SubsysReco *unpack = new CaloUnpackPRDF();
// unpack->Verbosity(1);
  se->registerSubsystem(unpack);

  CaloCalibration *calib = new CaloCalibration("CEMC");
  calib->GetCalibrationParameters().ReadFromFile("CEMC","xml",0,0,
      string(getenv("CALIBRATIONROOT")) + string("/Prototype2/Calibration/")); // calibration database
  se->registerSubsystem(calib);

  calib = new CaloCalibration("HCALIN");
  calib->set_calib_tower_node_prefix("CALIB_LG");
  calib->set_raw_tower_node_prefix("RAW_LG");
  calib->GetCalibrationParameters().set_name("hcalin_lg");
  calib->GetCalibrationParameters().ReadFromFile("hcalin_lg", "xml",0,0,
      string(getenv("CALIBRATIONROOT")) + string("/Prototype2/Calibration/")); // calibration database
  se->registerSubsystem(calib);

  calib = new CaloCalibration("HCALIN");
  calib->set_calib_tower_node_prefix("CALIB_HG");
  calib->set_raw_tower_node_prefix("RAW_HG");
  calib->GetCalibrationParameters().set_name("hcalin_hg");
  calib->GetCalibrationParameters().ReadFromFile("hcalin_hg", "xml",0,0,
      string(getenv("CALIBRATIONROOT")) + string("/Prototype2/Calibration/")); // calibration database
  se->registerSubsystem(calib);

  calib = new CaloCalibration("HCALOUT");
  calib->set_calib_tower_node_prefix("CALIB_LG");
  calib->set_raw_tower_node_prefix("RAW_LG");
  calib->GetCalibrationParameters().set_name("hcalout_lg");
  calib->GetCalibrationParameters().ReadFromFile("hcalout_lg", "xml",0,0,
      string(getenv("CALIBRATIONROOT")) + string("/Prototype2/Calibration/")); // calibration database
  se->registerSubsystem(calib);

  calib = new CaloCalibration("HCALOUT");
  calib->set_calib_tower_node_prefix("CALIB_HG");
  calib->set_raw_tower_node_prefix("RAW_HG");
  calib->GetCalibrationParameters().set_name("hcalout_hg");
  calib->GetCalibrationParameters().ReadFromFile("hcalout_hg", "xml",0,0,
      string(getenv("CALIBRATIONROOT")) + string("/Prototype2/Calibration/")); // calibration database
  se->registerSubsystem(calib);

  // ------------------- Hodoscpes -------------------

  const int first_packet_id = PROTOTYPE2_FEM::PACKET_ID; // 21101
  const int second_packet_id = 21102;

  const int N_hodo = 8;

  GenericUnpackPRDF *gunpack = new GenericUnpackPRDF("HODO_VERTICAL");
  for (int i = 0; i < N_hodo; ++i)
    gunpack->add_channel(first_packet_id, 104 + i, i); // 24 Cerenkov 1
  se->registerSubsystem(gunpack);

  gunpack = new GenericUnpackPRDF("HODO_HORIZONTAL");
  for (int i = 0; i < N_hodo; ++i)
    gunpack->add_channel(first_packet_id, 96 + i, i); // 24 Cerenkov 1
  se->registerSubsystem(gunpack);

  calib = new CaloCalibration("HODO_VERTICAL");
  calib->GetCalibrationParameters().set_int_param("use_chan_calibration", 1);
  // Martin find that even channel has negative polarity and odd channel has positive polarity
  for (int i = 0; i < N_hodo; ++i)
    calib->GetCalibrationParameters().set_double_param(
        Form("calib_const_column0_row%d", i), ((i % 2 > 0) ? -1 : +1));
  se->registerSubsystem(calib);

  calib = new CaloCalibration("HODO_HORIZONTAL");
  calib->GetCalibrationParameters().set_int_param("use_chan_calibration", 1);
  // Martin find that even channel has negative polarity and odd channel has positive polarity
  for (int i = 0; i < N_hodo; ++i)
    calib->GetCalibrationParameters().set_double_param(
        Form("calib_const_column0_row%d", i), ((i % 2 > 0) ? -1 : +1));
  se->registerSubsystem(calib);

  // ------------------- Other detectors -------------------

  gunpack = new GenericUnpackPRDF("C1");
// unpack->Verbosity(1);
  gunpack->add_channel(second_packet_id, 24, 0); // 24 Cerenkov 1
  se->registerSubsystem(gunpack);

  calib = new CaloCalibration("C1");
  se->registerSubsystem(calib);

  // more info see https://wiki.bnl.gov/sPHENIX/index.php/T-1044#Cerenkov_Counters
  gunpack = new GenericUnpackPRDF("C2");
// unpack->Verbosity(1);
  gunpack->add_channel(second_packet_id, 25, 0); //25 Cerenkov 2 Inner
  gunpack->add_channel(second_packet_id, 26, 1); //26  Cerenkov 2 Outer
  gunpack->add_channel(second_packet_id, 22, 10); //Channel 22 C2inner earlier copy added before run 2210
  gunpack->add_channel(second_packet_id, 23, 11); //Channel 23 C2outer earlier copy added before run 2210
  se->registerSubsystem(gunpack);

  calib = new CaloCalibration("C2");
  calib->GetCalibrationParameters().set_double_param("calib_const_scale", 1);
  calib->GetCalibrationParameters().set_int_param("use_chan_calibration", 1);
  calib->GetCalibrationParameters().set_double_param("calib_const_column0_row0", +1);
  calib->GetCalibrationParameters().set_double_param("calib_const_column0_row1", -1);
  calib->GetCalibrationParameters().set_double_param("calib_const_column0_row10", -1);
  calib->GetCalibrationParameters().set_double_param("calib_const_column0_row11", +1);
  se->registerSubsystem(calib);

//  John H. : should be 19, 20, 21 and the other channels are a litle permuted from  what I thought
  gunpack = new GenericUnpackPRDF("HCAL_SCINT");
// unpack->Verbosity(1);
  gunpack->add_channel(second_packet_id, 19, 1);
  gunpack->add_channel(second_packet_id, 20, 2);
  gunpack->add_channel(second_packet_id, 21, 3);
  se->registerSubsystem(gunpack);

  calib = new CaloCalibration("HCAL_SCINT");
  se->registerSubsystem(calib);

  gunpack = new GenericUnpackPRDF("PbGL");
// unpack->Verbosity(1);
  gunpack->add_channel(second_packet_id, 0, 0); // 0 PbGL  Only inserted in beam for testing
  se->registerSubsystem(gunpack);

  calib = new CaloCalibration("PbGL");
  se->registerSubsystem(calib);

  gunpack = new GenericUnpackPRDF("TRIGGER_VETO");
// unpack->Verbosity(1);
  gunpack->add_channel(second_packet_id, 28, 0); //  28  Bottom trigger veto
  gunpack->add_channel(second_packet_id, 29, 1); //  29  Top trigger veto
  gunpack->add_channel(second_packet_id, 30, 2); //  30  Left trigger veto
  gunpack->add_channel(second_packet_id, 31, 3); //  31  Right trigger veto
  se->registerSubsystem(gunpack);

  // Calibrate the MIP peak to an relative energy of +1.0
  calib = new CaloCalibration("TRIGGER_VETO");
  calib->GetCalibrationParameters().set_double_param("calib_const_scale", 1);
  calib->GetCalibrationParameters().set_int_param("use_chan_calibration", 1);
  calib->GetCalibrationParameters().set_double_param("calib_const_column0_row0", -1./29.4155);
  calib->GetCalibrationParameters().set_double_param("calib_const_column0_row1", +1./91);
  calib->GetCalibrationParameters().set_double_param("calib_const_column0_row2", -1./31.3981);
  calib->GetCalibrationParameters().set_double_param("calib_const_column0_row3", +1./1.43839e+02);
  se->registerSubsystem(calib);

  const int N_TileMapper = 16;

  gunpack = new GenericUnpackPRDF("TILE_MAPPER");
  for (int i = 0; i < N_TileMapper; ++i)
    gunpack->add_channel(second_packet_id, 32 + i, i); // 24 Cerenkov 1
  se->registerSubsystem(gunpack);

  calib = new CaloCalibration("TILE_MAPPER");
  se->registerSubsystem(calib);

  // -------------------  Output -------------------
  //main DST output
  Fun4AllOutputManager *out_Manager = new Fun4AllDstOutputManager("DSTOUT",output_file);
  se->registerOutputManager(out_Manager);

  //alternatively, fast check on DST using DST Reader:
  Prototype2DSTReader *reader = new Prototype2DSTReader(
      string(output_file) + string("_DSTReader.root"));

  reader->AddRunInfo("beam_MTNRG_GeV");
  reader->AddRunInfo("beam_2CH_mm");
  reader->AddRunInfo("beam_2CV_mm");
  reader->AddRunInfo("EMCAL_A0_HighGain");
  reader->AddRunInfo("EMCAL_GR0_BiasOffset_Tower21");
  reader->AddRunInfo("EMCAL_T0_Tower21");

  reader->AddTower("RAW_LG_HCALIN");
  reader->AddTower("RAW_HG_HCALIN");
  reader->AddTower("RAW_LG_HCALOUT");
  reader->AddTower("RAW_HG_HCALOUT");

  reader->AddTower("CALIB_LG_HCALIN");
  reader->AddTower("CALIB_HG_HCALIN");
  reader->AddTower("CALIB_LG_HCALOUT");
  reader->AddTower("CALIB_HG_HCALOUT");

  reader->AddTower("RAW_CEMC");
  reader->AddTower("CALIB_CEMC");

  reader->AddTower("RAW_HODO_VERTICAL");
  reader->AddTower("RAW_HODO_HORIZONTAL");
  reader->AddTower("CALIB_HODO_VERTICAL");
  reader->AddTower("CALIB_HODO_HORIZONTAL");

  reader->AddTower("RAW_C1");
  reader->AddTower("CALIB_C1");

  reader->AddTower("RAW_C2");
  reader->AddTower("CALIB_C2");

  reader->AddTower("RAW_HCAL_SCINT");
  reader->AddTower("CALIB_HCAL_SCINT");

  reader->AddTower("RAW_PbGL");
  reader->AddTower("CALIB_PbGL");

  reader->AddTower("RAW_TRIGGER_VETO");
  reader->AddTower("CALIB_TRIGGER_VETO");

  reader->AddTower("RAW_TILE_MAPPER");
  reader->AddTower("CALIB_TILE_MAPPER");

  reader->AddTowerTemperature("HCALIN");
  reader->AddTowerTemperature("HCALIN");
  reader->AddTowerTemperature("HCALOUT");

  se->registerSubsystem(reader);

  Fun4AllInputManager *in = new Fun4AllPrdfInputManager("PRDFin");
  in->fileopen(input_file);
  se->registerInputManager(in);

  se->run(nEvents);

  se->End();

}
