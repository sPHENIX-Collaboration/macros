// Authors: Ross Corliss & Molly Taylor
// Date: 10/23/18
// Goal: evaluate dependence of Kalman extrapolation on pt, pz.

// layout 0 = four layers, laddertypes 0-1-1-1
// layout 1 = four layers, laddertypes 1-1-0-1
// layout 2 = three outer layers, laddertypes 1-0-1
// layout 3 = three outer layers, laddertypes 1-1-1
// layout 4 = two outer layers, laddertypes 0-1
// layout 5 = two outer layers, laddertypes 1-1
// layout 6 = one (two) outer layers, laddertype 1
// layout 7 = no outer layers

#include <string>
#include "TFile.h"

// define constants common to all sets:
const int n_layouts=9;					// number of INTT layouts we are testing
const int n_intt_layers[n_layouts]={4,4,3,3,2,2,1,0,1};	// number of INTT layers in each layout
const string basepath="/gpfs/mnt/gpfs04/sphenix/user/mitay/data/";	// base path to where data is located
const string layout[n_layouts]={"zppp","ppzp","0pzp","0ppp","00zp","00pp","000p","0000","00pp_out"};	// convert layout# to layout configuration

// momentum scan sets
const string studypath="mom_scan";	//doubles as a naming convention?

const int n_datasets=8;

/*
const string subpath[n_datasets]={"pi+_pt0.5GeV_phi-180-180d_z0cm_eta-1.2-1.2",
				  "pi+_pt0.6GeV_phi-180-180d_z0cm_eta-1.2-1.2",
				  "pi+_pt0.7GeV_phi-180-180d_z0cm_eta-1.2-1.2",
				  "pi+_pt0.8GeV_phi-180-180d_z0cm_eta-1.2-1.2",
				  "pi+_pt0.9GeV_phi-180-180d_z0cm_eta-1.2-1.2",
				  "pi+_pt1.0GeV_phi-180-180d_z0cm_eta-1.2-1.2",
				  "pi+_pt2.0GeV_phi-180-180d_z0cm_eta-1.2-1.2",
				  "pi+_pt3.0GeV_phi-180-180d_z0cm_eta-1.2-1.2"};
*/

/*
const string subpath[n_datasets]={"mu-_pt0.5GeV_phi-180-180d_z0cm_eta-1.2-1.2",
				  "mu-_pt0.6GeV_phi-180-180d_z0cm_eta-1.2-1.2",
				  "mu-_pt0.7GeV_phi-180-180d_z0cm_eta-1.2-1.2",
				  "mu-_pt0.8GeV_phi-180-180d_z0cm_eta-1.2-1.2",
				  "mu-_pt0.9GeV_phi-180-180d_z0cm_eta-1.2-1.2",
				  "mu-_pt1.0GeV_phi-180-180d_z0cm_eta-1.2-1.2",
				  "mu-_pt2.0GeV_phi-180-180d_z0cm_eta-1.2-1.2",
				  "mu-_pt3.0GeV_phi-180-180d_z0cm_eta-1.2-1.2"};
*/

/*
const string subpath[n_datasets]={"e-_pt0.5GeV_phi-180-180d_z0cm_eta-1.2-1.2",
				  "e-_pt0.6GeV_phi-180-180d_z0cm_eta-1.2-1.2",
				  "e-_pt0.7GeV_phi-180-180d_z0cm_eta-1.2-1.2",
				  "e-_pt0.8GeV_phi-180-180d_z0cm_eta-1.2-1.2",
				  "e-_pt0.9GeV_phi-180-180d_z0cm_eta-1.2-1.2",
				  "e-_pt1.0GeV_phi-180-180d_z0cm_eta-1.2-1.2",
				  "e-_pt2.0GeV_phi-180-180d_z0cm_eta-1.2-1.2",
				  "e-_pt3.0GeV_phi-180-180d_z0cm_eta-1.2-1.2"};
*/


const string subpath[n_datasets]={"pi-_pt0.5GeV_phi-180-180d_z0cm_eta-1.2-1.2",
				  "pi-_pt0.6GeV_phi-180-180d_z0cm_eta-1.2-1.2",
				  "pi-_pt0.7GeV_phi-180-180d_z0cm_eta-1.2-1.2",
				  "pi-_pt0.8GeV_phi-180-180d_z0cm_eta-1.2-1.2",
				  "pi-_pt0.9GeV_phi-180-180d_z0cm_eta-1.2-1.2",
				  "pi-_pt1.0GeV_phi-180-180d_z0cm_eta-1.2-1.2",
				  "pi-_pt2.0GeV_phi-180-180d_z0cm_eta-1.2-1.2",
				  "pi-_pt3.0GeV_phi-180-180d_z0cm_eta-1.2-1.2"};


const string setname[n_datasets]={"0.5GeV","0.6GeV","0.7GeV","0.8GeV","0.9GeV","1.0GeV","2.0GeV","3.0GeV"};

// define variables that will make our life easier to be able to access directly
TFile *fin[n_datasets][n_layouts][2];

void find_zombies() {
	gSystem->Load("libg4hough.so");

	//  files, branches and their associated 'isZombie' (this could be called each time by fin[z][q]->isZombie() as well)
  	const string path=basepath+studypath+"/";

	// open all the files we need;  first index is subpath, second index is layout
	for (int i = 0; i < n_datasets; i++) {
		for (int j = 0; j < n_layouts; j++) {
			fin[i][j][0] = new TFile((path+subpath[i]+"/G4_sPHENIX_"+layout[j]+".root_g4kalman_eval.root").c_str(),"READ");

	    		if (fin[i][j][0]->IsZombie();) {
				cout << ("Zombie " + string(subpath[i]) + "G4_sPHENIX_" + string(layout[j]) + ".root_g4kalman_eval.root").c_str() << endl;
			}

			fin[i][j][1] = new TFile((path+subpath[i]+"/G4_sPHENIX_"+layout[j]+".root_g4svtx_eval.root").c_str(),"READ");

	    		if (fin[i][j][1]->IsZombie();) {
				cout << ("Zombie " + string(subpath[i]) + "/G4_sPHENIX_" + string(layout[j]) + ".root_g4svtx_eval.root").c_str() << endl;
			}
	  	}
	}

	gSystem->Exit(0);
	return;
}
