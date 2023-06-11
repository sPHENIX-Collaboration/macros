Note this folder is simpler subset of previously committed macros in macros/calibrations/calo/tower_slope_macro, 
some of those are out of date due mostly to the removal the DBAccessor class/package in favor of just standard
Conditions Data Base (CDB) data structures.   Also it is very RawTower centric. 

This folder, and the tower_slope_* macros generally are macros, etc. to run the calibration tool located in 
coresoftware/calibrations/calorimeter/calo_tower_slope/  (the LiteCaloEval.h class)

**Status of coresoftware/calibrations/calorimeter/calo_tower_slope**

Current status: contains MDC2 updated relative fitting method using data-defined histogram shape. Currently 
designed to run in two different modes 1) "gain tracing" mode where calibration shift in one time slice file 
is detected from tower energy distributions, quantified, and correction file is generated for, by fitting its
shape with same energy distribution for reference time slice. 2) “Eta slice flattening mode” where a gain shift
in a single tower is detected by defining a phi-integrated histogram (that has the same eta coordinate as the 
tower of interest) that serves as the function that will be fit to a single tower’s energy distribution. The
gain shift in a single tower is defined as  1 / p1, where p1 is a fit parameter extracted from the fit on the 
tower. A correction file is also generated storing a 1/p1 for each tower.  Currently out of date in this corr
file format, will be soon converted to CBDTree objects for both emcal and hcal.

**Running/Macros**

dst_simple_towerslope_runmacro.C:   Macro/script for running over real or sim DST files already containing a 
TOWERINFO (or RawTower) container. Outputs tower slope histograms (energy distributions for every tower and some 
integrated regions, such as eta bin slices/rings integrated over phi.)

**Macros for doing the actual fitting step**

**For gain trace mode:** do_eta_fit2_<caloname>.C
For flattening ("eta slice/rings") mode: rings_do_eta_fit2_<caloname>.C
<caloname> is either CEMC, HCALIN, or HCALOUT
Both macros have the same exact steps involved, the rings version just uses a different fitting method. 
The fitting step (timeslice1 --> timeslice 2 --see above) should generally be after merging of tower energy 
    distribution histogram files (the output of the tower slope module over the mdc2/production data files). 
    The last function called in the macro LiteCaloEval::Fit_RelativeShift outputs the correction which can be 
    read as input in place of the "Example Simple Database" files above. For CEMC, this is output into the root
    file output containing all the other root output (various graphs, etc.) into a tree within that file with
    the correct name ("emc_corr_tree"). For the Hcals, for which the correction files should be simple text file 
    formats, the correction text file is outputted to: [output_root_filename].HCAL[IN|OUT]_CORR_TXTFILE.txt as an 
    additional output file besides the usual output_root_file.

**To run** \: Each calorimeter has its own macro (see macro folder – /calibrations/calo/tower_slope_macro). More so, each mode as mentioned above has its own macro with the eta slice flattening mode having a prefix of ‘rings’ added to the name (rings_do_eta_fit2_<caloname>.C) See further below under ‘Mode Types’ for more details.
The histogram naming is separated by calotype. The most important ones are individual towers, and the phi-integrated (a.k.a eta slice / eta ring) ones. 
    
The histogram names are:
•	EMCal
o	Tower – emc_ietaX_phiY, where X is eta bin and Y is the phi bin
o	Eta slice – eta_X
•	I/OHCal
o	Tower – hcal_<in/out>_eta_X_phi_Y
o	Eta Slice – hcal<in/out>_eta_X

**Mode Types**
	
  _The gain trace mode_ macro (do_eta_fit2_<caloname>.C ) takes 3 parameters respectively:
•	Reffile – this parameter should be a root file that contains all merged calo histograms (see Init Run) from a specific run in sims, or whatever time period with real data
•	Infile – this parameter is the exact same file as refile but with modified tower energies, thus modified tower energy distributions
•	Modfile – the output file you want to write to
 The very last function used in the macro (FitRelativeShifts) called takes on two parameters. The first is the object, reflce, is the object made in the macro and the second is a parameter that sets several flags. The second variable is an integer and typical value is 10. In the 10’s place (the 1) is for smoothing of histograms [ hist->Smooth( ) ], and the digit in the 1’s place (the 0) is to run over all towers – if it were set to 1, it would only run eta slices. So, typical value for this parameter are 0, 10, or 20. 

  _Flattening mode_ ( rings_do_eta_fit2_<calotype>.C ) takes only 2 parameters:
•	Infile – the same parameter as above. 
•	Modfile – same parameter as above 

The FitRelativeShifts function call in this macro takes on the same parameters, but the second (the integer mentioned above) takes on 3 numbers. The usual incantation is 110, again where each digit controls a flag (see LiteCaloEval.cc, L940). The hundreds place controls whether or not the flattening mode runs, and the tens and ones place are the same flag as above.

**Exclusion of Towers in flattening mode**

In the flattening mode, when a tower is being fit, its histogram is removed from the eta slice histogram. For example, in IHCal, one eta slice histogram contains only 63 towers, not 64. Same concept applies to OHCal and EMCal. 

The towers that correspond to the chimney sector of OHCal are skipped over, or not fit at all, and also their respective towers are removed from the eta slice histogram. As of now (6/6/23), the towers corresponding from the TPC support rods are also skipped and their towers removed from the eta slice histogram. 

This code will be generalized soon to use a generic bad tower definition to exclude for the eta slices/rings.
  
**Fit Ranges of Spectra**
  
For fit  Min/Max fit ranges, see any of the "Macros for doing the fitting step listed above", pretty much most macros in this folder, for examples of setting these.  Fit ranges were determined by trial and error, close to the values used for some latest performance plots as of start of run23 can be found in the macros.



