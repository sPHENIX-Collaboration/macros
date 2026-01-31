/*
 * Convert TPOT alignment translation offset, given in centimeter unit,
 * and mesured in local coordinates into global coordinates, and millimeter
 * units, suitable for modifying the alignment parameters
 * text file passed to ACTS in fun4all
 * local coordinates translation offsets are hard-coded
 */

#include <g4detectors/PHG4CylinderGeomContainer.h>

#include <micromegas/CylinderGeomMicromegas.h>

#include <trackreco/MakeActsGeometry.h>

#include <ffamodules/CDBInterface.h>

#include <fun4all/Fun4AllRunNodeInputManager.h>
#include <fun4all/Fun4AllServer.h>

#include <micromegas/MicromegasDefs.h>

#include <phool/getClass.h>
#include <phool/recoConsts.h>

#include <Rtypes.h> // for R__LOAD_LIBRARY macro
#include <TVector3.h>

R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libtrack_reco.so)

namespace
{
  //! small class to restore std::cout precision at end-of-scope
  class std_precision_restore_t // NOLINT(hicpp-special-member-functions)
  {
    public:

    //! constructor
    explicit std_precision_restore_t( std::ostream& out = std::cout ):
      m_out( out ),
      m_precision( out.precision() )
    {}

    //! destructor
    ~std_precision_restore_t()
    { m_out << std::setprecision( m_precision ); }

    private:
    std::ostream& m_out;
    int m_precision = 0;
  };

  //! handles TPOT local and global alignment parameters
  class translation_parameters_t
  {
    public:

    translation_parameters_t( TrkrDefs::hitsetkey key, double local_x, double local_y ):
      m_hitsetkey( key ),
      m_local_x( local_x ),
      m_local_y( local_y )
    {}

//NOLINTBEGIN(misc-non-private-member-variables-in-classes)
    TrkrDefs::hitsetkey m_hitsetkey = 0;
    double m_local_x = 0;
    double m_local_y = 0;
    double m_global_x = 0;
    double m_global_y = 0;
    double m_global_z = 0;
//NOLINTEND(misc-non-private-member-variables-in-classes)

    friend std::ostream& operator << (std::ostream& out, const translation_parameters_t& p )
    {
      std_precision_restore_t precision_restore(out);
      out << p.m_hitsetkey << " 0 0 0 "
        << std::setprecision(3)
        << p.m_global_x*Acts::UnitConstants::cm << " "
        << p.m_global_y*Acts::UnitConstants::cm << " "
        << p.m_global_z*Acts::UnitConstants::cm;
      return out;
    }

    using list = std::vector<translation_parameters_t>;

  };

  /*
   * TPOT local alignment parameters
   * local translation parameters along local x (approximatle rphi) and local y (approximately z)
   * first parameter is hitsetkey, used to identify individual Micromegas detectors
   */
  translation_parameters_t::list translation_parameters =
  {
    { 53936384, 1.86, -0.56 },
    { 53936385, 1.74, -0.61 },
    { 53936386, 1.49, -1.23 },
    { 53936387, 1.58, -1.02 },
    { 53936388, 0.34, -0.96 },
    { 53936389, 0.45, 0.66 },
    { 53936390, 2.02, -0.52 },
    { 53936391, 2.14, -1.13 },
    { 54001664, 1.86, -0.56 },
    { 54001665, 1.74, -0.61 },
    { 54001666, 1.49, -1.23 },
    { 54001667, 1.58, -1.02 },
    { 54001668, 0.34, -0.96 },
    { 54001669, 0.45, 0.66 },
    { 54001670, 2.02, -0.52 },
    { 54001671, 2.14, -1.13 }
  };

}

//_________________________________________________________________
void ConvertTpotAlignment()
{

  // initialization
  auto *rc = recoConsts::instance();
  auto *se = Fun4AllServer::instance();
  auto *topNode = se->topNode();

  // set run number to get proper CDB entries
  const int runnumber = 52077;

  rc->set_IntFlag("RUNNUMBER", runnumber);
  rc->set_IntFlag("RUNSEGMENT", 0);
  rc->set_StringFlag("CDB_GLOBALTAG", "newcdbtag");
  rc->set_uint64Flag("TIMESTAMP", runnumber);

  // load geometry file
  std::string geofile = CDBInterface::instance()->getUrl("Tracking_Geometry");
  std::cout << "Geometry - geofile: " << geofile << std::endl;
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

  // write down the center of all tiles
  const auto [begin,end] = m_micromegas_geomcontainer->get_begin_end();
  for( auto iter = begin; iter != end; ++iter )
  {

    auto *layergeom = dynamic_cast<CylinderGeomMicromegas*>(iter->second);

    // get layer
    const unsigned int layer = layergeom->get_layer();

    // get tiles
    const unsigned int tile_count = layergeom->get_tiles_count();

    // loop over tiles
    for( unsigned int tileid = 0; tileid < tile_count; ++tileid )
    {
      // get tile center and print
      const auto center = layergeom->get_world_from_local_coords( tileid, m_tGeometry, {0,0} );
      std::cout << "ConvertTpotAlignment - layer: " << layer << " tile: " << tileid << " center: {" << center.x() << "," << center.y() << "," << center.z() << "}" << std::endl;
    }
  }


  // loop over TPOT alignment parameters
  for( auto&& p:translation_parameters )
  {
    // get layer and tile from hitsetkey
    const auto layer = TrkrDefs::getLayer(p.m_hitsetkey);
    const auto tileid = MicromegasDefs::getTileId(p.m_hitsetkey);

    // get relevant geometry
    const auto *const layergeom =  static_cast<const CylinderGeomMicromegas*>(m_micromegas_geomcontainer->GetLayerGeom(layer));

    // get global translation
    const auto translation_global = layergeom->get_world_from_local_vect(tileid, m_tGeometry, {p.m_local_x, p.m_local_y, 0} );
    p.m_global_x = translation_global.x();
    p.m_global_y = translation_global.y();
    p.m_global_z = translation_global.z();

  }

  // printout
  for( const auto& p:translation_parameters )
  { std::cout << p << std::endl; }

  std::cout << "All done" << std::endl;

}
