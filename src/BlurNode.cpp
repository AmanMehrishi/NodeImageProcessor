#include "BlurNode.h"
#include <opencv2/imgproc.hpp>
void BlurNode::process(){
    if(input.empty()) return;
    if(!directional){
        int ksize = radius*2+1;
        cv::GaussianBlur(input, output, cv::Size(ksize, ksize), 0);
    } else {
        int ksize = radius*2+1;
        cv::GaussianBlur(input, output, cv::Size(ksize, 1), 0);
    }
}
