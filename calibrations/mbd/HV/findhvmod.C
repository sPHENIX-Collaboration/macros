//
// Find the HV mod for a channel
//

#include <mbd/MbdGeomV2.h>

R__LOAD_LIBRARY(libmbd.so)
R__LOAD_LIBRARY(libmbd_io.so)

MbdGeomV2 *mbdgeom{nullptr};


void findhvmod()
{
  mbdgeom = new MbdGeomV2();
  std::multimap hvmap = mbdgeom->get_hvmap();

  int pmts[] = { 51, 84, 85, 26, 75, 89, 121 };
  for (int pmt : pmts)
  {
    for ( auto hv : hvmap )
    {
      if ( hv.second == pmt )
      {
        std::cout << hv.second << "\t" << hv.first << std::endl;
        break;
      }
    }

  }
}
