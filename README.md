# SurfaceRemesh

This application uses the CGAL API [1] to remesh surfaces (i.e. the skin compartment). The application is detailed in in the paper **A flexible workflow for simulating transcranial electric stimulation in healthy and lesioned brains** by *Benjamin Kalloch, Pierre-Louis Bazin, Arno Villringer, Bernhard Sehm, and Mario Hlawitschka*.

## How to compile
You need the following prerequisite
- the CGAL development library v4.13.1
- On Ubuntu 19.04 the following packages are required: *build-essential libcgal-dev libgmp-dev libmpfr-dev libvtk6-dev cmake* (this will also install the CGAL development files of the required version)

## How to run
We provide a bash script *runMeshingTool.sh* in the deploy directory.
The following parameters are supported by our meshing tool ([!]=mandatory parameter, [*]=optional parameter):
- **_inputfiles_** = a comma-separated list of the input surface files (in OFF file format) [!]
- **_edge_length_** = target length of a triangle edge [*]
- **_nb_iterations_** = number of remeshing iterations (bigger numbers yield smoother surfaces and an edge lenght closer to the target edge length)  [*]
- **_nb_relaxations_** = numer of relaxation steps (the more steps the more isotropic the mesh will be) [*]
- **_help_** = display the help text [*]

## Secondary information
To facilitate the setup of the proper environment to built the SurfaceRemesh tool, we provide a Dockerfile to create a container based on Ubuntu 19.04 which contains all required libraries. 

Simply build the container with `docker build -t cgal_ubuntu1904 .` in the docker directory of this repository.
Note: The container built according to the descriptions of the MeshHeadModel-tool can be used for the SurfaceRemesh-tool as well, but not vice-versa.

When compiled in the container, the tool also must be launched from inside the container. You can do this by calling the *docker_run.sh* script which again invokes the *runMeshingTool.sh* script as mentioned before from inside the container. When using docker to run the tool consider that all paths within *runMeshingtTool.sh* (both input files and output files) must be specified according to the docker environment NOT according to the host machine. The best practice is to use the predefined input & output directories and to use paths relative to the SurfaceRemesh executable. 

! Note that for all three docker scripts you may have to adjust the variables $BASE_DIR and $CONTAINER_NAME if you use a path/name other then predefined in the file.

###### Useful links
[1] The CGAL project: https://doc.cgal.org/4.13/Manual/index.html