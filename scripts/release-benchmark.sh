#!/bin/bash

echo benchmark/data/* | ./release/bin/tilerbm --benchmark_out="benchmark/$(date '+%Y-%m-%d_%H-%M-%S').json"
