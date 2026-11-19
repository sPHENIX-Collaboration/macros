#!/bin/tcsh

source /etc/csh.login
foreach i(/etc/profile.d/*.csh)
    source $i
end
 
set runnumber=$1 #run number

set nFiles=$2 #files you want to hadd per job

set totalFiles=$3 #total number of files to hadd

@ nJobs=$totalFiles / $nFiles #number of jobs to be submitted

if ( ! -d  submission_hadd/$runnumber ) then
	mkdir submission_hadd/$runnumber
endif 

set runRange=0

while ($runRange <= $nJobs)

    if ( -f  submission_hadd/$runnumber/submitFor_Batch$$runRange.job) then
	rm submission_hadd/$runnumber/submitFor_Batch$runRange.job
    endif

    if( ! -d '/tmp/log_hadd_'$runnumber) then 
	mkdir '/tmp/log_hadd_'$runnumber
    endif 

    if( ! -d 'out_hadd/'$runnumber) then 
	mkdir 'out_hadd/'$runnumber
    endif

    if( ! -d 'err_hadd/'$runnumber) then 
	mkdir 'err_hadd/'$runnumber
    endif
    
    if( -f submission_hadd/$runnumber/submitFor_Batch$runRange.job) then
	rm submission_hadd/$runnumber/submitFor_Batch$runRange.job
    endif
    
    touch submission_hadd/$runnumber/submitFor_Batch$runRange.job
    echo 'initialdir = .' >> submission_hadd/$runnumber/submitFor_Batch$runRange.job
    echo 'executable = hadd_condor.csh' >> submission_hadd/$runnumber/submitFor_Batch$runRange.job
    echo 'arguments = '$runRange $nFiles $runnumber >> submission_hadd/$runnumber/submitFor_Batch$runRange.job
    echo 'log  = /tmp/log_hadd_'$runnumber'/job-'$runRange'.log' >> submission_hadd/$runnumber/submitFor_Batch$runRange.job
    echo 'output  = out_hadd/'$runnumber'/job-'$runRange'.out'  >> submission_hadd/$runnumber/submitFor_Batch$runRange.job 
    echo 'error = err_hadd/'$runnumber'/job-'$runRange'.err' >> submission_hadd/$runnumber/submitFor_Batch$runRange.job
    echo 'request_memory = 4GB' >> submission_hadd/$runnumber/submitFor_Batch$runRange.job
    echo 'should_transfer_files  = NO' >> submission_hadd/$runnumber/submitFor_Batch$runRange.job
    echo 'Queue' >> submission_hadd/$runnumber/submitFor_Batch$runRange.job

    condor_submit submission_hadd/$runnumber/submitFor_Batch$runRange.job
    @ runRange++
end
