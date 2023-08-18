
root -q -b -x  'runLCELoop.C(1e7,"condor/alloutr21598.list","opi/rc_cpi1_21598.root")'
root -q -b -x  'runLCELoop.C(1e7,"condor/alloutr21615.list","opi/rc_cpi1_21615.root")'
root -q -b -x  'runLCELoop.C(1e7,"condor/alloutr21796.list","opi/rc_cpi1_21796.root")'
root -q -b -x  'runLCELoop.C(1e7,"condor/alloutr21813.list","opi/rc_cpi1_21813.root")'
root -q -b -x  'runLCELoop.C(1e7,"condor/alloutr21889.list","opi/rc_cpi1_21889.root")'
root -q -b -x  'runLCELoop.C(1e7,"condor/alloutr22949.list","opi/rc_cpi1_22949.root")'
root -q -b -x  'runLCELoop.C(1e7,"condor/alloutr22979.list","opi/rc_cpi1_22979.root")'
root -q -b -x  'runLCELoop.C(1e7,"condor/alloutr21518.list","opi/rc_cpi1_21518.root")'

root -q -b -x 'emc_pi0_tbt/save_correc_value.C("opi/rc_cpi1_21598.root","pifit/tsYfcpi1_fit1_21598.root")'
root -q -b -x 'emc_pi0_tbt/save_correc_value.C("opi/rc_cpi1_21615.root","pifit/tsYfcpi1_fit1_21615.root")'
root -q -b -x 'emc_pi0_tbt/save_correc_value.C("opi/rc_cpi1_21796.root","pifit/tsYfcpi1_fit1_21796.root")'
root -q -b -x 'emc_pi0_tbt/save_correc_value.C("opi/rc_cpi1_21813.root","pifit/tsYfcpi1_fit1_21813.root")'
root -q -b -x 'emc_pi0_tbt/save_correc_value.C("opi/rc_cpi1_21889.root","pifit/tsYfcpi1_fit1_21889.root")'
root -q -b -x 'emc_pi0_tbt/save_correc_value.C("opi/rc_cpi1_22949.root","pifit/tsYfcpi1_fit1_22949.root")'
root -q -b -x 'emc_pi0_tbt/save_correc_value.C("opi/rc_cpi1_22979.root","pifit/tsYfcpi1_fit1_22979.root")'
root -q -b -x 'emc_pi0_tbt/save_correc_value.C("opi/rc_cpi1_21518.root","pifit/tsYfcpi1_fit1_21518.root")'

