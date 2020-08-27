#!/bin/bash

BASE_DIR=/home/benny/Projects/tdcs-pipeline/surfaceremesh

CONTAINER_NAME=cgal_and_gmsh_compiled_with_tbb_ubuntu2004

docker run               \
	--rm                 \
	--name=ubuntu        \
	-v $BASE_DIR:/shares \
	$CONTAINER_NAME      \
	bash -c "cd /shares/ && cmake . && make VERBOSE=1"	
