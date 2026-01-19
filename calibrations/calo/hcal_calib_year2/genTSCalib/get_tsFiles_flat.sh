#!/bin/bash

currDir=$PWD

cosmicsFile=$1 #cosmics runs
tsFile=$2      #file to write merged TS root files to
mergeNum=$3    #how many TS files to merge after given cosmics run #
run_ohcal=$4   # true/false: run HCALOUT
run_ihcal=$5   # true/false: run HCALIN


dummyFile="dummy.txt"  #file to hold first 'x' files after cosmics run # - sort of a staging file to ensure we got right files
tag="cosmics"          #when searching for merged ts files, make sure to look for "merged_XXXXX.root" and not the merged root files with "cosmics" in name - given we use cosmics in the name for this scripts output

mkdir -p merged

while IFS= read -r line
do

    echo "looking at run " 
    echo "${IFS}" 
    x=0  #counter
    incr=$line

    #path to LCE output files
   # cd /sphenix/u/bseidlitz/work/macros/calibrations/calo/hcal_towerSlope_y2/cos_only_tsc_spec_output2/
   cd /sphenix/user/anjsmenon/work/macros/calibrations/calo/hcal_calib_year2/condor_histmaker/out_run/ 

    touch $dummyFile

    while [ $x -lt $mergeNum ]
    do
	
	incr=$(($incr+1))

        if [ $incr -gt 70000 ]; then
            echo "Incr has exceeded 60000, exiting loop."
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
              continue
	fi

    echo "Merging these files..."
    cat $dummyFile  

    #merge files in dummyFile and send to new text file
    string="$currDir/merged/mergedTS_cosmics_$line.root"

    rm $string 
    bash -c "hadd -f $string @$dummyFile"

    echo "$string" >> $tsFile

    rm $dummyFile

done < $cosmicsFile

mv $tsFile $currDir

#execute fitting macro
cd $currDir

mkdir -p "$currDir/fitResults"


root.exe -b -q run_cosmicsTS.C\(\"$cosmicsFile\",\"$tsFile\",$run_ohcal,$run_ihcal\) 

echo "Finished running get_ts_files.sh."
echo "All done!"
