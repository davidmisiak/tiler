#!/bin/bash

git submodule update --init --recursive
cd external/pblib
[[ -e build ]] && rm -r build
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DPBLIB_BUILD_BINARIES=OFF
cmake --build . -j 4
