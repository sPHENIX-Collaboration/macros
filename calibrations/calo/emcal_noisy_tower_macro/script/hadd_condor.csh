#!/bin/tcsh

setenv HOME /phenix/u/$LOGNAME
source /etc/csh.login
foreach i(/etc/profile.d/*.csh)
    source $i
end

source $HOME/.login
    
set runRange=$1

set range=$2

set runnumber=$3

set histoDir=output/${runnumber}

echo $histoDir

cd $histoDir

@ first=$range * $runRange

@ runRange+=1

@ last=$runRange * $range - 1

set allFiles=""

echo In $PWD

set n=$first

while ($n <= $last)
set m=`printf '%04d' "$n"`
set oneFile="`ls -1 DST_CALOR-000"$runnumber"-"$m"_badTowerMapTree.root`"
set allFiles="$allFiles $oneFile "
#echo $oneFile
@ n+=1
#@ allFiles+=$oneFile
end

#echo $allFiles
hadd -f -k DST_CALOR-000${runnumber}-RANGE$runRange.root $allFiles
