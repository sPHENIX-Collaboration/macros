#!/bin/bash

currDir=$PWD

cosmicsFile=$1 #cosmics runs, or the first run of each towerslope period
tsFile=$2      #file to write merged TS root files to
mergeNum=$3    #how many TS files to merge after given cosmics run #
               #"all": merge all runs from the run in the list up to the next run in the list (one towerslope per period)
run_ohcal=$4   # true/false: run HCALOUT
run_ihcal=$5   # true/false: run HCALIN
#path to LCE output files, default is where condor_histmaker/copy_outfiles.sh puts them
spectraDir=${6:-$currDir/../condor_histmaker/out_run}
if [ ! -d "$spectraDir" ]; then
    echo "WARNING: spectra folder $spectraDir not found. Make it with condor_histmaker (hadd_condor.sh, copy_outfiles.sh) or give the folder as the last argument"
    exit 1
fi
echo "spectra from $spectraDir"


dummyFile="dummy.txt"  #file to hold first 'x' files after cosmics run # - sort of a staging file to ensure we got right files
tag="cosmics"          #when searching for merged ts files, make sure to look for "merged_XXXXX.root" and not the merged root files with "cosmics" in name - given we use cosmics in the name for this scripts output

mkdir -p merged
rm -f $tsFile

#runs in the list, to find where each period ends
allRuns=($(grep -v '^$' $cosmicsFile))
iline=0

while IFS= read -r line
do

    [ -z "$line" ] && continue  # skip blank lines

    echo "looking at run "
    echo "${IFS}" 
    x=0  #counter
    incr=$line

    cd $spectraDir

    touch $dummyFile

    if [ "$mergeNum" = "all" ]; then
        #all runs of the period: from this run up to the run before the next one in the list
        iline=$((iline+1))
        nextRun=${allRuns[$iline]}
        if [ -z "$nextRun" ]; then
            #last period of the list: it ends at the last run of the dataset in ../HcalCalibDatasetinputs.h
            lastRun=`root -l -b -q "$currDir/../PrintHcalCalibDatasetinputs.C(\"$line\")" | grep '^HCALCALIB_LASTRUN=' | cut -d= -f2-`
            if [ -n "$lastRun" ]; then
                nextRun=$((lastRun+1))
            else
                echo "WARNING: last run of the dataset of run $line not found in ../HcalCalibDatasetinputs.h, merging all runs from $line on"
                nextRun=1000000
            fi
        fi
        for FILE in `find ~+ -name "*.root" -not -name "*$tag*" | sort`
        do
            run=`basename $FILE | grep -oE '[0-9]{5,6}' | head -1`
            if [ -n "$run" ] && [ $run -ge $line ] && [ $run -lt $nextRun ]; then
                echo $FILE >> $dummyFile
            fi
        done
        echo "period starting at run $line: `cat $dummyFile | wc -l` runs before run $nextRun"
    fi

    while [ "$mergeNum" != "all" ] && [ $x -lt $mergeNum ]
    do
	
	incr=$(($incr+1))

        if [ $incr -gt 90000 ]; then
            echo "Incr has exceeded 90000, exiting loop."
            break
        fi


	FILE=`find ~+ -name "*$incr*" -not -name "*$tag*" -print -quit`

	if [[ -a $FILE ]]
	then
	    echo $FILE >> $dummyFile
	    x=$((x+1))
	fi

    done
    # Before running hadd, check if dummy.txt is empty
	if [ ! -s $dummyFile ]; then
   	     echo "No valid files found for run $line, skipping..."
              rm -f $dummyFile
              continue
	fi

    echo "Merging these files..."
    cat $dummyFile  

    #merge files in dummyFile and send to new text file
    string="$currDir/merged/mergedTS_cosmics_$line.root"

    rm $string 
    bash -c "hadd -f $string @$dummyFile"

    echo "$line $string" >> $currDir/$tsFile

    rm $dummyFile

done < $cosmicsFile


#execute fitting macro
cd $currDir

#the fitting macro reads the run list and the merged file list line by line together
cut -d' ' -f1 $tsFile > ${tsFile}.runs
cut -d' ' -f2 $tsFile > ${tsFile}.files
mv ${tsFile}.files $tsFile

mkdir -p "$currDir/fitResults"


root.exe -b -q run_cosmicsTS.C\(\"${tsFile}.runs\",\"$tsFile\",$run_ohcal,$run_ihcal\) 

echo "Finished running get_ts_files.sh."
echo "All done!"
