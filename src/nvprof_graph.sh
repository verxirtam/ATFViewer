#!/bin/bash
gnuplot << EOF
set terminal png
set output 'nvprof_$1.png'
set datafile separator ","
set xtics rotate by 90
plot '< tail -n +$((`grep -n "API calls:" $1.nvprof | cut -d ":" -f 1` + 3)) $1.nvprof' using 0:1:xtic(7) with boxes
EOF
