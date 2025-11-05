#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4allraw/Fun4AllStreamingInputManager.h>
#include <fun4allraw/InputManagerType.h>
#include <fun4allraw/SingleMvtxPoolInput.h>

#include <mvtxcalib/MvtxFakeHitRate.h>

#include <mvtx/MvtxPixelDefs.h>

#include <trackbase/MvtxDefs.h>
#include <trackbase/TrkrDefs.h>

#include <TFile.h>
#include <TTree.h>

#include <cdbobjects/CDBTTree.h>

#include <GlobalVariables.C>
#include <phool/recoConsts.h>

#include <string>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libffarawmodules.so)
R__LOAD_LIBRARY(libmvtx.so)
R__LOAD_LIBRARY(libmvtxcalib.so)
R__LOAD_LIBRARY(libcdbobjects.so)

using json = nlohmann::json;

std::map<std::string, int> felix_map{
{"L0_03", 0},
{"L0_04", 0},
{"L2_01", 0},
{"L2_02", 0},
{"L0_05", 0},
{"L2_03", 0},
{"L2_04", 0},
{"L2_15", 0},
{"L1_00", 1},
{"L1_01", 1},
{"L2_06", 1},
{"L2_07", 1},
{"L1_02", 1},
{"L1_03", 1},
{"L2_08", 1},
{"L2_09", 1},
{"L0_00", 2},
{"L0_01", 2},
{"L1_04", 2},
{"L2_10", 2},
{"L0_02", 2},
{"L1_05", 2},
{"L1_06", 2},
{"L1_07", 2},
{"L0_09", 3},
{"L0_10", 3},
{"L2_05", 3},
{"L2_11", 3},
{"L0_11", 3},
{"L2_12", 3},
{"L2_13", 3},
{"L2_14", 3},
{"L0_06", 4},
{"L0_07", 4},
{"L1_12", 4},
{"L2_00", 4},
{"L0_08", 4},
{"L1_13", 4},
{"L1_14", 4},
{"L1_15", 4},
{"L1_08", 5},
{"L1_09", 5},
{"L2_16", 5},
{"L2_17", 5},
{"L1_10", 5},
{"L1_11", 5},
{"L2_18", 5},
{"L2_19", 5}
};

//https://stackoverflow.com/questions/997946/how-to-get-current-time-and-date-in-c
std::string getDate()
{
    std::time_t t = std::time(0);   // get time now
    std::tm* now = std::localtime(&t);

    std::stringstream date;
    date << (now->tm_year + 1900) << (now->tm_mon + 1) << now->tm_mday;
    return date.str();
}

// https://stackoverflow.com/questions/478898/how-do-i-execute-a-command-and-get-the-output-of-the-command-within-c-using-po
string exec(const char *cmd)
{
  std::array<char, 128> buffer;
  std::string result;
  std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
  if (!pipe)
  {
    throw std::runtime_error("popen() failed!");
  }
  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
  {
    result += buffer.data();
  }
  return result;
}

void calculate_FHR(const int run_number = 64639, std::string output_name = "output.root")
{
  std::string calib_files = "/sphenix/lustre01/sphnxpro/physics/MVTX/calib/calib_mvtx";
  std::stringstream nice_run_number;
  nice_run_number << std::setw(8) << std::setfill('0') << std::to_string(run_number);

  //Lets see how many FELIX servers took data for this run
  std::string felix_count_executable_command = "ls -1 ";
  felix_count_executable_command += calib_files;
  felix_count_executable_command += "*";
  felix_count_executable_command += nice_run_number.str();
  felix_count_executable_command += "-0000.evt | wc -l";
  int felix_count = std::stoi(exec(felix_count_executable_command.c_str()));

/*
  std::string strobe_length_executable_command = "psql -h sphnxdaqdbreplica daq --csv -c \"SELECT strobe FROM mvtx_strobe WHERE hostname = 'mvtx0' AND runnumber = ";
  strobe_length_executable_command += std::to_string(run_number);
  strobe_length_executable_command += ";\" | tail -n 1";
  float strobe_length = std::stof(exec(strobe_length_executable_command.c_str()));
*/

  Fun4AllServer *se = Fun4AllServer::instance();

  Fun4AllStreamingInputManager *in = new Fun4AllStreamingInputManager("Comb");
  in->Verbosity(1);

  int counter = 0;
  for(int iflx = 0; iflx < felix_count; iflx++)
  {
    SingleMvtxPoolInput* mvtx_sngl = new SingleMvtxPoolInput("MVTX_FLX" + to_string(iflx));
    mvtx_sngl->Verbosity(1);
    mvtx_sngl->SetBcoRange(10);
    mvtx_sngl->SetNegativeBco(10);
    mvtx_sngl->runMVTXstandalone();

    bool foundGoodFile = false;
    while (!foundGoodFile)
    {
      if (counter == 6) break;

      std::string infile = calib_files + std::to_string(counter) + "-" +  nice_run_number.str() + "-0000.evt";
      ifstream file(infile.c_str());
      if (file.good())
      {
        mvtx_sngl->AddFile(infile);
        foundGoodFile = true;
      }
      else 
      {
        ++counter;
      }
    }

    in->registerStreamingInput(mvtx_sngl, InputManagerType::MVTX);
    ++counter;
  }
  se->registerInputManager(in);

  MvtxFakeHitRate *mfhr = new MvtxFakeHitRate();
  mfhr->SetOutputfile(output_name);
  mfhr->SetMaxMaskedPixels(100000);
  se->registerSubsystem(mfhr);
}

void generate_JSON_mask(const std::string &calibration_file = "output.root",
                        const std::string output_file = "masked_pixels",
                        const double target_threshold = 3.2e-9)
{
  TFile* f = new TFile(calibration_file.c_str(), "READ");
  TTree* t = dynamic_cast<TTree*>(f->Get("masked_pixels"));
  int num_strobes = 0;
  int num_masked_pixels = 0;
  double noise_threshold = 0.0;
  std::vector<uint64_t> * masked_pixels = 0;
  t->SetBranchAddress("num_strobes", &num_strobes);
  t->SetBranchAddress("num_masked_pixels", &num_masked_pixels);
  t->SetBranchAddress("noise_threshold", &noise_threshold);
  t->SetBranchAddress("masked_pixels", &masked_pixels);

  int nentries = t->GetEntries();
  int selected_entry = -1;
  for (int i = 0; i < nentries; i++)
  {
    t->GetEntry(i);
    if(noise_threshold < target_threshold)
    {
      selected_entry = i;
      break;
    }
  }

  t->GetEntry(selected_entry);

  json masked_pixels_file, masked_pixels_file_by_felix[6];

  for (int i = 0; i < num_masked_pixels; i++)
  {
    uint64_t pixel = masked_pixels->at(i);
    uint32_t hitsetkey = MvtxPixelDefs::get_hitsetkey(pixel);
    uint32_t hitkey = MvtxPixelDefs::get_hitkey(pixel);

    uint8_t layer = TrkrDefs::getLayer(hitsetkey);
    uint8_t stave = MvtxDefs::getStaveId(hitsetkey);
    uint8_t chip = MvtxDefs::getChipId(hitsetkey);
    uint16_t column = MvtxDefs::getCol(hitkey);
    uint16_t row = MvtxDefs::getRow(hitkey);

    std::stringstream stave_sw_name;
    stave_sw_name << "L" << std::to_string(layer) << "_" << std::setw(2) << std::setfill('0') << std::to_string(stave);
    json pixel_info = json::array({column, row});
    masked_pixels_file[stave_sw_name.str()][std::to_string(chip)] += pixel_info;
    masked_pixels_file_by_felix[felix_map[stave_sw_name.str()]][stave_sw_name.str()][std::to_string(chip)] += pixel_info;
  }

  f->Close();
  delete f;

  std::cout << "Number of masked pixels: " << num_masked_pixels << std::endl;
  std::cout << "Noise threshold: " << noise_threshold << std::endl;

  std::cout << "Writing masked pixels to json file" << std::endl;

  std::ofstream json_output(output_file);
  json_output << masked_pixels_file.dump(2);
  json_output.close();

  string output_file_no_trailer = output_file;
  string trailer = ".json";
  size_t pos = output_file_no_trailer.find(trailer);
  if (pos != string::npos)
  {
    output_file_no_trailer.erase(pos, trailer.length());
  }

  for (int i = 0; i < 6; ++i)
  {
    std::string individual_mask_name = output_file_no_trailer + "_mvtx" + std::to_string(i) + ".json";
    std::ofstream individual_json_output(individual_mask_name);
    individual_json_output << masked_pixels_file_by_felix[i].dump(2);
    individual_json_output.close();
  }
}

void generate_CDB_mask(const std::string input_file = "masked_pixels.json")
{
  string output_file = input_file;
  string trailer = ".json";
  size_t pos = output_file.find(trailer);
  if (pos != string::npos)
  {
    output_file.erase(pos, trailer.length());
  }

  output_file += ".root";

  CDBTTree *cdbttree = new CDBTTree(output_file);

  unsigned int nStave[3] = {12, 16, 20};

  char *calibrationsroot = getenv("CALIBRATIONROOT");

  std::ifstream f(input_file);
  json data = json::parse(f);
  int PixelIndex = 0;

  for (unsigned int layer = 0; layer < 3; ++layer)
  {
    for (unsigned int stave = 0; stave < nStave[layer]; ++stave)
    {
      std::stringstream staveStream;
      staveStream << "L" << layer << "_" << std::setw(2) << std::setfill('0') << stave;
      std::string staveName = staveStream.str();

      bool doesStaveExist = data.contains(staveName);
      if (!doesStaveExist) continue;

      for (unsigned int chip = 0; chip < 9; ++chip)
      {
        bool doesChipExist = data.at(staveName).contains(std::to_string(chip));
        if (!doesChipExist) continue;

        std::vector<std::pair<int, int>> dead_pixels = data.at(staveName).at(std::to_string(chip));

        for (auto &pixel : dead_pixels)
        {
          cdbttree->SetIntValue(PixelIndex,"layer",layer);
          cdbttree->SetIntValue(PixelIndex,"stave",stave);
          cdbttree->SetIntValue(PixelIndex,"chip",chip);
          cdbttree->SetIntValue(PixelIndex,"col",pixel.first);
          cdbttree->SetIntValue(PixelIndex,"row",pixel.second);
          PixelIndex = PixelIndex + 1;
        }
      }
    }
  }

  cdbttree->Commit();
  cdbttree->SetSingleIntValue("TotalHotPixels",PixelIndex);
  cdbttree->CommitSingle();
  cdbttree->WriteCDBTTree();
  delete cdbttree;
}

void Fun4All_Mvtx_hot_pixel_masking(const int nEvents = 5e5, const int run_number = 64639, const float threshold = 3.2e-9, bool doFHR = true, bool doJSONmask = true, bool doCDBmask = true)
{
    std::string date = getDate();
    std::string run_string = std::to_string(run_number);
    std::string outputDir = "FHR_ana_run_" + run_string + "_" + date;
    std::string makeDirectory = "mkdir -p " + outputDir;
    system(makeDirectory.c_str());

    Fun4AllServer *se = Fun4AllServer::instance();
    se->Verbosity(1);

    recoConsts *rc = recoConsts::instance();
    Enable::CDB = true;
    rc->set_StringFlag("CDB_GLOBALTAG", CDB::global_tag);
    rc->set_uint64Flag("TIMESTAMP", run_number);
    rc->set_IntFlag("RUNNUMBER", run_number);

    std::string run_info = "nEvents_" + std::to_string(nEvents) + "_run_" + run_string + "_date_" + date;
    std::string FHR_output_name = outputDir + "/FHR_output_" + run_info + ".root";
    std::string JSON_output_name = outputDir + "/masked_pixels_" + run_info + ".json";

    if (doFHR)
    {
      calculate_FHR(run_number, FHR_output_name);
      se->run(nEvents);
      se->End();
      delete se;
    } 

    if (doJSONmask) generate_JSON_mask(FHR_output_name, JSON_output_name, threshold);
    if (doCDBmask) generate_CDB_mask(JSON_output_name);
    gSystem->Exit(0);
}
