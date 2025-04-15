#ifndef EDGE_DETECTION_NODE_H
#define EDGE_DETECTION_NODE_H
#include "Node.h"
enum class EdgeDetectionAlgorithm { CANNY, SOBEL };
class EdgeDetectionNode : public Node {
public:
    EdgeDetectionNode(const std::string &id);
    void setInput(const cv::Mat &in) override;
    virtual void process() override;
    virtual cv::Mat getOutput() const override;
    void setAlgorithm(EdgeDetectionAlgorithm algo);
    EdgeDetectionAlgorithm getAlgorithm() const;
    void setCannyParameters(double lower, double upper, int aperture = 3);
    double getCannyLowerThreshold() const;
    double getCannyUpperThreshold() const;
    int getCannyAperture() const;
    void setSobelParameters(int kernelSize);
    int getSobelKernelSize() const;
    void setOverlay(bool flag);
    bool getOverlay() const;
private:
    cv::Mat input, output;
    EdgeDetectionAlgorithm algorithm;
    double lowerThreshold, upperThreshold;
    int apertureSize, sobelKernelSize;
    bool overlayOnOriginal;
};
#endif
