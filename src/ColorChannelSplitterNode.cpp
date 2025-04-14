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
        cv::Mat color;
        cv::cvtColor(ch, color, cv::COLOR_GRAY2BGR);
        channelImages.push_back(color);
    }
    if(channelImages.size() > 1)
        cv::hconcat(channelImages,output);
    else
        output = channelImages[0];
    
    cout<<"ColorChannelSplit: Successfully split images into"<< channels.size() << " channels"<< endl;
}