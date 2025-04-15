#ifndef BLUR_NODE_H
#define BLUR_NODE_H
#include "Node.h"
#include <opencv2/opencv.hpp>
class BlurNode : public Node {
public:
    BlurNode(const std::string &id) : Node(id), radius(5), directional(false) {}
    void setInput(const cv::Mat &in) override { input = in; }
    void process() override;
    cv::Mat getOutput() const override { return output; }
    void setRadius(int r) { radius = r; }
    int getRadius() const { return radius; }
    void setDirectional(bool flag) { directional = flag; }
    bool getDirectional() const { return directional; }
private:
    cv::Mat input, output;
    int radius;
    bool directional;
};
#endif
