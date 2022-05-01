#!/bin/bash

git submodule update --init --recursive
cd external/chuffed

# dirty hack to resolve something that is likely a Bison version mismatch, TODO better solution
sed -i '/^find_package(BISON)/s/^/#/' CMakeLists.txt
sed -i '/^find_package(FLEX)/s/^/#/' CMakeLists.txt

[[ -e build ]] && rm -r build
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DBISON_FOUND=false -DFLEX_FOUND=false
cmake --build .
