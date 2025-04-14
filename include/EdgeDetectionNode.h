#ifndef EDGE_DETECTION_NODE_H
#define EDGE_DETECTION_NODE_H

#include "Node.h"

enum class EdgeDetectionAlgorithm{
    CANNY,
    SOBEL
};

class EdgeDetectionNode : public Node {
    public:
        EdgeDetectionNode(const std::string& id)
            : Node(id), algorithm(EdgeDetectionAlgorithm::CANNY),
              lowerThreshold(50), upperThreshold(150), apertureSize(3),
              sobelKernelSize(3) {}
    
        void setInput(const cv::Mat& in) { input = in; }
    
        cv::Mat getOutput() const { return output; }
    
        void setAlgorithm(EdgeDetectionAlgorithm algo) { algorithm = algo; }
    
        void setCannyParameters(double lower, double upper, int aperture = 3) {
            lowerThreshold = lower;
            upperThreshold = upper;
            apertureSize = aperture;
        }
    
        void setSobelParameters(int kernelSize = 3) {
            sobelKernelSize = kernelSize;
        }

        void setOverlay(bool flag){ overlayOnOriginal = flag;}

        virtual void process() override;
    
    private:
        cv::Mat input, output;
        EdgeDetectionAlgorithm algorithm;
        double lowerThreshold, upperThreshold;
        int apertureSize;
        int sobelKernelSize;
        bool overlayOnOriginal;
    };

#endif