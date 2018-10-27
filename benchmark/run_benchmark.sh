#!/bin/bash
N=1000
for idx in `seq 10 10 $N`;
do
    LEN=$idx ./random_string --benchmark_format=json --benchmark_out="data/data_$idx.json"
done
