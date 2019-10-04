/*
 * Library for handelling simple I/O
 */

#ifndef __READ__
#define __READ__

#include "graph.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

/*
 * Function for reading and encoding the graphs
 * @param filename (string) : the local path of the graph file
 * @param gl (Graph&) : the graph g' (larger graph)
 * @param gs (Graph&) : the graph g  (smaller graph, which is to be mapped)
 */
void read(string filename, Graph& gl, Graph& gs) {
    try {
        string line; // Line buffer
        int v1, v2; // Vertex buffer
        // Creating an ifstream
        ifstream input(filename);
        // Reading the first graph
        while(getline(input, line)) {
            // Line reader
            istringstream l_(line);
            // Reading vertex values
            l_ >> v1;
            l_ >> v2;
            // Break if graph is complete
            if((v1 == 0) && (v2 == 0)) break;
            // Adding edge if valid
            gl.add_edge(v1, v2);
        }
        // Reading the second graph
        while(getline(input, line)) {
            if(line == "") break;
            // Line reader
            istringstream l_(line);
            // Reading vertex values
            l_ >> v1;
            l_ >> v2;
            // Adding edge if valid
            gs.add_edge(v1, v2);
        }
        // Closing the stream
        input.close();
    } 
    catch(...) {
        cout << "Some error in reading the file: "+filename+" [reader.read]\n";
    }
}

#endif