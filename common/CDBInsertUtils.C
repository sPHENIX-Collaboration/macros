#include <CDBUtils.C>

#include <TSystem.h>

std::map<std::string, std::string> md5map;
std::string md5sumfile;

void loadmd5map(const std::string &fname);
int md5check(const std::string &cdbfname, const std::string &fname);
int insertpl(const std::string &fname, const std::string &ptype, int runnumber);
int checkpl(const std::string &fname, const std::string &ptype, int runnumber);
int insertplrunrange(const std::string &fname, const std::string &ptype, int runnumber, int endrun);
int checkplrunrange(const std::string &fname, const std::string &ptype, int runnumber, int endrun);
int insertplinfinity(const std::string &fname, const std::string &ptype, int runnumber);
int checkplinfinity(const std::string &fname, const std::string &ptype, int runnumber);

int checkpl(const std::string &fname, const std::string &ptype, int runnumber)
{
  std::string cdbres = getCalibration(ptype, runnumber);
  if (cdbres.empty() || cdbres.find(fname) == std::string::npos)
  {
    std::cout << fname << " not in cdb, inserting it" << std::endl;
    insertpl(fname, ptype, runnumber);
  }
  else
  {
    int iret = md5check(cdbres, fname);
    switch (iret)
    {
    case 0:
      //      std::cout << fname << " checks out" << std::endl;
      break;
    case 1:
      std::cout << fname << " will be overwritten with new md5sum" << std::endl;
      insertpl(fname, ptype, runnumber);
      break;
    case -1:
      std::cout << "could not find " << fname << " in " << md5sumfile << " doing nothing" << std::endl;
      break;
    default:
      std::cout << "strange return from md5check(" << cdbres << ", "
                << fname << ")" << " doing nothing" << std::endl;
      break;
    }
  }
  return 0;
}

int checkplrunrange(const std::string &fname, const std::string &ptype, int runnumber, int endrun)
{
  std::string cdbres = getCalibration(ptype, runnumber);
  if (cdbres.empty() || cdbres.find(fname) == std::string::npos)
  {
    std::cout << fname << " not in cdb, inserting it" << std::endl;
    insertplrunrange(fname, ptype, runnumber, endrun);
  }
  else
  {
    int iret = md5check(cdbres, fname);
    switch (iret)
    {
    case 0:
      //      std::cout << fname << " checks out" << std::endl;
      break;
    case 1:
      std::cout << fname << " will be overwritten with new md5sum" << std::endl;
      insertplrunrange(fname, ptype, runnumber, endrun);
      break;
    case -1:
      std::cout << "could not find " << fname << " in " << md5sumfile << " doing nothing" << std::endl;
      break;
    default:
      std::cout << "strange return from md5check(" << cdbres << ", "
                << fname << ")" << " doing nothing" << std::endl;
      break;
    }
  }
  return 0;
}

int checkplinfinity(const std::string &fname, const std::string &ptype, int runnumber)
{
  std::string cdbres = getCalibration(ptype, runnumber);
  if (cdbres.empty() || cdbres.find(fname) == std::string::npos)
  {
    std::cout << fname << " not in cdb, inserting it" << std::endl;
    insertplinfinity(fname, ptype, runnumber);
  }
  else
  {
    int iret = md5check(cdbres, fname);
    switch (iret)
    {
    case 0:
      //      std::cout << fname << " checks out" << std::endl;
      break;
    case 1:
      std::cout << fname << " will be overwritten with new md5sum" << std::endl;
      insertplinfinity(fname, ptype, runnumber);
      break;
    case -1:
      std::cout << "could not find " << fname << " in " << md5sumfile << " doing nothing" << std::endl;
      break;
    default:
      std::cout << "strange return from md5check(" << cdbres << ", "
                << fname << ")" << " doing nothing" << std::endl;
      break;
    }
  }
  return 0;
}

int insertpl(const std::string &fname, const std::string &ptype, int runnumber)
{
  insertPayload(ptype, fname, runnumber, runnumber + 1);
  //  gSystem->Exit(0);
  return 0;
}

int insertplrunrange(const std::string &fname, const std::string &ptype, int runnumber, int endrun)
{
  std::cout << "insert " << fname << ", from " << runnumber << " to "
            << endrun << std::endl;
  insertPayload(ptype, fname, runnumber, endrun + 1);
  //  gSystem->Exit(0);
  return 0;
}

int insertplinfinity(const std::string &fname, const std::string &ptype, int runnumber)
{
  //  std::cout << "inserting payload " << fname << " start " << runnumber << std::endl;
  std::cout << "insert " << fname << ", from " << runnumber << " to end of time" << std::endl;
  insertPayload(ptype, fname, runnumber);
  //  gSystem->Exit(0);
  return 0;
}

void loadmd5map(const std::string &fname)
{
  md5sumfile = fname;
  std::ifstream infile(fname);
  if (infile.is_open())
  {
    std::string line;
    while (std::getline(infile, line))
    {
      std::istringstream istr(line);
      std::string md5;
      std::string cdbfile;
      istr >> md5;
      istr >> cdbfile;
      md5map[cdbfile] = md5;
    }
    infile.close();
    // for (auto iter : md5map)
    // {
    //        std::cout << "file: " << iter.first << ", md5: " << iter.second << std::endl;
    // }
  }
  else
  {
    std::cout << "could not open " << fname << std::endl;
    gSystem->Exit(1);
  }
}

int md5check(const std::string &cdbfname, const std::string &fname)
{
  std::filesystem::path fullfile(cdbfname);
  std::string lfn = fullfile.stem();
  std::filesystem::path fullfname(fname);
  int pos = lfn.find(fullfname.stem());
  pos--;
  //  std::cout << "lfn: " << fullfile.stem() << ", fulln: "<< fullfname.stem() << ", pos: " << pos << std::endl;
  lfn.erase(pos);
  auto iter = md5map.find(fname);
  if (iter != md5map.end())
  {
    //    std::cout << "file " << iter->first << " md5: " << iter->second << " cdbmd5: " << lfn << std::endl;
    if (iter->second == lfn)
    {
      //      std::cout << "all good" << std::endl;
      return 0;
    }
    return 1;
  }
  return -1;
}

int checkrunrange(const std::string &fname, const std::string &ptype, int beginrun, int endrun)
{
  int iret = 0;
  for (int irun = beginrun; irun <= endrun; irun++)
  {
    if (irun % 1000 == 0)
    {
      std::cout << "checking " << irun << std::endl;
    }
    std::string cdbres = getCalibration(ptype,irun);
    if (cdbres.empty())
    {
      std::cout << "no " << ptype << " calibration for run " << irun << std::endl;
      iret = -1;
      continue;
    }
    if (cdbres.find(fname) == std::string::npos)
    {
      std::cout << "calibration mismatch for " << ptype << " calibration, expected " << fname << ", read back "
	   << cdbres << std::endl;
      iret = -2;
      continue;
    }
    int md5ret = md5check(cdbres,fname);
    if (md5ret != 0)
    {
      std::cout << "md5sum check failed" << std::endl;
      iret = -3;
    }
  }
  return iret;
}
