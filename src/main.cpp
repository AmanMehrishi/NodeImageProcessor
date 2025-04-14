#include <iostream>
#include <filesystem>
#include "ImageInputNode.h"
#include "EdgeDetectionNode.h"
#include "OutputNode.h"



int main() {
    cout<<"Current Working Directory" << filesystem::current_path() <<endl;

    ImageInputNode inputNode("input_node");
    EdgeDetectionNode edgeNode("edge_detection_node");
    OutputNode outputNode("output_node");

    inputNode.setImagePath("D:/MIXAR-PROJECT/resources/test.jpg"); 
    inputNode.process();

    edgeNode.setInput(inputNode.getOutput());

    edgeNode.setAlgorithm(EdgeDetectionAlgorithm::CANNY);
    edgeNode.setCannyParameters(50, 150, 3);

    // edgeNode.setAlgorithm(EdgeDetectionAlgorithm::SOBEL);
    // edgeNode.setSobelParameters(3);

    edgeNode.process();

    outputNode.setInput(edgeNode.getOutput());
    outputNode.setOutputPath("output/edge_detected_image.jpg");
    outputNode.process();

    return 0;
}