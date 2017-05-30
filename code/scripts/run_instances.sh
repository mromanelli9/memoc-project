#!/usr/bin/bash
#title           : gen_instances.sh
#description     : This script will run the program over all the instances.
#author          : Marco Romanelli
#date            : 30/05/2017
#version         : 1
#usage           : bash gen_instances.sh
#==============================================================================

INSTANCES_DIR="instances"
MAIN=bin/main
OUTFILE_DIR="output"
OUTFILE=$OUTFILE_DIR/results.txt

mkdir "$OUTFILE_DIR"
touch $OUTFILE
for file in `ls $INSTANCES_DIR/*.tsp | sort -V`; do
	echo "Running on $file.";
	TIME=`/usr/bin/time -f "%U\t%S\t%x" $MAIN "$file" 2>&1`
	RES=`echo $?`
	echo -e "$TIME\t$RES" >>$OUTFILE
	sleep 2
done

echo "Done."

exit 0
