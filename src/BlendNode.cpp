#include "BlendNode.h"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
void BlendNode::setInput1(const cv::Mat &in) { input1 = in; }
void BlendNode::setInput2(const cv::Mat &in) { input2 = in; }
static cv::Mat blendNormal(const cv::Mat &A, const cv::Mat &B, double opacity) {
    cv::Mat res;
    cv::addWeighted(A, opacity, B, 1.0 - opacity, 0, res);
    return res;
}
static cv::Mat blendMultiply(const cv::Mat &A, const cv::Mat &B, double opacity) {
    cv::Mat mult;
    cv::multiply(A, B, mult, 1.0/255.0);
    return blendNormal(A, mult, opacity);
}
static cv::Mat blendScreen(const cv::Mat &A, const cv::Mat &B, double opacity) {
    cv::Mat screen = 255 - ((255 - A).mul(255 - B)) / 255;
    return blendNormal(A, screen, opacity);
}
static cv::Mat blendOverlay(const cv::Mat &A, const cv::Mat &B, double opacity) {
    cv::Mat overlay = A.clone();
    for (int y = 0; y < A.rows; y++){
        for (int x = 0; x < A.cols; x++){
            for (int c = 0; c < A.channels(); c++){
                uchar a = A.at<cv::Vec3b>(y,x)[c];
                uchar b = B.at<cv::Vec3b>(y,x)[c];
                float result = (a < 128) ? (2.0f * a * b / 255) : (255 - 2.0f * (255 - a) * (255 - b) / 255);
                overlay.at<cv::Vec3b>(y,x)[c] = static_cast<uchar>(result);
            }
        }
    }
    return blendNormal(A, overlay, opacity);
}
static cv::Mat blendDifference(const cv::Mat &A, const cv::Mat &B, double opacity) {
    cv::Mat diff;
    cv::absdiff(A, B, diff);
    return blendNormal(A, diff, opacity);
}
void BlendNode::process(){
    if(input1.empty() || input2.empty()) return;
    cv::Mat A, B;
    if(input1.size() != input2.size()){
        A = input1;
        cv::resize(input2, B, input1.size());
    } else {
        A = input1;
        B = input2;
    }
    cv::Mat blended;
    switch(mode) {
        case BlendMode::NORMAL: blended = blendNormal(A, B, opacity); break;
        case BlendMode::MULTIPLY: blended = blendMultiply(A, B, opacity); break;
        case BlendMode::SCREEN: blended = blendScreen(A, B, opacity); break;
        case BlendMode::OVERLAY: blended = blendOverlay(A, B, opacity); break;
        case BlendMode::DIFFERENCE: blended = blendDifference(A, B, opacity); break;
    }
    output = blended;
}
