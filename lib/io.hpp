/*
 * Library for handelling simple I/O
 */

#ifndef __IO__
#define __IO__

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
 * @return (bool) : whether the isolated nodes can be mapped b/w the graphs
 */
bool read(string filename, Graph& gl, Graph& gs) {
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

        // Now pre-processing the graph for isolated nodes
        
        // Set isolated nodes
        gs.set_isolated();
        gl.set_isolated();

        // Adding candidates for nodes in graph g1 (all nodes of gl)
        for(Node& n_: gs.nodes_()) {
            n_.create_set(gl.nodes_());
        }

        // Return whether all the isolated nodes can be satisfied
        return gs.isolated().size() <= gl.isolated().size();
    } 
    catch(...) {
        cout << "Some error in reading the file: "+filename+" [reader.read]\n";
        return false;
    }
}

/*
 * Function for writing the graph-mappings and reductions to sat-input (FOR FIRST RUNNER)
 * @param _name: name header for files
 * @param gs : smaller graph
 * @param gl : larger graph
 */
void write_map_input(string _name, Graph& gs, Graph& gl) {
    
    //=============================================================================
    // Writing the map-file
    //=============================================================================

    // Singular mappings and varaible mappings
    ofstream _map(_name + ".map");
    // Vector of sets of possible mappings for a node
    vector<vector<int> > gs_map;
    // Map for holding the varaible names of mappings
    map<pair<int, int>, int> _var;
    // Temporary container
    vector<int> _v;
    // Map of gs-nodes for which a gl-node is a candidate
    map<int, vector<int> > gl_cands;
    // Clause count
    int _cls = 0;
    // Mapping count
    int _cnt = 1;
    // Looping on the nodes
    for (Node& n_: gs.nodes_()) {
        _v.clear();
        // If multiple possibilities  for match
        if (n_.get_set().size() > 1) {
            for (auto it = n_.get_set().begin(); it!=n_.get_set().end(); it++) {
                tuple<int, int, int> m_ = *it;
                // Setting count
                _var[pair<int, int>(n_.index_(), get<0>(m_))] = _cnt;
                // Writing count on file
                _map << _cnt << " " << n_.index_() << " " << get<0>(m_) << endl;
                // Adding count to local vector
                _v.push_back(_cnt);

                // Creating GL-Candidates
                if (gl_cands.find(get<0>(m_))!=gl_cands.end()) {
                    gl_cands.at(get<0>(m_)).push_back(n_.index_());
                }
                else {
                    gl_cands.insert(pair<int, vector<int>> (get<0>(m_), vector<int> ()));
                    gl_cands.at(get<0>(m_)).push_back(n_.index_());
                }
                
                _cnt ++;
            }
        }
        // If singly matched
        else {
            // Write on file
            _map << 0 << " " << n_.index_() << " " << get<0>(*(n_.get_set().begin())) << endl;
        }
        // For keeping the check of clauses
        if (_v.size() > 1) {
            gs_map.push_back(_v);
            _cls += (1 + _v.size()*(_v.size()-1)/2);
        }
    }
    // Closing the map file
    _map.close();

    //=============================================================================
    // Now writing the sat-clauses
    //=============================================================================

    // Sat-input file
    ofstream _satout(_name + ".satinput");
    // Writing the header
    _satout << "p cnf " << _cnt-1 << " " << _cls << "\n";

    // Writing the basic clauses
    for(vector<int>& v_: gs_map) {
        // Writing the ALL-OR clause
        for(int _c : v_)
            _satout << _c << " ";
        _satout << "0\n";
        // Writing SINGLE-TWO clauses
        for(int i=0; i<v_.size(); i++)
            for(int j=i+1; j<v_.size(); j++)
                _satout << "-" << v_[i] << " -" << v_[j] << " 0\n";
    }

    // Working out edge consistency on the smaller graph
    for(auto& p_: gs.edges()) {
        Node _1 = gs.get_node(p_.first), _2 = gs.get_node(p_.second);
        for(auto it1 = _1.get_set().begin(); it1!=_1.get_set().end(); it1++) {
            tuple<int, int, int> _t1 = *it1;
            for(auto it2 = _2.get_set().begin(); it2!=_2.get_set().end(); it2++) {
                tuple<int, int, int> _t2 = *it2;
                if(!gl.contains_edge(get<0>(_t1), get<0>(_t2))) {
                    _satout << "-" << _var[pair<int, int>(_1.index_(), get<0>(_t1))] << " -" << _var[pair<int, int>(_2.index_(), get<0>(_t2))] << " 0\n";
                }
            }
        }
    }

    // Working almost same with the larger graph
    for(auto& p_ : gl.edges()) {
        vector<int> _cand1 = gl_cands[p_.first], _cand2 = gl_cands[p_.second];
        for(int i : _cand1)
            for(int j : _cand2) {
                if(!gs.contains_edge(i, j)) {
                    _satout << "-" << _var[pair<int, int>(i, p_.first)] << " -" << _var[pair<int, int>(j, p_.second)] << " 0\n";
                }
            }
    }

    // Writing the clauses for GL uniqueness
    vector<int> _temp;
    for(auto _i : gl_cands) {
        _temp = _i.second;
        for(int i=0; i<_temp.size(); i++)
            for(int j=i+1; j<_temp.size(); j++)
                _satout << "-" << _var[pair<int, int>(_temp[i], _i.first)] << " -" << _var[pair<int, int>(_temp[j], _i.first)] << " 0\n";
    }

    // Closing the sat-output file
    _satout.close();

}


/*
 * Function for reading and processing the sat-output (and pre-matched index) for preparing the matchings
 * It also writes to the mapping file right away...
 * @param _name : header name for the files
 */
bool read_satoutput(string _name) {
    // Opening the files for input
    ifstream _sat(_name+".satoutput");
    ifstream _premap(_name+".map");
    // Output file
    ofstream _map(_name+".mapping");
    // String container
    string out_string;
    // Reading the first line of SAT-output
    getline(_sat, out_string);
    // Analzysing the result
    if(out_string != "SAT") return false;
    
    // Reading the satinput
    vector<int> _sat_result;
    // Reading the result-line
    getline(_sat, out_string);
    istringstream _line(out_string);
    int _v, _m, _s, _l;
    while(_line >> _v) {
        if(!_v) break;
        _sat_result.push_back(_v);
    }
    // Using sat result on mappings
    for(auto itr = _sat_result.begin(); itr != _sat_result.end(); itr++) {
        _v = *itr;
        if(_v < 0) continue;
        while(getline(_premap, out_string)) {
            _line = istringstream(out_string);
            _line >> _m;
            if(_m == 0) {
                _line >> _s; _line >> _l;
                _map << _s << " " << _l << "\n";
            }
            else if (_m != _v) continue;
            else {
                _line >> _s; _line >> _l;
                _map << _s << " " << _l << "\n";
                break;
            }
        }
    }
    // If some of the mapping is still left
    while(getline(_premap, out_string)) {
        _line = istringstream(out_string);
        _line >> _m;
        if(_m == 0) {
            _line >> _s; _line >> _l;
            _map << _s << " " << _l << "\n";
        }
    }
    // Closing the files
    _sat.close();
    _premap.close();
    _map.close();

    // Returning that eeverything went ok
    return true;
}

#endif