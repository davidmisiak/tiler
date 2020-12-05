#!/bin/bash

[[ -e build ]] && rm -r build
mkdir build
cd build
conan install ..
cmake .. -D BUILD_TESTS=ON
