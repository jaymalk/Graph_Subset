/*
 * Library for reductions on the graph linking CSP
 */

#include <set>
#include <tuple>
#include <vector>
#include <fstream>

#include "graph.hpp"

#ifndef __REDUCE__
#define __REDUCE__


/*
 * Reduction using the idea that incoming and outgoing degree of a match must be at least greater than that of the node.
 */
void unary_reduction(Graph& g1) {
    // Looping in the nodes of g1
    for(Node& n_: g1.nodes_()) {
        // Getting the present node incoming and outgoing degrees
        int _fs = n_.edges(false).size(), _ts = n_.edges(true).size();
        // Getting the candidate set
        set<tuple<int, int, int>>& _cand = n_.get_set();
        // Looping over all candidates
        set<tuple<int, int, int>>::iterator iter = _cand.begin(), crnt;
        while(iter != _cand.end()) {
            tuple<int, int, int> m_ = *iter;
            crnt = iter++;
            // Checking if candidate is valid
            if(get<2>(m_) < _fs || get<1>(m_) < _ts)
                // Remove if not 
                _cand.erase(crnt);
        }
    }
}

/*
 * A minimal attempt at arc-consistency
 * Runs arc-consistency multiple times in order until no changes are observed
 * @return (bool) whether there was a change observed or not
 */
bool secondary_reduction(Graph& g1, Graph& g2) {
    // Local variables
    bool _valid, change = false;
    // Looping on the nodes of g1
    for(Node& n_: g1.nodes_()) {
        // Getting the candidates set
        set<tuple<int, int, int>>& _cand1 = n_.get_set();
        // Looping on all the neighbours of n_ (outgoing edge from n_ to m_)
        for(int _m: n_.edges()) {
            // Getting the candidates set
            set<tuple<int, int, int>>& _cand2 = g1.get_node(_m).get_set();
            // Now we test all candidates of n_
            set<tuple<int, int, int>>::iterator itr = _cand1.begin(), crnt, itr2;
            while(itr != _cand1.end()) {
                _valid = false;
                tuple<int, int, int> _1 = *itr;
                crnt = itr++;
                itr2 = _cand2.begin();
                while(itr2 != _cand2.end()) {
                    tuple<int, int, int> _2 = *itr2;
                    itr2++;
                    if(g2.contains_edge(get<0>(_1), get<0>(_2))) {
                        _valid = true;
                        break;
                    }
                }
                if(!_valid) {
                    _cand1.erase(crnt);
                    change = true;
                }
            }
        }
        // Looping on all the neighbours of n_ (incoming edge from m_ to n_)
        for(int _m: n_.edges(true)) {
            // Getting the candidates set
            set<tuple<int, int, int>>& _cand2 = g1.get_node(_m).get_set();
            // Now we test all candidates of n_
            set<tuple<int, int, int>>::iterator itr = _cand1.begin(), crnt, itr2;
            while(itr != _cand1.end()) {
                _valid = false;
                tuple<int, int, int> _1 = *itr;
                crnt = itr++;
                itr2 = _cand2.begin();
                while(itr2 != _cand2.end()) {
                    tuple<int, int, int> _2 = *itr2;
                    itr2++;
                    if(g2.contains_edge(get<0>(_2), get<0>(_1))) {
                        _valid = true;
                        break;
                    }
                }
                if(!_valid) {
                    _cand1.erase(crnt);
                    change = true;
                }
            }
        }
    }
    return change;
}


/*
 * Removing all the candidates which have been assigned
 */
void remove_mapped_variable(Graph& g) {
    // Looping on the nodes of g1
    for(Node& n_: g.nodes_()) {
        // Getting the candidates set
        set<tuple<int, int, int>>& _cand1 = n_.get_set();
        // Return if not signled out
        if(_cand1.size() > 1) return;
        // The current value
        tuple<int, int, int> _val = *(_cand1.begin());
        // Looping on all the nodes
        for(Node& m_ : g.nodes_()) {
            // Skipping the same node
            if(m_.index_() == n_.index_()) continue;
            // Getting the candidate set of other
            set<tuple<int, int, int>>& _cand2 = m_.get_set();
            // Removing if it exists
            if(_cand2.find(_val) != _cand2.end())
                _cand2.erase(_val);
        }
    }
}

/*
 * If a set is of size n, this function prints all subsets of size k to n-1 in the file
 */
void subset(int k, vector<int> set, vector<int> temp, int index, std::ofstream& file) {
    // if set is empty, then nothing is possible
    if (set.size() == 0) {
    }
    else if (k == (set.size() - index)) {
        for (int i = 0; i < temp.size(); i++) {
            file << "-" << (temp[i]) << " ";
        }
        for (int j = index; j < set.size(); j++) {
            file << "-" << set[j] << " ";
        }
        file << 0 << endl;
    }
    else {
        subset(k, set, temp, index + 1, file);
        if (index < set.size()) {
            temp.push_back(set[index]);
            set.erase(set.begin());
            subset(k-1, set, temp, index, file);
        }

    }
}
#endif
