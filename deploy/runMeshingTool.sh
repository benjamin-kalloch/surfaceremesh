#!/bin/bash

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.:$(pwd)/libs

echo "Remeshing $1"

./remeshsurface --inputfiles $1    \
                --edge_length 1.3  \
                --nb_iterations 10 \
                --nb_relaxations 10
