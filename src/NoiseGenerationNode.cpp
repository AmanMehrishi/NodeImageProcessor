#include "NoiseGenerationNode.h"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
void NoiseGenerationNode::process(){
    cv::Mat noise = cv::Mat::zeros(512,512, CV_8UC1);
    cv::randu(noise, 0, 255);
    if(asDisplacement){
        cv::cvtColor(noise, output, cv::COLOR_GRAY2BGR);
    } else {
        output = noise;
    }
}
