#!/bin/sh

source /opt/sphenix/core/bin/sphenix_setup.sh -n new

echo "nevent is "$1
echo "Run number is "$2
echo "FELIX/Server/EBDC number is" $3
echo "MVTX(0)/INTT(1)/TPC(2)/TPOT(3) is "$4
echo "Outfile name is "$5
echo "type is "$6 # prdf type, e.g. physics, beam, cosmics, etc

rm *.list

if [ $4 -eq 0 ]
then
    filename=`printf "mvtx%01i.list" $3`
    echo "Processing filename "$filename
    sh gl1_makelist.sh $6 $2
    sh mvtx_makelist.sh $6 $2
    root -b -q Fun4All_SingleStream_Combiner.C\($1,$2,\"./\",\"$5\",\"gl1daq.list\",\"\",\"\",\"$filename\"\)
elif [ $4 -eq 1 ]
then
    filename=`printf "intt%01i.list" $3`
    echo "Processing filename "$filename
    sh gl1_makelist.sh $6 $2
    sh intt_makelist.sh $6 $2
    root -b -q Fun4All_SingleStream_Combiner.C\($1,$2,\"./\",\"$5\",\"gl1daq.list\",\"\",\"$filename\",\"\"\)
elif [ $4 -eq 2 ]
then
    filename=`printf "tpc%02i.list" $3`
    echo "Processing filename "$filename
    sh gl1_makelist.sh $6 $2
    sh tpc_makelist.sh $6 $2
    root -b -q Fun4All_SingleStream_Combiner.C\($1,$2,\"./\",\"$5\",\"gl1daq.list\",\"$filename\",\"\",\"\"\)
elif [ $4 -eq 3 ]
then
    filename="tpot.list"
    echo "Processing filename "$filename
    sh gl1_makelist.sh $6 $2
    sh tpot_makelist.sh $6 $2
    root -b -q Fun4All_SingleStream_Combiner.C\($1,$2,\"./\",\"$5\",\"gl1daq.list\",\"\",\"\",\"\",\"$filename\"\)

else
    echo "Unsupported number, exiting"
fi
