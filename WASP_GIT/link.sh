#!/bin/bash

DIRNAME=`dirname $0`
ln -s $DIRNAME/*.h $DIRNAME/*.cpp $DIRNAME/learning $DIRNAME/stl $1
