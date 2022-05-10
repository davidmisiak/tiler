#!/bin/bash

source scripts/load-env.sh

/bin/time -v ./release/bin/tilerbm \
    --benchmark_out="benchmark/$(date '+%Y-%m-%d_%H-%M-%S').json" \
    --benchmark_filter='@simple|@dlx|@sat|@ilp' \
    # --benchmark_filter="-$(cat benchmark/known-timeout.txt | head -c -1 | tr '\n' '|')"
