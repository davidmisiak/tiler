#!/bin/bash

source scripts/load-env.sh

cd release
cmake --build . -j 4
