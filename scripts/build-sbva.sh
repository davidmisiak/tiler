#!/bin/bash

cd external/SBVA
[[ -e build ]] && rm -r build
mkdir build
g++ -I eigen-3.4.0/ sbva_lib.cc -o build/sbva_lib.o -c -O3 -std=c++11
ar rcs build/libsbva.a build/sbva_lib.o
