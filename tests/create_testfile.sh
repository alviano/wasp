#!/bin/bash
if [ $# -ne 2 ]; then
    echo "Usage: ./scriptname filename sat/unsat";
    exit
fi

filename=$1.$2.test.py

echo "input = \"\"\"" > $filename
cat $1 >> $filename
echo "\"\"\"" >> $filename
echo "output = \"\"\"" >> $filename
echo $2 >> $filename
echo "\"\"\"" >> $filename
