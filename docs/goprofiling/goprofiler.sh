#!/bin/sh

N=25
CMD="./go/bin/profiler"

i=1
while [ $i -le $N ]; do
    $CMD
    i=$((i + 1))
done
