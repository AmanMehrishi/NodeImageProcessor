#include <iostream>
#include <filesystem>
#include "ImageInputNode.h"
#include "EdgeDetectionNode.h"
#include "OutputNode.h"
#include "BrightnessContrastNode.h"
#include "NodeGraph.h"

int main() {
    
    NodeGraph graph;

    auto inputNode = make_shared<ImageInputNode>("input_node");
    inputNode->setImagePath("resources/test.jpg");
    graph.addNode(inputNode);
    


    auto bcNode = make_shared<BrightnessContrastNode>("brightness_contrast_node");
    bcNode->setInput(inputNode->getOutput());
    bcNode->setBrightness(30);
    bcNode->setContrast(1.1);
    graph.addNode(bcNode);

    auto edgeNode = make_shared<EdgeDetectionNode>("edge_detection_node");
    edgeNode->setInput(bcNode->getOutput());
    edgeNode->setAlgorithm(EdgeDetectionAlgorithm::CANNY);
    edgeNode->setCannyParameters(50, 150, 3);
    graph.addNode(edgeNode);

    auto outputNode = make_shared<OutputNode>("output_node");
    outputNode->setInput(edgeNode->getOutput());
    outputNode->setOutputPath("output/final_results.jpg");
    graph.addNode(outputNode);

    graph.connect(inputNode, bcNode);

    graph.connect(bcNode, edgeNode);

    graph.connect(edgeNode, outputNode);

    graph.processAll();

    if(!std::filesystem::exists("output")) {
        std::filesystem::create_directory("output");
    }


    cout<<"COMPLETE"<<endl;



    return 0;
}