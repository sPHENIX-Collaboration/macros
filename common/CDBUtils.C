#ifndef CDBUTILS_C
#define CDBUTILS_C

#include <sphenixnpc/CDBUtils.h>
#include <sphenixnpc/SphenixClient.h>

#include <phool/recoConsts.h>

R__LOAD_LIBRARY(libsphenixnpc.so)
R__LOAD_LIBRARY(libphool.so)

CDBUtils *uti = nullptr;

CDBUtils *getCDBUtils()
{
  return uti;
}

void listGlobalTags()
{
  if (!uti) uti = new CDBUtils();
  uti->listGlobalTags();
  return;
}

void listPayloadTypes()
{
  if (!uti) uti = new CDBUtils();
  uti->listPayloadTypes();
  return;
}

void listPayloadIOVs(uint64_t iov)
{
  if (!uti) uti = new CDBUtils();
  uti->listPayloadIOVs(iov);
  return;
}

void createGlobalTag(const std::string &tagname)
{
  if (!uti) uti = new CDBUtils();
  uti->createGlobalTag(tagname);
  return;
}

void deleteGlobalTag(const std::string &tagname)
{
  if (!uti) uti = new CDBUtils();
  uti->deleteGlobalTag(tagname);
  return;
}

void lockGlobalTag(const std::string &tagname)
{
  if (!uti) uti = new CDBUtils();
  uti->lockGlobalTag(tagname);
  return;
}

void unlockGlobalTag(const std::string &tagname)
{
  if (!uti) uti = new CDBUtils();
  uti->unlockGlobalTag(tagname);
  return;
}

int setGlobalTag(const std::string &tagname)
{
  if (!uti) uti = new CDBUtils();
  int iret = uti->setGlobalTag(tagname);
  return iret;
}

int cloneGlobalTag(const std::string &source, const std::string &target)
{
  if (!uti) uti = new CDBUtils();
  int iret = uti->cloneGlobalTag(source, target);
  return iret;
}


int createPayloadType(const std::string &pt)
{
  if (!uti) uti = new CDBUtils();
  int iret = uti->createPayloadType(pt);
  return iret;
}

int insertPayload(const std::string &pl_type, const std::string &file_url, uint64_t iov_start)
{
  if (!uti) uti = new CDBUtils();
  if (uti->isGlobalTagSet())
  {
    uti->insertPayload(pl_type,file_url,iov_start);
  }
  return 0;
}

int insertPayload(const std::string &pl_type, const std::string &file_url, uint64_t iov_start, uint64_t iov_end)
{
  if (!uti) uti = new CDBUtils();
  if (uti->isGlobalTagSet())
  {
    uti->insertPayload(pl_type,file_url,iov_start,iov_end);
  }
  return 0;
}

std::string getCalibration(const std::string &pl_type, uint64_t iov)
{
  if (!uti) uti = new CDBUtils();
  return uti->getUrl(pl_type,iov);
}

void clearCache()
{
  if (!uti) uti = new CDBUtils();
  return uti->clearCache();
}

void Verbosity(int verb)
{
  if (!uti) uti = new CDBUtils();
  return uti->Verbosity(verb);
}

int deletePayloadIOV(const std::string& pl_type, uint64_t iov_start)
{
  if (!uti) uti = new CDBUtils();
  return uti->deletePayloadIOV(pl_type,iov_start);
}

int deletePayloadIOV(const std::string& pl_type, uint64_t iov_start, uint64_t iov_end)
{
  if (!uti) uti = new CDBUtils();
  return uti->deletePayloadIOV(pl_type,iov_start, iov_end);
}

#endif
