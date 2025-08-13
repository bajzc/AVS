#!/usr/bin/env bash

set -e

rm -vf callgrind* cachegrind*

cmake -B build .
cmake --build build --config RelWithDebInfo -j 4

valgrind --tool=callgrind --cache-sim=yes --dump-instr=yes --callgrind-out-file=callgrind.soa.out \
         --D1=4096,8,64 ./build/soa

valgrind --tool=callgrind --cache-sim=yes --dump-instr=yes --callgrind-out-file=callgrind.aos.out \
         --D1=4096,8,64 ./build/aos