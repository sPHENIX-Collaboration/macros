R__LOAD_LIBRARY(libmicromegas.so)

#include <micromegas/MicromegasCalibrationData.h>

#include <micromegas/MicromegasCalibrationData.h>
#include <micromegas/MicromegasMapping.h>

#include <nlohmann/json.hpp>

void ConvertCalibrationToJSon( int runnumber = 74871 )
{

  const auto inputfile = Form("TPOT_Pedestal-%08i-0000.root", runnumber );
  const auto outputfile = Form("TPOT_Pedestal-%08i-0000.json", runnumber );

  std::cout << "EvaluateCalibration - inputfile: " << inputfile << std::endl;
  std::cout << "EvaluateCalibration - outputfile: " << outputfile << std::endl;

  MicromegasCalibrationData m_calibration_data;
  m_calibration_data.read(inputfile);

  const MicromegasMapping m_mapping;

  static const int nsigma = 5;
  static const bool use_maximum = true;

  nlohmann::json calib_list;
  // loop over available fees
  /// get list of fee ids
  for( const auto& fee_id:m_mapping.get_fee_id_list() )
  {
    double mean_pedestal = 0;
    double mean_rms = 0;

    double max_pedestal = 0;
    double max_rms = 0;

    int entries = 0;

    for( int ich = 0; ich < MicromegasDefs::m_nchannels_fee; ++ich )
    {
      const auto pedestal = m_calibration_data.get_pedestal( fee_id, ich );
      const auto rms = m_calibration_data.get_rms( fee_id, ich );

      if(!pedestal || pedestal<20) continue;
      if(!rms) continue;

      // need to comment out SCOZ - FEE = 8
      if( fee_id == 8 && ich > 128 ) continue;

      max_pedestal = std::max<double>(max_pedestal,pedestal);
      max_rms = std::max<double>(max_rms,rms);

      mean_pedestal += pedestal;
      mean_rms += rms;
      ++entries;
    }

    mean_pedestal/=entries;
    mean_rms/=entries;

    const double mean_threshold = mean_pedestal+nsigma*mean_rms;
    std::cout
      << "feid: " << fee_id
      << " pedestal: " << mean_pedestal
      << " rms: " << mean_rms
      << " threshold: " << mean_threshold
      << " value: " << int( 4.*mean_threshold )
      << std::endl;

    const double max_threshold = max_pedestal+nsigma*max_rms;
    std::cout
      << "feid: " << fee_id
      << " pedestal: " << max_pedestal
      << " rms: " << max_rms
      << " threshold: " << max_threshold
      << " value: " << int( 4.*max_threshold )
      << std::endl;

    // cable swap. This is a pain. I do not know how to address this
    const int fee_id_new = (fee_id == 11) ? 21:fee_id;
    nlohmann::json calibration = {
      {"fee_id", fee_id_new },
      {"threshold", int(4*std::round( use_maximum ? max_threshold:mean_threshold ))}
    };

    calib_list.emplace_back(calibration);
  }

  std::ofstream out( outputfile );
  out << std::setw(2) << calib_list << std::endl;
}
