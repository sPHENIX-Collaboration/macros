#!/usr/bin/bash

filename=`basename ${1}`   # must be a local file
destination=${2}
rename=${3:-${filename}}

if [[ "$USER" == "chiu" ]]
then
  echo $0 $filename $destination $rename
  exit
fi

mv ${filename} ${rename}
filename=${rename}

echo stageout ${filename} ${destination} start `date`

regex_dsttype_run="(DST_[A-Z_]+[a-z0-9]+)_([a-z0-9]+)_(202[345]p[0-9][0-9][0-9])-([0-9]+)-([0-9]+)"
regex_dsttype_range="(DST_[A-Z_]+[a-z0-9]+)_([a-z0-9]+)_(202[345]p[0-9][0-9][0-9])-([0-9]+)-([0-9]+)-([0-9]+)"

# decode filename
base=${filename/.root/}
dstname=${base%%-*}

# Filename matches a dsttype with a single run
if [[ $base =~ $regex_dsttype_run ]]; then
   dsttype=${BASH_REMATCH[1]}
 echo $dsttype ...
   build=${BASH_REMATCH[2]}
 echo $build
   dbtag=${BASH_REMATCH[3]}
 echo $dbtag
   runnumber=${BASH_REMATCH[4]}
 echo $runnumber
   segment=${BASH_REMATCH[5]}
 echo $segment
fi
# Filename matches a dst "run range" type
if [[ $base =~ $regex_dsttype_range ]]; then
   dsttype=${BASH_REMATCH[1]}
 echo $dsttype ...
   build=${BASH_REMATCH[2]}
 echo $build
   dbtag=${BASH_REMATCH[3]}
 echo $dbtag
   runnumber=${BASH_REMATCH[4]}
 echo $runnumber
   runnumber2=${BASH_REMATCH[5]}
 echo $runnumber
   segment=${BASH_REMATCH[6]}
 echo $segment
fi

nevents_=$( root.exe -q -b GetEntries.C\(\"${filename}\"\) | awk '/Number of Entries/{ print $4; }' )
nevents=${nevents_:--1}

# prodtype is required... specifies whether the production status entry manages a single output file (only) or many output files (many).
echo ./cups.py -r ${runnumber} -s ${segment} -d ${dstname}  stageout ${filename} ${destination} --dsttype ${dsttype} --dataset ${build}_${dbtag} --nevents ${nevents} --inc --prodtype many
     ./cups.py -r ${runnumber} -s ${segment} -d ${dstname}  stageout ${filename} ${destination} --dsttype ${dsttype} --dataset ${build}_${dbtag} --nevents ${nevents} --inc --prodtype many


echo stageout ${filename} ${destination} finish `date`




