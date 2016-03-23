#!/bin/bash

patch=`dirname $0`/checkpatch_tizen.pl

echo "This script only checks brace rules."
echo "If you want to see details, remove BRC in this script."
echo "Please wait for a while..."

for i in `find $1 -name "*.cpp"; find $1 -name "*.h"`
do
	$patch $i | grep BRC
done

echo "Done..."

exit
