/*
 * Graph Library
 * Contains 'Graph' class 
 */

#include <iostream>
#include <vector>

using namespace std;


/*
 * Graph Class
 * Class for representing a directed graph
 */
class Graph {

// Private 
private:

    // Declaration
    class Edge;
    class Node;

    /*
    * Node Class (Private)
    */
    class Node {
    
    // private
    private:
        int index;
        vector<Edge> in_edges, out_edges;
    
    // public
    public:
        /*
         * Constructor
         */
        Node(int index) {
            this->index = index;
            this->in_edges = vector<Edge>();
            this->out_edges = vector<Edge>();
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
        void addEdge(Edge edge, bool incoming) {
            if (incoming)
                in_edges.push_back(edge);
            else
                out_edges.push_back(edge);
        }
    };

    /*
     * Edge Class (Private)
     */
    class Edge {
    
    // Private
    private:
        Node *source, *end;
        
    // Public
    public:
        /*
         * Constructor
         */
        Edge(Node* a, Node* b) {
            a->addEdge(*this, false);
            b->addEdge(*this, true);
            this->source = a;
            this->end = b;
        }
    };

    // Containers for the graph
    vector<Node> nodes;
    vector<Edge> edges;

// Public
public:

    /*
     * Constructor
     */
    Graph() {
        this->edges = vector<Edge>();
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
        edges.push_back(Edge(a_, b_));
    }
};