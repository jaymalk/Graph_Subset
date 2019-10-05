
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
    cout << "Graph Created\n";
    // Adding candidates for nodes in graph g1 (all nodes of gl)
    for(Node& n_: gs.nodes_()) {
        n_.create_set(gl.nodes_());
    }
    cout << "Set Created\nUnary\n";
    // Starting with reductions
    // First reduction (based on degrees)
    unary_reduction(gs);
    for(Node& n_: gs.nodes_()) {
        cout << "Node: " << n_.index_() << " -> ";
        for(auto it=n_.get_set().begin(); it!=n_.get_set().end(); it++) {
            tuple<int, int, int> m_ = *it;
            cout << get<0>(m_) << " ";
        }
        cout << "\n";
    }
    // Secondary reduction (based on arc consitency)
    cout << "Binary\n";
    while(secondary_reduction(gs, gl));
    // TODO
    for(Node& n_: gs.nodes_()) {
        cout << "Node: " << n_.index_() << " -> ";
        for(auto it=n_.get_set().begin(); it!=n_.get_set().end(); it++) {
            tuple<int, int, int> m_ = *it;
            cout << get<0>(m_) << " ";
        }
        cout << "\n";
    }
    return 0;
}
