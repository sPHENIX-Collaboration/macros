#include "CaloCalib_wvfm.h"

#include <calobase/RawCluster.h>
#include <calobase/RawClusterContainer.h>
#include <calobase/RawClusterUtility.h>
#include <calobase/RawTower.h>
#include <calobase/RawTowerContainer.h>
#include <calobase/RawTowerDefs.h>
#include <calobase/RawTowerGeomContainer.h>
#include <calobase/TowerInfoContainer.h>
#include <calobase/TowerInfo.h>

#include <globalvertex/GlobalVertex.h>
#include <globalvertex/GlobalVertexMap.h>

#include <fun4all/Fun4AllReturnCodes.h>
#include <fun4all/SubsysReco.h>

#include <phool/getClass.h>
#include <phool/phool.h>

#include <TF1.h>
#include <TFile.h>
#include <TGraphErrors.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TLorentzVector.h>
#include <TString.h>
#include <TStyle.h>
#include <TTree.h>
#include <TNtuple.h>
#include <TSystem.h>

#include <CLHEP/Vector/ThreeVector.h>  // for Hep3Vector

#include <algorithm>  // for max, max_element
#include <cmath>      // for abs
#include <cstdlib>
#include <iostream>
#include <map>      // for _Rb_tree_const_iterator
#include <utility>  // for pair
#include <vector>   // for vector
#include <string>   // for string

//using namespace std;

//____________________________________________________________________________..
CaloCalib_wvfm::CaloCalib_wvfm(const std::string &name, const std::string &filename)
  : SubsysReco(name)
  ,m_ievent(0)
  , m_Filename(filename)
  , cal_output(0)
  , _caloname("CEMC")
  ,f_temp(0)
{

  waveform_time = NULL;
	
	
	for (int nj = 0; nj < 96; nj++)
  {
    cemc_wvfm_eta[nj] = nullptr; // for eta-slice
		
    for (int nk = 0; nk < 258; nk++)
    {
      cemc_wvfm_eta_phi[nj][nk] = nullptr; // for all EMCal towers
    }
		
  }

}

//____________________________________________________________________________..
int CaloCalib_wvfm::InitRun(PHCompositeNode *topNode)
{
  std::cout << "CaloCalib_wvfm::Init(PHCompositeNode *topNode) Initializing" << std::endl;

  m_ievent = 0;

  cal_output = new TFile(m_Filename.c_str(), "RECREATE");

  waveform_time = new TH1F("waveform_time", "Waveform_Time", 33, -0.5, 32.5);


	// histo to record for all eta-slice
	// it's better to loop all 24K + towers and eta slice seperately, results in root file will be seperate which is good for visual inspection
  for (int i = 0; i < 96; i++)  // eta rows
  {
			// Histogram for all eta slice
      TString i_1;
      i_1.Form("%d", i);
      TString hist_name_1 = "wvfm_time_eta-bin_" + i_1;		
      cemc_wvfm_eta[i] = new TH1F(hist_name_1.Data(), "Hist_wvfm_eta-bin_", 33, -0.5, 32.5);
	}

	// histo to record every tower by tower locations
  for (int i = 0; i < 96; i++)  // eta rows
  {		
		// histogram for all EMCal towers
    for (int j = 0; j < 258; j++)  // phi columns
    {
      TString i1;
      TString j1;
      i1.Form("%d", i);
      j1.Form("%d", j);
      TString hist_name = "wvfm_time_eta-bin_" + i1 + "_phi-bin_" + j1;

      cemc_wvfm_eta_phi[i][j] = new TH1F(hist_name.Data(), "Hist_wvfm_eta-bin_phi-bin_", 33, -0.5, 32.5);
    }
  }


	if(!topNode)
	{
		std::cout << "topNode not found" << std::endl;
	}

  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int CaloCalib_wvfm::process_event(PHCompositeNode *topNode)
{ 
	//float conversion_factor = 0.002; // ADC to GeV conversion factor
  if (m_ievent % 1000 == 0)
  {
    std::cout << std::endl;
    std::cout << "Beginning of the event " << m_ievent << std::endl;
    std::cout << "====================================" << std::endl;
  }

  _eventNumber = m_ievent++;


  // create a tower object
	TowerInfoContainer *_towerinfos = NULL;
			
	// we will be using either of "TOWERINFO_CALIB_" or "TOWERS_" node as of now I am not sure about it (may be "*CALIB*" one has calibrated data)
	// we can simply use one of those (whichever available)
	
	//std::string towernode = "TOWERINFO_CALIB_" + _caloname;
	std::string towernode = "TOWERS_" + _caloname;
	_towerinfos = findNode::getClass<TowerInfoContainer>(topNode, towernode.c_str());
			
	if (!_towerinfos) 
	{
		std::cout << PHWHERE << " ERROR: Can't find " << towernode << std::endl;
		return Fun4AllReturnCodes::ABORTEVENT;
	}


  // getting size of node
  unsigned int nchannels = _towerinfos->size();

  //TowerInfo *tower_info;
  //RawTower *tower;
  //RawTowerGeom *tower_geom;
	

  for (unsigned int channel = 0; channel < nchannels; channel++)
		{				
			TowerInfo *towinfo = _towerinfos->get_tower_at_channel(channel); // get individual tower at particular channel
			unsigned int towerkey = _towerinfos->encode_key(channel); // get tower key using channel  num

			//std::cout << nchannels << " , " << channel << " , " << towerkey << std::endl;
		
			// extract eta-bin and phi-bin using towerkey 
			unsigned int ieta = _towerinfos->getTowerEtaBin(towerkey);
			unsigned int iphi = _towerinfos->getTowerPhiBin(towerkey);

			float tower_time = (towinfo->get_time());

			if (tower_time > 0)
			{
				waveform_time->Fill(tower_time);
				cemc_wvfm_eta[ieta]->Fill(tower_time);
				cemc_wvfm_eta_phi[ieta][iphi]->Fill(tower_time);
			}

			}
	 //std::cout << _eventNumber << std::endl;								

  return Fun4AllReturnCodes::EVENT_OK;
}


//____________________________________________________________________________..
int CaloCalib_wvfm::End(PHCompositeNode * topNode)
{
  if (topNode == 0 && f_temp)
    {
      cal_output->Close();
      f_temp->Close();
      delete f_temp;
      delete cal_output;
      return Fun4AllReturnCodes::EVENT_OK;
    }
  
  cal_output->cd();
  cal_output->Write();
  cal_output->Close();
  delete cal_output;

  return Fun4AllReturnCodes::EVENT_OK;
}

