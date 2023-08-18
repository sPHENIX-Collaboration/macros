#include "CaloTowerCalib.h"

#include <fun4all/Fun4AllReturnCodes.h>
#include <fun4all/SubsysReco.h>  // for SubsysReco

#include <phool/PHCompositeNode.h>
#include <phool/PHIODataNode.h>    // for PHIODataNode
#include <phool/PHNodeIterator.h>  // for PHNodeIterator
#include <phool/PHObject.h>        // for PHObject
#include <phool/getClass.h>
#include <phool/phool.h>
#include <phool/recoConsts.h>


#include <TFile.h>
#include <TH2.h>
#include <TGraph.h>
#include <TF1.h>
#include <TNtuple.h>


#include <Event/Event.h>
#include <Event/EventTypes.h>
#include <Event/packet.h>
#include <phool/PHCompositeNode.h>

#include <calobase/TowerInfoContainerv1.h>
#include <calobase/TowerInfov1.h>

#include <ffamodules/CDBInterface.h>
#include <ffaobjects/EventHeader.h>

//____________________________________________________________________________..
CaloTowerCalib::CaloTowerCalib(const std::string &name)
  : SubsysReco(name)
  , m_dettype(CaloTowerCalib::HCALOUT)
  , m_detector("HCALOUT")
  , m_DETECTOR(TowerInfoContainer::HCAL)
  , m_fieldname("")
  , m_runNumber(-1)
{
  std::cout << "CaloTowerCalib::CaloTowerCalib(const std::string &name) Calling ctor" << std::endl;
}

//____________________________________________________________________________..
CaloTowerCalib::~CaloTowerCalib()
{
  std::cout << "CaloTowerCalib::~CaloTowerCalib() Calling dtor" << std::endl;
}

//____________________________________________________________________________..
int CaloTowerCalib::InitRun(PHCompositeNode *topNode)
{
  PHNodeIterator nodeIter(topNode);

  EventHeader *evtHeader = findNode::getClass<EventHeader>(topNode, "EventHeader");

  if (evtHeader)
  {
    m_runNumber = evtHeader->get_RunNumber();
  }
  else
  {
    m_runNumber = -1;
  }
  std::cout << "at run" << m_runNumber << std::endl;
  if (m_dettype == CaloTowerCalib::CEMC)
  {
    m_detector = "CEMC";
    m_DETECTOR = TowerInfoContainer::EMCAL;

    cdb = CDBInterface::instance();
    if (!m_overrideCalibName) m_calibName = "cemc_relcalib_cosmic";
    if (!m_overrideFieldName) m_fieldname = "cemc_relcalib_cosmic";
    std::string calibdir = cdb->getUrl(m_calibName);
    if (calibdir[0] == '/')
    {
      cdbttree = new CDBTTree(calibdir.c_str());
    }
    else
    {
      std::cout << "CaloTowerCalib::::InitRun No calibration file found" << std::endl;
      exit(1);
    }
  }
  else if (m_dettype == CaloTowerCalib::HCALIN)
  {
    m_detector = "HCALIN";
    m_DETECTOR = TowerInfoContainer::HCAL;

    if (!m_overrideCalibName) m_calibName = "ihcal_abscalib_mip_bldg912";
    if (!m_overrideFieldName) m_fieldname = "ihcal_abscalib_mip";
    cdb = CDBInterface::instance();
    std::string calibdir = cdb->getUrl(m_calibName);
    if (calibdir[0] == '/')
    {
      cdbttree = new CDBTTree(calibdir.c_str());
    }
    else
    {
      std::cout << "CaloTowerCalib::::InitRun No calibration file found" << std::endl;
      exit(1);
    }
  }
  else if (m_dettype == CaloTowerCalib::HCALOUT)
  {
    m_detector = "HCALOUT";
    m_DETECTOR = TowerInfoContainer::HCAL;

    if (!m_overrideCalibName) m_calibName = "ohcal_abscalib_mip_bldg912";
    if (!m_overrideFieldName) m_fieldname = "ohcal_abscalib_mip";
    cdb = CDBInterface::instance();
    std::string calibdir = cdb->getUrl(m_calibName);
    if (calibdir[0] == '/')
    {
      cdbttree = new CDBTTree(calibdir.c_str());
    }
    else
    {
      std::cout << "CaloTowerCalib::::InitRun No calibration file found" << std::endl;
      exit(1);
    }
  }
  else if (m_dettype == CaloTowerCalib::EPD)
  {
    m_detector = "EPD";
    m_DETECTOR = TowerInfoContainer::SEPD;
    if (!m_overrideCalibName) m_calibName = "noCalibYet";
    if (!m_overrideFieldName) m_fieldname = "noCalibYet";
    cdb = CDBInterface::instance();
    std::string calibdir = cdb->getUrl(m_calibName);
    if (calibdir[0] == '/')
    {
      cdbttree = new CDBTTree(calibdir.c_str());
    }
    else
    {
      std::cout << "CaloTowerCalib::::InitRun No calibration file found" << std::endl;
      exit(1);
    }
  }

  PHNodeIterator iter(topNode);

  // Looking for the DST node
  PHCompositeNode *dstNode;
  dstNode = dynamic_cast<PHCompositeNode *>(iter.findFirst("PHCompositeNode",
                                                           "DST"));
  if (!dstNode)
  {
    std::cout << Name() << "::" << m_detector << "::" << __PRETTY_FUNCTION__
              << "DST Node missing, doing nothing." << std::endl;
    exit(1);
  }
  try
  {
    CreateNodeTree(topNode);
  }
  catch (std::exception &e)
  {
    std::cout << e.what() << std::endl;
    return Fun4AllReturnCodes::ABORTRUN;
  }
  topNode->print();
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int CaloTowerCalib::process_event(PHCompositeNode * /*topNode*/)
{


  static TH2F * tcorrpat_r21  = 0;
  //  static TH2F * tcorrpatch_r21  = 0;
  static TFile * tmf_r21= 0; 

  if (!tcorrpat_r21)
    {
       TString tfnm = "timing/sj/timecutOhioU_";
      tfnm += m_runNumber;
      tfnm += ".root";

     std::cout << "test TIMING  first time in calotowcalib: using file: " 
	       << tfnm.Data() << std::endl;

      tmf_r21 = new TFile(tfnm.Data());
      tcorrpat_r21 = (TH2F * ) tmf_r21->Get("t0corrPati");
      if (!tcorrpat_r21) 
	{
	  std::cout << "problem_r21 timing" << std::endl;
	  exit(1);
	}

    }


  static TH2F * corrpat_r21  = 0;
  static TH2F * corrpatch_r21  = 0;
  static TFile * mf_r21= 0; 

  if (!corrpat_r21)
    {

      std::cout << "test  first time in calotowcalib " << std::endl;
      if (m_runNumber < 21813)
	mf_r21 = new TFile("tsfitting/wst13ref_21598.root");
      else
	mf_r21 = new TFile("tsfitting/vst13ref_22949.root");


      corrpat_r21 = (TH2F * ) mf_r21->Get("corrPat");
      if (!corrpat_r21) 
	{
	  std::cout << "problem_r21 " << std::endl;
	  exit(1);
	}

      corrpatch_r21 = (TH2F * ) mf_r21->Get("chi2corrPati");
      if (!corrpatch_r21) 
	{
	  std::cout << "problem_r21 chi " << std::endl;
	  exit(1);
	}
    }


  float maxchi2 = 2.1;

  if (m_runNumber > 21813)
    maxchi2 = 8.0;




  static TH2F * corrpatch_fr21  = 0;
  static TFile * mf2fr21= 0; 
  
  if (!corrpatch_fr21)
    {
      std::cout << "test  first time in calotowcalib corpatCH2" << std::endl;
      if (m_runNumber > 21813)
	mf2fr21 = new TFile("bad_22949.root");
      else
	mf2fr21 = new TFile("bad_21598.root");
      corrpatch_fr21 = (TH2F * ) mf2fr21->Get("corrPatChk");
      if (!corrpatch_fr21) 
	{
	  std::cout << "problem patch " << std::endl;
	  exit(1);
	}
    }



  /*

  static TGraph * eta_corrsim  = 0;
  static TFile * mf3= 0; 

  if (!eta_corrsim)
    {
      std::cout << "test  first time in calotowcalib etacorr_sim" << std::endl;
      mf3 = new TFile("tsst_alleta.root");
      eta_corrsim = (TGraph * ) mf3->Get("Fit1_etaout");
      if (!eta_corrsim) 
	{
	  std::cout << "problem eta_corrsim " << std::endl;
	  exit(1);
	}
    }
  */

  /*
  static TGraph * eta_corrdat  = 0;
  static TFile * mf4= 0; 

  if (!eta_corrdat)
    {
      std::cout << "test  first time in calotowcalib etacorr_dat" << std::endl;
      mf4 = new TFile("cop_towslopemacs/st1_xsout_across_21813.root");
      eta_corrdat = (TGraph * ) mf4->Get("Fit1_etaout");
      if (!eta_corrdat) 
	{
	  std::cout << "problem eta_corrdat " << std::endl;
	  exit(1);
	}
    }

  */

  static TFile * mf5= 0; 
  static float myaggcorr[96][260];

  if (!mf5)
    {

      TString incfile = "pifit/VfIt3_22949.root";
      if (m_runNumber < 21813)
	incfile = "pifit/WfIt2_st1_21598.root";
      
      //char incorrFile[] = "pifit/ivrabletion4_fourpi21813_pifit.root";
      const char * incorrFile = incfile.Data();
  

  for (int cci = 0; cci < 96; cci++)
  {
		for (int ccj = 0; ccj < 260; ccj++)
		{
			myaggcorr[cci][ccj] = 1.00000;
		}
  }

  std::cout << "running w/ corr file? : " << incorrFile << std::endl;  

  std::string inF = incorrFile;
  if (!(inF == ""))
  {

                mf5 = new TFile(incorrFile);
		TFile * infileNt = mf5;
		std::cout << "loaded incorrFile " << infileNt << std::endl;

		float myieta;
		float myiphi;
		float mycorr;
		float myaggcv;
		
		TNtuple * innt_corrVals = (TNtuple *) infileNt->Get("nt_corrVals");
		
		innt_corrVals->SetBranchAddress("tower_eta",&myieta);
		innt_corrVals->SetBranchAddress("tower_phi",&myiphi);
		innt_corrVals->SetBranchAddress("corr_val",&mycorr);
		innt_corrVals->SetBranchAddress("agg_cv",&myaggcv);

		int ntCorrs = innt_corrVals->GetEntries();

		for (int ij = 0; ij < ntCorrs; ij++)
		{
			innt_corrVals->GetEntry(ij);
			int ci = (int) myieta;
			int cj = (int) myiphi;
			if (ci < 9 || ci > 94)
			  continue;
			//			myaggcorr[ci][cj] = 0.170/0.155*myaggcv;
			myaggcorr[ci][cj] = myaggcv;

			if (ij > ntCorrs -2  || ij == ntCorrs/2)
				std::cout << "loaded corrs eta,phi,aggcv " << myieta 
				<< " " << myiphi << " " << myaggcv << std::endl; 
	
		}

		//		infileNt->Close();
		//delete infileNt;
		
  }
    }

  unsigned int ntowers = _raw_towers->size();
  for (unsigned int channel = 0; channel < ntowers; channel++)
  {
    unsigned int key = _raw_towers->encode_key(channel);
    TowerInfo *caloinfo_raw = _raw_towers->get_tower_at_channel(channel);
    float raw_amplitude = caloinfo_raw->get_energy();
   float raw_time  =  caloinfo_raw->get_time();
    float calibconst = cdbttree->GetFloatValue(key, m_fieldname);

    unsigned int towerkey = _raw_towers->encode_key(channel); 
    
    int ieta = _raw_towers->getTowerEtaBin(towerkey);
    int iphi = _raw_towers->getTowerPhiBin(towerkey);
    
    float timecutctr = tcorrpat_r21->GetBinContent(ieta+1,iphi+1);
    
      //    if (calibconst2 > 0.3 && calibconst2 < 3.0 && raw_time > 7.8 && raw_time < 9.1)
    
    float calibconst2 = corrpat_r21->GetBinContent(ieta+1,iphi+1);
    //    calibconst2 = 1.0;
    //    if (timecutctr > 0.5 && fabs(raw_time - timecutctr) < 0.6 && calibconst2 > 0.3 && calibconst2 < 3.0 )
    //     calibconst *= 1.0/calibconst2;

    if (timecutctr > 0.5 && fabs(raw_time - timecutctr) < 0.6)
      calibconst *= 1.0/calibconst2;
    else
      calibconst = 0.0;

    
    if (corrpatch_r21->GetBinContent(ieta+1,iphi+1) > maxchi2)
      calibconst = 0.0;



    if (corrpatch_fr21->GetBinContent(ieta+1,iphi+1) > 0) 
      calibconst = 0.0;

    /*
    if (ieta > 10 && ieta < 95)
      {
	float plata = eta_corrdat->GetY()[ieta];
	calibconst *= plata;
      }

    */

    float aggcv = myaggcorr[ieta][iphi];
			
    calibconst *= aggcv;

    

    /*
      

    //      t = 1.0*tower_info->get_time();
    // if (t < 7.8 || t > 9.1) 
      
    if (calibconst2 > 0.2 && calibconst2 < 3.0 && raw_time > 5.8 && raw_time < 7.1)
    //    if (calibconst2 > 0.3 && calibconst2 < 3.0 && raw_time > 7.8 && raw_time < 9.1)
      calibconst *= 1.0/calibconst2;
    else
      calibconst = 0.0;

    if (corrpatch_r21->GetBinContent(ieta+1,iphi+1) > 4.0)
      calibconst = 0.0;

    if (ieta > 10 && ieta < 95)
      {
	float plata = eta_corrdat->GetY()[ieta];
	calibconst *= plata;
      }



    float aggcv = myaggcorr[ieta][iphi];
			
    calibconst *= aggcv;

    */

    // if (!(raw_time > 5.8 && raw_time < 7.1))
    //   calibconst = 0.0;

    _calib_towers->get_tower_at_channel(channel)->set_energy(raw_amplitude * calibconst);
  }

  return Fun4AllReturnCodes::EVENT_OK;
}

void CaloTowerCalib::CreateNodeTree(PHCompositeNode *topNode)
{
  std::cout << "creating node" << std::endl;
  PHNodeIterator iter(topNode);
  PHCompositeNode *dstNode = dynamic_cast<PHCompositeNode *>(iter.findFirst(
      "PHCompositeNode", "DST"));
  if (!dstNode)
  {
    std::cerr << Name() << "::" << m_detector << "::" << __PRETTY_FUNCTION__
              << "DST Node missing, doing nothing." << std::endl;
    throw std::runtime_error(
        "Failed to find DST node in RawTowerCalibration::CreateNodes");
  }

  // towers
  std::string RawTowerNodeName = "TOWERS_" + m_detector;
  _raw_towers = findNode::getClass<TowerInfoContainerv1>(dstNode,
                                                         RawTowerNodeName);
  if (!_raw_towers)
  {
    std::cerr << Name() << "::" << m_detector << "::" << __PRETTY_FUNCTION__
              << " " << RawTowerNodeName << " Node missing, doing bail out!"
              << std::endl;
    throw std::runtime_error(
        "Failed to find " + RawTowerNodeName + " node in RawTowerCalibration::CreateNodes");
  }

  std::string CalibTowerNodeName = "TOWERS_Calib_" + m_detector;
  _calib_towers = findNode::getClass<TowerInfoContainerv1>(dstNode,
                                                           CalibTowerNodeName);
  if (!_calib_towers)
  {
    _calib_towers = new TowerInfoContainerv1(m_DETECTOR);

    PHIODataNode<PHObject> *calibtowerNode = new PHIODataNode<PHObject>(_calib_towers, CalibTowerNodeName, "PHObject");
    std::cout << "adding calib tower" << std::endl;
    dstNode->addNode(calibtowerNode);
  }

  return;
}
