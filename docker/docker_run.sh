#!/bin/bash

BASE_DIR=/home/benny/Projects/tdcs-pipeline/surfaceremesh/
CONTAINER_NAME=cgal_and_gmsh_compiled_with_tbb_ubuntu2004
CGAL_LIBRARY_LOCATION=/usr/local/lib/

#INFILES="input/csf_taubinsmoothed_fixed.off input/gm_taubinsmoothed_fixed.off input/wm_taubinsmoothed_fixed.off input/skull_taubinsmoothed_fixed.off input/midlayer_taubinsmoothed_fixed.off input/skin_blendersmoothed_fixed.off"
INFILES="input/skin_blendersmoothed_fixed.off"

echo $BASE_DIR
ls $BASE_DIR

# parallel calls of the SurfaceRemesh tool
CTR=0
for file in $INFILES; do
    docker run                  \
        --rm                    \
        --name=ubuntu$CTR       \
        -v ${BASE_DIR}:/shares  \
        $CONTAINER_NAME         \
        bash -c "cd /shares/deploy/ && export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$CGAL_LIBRARY_LOCATION && ./runMeshingTool.sh $file" &
    CTR=$((CTR+1))
done

