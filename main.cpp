
#include <string>
#include <map>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

#include "lib/io.hpp"
#include "lib/graph.hpp"
#include "lib/reduction.hpp"


// Main function
int main(int argc, char const *argv[])
{
    // First Runner
    if(strcmp(argv[2], "1") == 0) {
        // File name
        string name = string(argv[1])+".graphs";
        // Intantiating graphs
        Graph gs = Graph(); // The graph g
        Graph gl = Graph(); // The graph g'


        // Reading from file and creating the graphs
        read(name, gl, gs);
        cout << "Read\n";

        // Starting with reductions
        // First reduction (based on degrees)
        unary_reduction(gs);
        cout << "Unary\n";
        // Secondary reduction (based on arc consitency)
        while(secondary_reduction(gs, gl)) cout << "Iteration\n";
        cout << "Binary\n";
        // Sorting to keep the nodes in order
        sort(gs.nodes_().begin(), gs.nodes_().end(), [](Node& _1, Node& _2){return _1.index_() < _2.index_();});
        

        // Writing the data created to files
        // 1. Write the singular and variable mappings
        // 2. Write the satinput file
        write_map_input(argv[1], gs, gl);
    }

    // Second Runner
    else {
        assert(read_satoutput(argv[1]));
    }
    // Return
    return 0;
}
