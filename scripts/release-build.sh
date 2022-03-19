#!/bin/bash

source scripts/gurobi-env.sh

cd release
cmake --build .
