#include <CDBUtils.C>

std::map<std::string, std::string> md5map;

void loadmd5map(const std::string &fname);
bool md5check(const std::string &cdbfname, const std::string &fname);
int insertpl(const std::string &fname, const std::string &ptype, int runnumber);
int checkpl(const std::string &fname, const std::string &ptype, int runnumber);
int insertplrunrange(const std::string &fname, const std::string &ptype, int runnumber, int endrun);
int checkplrunrange(const std::string &fname, const std::string &ptype, int runnumber, int endrun);
int insertplinfinity(const std::string &fname, const std::string &ptype, int runnumber);
int checkplinfinity(const std::string &fname, const std::string &ptype, int runnumber);

int checkpl(const std::string &fname, const std::string &ptype, int runnumber)
{
  std::string cdbres = getCalibration(ptype,runnumber);
  if (cdbres.empty() || cdbres.find(fname) == std::string::npos)
  {
    cout << "could not find " << fname << " inserting it" << std::endl;
    insertpl(fname,ptype,runnumber);
  }
  else
  {
    if (md5check(cdbres,fname))
    {
//      cout << fname << " checks out" << std::endl;
    }
    else
    {
      cout << fname << " will be overwritten with new md5sum" << std::endl;
      insertpl(fname,ptype,runnumber);
    }
      
    }
  return 0;
}

int checkplrunrange(const std::string &fname, const std::string &ptype, int runnumber, int endrun)
{
  std::string cdbres = getCalibration(ptype,runnumber);
  if (cdbres.empty() || cdbres.find(fname) == std::string::npos)
  {
    cout << "could not find " << fname << " inserting it" << std::endl;
    insertplrunrange(fname,ptype,runnumber,endrun);
  }
  else
  {
    if (md5check(cdbres,fname))
    {
//      cout << fname << " checks out" << std::endl;
    }
    else
    {
      cout << fname << " will be overwritten with new md5sum" << std::endl;
      insertplrunrange(fname,ptype,runnumber,endrun);
    }
      
    }
  return 0;
}

int checkplinfinity(const std::string &fname, const std::string &ptype, int runnumber)
{
  std::string cdbres = getCalibration(ptype,runnumber);
  if (cdbres.empty() || cdbres.find(fname) == std::string::npos)
  {
    cout << "could not find " << fname << " inserting it" << std::endl;
    insertplinfinity(fname,ptype,runnumber);
  }
  else
  {
    if (md5check(cdbres,fname))
    {
//      cout << fname << " checks out" << std::endl;
    }
    else
    {
      cout << fname << " will be overwritten with new md5sum" << std::endl;
      insertplinfinity(fname,ptype,runnumber);
    }
      
    }
  return 0;
}

int insertpl(const std::string &fname, const std::string &ptype, int runnumber)
{
  insertPayload(ptype,fname,runnumber,runnumber+1);
//  gSystem->Exit(0);
  return 0;
}

int insertplrunrange(const std::string &fname, const std::string &ptype, int runnumber, int endrun)
{
  cout << "insert " << fname << ", from " << runnumber << " to "
       << endrun+1 << endl;
  insertPayload(ptype,fname,runnumber,endrun+1);
//  gSystem->Exit(0);
  return 0;
}

int insertplinfinity(const std::string &fname, const std::string &ptype, int runnumber)
{
//  std::cout << "inserting payload " << fname << " start " << runnumber << std::endl;
  insertPayload(ptype,fname,runnumber);
//  gSystem->Exit(0);
  return 0;
}

void loadmd5map(const std::string &fname)
{
  ifstream infile(fname);
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

bool md5check(const std::string &cdbfname, const std::string &fname)
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
      return true;
    }
  }
  return false;
}
