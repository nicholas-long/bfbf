#!/bin/bash

./build.sh
rm data/*
./bfbf -i

hash="fde5f29a4ff67cd1439428c865274637"
hash2="abe5f29a4ff67cd1439428c865274637"
echo Adding
echo $hash | ./bfbf -a
echo Querying : Should print Y and then N
(echo $hash && echo $hash2) | ./bfbf -q

# bottom part
# tail -n +$(($split + 1)) testdata.lst
# top part
# head -n $split testdata.lst
