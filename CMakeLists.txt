cmake_minimum_required(VERSION 2.6)

#
# NOTE: all paths are relative to the docker environment NOT the host system!
#		See: 'dockerrun.sh' for the entry point of the host system to the docker environment. 
#

project(remeshsurface)
set (volmesh_VERSION_MAJOR 1)
set (volmesh_VERSION_MINOR 0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/deploy)
set (CMAKE_CXX_STANDARD 17)	# for std::remove to work properly

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

add_executable(remeshsurface remesh_surface.cpp)

target_link_libraries(remeshsurface ${VTK_LIBRARIES} stdc++fs gmp mpfr CGAL CGAL_ImageIO)

# we need to link against stdc++fs to use std::filesystems