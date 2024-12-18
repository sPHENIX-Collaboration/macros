//
// Make the MBD LUT's
//
#include <cmath>
#include <TMath.h>
#include <mbd/MbdCalib.h>
#include <mbd/MbdGeomV1.h>
#include <mbd/MbdDefs.h>
#include <cstdint>


#if defined(__CLING__)
R__LOAD_LIBRARY(libmbd.so)
#endif

MbdGeom *mbdgeom{nullptr};
MbdCalib *mcal{nullptr};


float tdclut[128][1024] = {};
uint16_t adclut[128][1024] = {};
uint16_t slewlut[128][4096] = {};
double step = 0;                   // ns/LL1 tdc count
double slewcorr_step[128] = {};

void make_generic_adclut();
void make_generic_slewlut();
void make_adclut(const char *scorr_fname = "mbd_slewcorr.calib");
void make_slewlut(const char *scorr_fname = "mbd_slewcorr.calib");

void make_generic_adclut()
{
  // write adc lut
  ofstream adclutfile("mbdadc.lut");
  for (int ipmt=0; ipmt<MbdDefs::MBD_N_PMT; ipmt++)
  {
    adclutfile << "adc " << ipmt << endl;
    for (int iaddr=0; iaddr<1024; iaddr++)
    {
      uint16_t qsum7 = iaddr>>3;
      uint16_t slew3 = iaddr>>7;
      adclut[ipmt][iaddr] = (slew3<<7) + qsum7;   // assuming slew3 are high order bits

      adclutfile << slew3*1000 + qsum7 << "\t";

      if ( iaddr%8 == 7 )
      {
        adclutfile << endl;
      }
    }
  }
  adclutfile.close();
}

void make_generic_slewlut()
{
  // write slew lut
  ofstream slewlutfile("mbdslew.lut");
  for (int ipmt=0; ipmt<MbdDefs::MBD_N_PMT; ipmt++)
  {
    slewlutfile << "slew " << ipmt << endl;
    for (int islew=0; islew<8; islew++)   // 3-bit
    {
      uint16_t saddr = islew;
      for (int itdc=0; itdc<512; itdc++)  // 9-bit
      {
        uint16_t taddr = itdc;
        if ( taddr!=0 )
        {
          slewlut[ipmt][itdc] = 1000 + taddr;
          // 1000 represents the active bit in the lut file
          // when loading into the actual MBD ADC LUT, we should
          // we should set bit 10 = 1.
        }
        else
        {
          slewlut[ipmt][itdc] = taddr;
        }

        slewlutfile << slewlut[ipmt][itdc] << "\t";

        if ( itdc%8 == 7 )
        {
          slewlutfile << endl;
        }
      }
    }
  }
  slewlutfile.close();
}

void make_adclut(const char *scorr_fname = "mbd_slewcorr.calib")
{


  // write adc lut
  ofstream adclutfile("mbdadc.lut");
  for (int ipmt=0; ipmt<MbdDefs::MBD_N_PMT; ipmt++)
  {
    int feech = mbdgeom->get_feech(ipmt,0);

    // get the max slew correction, which we set at ADC=40
    double max_slew = mcal->get_scorr(feech,40);
    slewcorr_step[ipmt] = max_slew/8;
    int nsteps = 0;
 
    adclutfile << "adc " << ipmt << endl;
    for (int iaddr=0; iaddr<1024; iaddr++)
    {
      uint16_t qsum7 = iaddr>>3;

      uint16_t actual_adc = (iaddr<<4) + 8; // 8 = 2^4/2, so actual_adc is at midpoint
      double scorr = mcal->get_scorr(feech,actual_adc);
      int scorr_step = static_cast<int>( scorr/slewcorr_step[ipmt] );
      if ( scorr_step < 0 )
      {
        scorr_step = 0;
      }
      else if ( scorr_step > 7 )
      {
        scorr_step = 7;
      }

      uint16_t slew3 = 7 - scorr_step;

      if ( ipmt==0 )
      {
        cout << "adcslewbin " << iaddr << "\t" << scorr << "\t" << scorr_step << "\t" << slew3 << endl;
      }

      adclut[ipmt][iaddr] = (slew3<<7) + qsum7;   // assuming slew3 are high order bits

      adclutfile << slew3*1000 + qsum7 << "\t";

      if ( iaddr%8 == 7 )
      {
        adclutfile << endl;
      }
    }
  }
  adclutfile.close();
}

void make_slewlut(const char *scorr_fname = "mbd_slewcorr.calib")
{

  // write slew lut
  ofstream slewlutfile("mbdslew.lut");
  for (int ipmt=0; ipmt<MbdDefs::MBD_N_PMT; ipmt++)
  {
    slewlutfile << "slew " << ipmt << endl;
    for (int islew=0; islew<8; islew++)   // 3-bit
    {
      uint16_t saddr = islew;
      for (int itdc=0; itdc<512; itdc++)  // 9-bit
      {
        int taddr = itdc;
        if ( taddr!=0 )
        {
          // apply slew correction
          taddr -= static_cast<int>( (7-islew)*(slewcorr_step[ipmt]/step) );
          if ( ipmt==0 )
          {
            cout << "slewxxx " << islew << "\t" << itdc << "\t" << (7-islew) << "\t" << slewcorr_step[ipmt] << "\t" << step << "\t" << (slewcorr_step[ipmt]/step) << "\t" << taddr << endl;
          }

          if ( taddr<0 )
          {
            taddr = 1;
          }
          slewlut[ipmt][itdc] = 1000 + taddr;
          // 1000 represents the active bit in the lut file
          // when loading into the actual MBD ADC LUT, we should
          // we should set bit 10 = 1.
        }
        else  // 0 is no-hit for TDC
        {
          slewlut[ipmt][itdc] = taddr;
        }

        slewlutfile << slewlut[ipmt][itdc] << "\t";

        if ( itdc%8 == 7 )
        {
          slewlutfile << endl;
        }
      }
    }
  }
  slewlutfile.close();
}

void make_mbd_l1lut(const char *tcorr_fname = "mbd_timecorr.calib",
    const char *t0_fname = "mbd_tt_t0.calib",
    const char *scorr_fname = "mbd_slewcorr.calib",
    const char *tpranges_fname = "mbd_tpscan.txt")
{
  mbdgeom = new MbdGeomV1();

  mcal = new MbdCalib();
  mcal->Download_TimeCorr( tcorr_fname );
  mcal->Download_TTT0( t0_fname );
  mcal->Download_SlewCorr( scorr_fname );

  ifstream tprangesfile(tpranges_fname);
  int temp_feech;
  double mintdc[128] = {0};
  double maxtdc[128] = {0};
  double mintime[128] = {0};
  double maxtime[128] = {0};
  double dtdc[128] = {0};
  double dtime[128] = {0};

  // Find the overlapping good time ranges for all the channels
  TGraph *g_mintdc = new TGraph();
  TGraph *g_maxtdc = new TGraph();
  g_mintdc->SetName("g_mintdc");
  g_maxtdc->SetName("g_maxtdc");
  TGraph *g_mintimes = new TGraph();
  TGraph *g_maxtimes = new TGraph();
  g_mintimes->SetName("g_mintdc");
  g_maxtimes->SetName("g_maxtdc");

  while ( tprangesfile >> temp_feech )
  {
    int pmtch = (double)mbdgeom->get_pmt(temp_feech);

    tprangesfile >> mintdc[pmtch] >> maxtdc[pmtch] >> dtdc[pmtch] >> mintime[pmtch] >> maxtime[pmtch] >> dtime[pmtch];

    //cout << "maxtime " << pmtch << "\t" << maxtime[pmtch] << "\t" << mcal->get_tcorr( temp_feech, 16 ) << endl;;
    maxtime[pmtch] = mcal->get_tcorr( temp_feech, 16 ) - mcal->get_tt0( pmtch );

    cout << "mintime " << pmtch << "\t" << mintime[pmtch] << "\t" << mcal->get_tcorr( temp_feech, static_cast<int>(std::round(maxtdc[pmtch])) ) << endl;;
    cout << mcal->get_tt0(pmtch) << endl;
    mintime[pmtch] = mcal->get_tcorr( temp_feech, static_cast<int>(round(maxtdc[pmtch])) ) - mcal->get_tt0( pmtch );

    // correct for bad channels
    if ( fabs(mcal->get_tt0( pmtch ))>100. )
    {
      maxtime[pmtch] = mcal->get_tcorr( temp_feech, 16 ) - 3.0;
      mintime[pmtch] = mcal->get_tcorr( temp_feech, static_cast<int>(round(maxtdc[pmtch])) ) - 3.0;
    }

    int n = g_mintdc->GetN();
    g_mintdc->SetPoint( n, pmtch, mintdc[pmtch] );

    n = g_maxtdc->GetN();
    g_maxtdc->SetPoint( n, pmtch, maxtdc[pmtch] );

    n = g_mintimes->GetN();
    g_mintimes->SetPoint( n, pmtch, mintime[pmtch] );

    n = g_maxtimes->GetN();
    g_maxtimes->SetPoint( n, pmtch, maxtime[pmtch] );
  }

  int n = g_mintimes->GetN();
  double lut_mintime = TMath::MaxElement(n,g_mintimes->GetY());

  n = g_maxtimes->GetN();
  double lut_maxtime = TMath::MinElement(n,g_maxtimes->GetY());

  cout << "max-min time: " << lut_mintime << "\t" << lut_maxtime << "\t" << lut_maxtime - lut_mintime << endl;

  g_mintimes->SetMarkerStyle(20);
  g_mintimes->SetMarkerColor(3);
  g_maxtimes->SetMarkerStyle(20);
  g_maxtimes->SetMarkerColor(4);
  g_mintimes->SetMinimum(lut_mintime*1.5);
  g_mintimes->SetMaximum(lut_maxtime*1.2);
  g_mintimes->Draw("ap");
  g_maxtimes->Draw("p");

  double max_range = lut_maxtime - lut_mintime;
  step = max_range/511.;           // ns/LL1 tdc count
  double zstep = (2.0/29.9792458)*(1.0/step);    
  cout << "L1 trig time unit is " << step << " ns/count" << endl;
  cout << "L1 trig time unit is " << zstep << " count/cm" << endl;

  for (int ifeech=0; ifeech<MbdDefs::MBD_N_FEECH; ifeech++)
  {
    if ( mbdgeom->get_type(ifeech) == 1 ) continue; // skip q-ch's
    int pmtch = mbdgeom->get_pmt(ifeech);

    for (int iaddr=0; iaddr<1024; iaddr++ )
    {
      // could average over the 16 values
      int tdc = (iaddr<<4) + 8;
      float true_time = mcal->get_tcorr(ifeech,tdc) - mcal->get_tt0(pmtch) - lut_mintime;

      if ( mcal->get_tt0(pmtch)<-100. )
      {
        // set bad channels to 0;
        //tdclut[pmtch][iaddr] = 0;
        //continue;

        true_time = mcal->get_tcorr(ifeech,tdc) - 3.0 - lut_mintime;
      }

      if ( true_time<0. || true_time>max_range || isnan(true_time) || true_time>22.5 )
      {
        tdclut[pmtch][iaddr] = 0;
      }
      else
      {
        if ( isnan(true_time) )
        {
          cout << "isnan " << ifeech << " " << mcal->get_tcorr(ifeech,tdc) 
            << "\t" << mcal->get_tt0(pmtch) << endl;;
        }

        //tdclut[pmtch][iaddr] = static_cast<int>( round(true_time/step) );
        tdclut[pmtch][iaddr] = round(true_time/step);
      }

      if ( ifeech==0 && (iaddr<24 || iaddr>(1023-96) ) )
      {
        if (iaddr%8==0) cout << iaddr << "\t";
        cout << tdclut[pmtch][iaddr] << "\t";   // be sure to set upper bit to 1 for non-zero values when writing to actual LUT in ADC
        if (iaddr%8==7) cout << endl;
      }
    }
  }

  // write tdc lut
  ofstream tdclutfile("mbdtdc.lut");
  for (int ifeech=0; ifeech<MbdDefs::MBD_N_FEECH; ifeech++)
  {
    if ( mbdgeom->get_type(ifeech) == 1 ) continue; // skip q-ch's
    int pmtch = mbdgeom->get_pmt(ifeech);

    tdclutfile << "tdc " << pmtch << endl;
    for (int iaddr=0; iaddr<1024; iaddr++)
    {

      tdclutfile << tdclut[pmtch][iaddr] << "\t";

      if ( iaddr%8 == 7 )
      {
        tdclutfile << endl;
      }
    }
  }
  tdclutfile.close();


  make_adclut();
  make_slewlut();
}

