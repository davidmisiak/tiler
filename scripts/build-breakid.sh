#!/bin/bash

git submodule update --init --recursive
cd external/breakid
[[ -e build ]] && rm -r build
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DSTATICCOMPILE=ON
cmake --build .
