#!/usr/bin/env bash

set -e

rm -vf callgrind* cachegrind*

cmake -B build . -DCMAKE_EXPORT_COMPILE_COMMANDS=1
cmake --build build --config RelWithDebInfo -j 4

valgrind --tool=callgrind --cache-sim=yes --dump-instr=yes --callgrind-out-file=callgrind.soa.out \
         --D1=4096,8,64 ./build/soa

valgrind --tool=callgrind --cache-sim=yes --dump-instr=yes --callgrind-out-file=callgrind.aos.out \
         --D1=4096,8,64 ./build/aos

kcachegrind callgrind.soa.out &
kcachegrind callgrind.aos.out &