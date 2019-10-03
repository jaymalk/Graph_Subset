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
    // index of the node
    int index;
    // in and out edges if the node
    vector<Node> in_edges, out_edges;
    // the candidate set
    set<Node> candidates;

// public
public:
    /*
     * Constructor
     * @param index (int) : index of the node
     */
    Node(int index) {
        this->index = index;
        this->in_edges = vector<Node>();
        this->out_edges = vector<Node>();
    }
    /*
     * Return the index of the node
     * @return index (int)
     */
    int index_() {
        return index;
    }
    /*
     * Adding edge for the node (whether incoming or outgoing)
     * @param other (Node&) : other node in the edge
     * @param incoming (bool) : whether the edge is incoming or outgoing
     */
    void add_edge(Node& other, bool incoming) {
        if (incoming) {
            in_edges.push_back(other);
        }
        else {
            out_edges.push_back(other);
        }
    }

    /*
     * Get all the edges (incoming or outgoing) of the node
     * @param _in (bool default:false) : whether incoming or outgoing edges are needed
     * @return in/out _edges (vector<Node>&)
     */
    vector<Node>& edges(bool _in = false) {
        if(_in) return in_edges;
        return out_edges;
    }

    /*
     * Create the candidate set for mapping
     * @param _nodes (vector<Node>&) : vector of all the suitable nodes
     */
    void create_set(vector<Node>& _nodes) {
        this->candidates = set<Node>(_nodes.begin(), _nodes.end());
    }

    /*
     * Getting the candidate set of the node
     * @return candidates (set<Node>&)
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

    // The vector containing all the nodes in the graph
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
     * Check whether there is a node with the given index in the graph
     * @param index (int) : marker for the node to be found
     * @return (bool) whether the node is or is not in the graph
     */
    bool contains_node(int index) {
        for(Node& _n : nodes)
            if(_n.index_() == index)
                return true;
        return false;
    }

    /*
     * Get the node from the graph (assumes the node is there)
     * @param index (int) : marker for the node to be found
     * @return (Node&) the node reference
     * @throws [runtime_error] if node doesn't exist in the graph
     * 
     * Note: 
     * This function must be called only after confirming the presence of the node after calling [contains_node]
     */
    Node& get_node(int index) {
        for(Node& _n : nodes)
            if(_n.index_() == index)
                return _n;
        throw new runtime_error("Node note found.");
    }

    /*
     * Get all the nodes in the graph
     * @return nodes_ (vector<Node>&)
     */
    vector<Node>& nodes_() {
        return nodes;
    }

    /*
     * Adding an edge in the graph, adds an edge from the node with index a to the node with index b (a -> b)
     * @param a, b (int) : indices of the nodes involved
     */
    void add_edge(int a, int b) {
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
        b_.add_edge(a_, true);
        a_.add_edge(b_, false);
    }
};

#endif