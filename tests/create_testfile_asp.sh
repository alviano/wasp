#!/bin/bash
if [ $# -ne 2 ]; then
    echo "Usage: ./scriptname input_filename output_filename";
    exit
fi

filename=$1.test.py

echo "input = \"\"\"" > $filename
cat $1 >> $filename
echo "\"\"\"" >> $filename
echo "output = \"\"\"" >> $filename
cat $2 >> $filename
echo "\"\"\"" >> $filename
