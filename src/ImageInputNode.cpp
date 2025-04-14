#include "ImageInputNode.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <filesystem>


using namespace std;

void ImageInputNode::process(){
    output = cv::imread(imagePath);
    if(output.empty())
    {
        cerr << "Failed to load image" << imagePath << endl;
        cin.get();
    }else {
        cout<<"Loaded Image: "<< imagePath <<endl;
        cout<<" - Dimensions: " << output.cols << " x " << output.rows << endl;
        try{
            auto fsize = filesystem::file_size(imagePath);
            cout<<" - File Size: "<<fsize<<" bytes"<<endl;
        } catch(...){
            cout<<" - File size unavailable"<<endl;
        }
    }
    filesystem::path p(imagePath);
    cout<<" - Format: "<<p.extension().string()<<endl;
}
