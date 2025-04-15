#ifndef BLEND_NODE_H
#define BLEND_NODE_H
#include "Node.h"
#include <opencv2/opencv.hpp>
enum class BlendMode { NORMAL, MULTIPLY, SCREEN, OVERLAY, DIFFERENCE };
class BlendNode : public Node {
public:
    BlendNode(const std::string &id) : Node(id), opacity(1.0), mode(BlendMode::NORMAL) {}
    void setInput1(const cv::Mat &in);
    void setInput2(const cv::Mat &in);
    void process() override;
    cv::Mat getOutput() const override { return output; }
    void setOpacity(double op) { opacity = op; }
    double getOpacity() const { return opacity; }
    void setBlendMode(BlendMode m) { mode = m; }
    BlendMode getBlendMode() const { return mode; }
private:
    cv::Mat input1, input2, output;
    double opacity;
    BlendMode mode;
};
#endif
