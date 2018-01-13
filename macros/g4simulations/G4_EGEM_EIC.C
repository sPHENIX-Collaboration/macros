// $Id: G4_FGEM_ePHENIX.C,v 1.4 2013/10/13 21:45:27 jinhuang Exp $

/*!
 * \file G4_FGEM_ePHENIX.C
 * \brief
 * \author Jin Huang <jhuang@bnl.gov>
 * \version $Revision: 1.4 $
 * \date $Date: 2013/10/13 21:45:27 $
 */

using namespace std;

void
EGEM_Init()
{

}

void
EGEMSetup(PHG4Reco* g4Reco)
{
  /* Careful with dimensions! If GEM station volumes overlap, e.g. with TPC volume, they will be
   * drawn in event display but will NOT register any hits.
   *
   * Geometric constraints:
   * TPC length = 211 cm --> from z = -105.5 to z = +105.5
   */
  make_GEM_station("EGEM_0", g4Reco, -32., -1.72, -3.4); // increase min eta from -1.6 to -1.72 to fit into barrel tracker geometry
  make_GEM_station("EGEM_1", g4Reco, -58., -2.1, -3.98); // reduce max eta from -4.0 to -3.98 to avoid overlap with volume S_AL_PIPE_5 (aluminum beam pipe)
  make_GEM_station("EGEM_2", g4Reco, -106., -1.2, -4.5); // move station from z = -101. to z= -106. because of increased TPC length

}

int
make_GEM_station(string name, PHG4Reco* g4Reco, double zpos, double etamin,
                 double etamax)
{

  //  cout
  //    << "make_GEM_station - GEM construction with PHG4SectorSubsystem - make_GEM_station_EdgeReadout  of "
  //    << name << endl;

  double polar_angle = 0;

  if (zpos < 0)
    {
      zpos = -zpos;
      polar_angle = TMath::Pi();

    }
  if (etamax < etamin)
    {
      double t = etamax;
      etamax = etamin;
      etamin = t;
    }

  PHG4SectorSubsystem *gem;
  gem = new PHG4SectorSubsystem(name.c_str());

  gem->get_geometry().set_normal_polar_angle(polar_angle);
  gem->get_geometry().set_normal_start(
                                       zpos * PHG4Sector::Sector_Geometry::Unit_cm());
  gem->get_geometry().set_min_polar_angle(
                                          PHG4Sector::Sector_Geometry::eta_to_polar_angle(etamax));
  gem->get_geometry().set_max_polar_angle(
                                          PHG4Sector::Sector_Geometry::eta_to_polar_angle(etamin));
  gem->get_geometry().set_material("G4_METHANE");
  gem->get_geometry().set_N_Sector(1);
  gem->OverlapCheck(overlapcheck);

  gem->get_geometry().AddLayers_DriftVol_COMPASS();
  gem->get_geometry().AddLayers_HBD_GEM();
  g4Reco->registerSubsystem(gem);

}

void EGEM_FastSim_Reco(int verbosity = 0) {

  //---------------
  // Load libraries
  //---------------

  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4hough.so");

  //---------------
  // Fun4All server
  //---------------

  Fun4AllServer *se = Fun4AllServer::instance();

  PHG4TrackFastSim* kalman = new PHG4TrackFastSim("PHG4TrackFastSim");
  kalman->Verbosity(0);

  kalman->set_use_vertex_in_fitting(true);
  kalman->set_vertex_xy_resolution(50E-4);
  kalman->set_vertex_z_resolution(50E-4);

  kalman->set_detector_type(PHG4TrackFastSim::Vertical_Plane); // Vertical_Plane, Cylinder
  kalman->set_phi_resolution(50E-4);
  kalman->set_r_resolution(1.);

  kalman->set_pat_rec_hit_finding_eff(1.);
  kalman->set_pat_rec_noise_prob(0.);

  std::string phg4hits_names[] = {"G4HIT_EGEM_0","G4HIT_EGEM_1","G4HIT_EGEM_2"};
  kalman->set_phg4hits_names(phg4hits_names, 3);
  kalman->set_sub_top_node_name("SVTX");
  kalman->set_trackmap_out_name("SvtxTrackMap_FastSimEtaMinus");

  // Saved track states (projections)
  std::string state_names[] = {"EEMC"};
  kalman->set_state_names(state_names, 1);

  kalman->set_fit_alg_name("KalmanFitterRefTrack");//
  kalman->set_primary_assumption_pid(-211); // -211 = pi-
  kalman->set_do_evt_display(false);

  se->registerSubsystem(kalman);

}

