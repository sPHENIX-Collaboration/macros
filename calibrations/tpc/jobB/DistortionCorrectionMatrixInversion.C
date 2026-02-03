#include <g4detectors/PHG4CylinderGeomContainer.h>
#include <g4detectors/PHG4CylinderGeom.h>

#include <tpccalib/TpcSpaceChargeMatrixInversion.h>
#include <tpccalib/TpcSpaceChargeReconstructionHelper.h>

#include <micromegas/CylinderGeomMicromegas.h>
#include <micromegas/MicromegasDefs.h>

#include <trackreco/MakeActsGeometry.h>

#include <ffamodules/CDBInterface.h>

#include <fun4all/Fun4AllRunNodeInputManager.h>
#include <fun4all/Fun4AllServer.h>

#include <phool/getClass.h>
#include <phool/recoConsts.h>

#include <TString.h>

#include <cstdio>
#include <sstream>
#include <string>

R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libtpccalib.so)
R__LOAD_LIBRARY(libtrack_reco.so)

namespace
{

  // range adaptor to be able to use range-based for loop
  template<class T> class range_adaptor
  {
    public:
    explicit range_adaptor( const T& range ):m_range(range){}
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

//_______________________________________________
// load tpot geometry ranges from current calibrations
void load_tpot_geometry( int runnumber )
{

  // initialization
  auto *rc = recoConsts::instance();
  auto *se = Fun4AllServer::instance();
  auto *topNode = se->topNode();

  rc->set_IntFlag("RUNNUMBER", runnumber);
  rc->set_IntFlag("RUNSEGMENT", 0);
  rc->set_StringFlag("CDB_GLOBALTAG", "newcdbtag");
  rc->set_uint64Flag("TIMESTAMP", runnumber);

  // load geometry file
  std::string geofile = CDBInterface::instance()->getUrl("Tracking_Geometry");
  std::cout << "Geometry - geofile: " <<  geofile.c_str() << std::endl;
  auto *ingeo = new Fun4AllRunNodeInputManager("GeoIn");
  ingeo->AddFile(geofile);
  ingeo->run(0);

  // acts geometry initialization
  auto *geom = new MakeActsGeometry;
  geom->set_mvtx_applymisalign(true);
  geom->InitRun( topNode );

  // get relevant nodes
  // micromegas geometry
  auto *m_micromegas_geomcontainer = findNode::getClass<PHG4CylinderGeomContainer>(topNode, "CYLINDERGEOM_MICROMEGAS_FULL");

  // ACTS geometry
  auto *m_tGeometry = findNode::getClass<ActsGeometry>(topNode, "ActsGeometry");

  using range_list_t = std::vector<CylinderGeomMicromegas::range_t>;
  range_list_t theta_range_list;
  range_list_t phi_range_list;

  // loop over layers
  for( const auto& [layer, layergeom] : range_adaptor( m_micromegas_geomcontainer->get_begin_end() ) )
  {
    // sanity
    assert(layer == layergeom->get_layer());

    auto *micromegas_layergeom = static_cast<CylinderGeomMicromegas*>(layergeom);

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
  auto get_phi_range = [phi_range_list]( const std::vector<int>& indexes )
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

  // calculate phi ranges
  // central (= bottommost) sector corresponds to tiles 0 to 3
  const auto phi_range_central = get_phi_range( {0,1,2,3} );

  // east sector corresponds to tiles 4 and 5
  const auto phi_range_east = get_phi_range( {4,5} );

  // west sector corresponds to tiles 6 and 7
  const auto phi_range_west = get_phi_range( {6,7} );

  // print
  std::cout << "phi_range_central=" << phi_range_central << std::endl;
  std::cout << "phi_range_east=" << phi_range_east << std::endl;
  std::cout << "phi_range_west=" << phi_range_west << std::endl;
  std::cout << std::endl;

  // assign
  TpcSpaceChargeReconstructionHelper::set_phi_range_central(phi_range_central);
  TpcSpaceChargeReconstructionHelper::set_phi_range_east(phi_range_east);
  TpcSpaceChargeReconstructionHelper::set_phi_range_west(phi_range_west);

  // generate theta ranges
  const range_list_t theta_range_central{theta_range_list[0], theta_range_list[1], theta_range_list[2], theta_range_list[3]};
  const range_list_t theta_range_east{theta_range_list[4], theta_range_list[5]};
  const range_list_t theta_range_west{theta_range_list[6], theta_range_list[7]};

  // print
  std::cout << "theta_range_central=" << theta_range_central << std::endl;
  std::cout << "theta_range_east=" << theta_range_east << std::endl;
  std::cout << "theta_range_west=" << theta_range_west << std::endl;

  // assign
  TpcSpaceChargeReconstructionHelper::set_theta_range_central(theta_range_central);
  TpcSpaceChargeReconstructionHelper::set_theta_range_east(theta_range_east);
  TpcSpaceChargeReconstructionHelper::set_theta_range_west(theta_range_west);
}

//_______________________________________________
// get list of files matching selection
std::vector<std::string> list_files( const std::string& selection )
{
  std::vector<std::string> out;

  std::cout << "list_files - selection: " << selection.c_str() << std::endl;
  if( selection.empty() ) return out;

  const std::string command = std::string("ls -1 ") + selection;
  auto *tmp = popen( command.c_str(), "r" );
  char line[512];
  while( fgets( line, 512, tmp ) )
  {

    std::istringstream istr( line );
    std::string filename;
    istr >> filename;

    if( filename.empty() ) continue;
    if( access( filename.c_str(), R_OK ) ) continue;

    out.push_back( filename );
  }
  pclose( tmp );
  return out;
}

//_______________________________________________
void DistortionCorrectionMatrixInversion()
{

  // input files
  /*
   * this is the list of distortion correction matrices files coming out from Job A
   * that needs to be inverted, to get track-based, beam-induced distortions inside
   * TPOT acceptance
   */
  std::string tag = "_flat_genfit_truth_notpc_distorted-new";
  std::string inputFile = "DST/CONDOR" + tag + "/TpcSpaceChargeMatrices" + tag + std::string("_*.root");

  // Central membrane distortion corrections
  /*
   * this is the 2D distortion corrections measured at the central membrane using diffuse lasers
   * it is used to extrapolate the distortions measured in the TPOT acceptance to the rest of the TPC acceptance
   * see: https://indico.bnl.gov/event/22887/contributions/90413/attachments/54020/92443/distortion_extrapolation_hp.pdf
   */
  const std::string inputfile_cm = "distortion_maps/average_minus_static_distortion_cm.root";

  // output file
  std::string outputFile = "Rootfiles/Distortions_full" + tag + "_mm.root";

  std::cout << "DistortionCorrectionMatrixInversion - inputFile: " << inputFile.c_str() << std::endl;
  std::cout << "DistortionCorrectionMatrixInversion - inputfile_cm: " << inputfile_cm.c_str() << std::endl;
  std::cout << "DistortionCorrectionMatrixInversion - outputFile: " << outputFile .c_str()<< std::endl;

  auto filenames = list_files( inputFile );
  std::cout << "SpaceChargeMatrixInversion - loaded " << filenames.size() << " files" << std::endl;

  // update TPOT phi range, needed for the interpolation
  load_tpot_geometry(52077);
  std::cout << "SpaceChargeMatrixInversion - done adjusting TPOT phi and theta ranges" << std::endl;

  // perform matrix inversion
  TpcSpaceChargeMatrixInversion spaceChargeMatrixInversion;

  // load input files
  for( const auto& file:filenames )
  { spaceChargeMatrixInversion.add_from_file( file ); }

  // calculate the distortions in TPOT acceptance
  spaceChargeMatrixInversion.calculate_distortion_corrections();

  // load central membrane corrections
  spaceChargeMatrixInversion.load_cm_distortion_corrections( inputfile_cm );
  spaceChargeMatrixInversion.extrapolate_distortion_corrections();

  // write to output
  spaceChargeMatrixInversion.save_distortion_corrections( outputFile );

  std::cout << "DistortionCorrectionMatrixInversion - all done." << std::endl;
}
