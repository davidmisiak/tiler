#!/bin/bash

source scripts/gurobi-env.sh

/bin/time -v ./release/bin/tilerbm \
    --benchmark_out="benchmark/$(date '+%Y-%m-%d_%H-%M-%S').json" \
    --benchmark_filter='' \
    # --benchmark_filter="-$(cat benchmark/known-timeout.txt | head -c -1 | tr '\n' '|')" \
    # --benchmark_repetitions=10 --benchmark_report_aggregates_only=true
