#include"getCaloTemp.C"

void loopCaloTemp(const char* runListFile = "runList.txt", const char* detector = "HCALIN")
{
  std::ifstream fin(runListFile);
  if (!fin.is_open()) {
    std::cerr << "Error: Could not open " << runListFile << std::endl;
    return;
  }

  std::vector<int> runs;
  int run;
  while (fin >> run) {
    runs.push_back(run);
  }
  fin.close();

  std::cout << "Found " << runs.size() << " runs in " << runListFile << std::endl;

  // Loop over runs and call your macro
  for (size_t i = 0; i < runs.size(); ++i) {
    std::cout << "Processing run " << runs[i] << " ..." << std::endl;
    getCaloTemp(runs[i], detector);
  }

  std::cout << "All runs processed!" << std::endl;
}

