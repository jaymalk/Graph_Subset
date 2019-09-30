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
 * Graph Class
 * Class for representing a directed graph
 */
class Graph {

// Private 
private:

    /*
    * Node Class (Private)
    */
    class Node {
    
    // private
    private:
        int index;
        vector<Node> in_edges, out_edges;
    
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
            if (incoming)
                in_edges.push_back(other);
            else
                out_edges.push_back(other);
        }
    };

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
     * Get Node : Getting the node with a certain index
     */
    Node* getNode(int index) {
        for(Node& _n : nodes)
            if(_n.index_() == index)
                return &_n;
        return NULL;
    }

    /*
     * Edge processing
     */
    void addEdge(int a, int b) {
        // Processing node with index 'a'
        // Getting if it exists
        Node* a_ = getNode(a);
        // Creating & adding if it doesnt exist
        if (a_ == NULL) {
            a_ = new Node(a);
            nodes.push_back(*a_);
        }
        // Processing node with index 'b'
        // Getting if it exists
        Node* b_ = getNode(b);
        // Creating & adding if it doesnt exist
        if (b_ == NULL) {
            b_ = new Node(b);
            nodes.push_back(*b_);
        }
        // Adding edge
        a_->addEdge(*b_, false);
        b_->addEdge(*a_, true);
    }
};

#endif