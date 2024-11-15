START_RUN=53000
END_RUN=53400
#END_RUN=49700

# Loop through the range and execute the script with each runnumber
for (( runnumber=START_RUN; runnumber<=END_RUN; runnumber++ ))
do
  echo "Running ./condor.sh with runnumber=$runnumber"
  ./condor.sh $runnumber 100000 
done;
