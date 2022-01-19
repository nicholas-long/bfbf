#!/bin/bash

gcc -D DATA_SIZE_CHAR=32 bfbf.c bloom/src/bloom.c -o bfbf -fopenmp -lm 
gcc -D DATA_SIZE_CHAR=40 bfbf.c bloom/src/bloom.c -o bfbf_sha1 -fopenmp -lm 
