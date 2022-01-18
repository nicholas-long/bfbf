#!/bin/bash

./build.sh

hash="fde5f29a4ff67cd1439428c865274637"
echo $hash | ./testbloom -a
echo $hash | ./testbloom -q

