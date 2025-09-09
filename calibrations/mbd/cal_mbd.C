#include <iostream>
#include <fstream>

#include <TString.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TSystem.h>

#include <ffamodules/CDBInterface.h>
#include <phool/recoConsts.h>

#include "get_runstr.h"
#include "make_cdbtree.C"
#include <mbd/MbdCalib.h>
#include <mbd/MbdDefs.h>
#include "read_dstmbd.C"
#include "recal_mbd_mip.C"

#if defined(__CLING__)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libphool.so)
R__LOAD_LIBRARY(libmbd.so)
#endif

// This macro executes the sub-passes for the pass2 calibrations
//pass 2.0: do tt_t0 and tq_t0 offset calibration
//pass 2.1: do the slew correction calibration
//pass 2.2: do the next iteration of the slew correction calibration
//pass 2.3: do the mip fits for charge calibration 

//runtype 0: au+au 200 GeV
//runtype 1: p+p 200 GeV
void cal_mbd(const char *tfname = "DST_MBDUNCAL-00020869-0000.root", const int subpass = 0, const int nevt = 0, const int runtype = 0, const std::string_view dbtag = "")
{
  cout << "cal_mbd(), tfname " << tfname << endl;
  cout << "cal_mbd(), runtype " << runtype << endl;
  read_dstmbd( tfname );

  //== Create output directory (should already exist)
  int runnumber = get_runnumber(tfname);
  TString dir = "results/";
  dir += runnumber;
  dir += "/";
  TString name = "mkdir -p " + dir;
  gSystem->Exec( name );

  cout << name << endl;

  //== Load in calib constants
  //Float_t tq_t0_offsets[MbdDefs::MBD_N_PMT] = {};

  MbdCalib *mcal = new MbdCalib();
  mcal->Verbosity(1);

  // Load local mbd_sampmax and ped if they exist
  TString calfile = dir + "/mbd_sampmax.calib";
  if ( gSystem->AccessPathName(calfile)==0 )
  {
    mcal->Download_SampMax( calfile.Data() );
  }
  calfile = dir + "/mbd_ped.calib";
  if ( gSystem->AccessPathName(calfile)==0 )
  {
    mcal->Download_Ped( calfile.Data() );
  }

  TString savefname = dir;
  if ( subpass==0 )
  {
    savefname += "calmbdpass2."; savefname += subpass; savefname += "_time-"; savefname += runnumber; savefname += ".root";

    // temp, for final t0
    /*
    calfile = dir + "/mbd_slewcorr.calib";
    mcal->Download_SlewCorr( calfile.Data() );
    cout << "Loaded " << calfile << endl;
    */
  }
  else if ( subpass==1 || subpass==2 )
  {
    savefname += "calmbdpass2."; savefname += subpass; savefname += "_slew-"; savefname += runnumber; savefname += ".root";
  }
  else if ( subpass==3 )
  {
    savefname += "calmbdpass2."; savefname += subpass; savefname += "_q-"; savefname += runnumber; savefname += ".root";
  }
  cout << "saving to " << savefname << endl;

  // Load whatever calibrations are available at each subpass
  if ( subpass==3 )
  {
    calfile = dir + "/pass0_mbd_tq_t0.calib";
    mcal->Download_TQT0( calfile.Data() );
    cout << "Loaded " << calfile << endl;

    calfile = dir + "/pass0_mbd_tt_t0.calib";
    mcal->Download_TTT0( calfile.Data() );
    cout << "Loaded " << calfile << endl;
  }
  if ( subpass>1 )
  {
    if ( dbtag.empty() )
    {
      calfile = dir + "/mbd_slewcorr.calib";
      mcal->Download_SlewCorr( calfile.Data() );
      cout << "Loaded " << calfile << endl;
    }
    else
    {
      recoConsts *rc = recoConsts::instance();
      rc->set_StringFlag("CDB_GLOBALTAG","newcdbtag"); 
      rc->set_uint64Flag("TIMESTAMP",runnumber);
      CDBInterface *cdb = CDBInterface::instance();
      std::string slew_url = cdb->getUrl("MBD_SLEWCORR");
      mcal->Download_SlewCorr(slew_url);
      cout << "Loaded " << slew_url << endl;
    }
  }
  if ( subpass>3 )
  {
    calfile = dir + "/mbd_qfit.calib";
    mcal->Download_Gains( calfile.Data() );
    cout << "Loaded " << calfile << endl;
  }

  TFile *savefile = new TFile(savefname,"RECREATE");

  TH1 *h_q[MbdDefs::MBD_N_PMT];
  TH1 *h_tt[MbdDefs::MBD_N_PMT];
  TH1 *h_tq[MbdDefs::MBD_N_PMT];

  TH2 *h2_slew[MbdDefs::MBD_N_PMT];

  TString title;
  for (int ipmt=0; ipmt<MbdDefs::MBD_N_PMT; ipmt++)
  {
    name = "h_q"; name += ipmt;
    title = "q"; title += ipmt;
    h_q[ipmt] = new TH1F(name,title,3000,-100,15000-100);
    h_q[ipmt]->SetXTitle("ADC");

    name = "h_tt"; name += ipmt;
    title = "tt"; title += ipmt;
    //h_tt[ipmt] = new TH1F(name,title,7000,-150,31*17.76);
    h_tt[ipmt] = new TH1F(name,title,7000,-30.,30.);
    h_tt[ipmt]->SetXTitle("ns");

    name = "h_tq"; name += ipmt;
    title = "tq"; title += ipmt;
    h_tq[ipmt] = new TH1F(name,title,7000,-150,31*17.76);
    h_tq[ipmt]->SetXTitle("ns");

    if ( subpass>0 )
    {
      name = "h2_slew"; name += ipmt;
      title = "slew curve, ch "; title += ipmt;
      h2_slew[ipmt] = new TH2F(name,title,4000,-0.5,16000-0.5,1100,-5,6);
      h2_slew[ipmt]->SetXTitle("ADC");
      h2_slew[ipmt]->SetYTitle("#Delta T (ns)");
    }
  }
  TH2 *h2_tq = new TH2F("h2_tq","ch vs tq",900,-150,150,MbdDefs::MBD_N_PMT,-0.5,MbdDefs::MBD_N_PMT-0.5);
  h2_tq->SetXTitle("tq [ns]");
  h2_tq->SetYTitle("pmt ch");
  TH2 *h2_tt = new TH2F("h2_tt","ch vs tt",900,-150,150,MbdDefs::MBD_N_PMT,-0.5,MbdDefs::MBD_N_PMT-0.5);
  h2_tt->SetXTitle("tt [ns]");
  h2_tt->SetYTitle("pmt ch");

  // Event loop, each ientry is one triggered event
  int nentries = tree->GetEntries();
  if ( nevt!=0 && nevt<nentries )
  {
    nentries = nevt;
  }

  double armtime[2]{0.,0.};
  double nhit[2]{0.,0.};
  float  ttcorr[128] = {0.};

  std::cout << "Processing " << nentries << std::endl;
  for (int ientry=0; ientry<nentries; ientry++)
  {
    dstmbd_GetEntry(ientry);

    if (ientry<4)
    {
      // print charge from channels 0 and 127
      cout << f_evt << "\tch0\t" << f_tt[0] << "\t" << f_tq[0] << "\t" << f_q[0] << endl;
      cout << "ch127\t" << f_tt[127] << "\t" << f_tq[127] << "\t" << f_q[127] << endl;
    }

    // make npmt cut
    /*
    if ( f_npmt==0 )
    {
      cout << "f_npmt == 0" << endl;
      continue;
    }
    */

    // Make vertex cut
    if ( subpass!=0 && fabs(f_bz)>60. )
    {
      continue;
    }

    if ( subpass>0 )
    {
      armtime[0] = 0.;
      armtime[1] = 0.;
      nhit[0] = 0.;
      nhit[1] = 0.;
    }

    for (int ipmt=0; ipmt<MbdDefs::MBD_N_PMT; ipmt++)
    {
      int arm = ipmt/64;

      ttcorr[ipmt] = f_tt[ipmt];
      float tq = f_tq[ipmt];

      if ( subpass>0 )
      {
        if ( !isnan(mcal->get_tt0(ipmt)) && mcal->get_tt0(ipmt)>-100. && f_q[ipmt]>0. && f_q[ipmt]<16000. )
        {
          ttcorr[ipmt] -= mcal->get_tt0(ipmt);
        }
        if ( !isnan(mcal->get_tq0(ipmt)) && mcal->get_tq0(ipmt)>-100. )
        {
          tq -= mcal->get_tq0(ipmt);
        }
      }

      //if ( subpass==0 )  // temp, for final t0
      if ( subpass>1 && subpass<3 ) // apply slewcorr if subpass > 1
      {
        int feech = (ipmt / 8) * 16 + ipmt % 8;
        if ( !isnan(mcal->get_tt0(ipmt)) && mcal->get_tt0(ipmt)>-100. && f_q[ipmt]>0. && f_q[ipmt]<16000. )
        {
          ttcorr[ipmt] -= mcal->get_scorr(feech,f_q[ipmt]);
        }
      }

      h_tt[ipmt]->Fill( ttcorr[ipmt] );
      h2_tt->Fill( ttcorr[ipmt], ipmt );

      h_tq[ipmt]->Fill( tq );
      h2_tq->Fill( tq, ipmt );

      if ( subpass==0 && fabs(f_tt[ipmt])<26. )
      {
        h_q[ipmt]->Fill( f_q[ipmt] );
      }
      //else if ( subpass>0 && fabs(ttcorr[ipmt])<26. )
      else if ( subpass>0 && (fabs(ttcorr[ipmt])<26.||f_q[ipmt]>40.) )  // to get around high threshold
      {
        h_q[ipmt]->Fill( f_q[ipmt] );

        //if ( f_q[ipmt] > 1000. && fabs(ttcorr[ipmt])<26. )    // for p+p
        //if ( f_q[ipmt] > 2000. && fabs(ttcorr[ipmt])<26. )
        if ( fabs(ttcorr[ipmt])<26. )
        {
          nhit[arm] += 1.0;
          armtime[arm] += ttcorr[ipmt];
        }

        // check charge
        // why cuts on f_bn?
        /*
        if ( arm==0 && f_bn[0]<30 )
        {
          h_q[ipmt]->Fill( f_q[ipmt] );
        }
        else if ( arm==1 && f_bn[1]<30 )
        {
          h_q[ipmt]->Fill( f_q[ipmt] );
        }
        */
      }
    } // end loop over PMTs

    // calc meantime for high amplitude
    for (int iarm=0; iarm<2; iarm++)
    {
      if ( nhit[iarm]>1. )
      {
        armtime[iarm] = armtime[iarm]/nhit[iarm];
      }
      //cout << "aaa " << iarm << "\t" << nhit[iarm] << "\t" << armtime[iarm] << endl;
    }

    for (int ipmt=0; ipmt<MbdDefs::MBD_N_PMT; ipmt++)
    {
      //int ifeech = (ipmt/8)*16 + 8 + ipmt%8;  // time ifeech only
      int arm = ipmt/64;

      if ( nhit[arm]<2 || f_q[ipmt]<=0. ) continue;

      double dt = ttcorr[ipmt] - armtime[arm];

      //cout << "filling" << endl;
      h2_slew[ipmt]->Fill( f_q[ipmt], dt );
    }
  }

  TCanvas *ac[100];
  int cvindex = 0;

  TString pdfname;  // output pdf

  //== Calculate tq_t0 and tt_t0 on subpass, or apply tt_t0 if subpass>0
  ac[cvindex] = new TCanvas("cal_tt","ch vs tt",425*1.5,550*1.5);
  h2_tt->Draw("colz");
  pdfname = dir; pdfname += "calmbdpass2."; pdfname += subpass; pdfname += "_time-"; pdfname += runnumber; pdfname += ".pdf";
  if ( subpass==0 )
  {
    //name = dir + "h2_tt.png";
    title = "t0, Timing Ch's";
  }
  else if ( subpass>0 )
  {
    //name = dir + "h2_ttcorr.png";
    title = "t0 corrected, Timing Ch's";
    h2_tt->GetXaxis()->SetRangeUser( -12.5, 12.5 );
    gPad->Modified();
    gPad->Update();
  }
  cout << pdfname << endl;
  ac[cvindex]->Print( pdfname + "[");
  ac[cvindex]->Print( pdfname, title );
  ++cvindex;

  // now tq_t0
  ac[cvindex] = new TCanvas("cal_tq","ch vs tq",425*1.5,550*1.5);
  h2_tq->Draw("colz");
  if ( subpass==0 )
  {
    //name = dir + "h2_tq.png";
    title = "t0, Charge Ch's";
  }
  else if ( subpass>0 )
  {
    //name = dir + "h2_tqcorr.png";
    title = "t0 corrected, Charge Ch's";
    h2_tq->GetXaxis()->SetRangeUser( -20,20 );
    gPad->Modified();
    gPad->Update();
  }
  cout << pdfname << endl;
  ac[cvindex]->Print( pdfname, title );
  ++cvindex;

  // Here we calculate tt_t0 and tq_t0, starting with tt_t0 first
  ac[cvindex] = new TCanvas("cal_tt_ch","tt",550*1.5,425*1.5);
  gPad->SetLogy(1);

  ofstream cal_tt_t0_file;
  TString cal_fname = dir; cal_fname += "pass"; cal_fname += subpass; cal_fname += "_mbd_tt_t0.calib";
  cal_tt_t0_file.open( cal_fname );
  cout << "Creating " << cal_fname << endl;

  TF1 *gaussian = new TF1("gaussian","gaus",-25,25);
  gaussian->SetLineColor(2);
  double min_twindow = -25.;
  double max_twindow = 25.;

  for (int ipmt=0; ipmt<MbdDefs::MBD_N_PMT; ipmt++)
  {
    // only look in the middle
    if ( ipmt==0 || ipmt==64 )
    {
      // use wide range for 1st channel in each arm
      h_tt[ipmt]->SetAxisRange(-25.,25.);
      //h_tt[ipmt]->SetAxisRange(9.,13.);
    }
    else
    {
      // for subsequent channels in an arm, use a window
      // determined by the 1st channel
      h_tt[ipmt]->SetAxisRange(min_twindow,max_twindow);
    }
    Double_t peak = h_tt[ipmt]->GetMaximum();
    int peakbin = h_tt[ipmt]->GetMaximumBin();
    Double_t mean = h_tt[ipmt]->GetBinCenter( peakbin );
    Double_t sigma = 1.0;
    //Double_t sigma = 3.0;
    gaussian->SetParameters( peak, mean, 5 );
    gaussian->SetRange( mean-3*sigma, mean+3*sigma );

    if ( ipmt==0 || ipmt==64 )
    {
      min_twindow = mean - 3*sigma;
      max_twindow = mean + 3*sigma;
    }

    h_tt[ipmt]->Fit(gaussian,"R");
    h_tt[ipmt]->SetAxisRange(mean-15.*sigma,mean+15.*sigma);
    //gPad->SetLogy(1);
    mean = gaussian->GetParameter(1);
    Double_t meanerr = gaussian->GetParError(1);
    sigma = gaussian->GetParameter(2);
    Double_t sigmaerr = gaussian->GetParError(2);

    // normalize th2 histogram
    int nbinsx = h2_tt->GetNbinsX();
    for (int ibinx=1; ibinx<=nbinsx; ibinx++)
    {
      Double_t fitpeak = gaussian->GetParameter(0);
      if ( fitpeak!=0 )
      {
        Float_t bincontent = h2_tt->GetBinContent(ibinx,ipmt+1);
        h2_tt->SetBinContent(ibinx,ipmt+1,bincontent/fitpeak);
      }
    }

    cal_tt_t0_file << ipmt << "\t" << mean << "\t" << meanerr << "\t" << sigma << "\t" << sigmaerr << endl;
    cout << ipmt << "\t" << mean << "\t" << meanerr << "\t" << sigma << "\t" << sigmaerr << endl;

    if ( subpass==0 )
    {
      //name = dir + "h_tt"; name += ipmt; name += ".png";
      title = "h_tt"; title += ipmt;
    }
    else if ( subpass==1 || subpass==2 )
    {
      //name = dir + "h_ttcorr"; name += ipmt; name += ".png";
      title = "h_ttcorr"; title += ipmt;
    }
    //cout << title << endl;
    ac[cvindex]->Print( pdfname, title );
  }
  cal_tt_t0_file.close();
  make_cdbtree( cal_fname );
  ++cvindex;

  // Now calculate tq_t0
  ac[cvindex] = new TCanvas("cal_tq_ch","tq",550*1.5,425*1.5);
  gPad->SetLogy(1);

  ofstream cal_tq_t0_file;
  cal_fname = dir; cal_fname += "pass"; cal_fname += subpass; cal_fname += "_mbd_tq_t0.calib";
  cal_tq_t0_file.open( cal_fname );

  for (int ipmt=0; ipmt<MbdDefs::MBD_N_PMT; ipmt++)
  {
    // only look in the middle
    if ( ipmt==0 || ipmt==64 )
    {
      // use wide range for 1st channel in each arm
      h_tq[ipmt]->SetAxisRange(-25.,25.);
      //h_tq[ipmt]->SetAxisRange(9.,13.); // kludge to select middle bunch during stochastic cooling
    }
    else
    {
      // for subsequent channels in an arm, use a window
      // determined by the 1st channel
      h_tq[ipmt]->SetAxisRange(min_twindow,max_twindow);
    }
    Double_t peak = h_tq[ipmt]->GetMaximum();
    int peakbin = h_tq[ipmt]->GetMaximumBin();
    Double_t mean = h_tq[ipmt]->GetBinCenter( peakbin );
    Double_t sigma = 1.0;
    //Double_t sigma = 3.0;
    gaussian->SetParameters( peak, mean, 5 );
    gaussian->SetRange( mean-3*sigma, mean+3*sigma );

    if ( ipmt==0 || ipmt==64 )
    {
      min_twindow = mean - 3*sigma;
      max_twindow = mean + 3*sigma;
    }

    h_tq[ipmt]->Fit(gaussian,"R");
    h_tq[ipmt]->SetAxisRange(mean-12.*sigma,mean+12.*sigma);
    //gPad->SetLogy(1);
    mean = gaussian->GetParameter(1);
    Double_t meanerr = gaussian->GetParError(1);
    sigma = gaussian->GetParameter(2);
    Double_t sigmaerr = gaussian->GetParError(2);

    // normalize th2 histogram
    int nbinsx = h2_tq->GetNbinsX();
    for (int ibinx=1; ibinx<=nbinsx; ibinx++)
    {
      Double_t fitpeak = gaussian->GetParameter(0);
      if ( fitpeak!=0 )
      {
        Float_t bincontent = h2_tq->GetBinContent(ibinx,ipmt+1);
        h2_tq->SetBinContent(ibinx,ipmt+1,bincontent/fitpeak);
      }
    }

    cal_tq_t0_file << ipmt << "\t" << mean << "\t" << meanerr << "\t" << sigma << "\t" << sigmaerr << endl;

    if ( subpass==0 )
    {
      //name = dir + "h_tq"; name += ipmt; name += ".png";
      title = "h_tq"; title += ipmt;
    }
    else if ( subpass==1 || subpass==2 )
    {
      //name = dir + "h_tqcorr"; name += ipmt; name += ".png";
      title = "h_tqcorr"; title += ipmt;
    }
    //cout << name << endl;
    ac[cvindex]->Print( pdfname, title );
  }

  cal_tq_t0_file.close();
  make_cdbtree( cal_fname );

  ac[0]->Print( pdfname + "]" );
  ++cvindex;

  if ( subpass==1 || subpass==2 )
  {
    //== Draw the slewcorr histograms
    ac[cvindex] = new TCanvas("cal_slew","slew",425*1.5,550*1.5);
    pdfname = dir; pdfname += "calmbdpass2."; pdfname += subpass; pdfname += "_slew-"; pdfname += runnumber; pdfname += ".pdf";
    ac[cvindex]->Print( pdfname + "[" );

    for (int ipmt=0; ipmt<MbdDefs::MBD_N_PMT; ipmt++)
    {
      h2_slew[ipmt]->Draw("colz");
      gPad->SetLogz(1);

      //name = dir + "h2_slew"; name += ipmt; name += "_pass"; name += subpass; name += ".png";
      name = dir + "h2_slew"; name += ipmt; name += "_pass"; name += subpass;
      cout << name << endl;
      ac[cvindex]->Print( pdfname, name );
    }

    // Here we could perhaps add a summary page
 
    ac[cvindex]->Print( pdfname + "]" );
    ++cvindex;
  }

  //== Draw the charge histograms
  if ( subpass==0 )
  {
    ac[cvindex] = new TCanvas("cal_q","q",425*1.5,550*1.5);

    pdfname = dir; pdfname += "calmbdpass2."; pdfname += subpass; pdfname += "_adc-"; pdfname += runnumber; pdfname += ".pdf";
    ac[cvindex]->Print( pdfname + "[" );

    gPad->SetLogy(1);

    for (int ipmt=0; ipmt<MbdDefs::MBD_N_PMT && subpass==0; ipmt++)
    {
      h_q[ipmt]->Draw();

      //name = dir + "h_adc"; name += ipmt; name += ".png";
      title = "h_adc"; title += ipmt;
      //cout << pdfname << " " << title << endl;
      ac[cvindex]->Print( pdfname, title );
    }
    ac[cvindex]->Print( pdfname + "]" );
    ++cvindex;
  }

  savefile->Write();
  //savefile->Close();

  if ( subpass==3 )
  {
    recal_mbd_mip( tfname, subpass, runtype );
  }

}

