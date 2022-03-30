#!/bin/bash

git submodule update --init --recursive
cd external/libminizinc
[[ -e build ]] && rm -r build
mkdir build
cd build
GECODE_PATH="../../gecode/build"
cmake .. -DCMAKE_BUILD_TYPE=Release -DGecode_ROOT=$GECODE_PATH -DGECODE_INCLUDE=$GECODE_PATH/include
cmake --build .
