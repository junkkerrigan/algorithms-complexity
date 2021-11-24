#!/bin/bash

[ -f external/benchmark/include/benchmark/benchmark.h ] && [ -d external/becnhmark/build ] && exit 0

mkdir external && cd external
git clone https://github.com/google/benchmark.git
cd benchmark

# https://github.com/google/benchmark#installation
cmake -E make_directory "build"
cmake -E chdir "build" cmake -DBENCHMARK_DOWNLOAD_DEPENDENCIES=on -DCMAKE_BUILD_TYPE=Release ../
cmake --build "build" --config Release