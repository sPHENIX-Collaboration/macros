#!/usr/bin/env bash


if [[ $USER == "sphnxpro" ]]
then
  source /opt/sphenix/core/bin/sphenix_setup.sh -n new
  TOPDIR=/sphenix/u/sphnxpro/chiu/new_2025/CALIBPRODUCTION/
  # work area for this sub-production
  tobedonedir=${TOPDIR}/SUBMITTED.$(date +"%Y%m%d%H%M")
  # directory containing the calib macros and scripts
  SUBMITDIR=${tobedonedir}
elif [[ $USER == "chiu" ]]
then
  TOPDIR=/sphenix/user/chiu/sphenix_bbc/run2025/CALIBPRODUCTION/lists
  tobedonedir=${TOPDIR}/SUBMITTED.$(date +"%Y%m%d%H%M")
  SUBMITDIR=/sphenix/user/chiu/sphenix_bbc/run2025/CALIBPRODUCTION/
fi

mkdir ${tobedonedir}
cd ${tobedonedir}

# Get runs
#tmp_runsfile=/tmp/runs.$$
tmp_runsfile=${TOPDIR}/allrunsraw.txt
min_runnumber=66457
max_runnumber=99999
psql -U phnxrc -h sphnxdaqdbreplica.sdcc.bnl.gov -c "select runnumber,eventsinrun,ertimestamp-brtimestamp from run where runnumber>= $min_runnumber and runnumber <= $max_runnumber and runtype='physics' and eventsinrun >= 100000 and EXTRACT(EPOCH FROM (ertimestamp-brtimestamp)) > 300 order by runnumber" daq | tail -n +3 | grep '|' | sort -n -k1 > $tmp_runsfile

awk '{print $1}' $tmp_runsfile > ${TOPDIR}/all.runs

tobedonefile=tobedone.$(date +"%Y%m%d%H%M")
fgrep -v -f ${TOPDIR}/submitted.runs ../all.runs | join - $tmp_runsfile | awk '{print $1}' > $tobedonefile

# Get Current TimeStamp
tstamp_file=${TOPDIR}/.tstamp.new
psql -c 'select NOW();' FileCatalog | awk 'NR==3' | sed 's/^ //' > ${tstamp_file}

# Get list of new files
tmpfile=/tmp/get_files2calib.$$
all_files=${TOPDIR}/ALL_FILES/all_files.$(date +"%Y%m%d%H%M")

pcmd="SELECT lfn FROM files WHERE time >= '$(tail -1 ${TOPDIR}/tstamp.history)';"
echo psql -c \"$pcmd\" FileCatalog
psql -c "$pcmd" FileCatalog > $tmpfile
grep DST_TRIGGERED_EVENT_seb18_run3auau $tmpfile | sed 's/^ //' | sort > ${all_files}

# If $all_files is not empty, then the psql flist worked
if [[ -s ${all_files} ]]
then
  echo "New DSTs:"
  wc -l ${all_files}
  cat ${tstamp_file} >> tstamp.history
else
  echo "No new DSTs found"
  exit
fi

fgrep -f ${tobedonefile} ${TOPDIR}/ALL_FILES/all_files.* | sed 's/^.*://' | sort | uniq > flist.tobedone 
${TOPDIR}/splitbyrun.sh flist.tobedone

# copy macros and scripts
cd ${SUBMITDIR}
cp -p /sphenix/user/chiu/sphenix_bbc/offline/macros/calibrations/mbd/* .

# submit jobs to condor
wc -l ?????.list | grep -v total | while read ndsts fname
do
  if [[ $ndsts -lt 30 ]]
  then
    #maxdsts=$(grep ${fname%.list} ${tmp_runsfile} | awk '{print int($3/100000)}')
    echo $ndsts $fname >> SKIPPING
  else
    cd ${SUBMITDIR}
    echo ./submit.sh ./myrun.sh ${tobedonedir}/${fname}
    ./submit.sh ./myrun.sh ${tobedonedir}/${fname}
    cd ${tobedonedir}
    echo ${fname%.list} >> submit.runs
  fi
done

# Update submitted runs if there are any to submit
if [[ -f submit.runs ]]
then
  cp -p ${TOPDIR}/submitted.runs .
  cat ${TOPDIR}/submitted.runs submit.runs | sort | uniq > /tmp/submit.$$
  cp -p /tmp/submit.$$ ${TOPDIR}/submitted.runs
else
  echo No runs found
fi

