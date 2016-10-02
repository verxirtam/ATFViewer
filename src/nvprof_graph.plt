
set datafile separator ","
set xtics rotate by 90
plot '< tail -17 ./glunittest.nvprof' using 0:1:xtic(7) with boxes

