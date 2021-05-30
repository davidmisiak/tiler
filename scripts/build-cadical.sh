#!/bin/bash

git submodule update --init
cd external/cadical
[[ -e build ]] && rm -r build
./configure
make
