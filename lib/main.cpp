
#include <string>
#include <map>
#include <iostream>
#include <vector>

#include "reader.hpp"
#include "graph.hpp"
#include "reduction.hpp"


// Main function
int main(int argc, char const *argv[])
{
    // File name
    string name = string(argv[1])+".graph";
    // Intantiating graphs
    Graph gs = Graph(); // The graph g
    Graph gl = Graph(); // The graph g'
    // Reading from file and creating the graphs
    read(name, gl, gs);
    
    // Adding candidates for nodes in graph g1 (all nodes of gl)
    for(Node& n_: gs.nodes_()) {
        n_.create_set(gl.nodes_());
    }

    // Starting with reductions
    // First reduction (based on degrees)
    unary_reduction(gs);
    // Secondary reduction (based onn arc consitency)
    // TODO

    return 0;
}
