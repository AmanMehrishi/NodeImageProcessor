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

std::string ImageInputNode::getMetadata() const {
    if (output.empty())
        return "No image loaded.";
    
    std::string meta;
    meta += "Path: " + imagePath + "\n";
    meta += "Dimensions: " + std::to_string(output.cols) + " x " + std::to_string(output.rows) + "\n";
    try {
        auto size = filesystem::file_size(imagePath);
        meta += "File Size: " + std::to_string(size) + " bytes\n";
    } catch (...) {
        meta += "File Size: N/A\n";
    }
    filesystem::path p(imagePath);
    meta += "Format: " + p.extension().string();
    return meta;
}