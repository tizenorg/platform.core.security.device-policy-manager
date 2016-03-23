#!/bin/bash

cpplint=`dirname $0`/cpplint_tizen.py

echo "Please wait for a while"

for i in `find $1 -name "*.h"; find $1 -name "*.cpp"`
do
	python $cpplint $i
done
