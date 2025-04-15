#ifndef COLOR_CHANNEL_SPLITTER_NODE_H
#define COLOR_CHANNEL_SPLITTER_NODE_H
#include "Node.h"
#include <string>
#include <opencv2/opencv.hpp>
class ColorChannelSplitterNode : public Node {
public:
    ColorChannelSplitterNode(const std::string &id) : Node(id), grayscale(false) {}
    void setInput(const cv::Mat &in) override { input = in; }
    void process() override;
    cv::Mat getOutput() const override { return output; }
    void setGrayscale(bool flag) { grayscale = flag; }
    bool getGrayscale() const { return grayscale; }
private:
    cv::Mat input, output;
    bool grayscale;
};
#endif
