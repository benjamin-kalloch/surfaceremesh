#!/bin/bash

BASE_DIR=$HOME/Repositories/tdcs-pipeline/

CONTAINER_NAME=cgal_and_gmsh_compiled_with_tbb_ubuntu1904

docker run               \
	--rm                 \
	--name=ubuntu        \
	-v $BASE_DIR:/shares \
	$CONTAINER_NAME      \
	bash -c "cd /shares/ && cmake . && make VERBOSE=1"	
