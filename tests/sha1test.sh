#!/bin/bash

echo Initializing
./build.sh
rm data/*
./bfbf_sha1 -i
echo Done

split=500

date
echo "Adding $split hashes"
head -n $split testinput/sha1_test_hashes.lst | ./bfbf_sha1 -a

date
echo Querying contained: Should print Y for all
head -n $split testinput/sha1_test_hashes.lst | ./bfbf_sha1 -q | grep N

date
echo Querying not contained: Should print N for most
failedcount=$(tail -n +$(($split + 1)) testinput/sha1_test_hashes.lst | ./bfbf_sha1 -q | grep Y | wc -l)
echo "Failed: $failedcount / $(tail -n +$(($split + 1)) testinput/sha1_test_hashes.lst | wc -l)"


date

# bottom part
# tail -n +$(($split + 1)) testinput/sha1_test_hashes.lst
# top part
# head -n $split testinput/sha1_test_hashes.lst
