#!/bin/csh
echo ' '
echo 'START: '`date`

source /etc/csh.login
foreach i (/etc/profile.d/*.csh)
  source $i
end


echo hi Shyam doing job num $1

source /opt/sphenix/core/bin/sphenix_setup.csh -n new

source /opt/sphenix/core/bin/setup_local.csh /sphenix/user/schauhan1/install/jan23install


cd /gpfs/mnt/gpfs02/sphenix/user/schauhan1/jan23_pi0/macro

 root.exe -b <<EOF
   .L Fun4All_pi0_SIMPLE_EMBED_JF.C
	  Fun4All_pi0_SIMPLE_EMBED_JF(
    100,
		$1,
    "/sphenix/user/schauhan1/jan23_pi0/macro/condor/out_jan23_$1.root",
    0,
    ".")		
.q
EOF
echo ' '
echo ' '
echo 'END: '`date`
echo ' '

