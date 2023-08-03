#!/bin/bash

source scripts/load-env.sh

cd build
cmake --build . -j 4
