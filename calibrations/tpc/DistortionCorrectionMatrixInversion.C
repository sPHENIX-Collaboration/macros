#include <tpccalib/TpcSpaceChargeMatrixInversion.h>

#include <cstdio>
#include <sstream>

R__LOAD_LIBRARY(libtpccalib.so)

//_______________________________________________
// get list of files matching selection
std::vector<std::string> list_files( const std::string& selection )
{
  std::vector<std::string> out;

  std::cout << "list_files - selection: " << selection << std::endl;
  if( selection.empty() ) return out;

  const std::string command = std::string("ls -1 ") + selection;
  auto tmp = popen( command.c_str(), "r" );
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
  const TString tag = "_flat_genfit_truth_notpc_distorted-new";
  const TString inputFile = Form( "DST/CONDOR%s/TpcSpaceChargeMatrices%s_*.root", tag.Data(), tag.Data() );

  // Central membrane distortion corrections
  /*
   * this is the 2D distortion corrections measured at the central membrane using diffuse lasers
   * it is used to extrapolate the distortions measured in the TPOT acceptance to the rest of the TPC acceptance
   * see: https://indico.bnl.gov/event/22887/contributions/90413/attachments/54020/92443/distortion_extrapolation_hp.pdf
   */
  const std::string inputfile_cm = "distortion_maps/average_minus_static_distortion_cm.root";

  // output file
  const TString outputFile = Form( "Rootfiles/Distortions_full%s_mm.root", tag.Data() );

  std::cout << "DistortionCorrectionMatrixInversion - inputFile: " << inputFile << std::endl;
  std::cout << "DistortionCorrectionMatrixInversion - inputfile_cm: " << inputfile_cm << std::endl;
  std::cout << "DistortionCorrectionMatrixInversion - outputFile: " << outputFile << std::endl;

  auto filenames = list_files( inputFile.Data() );
  std::cout << "SpaceChargeMatrixInversion - loaded " << filenames.size() << " files" << std::endl;

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
  spaceChargeMatrixInversion.save_distortion_corrections( outputFile.Data() );

  std::cout << "DistortionCorrectionMatrixInversion - all done." << std::endl;
}
