#ifndef OUTPUT_NODE_H
#define OUTPUT_NODE_H
#include "Node.h"
#include <string>
class OutputNode : public Node {
public:
    OutputNode(const std::string &id) : Node(id) {}
    void setInput(const cv::Mat &in) override { input = in; }
    void setOutputPath(const std::string &path) { outputPath = path; }
    virtual void process() override;
    virtual cv::Mat getOutput() const override { return input; }
private:
    cv::Mat input;
    std::string outputPath;
};
#endif
