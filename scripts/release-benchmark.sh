#!/bin/bash

./release/bin/tilerbm \
    --benchmark_out="benchmark/$(date '+%Y-%m-%d_%H-%M-%S').json" \
    --benchmark_filter='' \
    # --benchmark_repetitions=10 --benchmark_report_aggregates_only=true
