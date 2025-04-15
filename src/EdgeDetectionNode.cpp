#include "EdgeDetectionNode.h"
#include <opencv2/opencv.hpp>
#include <iostream>
EdgeDetectionNode::EdgeDetectionNode(const std::string &id)
    : Node(id), algorithm(EdgeDetectionAlgorithm::CANNY), lowerThreshold(50), upperThreshold(150),
      apertureSize(3), sobelKernelSize(3), overlayOnOriginal(false) {}
void EdgeDetectionNode::setInput(const cv::Mat &in) { input = in; }
void EdgeDetectionNode::process() {
    if (input.empty()) { std::cerr << "EdgeDetectionNode: Input image is empty." << std::endl; return; }
    cv::Mat gray, edges;
    if (input.channels() == 3) cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY); else gray = input;
    if (algorithm == EdgeDetectionAlgorithm::CANNY) {
        cv::Canny(gray, edges, lowerThreshold, upperThreshold, apertureSize, false);
    } else if (algorithm == EdgeDetectionAlgorithm::SOBEL) {
        cv::Mat gradX, gradY, absGradX, absGradY;
        cv::Sobel(gray, gradX, CV_16S, 1, 0, sobelKernelSize);
        cv::convertScaleAbs(gradX, absGradX);
        cv::Sobel(gray, gradY, CV_16S, 0, 1, sobelKernelSize);
        cv::convertScaleAbs(gradY, absGradY);
        cv::addWeighted(absGradX, 0.5, absGradY, 0.5, 0, edges);
    }
    if (overlayOnOriginal && input.channels() == 3) {
        cv::Mat edgeColor;
        cv::cvtColor(edges, edgeColor, cv::COLOR_GRAY2BGR);
        std::vector<cv::Mat> channels;
        cv::split(edgeColor, channels);
        channels[0] = cv::Mat::zeros(edges.size(), channels[0].type());
        channels[1] = cv::Mat::zeros(edges.size(), channels[1].type());
        cv::merge(channels, edgeColor);
        cv::addWeighted(input, 0.7, edgeColor, 0.3, 0, output);
    } else {
        output = edges;
    }
}
cv::Mat EdgeDetectionNode::getOutput() const { return output; }
void EdgeDetectionNode::setAlgorithm(EdgeDetectionAlgorithm algo) { algorithm = algo; }
EdgeDetectionAlgorithm EdgeDetectionNode::getAlgorithm() const { return algorithm; }
void EdgeDetectionNode::setCannyParameters(double lower, double upper, int aperture) { lowerThreshold = lower; upperThreshold = upper; apertureSize = aperture; }
double EdgeDetectionNode::getCannyLowerThreshold() const { return lowerThreshold; }
double EdgeDetectionNode::getCannyUpperThreshold() const { return upperThreshold; }
int EdgeDetectionNode::getCannyAperture() const { return apertureSize; }
void EdgeDetectionNode::setSobelParameters(int kernelSize) { sobelKernelSize = kernelSize; }
int EdgeDetectionNode::getSobelKernelSize() const { return sobelKernelSize; }
void EdgeDetectionNode::setOverlay(bool flag) { overlayOnOriginal = flag; }
bool EdgeDetectionNode::getOverlay() const { return overlayOnOriginal; }
