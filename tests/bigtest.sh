#!/bin/bash

echo Initializing
./build.sh
rm data/*
./bfbf -i
echo Done

split=50000

date
echo Adding
head -n $split testinput/big_dataset.lst | ./bfbf -a

date
echo Querying contained: Should print Y for all
head -n $split testinput/big_dataset.lst | ./bfbf -q | grep N

date
echo Querying not contained: Should print N for most
tail -n +$(($split + 1)) testinput/big_dataset.lst | ./bfbf -q | grep Y

date

# bottom part
# tail -n +$(($split + 1)) testinput/big_dataset.lst
# top part
# head -n $split testinput/big_dataset.lst
