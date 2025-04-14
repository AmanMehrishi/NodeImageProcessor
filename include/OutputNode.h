// include/OutputNode.h
#ifndef OUTPUT_NODE_H
#define OUTPUT_NODE_H

#include "Node.h"

using namespace std;


class OutputNode : public Node {
public:
    OutputNode(const string& id) : Node(id) {}

    void setInput(const cv::Mat& in) { input = in; }

    void setOutputPath(const string& path) { outputPath = path; }

    virtual void process() override;

private:
    cv::Mat input;
    string outputPath;
};


#endif