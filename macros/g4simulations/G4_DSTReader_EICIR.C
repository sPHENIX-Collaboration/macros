//////////////////////////////////////////////////////////////////
/*!
  \file G4_DSTReader.C
  \brief Convert DST to human command readable TTree for quick poke around the outputs
  \author  Jin Huang
  \version $Revision:  $
  \date    $Date: $
*/
//////////////////////////////////////////////////////////////////

#include <string>

void
G4DSTreader_EICIR( const char * outputFile = "G4sPHENIXCells.root",//
		   int absorberactive = 1 )
{

  //! debug output on screen?
  const bool debug = false;

  //! save raw g4 hits
  const bool save_g4_raw = true;

  // save a comprehensive  evaluation file
  PHG4DSTReader* ana = new PHG4DSTReader(
                                         string(outputFile) + string("_DSTReader.root"));
  ana->set_save_particle(true);
  ana->set_load_all_particle(false);
  ana->set_load_active_particle(true);
  ana->set_save_vertex(true);

  if (debug)
    {
      ana->Verbosity(2);
    }

  if (save_g4_raw)
    {
      ana->AddNode("BH_FORWARD_PLUS");
    }

  Fun4AllServer *se = Fun4AllServer::instance();
  se->registerSubsystem(ana);
}
