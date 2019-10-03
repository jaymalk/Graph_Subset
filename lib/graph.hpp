/*
 * Graph Library
 * Contains 'Graph' class 
 */

#ifndef _GRAPH_
#define _GRAPH_

#include <iostream>
#include <vector>
#include <set>

using namespace std;


/*
* Node Class
*/
class Node {

// private
private:
    int index;
    vector<Node> in_edges, out_edges;
    set<Node> candidates;

// public
public:
    /*
     * Constructor
     */
    Node(int index) {
        this->index = index;
        this->in_edges = vector<Node>();
        this->out_edges = vector<Node>();
    }
    /*
     * Get Index 
     */
    int index_() {
        return index;
    }
    /*
     * Adding edge for the node (whether incoming or outgoing)
     */
    void addEdge(Node& other, bool incoming) {
        if (incoming) {
            in_edges.push_back(other);
        }
        else {
            out_edges.push_back(other);
        }
    }

    /*
     * Edges: returning the node list for incoming and outgoing edgees
     */
    vector<Node>& edges(bool _in = false) {
        if(_in) return in_edges;
        return out_edges;
    }

    /*
     * Create set
     */
    void create_set(vector<Node>& _nodes) {
        this->candidates = set<Node>(_nodes.begin(), _nodes.end());
    }

    /*
     * Getting the candidate set
     */
    set<Node>& get_set() {
        return this->candidates;
    }

    /*
     * Comparison function for Nodes
     */
    bool operator<(Node other) const {
        return index < other.index_();
    }
};


/*
 * Graph Class
 * Class for representing a directed graph
 */
class Graph {

// Private 
private:

    // Containers for the graph
    vector<Node> nodes;

// Public
public:

    /*
     * Constructor
     */
    Graph() {
        this->nodes = vector<Node>();
    }

    /*
     * Contains node: checking if a node exists
     */
    bool contains_node(int index) {
        for(Node& _n : nodes)
            if(_n.index_() == index)
                return true;
        return false;
    }

    /*
     * Get Node : Getting the node with a certain index
     */
    Node& get_node(int index) {
        for(Node& _n : nodes)
            if(_n.index_() == index)
                return _n;
        throw new runtime_error("Node note found.");
    }

    /*
     * Node Vector : Getting the nodes vector
     */
    vector<Node>& nodes_() {
        return nodes;
    }

    /*
     * Edge processing
     */
    void addEdge(int a, int b) {
        // Processing node with index 'a'
        // Adding if it doesnt exist
        if(!contains_node(a)) {
            nodes.push_back(Node(a));
        }
        // Processing node with index 'b'
        // Adding if it doesnt exist
        if(!contains_node(b)) {
            nodes.push_back(Node(b));
        }
        // Getting the node
        Node& a_ = get_node(a);
        // Getting the node
        Node& b_ = get_node(b);
        // Adding edges in the node
        b_.addEdge(a_, true);
        a_.addEdge(b_, false);
    }

    void __print() {
        cerr << flush << "Starting\n";
        for(Node& n_ : nodes) {
            for(Node& m_ :n_.edges(true)) 
                cout << n_.index_() << " <- " << m_.index_() << "\n";
            for(Node& m_ :n_.edges(false)) 
                cout << n_.index_() << " -> " << m_.index_() << "\n";
        }
    }
};

#endif