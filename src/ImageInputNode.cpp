#include "ImageInputNode.h"
#include <opencv2/opencv.hpp>
#include <iostream>


using namespace std;

void ImageInputNode::process(){
    output = cv::imread(imagePath);
    if(output.empty())
    {
        cerr << "Failed to load image" << imagePath << endl;
        cin.get();
    }else {
        cout<<"Loaded Image: "<< imagePath <<endl;
    }
}
