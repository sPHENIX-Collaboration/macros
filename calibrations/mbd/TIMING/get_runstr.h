#ifndef MACRO_GET_RUNSTR_H
#define MACRO_GET_RUNSTR_H

#include <TString.h>

#include <iostream>

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

namespace MBDRUNS {
  enum type {
    AUAU200 = 0,
    PP200,
    SIMAUAU200,
    SIMPP200
  };
}

#endif  // __get_runstr__
