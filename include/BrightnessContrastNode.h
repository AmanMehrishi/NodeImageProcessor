#ifndef BRIGHTNESS_CONTRAST_NODE_H
#define BRIGHTNESS_CONTRAST_NODE_H

#include "Node.h"



class BrightnessContrastNode : public Node {
public:
    BrightnessContrastNode(const string& id)
        : Node(id), brightness(0), contrast(1.0) {}

    void setInput(const cv::Mat& in) { input = in; }
    cv::Mat getOutput() const { return output; }
    
    void setBrightness(int b) { brightness = b; }
    void setContrast(double c) { contrast = c; }

    virtual void process() override;

private:
    cv::Mat input, output;
    int brightness;   
    double contrast; 
};

#endif 