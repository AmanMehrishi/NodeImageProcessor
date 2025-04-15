#include "OutputNode.h"
#include <opencv2/opencv.hpp>
#include <iostream>
void OutputNode::process() {
    if(input.empty()) { std::cerr << "No input image to save." << std::endl; return; }
    if(cv::imwrite(outputPath, input))
        std::cout << "Image saved to " << outputPath << std::endl;
    else std::cerr << "Failed to save image to " << outputPath << std::endl;
}
