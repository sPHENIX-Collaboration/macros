//
// Plot laser amplitudes, make gain curves, extract HV to run gain at 
//
#include <TH1.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TFile.h>
#include <TGraphErrors.h>

#include <iostream>
#include <fstream>
#include <vector>

#include "read_dstmbd.C"

const int MAXRUNS = 100;    // 11 is the typical
const int MAXCH = 256;
const int N_PMT = 128;
const int MAXBOARDS = MAXCH/16;     // FEM boards
int NCH;                 // total number of channels (including charge channels)
int NBOARDS;

TH1 *h_laseramp[MAXRUNS][MAXCH];    //[run][ch] 
TF1 *fgaus[MAXRUNS][MAXCH];
int nrun = 0;
int run_number[MAXRUNS];
float hvscale[MAXRUNS];             // Scale from final Au+Au
float hvset[MAXRUNS];               // HV setting
float laseramp[MAXCH][MAXRUNS];
float laseramperr[MAXCH][MAXRUNS];
float norm_laseramp[MAXCH][MAXRUNS];
float norm_laseramperr[MAXCH][MAXRUNS];
TGraphErrors *g_laserscan[MAXCH];
TGraphErrors *g_norm_laserscan[MAXCH];

TF1 *f_gain[N_PMT];

const int verbose = 1;

TString name;
TString title;

using namespace std;

// 
//void anafile(const char *tfname = "prdf_478_times.root", const int nrun = 0)
void anafile(const char *tfname = "DST_UNCALMBD-00042674-0000.root", const int nrun = 0)
{
  cout << "tfname " << tfname << endl;

  // Book Histograms, etc
  for (int ich=0; ich<N_PMT; ich++)
  {
    name = "h_laseramp"; name += ich; name += "_"; name += nrun;
    title = name;
    h_laseramp[nrun][ich] = new TH1F(name,title,1620,-100,16100);
    //h_laseramp[nrun][ich] = new TH1F(name,title,160,0,16000);
  }

  /*
  Int_t   evt;
  Float_t f_tt[MAXCH];  // time, t-ch
  Float_t f_tq[MAXCH];  // time, q-ch
  Float_t f_q[MAXCH];   // charge
  */

  cout << "tfname " << tfname << endl;

  read_dstmbd(tfname);
  /*
  TFile *tfile = new TFile(tfname,"READ");
  TTree *tree = (TTree*)tfile->Get("t");
  tree->SetBranchAddress("evt",&evt);
  for (int ich=0; ich<N_PMT; ich++)
  {
    name = "tt"; name += ich;
    tree->SetBranchAddress(name,&f_tt[ich]);
    name = "tq"; name += ich;
    tree->SetBranchAddress(name,&f_tq[ich]);
    name = "q"; name += ich;
    tree->SetBranchAddress(name,&f_q[ich]);
  }
  */

  int nentries = tree->GetEntries();
  // skip 1st 70 because those are possibly bad
  for (int ientry=70; ientry<nentries; ientry++)
  {
    tree->GetEntry(ientry);

    for (int ich=0; ich<N_PMT; ich++)
    {
      //cout << evt << "\t" << t[1] << "\t" << f_q[1] << "\t" << t[14] << "\t" << f_q[14] << endl;
      if ( f_q[ich]>0 )
      {
        h_laseramp[nrun][ich]->Fill( f_q[ich] );
      }
    }
  }

  // Fit gaussians to get the average amplitude
  for (int ich=0; ich<N_PMT; ich++)
  {
    //cout << "Fitting ch " << ich << endl;
    name = "fgaus"; name += ich; name += "_"; name += nrun;
    fgaus[nrun][ich] = new TF1(name,"gaus",-100,16000);
    fgaus[nrun][ich]->SetLineColor(2);

    //h_laseramp[nrun][ich]->Rebin(2);

    float ampl = 0.;
    float amplerr = 0.;
    if ( h_laseramp[nrun][ich]->GetEntries() > 10 )
    {
      double seed_mean = h_laseramp[nrun][ich]->GetMean();
      double seed_rms = h_laseramp[nrun][ich]->GetRMS();
      fgaus[nrun][ich]->SetParameters(1000,seed_mean,seed_rms);
      fgaus[nrun][ich]->SetRange( seed_mean-5*seed_rms, seed_mean+5*seed_rms );
      //h_laseramp[nrun][ich]->Fit( fgaus[nrun][ich], "QR" );
      h_laseramp[nrun][ich]->Fit( fgaus[nrun][ich], "LLRQ" );
    }

    ampl = fgaus[nrun][ich]->GetParameter(1);
    amplerr = fgaus[nrun][ich]->GetParError(1);
    laseramp[ich][nrun] = ampl;
    laseramperr[ich][nrun] = amplerr;
  }

  if ( verbose && nrun==0 )
  {
    const int NCANVAS = 4;
    TCanvas *c_laseramp[NCANVAS];
    for (int icv = 0; icv<NCANVAS; icv++)
    {
      name = "c_laser"; name += icv;
      c_laseramp[icv] = new TCanvas(name,name,1600,800);
      c_laseramp[icv]->Divide(8,4);
    }

    for (int ipmt=0; ipmt<N_PMT; ipmt++)
    {
      int quad = ipmt/32;    // quadrant
      c_laseramp[quad]->cd( ipmt%32 + 1 );
      //h_laseramp[nrun][ipmt]->Rebin(100);
      h_laseramp[nrun][ipmt]->Draw();
      /*
      gPad->SetLogy(1);
      gPad->Modified();
      gPad->Update();
      */
    }

  }

}

float prev_ampl[N_PMT];

void get_prev_gains(const char *prev_gains_file = "gains_at_9_zerofield.out")
{
    ifstream infile( prev_gains_file );
    int ch;
    float hvscale;
    float ratio;
    while ( infile >> ch >> hvscale >> ratio )
    {
        if ( ch>=0 && ch<128 )
        {
          infile >> prev_ampl[ch];
          cout << ch << "\t" << endl;
        }
        else
        {
          cout << "Bad ch" << endl;
        }
    }
}

void analaserscan(const char *fname = "hvscan.62880")
{
  NCH = MAXCH;                 // total number of channels (including charge channels)
  NBOARDS = MAXBOARDS;

  //get_prev_gains();

  // Get the number of actual channels to process
  /*
  ifstream configfile("digsig.cfg");
  if ( configfile.is_open() )
  {
    string junk;
    configfile >> junk >> NCH;
    NBOARDS = NCH/16;

    cout << "Found config file digsig.cfg" << endl;
    cout << "Setting NCH = " << NCH << endl;
  }
  */

  TString rootfname;
  ifstream scanfile(fname);
  int nruns = 0;
  while ( scanfile >> run_number[nruns] >> hvscale[nruns] )
  {
    cout << run_number[nruns] << "\t" << hvscale[nruns] << endl;

    // get name of root file
    //rootfname.Form( "calib_mbd-%08d-0000_mbd.root", run_number[nruns] );
    rootfname.Form( "DST_UNCALMBD-%08d-0000.root", run_number[nruns] );
    cout << "Processing " << rootfname << endl;

    anafile( rootfname, nruns );
 
    nruns++;
  }
  cout << "Processed " << nruns << " runs" << endl;

  // Make the canvases for the gain curves
  const int NCANVAS = 4;
  TCanvas *c_laserscan[NCANVAS];
  for (int icv = 0; icv<NCANVAS; icv++)
  {
    name = "c_laserscan"; name += icv;
    c_laserscan[icv] = new TCanvas(name,name,1600,800);
    c_laserscan[icv]->Divide(8,4);
  }

  // Make and draw the gain curves, and
  // write out the HV scale needed to get XX gain
  name = "mbdlaser_"; name += fname; name += ".root";
  name.ReplaceAll("hvscan.","");
  TFile *savefile = new TFile(name,"RECREATE");
  ofstream gainfile("gains.out");
  ofstream gain9file("gains_at_9.out");
  float gain_wanted = 0.55;
  for (int ipmt=0; ipmt<N_PMT; ipmt++)
  {
    int quad = ipmt/32;  // quadrant

    // Create the TGraph with the summary results
    name = "g_laserscan"; name += ipmt;
    title = "ch "; title += ipmt;
    g_laserscan[ipmt] = new TGraphErrors(nruns,hvscale,laseramp[ipmt],0,laseramperr[ipmt]);
    g_laserscan[ipmt]->SetName(name);
    g_laserscan[ipmt]->SetTitle(title);
    g_laserscan[ipmt]->SetMarkerStyle(20);
    g_laserscan[ipmt]->SetMarkerSize(0.5);

    //cout << "ch " << pmtch << endl;
    c_laserscan[quad]->cd( ipmt%32 + 1 );
    g_laserscan[ipmt]->Draw("acp");
    g_laserscan[ipmt]->GetHistogram()->SetTitleSize(4);

    name = "f_gain"; name += ipmt;
    f_gain[ipmt] = new TF1(name,"[0]*exp([1]*x)-1",0,1.0);
    f_gain[ipmt]->SetParameters(1,1);
    //g_laserscan[ipmt]->Fit( f_gain[ipmt] );

    // now step down until we find the gain we want
    double maxgain = g_laserscan[ipmt]->Eval(1.0);

    /*
    double ref_gain = g_laserscan[ipmt]->Eval(0.81)/maxgain;
    gain9file << ipmt << "\t" << 0.81 << "\t" << ref_gain << "\t" << g_laserscan[ipmt]->Eval(0.81) << endl;

    for (double iv=0.5; iv<1.0; iv+=0.001)
    {
        double ampl = g_laserscan[ipmt]->Eval(iv);
        //double ratio = g_laserscan[ipmt]->Eval(iv)/maxgain;
        //if ( ratio>gain_wanted )
        //{
        //    gainfile << ipmt << "\t" << iv << "\t" << ratio << endl;
        //    break;
        //}

        if ( ampl > prev_ampl[ipmt] )
        {
            gainfile << ipmt << "\t" << iv << "\t" << ampl/prev_ampl[ipmt] << endl;
            break;
        }
    }
    */
  }

  gainfile.close();

  // Now create the normalized gain curves
  for (int ipmt=0; ipmt<N_PMT; ipmt++)
  {
    double ref_ampl = g_laserscan[ipmt]->Eval(1.0); 
    for (int irun=0; irun<nruns; irun++)
    {
      norm_laseramp[ipmt][irun] = laseramp[ipmt][irun]/ref_ampl;
      norm_laseramperr[ipmt][irun] = laseramperr[ipmt][irun]/ref_ampl;
    }
    name = "g_norm_laserscan"; name += ipmt;
    title = "ch "; title += ipmt; title += ", norm";
    g_norm_laserscan[ipmt] = new TGraphErrors(nruns,hvscale,norm_laseramp[ipmt],0,norm_laseramperr[ipmt]);
    g_norm_laserscan[ipmt]->SetName(name);
    g_norm_laserscan[ipmt]->SetTitle(title);
    g_norm_laserscan[ipmt]->SetMarkerStyle(20);
    g_norm_laserscan[ipmt]->SetMarkerSize(0.5);
  }

  for (int ipmt=0; ipmt<N_PMT; ipmt++)
  {
    g_laserscan[ipmt]->Write();
    g_norm_laserscan[ipmt]->Write();
  }

  savefile->Write();

  // dump full gainfile
  ofstream gain2file("gains_full.csv");

  // Header
  gain2file << "iv,";
  for (int ipmt=0; ipmt<N_PMT; ipmt++)
  {
      gain2file << ipmt << ",";
  }
  gain2file << endl;

  // Gains
  for (double iv=1.0; iv>0.5; iv -= 0.05)
  {
    gain2file << iv << ",";
    for (int ipmt=0; ipmt<N_PMT; ipmt++)
    {
        gain2file << g_norm_laserscan[ipmt]->Eval(iv) << ",";
    }
    gain2file << endl;
  }
  gain2file.close();

}

