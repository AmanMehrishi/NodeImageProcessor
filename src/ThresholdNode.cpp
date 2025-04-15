#include "ThresholdNode.h"
#include <opencv2/imgproc.hpp>
void ThresholdNode::process(){
    if(input.empty()) return;
    cv::Mat gray;
    if(input.channels() == 3)
        cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
    else
        gray = input;
    if(method == ThresholdMethod::BINARY){
        cv::threshold(gray, output, threshold, maxValue, cv::THRESH_BINARY);
    } else if(method == ThresholdMethod::BINARY_INV){
        cv::threshold(gray, output, threshold, maxValue, cv::THRESH_BINARY_INV);
    } else if(method == ThresholdMethod::ADAPTIVE_MEAN){
        cv::adaptiveThreshold(gray, output, maxValue, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 11, threshold);
    } else if(method == ThresholdMethod::OTSU){
        cv::threshold(gray, output, 0, maxValue, cv::THRESH_BINARY | cv::THRESH_OTSU);
    }
}
