#!/bin/bash

git submodule update --init --recursive
cd external/cryptominisat
[[ -e build ]] && rm -r build
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DSTATICCOMPILE=ON
cmake --build . -j 4
