#!/usr/bin/env bash

set -e

rm -vf callgrind* cachegrind*

cmake -B build .
cmake --build build

valgrind --tool=callgrind --cache-sim=yes --dump-instr=yes --callgrind-out-file=callgrind.soa.out ./build/soa

valgrind --tool=callgrind --cache-sim=yes --dump-instr=yes --callgrind-out-file=callgrind.aos.out ./build/aos