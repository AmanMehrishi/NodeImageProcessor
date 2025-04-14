#ifndef BRIGHTNESS_CONTRAST_NODE_H
#define BRIGHTNESS_CONTRAST_NODE_H

#include "Node.h"

class BrightnessContrastNode : public Node {
public:
    BrightnessContrastNode(const std::string& id)
        : Node(id), brightness(0), contrast(1.0) {}
    void setInput(const cv::Mat& in) override { input = in; }
    virtual void process() override;
    virtual cv::Mat getOutput() const override { return output; }
    void setBrightness(int b) { brightness = b; }
    void setContrast(double c) { contrast = c; }
    void reset() { brightness = 0; contrast = 1.0; }

    int getBrightness() const { return brightness; }
    double getContrast() const { return contrast; }
private:
    cv::Mat input, output;
    int brightness;   
    double contrast;  
};

#endif // BRIGHTNESS_CONTRAST_NODE_H
