#ifndef THRESHOLD_NODE_H
#define THRESHOLD_NODE_H
#include "Node.h"
#include <opencv2/opencv.hpp>
enum class ThresholdMethod { BINARY, BINARY_INV, ADAPTIVE_MEAN, OTSU };
class ThresholdNode : public Node {
public:
    ThresholdNode(const std::string &id) : Node(id), threshold(128), maxValue(255), method(ThresholdMethod::BINARY) {}
    void setInput(const cv::Mat &in) override { input = in; }
    void process() override;
    cv::Mat getOutput() const override { return output; }
    void setThreshold(double t) { threshold = t; }
    double getThreshold() const { return threshold; }
    void setMaxValue(double m) { maxValue = m; }
    double getMaxValue() const { return maxValue; }
    void setMethod(ThresholdMethod mtd) { method = mtd; }
    ThresholdMethod getMethod() const { return method; }
private:
    cv::Mat input, output;
    double threshold, maxValue;
    ThresholdMethod method;
};
#endif
