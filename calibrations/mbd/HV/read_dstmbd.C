#ifndef MACRO_READ_DSTMBD_C
#define MACRO_READ_DSTMBD_C
//
// macro to read in MBD data after the waveforms are processed
// at this stage, we only have charge and time from each channel, not the full waveform
//
#include <mbd/MbdDefs.h>
#include <mbd/MbdOut.h>
#include <mbd/MbdPmtContainer.h>
#include <mbd/MbdPmtHit.h>

#include <TChain.h>
#include <TFile.h>

#include <fstream>

R__LOAD_LIBRARY(libmbd_io.so)

// Set up variables to read from TTree
Int_t   f_evt{0};
UShort_t f_clk{0};
UShort_t f_femclk{0};
Short_t  f_npmt;
Float_t f_tt[MbdDefs::MBD_N_PMT];  // time from t-channels
Float_t f_tq[MbdDefs::MBD_N_PMT];  // time from q-channels
Float_t f_q[MbdDefs::MBD_N_PMT];   // charge

Short_t f_bn[MbdDefs::MBD_N_ARMS]; // num hit PMTs
Float_t f_bq[MbdDefs::MBD_N_ARMS]; // chargesum
Float_t f_bt[MbdDefs::MBD_N_ARMS]; // mean time in arm
Float_t f_bz;                      // z-vertex
Float_t f_bt0;                     // t-zero

MbdOut *mbdout{nullptr};
MbdPmtContainer *mbdpmts{nullptr};

TFile *tfile {nullptr};
TTree *tree {nullptr};

void dstmbd_GetEntry(const int ientry)
{
    tree->GetEntry(ientry);

    f_evt = mbdout->get_evt();
    f_clk = mbdout->get_clock();
    f_femclk = mbdout->get_femclock();
    f_bz = mbdout->get_zvtx();
    f_bt0 = mbdout->get_t0();
    for (int iarm=0; iarm<2; iarm++)
    {
      f_bn[iarm] = mbdout->get_npmt(iarm);
      f_bq[iarm] = mbdout->get_q(iarm);
      f_bt[iarm] = mbdout->get_time(iarm);
    }
    f_npmt = f_bn[0] + f_bn[1];

    // clear pmt info
    for (int ipmt=0; ipmt<128; ipmt++)
    {
      f_q[ipmt] = 0.;
      f_tt[ipmt] = NAN;
      f_tq[ipmt] = NAN;
    }

    // Loop over each MBD/BBC PMT container
    for (int ipmt=0; ipmt<mbdpmts->get_npmt(); ipmt++)
    {
      MbdPmtHit* mbdpmt = mbdpmts->get_pmt(ipmt);

      f_q[ipmt] = mbdpmt->get_q();
      f_tt[ipmt] = mbdpmt->get_tt();
      f_tq[ipmt] = mbdpmt->get_tq();
    }
}

void read_dstmbd(const char *tfname = "beam_mbd-00009184-0000_mbd.root")
{
  std::cout << "tfname " << tfname << std::endl;

  // Set up TTree
//  int is_dst = 0; // whether reading from DST or private root files
  tfile = new TFile(tfname,"READ");
  tree = (TTree*)tfile->Get("T");

  tree->SetBranchAddress("DST#MBD#MbdOut",&mbdout);
  tree->SetBranchAddress("DST#MBD#MbdPmtContainer",&mbdpmts);

  /* older dst style, with better leaf level
  tree->SetBranchAddress("EvtSequence",&f_evt);
  tree->SetBranchAddress("clk", &f_clk);
  tree->SetBranchAddress("femclk", &f_femclk);
  tree->SetBranchAddress("bns",&f_bn[0]);
  tree->SetBranchAddress("bnn",&f_bn[1]);
  tree->SetBranchAddress("bqs",&f_bq[0]);
  tree->SetBranchAddress("bqn",&f_bq[1]);
  tree->SetBranchAddress("bts",&f_bt[0]);
  tree->SetBranchAddress("btn",&f_bt[1]);
  tree->SetBranchAddress("bz",&f_bz);
  tree->SetBranchAddress("bt0",&f_bt0);
  tree->SetBranchAddress("MbdPmtHits.bq",f_q);
  tree->SetBranchAddress("MbdPmtHits.btt",f_tt);
  tree->SetBranchAddress("MbdPmtHits.btq",f_tq);
  */

  // Event loop, each ientry is one triggered event
  int nentries = tree->GetEntries();
  std::cout << "checking output of dst file " << tfname << std::endl;
  std::cout << "nentries = " << nentries << std::endl;
  for (int ientry=0; ientry<10; ientry++)
  {
    //std::cout << "ientry " << ientry << std::endl;
    dstmbd_GetEntry(ientry);
    //tree->GetEntry(ientry+100);

    if (ientry<10)
    {
      // print charge from channels 0 and 127
      std::cout << f_evt << "\t" << f_q[0] << "\t" << f_tt[0] << std::endl;
      std::cout <<  "\t" << f_q[127] << "\t" << f_tt[127] << std::endl;
      std::cout <<  "\t" << f_bz << std::endl;
    }

  }

}
#endif
