#ifndef NODE_GRAPH_H
#define NODE_GRAPH_H

#include "Node.h"
#include <vector>
#include <memory>

using namespace std;

class NodeGraph {
public:
    struct Connection {
        shared_ptr<Node> source;
        shared_ptr<Node> target;
    };

    void addNode(shared_ptr<Node> node) {
        nodes.push_back(node);
    }

    void connect(shared_ptr<Node> source, shared_ptr<Node> target) {
        connections.push_back({source, target});
    }

    void processAll() {
        for (auto &node : nodes) {
            for (auto &conn : connections) {
                if (conn.target == node) {
                    node->setInput(conn.source->getOutput());
                }
            }
            node->process();
        }
    }

private:
    vector<shared_ptr<Node>> nodes;
    vector<Connection> connections;
};

#endif 