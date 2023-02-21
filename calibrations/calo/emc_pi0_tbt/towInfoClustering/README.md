Example macros for running default clustering builder in mode which reads new towerinfo data structures rather than the old/original RawTower sim data structures.

NOTE: associated core software commit notes influencing content of these macros : pull request 2/21/23:  adds capability to use towerfinfo new tower data structs in default Template emcal cluster builder,  cluster position correction tool, and also in calibration pi0 TbT code [which then uses the new cluster version] although the cluster data structures didn't change, the links to the towers they hold in the version using towinfo are implied to be tower info objects rather than raw tower]. In all three cases, turn on with set_UseTowerInfo flag-- unlike some other of Tim's commits, this flag can only be used in modes 0 (don't use tower info) and 1 (use tower info) not in Tim's mode 2 (do both simultaneously). Corresponding example macros to use this can be found in macros/calibrations/calo/emc_pi0_tbt/towInfoClustering/


G4_Cemc_Spacal.C :   The usual, with extra ClusterBuilderTemplate and ClusterPosCorr which process the towinfo as described above producing additional nodes CLUSTERINFO_XXX in addition to old/orig CLUSTER_XXX nodes

spiNo2.C  :  Fun4all run macro that starts from mdc2 run 62 hits files (currently pi0 embed is default but there is also lines that can be uncommented for regular minbias run62. Assumes use of above G4_Cemc_Spacal.C macro [e.g. by placing in the same directory] Also runs both versions of Pi0 Calo TbT module.  

rundst_spiNo.C :  similar to previous spiNo2 except only outputs dst with both tower/toweinfo copies [all ~6 nodes, SIM/RAW/CALIB] but NOT any clustering nodes.  So clustering tests can be broken up into faster 2nd part testing only the clustering without needing all hits baggage.

srundst_mdc2_clustest.C:  runs on previous macro dst output, much simpler/faster libraries needed, contains RawClusBuild, etc. subsys recos and CAlib pi0 TbT modules as faster 2nd part, so testing only the clustering without needing all hits baggage



