#ifndef CONVOLUTION_FILTER_NODE_H
#define CONVOLUTION_FILTER_NODE_H
#include "Node.h"
#include <opencv2/opencv.hpp>
enum class ConvolutionPreset { CUSTOM, SHARPEN, EMBOSS, EDGE_ENHANCE };
class ConvolutionFilterNode : public Node {
public:
    ConvolutionFilterNode(const std::string &id) : Node(id), preset(ConvolutionPreset::CUSTOM) {
        kernel = cv::Mat::ones(3,3, CV_32F)/9.0f;
    }
    void process() override;
    cv::Mat getOutput() const override { return output; }
    void setKernel(const cv::Mat &k) { kernel = k.clone(); }
    cv::Mat getKernel() const { return kernel; }
    void setPreset(ConvolutionPreset p) { preset = p; }
    ConvolutionPreset getPreset() const { return preset; }
private:
    cv::Mat kernel;
    cv::Mat input;
    cv::Mat output;
    ConvolutionPreset preset;
};
#endif
