#!/bin/bash

./build.sh
rm data/*
./bfbf -i

split=500

hash="fde5f29a4ff67cd1439428c865274637"
hash2="abe5f29a4ff67cd1439428c865274637"
echo Adding
head -n $split testdata.lst | ./bfbf -a

echo Querying contained: Should print Y for all
head -n $split testdata.lst | ./bfbf -q

echo Querying not contained: Should print N for most
tail -n +$(($split + 1)) testdata.lst | ./bfbf -q

# bottom part
# tail -n +$(($split + 1)) testdata.lst
# top part
# head -n $split testdata.lst
