#!/bin/bash

cd external/dlx1
[[ -e build ]] && rm -r build
gcc dlx1-lib.cpp -o build/libdlx.o -O3
ar rcs build/libdlx.a build/libdlx.o
