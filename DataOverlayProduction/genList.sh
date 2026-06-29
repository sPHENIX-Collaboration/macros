psql FileCatalog -t -A -c  "select filename from datasets where segment=${1} and filename like 'DST_TRIG%' and runnumber=54404;" > test.list
