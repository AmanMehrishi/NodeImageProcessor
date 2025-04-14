#include "EdgeDetectionNode.h"
#include <opencv2/opencv.hpp>
#include <iostream>

void EdgeDetectionNode::process() {
    if (input.empty()) {
        cerr << "EdgeDetectionNode: Input image is empty." << endl;
        return;
    }

    cv::Mat gray, edges;
    if (input.channels() == 3) {
        cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = input;
    }

    if (algorithm == EdgeDetectionAlgorithm::CANNY) {
        cv::Canny(gray, output, lowerThreshold, upperThreshold, apertureSize, false);
        cout << "EdgeDetectionNode: Applied Canny edge detection." << endl;
    } else if (algorithm == EdgeDetectionAlgorithm::SOBEL) {
        cv::Mat grad_x, grad_y;
        cv::Mat abs_grad_x, abs_grad_y;
        cv::Sobel(gray, grad_x, CV_16S, 1, 0, sobelKernelSize);
        cv::convertScaleAbs(grad_x, abs_grad_x);
        cv::Sobel(gray, grad_y, CV_16S, 0, 1, sobelKernelSize);
        cv::convertScaleAbs(grad_y, abs_grad_y);
        cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, output);
        cout << "EdgeDetectionNode: Applied Sobel edge detection." << endl;
    }
    if(overlayOnOriginal && input.channels() == 3)
    {
        cv::Mat edgeColor;
        cv::cvtColor(edges, edgeColor, cv::COLOR_GRAY2BGR);
        vector<cv::Mat> channels;
        cv::split(edgeColor, channels);
        channels[0] = cv::Mat::zeros(edges.size(), channels[0].type());
        channels[0] = cv::Mat::zeros(edges.size(), channels[1].type());
        cv::merge(channels, edgeColor);
        cv::addWeighted(input, 0.7, edgeColor, 0.3, 0, output);
        cout<<"Overlayed edges on original image." << endl;
    }
}
