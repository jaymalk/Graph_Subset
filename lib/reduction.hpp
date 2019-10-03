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

#endif