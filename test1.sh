#!/bin/bash

./build.sh
rm data/*
./testbloom -i

hash="fde5f29a4ff67cd1439428c865274637"
echo Adding
echo $hash | ./testbloom -a
echo Querying
echo $hash | ./testbloom -q
echo Should fail
hash="abe5f29a4ff67cd1439428c865274637"
echo $hash | ./testbloom -q

