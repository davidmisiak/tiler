#!/bin/bash

# Used for automatized tuning of Gurobi parameters. Before running, uncomment `model_->write(...)`
# lines in `src/solvers/ilp/gurobi_wrapper.cpp`. Recommended parameters for each of the selected
# problems will be saved in the `tune/` directory.

for PROBLEM in "many-unique/2-3-4-5-09x14_s" "many-unique/5r-H-08x09'_u" \
    "many-unique/5r-diamond'_u" "many-unique/5r-skew-06x10'_u" "mixed/3L-4J-10x14_s"
do
    for SOLVER in "eq-ign" "eq-min" "geq-min" "leq-max"
    do
        RESULTS="tune/$PROBLEM/$SOLVER"
        echo "========================================"
        echo "$RESULTS"
        echo "========================================"
        mkdir -p "$RESULTS"
        ./build/bin/tiler solve -v -b "ilp_gurobi_default_$SOLVER" -i "problems/$PROBLEM"
        grbtune ResultFile="$RESULTS/tune" TuneTimeLimit=600 $(sed "s/  /=/" "model.prm") "model.mps"
    done
done
