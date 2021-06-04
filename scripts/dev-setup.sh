#!/bin/bash

[[ -e build ]] && rm -r build
mkdir build
cd build
conan install ..
cmake .. -DBUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Debug
