#!/bin/tcsh

set build=$1
set year=$2
set prod=$3

rm -rf fileLists/*.list

CreateDstList.pl --build ana$build --cdb ${year}p$prod DST_CALO --printruns > runlist.list
CreateDstList.pl --build ana$build --cdb ${year}p$prod DST_CALO --list runlist.list

rm runlist.list 

mv *.list fileLists
cd fileLists
cat dst*.list > allDSTlist.list
ls -1 $PWD/dst*.list > listOfLists.list
cd ..
makeSingleFileList.csh fileLists/listOfLists.list
