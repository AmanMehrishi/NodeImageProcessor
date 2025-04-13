#include "EdgeDetectionNode.h"
#include <opencv2/opencv.hpp>
#include <iostream>

void EdgeDetectionNode::process() {
    if (input.empty()) {
        std::cerr << "EdgeDetectionNode: Input image is empty." << std::endl;
        return;
    }

    cv::Mat gray;
    if (input.channels() == 3) {
        cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = input;
    }

    if (algorithm == EdgeDetectionAlgorithm::CANNY) {
        cv::Canny(gray, output, lowerThreshold, upperThreshold, apertureSize, false);
        std::cout << "EdgeDetectionNode: Applied Canny edge detection." << std::endl;
    } else if (algorithm == EdgeDetectionAlgorithm::SOBEL) {
        cv::Mat grad_x, grad_y;
        cv::Mat abs_grad_x, abs_grad_y;
        cv::Sobel(gray, grad_x, CV_16S, 1, 0, sobelKernelSize);
        cv::convertScaleAbs(grad_x, abs_grad_x);
        cv::Sobel(gray, grad_y, CV_16S, 0, 1, sobelKernelSize);
        cv::convertScaleAbs(grad_y, abs_grad_y);
        cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, output);
        std::cout << "EdgeDetectionNode: Applied Sobel edge detection." << std::endl;
    }
}
