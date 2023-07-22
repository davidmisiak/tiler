#!/bin/bash

git submodule update --init --recursive
cd external/breakid

# dirty hack to accomodate new gcc that has more strict include requirements, TODO better solution
sed -i '1s;^/;#include <cstdint>\n/;' src/config.hpp
sed -i '1s;^/;#include <cstdint>\n/;' src/breakid.hpp

[[ -e build ]] && rm -r build
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DSTATICCOMPILE=ON
cmake --build .
