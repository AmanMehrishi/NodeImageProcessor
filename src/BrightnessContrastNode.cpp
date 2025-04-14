#include "BrightnessContrastNode.h"
#include <opencv2/opencv.hpp>
#include <iostream>

void BrightnessContrastNode::process(){
    if(input.empty())
    {
        cerr<<"BrightnessContrast - No input Image"<<endl;
        return;
    }
    input.convertTo(output,-1,contrast, brightness);
    cout<< "Adjusted Brightness: " <<brightness<<" Contrast: "<<contrast<<endl;
}