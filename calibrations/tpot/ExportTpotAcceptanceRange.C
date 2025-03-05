/*
 * Export phi and theta range, in absolute coordinates, for each TPOT Tile.
 * this can be used to hard-code the TPOT acceptance, for instance when extrapolating
 * to the full TPC acceptance
 */


#include <ffamodules/CDBInterface.h>

#include <fun4all/Fun4AllRunNodeInputManager.h>
#include <fun4all/Fun4AllServer.h>

#include <micromegas/MicromegasDefs.h>

#include <phool/getClass.h>
#include <phool/recoConsts.h>

#include <trackreco/MakeActsGeometry.h>

R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libtrack_reco.so)

namespace
{

  // range adaptor to be able to use range-based for loop
  template<class T> class range_adaptor
  {
    public:
    range_adaptor( const T& range ):m_range(range){}
    const typename T::first_type& begin() {return m_range.first;}
    const typename T::second_type& end() {return m_range.second;}
    private:
    T m_range;
  };

  // generic pair printout
  template<class T, class U>
  std::ostream& operator << (std::ostream& out, const std::pair<T, U> pair)
  {
    out << "{" << pair.first << "," << pair.second << "}";
    return out;
  }

  // generic container printout
  template<template<typename,typename> class Container, class T, class A>
    std::ostream& operator << (std::ostream& out, const Container<T,A>& container)
  {
    out << "{";
    bool first = true;
    for( const auto& value:container )
    {
      if( !first ) { out << ", "; };
      first = false;
      out << value;
    }
    out << "}";
    return out;
  }

}

void ExportTpotAcceptanceRange()
{

  // initialization
  auto rc = recoConsts::instance();
  auto se = Fun4AllServer::instance();
  auto topNode = se->topNode();

  // set run number to get proper CDB entries
  const int runnumber = 52077;

  rc->set_IntFlag("RUNNUMBER", runnumber);
  rc->set_IntFlag("RUNSEGMENT", 0);
  rc->set_StringFlag("CDB_GLOBALTAG", "ProdA_2024");
  rc->set_uint64Flag("TIMESTAMP", runnumber);

  // load geometry file
  std::string geofile = CDBInterface::instance()->getUrl("Tracking_Geometry");
  std::cout << "Geometry - geofile: " << geofile << std::endl;
  auto ingeo = new Fun4AllRunNodeInputManager("GeoIn");
  ingeo->AddFile(geofile);
  ingeo->run(0);

  // acts geometry initialization
  auto geom = new MakeActsGeometry;
  geom->InitRun( topNode );

  // get relevant nodes
  // micromegas geometry
  auto m_micromegas_geomcontainer = findNode::getClass<PHG4CylinderGeomContainer>(topNode, "CYLINDERGEOM_MICROMEGAS_FULL");

  // ACTS geometry
  auto m_tGeometry = findNode::getClass<ActsGeometry>(topNode, "ActsGeometry");

  using range_list_t = std::vector<CylinderGeomMicromegas::range_t>;
  range_list_t theta_range_list;
  range_list_t phi_range_list;

  // loop over layers
  for( const auto& [layer, layergeom] : range_adaptor( m_micromegas_geomcontainer->get_begin_end() ) )
  {
    // sanity
    assert(layer == layergeom->get_layer());

    auto micromegas_layergeom = static_cast<CylinderGeomMicromegas*>(layergeom);

    // tiles
    const unsigned int tile_count = micromegas_layergeom->get_tiles_count();

    // segmentation
    const auto segmentation = micromegas_layergeom->get_segmentation_type();

    for( unsigned int itile = 0; itile < tile_count; ++itile )
    {
      switch(segmentation)
      {
        case MicromegasDefs::SegmentationType::SEGMENTATION_PHI:
        phi_range_list.emplace_back(micromegas_layergeom->get_phi_range(itile, m_tGeometry));
        break;

        case MicromegasDefs::SegmentationType::SEGMENTATION_Z:
        theta_range_list.emplace_back(micromegas_layergeom->get_theta_range(itile, m_tGeometry));
        break;
      }
    }
  }

  // calculate inner phi range for each sector
  auto get_phi_range = [phi_range_list]( std::vector<int> indexes )
  {
    CylinderGeomMicromegas::range_t out{0,0};
    for(const auto& i:indexes)
    {
      const auto& window = phi_range_list[i];
      if( out.first == 0 || window.first > out.first ) out.first = window.first;
      if( out.second == 0 || window.second < out.second ) out.second = window.second;
    }
    return out;
  };

  const auto phi_range_central = get_phi_range( {0,1,2,3} );
  const auto phi_range_east = get_phi_range( {4,5} );
  const auto phi_range_west = get_phi_range( {6,7} );

  std::cout << "phi_range_central=" << phi_range_central << std::endl;
  std::cout << "phi_range_east=" << phi_range_east << std::endl;
  std::cout << "phi_range_west=" << phi_range_west << std::endl;
  std::cout << std::endl;

  std::cout << "theta_range_central=" << range_list_t{theta_range_list[0], theta_range_list[1], theta_range_list[2], theta_range_list[3]} << std::endl;
  std::cout << "theta_range_east=" << range_list_t{theta_range_list[4], theta_range_list[5]} << std::endl;
  std::cout << "theta_range_west=" << range_list_t{theta_range_list[6], theta_range_list[7]} << std::endl;

  return;
}
