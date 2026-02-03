#!/bin/tcsh

setenv HOME /phenix/u/$LOGNAME
source /etc/csh.login
foreach i(/etc/profile.d/*.csh)
    source $i
end

source $HOME/.login
#source /opt/sphenix/core/bin/sphenix_setup.sh 
#source /opt/sphenix/core/bin/setup_local.sh $MYINSTALLSPHE

set filename=$1
set sigma=$2
set runnumber=`echo $filename | awk '{print substr($0,38,5)}'`
if( ! -d output/$runnumber )then
    mkdir -p output/$runnumber 
endif
if( ! -d cdbMaps/$runnumber )then
    mkdir -p cdbMaps/$runnumber 
endif
 
root -l -b -q ../Fun4All_EMCalBadTowerID.C'(0,"'$filename'","output/'$runnumber'/DST_CALOR-'`echo $filename | awk '{print substr($0,35,13)}'`'_badTowerMapTree.root", "cdbMaps/'$runnumber'/DST_CALOR-'`echo $filename | awk '{print substr($0,38,5)}'`'_badTowerMapCDBTree'$2'Low_'$2'High.root", 15, 15,  '$2', '$2', 0.50,  0.50,  0.50, 0)'
