#ifndef COLOR_CHANNEL_SPLITTER_NODE_H
#define COLOR_CHANNEL_SPLITTER_NODE_H

#include "Node.h"
#include <vector>

class ColorChannelSplitterNode : public Node {
public:
    ColorChannelSplitterNode(const std::string& id) : Node(id), greyscale(false) {}
    void setInput(const cv::Mat& in) override { input = in; }
    virtual void process() override;
    virtual cv::Mat getOutput() const override { return output; }
    void setGreyscale(bool flag) {greyscale = flag;}
private:
    cv::Mat input, output;
    bool greyscale;
};

#endif 