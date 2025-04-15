#ifndef NODE_GRAPH_H
#define NODE_GRAPH_H

#include "Node.h"
#include <vector>
#include <memory>
#include <stdexcept>

class NodeGraph {
public:
    struct Connection {
        std::shared_ptr<Node> source;
        std::shared_ptr<Node> target;
    };

    void addNode(std::shared_ptr<Node> node) {
        nodes.push_back(node);
    }

    void connect(std::shared_ptr<Node> source, std::shared_ptr<Node> target) {
        if (!source || !target)
            throw std::runtime_error("Invalid connection");
        connections.push_back({source, target});
    }

    void processAll() {
        for (auto &conn : connections) {
            cv::Mat out = conn.source->getOutput();
            if (out.empty())
                continue;
            conn.target->setInput(out);
            conn.target->process();
        }
    }

    const std::vector<Connection>& getConnections() const {
        return connections;
    }

private:
    std::vector<std::shared_ptr<Node>> nodes;
    std::vector<Connection> connections;
};

#endif // NODE_GRAPH_H
