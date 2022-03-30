#!/bin/bash

git submodule update --init --recursive
cd external/gecode
[[ -e build ]] && rm -r build
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .

mkdir include
cp -r ../gecode include/gecode
cp gecode/support/config.hpp include/gecode/support
