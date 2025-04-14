#include "BlurNode.h"
#include <opencv2/opencv.hpp>
#include <iostream>

void BlurNode::process() {
    if (input.empty()) {
        std::cerr << "BlurNode: No input image." << std::endl;
        return;
    }
    int ksize = blurRadius;
    if (ksize % 2 == 0) ksize++;  
    cv::GaussianBlur(input, output, cv::Size(ksize, ksize), 0);
    std::cout << "BlurNode: Applied Gaussian blur with radius " << blurRadius << "." << std::endl;
}