#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Polygon_mesh_processing/remesh.h>
#include <CGAL/Polygon_mesh_processing/border.h>
#include <boost/function_output_iterator.hpp>


#include <fstream>
#include <vector>
#include <sstream>       // istringstream
#include <getopt.h>      // use GNU extension of getopt to be able to parse more readable multi-character options
#include <iostream>
#include <filesystem>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Surface_mesh<K::Point_3> Mesh;

typedef boost::graph_traits<Mesh>::halfedge_descriptor halfedge_descriptor;
typedef boost::graph_traits<Mesh>::edge_descriptor     edge_descriptor;

namespace PMP = CGAL::Polygon_mesh_processing;

struct halfedge2edge
{
    halfedge2edge(const Mesh& m, std::vector<edge_descriptor>& edges) : 
        m_mesh(m),
        m_edges(edges)
    { }

    void operator()(const halfedge_descriptor& h) const
    {
        m_edges.push_back(edge(h, m_mesh));
    }
    
    const Mesh& m_mesh;
    std::vector<edge_descriptor>& m_edges;
};

    // function prototypes
void printHelpText();

int main(int argc, char* argv[])
{
    std::vector< std::string > input_files;
    
    // The edge length that is targeted in the remeshed patch.
    // If 0 is passed then only the edge-flip, tangential relaxation,
    // and projection steps will be done.
    double target_edge_length = 1.5;
    // #iterations of the sequence of iterations performed in isotropic_remeshing()
    // -> The bigger this number, the smoother and closer to target
    //    edge length the mesh will be.
    // Default value: 1
    unsigned int nb_iter = 10;
    // #iterations of tangential relaxation; 
    // -> A larger number of relaxation steps lead to a more isotropic mesh.
    // Default value: 1
    unsigned int nb_relaxations = 5;    
    
    // ************* parsing commandline arguments **************
    int c;
    int option_index = 0;
    static struct option long_options[] = {
        {"inputfiles",1,NULL,0},
        {"edge_length",1,NULL,0},
        {"nb_iterations",1,NULL,0},
        {"nb_relaxations",1,NULL,0},
        {"help",0,NULL,0},
        {NULL,0,NULL,0}
    };
    
    while( ( c = getopt_long_only( argc, argv, "", long_options, &option_index ) ) != -1 )
    {
        if( c == 0 )    // long option detected, c > 0 = short option detected, c = -1 end of options
        {
            switch( option_index )
            {
                case 0:    // inputfiles
                {
                    std::istringstream inputfiles_string( optarg );
                    std::string path;
                    for( std::size_t p = 0; std::getline(inputfiles_string, path, ',' ); p++ )
                    {
                        input_files.push_back( path );
                    }
                }
                break;

                case 1:    // faces edge length
                    target_edge_length = std::stof( optarg );
                break;

                case 2:    // #iterations
                    nb_iter = std::stoi( optarg );
                break;

                case 3:    // #relaxations
                    nb_relaxations = std::stof(optarg);
                break;
                case 4:    // help
                    printHelpText();
                     return 0;
                break;
            }
        }
    }

    for( std::string &filename : input_files )
    {
        std::ifstream input(filename);
        Mesh mesh;
        if (!input || !(input >> mesh) || !CGAL::is_triangle_mesh(mesh)) {
        std::cerr << "Not a valid input file." << std::endl;
        return 1;
        }
        
        std::cout << "Split border...";
        std::vector<edge_descriptor> border;
        PMP::border_halfedges
        (
            faces(mesh),
            mesh,
            boost::make_function_output_iterator(halfedge2edge(mesh, border))
        );
        PMP::split_long_edges(border, target_edge_length, mesh);
        std::cout << "done." << std::endl;

        std::cout << "Start remeshing of " << filename
                  << " (" << num_faces(mesh) << " faces)..." << std::endl;
        
        PMP::isotropic_remeshing
        (
            faces(mesh),
            target_edge_length,
            mesh,
            PMP::parameters::number_of_iterations(nb_iter)
                            .protect_constraints(false)
                            .number_of_relaxation_steps(nb_relaxations)
        );
        std::cout << "Remeshing done." << std::endl;
        
        std::string outpath("output/");
        outpath.append( std::filesystem::path( filename ).stem() );
        outpath.append( "_remeshed.off" );
        std::cout << "Write to " << outpath << std::endl;
        std::ofstream output( outpath  );
        output << mesh;
    }
    return 0;
}

void printHelpText()
{
    std::cout << "This tool remeshes a surface mesh to an isotropic surface mesh.\n"
              << "Usage: ./remesh [parameters], with the following parameters\n"
              << "\t --inputfiles ... paths to the input files, each of the input files will be remeshed seperately\n"
              << "\t --edge_length ... target edge length for the surface triangles\n"
              << "\t --nb_relaxations ... #relaxation steps; the more steps the more isotropic the mesh will be\n"
              << "\t --nb_iterations ... #remeshing iterations; the more iterations the smoother the mesh will be and the closer to the target edge length the acutal edge lenght will be" << std::endl;
}