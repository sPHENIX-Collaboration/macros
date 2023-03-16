#include <cdbobjects/CDBHistos.h>

R__LOAD_LIBRARY(libcdbobjects.so)

void TestCDBHistos(const std::string &fname = "test.root")
{
  CDBHistos *cdbhistos = new CDBHistos(fname);
  TH1 *h1 = new TH1F("h1","best 1d ever",100,0,1);
  cdbhistos->registerHisto(h1);
  TH2 *h2 = new TH2F("h2","best 2d ever",100,0,1,100,1,2);
  cdbhistos->registerHisto(h2);
  TH3 *h3 = new TH3F("h3","best 3d ever",100,0,1,100,1,2,100,2,3);
  cdbhistos->registerHisto(h3);
  TProfile *tp = new TProfile("tp","best tprofile ever",100,0,1,-10,10);
  cdbhistos->registerHisto(tp);
  for (int i = 0; i<100; i++)
  {
    h1->Fill(i/100.);
    h2->Fill(i/100.,1+i/100.);
    h3->Fill(i/100.,1+i/100.,2+i/100.,1.);
    tp->Fill(i/100.,1+i/100.);
  }
  cdbhistos->WriteCDBHistos();
  delete cdbhistos;
//  gSystem->Exit(0);
}

CDBHistos *TestWrite(const std::string &fname = "test.root")
{
  CDBHistos *cdbhistos = new CDBHistos(fname);
  return cdbhistos;
}

CDBHistos *Read(const std::string &fname = "test.root")
{
CDBHistos *cdbhistos = new CDBHistos(fname);
cdbhistos->LoadCalibrations();
  cdbhistos->Print();
  cdbhistos->getHisto("h1")->Draw();
  return cdbhistos;
//  delete cdbhistos;
//  gSystem->Exit(0);
}
