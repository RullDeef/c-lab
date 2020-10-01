#!/bin/bash

declare -i N_MAX=10000

py perf_test/generate.py $N_MAX

for ((N = 5; N <= N_MAX; N += 50))
do
  sed -n "1,${N}p" perf_test/data.txt > .temp
  ./app.exe .temp .temp_out 1>/dev/null 2>> perf_test/log.txt
done

rm -rf .temp .temp_out

py perf_test/graph.py
