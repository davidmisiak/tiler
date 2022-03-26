#!/bin/bash

cd external/dlx1
[[ -e build ]] && rm -r build
mkdir build
g++ dlx1-lib.cpp -o build/libdlx.o -c -O3
ar rcs build/libdlx.a build/libdlx.o
