#!/bin/bash

#root.exe Fun4All_G4_sPHENIX.C"("$n_events","$i","$z_width","$eta_min","$eta_max","$pt_min","$pt_max","$phi_min","$phi_max","\"/"$data"/"$parent"/"$directory"/G4_sPHENIX_"$config".root\"")"

#root.exe Fun4All_G4_sPHENIX.C"("$1","$2","$3","$4","$5","$6","$7","$8","$9","\"${10}/G4_sPHENIX_${11}.root\"")"
<<<<<<< HEAD
<<<<<<< HEAD
root.exe /direct/star+u/rcorliss/sphenix/macros/macros/g4simulations/Fun4All_G4_sPHENIX.C"("$1","$2","$3","$4","$5","$6","$7","$8","$9"."${10}","\"G4_sPHENIX_${12}.root\"")"
=======
root.exe /sphenix/u/mitay/Documents/latest/macros/macros/g4simulations/Fun4All_G4_sPHENIX.C"("$1","$2","$3","$4","$5","$6","$7","$8","$9","\"${10}\"","\"G4_sPHENIX_${12}.root\"")"
>>>>>>> cfc5f4ba4cc497237f1934ec09bfe83ae9ccf5c1
=======
root.exe /direct/star+u/rcorliss/sphenix/macros/macros/g4simulations/Fun4All_G4_sPHENIX.C"("$1","$2","$3","$4","$5","$6","$7","$8","$9"."${10}","\"G4_sPHENIX_${12}.root\"")"
>>>>>>> ab1fc6ee958cb827bd84d2f3de6276f38198c5cd

echo ${11}/G4_sPHENIX_${12}.root
mv G4_sPHENIX_${12}.root_g4svtx_eval.root ${11}/G4_sPHENIX_${12}.root_g4svtx_eval.root
mv PHG4TrackKalmanFitter_eval.root ${11}/G4_sPHENIX_${12}.root_g4kalman_eval.root
exit
