#include "ColorChannelSplitterNode.h"
#include <opencv2/opencv.hpp>
#include <iostream>

void ColorChannelSplitterNode::process(){
    if(input.empty()){
        cerr<<"ColorChannelSplitterNode: No input image." <<endl;
        return; 
    }
    vector<cv::Mat> channels;
    cv::split(input, channels);

    vector<cv::Mat> channelImages;
    for(auto& ch:channels)
    {
        if(!greyscale && input.channels() == 3)
        {
            cv::Mat color;
            cv::cvtColor(ch, color, cv::COLOR_GRAY2BGR);
            ch = color;
        }
    }
    if(!channels.empty())
    {
        cv::hconcat(channels, output);
    }
    
    cout<<"ColorChannelSplit: Successfully split images into"<< channels.size() << " channels"<< endl;
}