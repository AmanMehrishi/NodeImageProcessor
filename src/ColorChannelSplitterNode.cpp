#include "ColorChannelSplitterNode.h"
#include <opencv2/imgproc.hpp>
#include <vector>
using namespace std;
void ColorChannelSplitterNode::process(){
    if(input.empty()) return;
    vector<cv::Mat> channels;
    cv::split(input, channels);
    for(auto &ch : channels){
        if(!grayscale){
            cv::cvtColor(ch, ch, cv::COLOR_GRAY2BGR);
        }
    }
    if(!channels.empty()){
        cv::hconcat(channels, output);
    }
}
