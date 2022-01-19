#!/bin/bash

split=10
find /usr/bin -type f 2>/dev/null | xargs md5sum 2>/dev/null | awk '{print $1}' | sort -u > testdata.lst
echo YES SET
tail -n +$(($split + 1)) testdata.lst
echo TEST SET
head -n $split testdata.lst

