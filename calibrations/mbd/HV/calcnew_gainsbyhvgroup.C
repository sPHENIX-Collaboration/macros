// Make plots of the values of the gains
#include "mbd/MbdCalib.h"
#include "mbd/MbdGeomV2.h"
//#include "get_runstr.h"

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


TGraphErrors *g_gains{nullptr};
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

//void calcnew_gainsbyhvgroup(const char *fname = "results/42045/mbd_qfit.calib")
//void calcnew_gainsbyhvgroup(const char *fname = "results/53635/mbd_qfit.calib")
void calcnew_gainsbyhvgroup(const char *fname = "results/54935/mbd_qfit.calib")
{
  mbdgeom = new MbdGeomV2();

  read_calibgains(fname);

  TString name;
  //g_gains = new TGraph();
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
  std::array<double,16> gmin;
  std::array<double,16> gmax;
  std::array<int,16> minpmt;
  std::array<int,16> maxpmt;
  gmin.fill(1e9);      // the gain settings that were used for run analyzed
  gmax.fill(0);
  minpmt.fill(-1);
  maxpmt.fill(-1);
  std::multimap hvmap = mbdgeom->get_hvmap();
  for ( auto hv : hvmap )
  {
    int hvmod = hv.first;
    int pmt = hv.second;
    double gain = mcal->get_qgain( pmt );
    cout << hvmod << "\t" << pmt << "\t" << gain << endl;

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
  TCanvas *ac = new TCanvas("c_gainsbyhvmod","Gains by HV Mod",1250,600);
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

  // calculate means, and the scaling to normalize all of them to the same
  std::array<double,16> means;
  means.fill(0.);
  
  cout << "means in each hv group" << endl;
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
    cout << ihv << "\t" << means[ihv] << "\t" << newscale[ihv] << "\t" << new_gmin[ihv] << endl;

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

  cout << "MIN " << minpmt[minhvmod] << "\t" << minhvmod << "\t" << new_gmin[minhvmod] << endl;
  cout << "MAX " << maxpmt[maxhvmod] << "\t" << maxhvmod << "\t" << new_gmax[maxhvmod] << endl;
  cout << "SPREAD " <<  new_gmax[maxhvmod]/new_gmin[minhvmod] << endl;


  //double minscale = 150./overall_newgmin;   // 150 is the min ADC we want in p+p
  //double minscale = 65./overall_newgmin;   // 65 is the min ADC we want in Au+Au
  double minscale = 80./overall_newgmin;   // 80 is the min ADC we want in Au+Au

  cout << "new means in each hv group" << endl;
  cout << "hvmod\tnewscale\tnew means [ADC/mip]" << endl;
  for (int ihv=0; ihv<16; ihv++)
  {
    newscale[ihv] *= minscale;
    cout << ihv << "\t" << newscale[ihv] << "\t" << means[ihv]*newscale[ihv] << endl;
  }

  // Plot the gain curves by hvmod
  //TCanvas *bc = new TCanvas("c_gaincurvesbyhvmod","Gain curves by hvmod",1200,1200,.0001,.0001);
  TCanvas *bc = new TCanvas("c_gaincurvesbyhvmod","Gain curves by hvmod",1200,1200);
  bc->Divide(4,4);
  TFile *gcurvefile = new TFile("SCAN/mbdlaser_202405151654.root");
  TGraphErrors *g_norm_laserscan[128]{nullptr};
  TGraph *ginv_norm_laserscan[128]{nullptr};  // inverse curves
  for (int ipmt=0; ipmt<128; ipmt++)
  {
    name = "g_norm_laserscan"; name += ipmt;
    g_norm_laserscan[ipmt] = (TGraphErrors*)gcurvefile->Get( name );
    int n = g_norm_laserscan[ipmt]->GetN();
    double *x = g_norm_laserscan[ipmt]->GetX();
    double *y = g_norm_laserscan[ipmt]->GetY();
    ginv_norm_laserscan[ipmt] = new TGraph(n,y,x);
    name = "ginv_norm_laserscan"; name += ipmt;
    ginv_norm_laserscan[ipmt]->SetName( name );
  }

  std::array<double,16> ref_hvsetting {
      0.758629, 0.762746, 0.804495, 0.7953, 0.766233, 0.818163, 0.797741, 0.775627,   // south
      0.819641, 0.798468, 0.860734, 0.798129, 0.874018, 0.793739, 0.873797, 0.852621  // north
  };
  //ref_hvsetting.fill(0.81);      // the gain settings that were used for run analyzed
  std::array<double,16> sumsetting;
  sumsetting.fill(0.);
  std::array<int,16> npmts_in_hvmod;
  npmts_in_hvmod.fill(0.);
  std::array<double,16> newsetting;
  newsetting.fill(0.);

  for ( auto hv : hvmap )
  {
    int hvmod = hv.first;
    int pmt = hv.second;

    bc->cd( hvmod+1 );
    if ( npmts_in_hvmod[hvmod] == 0 ) g_norm_laserscan[pmt]->Draw("acp");
    else                              g_norm_laserscan[pmt]->Draw("cp");
    ++npmts_in_hvmod[hvmod];

    //cout << "ref_hvset " << hvmod << "\t" << ref_hvsetting[hvmod] << endl;
    double ref_gain = g_norm_laserscan[pmt]->Eval( ref_hvsetting[hvmod] );
    double target_gain = newscale[hvmod]*ref_gain;

    sumsetting[hvmod] += ginv_norm_laserscan[pmt]->Eval( target_gain );
    //cout << "xxx " << pmt << "\t" << ref_gain << "\t" << target_gain << "\t" << ginv_norm_laserscan[pmt]->Eval( target_gain ) << endl;
  }

  cout << "New Settings" << endl;
  for (int ihv=0; ihv<16; ihv++)
  {
    newsetting[ihv] = sumsetting[ihv]/npmts_in_hvmod[ihv];
    cout << ihv << "\t" << newsetting[ihv] << endl;
  }

  // to plug into hvscale.py
  cout << "hvscale = [ ";
  // first the north
  for (int ihv=8; ihv<16; ihv++)
  {
    cout << newsetting[ihv] << ", ";
  }
  for (int ihv=0; ihv<8; ihv++)
  {
    cout << newsetting[ihv] << ", ";
  }
  cout << " ]" << endl;
}
