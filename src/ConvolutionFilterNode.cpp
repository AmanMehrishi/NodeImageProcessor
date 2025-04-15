#include "ConvolutionFilterNode.h"
#include <opencv2/imgproc.hpp>
void ConvolutionFilterNode::process(){
    if(kernel.empty()) return;
    if(preset == ConvolutionPreset::SHARPEN){
        float sharpen[9] = {0, -1, 0, -1, 5, -1, 0, -1, 0};
        kernel = cv::Mat(3,3, CV_32F, sharpen).clone();
    } else if(preset == ConvolutionPreset::EMBOSS){
        float emboss[9] = {-2, -1, 0, -1, 1, 1, 0, 1, 2};
        kernel = cv::Mat(3,3, CV_32F, emboss).clone();
    } else if(preset == ConvolutionPreset::EDGE_ENHANCE){
        float edge[9] = {0,0,0,-1,1,0,0,0,0};
        kernel = cv::Mat(3,3, CV_32F, edge).clone();
    }
    if(input.empty()) return;
    cv::filter2D(input, output, -1, kernel);
}
