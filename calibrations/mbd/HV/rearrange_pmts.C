// Make plots of the values of the gains
// Use this to figure out which PMTs to swap around to normalize gains
#include <mbd/MbdCalib.h>
#include <mbd/MbdGeomV2.h>


#include <TCanvas.h>
#include <TFile.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TH1.h>
#include <TPad.h>
#include <TString.h>

R__LOAD_LIBRARY(libmbd.so)
R__LOAD_LIBRARY(libmbd_io.so)

int nruns = 0;
MbdCalib *mcal{nullptr};
MbdGeomV2 *mbdgeom{nullptr};

void read_calibgains(const char *fname)
{
  mcal = new MbdCalib();
  mcal->Download_Gains( fname );
}


TGraphErrors *g_gainsbyhv[16]{nullptr};

double gmean(TGraph *g)
{
  int n = g->GetN();
  Double_t *y = g->GetY();
  double sum = 0.;
  for (int i=0; i<n; i++)
  {
    sum += y[i];
  }
  double avg = sum / n;

  return avg;
}


void rearrange_pmts(const char *fname = "results/54935/mbd_qfit.calib")
{
  mbdgeom = new MbdGeomV2();

  read_calibgains(fname);

  TString name;
  for (int ihv=0; ihv<16; ihv++)
  {
    name = "g_gainsbyhv"; name += ihv;
    g_gainsbyhv[ihv] = new TGraphErrors();
    g_gainsbyhv[ihv]->SetName( name );
    g_gainsbyhv[ihv]->SetLineColor( ihv%8 + 1 );
    g_gainsbyhv[ihv]->SetMarkerColor( ihv%8 + 1 );
    /*
    if ( ihv+1 == 10 )
    {
      g_gainsbyhv[ihv]->SetLineColor( 50 );
      g_gainsbyhv[ihv]->SetMarkerColor( 50 );
    }
    */
    g_gainsbyhv[ihv]->SetMarkerStyle( 20 );
    g_gainsbyhv[ihv]->GetHistogram()->SetXTitle( "hv groups" );
    g_gainsbyhv[ihv]->GetHistogram()->SetYTitle( "gain [ADC/mip]" );
  }


  int npmts = 0;
  std::array<double,16> gmin{};
  std::array<double,16> gmax{};
  std::array<int,16> minpmt{};
  std::array<int,16> maxpmt{};
  gmin.fill(1e9);      // the gain settings that were used for run analyzed
  gmax.fill(0);
  minpmt.fill(-1);
  maxpmt.fill(-1);
  std::multimap hvmap = mbdgeom->get_hvmap();
  std::cout << "\thvmod\tpmt\tgain\tr" << std::endl;
  for ( auto hv : hvmap )
  {
    int hvmod = hv.first;
    int pmt = hv.second;
    double gain = mcal->get_qgain( pmt );

    // print out the outliers
    if ( gain>250 || gain<100 )
    {
      float r = mbdgeom->get_r( pmt );
      std::cout << "outlier\t"<< hvmod << "\t" << pmt << "\t" << gain << "\t" << r << std::endl;
    }

    int n = g_gainsbyhv[hvmod]->GetN();
    g_gainsbyhv[hvmod]->SetPoint(n,(double)npmts,gain);

    if ( gain>gmax[hvmod] )
    {
      gmax[hvmod] = gain;
      maxpmt[hvmod] = pmt;
    }
    if ( gain<gmin[hvmod] )
    {
      gmin[hvmod] = gain;
      minpmt[hvmod] = pmt;
    }

    npmts++;
  }

  double& overall_gmin = *std::min_element(gmin.begin(), gmin.end());
  double& overall_gmax = *std::max_element(gmax.begin(), gmax.end());

  // Draw the gains by HVMOD
//  TCanvas *ac = new TCanvas("c_gainsbyhvmod","Gains by HV Mod",1250,600);
  g_gainsbyhv[0]->Draw("ap");
  g_gainsbyhv[0]->GetXaxis()->SetLimits(0,128);
  g_gainsbyhv[0]->SetMaximum( overall_gmax*1.1 );
  g_gainsbyhv[0]->SetMinimum( overall_gmin*0.9 );
  gPad->Modified();
  gPad->Update();
  for (int ihv=1; ihv<16; ihv++)
  {
    g_gainsbyhv[ihv]->Draw("p");
  }

  //return;

  /*
  // calculate means, and the scaling to normalize all of them to the same
  std::array<double,16> means;
  means.fill(0.);
  
  std::cout << "means in each hv group" << std::endl;
  std::array<double,16> new_gmin;
  std::array<double,16> new_gmax;
  std::array<double,16> newscale;
  double overall_newgmin{1e9};
  double overall_newgmax{0};
  int   minhvmod = -1;  // hv mod with min gain
  int   maxhvmod = -1;  // hv mod with max gain
  for (int ihv=0; ihv<16; ihv++)
  {
    means[ihv] = gmean( g_gainsbyhv[ihv] );
    newscale[ihv] = means[0]/means[ihv];
    new_gmin[ihv] = gmin[ihv]*newscale[ihv];
    new_gmax[ihv] = gmax[ihv]*newscale[ihv];
    std::cout << ihv << "\t" << means[ihv] << "\t" << newscale[ihv] << "\t" << new_gmin[ihv] << std::endl;

    if ( new_gmax[ihv]>overall_newgmax )
    {
      overall_newgmax = new_gmax[ihv];
      maxhvmod = ihv;
    }
    if ( new_gmin[ihv]<overall_newgmin )
    {
      overall_newgmin = new_gmin[ihv];
      minhvmod = ihv;
    }
  }
  */

  // Starting voltages (from last known values for PHENIX BBC)
  std::array<double,16> orig_hv { 
    -1950.0, -2150.0, -2000.0, -2308.6, -2300.0, -1668.8, -2000.0, -2200.0,      // south
    -1850.0, -2400.0, -1950.0, -2170.0, -1612.0, -2350.0, -2050.0, -1811.5       // north
  };

  // Settings (relative) for HV in Run2 Au+Au
  std::array<double,16> ref_hvsetting {
      0.695889, 0.69948, 0.745683, 0.716363, 0.705564, 0.762345, 0.757273, 0.712111,   // south
      0.738386, 0.726954, 0.779238, 0.725638, 0.797808, 0.70735, 0.786134, 0.763983    // north
  };

  std::array<double,16> hv_setting{};

  std::cout << "HV SETTING" << std::endl;
  for (int imod=0; imod<orig_hv.size(); imod++)
  {
    hv_setting[imod] = orig_hv[imod]*ref_hvsetting[imod];
    std::cout << hv_setting[imod] << "\t";
    if ( imod%8==7 ) { std::cout << std::endl;
}
  }
  
  // Plot the gain curves by hvmod
  //TCanvas *bc = new TCanvas("c_gaincurvesbyhvmod","Gain curves by hvmod",1200,1200,.0001,.0001);
//  TCanvas *bc = new TCanvas("c_gaincurvesbyhvmod","Gain curves by hvmod",1200,1200);
  //bc->Divide(4,4);
  TFile *gcurvefile = new TFile("SCAN/mbdlaser_202405151654.root");
  TGraphErrors *g_norm_laserscan[128]{nullptr};
  TGraph *ginv_norm_laserscan[128]{nullptr};  // inverse curves
  for (int ipmt=0; ipmt<128; ipmt++)
  {
    name = "g_norm_laserscan"; name += ipmt;
    g_norm_laserscan[ipmt] = (TGraphErrors*)gcurvefile->Get( name );
  }

  std::array<double,16> sumsetting{};
  sumsetting.fill(0.);
  std::array<int,16> npmts_in_hvmod{};
  npmts_in_hvmod.fill(0.);
  std::array<double,16> newsetting{};
  newsetting.fill(0.);

  int flag = 0;
  for ( auto hv : hvmap )
  {
    int hvmod = hv.first;
    int pmt = hv.second;

    // convert x axis to HV and then create inverted graph
    int n = g_norm_laserscan[pmt]->GetN();
    double *x = g_norm_laserscan[pmt]->GetX();
    double *y = g_norm_laserscan[pmt]->GetY();
    for (int ip=0; ip<n; ip++)
    {
      x[ip] *= -orig_hv[hvmod];
    }

    ginv_norm_laserscan[pmt] = new TGraph(n,y,x);
    name = "ginv_norm_laserscan"; name += pmt;
    ginv_norm_laserscan[pmt]->SetName( name );

    //bc->cd( hvmod+1 );
    if ( flag == 0 )
    {
      g_norm_laserscan[pmt]->Draw("acp");
      g_norm_laserscan[pmt]->GetXaxis()->SetLimits(800.,2450.);
      gPad->Modified();
      gPad->Update();
      flag = 1;
    }
    else
    {
      g_norm_laserscan[pmt]->Draw("cp");
    }

    // See what happens when we change HV
    double gain = mcal->get_qgain( pmt );
    if ( pmt==17 )
    {
      float orig_hv_loc = g_norm_laserscan[pmt]->Eval( 1503. );
      float new_hv = g_norm_laserscan[pmt]->Eval( 1356. );
      float new_gain = (new_hv/orig_hv_loc)*gain;
      std::cout << "PMT " << pmt << "\t" << orig_hv_loc << "\t" << new_hv << "\t" << new_hv/orig_hv_loc << "\t" << new_gain << std::endl;
    }
    if ( pmt==26 )
    {
      float orig_hv_loc = g_norm_laserscan[pmt]->Eval( 1491. );
      float new_hv = g_norm_laserscan[pmt]->Eval( 1356. );
      float new_gain = (new_hv/orig_hv_loc)*gain;
      std::cout << "PMT " << pmt << "\t" << orig_hv_loc << "\t" << new_hv << "\t" << new_hv/orig_hv_loc << "\t" << new_gain << std::endl;
    }
    if ( pmt==36 )
    {
      float orig_hv_loc = g_norm_laserscan[pmt]->Eval( 1622. );
      float new_hv = g_norm_laserscan[pmt]->Eval( 1653. );
      float new_gain = (new_hv/orig_hv_loc)*gain;
      std::cout << "PMT " << pmt << "\t" << orig_hv_loc << "\t" << new_hv << "\t" << new_hv/orig_hv_loc << "\t" << new_gain << std::endl;
    }
    if ( pmt==51 )
    {
      float orig_hv_loc = g_norm_laserscan[pmt]->Eval( 1514. );
      //float new_hv = g_norm_laserscan[pmt]->Eval( 1566. );
      float new_hv = g_norm_laserscan[pmt]->Eval( 1622. );
      float new_gain = (new_hv/orig_hv_loc)*gain;
      std::cout << "PMT " << pmt << "\t" << orig_hv_loc << "\t" << new_hv << "\t" << new_hv/orig_hv_loc << "\t" << new_gain << std::endl;
    }
    if ( pmt==70 )
    {
      float orig_hv_loc = g_norm_laserscan[pmt]->Eval( 1366. );
      float new_hv = g_norm_laserscan[pmt]->Eval( 1519. );
      float new_gain = (new_hv/orig_hv_loc)*gain;
      std::cout << "PMT " << pmt << "\t" << orig_hv_loc << "\t" << new_hv << "\t" << new_hv/orig_hv_loc << "\t" << new_gain << std::endl;
    }
    if ( pmt==79 )
    {
      float orig_hv_loc = g_norm_laserscan[pmt]->Eval( 1574. );
      float new_hv = g_norm_laserscan[pmt]->Eval( 1611. );
      float new_gain = (new_hv/orig_hv_loc)*gain;
      std::cout << "PMT " << pmt << "\t" << orig_hv_loc << "\t" << new_hv << "\t" << new_hv/orig_hv_loc << "\t" << new_gain << std::endl;
    }
    if ( pmt==85 )
    {
      float orig_hv_loc = g_norm_laserscan[pmt]->Eval( 1574. );
      float new_hv = g_norm_laserscan[pmt]->Eval( 1662. );
      float new_gain = (new_hv/orig_hv_loc)*gain;
      std::cout << "PMT " << pmt << "\t" << orig_hv_loc << "\t" << new_hv << "\t" << new_hv/orig_hv_loc << "\t" << new_gain << std::endl;
    }
    if ( pmt==87 )
    {
      float orig_hv_loc = g_norm_laserscan[pmt]->Eval( 1774. );
      float new_hv = g_norm_laserscan[pmt]->Eval( 1662. );
      float new_gain = (new_hv/orig_hv_loc)*gain;
      std::cout << "PMT " << pmt << "\t" << orig_hv_loc << "\t" << new_hv << "\t" << new_hv/orig_hv_loc << "\t" << new_gain << std::endl;
    }
    if ( pmt==89 )
    {
      float orig_hv_loc = g_norm_laserscan[pmt]->Eval( 1519. );
      float new_hv = g_norm_laserscan[pmt]->Eval( 1366. );
      float new_gain = (new_hv/orig_hv_loc)*gain;
      std::cout << "PMT " << pmt << "\t" << orig_hv_loc << "\t" << new_hv << "\t" << new_hv/orig_hv_loc << "\t" << new_gain << std::endl;
    }
    if ( pmt==91 )
    {
      float orig_hv_loc = g_norm_laserscan[pmt]->Eval( 1774. );
      float new_hv = g_norm_laserscan[pmt]->Eval( 1662. );
      float new_gain = (new_hv/orig_hv_loc)*gain;
      std::cout << "PMT " << pmt << "\t" << orig_hv_loc << "\t" << new_hv << "\t" << new_hv/orig_hv_loc << "\t" << new_gain << std::endl;
    }
    if ( pmt==107 )
    {
      float orig_hv_loc = g_norm_laserscan[pmt]->Eval( 1611. );
      float new_hv = g_norm_laserscan[pmt]->Eval( 1662. );
      float new_gain = (new_hv/orig_hv_loc)*gain;
      std::cout << "PMT " << pmt << "\t" << orig_hv_loc << "\t" << new_hv << "\t" << new_hv/orig_hv_loc << "\t" << new_gain << std::endl;
    }
    if ( pmt==113 )
    {
      float orig_hv_loc = g_norm_laserscan[pmt]->Eval( 1383. );
      float new_hv = g_norm_laserscan[pmt]->Eval( 1286. );
      float new_gain = (new_hv/orig_hv_loc)*gain;
      std::cout << "PMT " << pmt << "\t" << orig_hv_loc << "\t" << new_hv << "\t" << new_hv/orig_hv_loc << "\t" << new_gain << std::endl;
    }
    if ( pmt==116 )
    {
      float orig_hv_loc = g_norm_laserscan[pmt]->Eval( 1611. );
      float new_hv = g_norm_laserscan[pmt]->Eval( 1574. );
      float new_gain = (new_hv/orig_hv_loc)*gain;
      std::cout << "PMT " << pmt << "\t" << orig_hv_loc << "\t" << new_hv << "\t" << new_hv/orig_hv_loc << "\t" << new_gain << std::endl;
    }
    if ( pmt==121 )
    {
      float orig_hv_loc = g_norm_laserscan[pmt]->Eval( 1611. );
      //float new_hv = g_norm_laserscan[pmt]->Eval( 1519. );
      float new_hv = g_norm_laserscan[pmt]->Eval( 1574. );
      float new_gain = (new_hv/orig_hv_loc)*gain;
      std::cout << "PMT " << pmt << "\t" << orig_hv_loc << "\t" << new_hv << "\t" << new_hv/orig_hv_loc << "\t" << new_gain << std::endl;
    }

  }

}
