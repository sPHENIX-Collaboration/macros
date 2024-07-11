#include <cdbobjects/CDBTTree.h>  // for CDBTTree
#include <litecaloeval/LiteCaloEval.h>
#include "TowerInfoDefs.h"

#include "sPhenixStyle.C"

R__LOAD_LIBRARY(libLiteCaloEvalTowSlope.so)
R__LOAD_LIBRARY(libcdbobjects)


void TSCtoCDBTTree(const char * infile, const char * outputfile);
void mergeCDBTTrees(const char * infile1, const char * infile2, const char * outputfile);

void doTscFit(const std::string &hist_fname = "base/combine_out/out1.root", const std::string &calib_fname = "base/local_calib_copy.root",int iter=1)
{

  string fitoutfile = Form("tsc_fitout_it%d.root",iter);

  if (iter <= 2){
    LiteCaloEval modlce;
    modlce.CaloType(LiteCaloEval::CEMC);
    modlce.Get_Histos(hist_fname.c_str(),fitoutfile.c_str());
    modlce.m_myminbin =  8;  
    modlce.m_mymaxbin =  96; 
    modlce.setFitMin(0.20);
    modlce.setFitMax(0.8);
    if (iter==1) modlce.set_doQA(false);
    if (iter==2) modlce.set_doQA(true);
    modlce.FitRelativeShifts(&modlce,110);
  }

  if (iter==3) {
    SetsPhenixStyle();
    LiteCaloEval modlce;
    modlce.CaloType(LiteCaloEval::CEMC);
    modlce.Get_Histos(hist_fname.c_str(),fitoutfile.c_str());
    modlce.plot_cemc("figures");
  }
    

   // create the cdbttree from tsc output andd multiply the corrections 
   // into the base calibration to pickup for pi0 first iteration

  TSCtoCDBTTree(fitoutfile.c_str(),Form("tsc_output_cdb_it%d.root",iter));
  mergeCDBTTrees(Form("tsc_output_cdb_it%d.root",iter),calib_fname.c_str(),calib_fname.c_str());

  size_t pos = calib_fname.find_last_of('.');
  string f_calib_save_name = calib_fname;
  f_calib_save_name.insert(pos,Form("_postTSC_it%d",iter));

  TFile* f_calib_mod = new TFile(calib_fname.c_str());
  f_calib_mod->Cp(f_calib_save_name.c_str());

  gSystem->Exit(0);
}


void mergeCDBTTrees(const char * infile1, const char * infile2, const char * outputfile)
{

  CDBTTree *cdbttree1 = new CDBTTree(infile1);  
  CDBTTree *cdbttree2 = new CDBTTree(infile2);  
  CDBTTree *cdbttreeOut = new CDBTTree(outputfile);

  string m_fieldname = "Femc_datadriven_qm1_correction";

  for(int i = 0; i < 96 ; i++)
  {
    for(int j = 0; j < 256; j++)
    {
      unsigned int key = TowerInfoDefs::encode_emcal(i,j);
      float val1 = cdbttree1->GetFloatValue(key, m_fieldname);
      float val2 = cdbttree2->GetFloatValue(key, m_fieldname);
      cdbttreeOut->SetFloatValue(key,m_fieldname,val1*val2);
    }
  }

  cdbttreeOut->Commit();
  cdbttreeOut->WriteCDBTTree();
  delete cdbttreeOut;
  delete cdbttree1;
  delete cdbttree2;

}//end macro



void TSCtoCDBTTree(const char * infile, const char * outputfile)
{

  bool chk4file = gSystem->AccessPathName(infile);
  TFile *f = nullptr;

  if(!chk4file)
  {
    f = new TFile(infile,"READ");
  }
  else
  {
    std::cout << "File " << infile << " cant be found in current directory." << std::endl;
    exit(0);
  }

  //write to cdb tree
  CDBTTree *cdbttree = new CDBTTree(outputfile);

  //gain values lie in the 2d histogram called corrPat
  TH2F *cp = (TH2F *)f->Get("corrPat");

  for(int i = 0; i < 96 ; i++)
  {
    for(int j = 0; j < 256; j++)
    {
      unsigned int key = TowerInfoDefs::encode_emcal(i,j);
      float gain = (1.0 / cp->GetBinContent(i+1,j+1) );
       if (cp->GetBinContent(i+1,j+1) <= 0) gain = 0;
       if (isnan(cp->GetBinContent(i+1,j+1))) {gain = 0; cout << "nan calib from tsc " << i << "," << j << endl;}
      cdbttree->SetFloatValue(key,"Femc_datadriven_qm1_correction",gain);
    }
  }

  cdbttree->Commit();
  cdbttree->WriteCDBTTree();
  //cdbttree->Print();
  f->Close();
  delete f;
  delete cdbttree;

}
