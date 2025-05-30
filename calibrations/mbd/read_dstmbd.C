//
// macro to read in MBD data after the waveforms are processed
// at this stage, we only have charge and time from each channel, not the full waveform
//
#include <fstream>
#include <mbd/MbdDefs.h>
#include <TChain.h>

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

TFile *tfile {nullptr};
TTree *tree {nullptr};

void read_dstmbd(const char *tfname = "beam_mbd-00009184-0000_mbd.root")
{
  cout << "tfname " << tfname << endl;

  // Set up TTree
  int is_dst = 0; // whether reading from DST or private root files
  tfile = new TFile(tfname,"READ");
  tree = (TTree*)tfile->Get("T");

  //tree->SetBranchAddress("EvtSequence",&f_evt);
  tree->SetBranchAddress("evtseq",&f_evt);
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
  tree->SetBranchAddress("npmt",&f_npmt);
  tree->SetBranchAddress("MbdPmtHits.bq",f_q);
  tree->SetBranchAddress("MbdPmtHits.btt",f_tt);
  tree->SetBranchAddress("MbdPmtHits.btq",f_tq);

  // Event loop, each ientry is one triggered event
  int nentries = tree->GetEntries();
  cout << "checking output of dst file " << tfname << endl;
  cout << "nentries = " << nentries << endl;
  for (int ientry=0; ientry<10; ientry++)
  {
    //cout << "ientry " << ientry << endl;
    tree->GetEntry(ientry);
    //tree->GetEntry(ientry+100);

    if (ientry<10)
    {
      // print charge from channels 0 and 127
      cout << f_evt << "\t" << f_q[0] << "\t" << f_tt[0] << endl;
      cout <<  "\t" << f_q[127] << "\t" << f_tt[127] << endl;
    }

  }

}

