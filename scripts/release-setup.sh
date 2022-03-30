#!/bin/bash

source scripts/load-env.sh

[[ -e release ]] && rm -r release
mkdir release
cd release
conan install ..
cmake .. -DBUILD_BENCHMARK=ON "$@"
