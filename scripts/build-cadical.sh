#!/bin/bash

git submodule update --init --recursive
cd external/cadical
[[ -e build ]] && rm -r build
./configure
make -j 4
