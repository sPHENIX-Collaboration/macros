#ifndef MACRO_SAVEGITTAGS_C
#define MACRO_SAVEGITTAGS_C

// This macro parses the rebuild.info file which is written by our build script
// it extracts in a crude way the build and the git tags for the repos which
// went into this build and sets stringflags. This even allows us to
// reproduce new builds

#include <phool/recoConsts.h>

#include <Rtypes.h>  // resolves R__LOAD_LIBRARY for clang-tidy

#include <fstream>
#include <iostream>

R__LOAD_LIBRARY(libphool.so)

void SaveGitTags();
void SetGitTagsFromFile(const std::string &filename);

void SaveGitTags(const std::string &commitid)
{
  recoConsts *rc = recoConsts::instance();
  rc->set_StringFlag("MDC2_GITTAG", commitid);
  SaveGitTags();
}

// build the filename from rebuild.info under $OFFLINE_MAIN
void SaveGitTags()
{
  const char *offline = getenv("OFFLINE_MAIN");
  std::string rebuildinfo = std::string(offline) + std::string("/rebuild.info");
  SetGitTagsFromFile(rebuildinfo);
  return;
}

void SetGitTagsFromFile(const std::string &filename)
{
  recoConsts *rc = recoConsts::instance();
  std::ifstream file(filename);
  std::string line;
  if (file.is_open())
  {
    while (std::getline(file, line))
    {
      if (line.find("install") != std::string::npos)
      {
        int res = 0;
        int foundlast = 0;
        while ((res = line.find('/', res + 1)) !=
               std::string::npos)
        {
          foundlast = res;
        }
        foundlast++;
        std::string build = line.substr(foundlast, line.length() - foundlast);
        rc->set_StringFlag("build", build);
      }
      if (line.find("git repo ") != std::string::npos)
      {
        int start = std::string(" git repo ").length();
        int res = line.find(',');
        std::string reponame = line.substr(start, res - start);
        res = line.find(':');
        std::string repotag = line.substr(res + 2, line.length() - (res + 2));
        rc->set_StringFlag(reponame, repotag);
      }
    }
    file.close();
  }
  else
  {
    std::cout << "Error: Could not open file." << std::endl;
  }
  return;
}
#endif
