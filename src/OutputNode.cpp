#include "OutputNode.h"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;


void OutputNode::process() {
    if (input.empty()) {
        cerr << "OutputNode: No input image to save." << endl;
        return;
    }
    if (cv::imwrite(outputPath, input)) {
        cout << "OutputNode: Image successfully saved to " << outputPath << endl;
    } else {
        cerr << "OutputNode: Failed to save image to " << outputPath << endl;
    }
}