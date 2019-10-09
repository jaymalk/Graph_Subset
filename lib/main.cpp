
#include <string>
#include <map>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

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
    // assert(gs.isolated().size() <= gl.isolated().size());
    cout << gs.isolated().size() << "\n";
    cout << "Graph Created\n";
    // Adding candidates for nodes in graph g1 (all nodes of gl)
    for(Node& n_: gs.nodes_()) {
        n_.create_set(gl.nodes_());
    }
    cout << "Set Created\nUnary\n";
    // Starting with reductions
    // First reduction (based on degrees)
    unary_reduction(gs);
    // for(Node& n_: gs.nodes_()) {
    //     cout << "Node: " << n_.index_() << " -> ";
    //     for(auto it=n_.get_set().begin(); it!=n_.get_set().end(); it++) {
    //         tuple<int, int, int> m_ = *it;
    //          cout << get<0>(m_) << " ";
    //            }
    //     cout << "\n";
    // }
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
    // Sorting to keep the nodes in order
    sort(gs.nodes_().begin(), gs.nodes_().end(), [](Node& _1, Node& _2){return _1.index_() < _2.index_();});

    // Singular mappings and varaible mappings
    ofstream _map(string(argv[1]) + ".map");

    cout << "Making different Maps" << endl;
    // vector of sets of variables out of which only one can be true
    vector<vector<int> > gsTogl;
    // Map for holding the varaible names of mappings
    map<pair<int, int>, int> _var;
    // tempprary vector to help in building of gsTogl
    vector<int> v;
    // glTogs is a map because of non contiguous entries coming
    map<int, vector<int> > gl_cands;

    // Clause count
    int _cls = 0;

    // Mapping count
    int _cnt = 1;
    // Looping on the nodes
    for (Node& n_: gs.nodes_()) {
        v.clear();
        // If multiple possibilities  for match
        if (n_.get_set().size() > 1) {
            for (auto it = n_.get_set().begin(); it!=n_.get_set().end(); it++) {
                tuple<int, int, int> m_ = *it;
                // Setting count
                _var[pair<int, int>(n_.index_(), get<0>(m_))] = _cnt;
                // Writing count on file
                _map << _cnt << " " << n_.index_() << " " << get<0>(m_) << endl;
                // Adding count to local vector
                v.push_back(_cnt);

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
        if (v.size() > 1) {
            gsTogl.push_back(v);
            _cls += (1 + v.size()*(v.size()-1)/2);
        }
    }
        
    //to update the variable clause
    // for(auto it = glTogs.begin(); it != glTogs.end(); it++) {
    //     v = it->second;
    //     if (v.size() < 2) {
    //     }
    //     else if (v.size() == 2) {
    //         clause = clause + 1;
    //     }
    //     else {
    //         clause = clause + (pow(2, v.size()) - v.size() - 2);
    //     }
    // }

    // Sat-input file
    ofstream _satout(string(argv[1]) + ".satinput");

    // Writing the header
    _satout << "p cnf " << _cnt-- << " " << _cls << "\n";

    // Writing the basic clauses
    for(vector<int>& v_: gsTogl) {
        // Writing the ALL-OR clause
        for(int _c : v_)
            _satout << _c << " ";
        _satout << "0\n";
        // Writing SINGLE-TWO clauses
        for(int i=0; i<v_.size(); i++)
            for(int j=i+1; j<v_.size(); j++)
                _satout << "-" << v_[i] << " -" << v_[j] << " 0\n";
    }

    // Writing the clauses for GL uniqueness
    vector<int> _temp;
    for(auto _i : gl_cands) {
        _temp = _i.second;
        for(int i=0; i<_temp.size(); i++)
            for(int j=i+1; j<_temp.size(); j++)
                _satout << "-" << _var[pair<int, int>(_temp[i], _i.first)] << " -" << _var[pair<int, int>(_temp[j], _i.first)] << " 0\n";
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

    return 0;
}
