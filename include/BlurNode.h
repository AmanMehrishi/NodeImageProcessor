#ifndef BLUR_NODE_H
#define BLUR_NODE_H

#include "Node.h"

class BlurNode : public Node {
public:
    BlurNode(const string& id) : Node(id), blurRadius(5) {}
    void setInput(const cv::Mat& in) override { input = in; }
    virtual void process() override;
    virtual cv::Mat getOutput() const override { return output; }
    void setBlurRadius(int radius) { blurRadius = radius; }
private:
    cv::Mat input, output;
    int blurRadius;  
};

#endif