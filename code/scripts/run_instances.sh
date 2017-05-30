#!/usr/bin/bash
#title           : gen_instances.sh
#description     : This script will run the program over all the instances.
#author          : Marco Romanelli
#date            : 30/05/2017
#version         : 1
#usage           : bash gen_instances.sh
#==============================================================================

for file in `ls instances/*.tsp | sort -V`; do
	echo "Running on $file.";
	TIME=`/usr/bin/time -f "%U\t%S\t%x" bin/main "$file" 2>&1`
	RES=`echo $?`
	echo -e "$TIME\t$RES" >>results.txt
	sleep 2
done

echo "Done."

exit 0
