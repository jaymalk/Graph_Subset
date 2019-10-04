/*
 * Library for reductions on the graph linking CSP
 */

#include <set>
#include <vector>

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
        set<Node>& _cand = n_.get_set();
        // Looping over all candidates
        set<Node>::iterator iter = _cand.begin(), crnt;
        while(iter != _cand.end()) {
            Node m_ = *iter;
            crnt = iter++;
            // Checking if candidate is valid
            if(m_.edges(false).size() < _fs || m_.edges(true).size() < _ts)
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
        set<Node>& _cand1 = n_.get_set();
        // Looping on all the neighbours of n_ (outgoing edge from n_ to m_)
        for(int _m: n_.edges()) {
            // Getting the candidates set
            set<Node>& _cand2 = g1.get_node(_m).get_set();
            // Now we test all candidates of n_
            set<Node>::iterator itr = _cand1.begin(), crnt, itr2;
            while(itr != _cand1.end()) {
                _valid = false;
                Node _1 = *itr;
                crnt = itr++;
                itr2 = _cand2.begin();
                while(itr2 != _cand2.end()) {
                    Node _2 = *itr2;
                    itr2++;
                    if(g2.contains_edge(_1.index_(), _2.index_())) {
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
            set<Node>& _cand2 = g1.get_node(_m).get_set();
            // Now we test all candidates of n_
            set<Node>::iterator itr = _cand1.begin(), crnt, itr2;
            while(itr != _cand1.end()) {
                _valid = false;
                Node _1 = *itr;
                crnt = itr++;
                itr2 = _cand2.begin();
                while(itr2 != _cand2.end()) {
                    Node _2 = *itr2;
                    itr2++;
                    if(g2.contains_edge(_2.index_(), _1.index_())) {
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

#endif