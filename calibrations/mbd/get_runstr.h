#ifndef MACRO_GET_RUNSTR_H
#define MACRO_GET_RUNSTR_H

#include <TString.h>

#include <iostream>

namespace MBDRUNS {
  enum type {
    RUN24AUAU = 0,
    RUN24PP,
    RUN25AUAU,
    RUN25PP,
    SIMAUAU200,
    SIMPP200
  };
}

namespace MBDRUNTYPE {
  enum type {
    AUAU200 = 0,
    PP200,
    SIMAUAU200,
    SIMPP200
  };
}

TString get_runstr(const char *fname)
{
  TString name = fname;
  name.ReplaceAll(".prdf","");
  name.ReplaceAll(".root","");
  int index = name.Last('/');
  if ( index > 0 )
  {
    name.Remove(0,index+1);
  }
  index = name.First('-');
  if ( index > 0 )
  {
    name.Remove(0,index+1);
  }
  //std::cout << "aaa " << name << std::endl;

  /*
  index = name.Last('-');
  if ( index > 0 )
  {
    name.Remove(index,name.Length());
  }
  //std::cout << "bbb " << name << std::endl;
  */

  return name;

}

int get_runnumber(const char *fname)
{
  TString str = get_runstr(fname);
  std::cout << str << std::endl;
  int index = str.Last('-');
  if ( index > 0 )
  {
    str.Remove(index,str.Length());
  }
  std::cout << " get_runnumber " << str << "\t" << str.Atoi() << std::endl;
  return str.Atoi();
}

// which run (eg, run24pp, run24auau, etc)
int get_run(const int runno)
{
  if ( runno <= 30000 )
  {
    // may need to update with way to determine if pp or auau
    return MBDRUNS::SIMAUAU200;
  }
  else if ( runno <= 53880 )
  {
    return MBDRUNS::RUN24PP;
  }
  else if ( runno <= 54962 )
  {
    return MBDRUNS::RUN24AUAU;
  }
  else if ( runno <= 78954 )
  {
    return MBDRUNS::RUN25AUAU;
  }
  else if ( runno <= 99999 )
  {
    return MBDRUNS::RUN25PP;
  }

  return -1;
}

int get_runtype(const int runno)
{
  if ( runno <= 30000 )
  {
    // may need to update with way to determine if pp or auau
    return MBDRUNTYPE::SIMAUAU200;
  }
  else if ( runno <= 53880 )
  {
    return MBDRUNTYPE::PP200;
  }
  else if ( runno <= 54962 )
  {
    return MBDRUNTYPE::AUAU200;
  }
  else if ( runno <= 78954 )
  {
    return MBDRUNTYPE::AUAU200;
  }
  else if ( runno <= 99999 )
  {
    return MBDRUNTYPE::PP200;
  }

  return -1;
}

#endif  // __get_runstr__
