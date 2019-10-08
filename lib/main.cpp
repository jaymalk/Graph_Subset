
#include <string>
#include <map>
#include <iostream>
#include <vector>
#include <cmath>

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
    // Set isolated nodes
    gs.set_isolated();
    gl.set_isolated();
    // Confirming that there are more isolated nodes in the larger graph
    assert(gs.isolated().size() <= gl.isolated().size());
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
    for(Node& n_: gs.nodes_()) {
        cout << "Node: " << n_.index_() << " -> ";
        for(auto it=n_.get_set().begin(); it!=n_.get_set().end(); it++) {
            tuple<int, int, int> m_ = *it;
            cout << get<0>(m_) << " ";
        }
        cout << "\n";
    }
    // Sorting nodes according to the no. candidates left
    sort(gs.nodes_().begin(), gs.nodes_().end(), [](Node& _1, Node& _2) {return _1.get_set().size() < _2.get_set().size();});
    // Reduce single mappings
    remove_mapped_variable(gs);
    cout << "Singular Removal\n";
    for(Node& n_: gs.nodes_()) {
        cout << "Node: " << n_.index_() << " -> ";
        for(auto it=n_.get_set().begin(); it!=n_.get_set().end(); it++) {
            tuple<int, int, int> m_ = *it;
            cout << get<0>(m_) << " ";
        }
        cout << "\n";
    }

    ofstream file;
    //.map file contains singular mappings and mappings of variables
    file.open(string(argv[1]) + ".map");
    //cout<< (file) << endl;

    cout << "Making different Maps" << endl;
    // vector of sets of variables out of which only one can be true
    vector<vector<int> > gsTogl;
    // tempprary vector to help in building of gsTogl
    vector<int> v;
    // glTogs is a map because of non contiguous entries coming
    map<int, vector<int> > glTogs;
    map<int, vector<int> >::iterator exist;

    int clause;

    //the numbers for each mapping
    int count = 1;
    if (file) {
        for (Node& n_: gs.nodes_()) {
            v.clear();
            if (n_.get_set().size() > 1) {
                for (auto it = n_.get_set().begin(); it!=n_.get_set().end(); it++) {
                    tuple<int, int, int> m_ = *it;
                    file << count << " " << n_.index_() << " " << get<0>(m_) << endl;
                    v.push_back(count);
                    exist = glTogs.find(get<0>(m_));
                    if (exist!=glTogs.end()) {
                        glTogs.at(get<0>(m_)).push_back(count);
                    }
                    else {
                        glTogs.insert(pair<int, vector<int> > (get<0>(m_), vector<int> ()));
                        glTogs.at(get<0>(m_)).push_back(count);
                    }
                    count ++;
                }
            }
            else {
                file << 0 << " " << n_.index_() << " " << get<0>(*(n_.get_set().begin())) << endl;
            }
            if (v.size() > 0) {
                gsTogl.push_back(v);
                if (v.size() == 2) {
                    clause = clause + 2;
                }
                else {
                    clause = clause + (pow(2, v.size()) - v.size() - 1);
                }
            }
        }
    }

    file.close();

    //to update the variable clause
    for(auto it = glTogs.begin(); it != glTogs.end(); it++) {
        v = it->second;
        if (v.size() < 2) {
        }
        else if (v.size() == 2) {
            clause = clause + 1;
        }
        else {
            clause = clause + (pow(2, v.size()) - v.size() - 2);
        }
    }

    vector<int> t; //as a helper vector to print subsets
    
    file.open(string(argv[1]) + ".satinput");
    count--;
    file << "p cnf " << count << " " << clause << endl;

    //printing subsets in satinput file
    for (int l = 0; l < gsTogl.size(); l++) {
        t.clear();
        for(int m = 0; m < gsTogl.at(l).size(); m++) {
            file << gsTogl.at(l).at(m) << " ";
        }
        file << 0 << endl;
        subset(2, gsTogl.at(l), t, 0, file);
    }

    for(auto it = glTogs.begin(); it != glTogs.end(); it++) {
        v = it->second;
        t.clear();
        if (v.size() != 1) {
            subset(2, v, t, 0, file);
        }
    }
    file.close();

    return 0;
}
