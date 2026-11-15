#include "HcalCalibDatasetinputs.h"

#include <TSystem.h>

#include <iostream>

// Prints the settings of a dataset in HcalCalibDatasetinputs.h as NAME=value lines, for the shell scripts
// (condor_histmaker/run.sh, genTSCalib/get_tsFiles_flat.sh). Without a dataset: the names of the datasets.
// With a run number instead of a name: the dataset of that run.
//   root -l -b -q 'PrintHcalCalibDatasetinputs.C("run3oo")'
//   root -l -b -q 'PrintHcalCalibDatasetinputs.C("82374")'
void PrintHcalCalibDatasetinputs(const std::string &dataset = "")
{
  if (dataset.empty())
  {
    std::cout << "HCALCALIB_DATASETS=" << hcalCalibDatasetNames() << std::endl;
    return;
  }
  const HCalCalibDataset *ds = findHCalCalibDataset(dataset);
  if (!ds && dataset.find_first_not_of("0123456789") == std::string::npos)
  {
    ds = &getHCalCalibDataset(std::stoi(dataset));
  }
  if (!ds)
  {
    std::cout << "no dataset " << dataset << " in HcalCalibDatasetinputs.h (" << hcalCalibDatasetNames() << ")" << std::endl;
    gSystem->Exit(1);
  }
  std::cout << "HCALCALIB_NAME=" << ds->name << std::endl;
  std::cout << "HCALCALIB_LASTRUN=" << ds->lastRun << std::endl;
  std::cout << "HCALCALIB_RUNLIST=" << ds->runList << std::endl;
  std::cout << "HCALCALIB_DSTTAG=" << ds->dstTag << std::endl;
  std::cout << "HCALCALIB_OHCOSMIC=" << ds->oh_cosmicFile << std::endl;
  std::cout << "HCALCALIB_IHCOSMIC=" << ds->ih_cosmicFile << std::endl;
  std::cout << "HCALCALIB_TSCDIR=" << ds->tscDir << std::endl;
  std::cout << "HCALCALIB_OHFINALDIR=" << ds->oh_finalDir << std::endl;
  std::cout << "HCALCALIB_IHFINALDIR=" << ds->ih_finalDir << std::endl;
}
