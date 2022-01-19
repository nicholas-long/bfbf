#!/bin/bash

for i in {0..1000000}; do
  echo $i | md5sum | awk '{print $1}'
done
