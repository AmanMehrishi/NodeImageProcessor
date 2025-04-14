#ifndef THRESHOLD_NODE_H
#define THRESHOLD_NODE_H

#include "Node.h"

enum class ThresholdMethod {
    BINARY,
    ADAPTIVE,
    OTSU
};

class ThresholdNode : public Node {
public:
    ThresholdNode(const string& id)
        : Node(id), thresholdValue(128), method(ThresholdMethod::BINARY) {}
    void setInput(const cv::Mat& in) override { input = in; }
    virtual void process() override;
    virtual cv::Mat getOutput() const override { return output; }
    void setThresholdValue(double value) { thresholdValue = value; }
    void setMethod(ThresholdMethod m) { method = m; }
    cv::Mat getHistogramPreview() const { return histogramImage; }
private:
    cv::Mat input, output, histogramImage;
    double thresholdValue;
    ThresholdMethod method;
};

#endif