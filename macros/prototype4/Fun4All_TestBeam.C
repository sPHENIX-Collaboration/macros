#include <string>

using namespace std;

void Fun4All_TestBeam(int nEvents = 100,
                      const char *input_file = "/sphenix/data/data03//phnxreco/sphenix/t1044/fnal/beam/beam_00000406-0000.prdf",
                      const char *output_file = "data/beam_00000406.root")
{
  gSystem->Load("libfun4all");
  gSystem->Load("libPrototype4.so");

  const bool do_cemc = true;
  const bool do_hcal = true;
  //  const bool do_cemc = false;
  //  const bool do_hcal = false;

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
  unpack_run->add_channel("EMCAL_A0_HighGain", 984, 0, 1);  // 1: pre-amp high gain, 0: nominal gain

  //  rcdaq_client create_device device_filenumbers_delete 9 983 "$HOME/DB_LOGGER_EMCAL_GR0.txt"
  unpack_run->add_channel("EMCAL_GR0_BiasOffset_Tower21", 983, 21 - 1, 1);  // bias offset in mV for tower 21

  // rcdaq_client create_device device_filenumbers_delete 9 982 "$HOME/DB_LOGGER_EMCAL_T0_values.txt"
  unpack_run->add_channel("EMCAL_T0_Tower21", 982, 21 - 1, 1e-3);  // temperature reading in C for tower 21
  //
  se->registerSubsystem(unpack_run);
  //
  // ------------------- Temperature output -------------------
  //  TempInfoUnpackPRDF *unpack_temp = new TempInfoUnpackPRDF();
  //  unpack_temp->Verbosity(RunInfoUnpackPRDF::VERBOSITY_SOME);
  //  se->registerSubsystem(unpack_temp);

  // ------------------- HCal and EMcal -------------------

  CaloCalibration *calib = NULL;

  if (do_cemc)
  {
    SubsysReco *unpack = new CaloUnpackPRDF();
    //  unpack->Verbosity(1);
    se->registerSubsystem(unpack);

    calib = new CaloCalibration("CEMC");
    //    calib->Verbosity(4);
    //    calib->GetCalibrationParameters().set_double_param("calib_const_scale", 8. / 3000);
    //    calib->GetCalibrationParameters().set_int_param("use_chan_calibration", 0);
    calib->GetCalibrationParameters().ReadFromFile("CEMC", "xml", 0, 0,
                                                   string(getenv("CALIBRATIONROOT")) + string("/Prototype4/Calibration/"));  // calibration database
    se->registerSubsystem(calib);
  }

  if (do_hcal)
  {
    // leading order energy scale from Xu Sun
    const double cin_cali = 0.00270145;
    const double cout_cali = 0.0065718;

    calib = new CaloCalibration("HCALIN");
    calib->set_calib_tower_node_prefix("CALIB_LG");
    calib->set_raw_tower_node_prefix("RAW_LG");
    calib->GetCalibrationParameters().set_name("hcalin_lg");
    calib->GetCalibrationParameters().set_double_param("calib_const_scale", cin_cali);
    calib->GetCalibrationParameters().set_int_param("use_chan_calibration", 0);
    //  calib->GetCalibrationParameters().ReadFromFile("hcalin_lg", "xml", 0, 0,
    //                                                 string(getenv("CALIBRATIONROOT")) + string("/Prototype3/Calibration/"));  // calibration database
    se->registerSubsystem(calib);

    //  calib = new CaloCalibration("HCALIN");
    //  calib->set_calib_tower_node_prefix("CALIB_HG");
    //  calib->set_raw_tower_node_prefix("RAW_HG");
    //  calib->GetCalibrationParameters().set_name("hcalin_hg");
    //  calib->GetCalibrationParameters().ReadFromFile("hcalin_hg", "xml", 0, 0,
    //                                                 string(getenv("CALIBRATIONROOT")) + string("/Prototype3/Calibration/"));  // calibration database
    //  se->registerSubsystem(calib);

    calib = new CaloCalibration("HCALOUT");
    calib->set_calib_tower_node_prefix("CALIB_LG");
    calib->set_raw_tower_node_prefix("RAW_LG");
    calib->GetCalibrationParameters().set_name("hcalout_lg");
    calib->GetCalibrationParameters().set_double_param("calib_const_scale", cout_cali);
    calib->GetCalibrationParameters().set_int_param("use_chan_calibration", 0);
    //  calib->GetCalibrationParameters().ReadFromFile("hcalout_lg", "xml", 0, 0,
    //                                                 string(getenv("CALIBRATIONROOT")) + string("/Prototype3/Calibration/"));  // calibration database
    se->registerSubsystem(calib);

    calib = new CaloCalibration("HCALOUT");
    calib->set_calib_tower_node_prefix("CALIB_HG");
    calib->set_raw_tower_node_prefix("RAW_HG");
    calib->GetCalibrationParameters().set_name("hcalout_hg");
    calib->GetCalibrationParameters().set_double_param("calib_const_scale", cout_cali / 32);
    calib->GetCalibrationParameters().set_int_param("use_chan_calibration", 0);
    //  calib->GetCalibrationParameters().ReadFromFile("hcalout_hg", "xml", 0, 0,
    //                                                 string(getenv("CALIBRATIONROOT")) + string("/Prototype3/Calibration/"));  // calibration database
    se->registerSubsystem(calib);
  }
  //
  //  // ------------------- Hodoscpes -------------------
  //
  const int first_packet_id = PROTOTYPE4_FEM::PACKET_ID;
  //  const int second_packet_id = 21102;
  //
  GenericUnpackPRDF *gunpack = NULL;

  const int N_hodo = 8;
  // mapping based on SPHENIX-doc-121-v6
  //  1 2 3 4 5 6 7 8
  //  101 100 103 102 97  96  99  98
  //
  //9 109
  //10  108
  //11  111
  //12  110
  //13  105
  //14  104
  //15  107
  //16  106

  gunpack = new GenericUnpackPRDF("HODO_VERTICAL");
  gunpack->add_channel(first_packet_id, 101, 0);
  gunpack->add_channel(first_packet_id, 100, 1);
  gunpack->add_channel(first_packet_id, 103, 2);
  gunpack->add_channel(first_packet_id, 102, 3);
  gunpack->add_channel(first_packet_id, 97, 4);
  gunpack->add_channel(first_packet_id, 96, 5);
  gunpack->add_channel(first_packet_id, 99, 6);
  gunpack->add_channel(first_packet_id, 98, 7);
  se->registerSubsystem(gunpack);
  //
  gunpack = new GenericUnpackPRDF("HODO_HORIZONTAL");
  gunpack->add_channel(first_packet_id, 109, 0);
  gunpack->add_channel(first_packet_id, 108, 1);
  gunpack->add_channel(first_packet_id, 111, 2);
  gunpack->add_channel(first_packet_id, 110, 3);
  gunpack->add_channel(first_packet_id, 105, 4);
  gunpack->add_channel(first_packet_id, 104, 5);
  gunpack->add_channel(first_packet_id, 107, 6);
  gunpack->add_channel(first_packet_id, 106, 7);
  se->registerSubsystem(gunpack);
  //

  //  Nicole Lewis [7:12 PM]
  //  added and commented on this Plain Text snippet: Run 545 MPV and Other Values
  //  Collumns: Height, MPV, Sigma, chi2, NDF
  //  Run 545
  //  HODO_HORIZONTAL[0] 18.0525 319.879 83.4359 139.487 199
  //  HODO_HORIZONTAL[1] 26.6811 262.209 65.1704 159.059 171
  //  HODO_HORIZONTAL[2] 27.5885 296.343 61.0538 171.291 205
  //  HODO_HORIZONTAL[3] 24.4132 299.135 72.4796 205.008 214
  //  HODO_HORIZONTAL[4] 28.6331 290.498 66.9209 177.386 205
  //  HODO_HORIZONTAL[5] 29.3528 263.781 61.5052 202.933 195
  //  HODO_HORIZONTAL[6] 21.5175 336.446 78.8985 170.031 206
  //  HODO_HORIZONTAL[7] 17.7948 336.247 91.8477 146.352 201
  //  HODO_VERTICAL[0] 15.3648 238.473 94.6679 111.272 166
  //  HODO_VERTICAL[1] 23.2368 225.202 54.8611 117.209 145
  //  HODO_VERTICAL[2] 25.3442 209.827 66.168 134.383 171
  //  HODO_VERTICAL[3] 36.8254 217.544 44.1445 122.811 167
  //  HODO_VERTICAL[4] 38.0982 210.6   44.9922 160.269 165
  //  HODO_VERTICAL[5] 38.1045 252.022 49.4073 165.239 200
  //  HODO_VERTICAL[6] 42.1052 223.528 44.5291 113.343 171
  //  HODO_VERTICAL[7] 31.1721 244.299 55.974 160.476 186

  calib = new CaloCalibration("HODO_VERTICAL");
  calib->GetCalibrationParameters().set_int_param("use_chan_calibration", 1);
  calib->GetCalibrationParameters().set_double_param(Form("calib_const_column0_row%d", 0), 1 / 238.473);  // normalize to 1.0
  calib->GetCalibrationParameters().set_double_param(Form("calib_const_column0_row%d", 1), 1 / 225.202);  // normalize to 1.0
  calib->GetCalibrationParameters().set_double_param(Form("calib_const_column0_row%d", 2), 1 / 209.827);  // normalize to 1.0
  calib->GetCalibrationParameters().set_double_param(Form("calib_const_column0_row%d", 3), 1 / 217.544);  // normalize to 1.0
  calib->GetCalibrationParameters().set_double_param(Form("calib_const_column0_row%d", 4), 1 / 210.6);    // normalize to 1.0
  calib->GetCalibrationParameters().set_double_param(Form("calib_const_column0_row%d", 5), 1 / 252.022);  // normalize to 1.0
  calib->GetCalibrationParameters().set_double_param(Form("calib_const_column0_row%d", 6), 1 / 223.528);  // normalize to 1.0
  calib->GetCalibrationParameters().set_double_param(Form("calib_const_column0_row%d", 7), 1 / 244.299);  // normalize to 1.0
  se->registerSubsystem(calib);

  calib = new CaloCalibration("HODO_HORIZONTAL");
  calib->GetCalibrationParameters().set_int_param("use_chan_calibration", 1);
  calib->GetCalibrationParameters().set_double_param(Form("calib_const_column0_row%d", 0), 1 / 319.879);  // normalize to 1.0
  calib->GetCalibrationParameters().set_double_param(Form("calib_const_column0_row%d", 1), 1 / 262.209);  // normalize to 1.0
  calib->GetCalibrationParameters().set_double_param(Form("calib_const_column0_row%d", 2), 1 / 296.343);  // normalize to 1.0
  calib->GetCalibrationParameters().set_double_param(Form("calib_const_column0_row%d", 3), 1 / 299.135);  // normalize to 1.0
  calib->GetCalibrationParameters().set_double_param(Form("calib_const_column0_row%d", 4), 1 / 290.498);  // normalize to 1.0
  calib->GetCalibrationParameters().set_double_param(Form("calib_const_column0_row%d", 5), 1 / 263.781);  // normalize to 1.0
  calib->GetCalibrationParameters().set_double_param(Form("calib_const_column0_row%d", 6), 1 / 336.446);  // normalize to 1.0
  calib->GetCalibrationParameters().set_double_param(Form("calib_const_column0_row%d", 7), 1 / 336.247);  // normalize to 1.0
  se->registerSubsystem(calib);
  //
  //  // ------------------- Other detectors -------------------
  //
  // mapping based on SPHENIX-doc-121-v6
  gunpack = new GenericUnpackPRDF("C1");
  // unpack->Verbosity(1);
  gunpack->add_channel(first_packet_id, 165, 0);  // 24 Cerenkov 1
  se->registerSubsystem(gunpack);
  //
  calib = new CaloCalibration("C1");
  calib->SetFitType(CaloCalibration::kPeakSample);
  se->registerSubsystem(calib);
  //
  // mapping based on SPHENIX-doc-121-v6
  gunpack = new GenericUnpackPRDF("C2");
  // unpack->Verbosity(1);
  gunpack->add_channel(first_packet_id, 166, 0);   // C2 inner fast
  gunpack->add_channel(first_packet_id, 160, 1);   // C2 outer fast
  gunpack->add_channel(first_packet_id, 167, 10);  // C2 inner slow
  se->registerSubsystem(gunpack);
  //
  calib = new CaloCalibration("C2");
  calib->SetFitType(CaloCalibration::kPeakSample);
  calib->GetCalibrationParameters().set_double_param("calib_const_scale", 1);
  calib->GetCalibrationParameters().set_int_param("use_chan_calibration", 1);
  calib->GetCalibrationParameters().set_double_param("calib_const_column0_row0", 1);
  calib->GetCalibrationParameters().set_double_param("calib_const_column0_row1", 1);
  calib->GetCalibrationParameters().set_double_param("calib_const_column0_row10", 1);
  calib->GetCalibrationParameters().set_double_param("calib_const_column0_row11", 1);
  se->registerSubsystem(calib);
  //
  ////  John H. : should be 19, 20, 21 and the other channels are a litle permuted from  what I thought
  //  gunpack = new GenericUnpackPRDF("HCAL_SCINT");
  //// unpack->Verbosity(1);
  //  gunpack->add_channel(second_packet_id, 19, 1);
  //  gunpack->add_channel(second_packet_id, 20, 2);
  //  gunpack->add_channel(second_packet_id, 21, 3);
  //  se->registerSubsystem(gunpack);
  //
  //  calib = new CaloCalibration("HCAL_SCINT");
  //  se->registerSubsystem(calib);
  //
  // mapping based on SPHENIX-doc-121-v6
  gunpack = new GenericUnpackPRDF("PbGL");
  // unpack->Verbosity(1);
  gunpack->add_channel(first_packet_id, 171, 0);  // PbGl (612AM-3)
  se->registerSubsystem(gunpack);

  calib = new CaloCalibration("PbGL");
  calib->GetCalibrationParameters().set_double_param("calib_const_scale", 1);
  se->registerSubsystem(calib);
  //
  // mapping based on SPHENIX-doc-121-v6
  gunpack = new GenericUnpackPRDF("TRIGGER_VETO");
  // unpack->Verbosity(1);
  gunpack->add_channel(first_packet_id, 172, 0);  //  172 Counters  Bottom veto
  gunpack->add_channel(first_packet_id, 173, 1);  //  173 Counters  Top veto
  gunpack->add_channel(first_packet_id, 174, 2);  //  174 Counters  Right veto
  gunpack->add_channel(first_packet_id, 175, 3);  //  175 Counters  Left Veto
  se->registerSubsystem(gunpack);

  // Calibrate the MIP peak to an relative energy of +1.0

  //  Nicole Lewis [7:12 PM]
  //  added and commented on this Plain Text snippet: Run 545 MPV and Other Values
  //  Collumns: Height, MPV, Sigma, chi2, NDF
  //  Run 545
  //  TRIGGER_VETO[0] 501.958 295.811 51.6134 223.972 194
  //  TRIGGER_VETO[1] 262.321 305.247 52.5851 138.403 158
  //  TRIGGER_VETO[2] 238.726 451.48 99.6016 274.339 245
  //  TRIGGER_VETO[3] 135.541 624.076 132.313 284.532 240

  calib = new CaloCalibration("TRIGGER_VETO");
  calib->SetFitType(CaloCalibration::kPeakSample);
  calib->GetCalibrationParameters().set_double_param("calib_const_scale", 1);
  calib->GetCalibrationParameters().set_int_param("use_chan_calibration", 1);
  calib->GetCalibrationParameters().set_double_param("calib_const_column0_row0", 1. / 295.811);
  calib->GetCalibrationParameters().set_double_param("calib_const_column0_row1", 1. / 305.247);
  calib->GetCalibrationParameters().set_double_param("calib_const_column0_row2", 1. / 451.48);
  calib->GetCalibrationParameters().set_double_param("calib_const_column0_row3", 1. / 624.076);
  se->registerSubsystem(calib);
  //
  const int N_TileMapper = 16;
  //
  gunpack = new GenericUnpackPRDF("TILE_MAPPER");
  for (int i = 0; i < N_TileMapper; ++i)
    gunpack->add_channel(first_packet_id, 176 + i, i);  // 24 Cerenkov 1
  se->registerSubsystem(gunpack);
  //
  calib = new CaloCalibration("TILE_MAPPER");
  calib->GetCalibrationParameters().set_double_param("calib_const_scale", 1);
  se->registerSubsystem(calib);
  //
  // mapping based on SPHENIX-doc-121-v6
  gunpack = new GenericUnpackPRDF("SC3");
  // unpack->Verbosity(1);
  gunpack->add_channel(first_packet_id, 169, 0);  // MT6SC3 (612AM-2)
  se->registerSubsystem(gunpack);
  //
  calib = new CaloCalibration("SC3");
  calib->GetCalibrationParameters().set_double_param("calib_const_scale", 1);
  se->registerSubsystem(calib);

  gunpack = new GenericUnpackPRDF("FINGER_COUNTER");
  // unpack->Verbosity(1);
  gunpack->add_channel(first_packet_id, 168, 0);  // 168  Counters  Finger counter
  se->registerSubsystem(gunpack);
  //
  calib = new CaloCalibration("FINGER_COUNTER");
  calib->GetCalibrationParameters().set_double_param("calib_const_scale", 1);
  se->registerSubsystem(calib);
  //
  //  gunpack = new GenericUnpackPRDF("SC_MWPC4");
  //// unpack->Verbosity(1);
  //  gunpack->add_channel(second_packet_id, 18, 0); // 18  SC behind MWPC4 From channel 4 of adjacent 612AM amplifier
  //  se->registerSubsystem(gunpack);
  //
  //  calib = new CaloCalibration("SC_MWPC4");
  //  calib->GetCalibrationParameters().set_double_param("calib_const_scale", -1);
  //  se->registerSubsystem(calib);
  //
  //  gunpack = new GenericUnpackPRDF("SPILL_WARBLER");
  //// unpack->Verbosity(1);
  //  gunpack->add_channel(second_packet_id, 16, 0); // Short Meritec cable 0 16  Spill warbler
  //  se->registerSubsystem(gunpack);
  //
  //  // -------------------  Event summary -------------------
  //
  //  EventInfoSummary * es = new EventInfoSummary();
  //  es->Verbosity(EventInfoSummary::VERBOSITY_MORE);
  //  se->registerSubsystem(es);

  // -------------------  Output -------------------
  //main DST output
  Fun4AllDstOutputManager *out_Manager = new Fun4AllDstOutputManager("DSTOUT",
                                                                     output_file);
  se->registerOutputManager(out_Manager);

  //alternatively, fast check on DST using DST Reader:
  Prototype4DSTReader *reader = new Prototype4DSTReader(
      string(output_file) + string("_DSTReader.root"));

  reader->AddRunInfo("beam_MTNRG_GeV");
  reader->AddRunInfo("beam_2CH_mm");
  reader->AddRunInfo("beam_2CV_mm");
  reader->AddRunInfo("EMCAL_A0_HighGain");
  reader->AddRunInfo("EMCAL_GR0_BiasOffset_Tower21");
  reader->AddRunInfo("EMCAL_T0_Tower21");
  reader->AddRunInfo("EMCAL_Is_HighEta");

  //  reader->AddEventInfo("beam_Is_In_Spill");
  //  reader->AddEventInfo("beam_SPILL_WARBLER_RMS");
  //  reader->AddEventInfo("CALIB_CEMC_Sum");
  //  reader->AddEventInfo("CALIB_LG_HCALIN_Sum");
  //  reader->AddEventInfo("CALIB_LG_HCALOUT_Sum");
  //
  if (do_hcal)
  {
    reader->AddTower("RAW_LG_HCALIN");
    //  reader->AddTower("RAW_HG_HCALIN");
    reader->AddTower("RAW_LG_HCALOUT");
    reader->AddTower("RAW_HG_HCALOUT");

    reader->AddTower("CALIB_LG_HCALIN");
    //  reader->AddTower("CALIB_HG_HCALIN");
    reader->AddTower("CALIB_LG_HCALOUT");
    reader->AddTower("CALIB_HG_HCALOUT");
  }

  if (do_cemc)
  {
    reader->AddTower("RAW_CEMC");
    reader->AddTower("CALIB_CEMC");
  }

  //
  reader->AddTower("RAW_HODO_VERTICAL");
  reader->AddTower("RAW_HODO_HORIZONTAL");
  reader->AddTower("CALIB_HODO_VERTICAL");
  reader->AddTower("CALIB_HODO_HORIZONTAL");
  //
  reader->AddTower("RAW_C1");
  reader->AddTower("CALIB_C1");

  reader->AddTower("RAW_C2");
  reader->AddTower("CALIB_C2");
  //
  //  reader->AddTower("RAW_HCAL_SCINT");
  //  reader->AddTower("CALIB_HCAL_SCINT");
  //
  reader->AddTower("RAW_PbGL");
  reader->AddTower("CALIB_PbGL");
  //
  reader->AddTower("RAW_TRIGGER_VETO");
  reader->AddTower("CALIB_TRIGGER_VETO");
  //
  reader->AddTower("RAW_TILE_MAPPER");
  reader->AddTower("CALIB_TILE_MAPPER");
  //
  reader->AddTower("RAW_SC3");
  reader->AddTower("CALIB_SC3");

  reader->AddTower("RAW_FINGER_COUNTER");
  reader->AddTower("CALIB_FINGER_COUNTER");

  //
  //  reader->AddTower("RAW_SC_MWPC4");
  //  reader->AddTower("CALIB_SC_MWPC4");
  //
  //  reader->AddTower("RAW_SPILL_WARBLER");

  //  reader->AddTowerTemperature("EMCAL");
  //  reader->AddTowerTemperature("HCALIN");
  //  reader->AddTowerTemperature("HCALOUT");

  se->registerSubsystem(reader);

  Fun4AllInputManager *in = new Fun4AllPrdfInputManager("PRDFin");
  in->fileopen(input_file);
  se->registerInputManager(in);

  se->run(nEvents);

  se->End();
}
